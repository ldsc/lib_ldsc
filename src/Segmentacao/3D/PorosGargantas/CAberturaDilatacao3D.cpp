#include <Segmentacao/3D/PorosGargantas/CAberturaDilatacao3D.h>
#include <Segmentacao/CObjetoImagem.h> // novo, repreenta objeto da imagem
#include <iomanip>
#include <cmath>

using namespace std;

bool CAberturaDilatacao3D::salvarResultadosParciais = 0;
/*
CAberturaDilatacao3D::CAberturaDilatacao3D() :
	fatorReducaoRaioElemEst (1), raioMaximoElementoEstruturante ( 30000 ), // usar limits
	incrementoRaioElementoEstruturante ( 1 ), pm(0), modelo(2), INDICE(1), FUNDO(0)
//, salvarResultadosParciais(0)
{
	matrizRotulo = new TCRotulador3D<bool>(INDICE, FUNDO);
	pfmf = new TCFEMMIDFd3453D<bool>( pm, INDICE, FUNDO );
}
*/

CAberturaDilatacao3D::CAberturaDilatacao3D( TCMatriz3D<bool>* &matriz , std::string _nomeImagem, int _indice, int _fundo)
	: pm(matriz), // pm é ponteiro para imagem externa (se mudar externamente teremos problemas).
		nomeImagem(_nomeImagem),
		fatorReducaoRaioElemEst (1), raioMaximoElementoEstruturante ( 500 ), // usar limits
		incrementoRaioElementoEstruturante ( 1 ), modelo(4), INDICE(_indice), FUNDO(_fundo)
{
	matrizRotulo = new TCRotulador3D<bool>( matriz, INDICE, FUNDO );
	pfmf = new TCFEMMIDFd3453D<bool>( matriz, INDICE, FUNDO );
}

CAberturaDilatacao3D::CAberturaDilatacao3D( TCImagem3D<bool>* &matriz , std::string _nomeImagem, int _indice, int _fundo)
	: nomeImagem(_nomeImagem),
		fatorReducaoRaioElemEst (1), raioMaximoElementoEstruturante ( 500 ), // usar limits
		incrementoRaioElementoEstruturante ( 1 ), modelo(4), INDICE(_indice), FUNDO(_fundo)
{
	pm = dynamic_cast<TCMatriz3D<bool> *>(matriz), // pm é ponteiro para imagem externa (se mudar externamente teremos problemas).
	matrizRotulo = new TCRotulador3D<bool>( pm, INDICE, FUNDO );
	pfmf = new TCFEMMIDFd3453D<bool>( pm, INDICE, FUNDO );
}

CAberturaDilatacao3D::~CAberturaDilatacao3D() {
	delete matrizRotulo;
	delete pfmf;
}

/*
void CAberturaDilatacao3D::Salvar(CVetor* &vetor, std::string nomeArquivo) {
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
*/

