#include <Segmentacao/3D/PorosGargantas/CAberturaDilatacao3D.h>
#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFd3453D.h>
#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIRA3D.h>
#include <iomanip>
#include <cmath>
#include <omp.h>

using namespace std;

CAberturaDilatacao3D::CAberturaDilatacao3D( TCMatriz3D<bool>* &matriz, int _indice, int _fundo)
	: pm(matriz), // pm é ponteiro para imagem externa (se mudar externamente teremos problemas).
		fatorReducaoRaioElemEst (1), raioMaximoElementoEstruturante ( 500 ), // usar limits
		incrementoRaioElementoEstruturante ( 1 ), raioEEDilatacao( 1 ), modelo(SETE), INDICE(_indice), FUNDO(_fundo),
		salvarResultadosParciais(false), gerarDetalhesObjetos(false)
{
	matrizRotulo = new TCRotulador3D<bool>( matriz, INDICE, FUNDO );
	matrizSitios = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );
	matrizSitios->SetFormato( D1_X_Y_Z_ASCII );
	matrizLigacoes = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );
	matrizLigacoes->SetFormato( D1_X_Y_Z_ASCII );
	pfmf = new TCFEMMIDFd3453D<bool>( matriz, INDICE, FUNDO );
}

CAberturaDilatacao3D::CAberturaDilatacao3D(TCImagem3D<bool>* &matriz , int _indice, int _fundo)
	: fatorReducaoRaioElemEst (1), raioMaximoElementoEstruturante ( 500 ), // usar limits
		incrementoRaioElementoEstruturante ( 1 ), raioEEDilatacao( 1 ), modelo(SETE), INDICE(_indice), FUNDO(_fundo),
		salvarResultadosParciais(false), gerarDetalhesObjetos(false)
{
	pm = dynamic_cast<TCMatriz3D<bool> *>(matriz), // pm é ponteiro para imagem externa (se mudar externamente teremos problemas).
	matrizRotulo = new TCRotulador3D<bool>( pm, INDICE, FUNDO );
	matrizSitios = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );
	matrizSitios->SetFormato( D1_X_Y_Z_ASCII );
	matrizLigacoes = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );
	matrizLigacoes->SetFormato( D1_X_Y_Z_ASCII );
	pfmf = new TCFEMMIDFd3453D<bool>( pm, INDICE, FUNDO );
}

CAberturaDilatacao3D::~CAberturaDilatacao3D() {
	delete matrizRotulo;
	delete matrizSitios;
	delete matrizLigacoes;
	delete pfmf;
}

double CAberturaDilatacao3D::Porosidade( TCMatriz3D<bool>* &_pm ) {
	double porosidade = 0.0;
	//executa os 3 for paralelamente, definindo todas as variáveis como compartilhadas, (exceto i, j e k). A variável porosidade terá os valores somados.
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private (i,j,k) reduction(+:porosidade) schedule(static,10)
	for ( i = 0; i < _pm->NX(); i++ )
		for ( j = 0; j < _pm->NY(); j++ )
			for ( k = 0; k < _pm->NZ(); k++ )
				if ( _pm->data3D[i][j][k] == 1 ) {
					porosidade++;
				}
	return 100.0 * porosidade / ( (double) _pm->NX() * _pm->NY() * _pm->NZ() );
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
		int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
		for ( i = 0; i < pm->NX(); i++)
			for ( j = 0; j < pm->NY(); j++)
				for ( k = 0; k < pm->NZ(); k++)
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

//	Recebe nome do arquivo a ser salvo e ponteiro para duas matrizes do tipo bool que serão mescladas.
//	Salva a mesclagem das matrizes em disco de forma que 0 será o fundo, 1 serão os índices da primeira matriz e 2 serão os índices da segunda matriz.
//	Se a possição dos índices coincidirem, o indice da última matriz informada como parâmetro será considerado.
//	Método criado para que se possa salvar em disco duas matrizes bool ao invés de utilizar uma matriz int para representar sólidos, sitios e ligações (consome menos memória).
bool CAberturaDilatacao3D::Write(string fileName) {
	ofstream fout; //  Abre arquivo disco
	fout.open(fileName.c_str(), ios::binary);
	int nx, ny, nz;
	if (fout.good()){
		//menores valores para nx, ny e nz.
		nx = pm->NX();
		ny = pm->NY();
		nz = pm->NZ();

		//cabeçalho
		fout << setw (0) << "D5" << '\n' << nx << ' ' << ny << ' ' << nz << '\n' << 3 << '\n';

		//percorre matrizes e mescla resultados salvando em disco.
		for (int k = 0; k < nz; ++k) {
			for (int j = 0; j < ny; ++j) {
				for (int i = 0; i < nx; ++i) {
					if( matrizSitios->data3D[i][j][k] == INDICE ) // se o voxel for índice em matrizSitios
						fout << (unsigned char) 1; //SÍTIO
					else if( matrizLigacoes->data3D[i][j][k] == INDICE ) // senão, se o voxel for índice em matrizLigacoes
						fout << (unsigned char) 2; //LIGACAO
					else // senão, só pode ser fundo
						fout << (unsigned char) 0; //FUNDO
				}
			}
		}
		return true;
	} else {
		return false;
	}
}

// Grava em disco, com o nome informado, os objetos identificados.
bool CAberturaDilatacao3D::SalvarListaObjetos(string fileName){
	if (gerarDetalhesObjetos) {
		ofstream fout; //  Abre arquivo disco
		fout.open(fileName.c_str());
		if (fout.good()){
			fout << "# " << matrizObjetos.size() << " " << pm->NX() << " " << pm->NY() << " " << pm->NZ() << endl;
			fout << "Obj.  X    Y    Z    Raio Tipo N.Voxeis N.ObjsCon LstObjsCons" << endl;
			for (it = matrizObjetos.begin(); it != matrizObjetos.end(); ++it) {
				fout << std::left << std::setw(6) << it->first;
				it->second.GravarObjeto(fout);
			}
			fout.close();
			return true;
		}
	}
	return false;
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

// Realiza a segmentação de poros e gargantas através do modelo informado.
void CAberturaDilatacao3D::Go( EModelo _modelo ) {
	modelo = _modelo;
	matrizObjetos.clear();
	matrizSitios->Constante(FUNDO);
	matrizLigacoes->Constante(FUNDO);
	switch (modelo) {
		case SETE: DistSitiosLigacoes_Modelo_7();
			break;
		case OITO: DistSitiosLigacoes_Modelo_8();
			break;
		case NOVE: DistSitiosLigacoes_Modelo_9();
			break;
		case DEZ: DistSitiosLigacoes_Modelo_10();
			break;
	}
	matrizObjetos.clear();
	GerarDetalhesMatrizObjetos();
}

void CAberturaDilatacao3D::GerarDetalhesMatrizObjetos() {
	// Se a flag estiver setada
	if (gerarDetalhesObjetos) {
		// Variáveis auxiliares
		map<int,CObjetoImagem>::iterator itt;
		int rip1, rjp1, rkp1;
		int rotulo = 0;
		int i,j,k;

		cout << "==>Rotulando a matrizSitios..." << endl ;
		matrizRotulo->Go(matrizSitios);

		cout << "==>Determinando IDF da matrizSitios..." << endl ;
		pfmf->Go(matrizSitios);

		// Armazena o número de objetos identificados na rotulagem
		int numObjs = matrizRotulo->NumeroObjetos();
		int nx = matrizSitios->NX();
		int ny = matrizSitios->NY();
		int nz = matrizSitios->NZ();

		cout << "==>Alimentando a matrizObjetos com os objetos identificados na rotulagem dos sítios..." << endl ;
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					rotulo = matrizRotulo->data3D[i][j][k];
					if ( rotulo != 0) {
						it = matrizObjetos.find(rotulo);
						if(it != matrizObjetos.end()){  // o elemento foi encontrado
							++(it->second);							// incrementa o número de objetos representados
						}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							matrizObjetos[rotulo] = CObjetoImagem( SITIO, 1 );
						}
						it->second.PontoCentral( i, j, k, pfmf->data3D[i][j][k] );
					}
				}
			}
		}

		// Cria matriz que irá acumular os rótulos identificados (inicialmente é cópia de matrizRotulo)
		TCMatriz3D<int>* matrizRotulada = new TCMatriz3D<int>( *matrizRotulo );

		cout << "==>Rotulando a matrizLigacoes..." << endl ;
		matrizRotulo->Go(matrizLigacoes);

		cout << "==>Determinando IDF da matrizLigacoes..." << endl ;
		pfmf->Go(matrizLigacoes);

		int numObjs_1 = numObjs-1;
		cout << "==>Alimentando a matrizObjetos com os objetos identificados na rotulagem das ligações..." << endl ;
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					rotulo = matrizRotulo->data3D[i][j][k];
					if ( rotulo > 0 ) { //só entra se for diferente de fundo, ou seja, neste caso, maior que numObjs
						rotulo = rotulo + numObjs_1;
						matrizRotulo->data3D[i][j][k] = rotulo; // para que os rótulos sejam sequenciais
						it = matrizObjetos.find(rotulo);
						if(it != matrizObjetos.end()){  // o elemento foi encontrado
							++(it->second);							// incrementa o número de objetos representados
						}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							matrizObjetos[rotulo] = CObjetoImagem( LIGACAO, 1);
						}
						it->second.PontoCentral( i, j, k, pfmf->data3D[i][j][k] );
					}
				}
			}
		}

		//salvando em disco
		//matrizLigacoes->Write("MatrizLigacoes.dbm");
		//matrizSitios->Write("MatrizSitios.dbm");
		//matrizRotulo->Write("MatrizLigacoesRotulada.dgm");
		//matrizRotulada->Write("MatrizSitiosRotulada.dgm");

		// Realizar conexões entre os objetos.
		for ( i = 0; i < (nx-1); ++i) {
			for ( j = 0; j < (ny-1); ++j) {
				for ( k = 0; k < (nz-1); ++k) {
					rotulo = matrizRotulo->data3D[i][j][k];
					if ( rotulo	> 0 ) {
						it = matrizObjetos.find(rotulo);
						rip1 = matrizRotulada->data3D[i+1][j][k];
						rjp1 = matrizRotulada->data3D[i][j+1][k];
						rkp1 = matrizRotulada->data3D[i][j][k+1];

						// Se os rotulos não representam sólido, e o vizinho é um sítio, então, marca a conexão.
						if ( rip1 > 0 ) {
							itt = matrizObjetos.find(rip1);
							itt->second.Conectar( rotulo );
							it->second.Conectar( rip1 );
						}
						if ( rjp1 > 0 ) {
							itt = matrizObjetos.find(rjp1);
							itt->second.Conectar( rotulo );
							it->second.Conectar( rjp1 );
						}
						if ( rkp1 > 0 ) {
							itt = matrizObjetos.find(rkp1);
							itt->second.Conectar( rotulo );
							it->second.Conectar( rkp1 );
						}
					}
				}
			}
		}
		// Libera memória
		delete matrizRotulada;
	}
}


