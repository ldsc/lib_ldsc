#include <AnaliseImagem/Segmentacao/PorosGargantas/CAberturaDilatacao3D.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd3453D.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIRA3D.h>
#include <iomanip>
#include <cmath>
#include <omp.h>

using namespace std;

CAberturaDilatacao3D::CAberturaDilatacao3D( TCMatriz3D<bool>* &matriz, int _indice, int _fundo)
	: CMatrizObjetoImagem(), pm(matriz), // pm é ponteiro para imagem externa (se mudar externamente teremos problemas).
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
	: CMatrizObjetoImagem(), fatorReducaoRaioElemEst (1), raioMaximoElementoEstruturante ( 500 ), // usar limits
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

	if ( matrizAuxiliar == nullptr ) {
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
	if (fout.good()){
		//menores valores para nx, ny e nz.
		int nx = pm->NX();
		int ny = pm->NY();
		int nz = pm->NZ();

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
	if (gerarDetalhesObjetos or salvarResultadosParciais) {
		return CMatrizObjetoImagem::SalvarListaObjetos(fileName, pm->NX(), pm->NY(), pm->NZ());
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
		mat->NumCores(mat->MaiorValor()+1);
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
		case ONZE: DistSitiosLigacoes_Modelo_11();
			break;
	}
	matrizObjetos.clear();
	GerarDetalhesMatrizObjetos();
}

void CAberturaDilatacao3D::ConectarObjetos(TCMatriz3D<int>* &mat, bool zerarConexoes) {
	map<int,CObjetoImagem>::iterator it1;
	map<int,CObjetoImagem>::iterator itp;
	int i,j,k,rijk,rip1,rjp1,rkp1;
	int nx = mat->NX();
	int ny = mat->NY();
	int nz = mat->NZ();
	if (zerarConexoes) { // se a flag estiver setada, apaga todos as conexões por ventura existentes
		for ( it1 = matrizObjetos.begin(); it1 != matrizObjetos.end(); ++it1) {
			it1->second.SConexao().clear();
		}
	}
	for ( i = 0; i < (nx-1); ++i) {
		for ( j = 0; j < (ny-1); ++j) {
			for ( k = 0; k < (nz-1); ++k) {
				if ( mat->data3D[i][j][k] > 0 ) {
					rijk = mat->data3D[i][j][k];
					it1 = matrizObjetos.find(rijk);
					rip1 = mat->data3D[i+1][j][k];
					rjp1 = mat->data3D[i][j+1][k];
					rkp1 = mat->data3D[i][j][k+1];

					if ( rijk != rip1 && rip1 > 0 ) {
						itp = matrizObjetos.find(rip1);
						itp->second.Conectar( rijk );
						it1->second.Conectar( rip1 );
					}
					if ( rijk != rjp1 && rjp1 > 0 ) {
						itp = matrizObjetos.find(rjp1);
						itp->second.Conectar( rijk );
						it1->second.Conectar( rjp1 );
					}
					if ( rijk != rkp1 && rkp1 > 0 ) {
						itp = matrizObjetos.find(rkp1);
						itp->second.Conectar( rijk );
						it1->second.Conectar( rkp1 );
					}
				}
			}
		}
	}
}

int CAberturaDilatacao3D::RotularECriarObjetos(TCMatriz3D<bool>* &matOriginal, TCMatriz3D<int>* &matRotulo, ETipoObjetoImagem tipo, int vlr ) {
	int i, j, k, rotuloijk;
	int nx = matOriginal->NX();
	int ny = matOriginal->NY();
	int nz = matOriginal->NZ();
	matrizRotulo->Go( matOriginal );
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				rotuloijk = matrizRotulo->data3D[i][j][k];
				if (rotuloijk != 0) {
					rotuloijk += vlr;
					matRotulo->data3D[i][j][k] = rotuloijk;
					it = matrizObjetos.find(rotuloijk);
					if ( it != matrizObjetos.end() ) {  // o elemento foi encontrado
						++(it->second);							// incrementa o número de objetos representados
					} else {													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
						matrizObjetos[rotuloijk] = CObjetoImagem( tipo, 1);
					}
				}
			}
		}
	}
	return matrizRotulo->NumeroObjetos() - 1; // -1 para não contar fundo
}

void CAberturaDilatacao3D::GerarDetalhesMatrizObjetos() {
	// Se a flag estiver setada
	if (gerarDetalhesObjetos) {
		// Variáveis auxiliares
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
						matrizRotulada->data3D[i][j][k] = rotulo; // para que os rótulos sejam sequenciais
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
		ConectarObjetos(matrizRotulada, true);

		// Libera memória
		delete matrizRotulada;
	}
}


/* ========================= Modelo 7 =========================
*	 Executa abertura para segmentar poros e gargantas e no final realiza dilatação na matriz de poros.
*  Após dilatar a matriz de poros rotula e verifica novamente a conexão entre os objetos fazendo com que ramos mortos virem sítio
*/
void CAberturaDilatacao3D::DistSitiosLigacoes_Modelo_7() {
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
	int borda = 1;
	int nx_borda = nx-borda;
	int ny_borda = ny-borda;
	int nz_borda = nz-borda;
	int raioEE = 1;
	int meioNX = nx/2;
	int i, j, k, rotuloijk;
	int rim1, rip1, rjm1, rjp1, rkm1, rkp1;

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

		os.str(""); os << "MatrizAberturaRotulada_" << raioEE << ".dgm";
		SalvarResultadosParciaisEmDisco( matrizRotulo, os.str() );

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

		os.str(""); os << "MatrizAberturaComplementar_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		cout << "-->Rotulando matriz abertura complementar...\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );//nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		os.str(""); os << "MatrizAberturaComplementarRotulada_" << raioEE << ".dgm";
		SalvarResultadosParciaisEmDisco( matrizRotulo, os.str() );

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

		cout << "-->Comparando matrizes e fazendo conexões..." << endl ;
		// Identifica os vizinhos e caso o rótulo ainda não tenha sido incluído, faz a conexão.
		// lembre-se que set não tem repeticao, e sConexao é do tipo set<int>.
		// Aqui a matrizRotulada tem SOLIDOs, POROs, SITIOs e RAMOs_MORTOs
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,rim1,rip1,rjm1,rjp1,rkm1,rkp1,it) //schedule(dynamic,10)
		for ( i = borda; i < nx_borda; ++i) {
			for ( j = borda; j < ny_borda; ++j) {
				for ( k = borda; k < nz_borda; ++k) {
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

		os.str(""); os << "MatrizSitios_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

		os.str(""); os << "MatrizLigacoes_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

		// Atualizando o número de objetos antes da abertura para o próximo passo.
		nObjetosAntesAbertura = nObjetosAberturaComplementar;

		// Incrementando raio do Elemento Estruturante
		raioEE += incrementoRaioElementoEstruturante;
	} // fim do While

	// Libera memória.
	delete matrizAbertura;

	os.str(""); os << "MatrizSitiosAntesDilatacao" << ".dbm";
	SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

	os.str(""); os << "MatrizLigacoesAntesDilatacao" << ".dbm";
	SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

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

	cout << "==>Rotulando a matrizSitios..." << endl ;
	matrizRotulo->Go(matrizSitios);

	//cout << "==>Determinando IDF da matrizSitios..." << endl ;
	//pfmf->Go(matrizSitios);

	// Armazena o número de objetos identificados na rotulagem menos o fundo.
	numObjetos = matrizRotulo->NumeroObjetos() - 1;

	//zera matrizObjetos
	matrizObjetos.clear();

	cout << "==>Alimentando a matrizObjetos com os objetos identificados na rotulagem dos sítios..." << endl ;
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				rotuloijk = matrizRotulo->data3D[i][j][k];
				matrizRotulada->data3D[i][j][k] = rotuloijk;
				if ( rotuloijk != 0) {
					it = matrizObjetos.find(rotuloijk);
					if(it != matrizObjetos.end()){  // o elemento foi encontrado
						++(it->second);							// incrementa o número de objetos representados
					}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
						matrizObjetos[rotuloijk] = CObjetoImagem( SITIO, 1 );
					}
					//it->second.PontoCentral( i, j, k, pfmf->data3D[i][j][k] );
				}
			}
		}
	}


	cout << "==>Rotulando a matrizLigacoes..." << endl ;
	matrizRotulo->Go(matrizLigacoes);

	//cout << "==>Determinando IDF da matrizLigacoes..." << endl ;
	//pfmf->Go(matrizLigacoes);

	cout << "==>Alimentando a matrizObjetos com os objetos identificados na rotulagem das ligações..." << endl ;
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				rotuloijk = matrizRotulo->data3D[i][j][k];
				if ( rotuloijk > 0 ) { //só entra se for diferente de fundo, ou seja, neste caso, maior que numObjs
					rotuloijk = rotuloijk + numObjetos;
					matrizRotulo->data3D[i][j][k] = rotuloijk; // para que os rótulos sejam sequenciais
					it = matrizObjetos.find(rotuloijk);
					if(it != matrizObjetos.end()){  // o elemento foi encontrado
						++(it->second);							// incrementa o número de objetos representados
					}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
						matrizObjetos[rotuloijk] = CObjetoImagem( LIGACAO, 1);
					}
					//it->second.PontoCentral( i, j, k, pfmf->data3D[i][j][k] );
				}
			}
		}
	}

	map<int,CObjetoImagem>::iterator itt;
	// Realizar conexões entre os objetos.
	for ( i = 0; i < nx_borda; ++i) {
		for ( j = 0; j < ny_borda; ++j) {
			for ( k = 0; k < nz_borda; ++k) {
				rotuloijk = matrizRotulo->data3D[i][j][k];
				if ( rotuloijk	> 0 ) {
					it = matrizObjetos.find(rotuloijk);
					rip1 = matrizRotulada->data3D[i+1][j][k];
					rjp1 = matrizRotulada->data3D[i][j+1][k];
					rkp1 = matrizRotulada->data3D[i][j][k+1];

					// Se os rotulos não representam sólido, e o vizinho é um sítio, então, marca a conexão.
					if ( rip1 > 0 ) {
						itt = matrizObjetos.find(rip1);
						itt->second.Conectar( rotuloijk );
						it->second.Conectar( rip1 );
					}
					if ( rjp1 > 0 ) {
						itt = matrizObjetos.find(rjp1);
						itt->second.Conectar( rotuloijk );
						it->second.Conectar( rjp1 );
					}
					if ( rkp1 > 0 ) {
						itt = matrizObjetos.find(rkp1);
						itt->second.Conectar( rotuloijk );
						it->second.Conectar( rkp1 );
					}
				}
			}
		}
	}

	//Ligações conectadas a um único sítio serão consideradas sítios.
	for ( i = 0; i < nx; ++i ) {
		for ( j = 0; j < ny; ++j ) {
			for ( k = 0; k < nz; ++k ) {
				if ( matrizLigacoes->data3D[i][j][k] == INDICE ) {
					rotuloijk = matrizRotulo->data3D[i][j][k];
					if ( matrizObjetos[rotuloijk].SConexao().size() < 2 ) {
						matrizLigacoes->data3D[i][j][k] = FUNDO;
						matrizSitios->data3D[i][j][k] = INDICE;
					}
				}
			}
		}
	}

	os.str(""); os << "MatrizSitiosFinal" << ".dbm";
	SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

	os.str(""); os << "MatrizLigacoesFinal" << ".dbm";
	SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

	delete matrizRotulada;
	cout << "==>Tempo total de execução: " << (omp_get_wtime()-totaltiming)/60 << " min." << endl;
}

