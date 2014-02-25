#include <AnaliseImagem/Segmentacao/PorosGargantas/CAberturaDilatacao.h>
#include <AnaliseImagem/Caracterizacao/CObjetoImagem.h> // novo, repreenta objeto da imagem
#include <iomanip>
#include <cmath>

using namespace std;

bool CAberturaDilatacao::salvarResultadosParciais = 0;

CAberturaDilatacao::CAberturaDilatacao() :
	fatorReducaoRaioElemEst (1), raioMaximoElementoEstruturante ( 30000 ), // usar limits
	incrementoRaioElementoEstruturante ( 1 ), pm(0), modelo(2), INDICE(1), FUNDO(0)
//, salvarResultadosParciais(0)
{
	matrizRotulo = new TCRotulador2D<bool>();
}

CAberturaDilatacao::CAberturaDilatacao( TCMatriz2D<bool>* &matriz , std::string _nomeImagem, int _indice, int _fundo)
	: pm(matriz), // pm é ponteiro para imagem externa (se mudar externamente teremos problemas).
		nomeImagem(_nomeImagem),
		fatorReducaoRaioElemEst (1), raioMaximoElementoEstruturante ( 30000 ), // usar limits
		incrementoRaioElementoEstruturante ( 1 ), matrizRotulo (0), modelo(2), INDICE(_indice), FUNDO(_fundo)
	//,salvarResultadosParciais(0)
{
	matrizRotulo = new TCRotulador2D<bool>( pm, INDICE, FUNDO );
}

CAberturaDilatacao::~CAberturaDilatacao()
{
	delete matrizRotulo ;
}

void CAberturaDilatacao::Salvar(CVetor* &vetor, std::string nomeArquivo)
{
	ofstream fout;
	fout.open(nomeArquivo.c_str());
	if (fout.fail()) {
		cout << "Erro ao abrir arquivo ... " << endl;
	}
	for (int i=0; i < vetor->NX();i++) {
		fout << double(vetor->data1D[i]) / 10000.0 << "\n";
	}
	fout.close();
}

void CAberturaDilatacao::Salvar(vector<double> v, std::string nomeArquivo)
{
	ofstream fout;
	fout.open ( nomeArquivo.c_str() );
	if ( fout.fail() ) {
		cout << "Erro ao abrir arquivo ... " << endl;
	}
	for ( int i = 0 ; i < v.size();i++) {
		fout << v[i]  << "\n";
	}
	fout.close();
}

double CAberturaDilatacao::Porosidade( TCMatriz2D<bool> *&pm )
{
	double porosidade = 0.0;
	for ( int i = 0 ;  i < pm->NX() ; i++ )
		for (int j = 0; j < pm->NY(); j++ )
			if ( pm->data2D[i][j] == 1 ) {
				porosidade++;
			}
	return porosidade / ( (double) pm->NX() * pm->NY() );
}

// Observações importantes:
// Observe que a porosidade não deve estar indo para zero (0), porque o filtro abertura desconsidera as bordas.
// para solucionar este e outros problemas com a borda, criar matriz aumentada, em que a borda é uma região de fundo (zeros)
void CAberturaDilatacao::DistTotalPoros()
{
	// Cria cópia da matriz
	TCMatriz2D<bool>* matrizAuxiliar = 0;

	matrizAuxiliar = new TCMatriz2D<bool>( *pm );

	if ( matrizAuxiliar == nullptr ) {
		cerr << "Erro alocação matrizAuxiliar  dentro de void CAberturaDilatacao::DistTotalPoros().";
		return ;
	}

	// Aloca e zera vetor com distribuicaoTotalPoros
	distribuicaoTotalPoros = new CVetor(( matrizAuxiliar->NX()-1)/2+1);
	distribuicaoTotalPoros->Constante(10000); //???
	distribuicaoTotalPoros->data1D[0]=0;

	// Calcula porosidade da imagem
	porosidade = Porosidade(matrizAuxiliar);
	cout << "Porosidade=" << porosidade << endl;

	// Porosidade parcial
	double porosidadeParcial = porosidade;

	// Tamanho 3 do elemento estruturante conforme codigo ANAIMP
	pfmf = new TCFEMorfologiaMatematica<bool>(matrizAuxiliar,3);
	if ( pfmf  == nullptr ) {
		cerr << "Falha alocação: pfmf = new CFEMorfologiaMatematica(matrizAuxiliar,3);";
		return;
	}

	// Calculo das porosidades parciais para cada raio do EE
	double distAcumulada;
	for ( int raioElem = 1; raioElem <= (matrizAuxiliar->NX()-1)/2; raioElem++ ) {
		// Aplica abertura a imagem matrizAuxiliar, usando elemento estruturante de raio raioElem
		pfmf->Abertura(matrizAuxiliar,raioElem);

		// Calcula porosidade parcial
		porosidadeParcial = Porosidade(matrizAuxiliar);

		// conferir no artigo
		distAcumulada = ( porosidade - porosidadeParcial ) / porosidade;

		distribuicaoTotalPoros->data1D[ raioElem ] = distAcumulada  * 10000;

		// Mostra resultado na tela
		cout << "raioElem = " << setw(4) << raioElem
				 << " distribuicao acumulada = " << distAcumulada << endl;
		// verificar
		if ( porosidadeParcial <= 0.0 || distAcumulada >= 100.0 )
			break;//raioElem = ( matrizAuxiliar->NX() - 1 ) / 2 + 1; // encerra o for

		// Condição de parada
		// necessário senão o calculo fica muito demorado.
		//if( raioElem > raioMaximoElementoEstruturante )
		//	break;
	}
	/* 		desnecessario, pois esta usando para calculo a matriz auxiliar
		for (int j=0; j < matrizAuxiliar->NY();j++)
			for (int i=0; i < matrizAuxiliar->NX();i++)
				pm->data2D[i][j] = copiaMatriz.data2D[i][j];
		*/
	Salvar( distribuicaoTotalPoros , "distPoros.dtp" );

	delete pfmf; // novo, criou com new tem de deletar com delete
}

// Este modelo foi copiado? do Anaimp.
// Apenas acrescentei comentários e saída dos resultados (para análise visual).

// Neste modelo a área dos objetos rotulados é usada para diferenciar
// pixeis isolados que foram eliminados na abertura dos pixeis das ligacoes.