/* ========================= Modelo 7 =========================
* Passos do algorítmo:
* Zera matrizObejtos
* Cria matrizAbertura, cópia de pm;
* Calcula porosidade;
* Rotula matriz abertura (matrizRotulo);
* Cria matrizRotulada, cópia de matrizRotulo;
* Calcula IDF da matrizAbertura;
* Entra no loop para identificar poros e gargantas;
*		Processa abertura em matrizAbertura;
*		Atualiza a porosidade da matrizAbertura;
*		Rotula a matrizAbertura (matrizRotulo);
*		Decrementa/Apaga da matrizObjetos objetos excluidos na operação de abertura;
*		Copia de forma incremental, os rótulos da matrizRotulo para a matrizRotulada;
*		Cria/Incrementa na matrizObjetos os novos objetos copiados para a metrizRotulada (SITIO);
*		Identifica o complemento da abertura excluindo objetos anteriormente identificados como ligações;
*		Rotula o complemento da abertura;
*		Percorre a matriz complemento da abertura e decrementa/apaga da matrizObjetos os objetos que deixarão de existir;
*		Copia para a matrizRotula, de forma incremental, os rótulos do complemento da abertura.
*		Cria/Incrementa na matrizObjetos os novos objetos copiados para a metrizRotulada (RAMO_MORTO);
*		Compara matrizes para fazer conexões entre os objetos.
*			Se o rótulo analizado for diferente do vizinho e o vizinho for um SITIO, faz a conexão entre os objetos;
*		Percorre o complemento da abertura para identificar quais RAMOS_MORTOS passarão a ser LIGACAO ou SITIO;
*			Se o objeto tiver mais de uma conexão, é LIGACAO; senão é SITIO.
*			Seta os objetos identificados na matrizSitios e matrizLigações;
*		Incrementa o raio do elemento estruturante raioEE;
*	Sai do loop quando a porosidade for 0, ou raioEE > raioMaximoElementoEstruturante, ou raioEE > NX/2;
*	Calcula IDF da matrizSitios;
*	Dilata a matrizSitio com EE de raio informado;
*	Percorre matrizSitios e matrizLigações para apagar pixeis inativos na matriz original e adequar uma a outra;
*/
void CAberturaDilatacao3D::DistSitiosLigacoes_Modelo_7() {
	// Variáveis auxiliares
	ostringstream os;
	double timing, totaltiming = omp_get_wtime();
	int nObjetosAntesAbertura = 0;	// número de objetos existentes na matriz pm antes de sofrer a abertura
	int nObjetosDepoisAbertura = 0;	// número de objetos existentes na matriz pm depois de sofrer a abertura
	int nObjetosAberturaComplementar = 0; //número de objetos existentes na matriz complementar da matriz abertura
	int nx = pm->NX();
	int ny = pm->NY();
	int nz = pm->NZ();
	int raioEE = 1;
	int meioNX = nx/2;
	int i, j, k, rotuloijk, borda;
	int rim1, rip1, rjm1, rjp1, rkm1, rkp1;
	int rim1jm1, rim1jp1, rim1km1, rim1kp1, rip1jp1, rip1jm1, rip1kp1, rip1km1, rjm1km1, rjm1kp1, rjp1kp1, rjp1km1;

	// Cria matriz abertura, cópia de pm.
	TCMatriz3D<bool>* matrizAbertura = new TCMatriz3D<bool>( *pm );
	matrizAbertura->SetFormato( D1_X_Y_Z_ASCII );

	// Calcula a porosidade na matrizAbertura;
	porosidade = Porosidade( matrizAbertura );

	// Rotula matrizRotulo;
	cout << "==>Rotulando Imagem...\t\t\t"; cout.flush(); timing = omp_get_wtime();
	matrizRotulo->Go( matrizAbertura );
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	nObjetosAntesAbertura = matrizRotulo->NumeroObjetos();

	// Cria matriz que irá armazenar os rótulos identificados nas diversas operações de abertura
	TCMatriz3D<int>* matrizRotulada = new TCMatriz3D<int>( *matrizRotulo );

	cout << "-->Preparando filtro...\t\t\t\t"; cout.flush(); timing = omp_get_wtime();
	pfmf->Go( matrizAbertura );//chama calculo idf 1x
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	cout << "Entrando no looping para indentificar poros e gargantas..." << endl ;
	while ( (porosidade > 0.0) and (raioEE <= meioNX) and (raioEE <= raioMaximoElementoEstruturante) ) {
		cout << "==>Executando passo = " << raioEE << endl;
		cout << "-->Porosidade = " << porosidade << endl;
		cout << "-->Num. objetos antes da abertura = " << matrizRotulo->NumeroObjetos() << endl;

		cout << "-->Processando Abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		pfmf->Abertura( matrizAbertura, raioEE );//para cada raio calcula abertura nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		os.str(""); os << "MatrizAbertura_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		// Atualizando porosidade
		porosidade = Porosidade( matrizAbertura );// nX

		cout << "-->Rotulando matriz abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );//rotula nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		// Acumula o número de objeto antes e depois da abertura
		nObjetosDepoisAbertura = nObjetosAntesAbertura + matrizRotulo->NumeroObjetos() - 1; // menos 1 para não contar fundo novamente
		cout << "-->Num. objetos depois da abertura = " << matrizRotulo->NumeroObjetos() << endl;

		// Copia a matriz abertura rotulada (matrizRotulo) para a matrizRotulada,
		// assim, a matrizRotulada terá também a informação dos rótulos dos sítios identificados.
		// A seguir, inverte a região porosa na matrizAbertura de forma que esta passa a ser o complemento da abertura
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,it) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// Se o pixel analizado é INDICE na matrizAbertura, copia o rótulo acrescidos do número de objetos antes da abertura para a matrizRotulada
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						//Primeiro decrementa o número de objetos representados na matrizObjetos e verifica se o objeto continuará existindo.
						//Os objetos existentes na matrizAbertura terão novos rótulos, então precisamos decrementar o número de objetos ou apagar os elementos que deixarão de representar objetos.
						rotuloijk = matrizRotulada->data3D[i][j][k];
						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento existe
							if(it->second.NumObjs() > 1){ // o elemento representa mais de um objeto
								--(it->second);							// decrementa o número de objetos representados
							}else{												// o elemento representa um ou menos objetos
								matrizObjetos.erase(it);		// apaga o elemento.
							}
						}
						//Agora atualiza a matriz rotulada com os novos rótulos e cria/incremeta os obejtos na matrizObejtos
						rotuloijk = matrizRotulo->data3D[i][j][k] + nObjetosAntesAbertura - 1;// menos 1 para não contar fundo novamente
						matrizRotulada->data3D[i][j][k] = rotuloijk;

						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento foi encontrado
							++(it->second);							// incrementa o número de objetos representados
						}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							//matrizObjetos[rotuloijk] = CObjetoImagem( SITIO , rotuloijk, 1);
							matrizObjetos[rotuloijk] = CObjetoImagem( SITIO, 1);
						}
					}
					//Identificando complemento da abertura:
					// Se o pixel analizado for INDICE em pm, inverte os valores da matrizAbertura assinalando como matriz complementar
					if ( pm->data3D[i][j][k] ==	INDICE ) {
						//if ( matrizAbertura->data3D[i][j][k] == FUNDO ) {
						if ( matrizAbertura->data3D[i][j][k] == FUNDO and matrizLigacoes->data3D[i][j][k] == FUNDO ) {
							matrizAbertura->data3D[i][j][k] = INDICE;
						} else {
							matrizAbertura->data3D[i][j][k] = FUNDO;
						}
					}
				}
			}
		}

		os.str(""); os << "MatrizComplementoAbertura_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		cout << "-->Rotulando matriz abertura complementar...\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );//nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		// Acumula o número de objetos depois da abertura como o número de objetos do complemento da abertura
		nObjetosAberturaComplementar = nObjetosDepoisAbertura + matrizRotulo->NumeroObjetos() - 1;
		cout << "-->Num. objetos na abertura complementar = " << matrizRotulo->NumeroObjetos() << endl;

		// Copia para a matrizRotulada os rótulos do complemento da matriz abertura, acrescidos do nObjetosDepoisAbertura, de forma que os rótulos sejam sequenciais
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,it) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// Se o pixel analizado é INDICE na matriz Abertura, copia o rótulo do acrescidos do número de objetos antes da abertura para a matrizRotulada
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						//Primeiro decrementa o número de objetos representados na matrizObjetos e verifica se o objeto continuará existindo.
						//Os objetos existentes na matrizAbertura terão novos rótulos, então precisamos decrementar o número de objetos ou apagar os elementos que deixarão de representar objetos.
						rotuloijk = matrizRotulada->data3D[i][j][k];
						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento existe
							if(it->second.NumObjs() > 1){ // o elemento representa mais de um objeto
								--(it->second);							// decrementa o número de objetos representados
							}else{												// o elemento represente um ou menos objetos
								matrizObjetos.erase(it);		// apaga o elemento.
							}
						}
						//Agora atualiza a matriz rotulada com os novos rótulos e cria/incremeta os obejtos na matrizObejtos
						//Os objetos identificados no complemento da abertura, inicialmente são considerados ramos mortos; depois iremos verificar quais ramos representam ligações.
						rotuloijk = matrizRotulo->data3D[i][j][k] + nObjetosDepoisAbertura - 1;
						matrizRotulada->data3D[i][j][k] = rotuloijk;
						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento foi encontrado
							++(it->second);								// incrementa o número de objetos representados
						}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							matrizObjetos[rotuloijk] = CObjetoImagem( RAMO_MORTO, 1);
						}
					}
				}
			}
		}

		borda = 1;
		cout << "-->Comparando matrizes e fazendo conexões..." << endl ;
		// Identifica os vizinhos e caso o rótulo ainda não tenha sido incluído, faz a conexão.
		// lembre-se que set não tem repeticao, e sConexao é do tipo set<int>.
		// Aqui a matrizRotulada tem SOLIDOs, POROs, SITIOs e RAMOs_MORTOs
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,rim1,rip1,rjm1,rjp1,rkm1,rkp1,it) //schedule(dynamic,10)
		for ( i = borda; i < (nx-borda); ++i) {
			for ( j = borda; j < (ny-borda); ++j) {
				for ( k = borda; k < (nz-borda); ++k) {
					rotuloijk = matrizRotulada->data3D[i][j][k];
					// Só devemos considerar os rotulos criados neste passo, i.e, rotulo >= nObjetosAntesAbertura
					if ( rotuloijk	>= nObjetosAntesAbertura ) {
						it = matrizObjetos.find(rotuloijk);
						rim1 = matrizRotulada->data3D[i-1][j][k];
						rip1 = matrizRotulada->data3D[i+1][j][k];
						rjm1 = matrizRotulada->data3D[i][j-1][k];
						rjp1 = matrizRotulada->data3D[i][j+1][k];
						rkm1 = matrizRotulada->data3D[i][j][k-1];
						rkp1 = matrizRotulada->data3D[i][j][k+1];
						rim1jm1 = matrizRotulada->data3D[i-1][j-1][k];
						rim1jp1 = matrizRotulada->data3D[i-1][j+1][k];
						rim1km1 = matrizRotulada->data3D[i-1][j][k-1];
						rim1kp1 = matrizRotulada->data3D[i-1][j][k+1];
						rip1jp1 = matrizRotulada->data3D[i+1][j+1][k];
						rip1jm1 = matrizRotulada->data3D[i+1][j-1][k];
						rip1kp1 = matrizRotulada->data3D[i+1][j][k+1];
						rip1km1 = matrizRotulada->data3D[i+1][j][k-1];
						rjm1km1 = matrizRotulada->data3D[i][j-1][k-1];
						rjm1kp1 = matrizRotulada->data3D[i][j-1][k+1];
						rjp1km1 = matrizRotulada->data3D[i][j+1][k-1];
						rjp1kp1 = matrizRotulada->data3D[i][j+1][k+1];

						// Se os rotulos são diferentes, fazem parte da matriz abertura e o vizinho é um sítio, então, marca a conexão.
						if ( rotuloijk != rim1 and matrizObjetos[rim1].Tipo() == SITIO)
							it->second.Conectar( rim1 );
						if ( rotuloijk != rip1 and matrizObjetos[rip1].Tipo() == SITIO)
							it->second.Conectar( rip1 );
						if ( rotuloijk != rjm1 and matrizObjetos[rjm1].Tipo() == SITIO)
							it->second.Conectar( rjm1 );
						if ( rotuloijk != rjp1 and matrizObjetos[rjp1].Tipo() == SITIO)
							it->second.Conectar( rjp1 );
						if ( rotuloijk != rkm1 and matrizObjetos[rkm1].Tipo() == SITIO)
							it->second.Conectar( rkm1 );
						if ( rotuloijk != rkp1 and matrizObjetos[rkp1].Tipo() == SITIO)
							it->second.Conectar( rkp1 );
						if ( rotuloijk != rim1jm1 and matrizObjetos[rim1jm1].Tipo() == SITIO)
							it->second.Conectar( rim1jm1 );
						if ( rotuloijk != rim1jp1 and matrizObjetos[rim1jp1].Tipo() == SITIO)
							it->second.Conectar( rim1jp1 );
						if ( rotuloijk != rim1km1 and matrizObjetos[rim1km1].Tipo() == SITIO)
							it->second.Conectar( rim1km1 );
						if ( rotuloijk != rim1kp1 and matrizObjetos[rim1kp1].Tipo() == SITIO)
							it->second.Conectar( rim1kp1 );
						if ( rotuloijk != rip1jp1 and matrizObjetos[rip1jp1].Tipo() == SITIO)
							it->second.Conectar( rip1jp1 );
						if ( rotuloijk != rip1jm1 and matrizObjetos[rip1jm1].Tipo() == SITIO)
							it->second.Conectar( rip1jm1 );
						if ( rotuloijk != rip1kp1 and matrizObjetos[rip1kp1].Tipo() == SITIO)
							it->second.Conectar( rip1kp1 );
						if ( rotuloijk != rip1km1 and matrizObjetos[rip1km1].Tipo() == SITIO)
							it->second.Conectar( rip1km1 );
						if ( rotuloijk != rjm1km1 and matrizObjetos[rjm1km1].Tipo() == SITIO)
							it->second.Conectar( rjm1km1 );
						if ( rotuloijk != rjm1kp1 and matrizObjetos[rjm1kp1].Tipo() == SITIO)
							it->second.Conectar( rjm1kp1 );
						if ( rotuloijk != rjp1km1 and matrizObjetos[rjp1km1].Tipo() == SITIO)
							it->second.Conectar( rjp1km1 );
						if ( rotuloijk != rjp1kp1 and matrizObjetos[rjp1kp1].Tipo() == SITIO)
							it->second.Conectar( rjp1kp1 );
					}
				}
			}
		}

		// Agora vamos percorrer os objetos anotados como RAMOs_MORTOs e identificar as ligações
		// Uma vez identificadas as ligações, podemos armazenar o resultado na matrizLigacoes.
		// Em seguida, aproveita o loop para restaurar a matrizAbertura para o estado anterior.
		cout << "-->Identificando sitios e ligações..." << endl ;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// matrizAbertura armazena a abertura complementar, tem RAMOs_MORTOs e LIGACOES
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						rotuloijk = matrizRotulada->data3D[i][j][k];
						if ( matrizObjetos[rotuloijk].SConexao().size() > 1 ) {
							matrizObjetos[rotuloijk].Tipo( LIGACAO );
							matrizLigacoes->data3D[i][j][k] = INDICE;
						}else{
							matrizObjetos[rotuloijk].Tipo( SITIO );
							matrizSitios->data3D[i][j][k] = INDICE;
						}
					}

					// restaura a matrizAbertura.
					matrizAbertura->data3D[i][j][k] = pm->data3D[i][j][k];
				}
			}
		}

		//os.str(""); os << "MatrizSitios_" << raioEE << ".dbm";
		//SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

		//os.str(""); os << "MatrizLigacoes_" << raioEE << ".dbm";
		//SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

		// Atualizando o número de objetos antes da abertura para o próximo passo.
		nObjetosAntesAbertura = nObjetosAberturaComplementar;

		// Incrementando raio do Elemento Estruturante
		raioEE += incrementoRaioElementoEstruturante;
	} // fim do While

	cout << "==>Preparando filtro..."; cout.flush(); timing = omp_get_wtime();
	pfmf->Go( matrizSitios );
	cout << "\t\t\t\ttempo: " << omp_get_wtime()-timing << " s." << endl;

	cout << "==>Processando Dilataço..."; cout.flush(); timing = omp_get_wtime();
	pfmf->Dilatacao(matrizSitios, raioEEDilatacao );
	cout << "\t\t\ttempo: " << omp_get_wtime()-timing << " s." << endl;

	cout << "==>Corrigindo sítios e ligações..." << endl ;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				if( pm->data3D[i][j][k] == FUNDO) {
					matrizSitios->data3D[i][j][k] = FUNDO;
					matrizLigacoes->data3D[i][j][k] = FUNDO;
				} else if ( matrizLigacoes->data3D[i][j][k] == INDICE ) {
					matrizSitios->data3D[i][j][k] = FUNDO;
				} else {
					matrizSitios->data3D[i][j][k] = INDICE;
				}
			}
		}
	}

	// Libera memória.
	delete matrizAbertura;
	delete matrizRotulada;

	cout << "==>Tempo total de execução: " << (omp_get_wtime()-totaltiming)/60 << " min." << endl;
}