void CAberturaDilatacao3D::Salvar(vector<double> v, std::string nomeArquivo) {
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

double CAberturaDilatacao3D::Porosidade( TCMatriz3D<bool>* &_pm ) {
	double porosidade = 0.0;
	for ( int i = 0; i < _pm->NX(); i++ )
		for ( int j = 0; j < _pm->NY(); j++ )
			for ( int k = 0; k < _pm->NZ(); k++ )
				if ( _pm->data3D[i][j][k] == 1 ) {
					porosidade++;
				}
	return porosidade / ( (double) _pm->NX() * _pm->NY() * _pm->NZ() );
}

// Observações importantes:
// Observe que a porosidade não deve estar indo para zero (0), porque o filtro abertura desconsidera as bordas.
// para solucionar este e outros problemas com a borda, criar matriz aumentada, em que a borda é uma região de fundo (zeros)
void CAberturaDilatacao3D::DistTotalPoros() {
	// Cria cópia da matriz
	TCMatriz3D<bool>* matrizAuxiliar = 0;

	matrizAuxiliar = new TCMatriz3D<bool>( *pm );

	if ( matrizAuxiliar == NULL ) {
		cerr << "Erro alocação matrizAuxiliar em CAberturaDilatacao3D::DistTotalPoros()!";
		return ;
	}

	// Aloca e zera vetor com distribuicaoTotalPoros
	CVetor * distribuicaoTotalPoros = new CVetor(( matrizAuxiliar->NX()-1)/2+1);
	distribuicaoTotalPoros->Constante(10000); //???
	distribuicaoTotalPoros->data1D[0]=0;

	// Calcula porosidade da imagem
	porosidade = Porosidade(matrizAuxiliar);
	cout << "Porosidade=" << porosidade << endl;

	// Porosidade parcial
	double porosidadeParcial = porosidade;

	// Calculo das porosidades parciais para cada raio do EE
	double distAcumulada;
	for ( int raioElem = 1; raioElem <= (matrizAuxiliar->NX()-1)/2; raioElem++ ) {
		// Aplica abertura a imagem matrizAuxiliar, usando elemento estruturante de raio raioElem
		pfmf->Go (matrizAuxiliar, raioElem);
		pfmf->Abertura(matrizAuxiliar, raioElem);

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

	//Salvar( distribuicaoTotalPoros , "distPoros.dtp" );
	distribuicaoTotalPoros->SetFormato(V1_X_ASCII);
	distribuicaoTotalPoros->Write("distPoros.dtp");
}

//	Recebe ponteiro para duas matrizes do tipo bool e o valor de indice a ser considerado para cada uma delas.
//	Salva a mesclagem das matrizes em disco de forma que 0 será o fundo, 1 serão os índices da primeira matriz e 2 serão os índices da segunda matriz.
//	Se a possição dos índices coincidirem, o indice da última matriz informada como parâmetro será considerado.
//	Método criado para que se possa salvar em disco duas matrizes bool ao invés de utilizar uma matriz int para representar sólidos, sitios e ligações (consome menos memória).
bool CAberturaDilatacao3D::Write(string fileName, TCMatriz3D<bool>* &mat1, TCMatriz3D<bool>* &mat2 ) {
	ofstream fout; //  Abre arquivo disco
	fout.open(fileName.c_str(), ios::binary);
	int nx, ny, nz;
	if (fout.good()){
		//menores valores para nx, ny e nz.
		nx = ( mat1->NX() <= mat2->NX() ) ? mat1->NX() : mat2->NX();
		ny = ( mat1->NY() <= mat2->NY() ) ? mat1->NY() : mat2->NY();
		nz = ( mat1->NZ() <= mat2->NZ() ) ? mat1->NZ() : mat2->NZ();

		//cabeçalho
		fout << setw (0) << "D5" << '\n' << nx << ' ' << ny << ' ' << nz << '\n' << 3 << '\n';

		//percorre matrizes e mescla resultados salvando em disco.
		for (int k = 0; k < nz; k++) {
			for (int j = 0; j < ny; j++) {
				for (int i = 0; i < nx; i++) {
					if( mat2->data3D[i][j][k] == INDICE )				// se o voxel for índice em mat2
						fout << (unsigned char) 2;
					else if( mat1->data3D[i][j][k] == INDICE )	// senão, se o voxel for índice em mat1
						fout << (unsigned char) 1;
					else																			// senão, só pode ser fundo
						fout << (unsigned char) 0;
				}
			}
		}
		return true;
	} else {
		return false;
	}
}

// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz bool informada como parametro.
void CAberturaDilatacao3D::SalvarResultadosParciaisEmDisco(TCMatriz3D<bool>* &mat, string fileName){
	if (salvarResultadosParciais) {
		cout << "-->Salvando imagem " << fileName.c_str() << endl ;
		mat->Write(fileName);
	}
}

// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz int informada como parametro.
void CAberturaDilatacao3D::SalvarResultadosParciaisEmDisco(TCMatriz3D<int>* &mat, string fileName){
	if (salvarResultadosParciais) {
		cout << "-->Salvando imagem " << fileName.c_str() << endl ;
		mat->Write(fileName);
	}
}

// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz rotulo informada como parametro.
void CAberturaDilatacao3D::SalvarResultadosParciaisEmDisco(TCRotulador3D<bool>* &mat, string fileName){
	if (salvarResultadosParciais) {
		cout << "-->Salvando imagem " << fileName.c_str() << endl ;
		mat->Write(fileName);
	}
}

// Este modelo foi copiado do Anaimp.
// Apenas acrescentei comentários e saída dos resultados (para análise visual).
// Neste modelo a área dos objetos rotulados é usada para diferenciar pixeis isolados que foram eliminados na abertura dos pixeis das ligacoes.
// Os resultados são bons apenas para poros muito bem comportados
// LP: Ao implementar a versão 3D a partir da 2D, foram realizadas modificações para economia de memória e para que pm não seja alterada.
void CAberturaDilatacao3D::DistSitiosLigacoes_Modelo_0() {
	// Variaveis auxiliares
	modelo = 0 ;

	// usada para setar nome dos arquivos de disco
	ostringstream os;

	// area das ligacoes, usada para
	double areaLigacoes;

	// porosidade da matriz das ligacoes num dado instante
	double porosidadeParcialdasLigacoes = 0.0;

	// auxiliar, usada para encerrar looping.
	double porosidadeAposAbertura =	0.0;

	cout << "Alocando imagens auxiliares..." << endl ;

	/* LP: Substituido pelo código abaixo para economizar memória
	// Novo, para salvar resultado final em disco cria MatrizSitiosLigacoes
	TCMatriz3D<int>* MatrizSitiosLigacoes = new TCMatriz3D<int>( pm->NX(), pm->NY(), pm->NZ() ); // cópia da matriz inicial
	MatrizSitiosLigacoes->Constante( 0 ); // Em matriz int o fundo será sempre zero.
	MatrizSitiosLigacoes->SetFormato( D2_X_Y_Z_GRAY_ASCII );
	MatrizSitiosLigacoes->NumCores ( 3 ); // 3 cores, fundo = 0,  ligacoes = 1, sitios = 2
	*/
	TCMatriz3D<bool>* matrizSitios = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );
	matrizSitios->SetFormato( D1_X_Y_Z_ASCII );

	TCMatriz3D<bool>* matrizLigacoes = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );
	matrizLigacoes->SetFormato( D1_X_Y_Z_ASCII );

	// 	Copia da matrizInicial
	TCMatriz3D<bool>* matrizInicial = new TCMatriz3D<bool>( *pm  ); // cópia da matriz pm

	// Representa a matrizInicial no passo anterior (antes da abertura-dilatacao)
	TCMatriz3D<bool> matrizInstanteAnterior( pm->NX(), pm->NY(), pm->NZ() );

	// Troquei pela linha abaixo para poder resolver o problema de referencia no metodo funcaoPorosidade
	// Armazena o conteúdo de matrizInicial em cada passo para poder restaurá-la posteriormente.
	TCMatriz3D<bool>* matrizAbertura = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );

	cout << "Alocando vetores distribuicao..." << endl ;
	CVetor* distribuicaoTotalPoros	= new CVetor( (pm->NX()-1)/2+1 ); // dimensao igual a do raio que zera a imagem abertura + 1
	CVetor* distribuicaoLigacoes		= new CVetor( (pm->NX()-1)/2+1 );
	CVetor* distribuicaoSitios			= new CVetor( (pm->NX()-1)/2+1 );

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
	porosidade = Porosidade( matrizInicial );
	cout << " = " << porosidade << endl ;

	// Cria vetor para area dos objetos mapeados
	// usado para diferenciar ???
	CVetor areaObjetosMapeados( pm->NX() * pm->NY() / 4 );

	// LOOOPING de calculo
	cout << "Entrando no looping de calculo das distribuicoes..." << endl ;
	for ( int raioEE = 1; raioEE <= ( pm->NX() - 1 ) / 2; raioEE += incrementoRaioElementoEstruturante ) {
		cout << "================================>RAIO Elemento Estruturante = " <<  raioEE << endl ;
		// COPIA VALORES de matrizInicial PARA matrizInstanteAnterior
		cout << "-->Inicializando matrizInstanteAnterior..." << endl ;
		// Testar sobrecarga operator= para substituir o código abaixo: *matrizInstanteAnterior = *pm;
		for (int i = 0; i < pm->NX(); i++)
			for (int j = 0; j < pm->NY(); j++)
				for (int k = 0; k < pm->NZ(); k++)
					matrizInstanteAnterior.data3D[i][j][k] = matrizInicial->data3D[i][j][k];

		// ROTULAGEM
		cout << "-->Realizando rotulagem sequencial..." << endl ;
		matrizRotulo->Go( matrizInicial ); 				// rotula matrizRotulo
		// salvar a matriz rotulo ?

		// ABERTURA
		cout << "-->Processando Abertura..." << endl ;
		pfmf->Go( matrizInicial, raioEE );
		pfmf->Abertura( matrizInicial, raioEE );

		// Salva matriz abertura
		os.str("");
		os << "MatrizAbertura_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco(matrizInicial, os.str());

		porosidadeAposAbertura = Porosidade ( matrizInicial );

		// Seta matriz abertura e calcula area dos objetos
		cout << "-->Calculando area dos objetos da matriz abertura..." << endl ;
		areaObjetosMapeados.Constante(0);
		for (int i = 0; i < pm->NX(); i++) {
			for (int j = 0; j < pm->NY(); j++) {
				for (int k = 0; k < pm->NZ(); k++){
					// Atualiza matrizAbertura
					// mudar realizar abertura na matriz abertura.
					matrizAbertura->data3D[i][j][k] = matrizInicial->data3D[i][j][k];
					if ( matrizAbertura->data3D[i][j][k] == INDICE)  // se o pixel esta na matriz abertura, pegue seu rotulo
						areaObjetosMapeados.data1D[ matrizRotulo->data3D[i][j][k] ] ++; // e acumule a area deste rotulo
				}
			}
		}

		// DILATACAO
		cout << "-->Processando dilatação..." << endl ;
		pfmf->Go ( matrizInicial, raioEE/fatorReducaoRaioElemEst);
		pfmf->Dilatacao( matrizInicial, raioEE/fatorReducaoRaioElemEst );

		// Salva matriz dilatacao
		os.str("");
		os << "MatrizAberturaDilatacao_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizInicial, os.str() );

		// Critério de parada
		if ( porosidadeAposAbertura == 0  ) {
			cout << "Criterio de parada: porosidadeAposAbertura == 0" << endl;
			break;
		} else if ( raioEE > raioMaximoElementoEstruturante ) { // bug para imagens com objetos muito grandes
			cout << "Criterio de parada: raioEE >  raioMaximoElementoEstruturante " << endl;
			break;
		}

		// Area ligaçoes
		areaLigacoes = 0;
		// Percorre a matriz AberturaDilatacao e determina ligacoes
		for ( int  i = 0; i < pm->NX(); i++ ) {
			for ( int j = 0; j < pm->NY(); j++ ) {
				for ( int k = 0; k < pm->NZ(); k++ ) {
					// Se o pixel existe na matrizInstanteAnterior,
					if ( ( matrizInstanteAnterior.data3D[i][j][k] == INDICE )
							 // e nao existe na matriz aberturaDilatacao pm
							 && ( matrizInicial->data3D[i][j][k] == FUNDO )
							 // e area do objeto > 0; entao e pixel de ligaçao (não é poro eliminado)
							 && ( areaObjetosMapeados.data1D[ matrizRotulo->data3D[i][j][k] ] > 0) )
					{
						areaLigacoes++;   		// acumula cada pixel de ligação e anota na matrizLigacoes
						matrizLigacoes->data3D[i][j][k] = INDICE;       // o armazenamento dos pontos das ligações só serve para visualização
					}
				}
			}
		}

		// Salva matriz ligacoes em disco
		os.str("");
		os << "MatrizLigacoes_" << raioEE << ".dgm";
		SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

		// DISTRIBUIÇOES
		cout << "-->Calculando distribuicoes acumuladas..." << endl ;
		// Porosidade eliminada / porosidade, dá a distribuicao acumulada pois a porosidade eliminada é acumulada
		distribuicaoTotalPoros->data1D[ raioEE ] = ( ( porosidade - Porosidade ( matrizAbertura ) ) / porosidade ) * 100;
		// calcula porosidade das ligacoes
		porosidadeParcialdasLigacoes = areaLigacoes / ( pm->NX() * pm->NY() * pm->NZ() );
		// porosidade ligacoes neste passo mais porosidade ligacoes passo anterior
		distribuicaoLigacoes->data1D[ raioEE ] =  porosidadeParcialdasLigacoes / porosidade * 100
																									+ distribuicaoLigacoes->data1D[ raioEE - 1 ];
		// esta na total e não esta na ligacao, então é sitio
		distribuicaoSitios->data1D[ raioEE ] = distribuicaoTotalPoros->data1D[ raioEE ] - distribuicaoLigacoes->data1D[ raioEE ];

		// matrizInicial sofreu dilatação, precisa voltar a ser matrizAbertura
		// garante que a próxima abertura parte da atual
		for (int  i = 0; i < pm->NX(); i++) {
			for (int  j = 0; j < pm->NY(); j++) {
				for (int k = 0; k < pm->NZ(); k++) {
					matrizInicial->data3D[i][j][k] = matrizAbertura->data3D[i][j][k];
				}
			}
		}
	}
	cout << "-->Encerrado looping." << endl ;

	cout << "Salvando distribuicoes..." << endl ;

	//Salvar(distribuicaoLigacoes,"distLigacoes.dl");
	distribuicaoLigacoes->SetFormato(V1_X_ASCII);
	distribuicaoLigacoes->Write("distLigacoes.dl");
	cout << "Salvou distLigacoes.dl..." << endl ;

	//Salvar(distribuicaoSitios,"distSitios.dp");
	distribuicaoSitios->SetFormato(V1_X_ASCII);
	distribuicaoSitios->Write("distSitios.dp");
	cout << "Salvou distSitios.dp..." << endl ;

	//Salvar(distribuicaoTotalPoros,"distTotalPoros.dtp");
	distribuicaoTotalPoros->SetFormato(V1_X_ASCII);
	distribuicaoTotalPoros->Write("distTotalPoros.dtp");
	cout << "Salvou distTotalPoros.dtp..." << endl ;

	// matrizRotulo->SetFormato( D2_X_Y_Z_GRAY_ASCII );
	// matrizRotulo->NumCores ( matrizRotulo->NumeroObjetos() );
	// matrizRotulo->Write("MatrizRotulo.dgm");

	// Gerando imagem final e salvando em disco
	// Apenas para visualizacao do resultado final
	// Se esta na matriRotulo e nao foi marcado como ligacao, entao é sitio
	for ( int i = 0; i < pm->NX(); i++) {
		for ( int j = 0; j < pm->NY(); j++) {
			for ( int k = 0; k < pm->NZ(); k++) {
				// É poro (esta em pm) e não é  ligação, então é  sítio
				if ( pm->data3D[i][j][k] == INDICE // é poro
						 and matrizLigacoes->data3D[i][j][k] != INDICE ) // não é ligacao
					matrizSitios->data3D[i][j][k] = INDICE; // então é sitio.
			}
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
		 << ".dgm";
	cout << "-->Salvando imagem " << os.str().c_str() << endl;
	Write( os.str(), matrizLigacoes, matrizSitios );

	delete matrizSitios;
	delete matrizLigacoes;
	delete matrizInicial;
	delete matrizAbertura;
}


// O primeiro modelo é o mais simples
// Realiza abertura, realiza dilatação, e marca como ligação o que esta na imagem pm e não esta na imagem dilatada.
// Depois, determina aŕea de sitios e ligações.
// Este modelo desconsidera fato de que poros pequenos, isolados, serão contabilizados como ligações.
// Não usa rotulagem.
// A diferença para DistSitiosLigacoes_Modelo_1_old_usaCVetor é que usa distribuições em vector<double>
// Ao implementar a versão 3D a partir da 2D, foram realizadas modificações para economia de memória e para que pm não seja alterada.
void CAberturaDilatacao3D::DistSitiosLigacoes_Modelo_1() {
	// Variaveis auxiliares
	modelo = 1 ;

	// usada para setar nome dos arquivos de disco
	ostringstream os;

	// Cria matrizPoros e deixa vazia
	cout << "Criando e inicializando matrizPoros..." << endl;
	TCMatriz3D<bool>* matrizPoros = new TCMatriz3D<bool>(pm->NX(), pm->NY(), pm->NZ());
	matrizPoros->Constante(FUNDO);
	matrizPoros->SetFormato( D1_X_Y_Z_ASCII );
	//matrizPoros->Write("matrizPoros_inicial.dbm");

	// Cria matrizLigacoes e deixa vazia
	cout << "Criando e inicializando matrizLigacoes..." << endl;
	TCMatriz3D<bool>* matrizLigacoes = new TCMatriz3D<bool>(pm->NX(), pm->NY(), pm->NZ());
	matrizLigacoes->Constante(FUNDO);
	matrizLigacoes->SetFormato( D1_X_Y_Z_ASCII );
	//matrizLigacoes->Write("matrizLigacoes_inicial.dbm");

	// Cria matrizSitios cópia da matriz pm inicial (durante o processo vai apagar)
	cout << "Criando e inicializando matrizSitios..." << endl;
	TCMatriz3D<bool>* matrizSitios = new TCMatriz3D<bool>( *pm );
	matrizSitios->SetFormato( D1_X_Y_Z_ASCII );
	//matrizSitios->Write("matrizSitios_inicial.dbm");

	// Cria matrizInicial, cópia de pm
	cout << "Criando matrizInicial (cópia de pm)..." << endl;
	TCMatriz3D<bool>* matrizInicial = new TCMatriz3D<bool>( *pm );
	//matrizInicial->Write("matrizInicial.dbm");

	cout << "Rotulando matrizInicial..." << endl;
	// Rotula matrizRotulo usando pm
	matrizRotulo->Go( matrizInicial );

	// Cria matrizAbertura, é a imagem apos abertura
	cout << "Criando e inicializando matrizAbertura..." << endl;
	TCMatriz3D<bool>* matrizAbertura= new TCMatriz3D<bool>(pm->NX(), pm->NY(), pm->NZ());

	// Cria vetor VPoros
	cout << "Criando e inicializando VPoros..." << endl;
	vector<double> VPoros ( ((pm->NX()-1)/2+1) , 0.0); // inicialia com zero

	// Cria vetor VSitios
	cout << "Criando e inicializando VSitios..." << endl;
	vector<double> VSitios ( ((pm->NX()-1)/2+1) , 0.0); // inicialia com zero

	// Cria vetor VLigacoes
	cout << "Criando e inicializando VLigacoes..." << endl;
	vector<double> VLigacoes ( ((pm->NX()-1)/2+1) , 0.0); // inicialia com zero

	// Entra num looping para o raio do elemento estruturante
	cout << "Entrando no looping de calculo das distribuicoes..." << endl ;
	for ( int raioEE = 1; raioEE <= (pm->NX()-1)/2; raioEE += incrementoRaioElementoEstruturante  )	{
		cout << "==>RAIO Elemento Estruturante = " <<  raioEE << endl ;

		// ABERTURA
		cout << "-->Processando Abertura..." << endl ;
		pfmf->Go (matrizInicial, raioEE);
		pfmf->Abertura(matrizInicial,raioEE);

		// Calcula porosidade apos abertura, vai ser usada no final como criterio de parada.
		porosidade = Porosidade ( matrizInicial );
		cout << "-->Porosidade = " << porosidade << endl;

		// Criterio de parada; temporario, melhorar o criterio de saida
		// note que como nao processa a borda sempre vai ter porosidade, ou seja, acrescente uma borda na imagem.
		if ( porosidade == 0 or raioEE >= raioMaximoElementoEstruturante ) // bug nao processa raios maiores, usar raioMaximoAbertura
			break;

		os.str("");
		os << "MatrizAbertura_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizInicial, os.str() );

		// Copia o que sobrou em pm para matrizAbertura (matriz abertura)
		// Copia o que foi eliminado na abertura para matriz de poros
		cout << "-->Setando matrizAbertura..." << endl ;
		cout << "-->Setando matrizPoros..." << endl ;
		for ( int i = 0; i < pm->NX(); i++ ) {
			for ( int j = 0; j < pm->NY(); j++ ) {
				for ( int k = 0; k < pm->NZ(); k++ ) {
					matrizAbertura->data3D[i][j][k] = matrizInicial->data3D[i][j][k];
					// Se esta na imagem original e não esta apos abertura, então é pixel/poro eliminado
					if ( pm->data3D[i][j][k] == INDICE and matrizInicial->data3D[i][j][k] == FUNDO )
						matrizPoros->data3D[i][j][k] = INDICE;
				}
			}
		}

		// DILATACAO
		cout << "-->Processando dilatação..." << endl ;
		pfmf->Go( matrizInicial, raioEE/fatorReducaoRaioElemEst );
		pfmf->Dilatacao( matrizInicial, raioEE/fatorReducaoRaioElemEst );

		os.str("");
		os << "MatrizAberturaDilatacao_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizInicial, os.str() );

		// Zera ligacoes. Necessario, pois para diferentes aberturas, teremos diferentes áreas das ligacoes
		matrizLigacoes->Constante(0);

		// Seta pixeis de matrizSitios e matrizLigacoes
		cout << "-->Setando matrizSitios e matrizLigacoes..." << endl ;
		for ( int i = 0; i < pm->NX(); i++ ) {
			for ( int j = 0; j < pm->NY(); j++ ) {
				for ( int k = 0; k < pm->NZ(); k++ ) {
					// Se esta em matrizPoros e não esta em matrizInicial, entao é ligacao
					if ( matrizPoros->data3D[i][j][k] == INDICE and matrizInicial->data3D[i][j][k] == FUNDO ) {
						matrizLigacoes->data3D[i][j][k] = INDICE;
						matrizSitios->data3D[i][j][k] = FUNDO; // apaga na imagem dos sitios
					}
				}
			}
		}

		// 	Calcula o numero de pixeis de cada matriz
		cout << "-->Calculando o numero de pixeis de cada matriz..." << endl ;
		for ( int i = 0; i < pm->NX(); i++ ) {
			for ( int j = 0; j < pm->NY(); j++ ) {
				for ( int k = 0; k < pm->NZ(); k++ ) {
					// 	Calcula VSitios[raioee]
					if ( matrizSitios->data3D[i][j][k] == INDICE)
						VSitios[ raioEE ]++;
					// 	Calcula VLigacoes[raioee]
					if ( matrizLigacoes->data3D[i][j][k] == INDICE)
						VLigacoes[ raioEE ]++;
					// 	Calcula VPoros[raioee]
					// O numero de sitios de VPoros pode ser calculado por
					// VPoros[raioEE] = VSitios[raioEE] + VLigacoes[raioEE];
					// ou por
					if ( matrizPoros->data3D[i][j][k] == INDICE)
						VPoros[ raioEE ]++;
				}
			}
		}

		os.str("");
		os << "MatrizPoros_"  << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizPoros, os.str() );

		os.str("");
		os  << "MatrizSitios_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

		os.str("");
		os << "MatrizLigacoes_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

		// Como fez dilatacao extra, precisa retornar matrizInicial para matrizAbertura
		cout << "-->Como fez dilatacao extra, precisa retornar a imagem matrizInicial a original..." << endl ;
		for ( int i = 0; i < pm->NX(); i++ ) {
			for ( int j = 0; j < pm->NY(); j++ ) {
				for ( int k = 0; k < pm->NZ(); k++ ) {
					matrizInicial->data3D[i][j][k] = matrizAbertura->data3D[i][j][k];
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

	// Salva MatrizSitiosLigacoes em disco
	os.str("");
	os << "MatrizSitiosLigacoes"
		 << "-Modelo-" 	<< modelo
		 << "-rmee-" 	<< raioMaximoElementoEstruturante
		 << "-frree-"	<< fatorReducaoRaioElemEst
		 << "-iree-"	<< incrementoRaioElementoEstruturante
		 << "-" << nomeImagem
		 << ".dgm";

	cout << "-->Salvando imagem " << os.str().c_str() << endl ;
	Write( os.str(), matrizLigacoes, matrizSitios );

	delete matrizPoros;
	delete matrizSitios;
	delete matrizLigacoes;
	delete matrizAbertura;
}

// Modelo que usa abertura, e matrizes rotuladas para identificar as ligações.
// Método totalmente novo, desenvolvido em 2010 e portado para 3D em 2012.
// Utiliza muita memória (TCMatriz3D<int>).
// Ao implementar a versão 3D a partir da 2D, foram realizadas modificações para economia de memória.
void CAberturaDilatacao3D::DistSitiosLigacoes_Modelo_2() {
	// Variaveis auxiliares
	modelo = 2;

	// usada para setar nome dos arquivos de disco
	ostringstream os;

	//	int numeroObjetosAcumulados = 0 ;
	int nObjetosAntesAbertura = 0 ;			// numeroObjetosAntesAbertura
	int nObjetosDepoisAbertura = 0 ;		// nObjetosDepoisAbertura
	int nObjetosDepoisAberturaComplementar = 0 ;	// nObjetosDepoisAberturaComplementar

	// Modificado: Utilizando matrizSitios e matrizLigacoes para economizar memória
	// Cria MSitiosLigacoes e deixa vazia (apenas para visualizacao das ligacoes - eliminar depois)
	//cout << "Criando e inicializando matrizLigacoes..." << endl;
	//TCMatriz3D<int>* MSitiosLigacoes = new TCMatriz3D<int>( pm->NX(), pm->NY(), pm->NZ() );
	//MSitiosLigacoes->Constante(0);
	//MSitiosLigacoes->SetFormato( D2_X_Y_Z_GRAY_ASCII );
	//MSitiosLigacoes->NumCores ( 6 );

	// Cria matrizSitios vazia (apenas para visualizacao - eliminar depois)
	cout << "Criando e inicializando matrizSitios..." << endl;
	TCMatriz3D<bool>* matrizSitios = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );
	matrizSitios->Constante(FUNDO);
	matrizSitios->SetFormato( D1_X_Y_Z_ASCII );

	// Cria matrizLigacoes vazia (apenas para visualizacao - eliminar depois)
	cout << "Criando e inicializando Mligacoes..." << endl;
	TCMatriz3D<bool>* matrizLigacoes = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );
	matrizLigacoes->Constante(FUNDO);
	matrizLigacoes->SetFormato( D1_X_Y_Z_ASCII );

	// Cria MInicialRotulada, é a imagem inicial rotulada
	// a cada etapa/raio/passo, é onde são armazenados os rótudos dos objetos identificados
	cout << "Rotulando a imagem inicial..." << endl;
	matrizRotulo->Go( pm );
	matrizRotulo->SetFormato( D2_X_Y_Z_GRAY_ASCII );
	nObjetosAntesAbertura = matrizRotulo->NumeroObjetos();
	cout << "-->nObjetosAntesAbertura =..." 	<< nObjetosAntesAbertura << endl ;

	cout << "Criando e inicializando MInicialRotulada - tons de cinza..." << endl;
	TCMatriz3D<int>* MInicialRotulada = new TCMatriz3D<int>( *matrizRotulo );
	MInicialRotulada->SetFormato( D2_X_Y_Z_GRAY_ASCII );
	MInicialRotulada->NumCores ( matrizRotulo->NumeroObjetos() ); // 256, numero objetos informa o maior rotulo utilizado.

	SalvarResultadosParciaisEmDisco( MInicialRotulada, "MatrizInicialRotulada.dgm" );

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

	//	Cria vetor de objetos
	//	No início todos os objetos são marcados como fundo (verificar se esta ok)
	vector< CObjetoImagem > Objeto ( matrizRotulo->NumeroObjetos() , CObjetoImagem( SOLIDO , 0 )); 	// inclui o objeto fundo (0)
	Objeto.reserve ( 100 *  matrizRotulo->NumeroObjetos() ); 		// evita realocacoes

	// Seta o Rotulo e o Tipo dos objetos da imagem inicial
	// Seta o fundo
	Objeto[0].Rotulo( 0 );
	Objeto[0].Tipo( SOLIDO );

	// Seta os demais objetos como sendo PORO
	for ( int i = 1; i < Objeto.size(); i++ ) {
		Objeto[i].Rotulo( i );
		Objeto[i].Tipo( PORO );
	}

	// Entra num looping para o raio do elemento estruturante
	// o incremento é dado pelo incrementoRaioElementoEstruturante
	cout << "Entrando no looping de calculo das distribuicoes..." << endl ;
	for ( int raioEE = 1; raioEE <= (pm->NX()-1)/2; raioEE += incrementoRaioElementoEstruturante  ) {
		cout << "==>RAIO Elemento Estruturante = " <<  raioEE << endl ;
		cout << "-->nObjetosAntesAbertura =..." 	<< nObjetosAntesAbertura << endl ;

		cout << "-->Processando Abertura..." << endl ;
		pfmf->Go( pm, raioEE );
		pfmf->Abertura( pm , raioEE );

		// Calcula porosidade apos abertura, vai usar como criterio de parada.
		porosidade = Porosidade ( pm );
		cout << "-->Porosidade = " << porosidade << endl;

		// Criterio de parada; temporario, melhorar o criterio de parada/saida.
		// note que como nao processa a borda sempre vai ter porosidade na borda, ou seja, acrescente uma borda na imagem.
		// ou modifique o critério de saída.
		if ( porosidade == 0.0 or raioEE >= raioMaximoElementoEstruturante )
			break;

		os.str("");
		os << "MatrizAbertura_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( pm, os.str() );

		// Rotula a imagem abertura
		cout << "-->Rotulando matriz pm apos abertura " << endl;
		matrizRotulo->Go( pm ); 		// matrizRotulo
		matrizRotulo->NumCores ( matrizRotulo->NumeroObjetos() ); // 256, numero objetos informa o maior rotulo utilizado.
		os.str("");
		os << "MatrizRotuloAposAbertura_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizRotulo, os.str() );

		// Marca o numero de objetos depois da abertura
		nObjetosDepoisAbertura = nObjetosAntesAbertura + matrizRotulo->NumeroObjetos();
		cout << "-->nObjetosDepoisAbertura =..." 	<< nObjetosDepoisAbertura << endl ;

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
		for ( int i = 0; i < pm->NX(); i++ ) {
			for ( int j = 0; j < pm->NY(); j++ ) {
				for ( int k = 0; k < pm->NZ(); k++ ) {
					// Se faz parte abertura; então copia para matriz inicial rotulada (copia rotulo dos sitios)
					// MInicialRotulada terá os rótulos iniciais e os rotulos da imagem abertura acrescidos do número de objetos antes da abertura
					// Isso precisa se feito pois o rotulador não mais permite informar o rótulo inicial.
					if ( pm->data3D[i][j][k] == INDICE )
						MInicialRotulada->data3D[i][j][k] = matrizRotulo->data3D[i][j][k] + nObjetosAntesAbertura;

					// Se é Poro, ou seja, MInicialRotulada->data3D[i][j][k] > 0
					// entao marca como abertura complementar (inverte valores de pm).
					if ( MInicialRotulada->data3D[i][j][k] > 0 ) {
						if ( pm->data3D[i][j][k] == FUNDO ) {
							pm->data3D[i][j][k] = INDICE;
						} else {
							pm->data3D[i][j][k] = FUNDO;
						}
					}
				}
			}
		}

		os.str("");
		os << "MatrizAberturaComplementar_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( pm, os.str() );

		// Rotulagem da matriz abertura complementar
		cout << "-->Processando rotulagem da matriz abertura complementar..." << endl ;
		matrizRotulo->Go( pm );
		nObjetosDepoisAberturaComplementar = nObjetosDepoisAbertura + matrizRotulo->NumeroObjetos();
		cout << "-->nObjetosDepoisAberturaComplementar =..."<< nObjetosDepoisAberturaComplementar << endl ;

		os.str("");
		os << "MatrizAberturaComplementarRotulada_" << raioEE << ".dgm";
		matrizRotulo->NumCores ( matrizRotulo->NumeroObjetos() );
		SalvarResultadosParciaisEmDisco( matrizRotulo, os.str() );

		// Adiciona objetos rotulados a lista de objetos -  são RAMOs_MORTOs
		// depois iremos identificar quais ramos_mortos são ligações
		for ( int i = nObjetosDepoisAbertura; i < nObjetosDepoisAberturaComplementar; i++ ) {
			Objeto.push_back( CObjetoImagem( RAMO_MORTO, i ) );
		}

		// Copia imagem abertura complementar rotulada para matriz inicial rotulada.
		// Para cada rótulo da imagem abertura complementar acrescenta o
		// nObjetosDepoisAbertura de forma que os rotulos sejam sequenciais.
		for ( int i = 0; i < pm->NX(); i++ ) {
			for ( int j = 0; j < pm->NY(); j++ ) {
				for ( int k = 0; k < pm->NZ(); k++ ) {
					// Se não é fundo, copia para matriz inicial rotulada
					if ( pm->data3D[i][j][k] == INDICE ) {
						MInicialRotulada->data3D[i][j][k] = matrizRotulo->data3D[i][j][k] + nObjetosDepoisAbertura;
					}
				}
			}
		}

		// Percorre a imagem MInicialRotulada e faz as conecções.
		// Identifica os rótulos dos vizinhos e faz a conexão se o rotulo já não tiver sido incluido
		// lembre-se que set não tem repeticao, e sConeccao é do tipo set<int>.
		// Desconsideramos os pixeis da borda para evitar bugs.
		// Aqui a MInicialRotulada tem SOLIDO, POROs, SITIOs e RAMOs_MORTOs
		int dimensaoBorda = 1;
		int rotuloPixel_i_j_k;  // auxiliar
		for ( int i = dimensaoBorda; i < pm->NX() - dimensaoBorda; i++) {
			for ( int j = dimensaoBorda; j < pm->NY() - dimensaoBorda; j++) {
				for ( int k = dimensaoBorda; k < pm->NZ() - dimensaoBorda; k++) {
					rotuloPixel_i_j_k	 =  MInicialRotulada->data3D[i][j][k];
					// Só devemos considerar os rotulos da imagem abertura, i.e,  rotulo >= nObjetosAntesAbertura
					if ( rotuloPixel_i_j_k	>= nObjetosAntesAbertura ) {
						// Se os rotulos são diferentes e fazem parte da matriz abertura
						// (rotulo >= nObjetosAntesAbertura) então marca a coneccão.
						if ( rotuloPixel_i_j_k != MInicialRotulada->data3D[i][j-1][k]
								 and MInicialRotulada->data3D[i][j-1][k] >= nObjetosAntesAbertura)
							// Como a lista de coneccões é do tipo set, só marca uma vez.
							Objeto[rotuloPixel_i_j_k].Conectar( MInicialRotulada->data3D[i][j-1][k] );

						if ( rotuloPixel_i_j_k != MInicialRotulada->data3D[i][j+1][k]
								 and MInicialRotulada->data3D[i][j+1][k]  >= nObjetosAntesAbertura)
							Objeto[rotuloPixel_i_j_k].Conectar( MInicialRotulada->data3D[i][j+1][k] );

						if ( rotuloPixel_i_j_k != MInicialRotulada->data3D[i-1][j][k]
								 and MInicialRotulada->data3D[i-1][j][k]  >= nObjetosAntesAbertura)
							Objeto[rotuloPixel_i_j_k].Conectar( MInicialRotulada->data3D[i-1][j][k] );

						if ( rotuloPixel_i_j_k != MInicialRotulada->data3D[i+1][j][k]
								 and MInicialRotulada->data3D[i+1][j][k]  >= nObjetosAntesAbertura)
							Objeto[rotuloPixel_i_j_k].Conectar( MInicialRotulada->data3D[i+1][j][k] );

						if ( rotuloPixel_i_j_k != MInicialRotulada->data3D[i][j][k-1]
								 and MInicialRotulada->data3D[i][j][k-1]  >= nObjetosAntesAbertura)
							Objeto[rotuloPixel_i_j_k].Conectar( MInicialRotulada->data3D[i][j][k-1] );

						if ( rotuloPixel_i_j_k != MInicialRotulada->data3D[i][j][k+1]
								 and MInicialRotulada->data3D[i][j][k+1]  >= nObjetosAntesAbertura)
							Objeto[rotuloPixel_i_j_k].Conectar( MInicialRotulada->data3D[i][j][k+1] );
					}
				}
			}
		}
		// Agora vamos percorrer os objetos anotados como RAMOs_MORTOs e identificar as ligações
		// Percorre apenas os rotulos marcados como RAMOs_MORTOs.
		for ( int i = nObjetosDepoisAbertura ; i < nObjetosDepoisAberturaComplementar; i++ )
			// Se o numero de conecções for maior que 1, então é ligação
			if ( Objeto[i].SConeccao().size() > 1 )
				Objeto[i].Tipo( LIGACAO );

		// Agora, todos o objetos foram anotados, temos SOLIDO, POROs, SITIOs, RAMOs_MORTOs e LIGACOES.
		// vamos armazenar resultado na MSitiosLigacoes e salvar resultado parcial em disco.
		for ( int i = 0; i < pm->NX(); i++) {
			for ( int j = 0; j < pm->NY(); j++) {
				for ( int k = 0; k < pm->NZ(); k++) {
					// pm armazena a abertura complementar, tem RAMOs_MORTOs e LIGACOES
					// Sé é ramo ou ligação
					if ( pm->data3D[i][j][k] == INDICE ) {
						// Se o numero de coneccoes >1, então é ligação
						if ( Objeto[ MInicialRotulada->data3D[i][j][k] ].Tipo() == LIGACAO )
							matrizLigacoes->data3D[i][j][k] = INDICE;
					}
				}
			}
		}

		// Precisamos fazer com que pm volte a ser a matriz abertura
		for ( int i = 0; i < pm->NX(); i++) {
			for ( int j = 0; j < pm->NY(); j++) {
				for ( int k = 0; k < pm->NZ(); k++) {
					int rot = MInicialRotulada->data3D[i][j][k];
					// Se maior ou igual a nObjetosAntesAbertura
					if ( rot  >= nObjetosAntesAbertura and rot < nObjetosDepoisAbertura ) {
						pm->data3D[i][j][k] = INDICE;
					} else {
						pm->data3D[i][j][k] = FUNDO;
					}
				}
			}
		}

		os.str("");
		os << "MatrizPmNoFinalDoLoop_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( pm, os.str() );

		// Atualiza o numero de objetos antes da abertura (i.e. da rotulagem anterior)
		nObjetosAntesAbertura = nObjetosDepoisAberturaComplementar;
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
			for ( int k = 0; k < pm->NZ(); k++) {
				//TEMPORARIO, verificar porque código abaixo (comentado) Não funciona?
				// Se não é fundo e não é ligação
				if ( MInicialRotulada->data3D[i][j][k] > 0 and matrizLigacoes->data3D[i][j][k] != INDICE ) {
					matrizSitios->data3D[i][j][k] = INDICE;
				}
				/*		if( MInicialRotulada->data3D[i][j][k] > 0 )
									{
									tipoObjeto = Objeto[ MInicialRotulada->data3D[i][j][k] ].Tipo() ;
									// SOLIDO = 0, PORO = 1, SITIO = 2, LIGACAO = 3, RAMO_MORTO = 4, NAO_IDENTIFICADO = 5
									// Se o numero de coneccoes >1, então é ligação
									if ( tipoObjeto == SOLIDO )
										MSitiosLigacoes->data3D[i][j][k] = SOLIDO;
									else if ( tipoObjeto  == PORO )
										MSitiosLigacoes->data3D[i][j][k] = PORO;
									else if ( tipoObjeto  == SITIO )
										MSitiosLigacoes->data3D[i][j][k] = SITIO;
									else if ( tipoObjeto  == LIGACAO )
										MSitiosLigacoes->data3D[i][j][k] = LIGACAO;
									else if ( tipoObjeto  == RAMO_MORTO )
										MSitiosLigacoes->data3D[i][j][k] = RAMO_MORTO;
									else if ( tipoObjeto  == NAO_IDENTIFICADO )
										MSitiosLigacoes->data3D[i][j][k] = NAO_IDENTIFICADO;
									}*/
			}
		}
	}
	// Salva MSitiosLigacoes em disco - resultado final (imagem com SOLIDO, PORO, SITIO, RAMO_MORTO, LIGACAO
	os.str("");
	os	<< "MatrizSitiosLigacoes"
			<< "-Modelo-" << modelo
			<< "-rmee-" 	<< raioMaximoElementoEstruturante
			<< "-frree-"	<< fatorReducaoRaioElemEst
			<< "-iree-"		<< incrementoRaioElementoEstruturante
			<< "-"				<< nomeImagem
			<< ".dgm";
	cout << "-->Salvando imagem " << os.str().c_str() << endl ;
	Write( os.str(), matrizSitios, matrizLigacoes );

	// Salva MInicialRotulada em disco - resultado final (rótulos dos objetos)
	// Associada ao grafo permite a identificação dos píxeis.
	os.str("");
	os << "MatrizInicialRotulada-Final"
		 << "-Modelo-" 	<< modelo
		 << "-rmee-" 	<< raioMaximoElementoEstruturante
		 << "-frree-"	<< fatorReducaoRaioElemEst
		 << "-iree-"	<< incrementoRaioElementoEstruturante
		 << "-" << nomeImagem
		 << ".dgm";
	MInicialRotulada->NumCores ( nObjetosDepoisAberturaComplementar ); // 256, numero objetos informa o maior rotulo utilizado.
	SalvarResultadosParciaisEmDisco( MInicialRotulada, os.str() );

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
		fout 	<< i << " " << Objeto[i].Tipo() << " " << Objeto[i].Rotulo() << " " << Objeto[i].SConeccao().size();
		for ( std::set<int>::iterator it = Objeto[i].SConeccao().begin(); it != Objeto[i].SConeccao().end(); it++	) {
			// indiceDasConeccoes" << endl
			fout <<  " " << *it << " ";
		}
		// Pula para linha de baixo
		fout << endl;
	}
	fout.close();

	delete matrizSitios;
	delete matrizLigacoes;
	delete MInicialRotulada;
}