/* ========================= Modelo 8 =========================
*	 Executa abertura para segmentar poros e gargantas e no final realiza dilatação na matriz de poros.
*/
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
	//int rim1jm1, rim1jp1, rim1km1, rim1kp1, rip1jp1, rip1jm1, rip1kp1, rip1km1, rjm1km1, rjm1kp1, rjp1kp1, rjp1km1;

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

		os.str(""); os << "MatrizAberturaRotulada_" << raioEE << ".dgm";
		SalvarResultadosParciaisEmDisco( matrizRotulo, os.str() );

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

		os.str(""); os << "MatrizAberturaComplementar_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		cout << "-->Rotulando matriz abertura complementar...\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );//nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		os.str(""); os << "MatrizAberturaComplementarRotulada_" << raioEE << ".dgm";
		SalvarResultadosParciaisEmDisco( matrizRotulo, os.str() );

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
						/*rim1jm1 = matrizRotulada->data3D[i-1][j-1][k];
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
						rjp1kp1 = matrizRotulada->data3D[i][j+1][k+1];*/

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
						/*						if ( rotuloijk != rim1jm1 and matrizObjetos[rim1jm1].Tipo() == SITIO)
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
							it->second.Conectar( rjp1kp1 ); */
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

		os.str(""); os << "MatrizSitios_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

		os.str(""); os << "MatrizLigacoes_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

		// Atualizando o número de objetos antes da abertura para o próximo passo.
		nObjetosAntesAbertura = nObjetosAberturaComplementar;

		// Incrementando raio do Elemento Estruturante
		raioEE += incrementoRaioElementoEstruturante;
	} // fim do While

	os.str(""); os << "MatrizSitiosAntesDilatacao" << ".dbm";
	SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

	os.str(""); os << "MatrizLigacoesAntesDilatacao" << ".dbm";
	SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

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
	os.str(""); os << "MatrizSitiosFinal" << ".dbm";
	SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

	os.str(""); os << "MatrizLigacoesFinal" << ".dbm";
	SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

	cout << "==>Tempo total de execução: " << (omp_get_wtime()-totaltiming)/60 << " min." << endl;
}

/* ========================= Modelo 9 =========================
*	Realiza abertura e dilatação a cada passo, porém só dilata enquanto o raioEE < raioEEDilatacao.
*/
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
	//int rim1jm1, rim1jp1, rim1km1, rim1kp1, rip1jp1, rip1jm1, rip1kp1, rip1km1, rjm1km1, rjm1kp1, rjp1kp1, rjp1km1;

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

	cout << "Entrando no looping para indentificar poros e gargantas..." << endl ;
	while ( (numObjetos > 1) and (raioEE <= meioNX) and (raioEE <= raioMaximoElementoEstruturante) ) {
		cout << "==>Executando passo = " << raioEE << endl;
		cout << "-->Num. objetos identificados na imagem = " << numObjetos << endl;
		cout << "-->Num. objetos antes da abertura = " << matrizRotulo->NumeroObjetos() << endl;

		//Só recalcula a IDF se o raio do EE utilizado na abertura form <= ao raio do EE utilizado na dilatação.
		if (raioEE <= raioEEDilatacao) {
			cout << "-->Preparando filtro...\t\t\t\t"; cout.flush(); timing = omp_get_wtime();
			pfmf->Go( matrizAbertura );//chama calculo idf 1x
			cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;
		}

		cout << "-->Processando Abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		pfmf->Abertura( matrizAbertura, raioEE );//para cada raio calcula abertura nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		os.str(""); os << "MatrizAbertura_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		//Só executa dilatação se o raio do EE utilizado na abertura form menor que raio do EE utilizado na dilatação.
		if (raioEE < raioEEDilatacao) {
			cout << "-->Preparando filtro...\t\t\t\t"; cout.flush(); timing = omp_get_wtime();
			pfmf->Go( matrizAbertura );//chama calculo idf 1x
			cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

			cout << "-->Processando Dilatacao...\t\t\t"; cout.flush(); timing = omp_get_wtime();
			pfmf->Dilatacao( matrizAbertura, raioEEDilatacao );//para cada raio calcula abertura nX
			cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

			cout << "==>Corrigindo matriz abertura dilatada..." << endl ;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
			for ( i = 0; i < nx; ++i) {
				for ( j = 0; j < ny; ++j) {
					for ( k = 0; k < nz; ++k) {
						if( pm->data3D[i][j][k] == FUNDO) {
							matrizAbertura->data3D[i][j][k] = FUNDO;
						}
					}
				}
			}
		}

		cout << "-->Rotulando matriz abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );//rotula nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		os.str(""); os << "MatrizAberturaRotulada_" << raioEE << ".dgm";
		SalvarResultadosParciaisEmDisco( matrizRotulo, os.str() );

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
						if ( matrizAbertura->data3D[i][j][k] == FUNDO and
								 matrizLigacoes->data3D[i][j][k] == FUNDO )
						{
							matrizAbertura->data3D[i][j][k] = INDICE;
						} else {
							matrizAbertura->data3D[i][j][k] = FUNDO;
						}
					}
				}
			}
		}

		os.str(""); os << "MatrizAberturaComplementar_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		cout << "-->Rotulando matriz abertura complementar...\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );//nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		os.str(""); os << "MatrizAberturaComplementarRotulada_" << raioEE << ".dgm";
		SalvarResultadosParciaisEmDisco( matrizRotulo, os.str() );

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

		os.str(""); os << "MatrizSitios_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

		os.str(""); os << "MatrizLigacoes_" << raioEE << ".dbm";
		SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

		// Atualizando o número de objetos antes da abertura para o próximo passo.
		nObjetosAntesAbertura = nObjetosAberturaComplementar;

		// Incrementando raio do Elemento Estruturante
		raioEE += incrementoRaioElementoEstruturante;
	} // fim do While

	// Libera memória.
	delete matrizAbertura;
	delete matrizRotulada;

	os.str(""); os << "MatrizSitiosFinal" << ".dbm";
	SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

	os.str(""); os << "MatrizLigacoesFinal" << ".dbm";
	SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

	cout << "==>Tempo total de execução: " << (omp_get_wtime()-totaltiming)/60 << " min." << endl;
}