/* ========================= Modelo 8 =========================*/
void CAberturaDilatacao3D::DistSitiosLigacoes_Modelo_8() {
	// Variáveis auxiliares
	ostringstream os;
	double timing, totaltiming = omp_get_wtime();
	int nObjetosAntesAbertura = 0;	// número de objetos existentes na matriz pm antes de sofrer a abertura
	int nObjetosDepoisAbertura = 0;	// número de objetos existentes na matriz pm depois de sofrer a abertura
	int nObjetosAberturaComplementar = 0; //número de objetos existentes na matriz complementar da matriz abertura
	int numObjetos = 0; // Armazena o número de identificados na rotulagem
	int nx = pm->NX();
	int ny = pm->NY();
	int nz = pm->NZ();
	int raioEE = 1;
	int meioNX = nx/2;
	int i, j, k, rotuloijk, borda;
	int rim1, rip1, rjm1, rjp1, rkm1, rkp1;
	int rim1jm1, rim1jp1, rim1km1, rim1kp1, rip1jp1, rip1jm1, rip1kp1, rip1km1, rjm1km1, rjm1kp1, rjp1kp1, rjp1km1;

	// Cria matriz abertura, cópia de pm.
	TCMatriz3D<bool>* matrizAbertura = new TCMatriz3D<bool>( *pm );
	matrizAbertura->SetFormato( D1_X_Y_Z_ASCII );

	// Rotula matrizRotulo;
	cout << "==>Rotulando Imagem...\t\t\t"; cout.flush(); timing = omp_get_wtime();
	matrizRotulo->Go( matrizAbertura );
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	numObjetos = nObjetosAntesAbertura = matrizRotulo->NumeroObjetos();

	// Cria matriz que irá armazenar os rótulos identificados nas diversas operações de abertura
	TCMatriz3D<int>* matrizRotulada = new TCMatriz3D<int>( *matrizRotulo );

	cout << "-->Preparando filtro...\t\t\t\t"; cout.flush(); timing = omp_get_wtime();
	pfmf->Go( matrizAbertura );//chama calculo idf 1x
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	cout << "Entrando no looping para indentificar poros e gargantas..." << endl ;
	while ( (numObjetos > 1) and (raioEE <= meioNX) and (raioEE <= raioMaximoElementoEstruturante) ) {
		cout << "==>Executando passo = " << raioEE << endl;
		cout << "-->Num. objetos identificados na imagem = " << numObjetos << endl;
		cout << "-->Num. objetos antes da abertura = " << matrizRotulo->NumeroObjetos() << endl;

		cout << "-->Processando Abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		pfmf->Abertura( matrizAbertura, raioEE );//para cada raio calcula abertura nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		os.str(""); os << "MatrizAbertura_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		cout << "-->Rotulando matriz abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );//rotula nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		// Atualiza o número de objetos identificados na imagem após a operação de abertura.
		numObjetos = matrizRotulo->NumeroObjetos();
		// Acumula o número de objeto antes e depois da abertura
		nObjetosDepoisAbertura = nObjetosAntesAbertura + numObjetos - 1; // menos 1 para não contar fundo novamente
		cout << "-->Num. objetos depois da abertura = " << matrizRotulo->NumeroObjetos() << endl;

		// Copia a matriz abertura rotulada (matrizRotulo) para a matrizRotulada,
		// assim, a matrizRotulada terá também a informação dos rótulos dos sítios identificados.
		// A seguir, inverte a região porosa na matrizAbertura de forma que esta passa a ser o complemento da abertura
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,it) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// Se o pixel analizado é INDICE na matrizAbertura, copia o rótulo acrescidos do número de objetos antes da abertura para a matrizRotulada
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						//Primeiro decrementa o número de objetos representados na matrizObjetos e verifica se o objeto continuará existindo.
						//Os objetos existentes na matrizAbertura terão novos rótulos, então precisamos decrementar o número de objetos ou apagar os elementos que deixarão de representar objetos.
						rotuloijk = matrizRotulada->data3D[i][j][k];
						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento existe
							if(it->second.NumObjs() > 1){ // o elemento representa mais de um objeto
								--(it->second);							// decrementa o número de objetos representados
							}else{												// o elemento representa um ou menos objetos
								matrizObjetos.erase(it);		// apaga o elemento.
							}
						}
						//Agora atualiza a matriz rotulada com os novos rótulos e cria/incremeta os obejtos na matrizObejtos
						rotuloijk = matrizRotulo->data3D[i][j][k] + nObjetosAntesAbertura - 1;// menos 1 para não contar fundo novamente
						matrizRotulada->data3D[i][j][k] = rotuloijk;

						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento foi encontrado
							++(it->second);							// incrementa o número de objetos representados
						}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							//matrizObjetos[rotuloijk] = CObjetoImagem( SITIO , rotuloijk, 1);
							matrizObjetos[rotuloijk] = CObjetoImagem( SITIO, 1);
						}
					}
					// Identificando complemento da abertura:
					// Se o pixel analizado for INDICE em pm, inverte os valores da matrizAbertura assinalando como matriz complementar
					if ( pm->data3D[i][j][k] ==	INDICE ) {
						if ( matrizAbertura->data3D[i][j][k] == FUNDO and matrizLigacoes->data3D[i][j][k] == FUNDO ) {
							matrizAbertura->data3D[i][j][k] = INDICE;
						} else {
							matrizAbertura->data3D[i][j][k] = FUNDO;
						}
					}
				}
			}
		}

		os.str(""); os << "MatrizComplementoAbertura_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		cout << "-->Rotulando matriz abertura complementar...\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );//nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		// Acumula o número de objetos depois da abertura como o número de objetos do complemento da abertura
		nObjetosAberturaComplementar = nObjetosDepoisAbertura + matrizRotulo->NumeroObjetos() - 1;
		cout << "-->Num. objetos na abertura complementar = " << matrizRotulo->NumeroObjetos() << endl;

		// Copia para a matrizRotulada os rótulos do complemento da matriz abertura, acrescidos do nObjetosDepoisAbertura, de forma que os rótulos sejam sequenciais
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,it) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// Se o pixel analizado é INDICE na matriz Abertura, copia o rótulo do acrescidos do número de objetos antes da abertura para a matrizRotulada
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						//Primeiro decrementa o número de objetos representados na matrizObjetos e verifica se o objeto continuará existindo.
						//Os objetos existentes na matrizAbertura terão novos rótulos, então precisamos decrementar o número de objetos ou apagar os elementos que deixarão de representar objetos.
						rotuloijk = matrizRotulada->data3D[i][j][k];
						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento existe
							if(it->second.NumObjs() > 1){ // o elemento representa mais de um objeto
								--(it->second);							// decrementa o número de objetos representados
							}else{												// o elemento represente um ou menos objetos
								matrizObjetos.erase(it);		// apaga o elemento.
							}
						}
						//Agora atualiza a matriz rotulada com os novos rótulos e cria/incremeta os obejtos na matrizObejtos
						//Os objetos identificados no complemento da abertura, inicialmente são considerados ramos mortos; depois iremos verificar quais ramos representam ligações.
						rotuloijk = matrizRotulo->data3D[i][j][k] + nObjetosDepoisAbertura - 1;
						matrizRotulada->data3D[i][j][k] = rotuloijk;
						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento foi encontrado
							++(it->second);								// incrementa o número de objetos representados
						}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							matrizObjetos[rotuloijk] = CObjetoImagem( RAMO_MORTO, 1);
						}
					}
				}
			}
		}

		borda = 1;
		cout << "-->Comparando matrizes e fazendo conexões..." << endl ;
		// Identifica os vizinhos e caso o rótulo ainda não tenha sido incluído, faz a conexão.
		// lembre-se que set não tem repeticao, e sConexao é do tipo set<int>.
		// Aqui a matrizRotulada tem SOLIDOs, POROs, SITIOs e RAMOs_MORTOs
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,rim1,rip1,rjm1,rjp1,rkm1,rkp1,it) //schedule(dynamic,10)
		for ( i = borda; i < (nx-borda); ++i) {
			for ( j = borda; j < (ny-borda); ++j) {
				for ( k = borda; k < (nz-borda); ++k) {
					rotuloijk = matrizRotulada->data3D[i][j][k];
					// Só devemos considerar os rotulos criados neste passo, i.e, rotulo >= nObjetosAntesAbertura
					if ( rotuloijk	>= nObjetosAntesAbertura ) {
						it = matrizObjetos.find(rotuloijk);
						rim1 = matrizRotulada->data3D[i-1][j][k];
						rip1 = matrizRotulada->data3D[i+1][j][k];
						rjm1 = matrizRotulada->data3D[i][j-1][k];
						rjp1 = matrizRotulada->data3D[i][j+1][k];
						rkm1 = matrizRotulada->data3D[i][j][k-1];
						rkp1 = matrizRotulada->data3D[i][j][k+1];
						rim1jm1 = matrizRotulada->data3D[i-1][j-1][k];
						rim1jp1 = matrizRotulada->data3D[i-1][j+1][k];
						rim1km1 = matrizRotulada->data3D[i-1][j][k-1];
						rim1kp1 = matrizRotulada->data3D[i-1][j][k+1];
						rip1jp1 = matrizRotulada->data3D[i+1][j+1][k];
						rip1jm1 = matrizRotulada->data3D[i+1][j-1][k];
						rip1kp1 = matrizRotulada->data3D[i+1][j][k+1];
						rip1km1 = matrizRotulada->data3D[i+1][j][k-1];
						rjm1km1 = matrizRotulada->data3D[i][j-1][k-1];
						rjm1kp1 = matrizRotulada->data3D[i][j-1][k+1];
						rjp1km1 = matrizRotulada->data3D[i][j+1][k-1];
						rjp1kp1 = matrizRotulada->data3D[i][j+1][k+1];

						// Se os rotulos são diferentes, fazem parte da matriz abertura e o vizinho é um sítio, então, marca a conexão.
						if ( rotuloijk != rim1 and matrizObjetos[rim1].Tipo() == SITIO)
							it->second.Conectar( rim1 );
						if ( rotuloijk != rip1 and matrizObjetos[rip1].Tipo() == SITIO)
							it->second.Conectar( rip1 );
						if ( rotuloijk != rjm1 and matrizObjetos[rjm1].Tipo() == SITIO)
							it->second.Conectar( rjm1 );
						if ( rotuloijk != rjp1 and matrizObjetos[rjp1].Tipo() == SITIO)
							it->second.Conectar( rjp1 );
						if ( rotuloijk != rkm1 and matrizObjetos[rkm1].Tipo() == SITIO)
							it->second.Conectar( rkm1 );
						if ( rotuloijk != rkp1 and matrizObjetos[rkp1].Tipo() == SITIO)
							it->second.Conectar( rkp1 );
						if ( rotuloijk != rim1jm1 and matrizObjetos[rim1jm1].Tipo() == SITIO)
							it->second.Conectar( rim1jm1 );
						if ( rotuloijk != rim1jp1 and matrizObjetos[rim1jp1].Tipo() == SITIO)
							it->second.Conectar( rim1jp1 );
						if ( rotuloijk != rim1km1 and matrizObjetos[rim1km1].Tipo() == SITIO)
							it->second.Conectar( rim1km1 );
						if ( rotuloijk != rim1kp1 and matrizObjetos[rim1kp1].Tipo() == SITIO)
							it->second.Conectar( rim1kp1 );
						if ( rotuloijk != rip1jp1 and matrizObjetos[rip1jp1].Tipo() == SITIO)
							it->second.Conectar( rip1jp1 );
						if ( rotuloijk != rip1jm1 and matrizObjetos[rip1jm1].Tipo() == SITIO)
							it->second.Conectar( rip1jm1 );
						if ( rotuloijk != rip1kp1 and matrizObjetos[rip1kp1].Tipo() == SITIO)
							it->second.Conectar( rip1kp1 );
						if ( rotuloijk != rip1km1 and matrizObjetos[rip1km1].Tipo() == SITIO)
							it->second.Conectar( rip1km1 );
						if ( rotuloijk != rjm1km1 and matrizObjetos[rjm1km1].Tipo() == SITIO)
							it->second.Conectar( rjm1km1 );
						if ( rotuloijk != rjm1kp1 and matrizObjetos[rjm1kp1].Tipo() == SITIO)
							it->second.Conectar( rjm1kp1 );
						if ( rotuloijk != rjp1km1 and matrizObjetos[rjp1km1].Tipo() == SITIO)
							it->second.Conectar( rjp1km1 );
						if ( rotuloijk != rjp1kp1 and matrizObjetos[rjp1kp1].Tipo() == SITIO)
							it->second.Conectar( rjp1kp1 );
					}
				}
			}
		}

		// Agora vamos percorrer os objetos anotados como RAMOs_MORTOs e identificar as ligações
		// Uma vez identificadas as ligações, podemos armazenar o resultado na matrizLigacoes.
		// Em seguida, aproveita o loop para restaurar a matrizAbertura para o estado anterior.
		cout << "-->Identificando sitios e ligações..." << endl ;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// matrizAbertura armazena a abertura complementar, tem RAMOs_MORTOs e LIGACOES
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						rotuloijk = matrizRotulada->data3D[i][j][k];
						if ( matrizObjetos[rotuloijk].SConexao().size() > 1 ) {
							matrizObjetos[rotuloijk].Tipo( LIGACAO );
							matrizLigacoes->data3D[i][j][k] = INDICE;
							matrizSitios->data3D[i][j][k] = FUNDO;
						}else{
							matrizObjetos[rotuloijk].Tipo( SITIO );
							matrizSitios->data3D[i][j][k] = INDICE;
							matrizLigacoes->data3D[i][j][k] = FUNDO;
						}
					}
					// restaura a matrizAbertura.
					matrizAbertura->data3D[i][j][k] = pm->data3D[i][j][k];
				}
			}
		}

		//os.str(""); os << "MatrizSitios_" << raioEE << ".dbm";
		//SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

		//os.str(""); os << "MatrizLigacoes_" << raioEE << ".dbm";
		//SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

		// Atualizando o número de objetos antes da abertura para o próximo passo.
		nObjetosAntesAbertura = nObjetosAberturaComplementar;

		// Incrementando raio do Elemento Estruturante
		raioEE += incrementoRaioElementoEstruturante;
	} // fim do While

	// Libera memória.
	delete matrizAbertura;
	delete matrizRotulada;

	cout << "==>Preparando filtro..."; cout.flush(); timing = omp_get_wtime();
	pfmf->Go( matrizSitios );
	cout << "\t\t\t\ttempo: " << omp_get_wtime()-timing << " s." << endl;

	cout << "==>Processando Dilataço..."; cout.flush(); timing = omp_get_wtime();
	pfmf->Dilatacao(matrizSitios, raioEEDilatacao );
	cout << "\t\t\ttempo: " << omp_get_wtime()-timing << " s." << endl;

	cout << "==>Corrigindo sítios e ligações..." << endl ;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				if( pm->data3D[i][j][k] == FUNDO) {
					matrizSitios->data3D[i][j][k] = FUNDO;
					matrizLigacoes->data3D[i][j][k] = FUNDO;
				} else if ( matrizSitios->data3D[i][j][k] == INDICE ) {
					matrizLigacoes->data3D[i][j][k] = FUNDO;
				} else {
					matrizLigacoes->data3D[i][j][k] = INDICE;
				}
			}
		}
	}

	cout << "==>Zerando a matrizObjetos..." << endl ;
	matrizObjetos.clear();

	cout << "==>Rotulando a matrizSitios..." << endl ;
	matrizRotulo->Go(matrizSitios);

	cout << "==>Determinando IDF da matrizSitios..." << endl ;
	pfmf->Go(matrizSitios);

	// Armazena o número de objetos identificados na rotulagem
	numObjetos = matrizRotulo->NumeroObjetos();

	cout << "==>Alimentando a matrizObjetos com os objetos identificados na rotulagem dos sítios..." << endl ;
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				rotuloijk = matrizRotulo->data3D[i][j][k];
				if ( rotuloijk != 0) {
					it = matrizObjetos.find(rotuloijk);
					if(it != matrizObjetos.end()){  // o elemento foi encontrado
						++(it->second);							// incrementa o número de objetos representados
					}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
						matrizObjetos[rotuloijk] = CObjetoImagem( SITIO, 1 );
					}
					it->second.PontoCentral( i, j, k, pfmf->data3D[i][j][k] );
				}
			}
		}
	}

	cout << "==>Rotulando a matrizLigacoes..." << endl ;
	matrizRotulo->Go(matrizLigacoes);

	cout << "==>Determinando IDF da matrizLigacoes..." << endl ;
	pfmf->Go(matrizLigacoes);

	cout << "==>Alimentando a matrizObjetos com os objetos identificados na rotulagem das ligações..." << endl ;
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				rotuloijk = matrizRotulo->data3D[i][j][k] + numObjetos;
				if ( rotuloijk != numObjetos ) {
					it = matrizObjetos.find(rotuloijk);
					if(it != matrizObjetos.end()){  // o elemento foi encontrado
						++(it->second);							// incrementa o número de objetos representados
					}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
						matrizObjetos[rotuloijk] = CObjetoImagem( LIGACAO, 1);
					}
					it->second.PontoCentral( i, j, k, pfmf->data3D[i][j][k] );
				}
			}
		}
	}

	cout << "==>Tempo total de execução: " << (omp_get_wtime()-totaltiming)/60 << " min." << endl;
}