// Os resultados são bons apenas para poros muito bem comportados
void CAberturaDilatacao::DistSitiosLigacoes_Modelo_0() {
	modelo = 0 ;

	// Novo, para salvar resultado final em disco cria MatrizSitiosLigacoes
	TCMatriz2D<int>* MatrizSitiosLigacoes = new TCMatriz2D<int>( pm->NX(), pm->NY()  ); // cópia da matriz inicial
	MatrizSitiosLigacoes->Constante( 0 );
	MatrizSitiosLigacoes->SetFormato( P2_X_Y_GRAY_ASCII );
	MatrizSitiosLigacoes->NumCores ( 3 ); // 3 cores, fundo = 0,  ligacoes = 1, sitios = 2

	// 	Copia da matrizInicial
	TCMatriz2D<bool>* MatrizInicial = new TCMatriz2D<bool>( *pm  ); // cópia da matriz inicial

	// Variaveis auxiliares
	ostringstream os;
	// area das ligacoes, usada para
	double areaLigacoes;


	// porosidade da matriz das ligacoes num dado instante
	double porosidadeParcialdasLigacoes = 0.0;

	cout << "Alocando imagens auxiliares..." << endl ;

	// Representa a matriz pm no passo anterior (antes da abertura-dilatacao)
	TCMatriz2D<bool> matrizInstanteAnterior( pm->NX(), pm->NY() );

	// TCMatriz2D< int > matrizAbertura(pm->NX(),pm->NY
	// troquei pela linha abaixo para poder resolver o problema de referencia no metodo funcaoPorosidade
	TCMatriz2D<bool>* matrizAbertura = new TCMatriz2D<bool>( pm->NX(), pm->NY() );

	cout << "Alocando vetores distribuicao..." << endl ;
	distribuicaoTotalPoros = new CVetor( (pm->NX()-1)/2+1 ); // dimensao igual a do raio que zera a imagem abertura + 1
	distribuicaoLigacoes =   new CVetor( (pm->NX()-1)/2+1 );
	distribuicaoSitios =     new CVetor( (pm->NX()-1)/2+1 );

	// Seta com valores acumulados máximos
	distribuicaoTotalPoros->Constante(100);
	distribuicaoLigacoes->Constante(100);
	distribuicaoSitios->Constante(100);

	// Seta valor inicial como sendo 0
	distribuicaoTotalPoros->data1D[0] = 0;
	distribuicaoLigacoes->data1D[0]   = 0;
	distribuicaoSitios->data1D[0]     = 0;

	// Calcula porosidade
	cout << "Calculando porosidade da imagem inicial..." << endl ;
	porosidade = Porosidade( pm );
	cout << " = " << porosidade << endl ;

	// Cria vetor para area dos objetos mapeados
	// usado para diferenciar ???
	CVetor areaObjetosMapeados( pm->NX() * pm->NY() / 4 );

	// Cria filtro de morfologia matemática
	cout << "Criando filtro CFEMorfologiaMatematica..." << endl ;
	pfmf = new TCFEMorfologiaMatematica<bool>( pm, 3, INDICE, FUNDO ); // mover para construtor, deletar no destrutor

	// auxiliar, usada para encerrar looping ??
	double porosidadeAposAbertura=	0.0;

	// LOOOPING de calculo
	cout << "Entrando no looping de calculo das distribuicoes..." << endl ;
	for ( int raioElemen = 1; raioElemen <= ( pm->NX() - 1 ) / 2; raioElemen += incrementoRaioElementoEstruturante ) {
		cout << "================================>RAIO Elemento Estruturante = " <<  raioElemen << endl ;

		// COPIA VALORES de pm PARA MATRIZ no instante anterior
		cout << "-->Inicializando matrizInstanteAnterior..." << endl ;
		for (  int i = 0; i < pm->NX(); i++)
			for (  int j = 0; j < pm->NY(); j++)
				matrizInstanteAnterior.data2D[i][j] = pm->data2D[i][j];

		// ROTULAGEM
		cout << "-->Realizando rotulagem sequencial..." << endl ;
		matrizRotulo->Go( pm ); 				// rotula matrizRotulo
		// salvar a matriz rotulo ?

		// ABERTURA
		cout << "-->Processando Abertura..." << endl ;
		pfmf->Abertura( pm , raioElemen );

		// Salva matriz abertura
		os.str("");
		os << "MatrizAbertura_" << raioElemen << ".pbm";
		cout << "-->Salvando imagem " << os.str().c_str() << endl ;
		pm->Write( os.str() );

		porosidadeAposAbertura = Porosidade ( pm );

		// Seta matriz abertura e calcula area dos objetos
		cout << "-->Calculando area dos objetos da matriz abertura..." << endl ;
		areaObjetosMapeados.Constante(0);
		for (  int i = 0; i < pm->NX(); i++) {
			for (  int j = 0; j < pm->NY(); j++) {
				// Atualiza matrizAbertura // mudar realizar abertura na matriz abertura.
				matrizAbertura->data2D[i][j] = pm->data2D[i][j];
				if ( matrizAbertura->data2D[i][j] > 0)  // se o pixel esta na matriz abertura, pegue seu rotulo
					areaObjetosMapeados.data1D[ matrizRotulo->data2D[i][j] ] ++; // e acumule a area deste rotulo
			}
		}

		// DILATACAO
		cout << "-->Processando dilatação..." << endl ;
		pfmf->Dilatacao( pm, raioElemen / fatorReducaoRaioElemEst );

		// Salva matriz dilatacao
		os.str("");
		os << "MatrizAberturaDilatacao_" << raioElemen << ".pbm";
		cout << "-->Salvando imagem " << os.str().c_str() << endl ;
		pm->Write( os.str() );

		// Critério de parada
		if ( porosidadeAposAbertura == 0  ) {
			cout << "Criterio de parada: porosidadeAposAbertura == 0" << endl;
			break;
		} else if ( raioElemen > raioMaximoElementoEstruturante ) { // bug para imagens com objetos muito grandes
			cout << "Criterio de parada: raioElemen >  raioMaximoElementoEstruturante " << endl;
			break;
		}

		// Area ligaçoes
		areaLigacoes = 0;
		// Percorre a matriz AberturaDilatacao e determina ligacoes
		for ( int  i = 0 ; i < pm->NX() ; i++ ) {
			for (  int j = 0 ; j < pm->NY() ; j++ ) {
				// Se o pixel existe na matrizInstanteAnterior,
				if ( ( matrizInstanteAnterior.data2D[i][j] > 0 )
						 // e nao existe na matriz aberturaDilatacao pm
						 && ( pm->data2D[i][j] == 0 )
						 // e area do objeto > 0; entao e pixel de ligaçao (não é poro eliminado)
						 && ( areaObjetosMapeados.data1D[ matrizRotulo->data2D[i][j] ] > 0) )
				{
					areaLigacoes++;   		// acumula cada pixel de ligação
					// se for ligação anota na MatrizSitiosLigacoes (em tons de cinza, 0=fundo, 1=ligacao, 2=sitio)
					MatrizSitiosLigacoes->data2D[i][j] = 1;       // o armazenamento dos pontos das ligações só serve para visualização
				}
			}
		}

		// Salva matriz ligacoes em disco
		os.str("");
		os << "MatrizLigacoes_" << raioElemen << ".pgm";
		cout << "-->Salvando imagem " << os.str().c_str() << endl ;
		MatrizSitiosLigacoes->Write( os.str() );

		// DISTRIBUIÇOES
		cout << "-->Calculando distribuicoes acumuladas..." << endl ;
		// Porosidade eliminada / porosidade, dá a distribuicao acumulada pois a porosidade eliminada é acumulada
		distribuicaoTotalPoros->data1D[ raioElemen ] = ( ( porosidade - Porosidade ( matrizAbertura ) ) / porosidade ) * 100;
		// calcula porosidade das ligacoes
		porosidadeParcialdasLigacoes = areaLigacoes / ( pm->NX() * pm->NY() );
		// porosidade ligacoes neste passo mais porosidade ligacoes passo anterior
		distribuicaoLigacoes->data1D[ raioElemen ] =  porosidadeParcialdasLigacoes / porosidade * 100
																									+ distribuicaoLigacoes->data1D[ raioElemen - 1 ];
		// esta na total e não esta na ligacao, então é sitio
		distribuicaoSitios->data1D[ raioElemen ] = distribuicaoTotalPoros->data1D[ raioElemen ] - distribuicaoLigacoes->data1D[ raioElemen ];

		// pm PASSA A SER MATRIZ ABERTURA
		// garante que a próxima abertura parte da atual
		for ( int  i = 0; i < pm->NX(); i++) {
			for ( int  j = 0; j < pm->NY(); j++) {
				pm->data2D[i][j] = matrizAbertura->data2D[i][j];
			}
		}
	}
	cout << "-->Encerrado looping." << endl ;

	cout << "Salvando distribuicoes..." << endl ;
	Salvar(distribuicaoLigacoes,"distLigacoes.dl");
	cout << "Salvou distLigacoes.dl..." << endl ;

	Salvar(distribuicaoSitios,"distSitios.dp");
	cout << "Salvou distSitios.dp..." << endl ;

	Salvar(distribuicaoTotalPoros,"distTotalPoros.dtp");
	cout << "Salvou distTotalPoros.dtp..." << endl ;

	//    matrizRotulo->SetFormato(  WRITEFORM_PI_X_Y_GRAY_ASCII );
	//    matrizRotulo->NumCores ( matrizRotulo->NumeroObjetos() );
	//    matrizRotulo->Write("MatrizRotulo.pgm");

	// Gerando imagem final e salvando em disco
	// Apenas para visualizacao do resultado final
	// Se esta na matriRotulo e nao foi marcado como ligacao, entao é sitio
	for ( int i = 0; i < pm->NX(); i++) {
		for ( int j = 0; j < pm->NY(); j++) {
			// É poro (esta na MatrizInicial) e não é  ligação, então é  sítio
			if ( MatrizInicial ->data2D[i][j] > 0                     // é poro
					 and MatrizSitiosLigacoes->data2D[i][j] != 1 )       // não é ligacao
				MatrizSitiosLigacoes->data2D[i][j] = 2; // então é sitio.
		}
	}

	// Salva MatrizSitiosLigacoes em disco
	// ## Modelo NomeImagem raioMaximoElementoEstruturante fatorReducaoRaioElemEst
	// incrementoRaioElementoEstruturante 20(finalizaPrograma)
	os.str("");
	os << "MatrizSitiosLigacoes"
		 << "-Modelo-" 	<< modelo
		 << "-rmee-" 	<< raioMaximoElementoEstruturante
		 << "-frree-"	<< fatorReducaoRaioElemEst
		 << "-iree-"	<< incrementoRaioElementoEstruturante
		 << "-"	<< nomeImagem
		 << ".pgm";

	cout << "-->Salvando imagem " << os.str().c_str() << endl ;
	MatrizSitiosLigacoes->Write( os.str() );

	delete pfmf; // novo, criou com new tem de deletar com delete
	delete matrizAbertura;
	delete MatrizSitiosLigacoes; // novo
}