/* ========================= Modelo 10 =========================
* Executa abertura a cada passo.
* Considera matriz ramos mortos.
* Após o loop:
* - dilata matrizes de sítios e ligações.
* - corrige matrizes.
* - converte ramos mortos em sítos e ligações
*/
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
	int raioEE = 2;
	int meioNX = nx/2;
	int i, j, k, rotuloijk, rotulov, borda;
	int rim1, rip1, rjm1, rjp1, rkm1, rkp1;
	//int rim1jm1, rim1jp1, rim1km1, rim1kp1, rip1jp1, rip1jm1, rip1kp1, rip1km1, rjm1km1, rjm1kp1, rjp1kp1, rjp1km1;
	int numConexoes, numSitios, numLigacoes;//, cont;
	bool continuar;
	//map<int,CObjetoImagem>::iterator itt;

	// Cria matriz abertura, cópia de pm.
	TCMatriz3D<bool>* matrizAbertura = new TCMatriz3D<bool>( *pm );
	matrizAbertura->SetFormato( D1_X_Y_Z_ASCII );

	//Copia pm para a matrizSitios
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				matrizSitios->data3D[i][j][k] = pm->data3D[i][j][k];
			}
		}
	}

	// Cria matriz ramos mortos vazia.
	TCMatriz3D<bool>* matrizRamosMortos = new TCMatriz3D<bool>( nx, ny, nz );
	matrizRamosMortos->Constante( FUNDO );
	matrizRamosMortos->SetFormato( D1_X_Y_Z_ASCII );

	// Rotula matrizRotulo;
	cout << "==>Rotulando Imagem...\t\t\t"; cout.flush(); timing = omp_get_wtime();
	matrizRotulo->Go( matrizAbertura );
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	numObjetos = nObjetosAntesAbertura = matrizRotulo->NumeroObjetos()-1;// menos 1 para não contar fundo

	// Cria matriz que irá armazenar os rótulos identificados nas diversas operações de abertura
	TCMatriz3D<int>* matrizRotulada = new TCMatriz3D<int>( *matrizRotulo );

	cout << "-->Preparando filtro...\t\t\t\t"; cout.flush(); timing = omp_get_wtime();
	pfmf->Go( matrizAbertura );//chama calculo idf 1x
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	cout << "Entrando no looping para indentificar poros e gargantas..." << endl ;
	while ( (numObjetos > 0) and (raioEE <= meioNX) and (raioEE <= raioMaximoElementoEstruturante) ) {
		cout << "==>Executando passo = " << raioEE << endl;
		cout << "-->Num. objetos identificados na imagem = " << numObjetos << endl;
		cout << "-->Num. objetos antes da abertura = " << nObjetosAntesAbertura << endl;

		cout << "-->Processando Abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		pfmf->Abertura( matrizAbertura, raioEE );//para cada raio calcula abertura nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		//após executar a abertura precisa corrigir indices ativos que por ventura não existam na matriz
		//original. Estudar se terá necessidade de permanecer.
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					if ( pm->data3D[i][j][k] ==	FUNDO and matrizAbertura->data3D[i][j][k] == INDICE ) {
						matrizAbertura->data3D[i][j][k] = FUNDO;
					}
				}
			}
		}

		os.str(""); os << "raio-" << raioEE << "-1-MatrizAbertura.dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		cout << "-->Rotulando matriz abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		os.str(""); os << "raio-" << raioEE << "-2-MatrizAberturaRotulada.dgm";
		SalvarResultadosParciaisEmDisco( matrizRotulo, os.str() );

		// Atualiza o número de objetos identificados na imagem após a operação de abertura.
		numObjetos = matrizRotulo->NumeroObjetos()-1; // menos 1 para não contar fundo
		// Acumula o número de objeto antes e depois da abertura
		nObjetosDepoisAbertura = nObjetosAntesAbertura + numObjetos;
		cout << "-->Num. objetos depois da abertura = " << nObjetosDepoisAbertura << endl;

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
						if ( it != matrizObjetos.end() ) {  // o elemento existe
							if ( it->second.NumObjs() > 1 ) { // o elemento representa mais de um objeto
								--(it->second);							// decrementa o número de objetos representados
							} else {												// o elemento representa um ou menos objetos
								matrizObjetos.erase(it);		// apaga o elemento.
							}
						}
						//Agora atualiza a matriz rotulada com os novos rótulos e cria/incremeta os obejtos na matrizObejtos
						rotuloijk = matrizRotulo->data3D[i][j][k] + nObjetosAntesAbertura;
						matrizRotulada->data3D[i][j][k] = rotuloijk;

						it = matrizObjetos.find(rotuloijk);
						if ( it != matrizObjetos.end() ) {  // o elemento foi encontrado
							++(it->second);							// incrementa o número de objetos representados
						} else {													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							matrizObjetos[rotuloijk] = CObjetoImagem( SITIO, 1);
						}
						matrizSitios->data3D[i][j][k] = INDICE;
						matrizLigacoes->data3D[i][j][k] = FUNDO;
						matrizRamosMortos->data3D[i][j][k] = FUNDO;
					}
				}
			}
		}

		//verificar na matrizObjetos quais vizinhos da matrizAbertura são sitios e precisam ter seu
		//rótulo atualizado
		for ( i = 1; i < nx-1; ++i) {
			for ( j = 1; j < ny-1; ++j) {
				for ( k = 1; k < nz-1; ++k) {
					if ( pm->data3D[i][j][k] == INDICE && matrizAbertura->data3D[i][j][k] == FUNDO) {
						rotuloijk = matrizRotulada->data3D[i][j][k];
						it = matrizObjetos.find(rotuloijk);
						if ( it->second.Tipo() == SITIO and it->second.NumObjs() < 7 ) {
							continuar = true;
							//verifica a vizinhança
							if (matrizAbertura->data3D[i-1][j][k] == INDICE) {
								rotulov = matrizRotulada->data3D[i-1][j][k];
								//se os rótulos forem diferentes e o vizinho for SITIO
								if ( rotuloijk != rotulov and matrizObjetos[rotulov].Tipo() == SITIO and continuar ) {
									continuar = false;
								}
							}
							if (continuar && matrizAbertura->data3D[i+1][j][k] == INDICE ) {
								rotulov = matrizRotulada->data3D[i+1][j][k];
								if ( rotuloijk != rotulov and matrizObjetos[rotulov].Tipo() == SITIO ) {
									continuar = false;
								}
								if (continuar && matrizAbertura->data3D[i][j-1][k] == INDICE) {
									rotulov = matrizRotulada->data3D[i][j-1][k];
									if ( rotuloijk != rotulov and matrizObjetos[rotulov].Tipo() == SITIO ) {
										continuar = false;
									}
									if (continuar && matrizAbertura->data3D[i][j+1][k] == INDICE) {
										rotulov = matrizRotulada->data3D[i][j+1][k];
										if ( rotuloijk != rotulov and matrizObjetos[rotulov].Tipo() == SITIO ) {
											continuar = false;
										}
										if (continuar && matrizAbertura->data3D[i][j][k-1] == INDICE) {
											rotulov = matrizRotulada->data3D[i][j][k-1];
											if ( rotuloijk != rotulov and matrizObjetos[rotulov].Tipo() == SITIO ) {
												continuar = false;
											}
											if (continuar && matrizAbertura->data3D[i][j][k+1] == INDICE) {
												rotulov = matrizRotulada->data3D[i][j][k+1];
												if ( rotuloijk != rotulov and matrizObjetos[rotulov].Tipo() == SITIO ) {
													continuar = false;
												}
											}
										}
									}
								}
							}
							if ( ! continuar ) { //Atendeu as condições. Altera as Matrizes.
								if ( it->second.NumObjs() > 1 ) {
									--(it->second);
								} else {
									matrizObjetos.erase(it);
								}
								++(matrizObjetos[rotulov]);
								matrizRotulada->data3D[i][j][k] = rotulov;
								matrizAbertura->data3D[i][j][k] = INDICE;
								matrizSitios->data3D[i][j][k] = INDICE;
								matrizLigacoes->data3D[i][j][k] = FUNDO;
								matrizRamosMortos->data3D[i][j][k] = FUNDO;
							}
						}
					}
				}
			}
		}
		os.str(""); os << "raio-" << raioEE << "-1-MatrizAberturaCorrigida.dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// Identificando complemento da abertura:
					// Se o pixel analizado for INDICE em pm, inverte os valores da matrizAbertura assinalando como matriz complementar
					// Desconsidera como complemento os pixeis assinalados com ligação ou ramo morto
					if ( pm->data3D[i][j][k] ==	INDICE ) {
						if ( matrizAbertura->data3D[i][j][k] == FUNDO &&
								 matrizLigacoes->data3D[i][j][k] == FUNDO &&
								 matrizRamosMortos->data3D[i][j][k] == FUNDO )
						{
							matrizAbertura->data3D[i][j][k] = INDICE;
						} else {
							matrizAbertura->data3D[i][j][k] = FUNDO;
						}
					} else {
						matrizAbertura->data3D[i][j][k] = FUNDO;
					}
				}
			}
		}

		os.str(""); os << "raio-" << raioEE << "-3-MatrizAberturaComplementar.dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		cout << "-->Rotulando matriz abertura complementar...\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );//nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		os.str(""); os << "raio-" << raioEE << "-4-MatrizAberturaComplementarRotulada.dgm";
		SalvarResultadosParciaisEmDisco( matrizRotulo, os.str() );

		// Acumula o número de objetos depois da abertura como o número de objetos do complemento da abertura
		nObjetosAberturaComplementar = nObjetosDepoisAbertura + matrizRotulo->NumeroObjetos() - 1;
		cout << "-->Num. objetos na abertura complementar = " << nObjetosAberturaComplementar << endl;

		// Copia para a matrizRotulada os rótulos do complemento da matriz abertura, acrescidos do nObjetosDepoisAbertura, de forma que os rótulos sejam sequenciais
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,it) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i ) {
			for ( j = 0; j < ny; ++j ) {
				for ( k = 0; k < nz; ++k ) {
					// Se o pixel analizado é INDICE na matriz Abertura, copia o rótulo acrescido do número de objetos antes da abertura para a matrizRotulada
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						//Primeiro decrementa o número de objetos representados na matrizObjetos e verifica se o objeto continuará existindo.
						//Os objetos existentes na matrizAbertura terão novos rótulos, então precisamos decrementar o número de objetos ou apagar os elementos que deixarão de representar objetos.
						rotuloijk = matrizRotulada->data3D[i][j][k];
						it = matrizObjetos.find(rotuloijk);
						if ( it != matrizObjetos.end() ) {  // o elemento existe
							if ( it->second.NumObjs() > 1 ) { // o elemento representa mais de um objeto
								--(it->second);							// decrementa o número de objetos representados
							}else{												// o elemento represente um ou menos objetos
								matrizObjetos.erase(it);		// apaga o elemento.
							}
						}
						//Agora atualiza a matriz rotulada com os novos rótulos e cria/incremeta os obejtos na matrizObejtos
						//Os objetos identificados no complemento da abertura, inicialmente são considerados ramos mortos; depois iremos verificar quais ramos representam ligações.
						rotuloijk = matrizRotulo->data3D[i][j][k] + nObjetosDepoisAbertura;
						matrizRotulada->data3D[i][j][k] = rotuloijk;
						it = matrizObjetos.find(rotuloijk);
						if ( it != matrizObjetos.end() ) {  // o elemento foi encontrado
							++(it->second);								// incrementa o número de objetos representados
						} else {													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							matrizObjetos[rotuloijk] = CObjetoImagem( RAMO_MORTO, 1);
						}
						matrizSitios->data3D[i][j][k] = FUNDO;
						matrizLigacoes->data3D[i][j][k] = FUNDO;
						matrizRamosMortos->data3D[i][j][k] = INDICE;
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
					// Só devemos considerar os rotulos do complemento da abertura
					//if ( rotuloijk	>= nObjetosDepoisAbertura ) {
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						rotuloijk = matrizRotulada->data3D[i][j][k];
						it = matrizObjetos.find(rotuloijk);
						rim1 = matrizRotulada->data3D[i-1][j][k];
						rip1 = matrizRotulada->data3D[i+1][j][k];
						rjm1 = matrizRotulada->data3D[i][j-1][k];
						rjp1 = matrizRotulada->data3D[i][j+1][k];
						rkm1 = matrizRotulada->data3D[i][j][k-1];
						rkp1 = matrizRotulada->data3D[i][j][k+1];
						/*
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
						*/
						// Se os rotulos são diferentes, fazem parte da matriz abertura e o vizinho é um sítio, então, marca a conexão.
						if ( rotuloijk != rim1 and matrizObjetos[rim1].Tipo() == SITIO )
							it->second.Conectar( rim1 );
						if ( rotuloijk != rip1 and matrizObjetos[rip1].Tipo() == SITIO )
							it->second.Conectar( rip1 );
						if ( rotuloijk != rjm1 and matrizObjetos[rjm1].Tipo() == SITIO )
							it->second.Conectar( rjm1 );
						if ( rotuloijk != rjp1 and matrizObjetos[rjp1].Tipo() == SITIO )
							it->second.Conectar( rjp1 );
						if ( rotuloijk != rkm1 and matrizObjetos[rkm1].Tipo() == SITIO )
							it->second.Conectar( rkm1 );
						if ( rotuloijk != rkp1 and matrizObjetos[rkp1].Tipo() == SITIO )
							it->second.Conectar( rkp1 );
						/*
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
						*/
					}
				}
			}
		}

		// Agora vamos percorrer os objetos anotados como RAMOs_MORTOs e identificar sitios, ligações e ramos mortos
		// Em seguida, aproveita o loop para restaurar a matrizAbertura.
		cout << "-->Identificando sitios, ligações e ramos mortos..." << endl ;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// matrizAbertura armazena a abertura complementar.
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						rotuloijk = matrizRotulada->data3D[i][j][k];
						numConexoes = matrizObjetos[rotuloijk].SConexao().size();
						numSitios = numLigacoes = 0;
						if ( numConexoes > 1 ) {
							matrizObjetos[rotuloijk].Tipo( LIGACAO );
							matrizLigacoes->data3D[i][j][k] = INDICE;
							matrizSitios->data3D[i][j][k] = FUNDO;
							matrizRamosMortos->data3D[i][j][k] = FUNDO;
						} else if ( numConexoes == 0 ) {
							matrizObjetos[rotuloijk].Tipo( SITIO );
							matrizLigacoes->data3D[i][j][k] = FUNDO;
							matrizSitios->data3D[i][j][k] = INDICE;
							matrizRamosMortos->data3D[i][j][k] = FUNDO;
						} else {
							matrizObjetos[rotuloijk].Tipo( RAMO_MORTO );
							matrizLigacoes->data3D[i][j][k] = FUNDO;
							matrizSitios->data3D[i][j][k] = FUNDO;
							matrizRamosMortos->data3D[i][j][k] = INDICE;
						}
					}
					// restaura a matrizAbertura.
					matrizAbertura->data3D[i][j][k] = pm->data3D[i][j][k];
				}
			}
		}

		os.str(""); os << "raio-" << raioEE << "-5-MatrizSitios.dbm";
		SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

		os.str(""); os << "raio-" << raioEE << "-6-MatrizLigacoes.dbm";
		SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

		os.str(""); os << "raio-" << raioEE << "-7-MatrizRamosMortos.dbm";
		SalvarResultadosParciaisEmDisco( matrizRamosMortos, os.str() );

		os.str(""); os << "raio-" << raioEE << "-8-MatrizRotulada.dgm";
		SalvarResultadosParciaisEmDisco( matrizRotulada, os.str() );

		if (salvarResultadosParciais) {
			os.str(""); os << "raio-" << raioEE << "-9-MatrizObjetos.rsl";
			SalvarListaObjetos( os.str() );
		}

		// Atualizando o número de objetos antes da abertura para o próximo passo.
		nObjetosAntesAbertura = nObjetosAberturaComplementar;

		// Incrementando raio do Elemento Estruturante
		raioEE += incrementoRaioElementoEstruturante;
	} // fim do While
	cout << "-->Fim do loop!" << endl;

	os.str(""); os << "raio-" << raioEE << "-10-MatrizRotulada.dgm";
	SalvarResultadosParciaisEmDisco( matrizRotulada, os.str() );

	cout << "-->Dilatando matriz de sitios..." << endl;
	pfmf->Go( matrizSitios );
	pfmf->Dilatacao( matrizSitios, raioEEDilatacao );
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	cout << "-->Dilatando matriz de ligacoes..." << endl;
	pfmf->Go( matrizLigacoes );
	pfmf->Dilatacao( matrizLigacoes, raioEEDilatacao );
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	cout << "-->Corrigindo matrizes..." << endl ;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk) //schedule(dynamic,10)
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				if ( pm->data3D[i][j][k] == INDICE ) {
					rotuloijk = matrizRotulada->data3D[i][j][k];
					if ( matrizSitios->data3D[i][j][k] == INDICE ) {
						matrizLigacoes->data3D[i][j][k] = FUNDO;
						matrizRamosMortos->data3D[i][j][k] = FUNDO;
					} else if ( matrizLigacoes->data3D[i][j][k] == INDICE ) {
						matrizSitios->data3D[i][j][k] = FUNDO;
						matrizRamosMortos->data3D[i][j][k] = FUNDO;
					}
				} else {
					matrizSitios->data3D[i][j][k] = FUNDO;
					matrizLigacoes->data3D[i][j][k] = FUNDO;
					matrizRamosMortos->data3D[i][j][k] = FUNDO;
				}
			}
		}
	}

	//zera a matriz de objetos
	matrizObjetos.clear();

	cout << "-->Rotulando sitios..." << endl ;
	matrizRotulo->Go( matrizSitios );
	numObjetos = matrizRotulo->NumeroObjetos()-1;// menos 1 para não contar fundo
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				rotuloijk = matrizRotulada->data3D[i][j][k] = matrizRotulo->data3D[i][j][k];
				if (rotuloijk != 0) {
					it = matrizObjetos.find(rotuloijk);
					if ( it != matrizObjetos.end() ) {  // o elemento foi encontrado
						++(it->second);							// incrementa o número de objetos representados
					} else {													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
						matrizObjetos[rotuloijk] = CObjetoImagem( SITIO, 1);
					}
				}
			}
		}
	}

	cout << "-->Rotulando ligacoes..." << endl ;
	matrizRotulo->Go( matrizLigacoes );
	numObjetos += matrizRotulo->NumeroObjetos()-1;// menos 1 para não contar fundo
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				rotuloijk = matrizRotulo->data3D[i][j][k];
				if (rotuloijk != 0) {
					rotuloijk += numObjetos;
					matrizRotulada->data3D[i][j][k] = rotuloijk;
					it = matrizObjetos.find(rotuloijk);
					if ( it != matrizObjetos.end() ) {  // o elemento foi encontrado
						++(it->second);							// incrementa o número de objetos representados
					} else {													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
						matrizObjetos[rotuloijk] = CObjetoImagem( LIGACAO, 1);
					}
				}
			}
		}
	}

	cout << "-->Rotulando Ramos Mortos..." << endl ;
	matrizRotulo->Go( matrizRamosMortos );
	numObjetos += matrizRotulo->NumeroObjetos()-1;// menos 1 para não contar fundo
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				rotuloijk = matrizRotulo->data3D[i][j][k];
				if (rotuloijk != 0) {
					rotuloijk += numObjetos;
					matrizRotulada->data3D[i][j][k] = rotuloijk;
					it = matrizObjetos.find(rotuloijk);
					if ( it != matrizObjetos.end() ) {  // o elemento foi encontrado
						++(it->second);							// incrementa o número de objetos representados
					} else {													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
						matrizObjetos[rotuloijk] = CObjetoImagem( RAMO_MORTO, 1);
					}
				}
			}
		}
	}

	//COMPARACOES E CONEXOES
	cout << "-->Comparando matrizes e fazendo conexões..." << endl ;
	ConectarObjetos(matrizRotulada, true);

	//fazer comparacoes convertendo ramos mortos em sitios ou ligacoes
	for ( i = 0; i < nx; ++i ) {
		for ( j = 0; j < ny; ++j ) {
			for ( k = 0; k < nz; ++k ) {
				rotuloijk = matrizRotulada->data3D[i][j][k];
				if (rotuloijk > 0) {
					it = matrizObjetos.find(rotuloijk);
					if ( it->second.Tipo() == RAMO_MORTO ) {
						numSitios = numLigacoes = 0;
						for ( set<int>::iterator con = it->second.SConexao().begin(); con != it->second.SConexao().end(); ++con ) {
							if ( matrizObjetos[*con].Tipo() == SITIO ) {
								++numSitios;
							} else if ( matrizObjetos[*con].Tipo() == LIGACAO ) {
								++numLigacoes;
							}
						}
						if ( numSitios > 1 ) { //ramo morto ligado a mais de um sítio é ligação
							it->second.Tipo(LIGACAO);
						} else if ( numSitios == 1 ) { //ramo morto ligado a um único sítio é sítio
							it->second.Tipo(SITIO);
						} else if ( numLigacoes > 0 ) { //ramo morto ligado a nenhum sítio e a pelo menos uma ligação é ligação
							it->second.Tipo(LIGACAO);
						} else { //ramo morto sem ligações é considerado sítio
							it->second.Tipo(SITIO);
						}
					}
					//aproveita o loop para corrigir matrizes de sítios e ligaçoes
					if ( it->second.Tipo() == SITIO ) {
						matrizSitios->data3D[i][j][k] = INDICE;
						matrizLigacoes->data3D[i][j][k] = FUNDO;
						matrizRamosMortos->data3D[i][j][k] = FUNDO;
					} else {
						matrizSitios->data3D[i][j][k] = FUNDO;
						matrizLigacoes->data3D[i][j][k] = INDICE;
						matrizRamosMortos->data3D[i][j][k] = FUNDO;
					}
				}
			}
		}
	}

	//Segunda etapa: Ligações conectadas a um único sítio precisam ser convertidas para sítios.
	//zera a matriz de objetos
	matrizObjetos.clear();

	cout << "-->Rotulando sitios..." << endl ;
	matrizRotulo->Go( matrizSitios );
	numObjetos = matrizRotulo->NumeroObjetos()-1;// menos 1 para não contar fundo
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				rotuloijk = matrizRotulada->data3D[i][j][k] = matrizRotulo->data3D[i][j][k];
				if (rotuloijk != 0) {
					it = matrizObjetos.find(rotuloijk);
					if ( it != matrizObjetos.end() ) {  // o elemento foi encontrado
						++(it->second);							// incrementa o número de objetos representados
					} else {													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
						matrizObjetos[rotuloijk] = CObjetoImagem( SITIO, 1);
					}
				}
			}
		}
	}

	cout << "-->Rotulando ligacoes..." << endl ;
	matrizRotulo->Go( matrizLigacoes );
	numObjetos += matrizRotulo->NumeroObjetos()-1;// menos 1 para não contar fundo
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				rotuloijk = matrizRotulo->data3D[i][j][k];
				if (rotuloijk != 0) {
					rotuloijk += numObjetos;
					matrizRotulada->data3D[i][j][k] = rotuloijk;
					it = matrizObjetos.find(rotuloijk);
					if ( it != matrizObjetos.end() ) {  // o elemento foi encontrado
						++(it->second);							// incrementa o número de objetos representados
					} else {													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
						matrizObjetos[rotuloijk] = CObjetoImagem( LIGACAO, 1);
					}
				}
			}
		}
	}

	//COMPARACOES E CONEXOES
	cout << "-->Comparando matrizes e fazendo conexões..." << endl ;
	ConectarObjetos(matrizRotulada, true);

	//Ligações conectadas a um único sítio deverão se tornar sítios
	for ( i = 0; i < nx; ++i ) {
		for ( j = 0; j < ny; ++j ) {
			for ( k = 0; k < nz; ++k ) {
				rotuloijk = matrizRotulada->data3D[i][j][k];
				if (rotuloijk > 0) {
					it = matrizObjetos.find(rotuloijk);
					if ( it->second.Tipo() == LIGACAO ) {
						numSitios = 0;
						for ( set<int>::iterator con = it->second.SConexao().begin(); con != it->second.SConexao().end(); ++con ) {
							if ( matrizObjetos[*con].Tipo() == SITIO ) {
								++numSitios;
							}
						}
						if ( numSitios == 1 ) { //ramo morto ligado a um único sítio é sítio
							it->second.Tipo(SITIO);
						}
					}
					//aproveita o loop para corrigir matrizes de sítios e ligaçoes
					if ( it->second.Tipo() == SITIO ) {
						matrizSitios->data3D[i][j][k] = INDICE;
						matrizLigacoes->data3D[i][j][k] = FUNDO;
					} else {
						matrizSitios->data3D[i][j][k] = FUNDO;
						matrizLigacoes->data3D[i][j][k] = INDICE;
					}
				}
			}
		}
	}

	os.str(""); os << "raio-" << raioEE << "-11-MatrizSitiosFinal.dbm";
	SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

	os.str(""); os << "raio-" << raioEE << "-12-MatrizLigacoesFinal.dbm";
	SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

	os.str(""); os << "raio-" << raioEE << "-12-MatrizRamosMortosFinal.dbm";
	SalvarResultadosParciaisEmDisco( matrizRamosMortos, os.str() );

	os.str(""); os << "raio-" << raioEE << "-13-MatrizRotuladaFinal.dgm";
	SalvarResultadosParciaisEmDisco( matrizRotulada, os.str() );

	// Libera memória.
	delete matrizAbertura;
	delete matrizRotulada;
	delete matrizRamosMortos;

	cout << "==>Tempo total de execução: " << (omp_get_wtime()-totaltiming)/60 << " min." << endl;
}