/*
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
// Ao implementar a versão 3D a partir da 2D, foram realizadas modificações para economia de memória.
void CAberturaDilatacao3D::DistSitiosLigacoes_Modelo_3() {
	// Variaveis auxiliares
	modelo = 3;

	// usada para setar nome dos arquivos de disco
	ostringstream os;

	//	int numeroObjetosAcumulados = 0;
	int nObjetosAntesAbertura = 0;			// numeroObjetosAntesAbertura
	int nObjetosDepoisAbertura = 0;		// nObjetosDepoisAbertura
	int nObjetosDepoisAberturaComplementar = 0;	// nObjetosDepoisAberturaComplementar

	// Modificado: Utilizando matrizSitios e matrizLigacoes (bool) para economizar memória
	// Cria MSitiosLigacoes e deixa vazia (apenas para visualizacao das ligacoes - eliminar depois)
	//cout << "Criando e inicializando matrizLigacoes..." << endl;
	//TCMatriz3D<int>* MSitiosLigacoes = new TCMatriz3D<int>( pm->NX(), pm->NY(), pm->NZ() );
	//MSitiosLigacoes->Constante(0);
	//MSitiosLigacoes->SetFormato( D2_X_Y_Z_GRAY_ASCII );
	//MSitiosLigacoes->NumCores(6); // 256, numero objetos informa o maior rotulo utilizado.

	// Cria matrizSitios vazia (apenas para visualizacao - eliminar depois)
	cout << "Criando e inicializando matrizSitios..." << endl;
	TCMatriz3D<bool>* matrizSitios = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );
	matrizSitios->Constante(FUNDO);
	matrizSitios->SetFormato( D1_X_Y_Z_ASCII );

	// Cria matrizLigacoes vazia (apenas para visualizacao - eliminar depois)
	cout << "Criando e inicializando Mligacoes..." << endl;
	TCMatriz3D<bool>* matrizLigacoes = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );
	matrizLigacoes->Constante(FUNDO);
	matrizLigacoes->SetFormato( D1_X_Y_Z_ASCII );

	// Cria MInicialRotulada, é a imagem inicial rotulada
	// a cada etapa/raio/passo, é onde são armazenados os rótudos dos objetos identificados
	cout << "Rotulando a imagem inicial..." << endl;
	matrizRotulo->Go( pm );
	nObjetosAntesAbertura = matrizRotulo->NumeroObjetos(); //inclusive fundo!
	cout << "-->nObjetosAntesAbertura =..." << nObjetosAntesAbertura << endl ;

	cout << "Criando e inicializando MInicialRotulada - tons de cinza..." << endl;
	TCMatriz3D<int>* MInicialRotulada = new TCMatriz3D<int>( *matrizRotulo );
	MInicialRotulada->SetFormato( D2_X_Y_Z_GRAY_ASCII );
	MInicialRotulada->NumCores ( matrizRotulo->NumeroObjetos() ); // 256, numero objetos informa o maior rotulo utilizado.
	SalvarResultadosParciaisEmDisco( MInicialRotulada, "MatrizInicialRotulada.dgm" );

	////NOVO TENTAR RETIRAR POIS CONSOME MAIS MEMORIA - NOVO MODELO 3
	TCMatriz3D<bool>* MatrizPmAntesAbertura = new TCMatriz3D<bool>( *pm ); //NOVO MODELO 3

	//	Cria vetor de objetos
	//	No início todos os objetos são marcados como fundo (verificar se esta ok)
	vector< CObjetoImagem > Objeto ( matrizRotulo->NumeroObjetos() , CObjetoImagem( SOLIDO, 0 )); 	// inclui o objeto fundo (0)
	Objeto.reserve ( 100 *  matrizRotulo->NumeroObjetos() ); 		// evita realocacoes

	// Seta o Rotulo e o Tipo dos objetos da imagem inicial
	// Seta o fundo
	Objeto[0].Rotulo( 0 );
	Objeto[0].Tipo( SOLIDO );
	// Seta os demais objetos como sendo PORO
	for ( int i = 1; i < Objeto.size(); i++ ) {
		Objeto[i].Rotulo( i );
		Objeto[i].Tipo( PORO );
	}

	// Entra num looping para o raio do elemento estruturante. O incremento é dado pelo incrementoRaioElementoEstruturante
	cout << "Entrando no looping de calculo das distribuicoes..." << endl ;
	for ( int raioEE = 1; raioEE <= (pm->NX()-1)/2; raioEE += incrementoRaioElementoEstruturante ) {
		cout << "==>RAIO Elemento Estruturante = " <<  raioEE << endl;
		cout << "-->nObjetosAntesAbertura =..." 	<< nObjetosAntesAbertura << endl;

		// NOVO TENTAR RETIRAR POIS CONSOME MAIS MEMORIA - NOVO MODELO 3
		for ( int i = 0; i < pm->NX(); i++)
			for ( int j = 0; j < pm->NY(); j++)
				for ( int k = 0; k < pm->NZ(); k++)
					MatrizPmAntesAbertura->data3D[i][j][k] = pm->data3D[i][j][k];

		cout << "-->Processando Abertura..." << endl ;
		pfmf->Go( pm, raioEE );
		pfmf->Abertura( pm, raioEE );

		// Calcula porosidade apos abertura, vai usar como criterio de parada.
		porosidade = Porosidade ( pm );
		cout << "-->Porosidade = " << porosidade << endl;

		// Criterio de parada; temporario, melhorar o criterio de parada/saida.
		// note que como nao processa a borda sempre vai ter porosidade na borda, ou seja, acrescente uma borda na imagem.
		// ou modifique o critério de saída.
		if ( porosidade == 0.0 or raioEE >= raioMaximoElementoEstruturante )
			break;

		os.str("");
		os << "MatrizAbertura_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( pm, os.str() );

		// Rotula a imagem abertura
		cout << "-->Rotulando matriz pm apos abertura " << endl;
		matrizRotulo->Go( pm ); 		// matrizRotulo

		// Marca o numero de objetos depois da abertura
		nObjetosDepoisAbertura = nObjetosAntesAbertura + matrizRotulo->NumeroObjetos();
		cout << "-->nObjetosDepoisAbertura =..." 	<< nObjetosDepoisAbertura << endl ;

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
				for ( int k = 0; k < pm->NZ(); k++) {
					// Se faz parte abertura; então copia para matriz inicial rotulada (copia rotulo dos sitios)
					// MInicialRotulada terá os rótulos iniciais e os rotulos da imagem abertura acrescidos do número de objetos antes da abertura
					// Isso precisa se feito pois o rotulador não mais permite informar o rótulo inicial.
					if ( pm->data3D[i][j][k] == 1 )
						MInicialRotulada->data3D[i][j][k] = matrizRotulo->data3D[i][j][k] + nObjetosAntesAbertura;

					// Se é Poro, ie, MInicialRotulada->data3D[i][j][k] > 0 (eliminamos o fundo da imagem)
					// e não faz parte da matriz abertura, pm->data3D[i][j][k] == 0
					// entao marca como abertura complementar (inverte valores de pm).
					// ERRO: a linha abaixo estava marcando todos os ramos  mortos, inclusive os dos passos anteriores!
					// if ( MInicialRotulada->data3D[i][j][k] > 0 )
					// Precisa incluir apenas as regiões envolvidas na abertura
					if ( MatrizPmAntesAbertura->data3D[i][j][k] == INDICE ) { // esta no passo anterior a abertura // NOVO MODELO 3 usa MatrizPmAntesAbertura
						if ( pm->data3D[i][j][k] == FUNDO ) {
							pm->data3D[i][j][k] = INDICE;
						} else {
							pm->data3D[i][j][k] = FUNDO;
						}
					}
				}
			}
		}

		os.str("");
		os << "MatrizAberturaComplementar_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( pm, os.str() );

		// Rotulagem da matriz abertura complementar
		cout << "-->Processando rotulagem da matriz abertura complementar..." << endl ;
		matrizRotulo->Go( pm );

		nObjetosDepoisAberturaComplementar = nObjetosDepoisAbertura + matrizRotulo->NumeroObjetos();
		cout << "-->nObjetosDepoisAberturaComplementar =..."<< nObjetosDepoisAberturaComplementar << endl ;

		os.str("");
		os << "MatrizAberturaComplementarRotulada_" << raioEE << ".dgm";
		matrizRotulo->NumCores ( matrizRotulo->NumeroObjetos() );
		SalvarResultadosParciaisEmDisco( matrizRotulo, os.str() );

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
				for ( int k = 0; k < pm->NZ(); k++) {
					// Se não é fundo, copia para matriz inicial rotulada
					if ( pm->data3D[i][j][k] == INDICE )
						MInicialRotulada->data3D[i][j][k] = matrizRotulo->data3D[i][j][k] + nObjetosDepoisAbertura;
				}
			}
		}

		// Percorre a imagem MInicialRotulada e faz as conecções.
		// Identifica os rótulos dos vizinhos e faz a conecção se o rotulo já não tiver sido incluido
		// lembre-se que set não tem repeticao, e sConeccao é do tipo set<int>.
		// Desconsideramos os pixeis da borda para evitar bugs.
		// Aqui a MInicialRotulada tem SOLIDO, POROs, SITIOs e RAMOs_MORTOs
		int dimensaoBorda = 1;
		int rotuloPixel_i_j_k;  // auxiliar
		for ( int i = dimensaoBorda; i < pm->NX() - dimensaoBorda; i++) {
			for ( int j = dimensaoBorda; j < pm->NY() - dimensaoBorda; j++) {
				for ( int k = dimensaoBorda; k < pm->NZ() - dimensaoBorda; k++) {
					rotuloPixel_i_j_k	 =  MInicialRotulada->data3D[i][j][k];
					// Só devemos considerar os rotulos da imagem abertura, i.e,  rotulo >= nObjetosAntesAbertura
					if (  rotuloPixel_i_j_k	 >=   nObjetosAntesAbertura ) {
						// Se os rotulos são diferentes e
						// fazem parte da matriz abertura (rotulo >= nObjetosAntesAbertura) então marca a coneccão.
						if ( rotuloPixel_i_j_k != MInicialRotulada->data3D[i][j-1][k]
								 and MInicialRotulada->data3D[i][j-1][k]  >= nObjetosAntesAbertura )
							// Como a lista de coneccões é do tipo set, só marca uma vez.
							Objeto[rotuloPixel_i_j_k].Conectar( MInicialRotulada->data3D[i][j-1][k] );

						if ( rotuloPixel_i_j_k != MInicialRotulada->data3D[i][j+1][k]
								 and MInicialRotulada->data3D[i][j+1][k]  >= nObjetosAntesAbertura )
							Objeto[rotuloPixel_i_j_k].Conectar( MInicialRotulada->data3D[i][j+1][k] );

						if ( rotuloPixel_i_j_k != MInicialRotulada->data3D[i-1][j][k]
								 and MInicialRotulada->data3D[i-1][j][k]  >= nObjetosAntesAbertura )
							Objeto[rotuloPixel_i_j_k].Conectar( MInicialRotulada->data3D[i-1][j][k] );

						if ( rotuloPixel_i_j_k != MInicialRotulada->data3D[i+1][j][k]
								 and MInicialRotulada->data3D[i+1][j][k]  >= nObjetosAntesAbertura )
							Objeto[rotuloPixel_i_j_k].Conectar( MInicialRotulada->data3D[i+1][j][k] );

						if ( rotuloPixel_i_j_k != MInicialRotulada->data3D[i][j][k-1]
								 and MInicialRotulada->data3D[i][j][k-1]  >= nObjetosAntesAbertura)
							Objeto[rotuloPixel_i_j_k].Conectar( MInicialRotulada->data3D[i][j][k-1] );

						if ( rotuloPixel_i_j_k != MInicialRotulada->data3D[i][j][k+1]
								 and MInicialRotulada->data3D[i][j][k+1]  >= nObjetosAntesAbertura)
							Objeto[rotuloPixel_i_j_k].Conectar( MInicialRotulada->data3D[i][j][k+1] );
					}
				}
			}
		}
		// Agora vamos percorrer os objetos anotados como RAMOs_MORTOs e identificar as ligações
		// Percorre apenas os rotulos marcados como RAMOs_MORTOs.
		for ( int i = nObjetosDepoisAbertura ; i < nObjetosDepoisAberturaComplementar; i++ ) {
			// No modelo 2:
			// Se o numero de conecções for maior que 1, então é ligação.
			if ( Objeto[i].SConeccao().size() > 1 )
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
			// 		for( std::set<int>::iterator it = Objeto[i].SConeccao().begin(); it != Objeto[i].SConeccao().end(); it++ )
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
				for ( int k = 0; k < pm->NZ(); k++) {
					// pm armazena a abertura complementar, tem RAMOs_MORTOs e LIGACOES
					// Sé é ramo ou ligação
					if ( pm->data3D[i][j][k] == INDICE ) {
						// Se o numero de coneccoes >1, então é ligação
						if ( Objeto[ MInicialRotulada->data3D[i][j][k] ].Tipo() == LIGACAO )
							matrizLigacoes->data3D[i][j][k] = INDICE;
					}
				}
			}
		}
		// Salva MSitiosLigacoes em disco
		os.str("");
		os << "MatrizSitios"
			 << "-Modelo-" 	<< modelo
			 << "-rmee-"		<< raioMaximoElementoEstruturante
			 << "-frree-"		<< fatorReducaoRaioElemEst
			 << "-iree-"		<< incrementoRaioElementoEstruturante
			 << "-ree-"			<< raioEE
			 << "-"					<< nomeImagem
			 << ".dgm";
		SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

		// Precisamos fazer com que pm volte a ser a matriz abertura
		for ( int i = 0; i < pm->NX(); i++) {
			for ( int j = 0; j < pm->NY(); j++) {
				for ( int k = 0; k < pm->NZ(); k++) {
					int rot = MInicialRotulada->data3D[i][j][k];
					// Se maior ou igual a nObjetosAntesAbertura
					if ( rot  >= nObjetosAntesAbertura and rot < nObjetosDepoisAbertura ) {
						pm->data3D[i][j][k] = INDICE;
					} else {
						pm->data3D[i][j][k] = FUNDO;
					}
				}
			}
		}

		os.str("");
		os << "MatrizPmNoFinalDoLoop_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( pm, os.str() );

		// Atualiza o numero de objetos antes da abertura (i.e. da rotulagem anterior)
		nObjetosAntesAbertura = nObjetosDepoisAberturaComplementar;
	}// fim do looping

	// Seta MSitiosLigacoes com resultado final
	// ETipoObjetoImagem tipoObjeto ;
	for ( int i = 0; i < pm->NX(); i++) {
		for ( int j = 0; j < pm->NY(); j++) {
			for ( int k = 0; k < pm->NZ(); k++) {
				//TEMPORARIO, verificar porque código abaixo (documentado) Não funciona?
				// Se não é fundo e não é ligação
				if ( MInicialRotulada->data3D[i][j][k] > 0 and matrizLigacoes->data3D[i][j][k] != INDICE ) {
					matrizSitios->data3D[i][j][k] = INDICE;
				}

				/*		if( MInicialRotulada->data3D[i][j][k] > 0 )
									{
									tipoObjeto = Objeto[ MInicialRotulada->data3D[i][j][k] ].Tipo() ;
									// SOLIDO = 0, PORO = 1, SITIO = 2, LIGACAO = 3, RAMO_MORTO = 4, NAO_IDENTIFICADO = 5
									// Se o numero de coneccoes >1, então é ligação
									if ( tipoObjeto == SOLIDO )
										MSitiosLigacoes->data3D[i][j][k] = SOLIDO;
									else if ( tipoObjeto  == PORO )
										MSitiosLigacoes->data3D[i][j][k] = PORO;
									else if ( tipoObjeto  == SITIO )
										MSitiosLigacoes->data3D[i][j][k] = SITIO;
									else if ( tipoObjeto  == LIGACAO )
										MSitiosLigacoes->data3D[i][j][k] = LIGACAO;
									else if ( tipoObjeto  == RAMO_MORTO )
										MSitiosLigacoes->data3D[i][j][k] = RAMO_MORTO;
									else if ( tipoObjeto  == NAO_IDENTIFICADO )
										MSitiosLigacoes->data3D[i][j][k] = NAO_IDENTIFICADO;
									}*/
			}
		}
	}
	// Salva MSitiosLigacoes em disco - resultado final (imagem com SOLIDO, SITIO, LIGACAO
	os.str("");
	os	<< "MatrizSitiosLigacoes"
			<< "-Modelo-" 	<< modelo
			<< "-rmee-" 	<< raioMaximoElementoEstruturante
			<< "-frree-"	<< fatorReducaoRaioElemEst
			<< "-iree-"	<< incrementoRaioElementoEstruturante
			<< "-" << nomeImagem
			<< ".dgm";
	cout << "-->Salvando imagem " << os.str().c_str() << endl ;
	Write( os.str(), matrizSitios, matrizLigacoes );

	//******************************
	// 1- Criar matriz nova
	// 2- Copiar sítios
	// 3- Calcular e salvar distribuicao de sitios
	// 4- Apagar e copiar dados ligaçoes
	// 5- Calcular e salvar distribuicao de  ligaçoes
	// TCMatriz3D< int >* ptrImg = new TCMatriz3D< int >(nx, ny);
	//*******************************

	// Salva MInicialRotulada em disco - resultado final (rótulos dos objetos)
	// Associada ao grafo permite a identificação dos píxeis.
	os.str("");
	os	<< "MatrizInicialRotulada-Final"
			<< "-Modelo-" << modelo
			<< "-rmee-" 	<< raioMaximoElementoEstruturante
			<< "-frree-"	<< fatorReducaoRaioElemEst
			<< "-iree-"		<< incrementoRaioElementoEstruturante
			<< "-"				<< nomeImagem
			<< ".dgm";
	cout << "-->Salvando imagem " << os.str().c_str() << endl ;
	MInicialRotulada->NumCores ( nObjetosDepoisAberturaComplementar ); // 256, numero objetos informa o maior rotulo utilizado.
	SalvarResultadosParciaisEmDisco( MInicialRotulada, os.str() );

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
		fout 	<< i << " " << Objeto[i].Tipo() << " " << Objeto[i].Rotulo() << " " << Objeto[i].SConeccao().size();
		for ( std::set<int>::iterator it = Objeto[i].SConeccao().begin(); it != Objeto[i].SConeccao().end(); it++	) {
			// indiceDasConeccoes" << endl
			fout <<  " " << *it << " ";
		}
		fout << endl; // Pula para linha de baixo
	}
	fout.close();

	delete matrizSitios;
	delete matrizLigacoes;
	delete MInicialRotulada;
	delete MatrizPmAntesAbertura;
}