// O primeiro modelo é o mais simples
// Realiza abertura, realiza dilatação, e marca como ligação o que esta na imagem pm e não esta na imagem dilatada.
// Depois, determina aŕea de sitios e ligações.
// Este modelo desconsidera fato de que poros pequenos, isolados, serão contabilizados como ligações.
// Não usa rotulagem.
// A diferença para DistSitiosLigacoes_Modelo_1_old_usaCVetor é que usa distribuições em vector<double>
void CAberturaDilatacao::DistSitiosLigacoes_Modelo_1() {
	modelo = 1 ;
	// Cria MPoros e deixa vazia
	cout << "Criando e inicializando MPoros..." << endl;
	TCMatriz2D<bool>* MPoros = new TCMatriz2D<bool>(pm->NX(), pm->NY());
	MPoros->Constante(0);
	MPoros->SetFormato( P1_X_Y_ASCII );
	//MPoros->Write("MPoros_inicial.pbm");

	// Cria MSitios cópia da matriz pm inicial (durante o processo vai apagar)
	cout << "Criando e inicializando MSitios..." << endl;
	TCMatriz2D<bool>* MSitios  = new TCMatriz2D<bool>( *pm );
	MSitios->SetFormato( P1_X_Y_ASCII );
	//MSitios->Write("MSitios_inicial.pbm");

	// Cria MLigacoes e deixa vazia
	cout << "Criando e inicializando MLigacoes..." << endl;
	TCMatriz2D<bool>* MLigacoes   = new TCMatriz2D<bool>(pm->NX(), pm->NY());
	MLigacoes->Constante(0);
	MLigacoes->SetFormato( P1_X_Y_ASCII );
	//MLigacoes->Write("MLigacoes_inicial.pbm");

	// Cria MInicialRotulada, é a imagem inicial rotulada
	cout << "Criando e inicializando MInicialRotulada - tons de cinza..." << endl;
	// Rotula matrizRotulo usando pm
	matrizRotulo->Go( pm );

	TCMatriz2D<int>* MInicialRotulada = new TCMatriz2D<int>( *matrizRotulo );
	MInicialRotulada->SetFormato( P2_X_Y_GRAY_ASCII );
	MInicialRotulada->NumCores ( matrizRotulo->NumeroObjetos() ); // 256, numero objetos informa o maior rotulo utilizado.
	MInicialRotulada->Write("MatrizInicialRotulada.pgm");

	// Cria MAbertura, é a imagem apos abertura
	cout << "Criando e inicializando MAbertura..." << endl;
	TCMatriz2D<bool>* MAbertura= new TCMatriz2D<bool>(pm->NX(), pm->NY());

	// Cria vetor VPoros
	cout << "Criando e inicializando VPoros..." << endl;
	vector<double> VPoros ( ((pm->NX()-1)/2+1) , 0.0); // inicialia com zero

	// Cria vetor VSitios
	cout << "Criando e inicializando VSitios..." << endl;
	vector<double> VSitios ( ((pm->NX()-1)/2+1) , 0.0); // inicialia com zero

	// Cria vetor VLigacoes
	cout << "Criando e inicializando VLigacoes..." << endl;
	vector<double> VLigacoes ( ((pm->NX()-1)/2+1) , 0.0); // inicialia com zero

	// Cria filtro para operacoes de abertura e dilatacao
	cout << "Criando filtro CFEMorfologiaMatematica..." << endl ;
	pfmf = new TCFEMorfologiaMatematica<bool>(pm, 3, INDICE, FUNDO );

	// Variaveis auxiliares

	// usada para setar nome dos arquivos de disco
	ostringstream os;

	// Entra num looping para o raio do elemento estruturante
	cout << "Entrando no looping de calculo das distribuicoes..." << endl ;
	for ( int raioElemen = 1; raioElemen <= (pm->NX()-1)/2; raioElemen += incrementoRaioElementoEstruturante  )	{
		cout << "==>RAIO Elemento Estruturante = " <<  raioElemen << endl ;

		// ABERTURA
		cout << "-->Processando Abertura..." << endl ;
		pfmf->Abertura(pm,raioElemen);

		// Calcula porosidade apos abertura, vai ser usada no final como criterio de parada.
		porosidade = Porosidade ( pm );
		cout << "-->Porosidade = " << porosidade << endl;

		// Criterio de parada; temporario, melhorar o criterio de saida
		// note que como nao processa a borda sempre vai ter porosidade, ou seja, acrescente uma borda na imagem.
		if ( porosidade == 0 or raioElemen >= raioMaximoElementoEstruturante ) // bug nao processa raios maiores, usar raioMaximoAbertura
			break;

		os.str("");
		os << "MatrizAbertura_" << raioElemen << ".pbm";
		cout << "-->Salvando imagem " << os.str().c_str() << endl ;
		pm->Write(os.str());

		// Copia o que sobrou em pm para MAbertura (matriz abertura)
		cout << "-->Setando MAbertura..." << endl ;
		for ( int i = 0; i < pm->NX(); i++)
			for ( int j = 0; j < pm->NY(); j++)
				if ( pm->data2D[i][j] >=1 )
					MAbertura->data2D[i][j] = 1;
				else
					MAbertura->data2D[i][j] = 0;

		// Copia o que foi eliminado na abertura para matriz de poros
		cout << "-->Setando MPoros..." << endl ;
		for ( int i = 0; i < pm->NX(); i++)
			for ( int j = 0; j < pm->NY(); j++)
				// Se esta na imagem original e não esta apos abertura, então é pixel/poro eliminado
				if ( MInicialRotulada->data2D[i][j] >=1 and pm->data2D[i][j] == 0 )
					MPoros->data2D[i][j] = 1;

		// DILATACAO
		cout << "-->Processando dilatação..." << endl ;
		pfmf->Dilatacao( pm, raioElemen / fatorReducaoRaioElemEst);

		os.str("");
		os << "MatrizAberturaDilatacao_" << raioElemen << ".pbm";
		cout << "-->Salvando imagem " << os.str().c_str() << endl ;
		pm->Write(os.str());

		// Zera sitios e ligacoes, necessario, pois para diferentes aberturas, teremos diferentes áreas das ligacoes
		//MSitios->Constante(0);
		MLigacoes->Constante(0);

		// Seta pixeis de MSitios e MLigacoes
		cout << "-->Setando MSitios e MLigacoes..." << endl ;
		for ( int i = 0; i < pm->NX(); i++)
			for ( int j = 0; j < pm->NY(); j++) {
				// Se esta em MPoros e não esta em pm, entao é ligacao
				if ( MPoros->data2D[i][j] >= 1 and pm->data2D[i][j] == 0 ) {
					MLigacoes->data2D[i][j] = 1;
					MSitios->data2D[i][j] = 0; // apaga na imagem dos sitios
				}
				// Se esta em MPoros e nao é ligacao, entao é sitio
				//else if ( MPoros->data2D[i][j] >= 1 )
				//	MSitios->data2D[i][j] = 1;
			}

		// 	Calcula o numero de pixeis de cada matriz
		cout << "-->Calculando o numero de pixeis de cada matriz..." << endl ;
		for ( int i = 0; i < pm->NX(); i++) {
			for ( int j = 0; j < pm->NY(); j++) {
				// 	Calcula VSitios[raioee]
				if ( MSitios->data2D[i][j] >= 1)
					VSitios[ raioElemen ]++;
				// 	Calcula VLigacoes[raioee]
				if ( MLigacoes->data2D[i][j] >= 1)
					VLigacoes[ raioElemen ]++;
				// 	Calcula VPoros[raioee]
				// O numero de sitios de VPoros pode ser calculado por
				// VPoros[raioElemen] = VSitios[raioElemen] + VLigacoes[raioElemen];
				// ou por
				if ( MPoros->data2D[i][j] >= 1)
					VPoros[ raioElemen ]++;
			}
		}

		os.str("");
		os << "MatrizPoros_"  << raioElemen << ".pbm";
		cout << "-->Salvando imagem " << os.str().c_str() << endl ;
		MPoros->Write(os.str());

		os.str("");
		os  << "MatrizSitios_" << raioElemen << ".pbm";
		cout << "-->Salvando imagem " << os.str().c_str() << endl ;
		MSitios->Write(os.str());

		os.str("");
		os << "MatrizLigacoes_" << raioElemen << ".pbm";
		cout << "-->Salvando imagem " << os.str().c_str() << endl ;
		MLigacoes->Write(os.str());

		// Como fez dilatacao extra, precisa retornar a imagem pm a original
		// Como fez dilatacao extra, precisa retornar a imagem pm a MAbertura
		cout << "-->Como fez dilatacao extra, precisa retornar a imagem pm a original..." << endl ;
		for ( int i = 0; i < pm->NX(); i++) {
			for ( int j = 0; j < pm->NY(); j++) {
				//if ( MInicialRotulada->data2D[i][j] >= 1 )
				if ( MAbertura->data2D[i][j] >= 1 ) {
					pm->data2D[i][j] = 1;
				} else {
					pm->data2D[i][j] = 0;
				}
			}
		}
	}// fim do looping

	// Salva Vetor VPoros
	Salvar( VPoros , "distribuicaoPoros.dtp" );

	// Salva Vetor VSitios
	Salvar( VSitios, "distribuicaoSitios.dtp" );

	// Salva Vetor VLigacoes*/
	Salvar( VLigacoes, "distribuicaoLigacoes.dtp" );

	// Apenas para visualizacao do resultado final
	for ( int i = 0; i < pm->NX(); i++) {
		for ( int j = 0; j < pm->NY(); j++) {
			if ( MLigacoes->data2D[i][j] >= 1 )
				MInicialRotulada->data2D[i][j] = 125;
			if ( MSitios->data2D[i][j] >= 1 )
				MInicialRotulada->data2D[i][j] = 255;
		}
	}
	// Salva MatrizSitiosLigacoes em disco
	// ## Modelo NomeImagem raioMaximoElementoEstruturante fatorReducaoRaioElemEst
	// incrementoRaioElementoEstruturante 20(finalizaPrograma)
	os.str("");
	os << "MatrizSitiosLigacoes"
		 << "-Modelo-" 	<< modelo
		 << "-rmee-" 	<< raioMaximoElementoEstruturante
		 << "-frree-"	<< fatorReducaoRaioElemEst
		 << "-iree-"	<< incrementoRaioElementoEstruturante
		 << "-" << nomeImagem
		 << ".pgm";

	cout << "-->Salvando imagem " << os.str().c_str() << endl ;
	MInicialRotulada->Write( os.str() );

	delete pfmf; // novo, criou com new tem de deletar com delete
	delete MPoros;
	delete MSitios;
	delete MLigacoes;
	delete MInicialRotulada;
	delete MAbertura;
	return;
}