/*========================== Modelo 11 =========================
* Executa abertura a cada passo.
* Considera matriz ramos mortos.
* Após o loop:
* - dilata matrizes de sítios e ligações.
* - corrige matrizes.
* - converte ramos mortos em sítos e ligações
* ==============================================================*/
void CAberturaDilatacao3D::DistSitiosLigacoes_Modelo_11() {
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
	int raioEE = 2;
	int meioNX = nx/2;
	int i, j, k, rotuloijk, rotulov, borda;
	int rim1, rip1, rjm1, rjp1, rkm1, rkp1;
	int numConexoes, numSitios, numLigacoes;//, cont;
	bool continuar;
	//map<int,CObjetoImagem>::iterator itt;

	// Cria matriz abertura, cópia de pm.
	TCMatriz3D<bool>* matrizAbertura = new TCMatriz3D<bool>( *pm );
	matrizAbertura->SetFormato( D1_X_Y_Z_ASCII );

	//Copia pm para a matrizSitios
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				matrizSitios->data3D[i][j][k] = pm->data3D[i][j][k];
			}
		}
	}

	// Cria matriz ramos mortos vazia.
	TCMatriz3D<bool>* matrizRamosMortos = new TCMatriz3D<bool>( nx, ny, nz );
	matrizRamosMortos->Constante( FUNDO );
	matrizRamosMortos->SetFormato( D1_X_Y_Z_ASCII );

	// Rotula matrizRotulo;
	cout << "==>Rotulando Imagem...\t\t\t"; cout.flush(); timing = omp_get_wtime();
	matrizRotulo->Go( matrizAbertura );
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	numObjetos = nObjetosAntesAbertura = matrizRotulo->NumeroObjetos()-1;// menos 1 para não contar fundo

	// Cria matriz que irá armazenar os rótulos identificados nas diversas operações de abertura
	TCMatriz3D<int>* matrizRotulada = new TCMatriz3D<int>( *matrizRotulo );

	cout << "-->Preparando filtro...\t\t\t\t"; cout.flush(); timing = omp_get_wtime();
	pfmf->Go( matrizAbertura );//chama calculo idf 1x
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	cout << "Entrando no looping para indentificar poros e gargantas..." << endl ;
	while ( (numObjetos > 0) and (raioEE <= meioNX) and (raioEE <= raioMaximoElementoEstruturante) ) {
		cout << "==>Executando passo = " << raioEE << endl;
		cout << "-->Num. objetos identificados na imagem = " << numObjetos << endl;
		cout << "-->Num. objetos antes da abertura = " << nObjetosAntesAbertura << endl;

		cout << "-->Processando Abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		pfmf->Abertura( matrizAbertura, raioEE );//para cada raio calcula abertura nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		//após executar a abertura precisa corrigir indices ativos que por ventura não existam na matriz
		//original. Estudar se terá necessidade de permanecer.
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					if ( pm->data3D[i][j][k] ==	FUNDO and matrizAbertura->data3D[i][j][k] == INDICE ) {
						matrizAbertura->data3D[i][j][k] = FUNDO;
					}
				}
			}
		}

		os.str(""); os << "raio-" << raioEE << "-1-MatrizAbertura.dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		cout << "-->Rotulando matriz abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		os.str(""); os << "raio-" << raioEE << "-2-MatrizAberturaRotulada.dgm";
		SalvarResultadosParciaisEmDisco( matrizRotulo, os.str() );

		// Atualiza o número de objetos identificados na imagem após a operação de abertura.
		numObjetos = matrizRotulo->NumeroObjetos()-1; // menos 1 para não contar fundo
		// Acumula o número de objeto antes e depois da abertura
		nObjetosDepoisAbertura = nObjetosAntesAbertura + numObjetos;
		cout << "-->Num. objetos depois da abertura = " << nObjetosDepoisAbertura << endl;

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
						if ( it != matrizObjetos.end() ) {  // o elemento existe
							if ( it->second.NumObjs() > 1 ) { // o elemento representa mais de um objeto
								--(it->second);							// decrementa o número de objetos representados
							} else {												// o elemento representa um ou menos objetos
								matrizObjetos.erase(it);		// apaga o elemento.
							}
						}
						//Agora atualiza a matriz rotulada com os novos rótulos e cria/incremeta os obejtos na matrizObejtos
						rotuloijk = matrizRotulo->data3D[i][j][k] + nObjetosAntesAbertura;
						matrizRotulada->data3D[i][j][k] = rotuloijk;

						it = matrizObjetos.find(rotuloijk);
						if ( it != matrizObjetos.end() ) {  // o elemento foi encontrado
							++(it->second);							// incrementa o número de objetos representados
						} else {													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							matrizObjetos[rotuloijk] = CObjetoImagem( SITIO, 1);
						}
						matrizSitios->data3D[i][j][k] = INDICE;
						matrizLigacoes->data3D[i][j][k] = FUNDO;
						matrizRamosMortos->data3D[i][j][k] = FUNDO;
					}
				}
			}
		}

		//verificar na matrizObjetos quais vizinhos da matrizAbertura são sitios e precisam ter seu
		//rótulo atualizado
		for ( i = 1; i < nx-1; ++i) {
			for ( j = 1; j < ny-1; ++j) {
				for ( k = 1; k < nz-1; ++k) {
					if ( pm->data3D[i][j][k] == INDICE && matrizAbertura->data3D[i][j][k] == FUNDO) {
						rotuloijk = matrizRotulada->data3D[i][j][k];
						it = matrizObjetos.find(rotuloijk);
						if ( it->second.Tipo() == SITIO and it->second.NumObjs() < 7 ) {
							continuar = true;
							//verifica a vizinhança
							if (matrizAbertura->data3D[i-1][j][k] == INDICE) {
								rotulov = matrizRotulada->data3D[i-1][j][k];
								//se os rótulos forem diferentes e o vizinho for SITIO
								if ( rotuloijk != rotulov and matrizObjetos[rotulov].Tipo() == SITIO and continuar ) {
									continuar = false;
								}
							}
							if (continuar && matrizAbertura->data3D[i+1][j][k] == INDICE ) {
								rotulov = matrizRotulada->data3D[i+1][j][k];
								if ( rotuloijk != rotulov and matrizObjetos[rotulov].Tipo() == SITIO ) {
									continuar = false;
								}
								if (continuar && matrizAbertura->data3D[i][j-1][k] == INDICE) {
									rotulov = matrizRotulada->data3D[i][j-1][k];
									if ( rotuloijk != rotulov and matrizObjetos[rotulov].Tipo() == SITIO ) {
										continuar = false;
									}
									if (continuar && matrizAbertura->data3D[i][j+1][k] == INDICE) {
										rotulov = matrizRotulada->data3D[i][j+1][k];
										if ( rotuloijk != rotulov and matrizObjetos[rotulov].Tipo() == SITIO ) {
											continuar = false;
										}
										if (continuar && matrizAbertura->data3D[i][j][k-1] == INDICE) {
											rotulov = matrizRotulada->data3D[i][j][k-1];
											if ( rotuloijk != rotulov and matrizObjetos[rotulov].Tipo() == SITIO ) {
												continuar = false;
											}
											if (continuar && matrizAbertura->data3D[i][j][k+1] == INDICE) {
												rotulov = matrizRotulada->data3D[i][j][k+1];
												if ( rotuloijk != rotulov and matrizObjetos[rotulov].Tipo() == SITIO ) {
													continuar = false;
												}
											}
										}
									}
								}
							}
							if ( ! continuar ) { //Atendeu as condições. Altera as Matrizes.
								if ( it->second.NumObjs() > 1 ) {
									--(it->second);
								} else {
									matrizObjetos.erase(it);
								}
								++(matrizObjetos[rotulov]);
								matrizRotulada->data3D[i][j][k] = rotulov;
								matrizAbertura->data3D[i][j][k] = INDICE;
								matrizSitios->data3D[i][j][k] = INDICE;
								matrizLigacoes->data3D[i][j][k] = FUNDO;
								matrizRamosMortos->data3D[i][j][k] = FUNDO;
							}
						}
					}
				}
			}
		}
		os.str(""); os << "raio-" << raioEE << "-1-MatrizAberturaCorrigida.dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// Identificando complemento da abertura:
					// Se o pixel analizado for INDICE em pm, inverte os valores da matrizAbertura assinalando como matriz complementar
					// Desconsidera como complemento os pixeis assinalados com ligação ou ramo morto
					if ( pm->data3D[i][j][k] ==	INDICE ) {
						if ( matrizAbertura->data3D[i][j][k] == FUNDO &&
								 matrizLigacoes->data3D[i][j][k] == FUNDO &&
								 matrizRamosMortos->data3D[i][j][k] == FUNDO )
						{
							matrizAbertura->data3D[i][j][k] = INDICE;
						} else {
							matrizAbertura->data3D[i][j][k] = FUNDO;
						}
					} else {
						matrizAbertura->data3D[i][j][k] = FUNDO;
					}
				}
			}
		}

		os.str(""); os << "raio-" << raioEE << "-3-MatrizAberturaComplementar.dbm";
		SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		cout << "-->Rotulando matriz abertura complementar...\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( matrizAbertura );//nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		os.str(""); os << "raio-" << raioEE << "-4-MatrizAberturaComplementarRotulada.dgm";
		SalvarResultadosParciaisEmDisco( matrizRotulo, os.str() );

		// Acumula o número de objetos depois da abertura como o número de objetos do complemento da abertura
		nObjetosAberturaComplementar = nObjetosDepoisAbertura + matrizRotulo->NumeroObjetos() - 1;
		cout << "-->Num. objetos na abertura complementar = " << nObjetosAberturaComplementar << endl;

		// Copia para a matrizRotulada os rótulos do complemento da matriz abertura, acrescidos do nObjetosDepoisAbertura, de forma que os rótulos sejam sequenciais
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,it) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i ) {
			for ( j = 0; j < ny; ++j ) {
				for ( k = 0; k < nz; ++k ) {
					// Se o pixel analizado é INDICE na matriz Abertura, copia o rótulo acrescido do número de objetos antes da abertura para a matrizRotulada
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						//Primeiro decrementa o número de objetos representados na matrizObjetos e verifica se o objeto continuará existindo.
						//Os objetos existentes na matrizAbertura terão novos rótulos, então precisamos decrementar o número de objetos ou apagar os elementos que deixarão de representar objetos.
						rotuloijk = matrizRotulada->data3D[i][j][k];
						it = matrizObjetos.find(rotuloijk);
						if ( it != matrizObjetos.end() ) {  // o elemento existe
							if ( it->second.NumObjs() > 1 ) { // o elemento representa mais de um objeto
								--(it->second);							// decrementa o número de objetos representados
							}else{												// o elemento represente um ou menos objetos
								matrizObjetos.erase(it);		// apaga o elemento.
							}
						}
						//Agora atualiza a matriz rotulada com os novos rótulos e cria/incremeta os obejtos na matrizObejtos
						//Os objetos identificados no complemento da abertura, inicialmente são considerados ramos mortos; depois iremos verificar quais ramos representam ligações.
						rotuloijk = matrizRotulo->data3D[i][j][k] + nObjetosDepoisAbertura;
						matrizRotulada->data3D[i][j][k] = rotuloijk;
						it = matrizObjetos.find(rotuloijk);
						if ( it != matrizObjetos.end() ) {  // o elemento foi encontrado
							++(it->second);								// incrementa o número de objetos representados
						} else {													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							matrizObjetos[rotuloijk] = CObjetoImagem( RAMO_MORTO, 1);
						}
						matrizSitios->data3D[i][j][k] = FUNDO;
						matrizLigacoes->data3D[i][j][k] = FUNDO;
						matrizRamosMortos->data3D[i][j][k] = INDICE;
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
					// Só devemos considerar os rotulos do complemento da abertura
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						rotuloijk = matrizRotulada->data3D[i][j][k];
						it = matrizObjetos.find(rotuloijk);
						rim1 = matrizRotulada->data3D[i-1][j][k];
						rip1 = matrizRotulada->data3D[i+1][j][k];
						rjm1 = matrizRotulada->data3D[i][j-1][k];
						rjp1 = matrizRotulada->data3D[i][j+1][k];
						rkm1 = matrizRotulada->data3D[i][j][k-1];
						rkp1 = matrizRotulada->data3D[i][j][k+1];
						// Se os rotulos são diferentes, fazem parte da matriz abertura e o vizinho é um sítio, então, marca a conexão.
						if ( rotuloijk != rim1 and matrizObjetos[rim1].Tipo() == SITIO )
							it->second.Conectar( rim1 );
						if ( rotuloijk != rip1 and matrizObjetos[rip1].Tipo() == SITIO )
							it->second.Conectar( rip1 );
						if ( rotuloijk != rjm1 and matrizObjetos[rjm1].Tipo() == SITIO )
							it->second.Conectar( rjm1 );
						if ( rotuloijk != rjp1 and matrizObjetos[rjp1].Tipo() == SITIO )
							it->second.Conectar( rjp1 );
						if ( rotuloijk != rkm1 and matrizObjetos[rkm1].Tipo() == SITIO )
							it->second.Conectar( rkm1 );
						if ( rotuloijk != rkp1 and matrizObjetos[rkp1].Tipo() == SITIO )
							it->second.Conectar( rkp1 );
					}
				}
			}
		}

		// Agora vamos percorrer os objetos anotados como RAMOs_MORTOs e identificar sitios, ligações e ramos mortos
		// Em seguida, aproveita o loop para restaurar a matrizAbertura.
		cout << "-->Identificando sitios, ligações e ramos mortos..." << endl ;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// matrizAbertura armazena a abertura complementar.
					if ( matrizAbertura->data3D[i][j][k] == INDICE ) {
						rotuloijk = matrizRotulada->data3D[i][j][k];
						numConexoes = matrizObjetos[rotuloijk].SConexao().size();
						if ( numConexoes > 1 ) {
							matrizObjetos[rotuloijk].Tipo( LIGACAO );
							matrizLigacoes->data3D[i][j][k] = INDICE;
							matrizSitios->data3D[i][j][k] = FUNDO;
							matrizRamosMortos->data3D[i][j][k] = FUNDO;
						} else if ( numConexoes == 0 ) {
							matrizObjetos[rotuloijk].Tipo( SITIO );
							matrizLigacoes->data3D[i][j][k] = FUNDO;
							matrizSitios->data3D[i][j][k] = INDICE;
							matrizRamosMortos->data3D[i][j][k] = FUNDO;
						} else {
							matrizObjetos[rotuloijk].Tipo( RAMO_MORTO );
							matrizLigacoes->data3D[i][j][k] = FUNDO;
							matrizSitios->data3D[i][j][k] = FUNDO;
							matrizRamosMortos->data3D[i][j][k] = INDICE;
						}
					}
					// restaura a matrizAbertura.
					matrizAbertura->data3D[i][j][k] = pm->data3D[i][j][k];
				}
			}
		}

		os.str(""); os << "raio-" << raioEE << "-5-MatrizSitios.dbm";
		SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

		os.str(""); os << "raio-" << raioEE << "-6-MatrizLigacoes.dbm";
		SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

		os.str(""); os << "raio-" << raioEE << "-7-MatrizRamosMortos.dbm";
		SalvarResultadosParciaisEmDisco( matrizRamosMortos, os.str() );

		os.str(""); os << "raio-" << raioEE << "-8-MatrizRotulada.dgm";
		SalvarResultadosParciaisEmDisco( matrizRotulada, os.str() );

		if (salvarResultadosParciais) {
			os.str(""); os << "raio-" << raioEE << "-9-MatrizObjetos.rsl";
			SalvarListaObjetos( os.str() );
		}

		// Atualizando o número de objetos antes da abertura para o próximo passo.
		nObjetosAntesAbertura = nObjetosAberturaComplementar;

		// Incrementando raio do Elemento Estruturante
		raioEE += incrementoRaioElementoEstruturante;
	} // fim do While
	cout << "-->Fim do loop!" << endl;

	os.str(""); os << "raio-" << raioEE << "-10-MatrizRotulada.dgm";
	SalvarResultadosParciaisEmDisco( matrizRotulada, os.str() );

	cout << "-->Dilatando matriz de sitios..." << endl;
	pfmf->Go( matrizSitios );
	pfmf->Dilatacao( matrizSitios, raioEEDilatacao );

	cout << "-->Dilatando matriz de ligacoes..." << endl;
	pfmf->Go( matrizLigacoes );
	pfmf->Dilatacao( matrizLigacoes, 1 );