/*
* Este método não altera o conteúdo de pm, a qual é utilizada como auxiliar.
*/
TCMatriz3D<int>* CAberturaDilatacao3D::DistSitiosLigacoes_Modelo_4() {
	// Variáveis auxiliares
	modelo = 4;
	//ostringstream os;
	int nObjetosAntesAbertura = 0;	// número de objetos existentes na matriz pm antes de sofrer a abertura
	int nObjetosDepoisAbertura = 0;	// número de objetos existentes na matriz pm depois de sofrer a abertura
	int nObjetosAberturaComplementar = 0; //número de objetos existentes na matriz complementar da matriz abertura
	int nx = pm->NX();
	int ny = pm->NY();
	int nz = pm->NZ();
	double area = nx*ny*nz;
	vector< pair< int, double > > distSitios;
	vector< pair< int, double > > distLigacoes;
	pair< int, double > dist;
	int i, j, k, cont;

	// Cria matriz para representar ligações.
	TCMatriz3D<bool>* matrizLigacoes = new TCMatriz3D<bool>( nx, ny, nz );
	matrizLigacoes->SetFormato( D1_X_Y_Z_ASCII );
	matrizLigacoes->Constante( FUNDO );

	// Cria matriz para representar sítios. Inicialmente é cópia de pm, depois os vexels serão apagados.
	// TCMatriz3D<bool>* matrizSitos = new TCMatriz3D<bool>( *pm );

	// Cria matriz abertura, cópia de pm.
	TCMatriz3D<bool>* matrizAbertura = new TCMatriz3D<bool>( *pm );

	// Cria matriz que ira representar o passo anterior da matriz abertura.
	// TCMatriz3D<bool>* matrizPassoAnterior = new TCMatriz3D<bool>( *matrizAbertura );

	// Calcula a porosidade na matrizAbertura;
	porosidade = Porosidade( matrizAbertura );

	// Rotula matrizRotulo;
	matrizRotulo->Go( matrizAbertura );
	nObjetosAntesAbertura = matrizRotulo->NumeroObjetos();

	// Cria matriz que irá armazenar os rótulos identificados nas diversas operações de abertura
	TCMatriz3D<int>* matrizRotulada = new TCMatriz3D<int>( *matrizRotulo );

	// Cria vetor de objetos do tipo CObjetoImagem
	vector< CObjetoImagem > Objeto ( matrizRotulo->NumeroObjetos() , CObjetoImagem( ));

	// O primeiro objeto irá representar sólidos e terá rótulo 0;
	Objeto[0].Rotulo( 0 );
	Objeto[0].Tipo( SOLIDO );

	// Os demais objetos irão representar poros e terão rótulo sequencial
	for ( i = 1; i < Objeto.size(); i++ ) {
		Objeto[i].Rotulo( i );
		Objeto[i].Tipo( PORO );
	}

	// Entra em loop para realizar operações de abertura e comparações nas matrizes
	cout << "Entrando no looping de calculo das distribuicoes..." << endl ;
	int raioEE = 1;
	int meioNX = nx/2;
	while ( (porosidade > 0.0) and (raioEE <= meioNX) and (raioEE <= raioMaximoElementoEstruturante) ) {
		cout << "==>Executando passo = " << raioEE << endl;
		cout << "-->Porosidade = " << porosidade << endl;
		cout << "-->nObjetosAntesAbertura = " << nObjetosAntesAbertura << endl;
		cout << "-->Processando Abertura..." << endl;
		pfmf->Go( matrizAbertura, raioEE );
		pfmf->Abertura( matrizAbertura, raioEE );

		cout << "-->Rotulando matriz abertura..." << endl ;
		matrizRotulo->Go( matrizAbertura );

		// Acumula o número de objeto antes e depois da abertura
		nObjetosDepoisAbertura = nObjetosAntesAbertura + matrizRotulo->NumeroObjetos() - 1; // menos 1 para não contar fundo novamente
		cout << "-->nObjetosDepoisAbertura = " << nObjetosDepoisAbertura << endl;

		// Os objetos identificados após a abertura são considerados sítios e serão adicionados a lista de objetos
		// Aproveita para calcular a área dos sítios
		cont = 0;
		for ( i = nObjetosAntesAbertura; i < nObjetosDepoisAbertura; i++ ) {
			Objeto.push_back( CObjetoImagem( SITIO , i ) ) ;
			cont++;
		}
		// Seta o par tamanho/area e armazena no vetor distribuição de ligações.
		dist.first  = raioEE;
		dist.second = 100.0 * cont / area;
		distSitios.push_back(dist);


		// Copia a matriz abertura rotulada (matrizRotulo) para a matrizRotulada,
		// assim, a matrizRotulada terá também a informação dos rótulos dos sítios identificados.
		// A seguir, inverte a região porosa na matrizAbertura de forma que esta passa a ser o complemento da abertura
		for ( i = 0; i < nx; i++) {
			for ( j = 0; j < ny; j++) {
				for ( k = 0; k < nz; k++) {
					// Se o pixel analizado é INDICE na matrizAbertura, copia o rótulo acrescidos do número de objetos antes da abertura para a matrizRotulada
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						matrizRotulada->data3D[i][j][k] = matrizRotulo->data3D[i][j][k] + nObjetosAntesAbertura - 1;// menos 1 para não contar fundo novamente
					}
					// Se o pixel analizado for INDICE em pm, inverte os valores da matrizAbertura assinalando como matriz complementar
					if ( pm->data3D[i][j][k] ==	INDICE ) {
						if ( matrizAbertura->data3D[i][j][k] == FUNDO ) {
							matrizAbertura->data3D[i][j][k] = INDICE;
						} else {
							matrizAbertura->data3D[i][j][k] = FUNDO;
						}
					}
				}
			}
		}

		cout << "-->Rotulando matriz abertura complementar..." << endl ;
		matrizRotulo->Go( matrizAbertura );

		// Acumula o número de objetos depois da abertura como o número de objetos do complemento da abertura
		nObjetosAberturaComplementar = nObjetosDepoisAbertura + matrizRotulo->NumeroObjetos() - 1;
		cout << "-->nObjetosAberturaComplementar = " << nObjetosAberturaComplementar << endl;

		// Adiciona a lista de objetos os rótulos identificados no complemento da matriz abertura (são ramos mortos)
		// Posteriormente irá identificar quais ramos mortos são ligações
		for ( i = nObjetosDepoisAbertura; i < nObjetosAberturaComplementar; i++ ) {
			Objeto.push_back( CObjetoImagem( RAMO_MORTO, i ) );
		}
		// Copia para a matrizRotulada os rótulos do complemento da matriz abertura, acrescidos do nObjetosDepoisAbertura, de forma que os rótulos sejam sequenciais
		for ( i = 0; i < nx; i++) {
			for ( j = 0; j < ny; j++) {
				for ( k = 0; k < nz; k++) {
					// Se o pixel analizado é INDICE na matriz Abertura, copia o rótulo do acrescidos do número de objetos antes da abertura para a matrizRotulada
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						matrizRotulada->data3D[i][j][k] = matrizRotulo->data3D[i][j][k] + nObjetosDepoisAbertura - 1;
					}
				}
			}
		}
		cout << "-->Comparando matrizes e fazendo conexões..." << endl ;
		// Identifica os vizinhos e caso o rótulo ainda não tenha sido incluído, faz a conexão.
		// lembre-se que set não tem repeticao, e sConeccao é do tipo set<int>.
		// Aqui a matrizRotulada tem SOLIDOs, POROs, SITIOs e RAMOs_MORTOs
		int borda = 1;
		int rotuloijk, rim1, rip1, rjm1, rjp1, rkm1, rkp1;
		for ( i = borda; i < (nx-borda); i++) {
			for ( j = borda; j < (ny-borda); j++) {
				for ( k = borda; k < (nz-borda); k++) {
					rotuloijk = matrizRotulada->data3D[i][j][k];
					// Só devemos considerar os rotulos da imagem abertura, i.e, rotulo >= nObjetosAntesAbertura
					if ( rotuloijk	>= nObjetosAntesAbertura ) {
						rim1 = matrizRotulada->data3D[i-1][j][k];
						rip1 = matrizRotulada->data3D[i+1][j][k];
						rjm1 = matrizRotulada->data3D[i][j-1][k];
						rjp1 = matrizRotulada->data3D[i][j+1][k];
						rkm1 = matrizRotulada->data3D[i][j][k-1];
						rkp1 = matrizRotulada->data3D[i][j][k+1];
						// Se os rotulos são diferentes e fazem parte da matriz abertura, então, marca a conexão.
						if ( rotuloijk != rim1 and rim1 >= nObjetosAntesAbertura)
							Objeto[rotuloijk].Conectar( rim1 );

						if ( rotuloijk != rip1 and rip1 >= nObjetosAntesAbertura)
							Objeto[rotuloijk].Conectar( rip1 );

						if ( rotuloijk != rjm1 and rjm1 >= nObjetosAntesAbertura)
							Objeto[rotuloijk].Conectar( rjm1 );

						if ( rotuloijk != rjp1 and rjp1  >= nObjetosAntesAbertura)
							Objeto[rotuloijk].Conectar( rjp1 );

						if ( rotuloijk != rkm1 and rkm1  >= nObjetosAntesAbertura)
							Objeto[rotuloijk].Conectar( rkm1 );

						if ( rotuloijk != rkp1 and rkp1  >= nObjetosAntesAbertura)
							Objeto[rotuloijk].Conectar( rkp1 );
					}
				}
			}
		}
		cout << "-->Identificando ligações..." << endl ;
		// Agora vamos percorrer os objetos anotados como RAMOs_MORTOs e identificar as ligações
		for ( i = nObjetosDepoisAbertura; i < nObjetosAberturaComplementar; i++ ) // Percorre apenas os rotulos marcados como RAMOs_MORTOs.
			if ( Objeto[i].SConeccao().size() > 1 ) // Se o numero de conexões for maior que 1, então é ligação
				Objeto[i].Tipo( LIGACAO );

		// Uma vez identificadas as ligações, podemos armazenar o resultado na matrizLigacoes.
		// Em seguida, aproveita o loop para restaurar a matrizAbertura para o estado anterior.
		cont = 0;
		for ( i = 0; i < nx; i++) {
			for ( j = 0; j < ny; j++) {
				for ( k = 0; k < nz; k++) {
					int rotulo = matrizRotulada->data3D[i][j][k];
					// matrizAbertura armazena a abertura complementar, tem RAMOs_MORTOs e LIGACOES
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						cont++; //incrementa independente de ser ligação ou ramo morto.
						if ( Objeto[ rotulo ].Tipo() == LIGACAO ) {
							matrizLigacoes->data3D[i][j][k] = INDICE;
						}
					}
					if ( rotulo  >= nObjetosAntesAbertura and rotulo < nObjetosDepoisAbertura ) {
						matrizAbertura->data3D[i][j][k] = INDICE;
					} else {
						matrizAbertura->data3D[i][j][k] = FUNDO;
					}
				}
			}
		}
		cout << "-->Área de sítios = " << dist.second << endl;
		// Seta o par tamanho/area e armazena no vetor distribuição de ligações.
		dist.first  = raioEE;
		dist.second = 100.0 * cont / area;
		distLigacoes.push_back(dist);
		cout << "-->Área de ligações = " << dist.second << endl;

		// Atualizando o número de objetos antes da abertura para o próximo passo.
		nObjetosAntesAbertura = nObjetosAberturaComplementar;

		// Atualizando porosidade
		porosidade = Porosidade( matrizAbertura );

		// Incrementando raio do Elemento Estruturante
		raioEE += incrementoRaioElementoEstruturante;
	} // fim do While

	cout << "==>Gerando resultado final..." << endl ;
	// Armazendo resultado final na matrizRotulada (reaproveitamento).
	for ( i = 0; i < nx; i++) {
		for ( j = 0; j < ny; j++) {
			for ( k = 0; k < nz; k++) {
				if ( matrizRotulada->data3D[i][j][k] != 0 ) {
					if ( matrizLigacoes->data3D[i][j][k] == INDICE ) {
						matrizRotulada->data3D[i][j][k] = 2;
					} else {
						matrizRotulada->data3D[i][j][k] = 1;
					}
				}
			}
		}
	}
	matrizRotulada->SetFormato(D2_X_Y_Z_GRAY_ASCII);
	matrizRotulada->NumCores( 3 );

	//delete matrizSitos;
	delete matrizLigacoes;
	delete matrizAbertura;
	//delete matrizPassoAnterior;

	return matrizRotulada;
}