/// Modelo que usa abertura, e matrizes rotuladas para identificar as ligações.
// Método totalmente novo, desenvolvido em 2010.
void CAberturaDilatacao::DistSitiosLigacoes_Modelo_2() {
	modelo = 2 ;
	//	int numeroObjetosAcumulados = 0 ;
	int nObjetosAntesAbertura = 0 ;			// numeroObjetosAntesAbertura
	int nObjetosDepoisAbertura = 0 ;		// nObjetosDepoisAbertura
	int nObjetosDepoisAberturaComplementar = 0 ;	// nObjetosDepoisAberturaComplementar

	// Cria MSitiosLigacoes e deixa vazia (apenas para visualizacao das ligacoes - eliminar depois)
	cout << "Criando e inicializando MLigacoes..." << endl;
	TCMatriz2D<int>* MSitiosLigacoes = new TCMatriz2D<int>( pm->NX(), pm->NY() );
	MSitiosLigacoes->Constante(0);
	MSitiosLigacoes->SetFormato( P2_X_Y_GRAY_ASCII );
	MSitiosLigacoes->NumCores ( 6 ); // 256, numero objetos informa o maior rotulo utilizado.

	// Cria MInicialRotulada, é a imagem inicial rotulada
	// a cada etapa/raio/passo, é onde são armazenados os rótudos dos objetos identificados
	cout << "Rotulando a imagem inicial..." << endl;
	matrizRotulo->Go( pm );
	nObjetosAntesAbertura = matrizRotulo->NumeroObjetos();
	cout << "-->nObjetosAntesAbertura =..." 	<< nObjetosAntesAbertura << endl ;

	cout << "Criando e inicializando MInicialRotulada - tons de cinza..." << endl;
	TCMatriz2D<int>* MInicialRotulada = new TCMatriz2D<int>( *matrizRotulo );
	MInicialRotulada->SetFormato( P2_X_Y_GRAY_ASCII );
	MInicialRotulada->NumCores ( matrizRotulo->NumeroObjetos() ); // 256, numero objetos informa o maior rotulo utilizado.

	if ( salvarResultadosParciais ) {
		MInicialRotulada->Write("MatrizInicialRotulada.pgm");
	}

	//	Cria vetor de objetos
	//	No início todos os objetos são marcados como fundo (verificar se esta ok)
	vector< CObjetoImagem > Objeto ( matrizRotulo->NumeroObjetos() , CObjetoImagem( SOLIDO , 0 )); 	// inclui o objeto fundo (0)
	Objeto.reserve ( 100 *  matrizRotulo->NumeroObjetos() ); 		// evita realocacoes

	/*
		// Cria vetor VPoros
			cout << "Criando e inicializando VPoros..." << endl;
			vector<double> VPoros ( ((pm->NX()-1)/2+1) , 0.0); // inicializa com zero

		// Cria vetor VSitios
			cout << "Criando e inicializando VSitios..." << endl;
			vector<double> VSitios ( ((pm->NX()-1)/2+1) , 0.0); // inicializa com zero

		// Cria vetor VLigacoes
			cout << "Criando e inicializando VLigacoes..." << endl;
			vector<double> VLigacoes ( ((pm->NX()-1)/2+1) , 0.0); // inicializa com zero
		*/

	// Cria filtro para operacoes de abertura e dilatacao
	cout << "Criando filtro CFEMorfologiaMatematica..." << endl ;
	pfmf = new TCFEMorfologiaMatematica<bool>( pm, 3, INDICE, FUNDO );

	// Variaveis auxiliares
	// usada para setar nome dos arquivos de disco
	ostringstream os;

	// Seta o Rotulo e o Tipo dos objetos da imagem inicial
	// Seta o fundo
	//Objeto[0].Rotulo( 0 );
	Objeto[0].Tipo( SOLIDO );
	// Seta os demais objetos como sendo PORO
	for ( int i = 1; i < Objeto.size(); i++ ) {
		//Objeto[i].Rotulo( i );
		Objeto[i].Tipo( PORO );
	}

	// Como no looping iremos setar nObjetosAntesAbertura  como o  nObjetosDepoisAberturaComplementar ,
	// aqui, devemos setar nObjetosDepoisAberturaComplementar  como sendo nObjetosAntesAbertura
	nObjetosDepoisAberturaComplementar = nObjetosAntesAbertura ;

	// Entra num looping para o raio do elemento estruturante
	// o incremento é dado pelo incrementoRaioElementoEstruturante
	cout << "Entrando no looping de calculo das distribuicoes..." << endl ;
	for ( int raioElemen = 1; raioElemen <= (pm->NX()-1)/2; raioElemen += incrementoRaioElementoEstruturante  ) {
		cout << "==>RAIO Elemento Estruturante = " <<  raioElemen << endl ;
		// 	cout << "Pressione enter para iniciar processamento deste raio." << endl;
		// 	cin.get();

		// ABERTURA
		// Armazena o numero de objetos antes da abertura (i.e. da rotulagem anterior)
		nObjetosAntesAbertura = nObjetosDepoisAberturaComplementar;
		cout << "-->nObjetosAntesAbertura =..." 	<< nObjetosAntesAbertura << endl ;

		cout << "-->Processando Abertura..." << endl ;
		pfmf->Abertura( pm , raioElemen );

		// Calcula porosidade apos abertura, vai usar como criterio de parada.
		porosidade = Porosidade ( pm );
		cout << "-->Porosidade = " << porosidade << endl;

		// Criterio de parada; temporario, melhorar o criterio de parada/saida.
		// note que como nao processa a borda sempre vai ter porosidade na borda, ou seja, acrescente uma borda na imagem.
		// ou modifique o critério de saída.
		if ( porosidade == 0 or raioElemen >= raioMaximoElementoEstruturante )
			break;

		// Toda saida, como a saida abaixo só deve ser executada se variavel controle for true, bool salvarResultadosParciais = true;
		// Rafael criar funcao para SalvarImagemParcial(ponteiro_para_imagem, NomeImagem);
		if ( salvarResultadosParciais ) {
			os.str("");
			os << "MatrizAbertura_" << raioElemen << ".pbm";
			cout << "-->Salvando imagem " << os.str().c_str() << endl ;
			pm->Write(os.str());
		}

		// Rotula a imagem abertura
		cout << "-->Rotulando matriz pm apos abertura " << endl;
		matrizRotulo->Go( pm ); 		// matrizRotulo
		// Marca o numero de objetos depois da abertura
		nObjetosDepoisAbertura = nObjetosAntesAbertura + matrizRotulo->NumeroObjetos();
		cout << "-->nObjetosDepoisAbertura =..." 	<< nObjetosDepoisAbertura << endl ;

		if ( salvarResultadosParciais ) {
			matrizRotulo->SetFormato( P2_X_Y_GRAY_ASCII );
			matrizRotulo->NumCores ( matrizRotulo->NumeroObjetos() ); // 256, numero objetos informa o maior rotulo utilizado.
			os.str("");
			os << "MatrizRotuloAposAbertura_" << raioElemen << ".pbm";
			cout << "-->Salvando imagem " << os.str().c_str() << endl ;
			matrizRotulo->Write(os.str());
		}


		// Adiciona objetos rotulados a lista de objetos -  o que ficou na imagem abertura são sitios.
		for ( int i = nObjetosAntesAbertura ; i < nObjetosDepoisAbertura; i++ ) {
			Objeto.push_back( CObjetoImagem( SITIO , i ) ) ;
		}

		// Aqui poderia conectar os POROS aos sítios. Pois como todos os POROS virarão ou SITIOS ou RAMOS_MORTOS ou LIGACOES,
		// não teremos na imagem nenhuma informação sobre os rótulos dos POROS!
		// e esta informação não esta sendo armazenada nos objetos (os primeiros objetos ficam vazios).

		// Copia imagem abertura rotulada para matriz inicial rotulada,
		// assim, a matriz inicial rotulada já terá a informação do rótulo dos sítios.
		// A seguir, inverte pm (pm passa a ser o complemento da abertura)
		for ( int i = 0; i < pm->NX(); i++) {
			for ( int j = 0; j < pm->NY(); j++) {
				// Se faz parte abertura; então copia para matriz inicial rotulada (copia rotulo dos sitios)
				// MInicialRotulada terá os rótulos iniciais e os rotulos da imagem abertura acrescidos do número de objetos antes da abertura
				// Isso precisa se feito pois o rotulador não mais permite informar o rótulo inicial.
				if ( pm->data2D[i][j] > 0 )
					MInicialRotulada->data2D[i][j] = matrizRotulo->data2D[i][j] + nObjetosAntesAbertura;

				// Se é Poro, ou seja, MInicialRotulada->data2D[i][j] > 0
				// entao marca como abertura complementar (inverte valores de pm).
				if ( MInicialRotulada->data2D[i][j] > 0 ) {
					if ( pm->data2D[i][j] == 0 ) {
						pm->data2D[i][j] = 1;
					} else {
						pm->data2D[i][j] = 0;
					}
				}
			}
		}
		if ( salvarResultadosParciais ) {
			os.str("");
			os << "MatrizAberturaComplementar_" << raioElemen << ".pbm";
			cout << "-->Salvando imagem " << os.str().c_str() << endl ;
			pm->SetFormato( P1_X_Y_ASCII );
			pm->Write(os.str());
		}

		// Rotulagem da matriz abertura complementar
		cout << "-->Processando rotulagem da matriz abertura complementar..." << endl ;
		matrizRotulo->Go( pm );
		nObjetosDepoisAberturaComplementar = nObjetosDepoisAbertura + matrizRotulo->NumeroObjetos();
		cout << "-->nObjetosDepoisAberturaComplementar =..."<< nObjetosDepoisAberturaComplementar << endl ;

		if ( salvarResultadosParciais )	{
			os.str("");
			os << "MatrizAberturaComplementarRotulada_" << raioElemen << ".pgm";
			cout << "-->Salvando imagem " << os.str().c_str() << endl ;
			matrizRotulo->NumCores ( matrizRotulo->NumeroObjetos() );
			matrizRotulo->SetFormato( P2_X_Y_GRAY_ASCII );
			matrizRotulo->Write(os.str());
		}

		// Adiciona objetos rotulados a lista de objetos -  são RAMOs_MORTOs
		// depois iremos identificar quais ramos_mortos são ligações
		for ( int i = nObjetosDepoisAbertura ; i < nObjetosDepoisAberturaComplementar; i++ ) {
			Objeto.push_back( CObjetoImagem(  RAMO_MORTO , i ) ) ;
		}

		// Copia imagem abertura complementar rotulada para matriz inicial rotulada.
		// Para cada rótulo da imagem abertura complementar acrescenta o
		// nObjetosDepoisAbertura de forma que os rotulos sejam sequenciais.
		for ( int i = 0; i < pm->NX(); i++) {
			for ( int j = 0; j < pm->NY(); j++) {
				// Se não é fundo, copia para matriz inicial rotulada
				if ( pm->data2D[i][j] > 0 ) {
					MInicialRotulada->data2D[i][j] = matrizRotulo->data2D[i][j] + nObjetosDepoisAbertura;
				}
			}
		}

		// Percorre a imagem MInicialRotulada e faz as conecções.
		// Identifica os rótulos dos vizinhos e faz a conecção se o rotulo já não tiver sido incluido
		// lembre-se que set não tem repeticao, e sConexao é do tipo set<int>.
		// Desconsideramos os pixeis da borda para evitar bugs.
		// Aqui a MInicialRotulada tem SOLIDO, POROs, SITIOs e RAMOs_MORTOs
		int dimensaoBorda = 1;
		int rotuloPixel_i_j;  // auxiliar
		for ( int i = dimensaoBorda; i < pm->NX() - dimensaoBorda; i++) {
			for ( int j = dimensaoBorda; j < pm->NY() - dimensaoBorda; j++) {
				rotuloPixel_i_j	 =  MInicialRotulada->data2D[i][j];
				// Só devemos considerar os rotulos da imagem abertura, i.e,  rotulo >= nObjetosAntesAbertura
				if (  rotuloPixel_i_j	 >=   nObjetosAntesAbertura ) {
					// Se os rotulos são diferentes e fazem parte da matriz abertura
					// (rotulo >= nObjetosAntesAbertura) então marca a coneccão.
					if ( rotuloPixel_i_j != MInicialRotulada->data2D[i][j-1]
							 and MInicialRotulada->data2D[i][j-1]  >= nObjetosAntesAbertura)
						// Como a lista de coneccões é do tipo set, só marca uma vez.
						Objeto[rotuloPixel_i_j].Conectar( MInicialRotulada->data2D[i][j-1] );

					if ( rotuloPixel_i_j != MInicialRotulada->data2D[i][j+1]
							 and MInicialRotulada->data2D[i][j+1]  >= nObjetosAntesAbertura)
						Objeto[rotuloPixel_i_j].Conectar( MInicialRotulada->data2D[i][j+1] );

					if ( rotuloPixel_i_j != MInicialRotulada->data2D[i-1][j]
							 and MInicialRotulada->data2D[i-1][j]  >= nObjetosAntesAbertura)
						Objeto[rotuloPixel_i_j].Conectar( MInicialRotulada->data2D[i-1][j] );

					if ( rotuloPixel_i_j != MInicialRotulada->data2D[i+1][j]
							 and MInicialRotulada->data2D[i+1][j]  >= nObjetosAntesAbertura)
						Objeto[rotuloPixel_i_j].Conectar( MInicialRotulada->data2D[i+1][j] );
				}
			}
		}
		// Agora vamos percorrer os objetos anotados como RAMOs_MORTOs e identificar as ligações
		// Percorre apenas os rotulos marcados como RAMOs_MORTOs.
		for ( int i = nObjetosDepoisAbertura ; i < nObjetosDepoisAberturaComplementar; i++ )
			// Se o numero de conecções for maior que 1, então é ligação
			if ( Objeto[i].SConexao().size() > 1 )
				Objeto[i].Tipo( LIGACAO );

		// Agora, todos o objetos foram anotados, temos SOLIDO, POROs, SITIOs, RAMOs_MORTOs e LIGACOES.
		// vamos armazenar resultado na MSitiosLigacoes e salvar resultado parcial em disco.
		for ( int i = 0; i < pm->NX(); i++) {
			for ( int j = 0; j < pm->NY(); j++) {
				// pm armazena a abertura complementar, tem RAMOs_MORTOs e LIGACOES
				// Sé é ramo ou ligação
				if ( pm->data2D[i][j] > 0 ) {
					// Se o numero de coneccoes >1, então é ligação
					if ( Objeto[ MInicialRotulada->data2D[i][j] ].Tipo() == LIGACAO )
						MSitiosLigacoes->data2D[i][j] = LIGACAO;
				}
			}
		}

		// Salva MSitiosLigacoes em disco
		// ## Modelo NomeImagem raioMaximoElementoEstruturante fatorReducaoRaioElemEst
		// incrementoRaioElementoEstruturante 20(finalizaPrograma)
		// 	if ( salvarResultadosParciais )
		// 		{
		// 		os.str("");
		// 		os << nomeImagem << "-MSitiosLigacoes"
		// 				<< "-Modelo-" 	<< modelo
		// 				<< "-rmee-" 	<< raioMaximoElementoEstruturante
		// 				<< "-frree-"	<< fatorReducaoRaioElemEst
		// 				<< "-iree-"	<< incrementoRaioElementoEstruturante
		// 				<< "-ree-" 	<< raioElemen
		// 				<< ".pgm";
		//
		// 		cout << "-->Salvando imagem " << os.str().c_str() << endl ;
		// 		MSitiosLigacoes->Write( os.str() );
		// 		}

		// Precisamos fazer com que pm volte a ser a matriz abertura
		for ( int i = 0; i < pm->NX(); i++) {
			for ( int j = 0; j < pm->NY(); j++) {
				int rot = MInicialRotulada->data2D[i][j];
				// Se maior ou igual a nObjetosAntesAbertura
				if ( rot  >= nObjetosAntesAbertura and rot < nObjetosDepoisAbertura ) {
					pm->data2D[i][j] = 1;
				} else {
					pm->data2D[i][j] = 0;
				}
			}
		}
		if ( salvarResultadosParciais )	{
			os.str("");
			os << "MatrizPmNoFinalDoLoop_" << raioElemen << ".pbm";
			cout << "-->Salvando imagem " << os.str().c_str() << endl ;
			pm->Write(os.str());
		}
	}// fim do looping
	/*
			// Salva Vetor VPoros
			Salvar( VPoros , "distribuicaoPoros.dtp" );

			// Salva Vetor VSitios
			Salvar( VSitios, "distribuicaoSitios.dtp" );

			// Salva Vetor VLigacoes
			Salvar( VLigacoes, "distribuicaoLigacoes.dtp" );
		*/
	// Seta MSitiosLigacoes com resultado final
	//int tipoObjeto ;
	for ( int i = 0; i < pm->NX(); i++) {
		for ( int j = 0; j < pm->NY(); j++) {
			//TEMPORARIO, verificar porque código abaixo (documentado) Não funciona?
			// Se não é fundo e não é ligação
			if ( MInicialRotulada->data2D[i][j] > 0 and MSitiosLigacoes->data2D[i][j] != LIGACAO ) {
				MSitiosLigacoes->data2D[i][j] = SITIO;
			}
			/*		if( MInicialRotulada->data2D[i][j] > 0 )
									{
									tipoObjeto = Objeto[ MInicialRotulada->data2D[i][j] ].Tipo() ;
									// SOLIDO = 0, PORO = 1, SITIO = 2, LIGACAO = 3, RAMO_MORTO = 4, NAO_IDENTIFICADO = 5
									// Se o numero de coneccoes >1, então é ligação
									if ( tipoObjeto == SOLIDO )
										MSitiosLigacoes->data2D[i][j] = SOLIDO;
									else if ( tipoObjeto  == PORO )
										MSitiosLigacoes->data2D[i][j] = PORO;
									else if ( tipoObjeto  == SITIO )
										MSitiosLigacoes->data2D[i][j] = SITIO;
									else if ( tipoObjeto  == LIGACAO )
										MSitiosLigacoes->data2D[i][j] = LIGACAO;
									else if ( tipoObjeto  == RAMO_MORTO )
										MSitiosLigacoes->data2D[i][j] = RAMO_MORTO;
									else if ( tipoObjeto  == NAO_IDENTIFICADO )
										MSitiosLigacoes->data2D[i][j] = NAO_IDENTIFICADO;
									}*/
		}
	}
	// Salva MSitiosLigacoes em disco - resultado final (imagem com SOLIDO, PORO, SITIO, RAMO_MORTO, LIGACAO
	os.str("");
	os	<< "MatrizSitiosLigacoes"
			<< "-Modelo-" 	<< modelo
			<< "-rmee-" 	<< raioMaximoElementoEstruturante
			<< "-frree-"	<< fatorReducaoRaioElemEst
			<< "-iree-"	<< incrementoRaioElementoEstruturante
			<< "-" << nomeImagem
			<< ".pgm";
	cout << "-->Salvando imagem " << os.str().c_str() << endl ;
	MSitiosLigacoes->SetFormato( P2_X_Y_GRAY_ASCII );
	// Como obter maior valor de uma enumeração ??
	MSitiosLigacoes->NumCores ( MSitiosLigacoes->MaiorValor() ); // 256, numero objetos informa o maior rotulo utilizado.
	MSitiosLigacoes->Write( os.str() );

	// Salva MInicialRotulada em disco - resultado final (rótulos dos objetos)
	// Associada ao grafo permite a identificação dos píxeis.
	os.str("");
	os << "MatrizInicialRotulada-Final"
				//		 << "-Modelo-" 	<< modelo
				//		 << "-rmee-" 	<< raioMaximoElementoEstruturante
				//		 << "-frree-"	<< fatorReducaoRaioElemEst
				//		 << "-iree-"	<< incrementoRaioElementoEstruturante
		 << "-" << nomeImagem
		 << ".pgm";
	cout << "-->Salvando imagem " << os.str().c_str() << endl ;
	MInicialRotulada->SetFormato( P2_X_Y_GRAY_ASCII );
	MInicialRotulada->NumCores ( nObjetosDepoisAberturaComplementar ); // 256, numero objetos informa o maior rotulo utilizado.
	MInicialRotulada->Write( os.str() );

	// Salva em disco a estrutura de dados (o grafo)
	os.str("");
	os 	<< nomeImagem
			<< "-Modelo-" 	<< modelo
				 //<< "-rmee-" 	<< raioMaximoElementoEstruturante
				 //<< "-frree-"	<< fatorReducaoRaioElemEst
				 //<< "-iree-"	<< incrementoRaioElementoEstruturante
			<< ".graph";
	ofstream fout ( os.str().c_str() );
	fout 	<< "# numeroTotalDeOBjetosDoGrafo" << endl
				<< "# idDoObjeto tipo rotulo numeroConeccoes indiceDasConeccoes" << endl
				<< Objeto.size() << endl;
	for ( int i = 0 ; i < Objeto.size(); i++ ) {
		// idDoObjeto tipo rotulo numeroConeccoes
		fout 	<< i << " " << Objeto[i].Tipo() //<< " " << Objeto[i].Rotulo()
					<< " " << Objeto[i].SConexao().size();
		for ( std::set<int>::iterator it = Objeto[i].SConexao().begin();
					it != Objeto[i].SConexao().end();
					it++	)
		{
			// indiceDasConeccoes" << endl
			fout <<  " " << *it << " ";
		}
		// Pula para linha de baixo
		fout << endl;
	}
	fout.close();

	delete MSitiosLigacoes;
	delete MInicialRotulada	;
	delete pfmf;
	return;
}