/* ========================= Modelo 9 =========================*/
void CAberturaDilatacao3D::DistSitiosLigacoes_Modelo_9() {
	// Variáveis auxiliares
	ostringstream os;
	double timing, totaltiming = omp_get_wtime();
	int nObjetosAntesAbertura = 0;	// número de objetos existentes na matriz pm antes de sofrer a abertura
	int nObjetosDepoisAbertura = 0;	// número de objetos existentes na matriz pm depois de sofrer a abertura
	int nObjetosAberturaComplementar = 0; //número de objetos existentes na matriz complementar da matriz abertura
	int numObjetos = 0; // Armazena o número de identificados na rotulagem
	int nx = pm->NX();
	int ny = pm->NY();
	int nz = pm->NZ();
	int raioEE = 1;
	int meioNX = nx/2;
	int i, j, k, rotuloijk, borda;
	int rim1, rip1, rjm1, rjp1, rkm1, rkp1;
	int rim1jm1, rim1jp1, rim1km1, rim1kp1, rip1jp1, rip1jm1, rip1kp1, rip1km1, rjm1km1, rjm1kp1, rjp1kp1, rjp1km1;

	// Cria matriz abertura, cópia de pm.
	TCMatriz3D<bool>* matrizAbertura = new TCMatriz3D<bool>( *pm );
	matrizAbertura->SetFormato( D1_X_Y_Z_ASCII );

	// Rotula matrizRotulo;
	cout << "==>Rotulando Imagem...\t\t\t"; cout.flush(); timing = omp_get_wtime();
	matrizRotulo->Go( matrizAbertura );
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	numObjetos = nObjetosAntesAbertura = matrizRotulo->NumeroObjetos();

	// Cria matriz que irá armazenar os rótulos identificados nas diversas operações de abertura
	TCMatriz3D<int>* matrizRotulada = new TCMatriz3D<int>( *matrizRotulo );

	cout << "-->Preparando filtro...\t\t\t\t"; cout.flush(); timing = omp_get_wtime();
	pfmf->Go( matrizAbertura );//chama calculo idf 1x
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	cout << "Entrando no looping para indentificar poros e gargantas..." << endl ;
	while ( (numObjetos > 1) and (raioEE <= meioNX) and (raioEE <= raioMaximoElementoEstruturante) ) {
		cout << "==>Executando passo = " << raioEE << endl;
		cout << "-->Num. objetos identificados na imagem = " << numObjetos << endl;
		cout << "-->Num. objetos antes da abertura = " << matrizRotulo->NumeroObjetos() << endl;

		cout << "-->Processando Abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		pfmf->Abertura( matrizAbertura, raioEE );//para cada raio calcula abertura nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		os.str(""); os << "MatrizAbertura_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		cout << "-->Rotulando matriz abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );//rotula nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		// Atualiza o número de objetos identificados na imagem após a operação de abertura.
		numObjetos = matrizRotulo->NumeroObjetos();
		// Acumula o número de objeto antes e depois da abertura
		nObjetosDepoisAbertura = nObjetosAntesAbertura + numObjetos - 1; // menos 1 para não contar fundo novamente
		cout << "-->Num. objetos depois da abertura = " << numObjetos << endl;

		// Copia a matriz abertura rotulada (matrizRotulo) para a matrizRotulada,
		// assim, a matrizRotulada terá também a informação dos rótulos dos sítios identificados.
		// A seguir, inverte a região porosa na matrizAbertura de forma que esta passa a ser o complemento da abertura
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,it) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// Se o pixel analizado é INDICE na matrizAbertura, copia o rótulo acrescidos do número de objetos antes da abertura para a matrizRotulada
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						//Primeiro decrementa o número de objetos representados na matrizObjetos e verifica se o objeto continuará existindo.
						//Os objetos existentes na matrizAbertura terão novos rótulos, então precisamos decrementar o número de objetos ou apagar os elementos que deixarão de representar objetos.
						rotuloijk = matrizRotulada->data3D[i][j][k];
						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento existe
							if(it->second.NumObjs() > 1){ // o elemento representa mais de um objeto
								--(it->second);							// decrementa o número de objetos representados
							}else{												// o elemento representa um ou menos objetos
								matrizObjetos.erase(it);		// apaga o elemento.
							}
						}
						//Agora atualiza a matriz rotulada com os novos rótulos e cria/incremeta os obejtos na matrizObejtos
						rotuloijk = matrizRotulo->data3D[i][j][k] + nObjetosAntesAbertura - 1;// menos 1 para não contar fundo novamente
						matrizRotulada->data3D[i][j][k] = rotuloijk;

						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento foi encontrado
							++(it->second);							// incrementa o número de objetos representados
						}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							matrizObjetos[rotuloijk] = CObjetoImagem( SITIO, 1);
						}
					}
					// Identificando complemento da abertura:
					// Se o pixel analizado for INDICE em pm, inverte os valores da matrizAbertura assinalando como matriz complementar
					if ( pm->data3D[i][j][k] ==	INDICE ) {
						if ( matrizAbertura->data3D[i][j][k] == FUNDO and matrizLigacoes->data3D[i][j][k] == FUNDO ) {
							matrizAbertura->data3D[i][j][k] = INDICE;
						} else {
							matrizAbertura->data3D[i][j][k] = FUNDO;
						}
					}
				}
			}
		}

		os.str(""); os << "MatrizComplementoAbertura_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		cout << "-->Rotulando matriz abertura complementar...\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );//nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		// Acumula o número de objetos depois da abertura como o número de objetos do complemento da abertura
		nObjetosAberturaComplementar = nObjetosDepoisAbertura + matrizRotulo->NumeroObjetos() - 1;
		cout << "-->Num. objetos na abertura complementar = " << matrizRotulo->NumeroObjetos() << endl;

		// Copia para a matrizRotulada os rótulos do complemento da matriz abertura, acrescidos do nObjetosDepoisAbertura, de forma que os rótulos sejam sequenciais
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,it) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// Se o pixel analizado é INDICE na matriz Abertura, copia o rótulo do acrescidos do número de objetos antes da abertura para a matrizRotulada
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						//Primeiro decrementa o número de objetos representados na matrizObjetos e verifica se o objeto continuará existindo.
						//Os objetos existentes na matrizAbertura terão novos rótulos, então precisamos decrementar o número de objetos ou apagar os elementos que deixarão de representar objetos.
						rotuloijk = matrizRotulada->data3D[i][j][k];
						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento existe
							if(it->second.NumObjs() > 1){ // o elemento representa mais de um objeto
								--(it->second);							// decrementa o número de objetos representados
							}else{												// o elemento represente um ou menos objetos
								matrizObjetos.erase(it);		// apaga o elemento.
							}
						}
						//Agora atualiza a matriz rotulada com os novos rótulos e cria/incremeta os obejtos na matrizObejtos
						//Os objetos identificados no complemento da abertura, inicialmente são considerados ramos mortos; depois iremos verificar quais ramos representam ligações.
						rotuloijk = matrizRotulo->data3D[i][j][k] + nObjetosDepoisAbertura - 1;
						matrizRotulada->data3D[i][j][k] = rotuloijk;
						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento foi encontrado
							++(it->second);								// incrementa o número de objetos representados
						}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							matrizObjetos[rotuloijk] = CObjetoImagem( RAMO_MORTO, 1);
						}
					}
				}
			}
		}

		borda = 1;
		cout << "-->Comparando matrizes e fazendo conexões..." << endl ;
		// Identifica os vizinhos e caso o rótulo ainda não tenha sido incluído, faz a conexão.
		// lembre-se que set não tem repeticao, e sConexao é do tipo set<int>.
		// Aqui a matrizRotulada tem SOLIDOs, POROs, SITIOs e RAMOs_MORTOs
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,rim1,rip1,rjm1,rjp1,rkm1,rkp1,it) //schedule(dynamic,10)
		for ( i = borda; i < (nx-borda); ++i) {
			for ( j = borda; j < (ny-borda); ++j) {
				for ( k = borda; k < (nz-borda); ++k) {
					rotuloijk = matrizRotulada->data3D[i][j][k];
					// Só devemos considerar os rotulos do complemento da abertura
					if ( rotuloijk	>= nObjetosDepoisAbertura ) {
						it = matrizObjetos.find(rotuloijk);
						rim1 = matrizRotulada->data3D[i-1][j][k];
						rip1 = matrizRotulada->data3D[i+1][j][k];
						rjm1 = matrizRotulada->data3D[i][j-1][k];
						rjp1 = matrizRotulada->data3D[i][j+1][k];
						rkm1 = matrizRotulada->data3D[i][j][k-1];
						rkp1 = matrizRotulada->data3D[i][j][k+1];
						rim1jm1 = matrizRotulada->data3D[i-1][j-1][k];
						rim1jp1 = matrizRotulada->data3D[i-1][j+1][k];
						rim1km1 = matrizRotulada->data3D[i-1][j][k-1];
						rim1kp1 = matrizRotulada->data3D[i-1][j][k+1];
						rip1jp1 = matrizRotulada->data3D[i+1][j+1][k];
						rip1jm1 = matrizRotulada->data3D[i+1][j-1][k];
						rip1kp1 = matrizRotulada->data3D[i+1][j][k+1];
						rip1km1 = matrizRotulada->data3D[i+1][j][k-1];
						rjm1km1 = matrizRotulada->data3D[i][j-1][k-1];
						rjm1kp1 = matrizRotulada->data3D[i][j-1][k+1];
						rjp1km1 = matrizRotulada->data3D[i][j+1][k-1];
						rjp1kp1 = matrizRotulada->data3D[i][j+1][k+1];

						// Se os rotulos são diferentes, fazem parte da matriz abertura e o vizinho é um sítio, então, marca a conexão.
						if ( rotuloijk != rim1 and matrizObjetos[rim1].Tipo() == SITIO)
							it->second.Conectar( rim1 );
						if ( rotuloijk != rip1 and matrizObjetos[rip1].Tipo() == SITIO)
							it->second.Conectar( rip1 );
						if ( rotuloijk != rjm1 and matrizObjetos[rjm1].Tipo() == SITIO)
							it->second.Conectar( rjm1 );
						if ( rotuloijk != rjp1 and matrizObjetos[rjp1].Tipo() == SITIO)
							it->second.Conectar( rjp1 );
						if ( rotuloijk != rkm1 and matrizObjetos[rkm1].Tipo() == SITIO)
							it->second.Conectar( rkm1 );
						if ( rotuloijk != rkp1 and matrizObjetos[rkp1].Tipo() == SITIO)
							it->second.Conectar( rkp1 );
						if ( rotuloijk != rim1jm1 and matrizObjetos[rim1jm1].Tipo() == SITIO)
							it->second.Conectar( rim1jm1 );
						if ( rotuloijk != rim1jp1 and matrizObjetos[rim1jp1].Tipo() == SITIO)
							it->second.Conectar( rim1jp1 );
						if ( rotuloijk != rim1km1 and matrizObjetos[rim1km1].Tipo() == SITIO)
							it->second.Conectar( rim1km1 );
						if ( rotuloijk != rim1kp1 and matrizObjetos[rim1kp1].Tipo() == SITIO)
							it->second.Conectar( rim1kp1 );
						if ( rotuloijk != rip1jp1 and matrizObjetos[rip1jp1].Tipo() == SITIO)
							it->second.Conectar( rip1jp1 );
						if ( rotuloijk != rip1jm1 and matrizObjetos[rip1jm1].Tipo() == SITIO)
							it->second.Conectar( rip1jm1 );
						if ( rotuloijk != rip1kp1 and matrizObjetos[rip1kp1].Tipo() == SITIO)
							it->second.Conectar( rip1kp1 );
						if ( rotuloijk != rip1km1 and matrizObjetos[rip1km1].Tipo() == SITIO)
							it->second.Conectar( rip1km1 );
						if ( rotuloijk != rjm1km1 and matrizObjetos[rjm1km1].Tipo() == SITIO)
							it->second.Conectar( rjm1km1 );
						if ( rotuloijk != rjm1kp1 and matrizObjetos[rjm1kp1].Tipo() == SITIO)
							it->second.Conectar( rjm1kp1 );
						if ( rotuloijk != rjp1km1 and matrizObjetos[rjp1km1].Tipo() == SITIO)
							it->second.Conectar( rjp1km1 );
						if ( rotuloijk != rjp1kp1 and matrizObjetos[rjp1kp1].Tipo() == SITIO)
							it->second.Conectar( rjp1kp1 );
					}
				}
			}
		}

		// Agora vamos percorrer os objetos anotados como RAMOs_MORTOs e identificar as ligações
		// Uma vez identificadas as ligações, podemos armazenar o resultado na matrizLigacoes.
		// Em seguida, aproveita o loop para restaurar a matrizAbertura para o estado anterior.
		cout << "-->Identificando sitios e ligações..." << endl ;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// matrizAbertura armazena a abertura complementar, tem RAMOs_MORTOs e LIGACOES
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						rotuloijk = matrizRotulada->data3D[i][j][k];
						if ( matrizObjetos[rotuloijk].SConexao().size() > 1 ) {
							matrizObjetos[rotuloijk].Tipo( LIGACAO );
							matrizLigacoes->data3D[i][j][k] = INDICE;
							matrizSitios->data3D[i][j][k] = FUNDO;
						}else{
							matrizObjetos[rotuloijk].Tipo( SITIO );
							matrizSitios->data3D[i][j][k] = INDICE;
							matrizLigacoes->data3D[i][j][k] = FUNDO;
						}
					}
					// restaura a matrizAbertura.
					matrizAbertura->data3D[i][j][k] = pm->data3D[i][j][k];
				}
			}
		}

		//os.str(""); os << "MatrizSitios_" << raioEE << ".dbm";
		//SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

		//os.str(""); os << "MatrizLigacoes_" << raioEE << ".dbm";
		//SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

		// Atualizando o número de objetos antes da abertura para o próximo passo.
		nObjetosAntesAbertura = nObjetosAberturaComplementar;

		// Incrementando raio do Elemento Estruturante
		raioEE += incrementoRaioElementoEstruturante;
	} // fim do While

	// Libera memória.
	delete matrizAbertura;
	delete matrizRotulada;

	cout << "==>Preparando filtro..."; cout.flush(); timing = omp_get_wtime();
	pfmf->Go( matrizSitios );
	cout << "\t\t\t\ttempo: " << omp_get_wtime()-timing << " s." << endl;

	cout << "==>Processando Dilataço..."; cout.flush(); timing = omp_get_wtime();
	pfmf->Dilatacao(matrizSitios, raioEEDilatacao );
	cout << "\t\t\ttempo: " << omp_get_wtime()-timing << " s." << endl;

	cout << "==>Corrigindo sítios e ligações..." << endl ;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				if( pm->data3D[i][j][k] == FUNDO) {
					matrizSitios->data3D[i][j][k] = FUNDO;
					matrizLigacoes->data3D[i][j][k] = FUNDO;
				} else if ( matrizSitios->data3D[i][j][k] == INDICE ) {
					matrizLigacoes->data3D[i][j][k] = FUNDO;
				} else {
					matrizLigacoes->data3D[i][j][k] = INDICE;
				}
			}
		}
	}
	cout << "==>Tempo total de execução: " << (omp_get_wtime()-totaltiming)/60 << " min." << endl;
}