//	cout << "-->Dilatando matriz ramos mortos..." << endl;
//	pfmf->Go( matrizRamosMortos );
//	pfmf->Dilatacao( matrizRamosMortos, raioEEDilatacao );

	cout << "-->Corrigindo matrizes..." << endl ;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk) //schedule(dynamic,10)
	for ( i = 0; i < nx; ++i) {
		for ( j = 0; j < ny; ++j) {
			for ( k = 0; k < nz; ++k) {
				if ( pm->data3D[i][j][k] == INDICE ) {
					if ( matrizSitios->data3D[i][j][k] == INDICE ) {
						matrizLigacoes->data3D[i][j][k] = FUNDO;
						matrizRamosMortos->data3D[i][j][k] = FUNDO;
					} else if ( matrizLigacoes->data3D[i][j][k] == INDICE ) {
						matrizSitios->data3D[i][j][k] = FUNDO;
						matrizRamosMortos->data3D[i][j][k] = FUNDO;
					} else if ( matrizRamosMortos->data3D[i][j][k] == INDICE ) {
						matrizSitios->data3D[i][j][k] = INDICE;
						matrizLigacoes->data3D[i][j][k] = FUNDO;
					}
				} else {
					matrizSitios->data3D[i][j][k] = FUNDO;
					matrizLigacoes->data3D[i][j][k] = FUNDO;
					matrizRamosMortos->data3D[i][j][k] = FUNDO;
				}
			}
		}
	}

	//zera a matriz de objetos
	matrizObjetos.clear();
	matrizRotulada->Constante(0);

	cout << "-->Rotulando sitios e criando objetos..." << endl ;
	numObjetos = RotularECriarObjetos(matrizSitios, matrizRotulada, SITIO, 0);

	cout << "-->Rotulando ligacoes e criando objetos..." << endl ;
	numObjetos += RotularECriarObjetos(matrizLigacoes, matrizRotulada, LIGACAO, numObjetos);

	cout << "-->Rotulando Ramos Mortos e criando objetos..." << endl ;
	numObjetos += RotularECriarObjetos(matrizRamosMortos, matrizRotulada, RAMO_MORTO, numObjetos);

	//COMPARACOES E CONEXOES
	cout << "-->Comparando matrizes e fazendo conexões..." << endl ;
	ConectarObjetos(matrizRotulada, true);

	//fazer comparacoes convertendo ramos mortos em sitios ou ligacoes
	for ( i = 0; i < nx; ++i ) {
		for ( j = 0; j < ny; ++j ) {
			for ( k = 0; k < nz; ++k ) {
				rotuloijk = matrizRotulada->data3D[i][j][k];
				if (rotuloijk > 0) {
					it = matrizObjetos.find(rotuloijk);
					if ( it->second.Tipo() == RAMO_MORTO ) {
						numSitios = numLigacoes = 0;
						for ( set<int>::iterator con = it->second.SConexao().begin(); con != it->second.SConexao().end(); ++con ) {
							if ( matrizObjetos[*con].Tipo() == SITIO ) {
								++numSitios;
							} else if ( matrizObjetos[*con].Tipo() == LIGACAO ) {
								++numLigacoes;
							}
						}
						if ( numSitios > 1 ) { //ramo morto ligado a mais de um sítio é ligação
							it->second.Tipo(LIGACAO);
						} else if ( numSitios == 1 ) { //ramo morto ligado a um único sítio é sítio
							it->second.Tipo(SITIO);
						} else if ( numLigacoes > 0 ) { //ramo morto ligado a nenhum sítio e a pelo menos uma ligação é ligação
							it->second.Tipo(LIGACAO);
						} else { //ramo morto sem ligações é considerado sítio
							it->second.Tipo(SITIO);
						}
						//aproveita o loop para corrigir matrizes de sítios e ligaçoes
						if ( it->second.Tipo() == SITIO ) {
							matrizSitios->data3D[i][j][k] = INDICE;
							matrizLigacoes->data3D[i][j][k] = FUNDO;
							matrizRamosMortos->data3D[i][j][k] = FUNDO;
						} else if ( it->second.Tipo() == LIGACAO ) {
							matrizSitios->data3D[i][j][k] = FUNDO;
							matrizLigacoes->data3D[i][j][k] = INDICE;
							matrizRamosMortos->data3D[i][j][k] = FUNDO;
						}
					}
				}
			}
		}
	}

	//Segunda etapa: Ligações conectadas a um único sítio precisam ser convertidas para sítios.
	//zera a matriz de objetos
	matrizObjetos.clear();
	matrizRotulada->Constante(0);

	cout << "-->Rotulando sitios e criando objetos..." << endl ;
	numObjetos = RotularECriarObjetos(matrizSitios, matrizRotulada, SITIO, 0);

	cout << "-->Rotulando ligacoes e criando objetos..." << endl ;
	numObjetos += RotularECriarObjetos(matrizLigacoes, matrizRotulada, LIGACAO, numObjetos);

	//COMPARACOES E CONEXOES
	cout << "-->Comparando matrizes e fazendo conexões..." << endl ;
	ConectarObjetos(matrizRotulada, true);

	//Ligações conectadas a um único sítio deverão se tornar sítios
	for ( i = 0; i < nx; ++i ) {
		for ( j = 0; j < ny; ++j ) {
			for ( k = 0; k < nz; ++k ) {
				rotuloijk = matrizRotulada->data3D[i][j][k];
				if (rotuloijk > 0) {
					it = matrizObjetos.find(rotuloijk);
					if ( it->second.Tipo() == LIGACAO ) {
						numSitios = 0;
						for ( set<int>::iterator con = it->second.SConexao().begin(); con != it->second.SConexao().end(); ++con ) {
							if ( matrizObjetos[*con].Tipo() == SITIO ) {
								++numSitios;
							}
						}
						if ( numSitios == 1 ) { //ligação conectada a um único sítio é sítio
							it->second.Tipo(SITIO);
						}
					}
					//aproveita o loop para corrigir matrizes de sítios e ligaçoes
					if ( it->second.Tipo() == SITIO ) {
						matrizSitios->data3D[i][j][k] = INDICE;
						matrizLigacoes->data3D[i][j][k] = FUNDO;
					} else {
						matrizSitios->data3D[i][j][k] = FUNDO;
						matrizLigacoes->data3D[i][j][k] = INDICE;
					}
				}
			}
		}
	}

	os.str(""); os << "raio-" << raioEE << "-11-MatrizSitiosFinal.dbm";
	SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

	os.str(""); os << "raio-" << raioEE << "-12-MatrizLigacoesFinal.dbm";
	SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );

	os.str(""); os << "raio-" << raioEE << "-12-MatrizRamosMortosFinal.dbm";
	SalvarResultadosParciaisEmDisco( matrizRamosMortos, os.str() );

	os.str(""); os << "raio-" << raioEE << "-13-MatrizRotuladaFinal.dgm";
	SalvarResultadosParciaisEmDisco( matrizRotulada, os.str() );

	// Libera memória.
	delete matrizAbertura;
	delete matrizRotulada;
	delete matrizRamosMortos;

	cout << "==>Tempo total de execução: " << (omp_get_wtime()-totaltiming)/60 << " min." << endl;
}