/**
 * Modelo 3: Parte do resultado da abertura, e usa extensivamente imagens rotuladas.
 * Andre, observando o resultado dos modelos 0 e 1  vemos alguns problemas,
 * tem muito sitio virando ligacao!
 * Este modelo usa extensivamente imagens rotuladas, para tentar identificar melhor as ligaçoes.
 *
 * Principio:
 * Caminhando pelas imagens aberturas observa-se que durante um tempo (alguns raios),
 * o objeto não sofre divisões, quando, para um dado raio, o objeto é dividido.
 * Este é um bom momento para diferenciar o que é sitio, e o que é ligação.
 * Os objetos que ficarem devem ser considerados sítios,
 * e os eliminados devem ser considerados ligações.
 * Para evitar que as bordas virem ligações, usa-se a dilatação extra.
*/
/**
 * Usa o modelo 0, mas seta o incrementoRaioElementoEstruturante como sendo 3,
 */
void CAberturaDilatacao::DistSitiosLigacoes_Modelo_3() {
	modelo = 3;
	//	int numeroObjetosAcumulados = 0;
	int nObjetosAntesAbertura = 0;			// numeroObjetosAntesAbertura
	int nObjetosDepoisAbertura = 0;		// nObjetosDepoisAbertura
	int nObjetosDepoisAberturaComplementar = 0;	// nObjetosDepoisAberturaComplementar

	// Cria MSitiosLigacoes e deixa vazia (apenas para visualizacao das ligacoes - eliminar depois)
	cout << "Criando e inicializando MLigacoes..." << endl;
	TCMatriz2D<int>* MSitiosLigacoes = new TCMatriz2D<int>( pm->NX(), pm->NY() );
	MSitiosLigacoes->Constante(0);
	MSitiosLigacoes->SetFormato( P2_X_Y_GRAY_ASCII );
	MSitiosLigacoes->NumCores(6); // 256, numero objetos informa o maior rotulo utilizado.

	// Cria MInicialRotulada, é a imagem inicial rotulada
	// a cada etapa/raio/passo, é onde são armazenados os rótudos dos objetos identificados
	cout << "Rotulando a imagem inicial..." << endl;
	matrizRotulo->Go( pm );
	nObjetosAntesAbertura = matrizRotulo->NumeroObjetos(); //desconsidera fundo como objeto
	cout << "-->nObjetosAntesAbertura =..." 	<< nObjetosAntesAbertura << endl ;

	cout << "Criando e inicializando MInicialRotulada - tons de cinza..." << endl;
	TCMatriz2D<int>* MInicialRotulada = new TCMatriz2D<int>( *matrizRotulo );
	MInicialRotulada->SetFormato( P2_X_Y_GRAY_ASCII );
	MInicialRotulada->NumCores ( matrizRotulo->NumeroObjetos() ); // 256, numero objetos informa o maior rotulo utilizado.
	if ( salvarResultadosParciais ) {
		MInicialRotulada->Write("MatrizInicialRotulada.pgm");
	}

	////NOVO TENTAR RETIRAR POIS CONSOME MAIS MEMORIA - NOVO MODELO 3
	TCMatriz2D<bool>* MatrizPmAntesAbertura = new TCMatriz2D<bool>( *pm ); //NOVO MODELO 3

	//	Cria vetor de objetos
	//	No início todos os objetos são marcados como fundo (verificar se esta ok)
	vector< CObjetoImagem > Objeto ( matrizRotulo->NumeroObjetos() , CObjetoImagem( SOLIDO, 0 )); 	// inclui o objeto fundo (0)
	Objeto.reserve ( 100 *  matrizRotulo->NumeroObjetos() ); 		// evita realocacoes

	// Seta o Rotulo e o Tipo dos objetos da imagem inicial
	// Seta o fundo
	//Objeto[0].Rotulo( 0 );
	Objeto[0].Tipo( SOLIDO );
	// Seta os demais objetos como sendo PORO
	for ( int i = 1; i < Objeto.size(); i++ ) {
		//Objeto[i].Rotulo( i );
		Objeto[i].Tipo( PORO );
	}

	/*
		// Cria vetor VPoros
			cout << "Criando e inicializando VPoros..." << endl;
			vector<double> VPoros ( ((pm->NX()-1)/2+1) , 0.0); // inicializa com zero

		// Cria vetor VSitios
			cout << "Criando e inicializando VSitios..." << endl;
			vector<double> VSitios ( ((pm->NX()-1)/2+1) , 0.0); // inicializa com zero

		// Cria vetor VLigacoes
			cout << "Criando e inicializando VLigacoes..." << endl;
			vector<double> VLigacoes ( ((pm->NX()-1)/2+1) , 0.0); // inicializa com zero
		*/

	// Cria filtro para operacoes de abertura e dilatacao
	cout << "Criando filtro CFEMorfologiaMatematica..." << endl ;
	pfmf = new TCFEMorfologiaMatematica<bool>( pm, 3, INDICE, FUNDO );

	// Variaveis auxiliares
	// usada para setar nome dos arquivos de disco
	ostringstream os;

	// Como no looping iremos setar nObjetosAntesAbertura  como o  nObjetosDepoisAberturaComplementar ,
	// aqui, devemos setar nObjetosDepoisAberturaComplementar  como sendo nObjetosAntesAbertura
	nObjetosDepoisAberturaComplementar = nObjetosAntesAbertura ;

	// Entra num looping para o raio do elemento estruturante
	// o incremento é dado pelo incrementoRaioElementoEstruturante
	cout << "Entrando no looping de calculo das distribuicoes..." << endl ;
	for ( int raioElemen = 1; raioElemen <= (pm->NX()-1)/2; raioElemen += incrementoRaioElementoEstruturante ) {
		cout << "==>RAIO Elemento Estruturante = " <<  raioElemen << endl ;
		// 	cout << "Pressione enter para iniciar processamento deste raio." << endl;
		// 	cin.get();

		//// NOVO TENTAR RETIRAR POIS CONSOME MAIS MEMORIA - NOVO MODELO 3
		for ( int i = 0; i < pm->NX(); i++)
			for ( int j = 0; j < pm->NY(); j++)
				MatrizPmAntesAbertura->data2D[i][j] = pm->data2D[i][j];


		// ABERTURA
		// Armazena o numero de objetos antes da abertura (i.e. da rotulagem anterior)
		nObjetosAntesAbertura = nObjetosDepoisAberturaComplementar;
		cout << "-->nObjetosAntesAbertura =..." 	<< nObjetosAntesAbertura << endl ;

		cout << "-->Processando Abertura..." << endl ;
		pfmf->Abertura( pm , raioElemen );

		// Calcula porosidade apos abertura, vai usar como criterio de parada.
		porosidade = Porosidade ( pm );
		cout << "-->Porosidade = " << porosidade << endl;

		// Criterio de parada; temporario, melhorar o criterio de parada/saida.
		// note que como nao processa a borda sempre vai ter porosidade na borda, ou seja, acrescente uma borda na imagem.
		// ou modifique o critério de saída.
		if ( porosidade == 0 or raioElemen >= raioMaximoElementoEstruturante )
			break;

		// Toda saida, como a saida abaixo só deve ser executada se variavel controle for true, bool salvarResultadosParciais = true;
		// Rafael criar funcao para SalvarImagemParcial(ponteiro_para_imagem, NomeImagem);
		if ( salvarResultadosParciais ) {
			os.str("");
			os << "MatrizAbertura_" << raioElemen << ".pbm";
			cout << "-->Salvando imagem " << os.str().c_str() << endl ;
			pm->Write(os.str());
		}

		// Rotula a imagem abertura
		cout << "-->Rotulando matriz pm apos abertura " << endl;
		matrizRotulo->Go( pm ); 		// matrizRotulo
		// Marca o numero de objetos depois da abertura
		nObjetosDepoisAbertura = nObjetosAntesAbertura + matrizRotulo->NumeroObjetos();
		cout << "-->nObjetosDepoisAbertura =..." 	<< nObjetosDepoisAbertura << endl ;

		// 	if ( salvarResultadosParciais )
		// 		{
		// 	 	os.str("");
		// 		os << "MatrizAberturaRotulada_" << raioElemen << ".pgm";
		// 		cout << "-->Salvando imagem " << os.str().c_str() << endl ;
		// 		matrizRotulo->NumCores ( nObjetosDepoisAbertura );
		// 		matrizRotulo->SetFormato(  WRITEFORM_PI_X_Y_GRAY_ASCII ); // WRITEFORM_PI_X_Y_ASCII
		// 		matrizRotulo->Write(os.str());
		// 		}

		// Adiciona objetos rotulados a lista de objetos -  o que ficou na imagem abertura são sitios.
		for ( int i = nObjetosAntesAbertura ; i < nObjetosDepoisAbertura; i++ ) {
			Objeto.push_back( CObjetoImagem( SITIO , i ) ) ;
		}

		// Aqui poderia conectar os POROS aos sítios. Pois como todos os POROS virarão ou SITIOS ou RAMOS_MORTOS ou LIGACOES,
		// não teremos na imagem nenhuma informação sobre os rótulos dos POROS!
		// e esta informação não esta sendo armazenada nos objetos (os primeiros objetos ficam vazios).

		// IMPLEMENTAR!!

		// Copia imagem abertura rotulada para matriz inicial rotulada,
		// assim, a matriz inicial rotulada já terá a informação do rótulo dos sítios.
		// A seguir, inverte pm (pm passa a ser o complemento da abertura)
		for ( int i = 0; i < pm->NX(); i++) {
			for ( int j = 0; j < pm->NY(); j++) {
				// Se faz parte abertura; então copia para matriz inicial rotulada (copia rotulo dos sitios)
				// MInicialRotulada terá os rótulos iniciais e os rotulos da imagem abertura acrescidos do número de objetos antes da abertura
				// Isso precisa se feito pois o rotulador não mais permite informar o rótulo inicial.
				if ( pm->data2D[i][j] > 0 )
					MInicialRotulada->data2D[i][j] = matrizRotulo->data2D[i][j] + nObjetosAntesAbertura;

				// Se é Poro, ie, MInicialRotulada->data2D[i][j] > 0 (eliminamos o fundo da imagem)
				// e não faz parte da matriz abertura, pm->data2D[i][j] == 0
				// entao marca como abertura complementar (inverte valores de pm).
				// ERRO: a linha abaixo estava marcando todos os ramos  mortos, inclusive os dos passos anteriores!
				// if ( MInicialRotulada->data2D[i][j] > 0 )
				// Precisa incluir apenas as regiões envolvidas na abertura
				if ( MatrizPmAntesAbertura->data2D[i][j] > 0 ) { // esta no passo anterior a abertura // NOVO MODELO 3 usa MatrizPmAntesAbertura
					if ( pm->data2D[i][j] == 0 ) {
						pm->data2D[i][j] = 1 ;
					} else {
						pm->data2D[i][j] = 0 ;
					}
				}
			}
		}
		if ( salvarResultadosParciais ) {
			os.str("");
			os << "MatrizAberturaComplementar_" << raioElemen << ".pbm";
			cout << "-->Salvando imagem " << os.str().c_str() << endl ;
			pm->SetFormato( P1_X_Y_ASCII ); // WRITEFORM_PI_X_Y_GRAY_ASCII
			pm->Write(os.str());
		}


		// Rotulagem da matriz abertura complementar
		cout << "-->Processando rotulagem da matriz abertura complementar..." << endl ;
		matrizRotulo->Go( pm );
		nObjetosDepoisAberturaComplementar = nObjetosDepoisAbertura + matrizRotulo->NumeroObjetos();
		cout << "-->nObjetosDepoisAberturaComplementar =..."<< nObjetosDepoisAberturaComplementar << endl ;

		if ( salvarResultadosParciais ) {
			os.str("");
			os << "MatrizAberturaComplementarRotulada_" << raioElemen << ".pgm";
			cout << "-->Salvando imagem " << os.str().c_str() << endl ;
			matrizRotulo->NumCores ( matrizRotulo->NumeroObjetos() );
			matrizRotulo->SetFormato( P2_X_Y_GRAY_ASCII );
			matrizRotulo->Write(os.str());
		}

		// Adiciona objetos rotulados a lista de objetos -  são RAMOs_MORTOs
		// depois iremos identificar quais ramos_mortos são ligações
		for ( int i = nObjetosDepoisAbertura ; i < nObjetosDepoisAberturaComplementar; i++ ) {
			Objeto.push_back( CObjetoImagem(  RAMO_MORTO , i ) ) ; // trocar por resize + rapido.
		}

		// Copia imagem abertura complementar rotulada para matriz inicial rotulada.
		// Para cada rótulo da imagem abertura complementar acrescenta o
		// nObjetosDepoisAbertura de forma que os rotulos sejam sequenciais.
		for ( int i = 0; i < pm->NX(); i++) {
			for ( int j = 0; j < pm->NY(); j++) {
				// Se não é fundo, copia para matriz inicial rotulada
				if ( pm->data2D[i][j] > 0 )
					MInicialRotulada->data2D[i][j] = matrizRotulo->data2D[i][j] + nObjetosDepoisAbertura;
			}
		}

		// Percorre a imagem MInicialRotulada e faz as conecções.
		// Identifica os rótulos dos vizinhos e faz a conecção se o rotulo já não tiver sido incluido
		// lembre-se que set não tem repeticao, e sConexao é do tipo set<int>.
		// Desconsideramos os pixeis da borda para evitar bugs.
		// Aqui a MInicialRotulada tem SOLIDO, POROs, SITIOs e RAMOs_MORTOs
		int dimensaoBorda = 1;
		int rotuloPixel_i_j;  // auxiliar
		for ( int i = dimensaoBorda; i < pm->NX() - dimensaoBorda; i++) {
			for ( int j = dimensaoBorda; j < pm->NY() - dimensaoBorda; j++) {
				rotuloPixel_i_j	 =  MInicialRotulada->data2D[i][j];
				// Só devemos considerar os rotulos da imagem abertura, i.e,  rotulo >= nObjetosAntesAbertura
				if (  rotuloPixel_i_j	 >=   nObjetosAntesAbertura ) {
					// Se os rotulos são diferentes e
					// fazem parte da matriz abertura (rotulo >= nObjetosAntesAbertura) então marca a coneccão.
					if ( rotuloPixel_i_j != MInicialRotulada->data2D[i][j-1]
							 and MInicialRotulada->data2D[i][j-1]  >= nObjetosAntesAbertura )
						// Como a lista de coneccões é do tipo set, só marca uma vez.
						Objeto[rotuloPixel_i_j].Conectar( MInicialRotulada->data2D[i][j-1] );

					if ( rotuloPixel_i_j != MInicialRotulada->data2D[i][j+1]
							 and MInicialRotulada->data2D[i][j+1]  >= nObjetosAntesAbertura )
						Objeto[rotuloPixel_i_j].Conectar( MInicialRotulada->data2D[i][j+1] );

					if ( rotuloPixel_i_j != MInicialRotulada->data2D[i-1][j]
							 and MInicialRotulada->data2D[i-1][j]  >= nObjetosAntesAbertura )
						Objeto[rotuloPixel_i_j].Conectar( MInicialRotulada->data2D[i-1][j] );

					if ( rotuloPixel_i_j != MInicialRotulada->data2D[i+1][j]
							 and MInicialRotulada->data2D[i+1][j]  >= nObjetosAntesAbertura )
						Objeto[rotuloPixel_i_j].Conectar( MInicialRotulada->data2D[i+1][j] );
				}
			}
		}
		// Agora vamos percorrer os objetos anotados como RAMOs_MORTOs e identificar as ligações
		// Percorre apenas os rotulos marcados como RAMOs_MORTOs.
		for ( int i = nObjetosDepoisAbertura ; i < nObjetosDepoisAberturaComplementar; i++ ) {
			// No modelo 2:
			// Se o numero de conecções for maior que 1, então é ligação.
			if ( Objeto[i].SConexao().size() > 1 )
				Objeto[i].Tipo( LIGACAO );

			// O problema que a ligação esta se estendendo até que o sítio seja eliminado pela abertura;
			// O resultado é bom, mas as ligações estão ficando muito grandes!

			// O problema ocorre porque estamos usando uma única matriz MInicialRotulada para todos os rótulos,
			// os do passo atual e anteriores. E foram consideradas as conecções com os objetos (rotulos) que já foram eliminados

			// No modelo 3:
			// corrige-se o fato de que só devemos considerar as conecções com os objetos do tipo SITIO e
			// no intervalo:  nObjetosDepoisAbertura <= i < nObjetosDepoisAberturaComplementar, ou seja,
			// as conecções entre Ligações e Sitios identificados nesta passagem.

			// Cria set que vai ter somente as conecções do rótulo i (ramo i), com os objetos sítios (sem repeticao)
			// 		std::set<int> rotuloSitiosConectados;
			// 		for( std::set<int>::iterator it = Objeto[i].SConexao().begin(); it != Objeto[i].SConexao().end(); it++ )
			// 			{
			// 			if ( Objeto[ *it ].Tipo() == SITIO and *it >= nObjetosDepoisAbertura ) // and *it < nObjetosDepoisAberturaComplementar
			// 				rotuloSitiosConectados.insert( *it );
			// 			if ( rotuloSitiosConectados.size() > 0 )
			// 				Objeto[i].Tipo( LIGACAO );
			// 			}
		}

		// Agora, todos o objetos foram anotados, temos SOLIDO, POROs, SITIOs, RAMOs_MORTOs e LIGACOES.
		// vamos armazenar resultado na MSitiosLigacoes e salvar resultado parcial em disco.
		for ( int i = 0; i < pm->NX(); i++) {
			for ( int j = 0; j < pm->NY(); j++) {
				// pm armazena a abertura complementar, tem RAMOs_MORTOs e LIGACOES
				// Sé é ramo ou ligação
				if ( pm->data2D[i][j] > 0 ) {
					// Se o numero de coneccoes >1, então é ligação
					if ( Objeto[ MInicialRotulada->data2D[i][j] ].Tipo() == LIGACAO )
						MSitiosLigacoes->data2D[i][j] = LIGACAO;
				}
			}
		}
		// Salva MSitiosLigacoes em disco
		// ## Modelo NomeImagem raioMaximoElementoEstruturante fatorReducaoRaioElemEst
		// incrementoRaioElementoEstruturante 20(finalizaPrograma)
		if ( salvarResultadosParciais ) {
			os.str("");
			os << "MatrizSitiosLigacoes"
				 << "-Modelo-" 	<< modelo
				 << "-rmee-"		<< raioMaximoElementoEstruturante
				 << "-frree-"		<< fatorReducaoRaioElemEst
				 << "-iree-"		<< incrementoRaioElementoEstruturante
				 << "-ree-"			<< raioElemen
				 << "-"					<< nomeImagem
				 << ".pgm";

			cout << "-->Salvando imagem " << os.str().c_str() << endl ;
			MSitiosLigacoes->Write( os.str() );
		}

		// Precisamos fazer com que pm volte a ser a matriz abertura
		for ( int i = 0; i < pm->NX(); i++) {
			for ( int j = 0; j < pm->NY(); j++) {
				int rot = MInicialRotulada->data2D[i][j];
				// Se maior ou igual a nObjetosAntesAbertura
				if ( rot  >= nObjetosAntesAbertura and rot < nObjetosDepoisAbertura ) {
					pm->data2D[i][j] = 1;
				} else {
					pm->data2D[i][j] = 0;
				}
			}
		}
		if ( salvarResultadosParciais )	{
			os.str("");
			os << "MatrizPmNoFinalDoLoop_" << raioElemen << ".pbm";
			cout << "-->Salvando imagem " << os.str().c_str() << endl ;
			pm->Write(os.str());
		}
	}// fim do looping
	/*
			// Salva Vetor VPoros
			Salvar( VPoros , "distribuicaoPoros.dtp" );

			// Salva Vetor VSitios
			Salvar( VSitios, "distribuicaoSitios.dtp" );

			// Salva Vetor VLigacoes
			Salvar( VLigacoes, "distribuicaoLigacoes.dtp" );
		*/
	// Seta MSitiosLigacoes com resultado final
	// ETipoObjetoImagem tipoObjeto ;
	for ( int i = 0; i < pm->NX(); i++) {
		for ( int j = 0; j < pm->NY(); j++) {
			//TEMPORARIO, verificar porque código abaixo (documentado) Não funciona?
			// Se não é fundo e não é ligação
			if ( MInicialRotulada->data2D[i][j] > 0 and  MSitiosLigacoes->data2D[i][j] != LIGACAO )
				MSitiosLigacoes->data2D[i][j] = SITIO;

			/*		if( MInicialRotulada->data2D[i][j] > 0 )
									{
									tipoObjeto = Objeto[ MInicialRotulada->data2D[i][j] ].Tipo() ;
									// SOLIDO = 0, PORO = 1, SITIO = 2, LIGACAO = 3, RAMO_MORTO = 4, NAO_IDENTIFICADO = 5
									// Se o numero de coneccoes >1, então é ligação
									if ( tipoObjeto == SOLIDO )
										MSitiosLigacoes->data2D[i][j] = SOLIDO;
									else if ( tipoObjeto  == PORO )
										MSitiosLigacoes->data2D[i][j] = PORO;
									else if ( tipoObjeto  == SITIO )
										MSitiosLigacoes->data2D[i][j] = SITIO;
									else if ( tipoObjeto  == LIGACAO )
										MSitiosLigacoes->data2D[i][j] = LIGACAO;
									else if ( tipoObjeto  == RAMO_MORTO )
										MSitiosLigacoes->data2D[i][j] = RAMO_MORTO;
									else if ( tipoObjeto  == NAO_IDENTIFICADO )
										MSitiosLigacoes->data2D[i][j] = NAO_IDENTIFICADO;
									}*/
		}
	}
	// Salva MSitiosLigacoes em disco - resultado final (imagem com SOLIDO, PORO, SITIO, RAMO_MORTO, LIGACAO
	os.str("");
	os	<< "MatrizSitiosLigacoes"
			<< "-Modelo-" 	<< modelo
			<< "-rmee-" 	<< raioMaximoElementoEstruturante
			<< "-frree-"	<< fatorReducaoRaioElemEst
			<< "-iree-"	<< incrementoRaioElementoEstruturante
			<< "-" << nomeImagem
			<< ".pgm";
	cout << "-->Salvando imagem " << os.str().c_str() << endl ;
	MSitiosLigacoes->SetFormato( P2_X_Y_GRAY_ASCII );
	MSitiosLigacoes->NumCores ( MSitiosLigacoes->MaiorValor() ); // 256, numero objetos informa o maior rotulo utilizado.
	MSitiosLigacoes->Write( os.str() );

	//******************************
	// 1- Criar matriz nova
	// 2- Copiar sítios
	// 3- Calcular e salvar distribuicao de sitios
	// 4- Apagar e copiar dados ligaçoes
	// 5- Calcular e salvar distribuicao de  ligaçoes
	// TCMatriz2D< int >* ptrImg = new TCMatriz2D< int >(nx, ny);
	//*******************************

	// Salva MInicialRotulada em disco - resultado final (rótulos dos objetos)
	// Associada ao grafo permite a identificação dos píxeis.
	os.str("");
	os	<< "MatrizInicialRotulada-Final"
			<< "-Modelo-" 	<< modelo
			<< "-rmee-" 	<< raioMaximoElementoEstruturante
			<< "-frree-"	<< fatorReducaoRaioElemEst
			<< "-iree-"	<< incrementoRaioElementoEstruturante
			<< "-" << nomeImagem
			<< ".pgm";
	cout << "-->Salvando imagem " << os.str().c_str() << endl ;
	MInicialRotulada->SetFormato( P2_X_Y_GRAY_ASCII );
	MInicialRotulada->NumCores ( nObjetosDepoisAberturaComplementar ); // 256, numero objetos informa o maior rotulo utilizado.
	MInicialRotulada->Write( os.str() );

	// Salva em disco a estrutura de dados (o grafo)
	os.str("");
	os 	<< nomeImagem
			<< "-Modelo-" 	<< modelo
				 //			<< "-rmee-" 	<< raioMaximoElementoEstruturante
				 //			<< "-frree-"	<< fatorReducaoRaioElemEst
				 //			<< "-iree-"	<< incrementoRaioElementoEstruturante
			<< ".graph";
	ofstream fout ( os.str().c_str() );
	fout 	<< "# numeroTotalDeOBjetosDoGrafo" << endl
				<< "# idDoObjeto tipo rotulo numeroConeccoes indiceDasConeccoes" << endl
				<< Objeto.size() << endl;
	for ( int i = 0 ; i < Objeto.size(); i++ ) {
		// idDoObjeto tipo rotulo numeroConeccoes
		fout 	<< i << " " << Objeto[i].Tipo() //<< " " << Objeto[i].Rotulo()
					<< " " << Objeto[i].SConexao().size();
		for ( std::set<int>::iterator it = Objeto[i].SConexao().begin(); it != Objeto[i].SConexao().end(); it++	) {
			// indiceDasConeccoes" << endl
			fout <<  " " << *it << " ";
		}
		fout << endl; // Pula para linha de baixo
	}
	fout.close();

	delete pfmf;
	delete MSitiosLigacoes;
	delete MInicialRotulada;
	delete MatrizPmAntesAbertura;

	return;
}