/* ========================= Modelo 10 =========================*/
void CAberturaDilatacao3D::DistSitiosLigacoes_Modelo_10() {
	// Variáveis auxiliares
	ostringstream os;
	double timing, totaltiming = omp_get_wtime();
	int nObjetosAntesAbertura = 0;	// número de objetos existentes na matriz pm antes de sofrer a abertura
	int nObjetosDepoisAbertura = 0;	// número de objetos existentes na matriz pm depois de sofrer a abertura
	int nObjetosAberturaComplementar = 0; //número de objetos existentes na matriz complementar da matriz abertura
	int numObjetos = 0; // Armazena o número de identificados na rotulagem
	int nx = pm->NX();
	int ny = pm->NY();
	int nz = pm->NZ();
	int raioEE = 1;
	int meioNX = nx/2;
	int i, j, k, rotuloijk, borda;
	int rim1, rip1, rjm1, rjp1, rkm1, rkp1;
	int rim1jm1, rim1jp1, rim1km1, rim1kp1, rip1jp1, rip1jm1, rip1kp1, rip1km1, rjm1km1, rjm1kp1, rjp1kp1, rjp1km1;

	// Cria matriz abertura, cópia de pm.
	TCMatriz3D<bool>* matrizAbertura = new TCMatriz3D<bool>( *pm );
	matrizAbertura->SetFormato( D1_X_Y_Z_ASCII );

	// Rotula matrizRotulo;
	cout << "==>Rotulando Imagem...\t\t\t"; cout.flush(); timing = omp_get_wtime();
	matrizRotulo->Go( matrizAbertura );
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	numObjetos = nObjetosAntesAbertura = matrizRotulo->NumeroObjetos();

	// Cria matriz que irá armazenar os rótulos identificados nas diversas operações de abertura
	TCMatriz3D<int>* matrizRotulada = new TCMatriz3D<int>( *matrizRotulo );

	cout << "-->Preparando filtro...\t\t\t\t"; cout.flush(); timing = omp_get_wtime();
	pfmf->Go( matrizAbertura );//chama calculo idf 1x
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	cout << "Entrando no looping para indentificar poros e gargantas..." << endl ;
	while ( (numObjetos > 1) and (raioEE <= meioNX) and (raioEE <= raioMaximoElementoEstruturante) ) {
		cout << "==>Executando passo = " << raioEE << endl;

		cout << "-->Processando Abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		pfmf->Abertura( matrizAbertura, raioEE );//para cada raio calcula abertura nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		//cout << "==>Processando Dilataço..."; cout.flush(); timing = omp_get_wtime();
		//pfmf->Dilatacao(matrizAbertura, raioEEDilatacao );
		//cout << "\t\t\ttempo: " << omp_get_wtime()-timing << " s." << endl;

		cout << "-->Rotulando matriz abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );//rotula nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		// Atualiza o número de objetos identificados na imagem após a operação de abertura.
		numObjetos = matrizRotulo->NumeroObjetos();
		// Acumula o número de objeto antes e depois da abertura
		nObjetosDepoisAbertura = nObjetosAntesAbertura + numObjetos - 1; // menos 1 para não contar fundo novamente

		// Copia a matriz abertura rotulada (matrizRotulo) para a matrizRotulada,
		// assim, a matrizRotulada terá também a informação dos rótulos dos sítios identificados.
		// A seguir, inverte a região porosa na matrizAbertura de forma que esta passa a ser o complemento da abertura
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,it) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// Se o pixel analizado é INDICE na matrizAbertura, copia o rótulo acrescidos do número de objetos antes da abertura para a matrizRotulada
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						//Primeiro decrementa o número de objetos representados na matrizObjetos e verifica se o objeto continuará existindo.
						//Os objetos existentes na matrizAbertura terão novos rótulos, então precisamos decrementar o número de objetos ou apagar os elementos que deixarão de representar objetos.
						rotuloijk = matrizRotulada->data3D[i][j][k];
						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento existe
							if(it->second.NumObjs() > 1){ // o elemento representa mais de um objeto
								--(it->second);							// decrementa o número de objetos representados
							}else{												// o elemento representa um ou menos objetos
								matrizObjetos.erase(it);		// apaga o elemento.
							}
						}
						//Agora atualiza a matriz rotulada com os novos rótulos e cria/incremeta os obejtos na matrizObejtos
						rotuloijk = matrizRotulo->data3D[i][j][k] + nObjetosAntesAbertura - 1;// menos 1 para não contar fundo novamente
						matrizRotulada->data3D[i][j][k] = rotuloijk;

						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento foi encontrado
							++(it->second);							// incrementa o número de objetos representados
						}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							matrizObjetos[rotuloijk] = CObjetoImagem( SITIO, 1);
						}
					}
					// Identificando complemento da abertura:
					// Se o pixel analizado for INDICE em pm, inverte os valores da matrizAbertura assinalando como matriz complementar
					if ( pm->data3D[i][j][k] ==	INDICE ) {
						if ( matrizAbertura->data3D[i][j][k] == FUNDO and matrizLigacoes->data3D[i][j][k] == FUNDO ) {
							matrizAbertura->data3D[i][j][k] = INDICE;
						} else {
							matrizAbertura->data3D[i][j][k] = FUNDO;
						}
					}
				}
			}
		}

		cout << "-->Rotulando matriz abertura complementar...\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );//nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		// Acumula o número de objetos depois da abertura como o número de objetos do complemento da abertura
		nObjetosAberturaComplementar = nObjetosDepoisAbertura + matrizRotulo->NumeroObjetos() - 1;

		// Copia para a matrizRotulada os rótulos do complemento da matriz abertura, acrescidos do nObjetosDepoisAbertura, de forma que os rótulos sejam sequenciais
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,it) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// Se o pixel analizado é INDICE na matriz Abertura, copia o rótulo do acrescidos do número de objetos antes da abertura para a matrizRotulada
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						//Primeiro decrementa o número de objetos representados na matrizObjetos e verifica se o objeto continuará existindo.
						//Os objetos existentes na matrizAbertura terão novos rótulos, então precisamos decrementar o número de objetos ou apagar os elementos que deixarão de representar objetos.
						rotuloijk = matrizRotulada->data3D[i][j][k];
						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento existe
							if(it->second.NumObjs() > 1){ // o elemento representa mais de um objeto
								--(it->second);							// decrementa o número de objetos representados
							}else{												// o elemento represente um ou menos objetos
								matrizObjetos.erase(it);		// apaga o elemento.
							}
						}
						//Agora atualiza a matriz rotulada com os novos rótulos e cria/incremeta os obejtos na matrizObejtos
						//Os objetos identificados no complemento da abertura, inicialmente são considerados ramos mortos; depois iremos verificar quais ramos representam ligações.
						rotuloijk = matrizRotulo->data3D[i][j][k] + nObjetosDepoisAbertura - 1;
						matrizRotulada->data3D[i][j][k] = rotuloijk;
						it = matrizObjetos.find(rotuloijk);
						if(it != matrizObjetos.end()){  // o elemento foi encontrado
							++(it->second);								// incrementa o número de objetos representados
						}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							matrizObjetos[rotuloijk] = CObjetoImagem( RAMO_MORTO, 1);
						}
					}
				}
			}
		}

		borda = 1;
		cout << "-->Comparando matrizes e fazendo conexões..." << endl ;
		// Identifica os vizinhos e caso o rótulo ainda não tenha sido incluído, faz a conexão.
		// lembre-se que set não tem repeticao, e sConexao é do tipo set<int>.
		// Aqui a matrizRotulada tem SOLIDOs, POROs, SITIOs e RAMOs_MORTOs
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,rim1,rip1,rjm1,rjp1,rkm1,rkp1,it) //schedule(dynamic,10)
		for ( i = borda; i < (nx-borda); ++i) {
			for ( j = borda; j < (ny-borda); ++j) {
				for ( k = borda; k < (nz-borda); ++k) {
					rotuloijk = matrizRotulada->data3D[i][j][k];
					// Só devemos considerar os rotulos do complemento da abertura
					if ( rotuloijk	>= nObjetosDepoisAbertura ) {
						it = matrizObjetos.find(rotuloijk);
						rim1 = matrizRotulada->data3D[i-1][j][k];
						rip1 = matrizRotulada->data3D[i+1][j][k];
						rjm1 = matrizRotulada->data3D[i][j-1][k];
						rjp1 = matrizRotulada->data3D[i][j+1][k];
						rkm1 = matrizRotulada->data3D[i][j][k-1];
						rkp1 = matrizRotulada->data3D[i][j][k+1];
						rim1jm1 = matrizRotulada->data3D[i-1][j-1][k];
						rim1jp1 = matrizRotulada->data3D[i-1][j+1][k];
						rim1km1 = matrizRotulada->data3D[i-1][j][k-1];
						rim1kp1 = matrizRotulada->data3D[i-1][j][k+1];
						rip1jp1 = matrizRotulada->data3D[i+1][j+1][k];
						rip1jm1 = matrizRotulada->data3D[i+1][j-1][k];
						rip1kp1 = matrizRotulada->data3D[i+1][j][k+1];
						rip1km1 = matrizRotulada->data3D[i+1][j][k-1];
						rjm1km1 = matrizRotulada->data3D[i][j-1][k-1];
						rjm1kp1 = matrizRotulada->data3D[i][j-1][k+1];
						rjp1km1 = matrizRotulada->data3D[i][j+1][k-1];
						rjp1kp1 = matrizRotulada->data3D[i][j+1][k+1];

						// Se os rotulos são diferentes, fazem parte da matriz abertura e o vizinho é um sítio, então, marca a conexão.
						if ( rotuloijk != rim1 and matrizObjetos[rim1].Tipo() == SITIO)
							it->second.Conectar( rim1 );
						if ( rotuloijk != rip1 and matrizObjetos[rip1].Tipo() == SITIO)
							it->second.Conectar( rip1 );
						if ( rotuloijk != rjm1 and matrizObjetos[rjm1].Tipo() == SITIO)
							it->second.Conectar( rjm1 );
						if ( rotuloijk != rjp1 and matrizObjetos[rjp1].Tipo() == SITIO)
							it->second.Conectar( rjp1 );
						if ( rotuloijk != rkm1 and matrizObjetos[rkm1].Tipo() == SITIO)
							it->second.Conectar( rkm1 );
						if ( rotuloijk != rkp1 and matrizObjetos[rkp1].Tipo() == SITIO)
							it->second.Conectar( rkp1 );
						if ( rotuloijk != rim1jm1 and matrizObjetos[rim1jm1].Tipo() == SITIO)
							it->second.Conectar( rim1jm1 );
						if ( rotuloijk != rim1jp1 and matrizObjetos[rim1jp1].Tipo() == SITIO)
							it->second.Conectar( rim1jp1 );
						if ( rotuloijk != rim1km1 and matrizObjetos[rim1km1].Tipo() == SITIO)
							it->second.Conectar( rim1km1 );
						if ( rotuloijk != rim1kp1 and matrizObjetos[rim1kp1].Tipo() == SITIO)
							it->second.Conectar( rim1kp1 );
						if ( rotuloijk != rip1jp1 and matrizObjetos[rip1jp1].Tipo() == SITIO)
							it->second.Conectar( rip1jp1 );
						if ( rotuloijk != rip1jm1 and matrizObjetos[rip1jm1].Tipo() == SITIO)
							it->second.Conectar( rip1jm1 );
						if ( rotuloijk != rip1kp1 and matrizObjetos[rip1kp1].Tipo() == SITIO)
							it->second.Conectar( rip1kp1 );
						if ( rotuloijk != rip1km1 and matrizObjetos[rip1km1].Tipo() == SITIO)
							it->second.Conectar( rip1km1 );
						if ( rotuloijk != rjm1km1 and matrizObjetos[rjm1km1].Tipo() == SITIO)
							it->second.Conectar( rjm1km1 );
						if ( rotuloijk != rjm1kp1 and matrizObjetos[rjm1kp1].Tipo() == SITIO)
							it->second.Conectar( rjm1kp1 );
						if ( rotuloijk != rjp1km1 and matrizObjetos[rjp1km1].Tipo() == SITIO)
							it->second.Conectar( rjp1km1 );
						if ( rotuloijk != rjp1kp1 and matrizObjetos[rjp1kp1].Tipo() == SITIO)
							it->second.Conectar( rjp1kp1 );
					}
				}
			}
		}

		// Agora vamos percorrer os objetos anotados como RAMOs_MORTOs e identificar as ligações
		// Uma vez identificadas as ligações, podemos armazenar o resultado na matrizLigacoes.
		// Em seguida, aproveita o loop para restaurar a matrizAbertura para o estado anterior.
		cout << "-->Identificando sitios e ligações..." << endl ;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// matrizAbertura armazena a abertura complementar, tem RAMOs_MORTOs e LIGACOES
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						rotuloijk = matrizRotulada->data3D[i][j][k];
						if ( matrizObjetos[rotuloijk].SConexao().size() > 1 ) {
							matrizObjetos[rotuloijk].Tipo( LIGACAO );
							matrizLigacoes->data3D[i][j][k] = INDICE;
							matrizSitios->data3D[i][j][k] = FUNDO;
						}else{
							matrizObjetos[rotuloijk].Tipo( SITIO );
							matrizSitios->data3D[i][j][k] = INDICE;
							matrizLigacoes->data3D[i][j][k] = FUNDO;
						}
					}
					// restaura a matrizAbertura.
					matrizAbertura->data3D[i][j][k] = pm->data3D[i][j][k];
				}
			}
		}

		// Atualizando o número de objetos antes da abertura para o próximo passo.
		nObjetosAntesAbertura = nObjetosAberturaComplementar;

		// Incrementando raio do Elemento Estruturante
		raioEE += incrementoRaioElementoEstruturante;
	} // fim do While

	// Libera memória.
	delete matrizAbertura;
	delete matrizRotulada;

	cout << "==>Corrigindo sítios e ligações..." << endl ;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				if( pm->data3D[i][j][k] == FUNDO) {
					matrizSitios->data3D[i][j][k] = FUNDO;
					matrizLigacoes->data3D[i][j][k] = FUNDO;
				} else if ( matrizSitios->data3D[i][j][k] == INDICE ) {
					matrizLigacoes->data3D[i][j][k] = FUNDO;
				} else {
					matrizLigacoes->data3D[i][j][k] = INDICE;
				}
			}
		}
	}
	matrizSitios->Write("MatrizSitios.dbm");
	matrizLigacoes->Write("MatrizLigacoes.dbm");

	cout << "==>Tempo total de execução: " << (omp_get_wtime()-totaltiming)/60 << " min." << endl;
}