// Objetiva determinar uma sequencia de abertura marcando e salvando a ImagemAbertura com diferentes tons de cinza.
// Se o pixel esta na matriz pm, copia para MAberturaDilatacao com o valor do raioEE, ficando cada abertura com tom de cinza diferente.
void CAberturaDilatacao3D::SequenciaAberturaTonsCinza() {
	// 	Cria matriz abertura
	TCMatriz3D<bool>* matrizAbertura = new TCMatriz3D<bool>( *pm );

	// Entra num looping para o raio do elemento estruturante
	cout << "Entrando no looping de calculo das aberturas..." << endl ;
	for ( int raioEE = 1; raioEE <= (pm->NX()-1)/2; raioEE++ ) {
		cout << "==>RAIO Elemento Estruturante = " <<  raioEE << endl ;

		cout << "-->Processando Abertura..." << endl ;
		pfmf->Go( pm, raioEE );
		pfmf->Abertura( pm , raioEE );

		cout << "-->Seta píxeis que ficaram com o valor do raioEE = " << raioEE << endl ;
		for ( int i = 0; i < pm->NX(); i++)
			for ( int j = 0; j < pm->NY(); j++)
				for ( int k = 0; k < pm->NZ(); k++)
					if ( pm->data3D[i][j][k] )
						matrizAbertura->data3D[i][j][k] = raioEE;
		// criterio parada
		if ( raioEE == raioMaximoElementoEstruturante)
			break;
	}

	matrizAbertura->SetFormato( D2_X_Y_Z_GRAY_ASCII );
	matrizAbertura->NumCores ( matrizAbertura->MaiorValor() + 1 );
	matrizAbertura->Write("matrizAbertura.dgm");
}