/**
 * A partir das imagens abertura, gerar imagem 3D, em que cada plano representa
 * resultado abertura para raioElemen. Ajuda na visualizacao em 3D.
	*/
void CAberturaDilatacao::DistSitiosLigacoes_Modelo_4() {
	cout << "\a\a\aNAO IMPLEMENTADO!" << endl;
	return;
}

// Objetiva determinar uma sequencia de abertura marcando
// e salvando a ImagemAbertura com diferentes tons de cinza.

// Se o pixel esta na matriz pm, copia para MAberturaDilatacao com o valor do .raioElemen,
// ficando cada abertura com tom de conza diferente.
void CAberturaDilatacao::SequenciaAberturaTonsCinza()
{
	// 	Cria matriz abertura
	TCMatriz2D<bool>* MAbertura = new TCMatriz2D<bool>( *pm );
	pfmf = new TCFEMorfologiaMatematica<bool>( pm , 3 );

	// Entra num looping para o raio do elemento estruturante
	cout << "Entrando no looping de calculo das aberturas..." << endl ;
	for ( int raioElemen = 1; raioElemen <= (pm->NX()-1)/2; raioElemen++ ) {
		cout << "==>RAIO Elemento Estruturante = " <<  raioElemen << endl ;

		cout << "-->Processando Abertura..." << endl ;
		pfmf->Abertura( pm , raioElemen );

		cout << "-->Seta píxeis que ficaram com o valor do raioElemen = " << raioElemen << endl ;
		for ( int i = 0; i < pm->NX(); i++)
			for ( int j = 0; j < pm->NY(); j++)
				if ( pm->data2D[i][j] )
					MAbertura->data2D[i][j] = raioElemen;
		// criterio parada
		if ( raioElemen == raioMaximoElementoEstruturante)
			break;
	}

	MAbertura->SetFormato( P2_X_Y_GRAY_ASCII );
	MAbertura->NumCores ( MAbertura->MaiorValor() + 1 );
	MAbertura->Write("MAbertura.pgm");

	return;
}
