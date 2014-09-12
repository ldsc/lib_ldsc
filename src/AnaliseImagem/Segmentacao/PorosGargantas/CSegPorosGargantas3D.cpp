#include <AnaliseImagem/Segmentacao/PorosGargantas/CSegPorosGargantas3D.h>
#include <iomanip>
#include <cmath>
#include <omp.h>

using namespace std;

CSegPorosGargantas3D::CSegPorosGargantas3D(TCMatriz3D<bool>* &matriz , int _indice, int _fundo)
	: CMatrizObjetoImagem(), pm(matriz), pi3d(nullptr), // pm é ponteiro para imagem externa (se mudar externamente teremos problemas).
		fatorReducaoRaioElemEst (1), raioMaximoElementoEstruturante ( 500 ), // usar limits
		incrementoRaioElementoEstruturante ( 1 ), raioEEDilatacao( 1 ), modelo(0), INDICE(_indice), FUNDO(_fundo),
		salvarResultadosParciais(false), gerarDetalhesObjetos(false)
{
	matrizSitios = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );
	matrizSitios->SetFormato( D1_X_Y_Z_ASCII );
	matrizLigacoes = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );
	matrizLigacoes->SetFormato( D1_X_Y_Z_ASCII );
	matrizRotulo = nullptr;
	pmira = nullptr;
}

CSegPorosGargantas3D::CSegPorosGargantas3D(TCImagem3D<bool>* &matriz , int _indice, int _fundo)
	: CMatrizObjetoImagem(), pi3d(matriz), fatorReducaoRaioElemEst (1), raioMaximoElementoEstruturante ( 500 ), // usar limits
		incrementoRaioElementoEstruturante ( 1 ), raioEEDilatacao(1), modelo(0), INDICE(_indice), FUNDO(_fundo),
		salvarResultadosParciais(false), gerarDetalhesObjetos(false)
{
	pm = dynamic_cast<TCMatriz3D<bool> *>(matriz); // pm é ponteiro para imagem externa (se mudar externamente teremos problemas).
	matrizSitios = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );
	matrizSitios->SetFormato( D1_X_Y_Z_ASCII );
	matrizLigacoes = new TCMatriz3D<bool>( pm->NX(), pm->NY(), pm->NZ() );
	matrizLigacoes->SetFormato( D1_X_Y_Z_ASCII );
	matrizRotulo = nullptr;
	pmira = nullptr;
}

CSegPorosGargantas3D::~CSegPorosGargantas3D() {
	delete matrizSitios;
	delete matrizLigacoes;
	if(matrizRotulo) delete matrizRotulo;
	if(pmira) delete pmira;
}

double CSegPorosGargantas3D::Porosidade( TCMatriz3D<int>* &_pm, int _ra ) {
	double porosidade = 0.0;
	//executa os 3 for paralelamente, definindo todas as variáveis como compartilhadas, (exceto i, j e k). A variável porosidade terá os valores somados.
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private (i,j,k) reduction(+:porosidade) schedule(static,10)
	for ( i = 0; i < _pm->NX(); ++i )
		for ( j = 0; j < _pm->NY(); ++j )
			for ( k = 0; k < _pm->NZ(); ++k )
				if ( _pm->data3D[i][j][k] > _ra ) {
					porosidade++;
				}
	return 100.0 * porosidade / ( (double) _pm->NX() * _pm->NY() * _pm->NZ() );
}

//	Recebe nome do arquivo a ser salvo e ponteiro para duas matrizes do tipo bool que serão mescladas.
//	Salva a mesclagem das matrizes em disco de forma que 0 será o fundo, 1 serão os índices da primeira matriz e 2 serão os índices da segunda matriz.
//	Se a possição dos índices coincidirem, o indice da última matriz informada como parâmetro será considerado.
//	Método criado para que se possa salvar em disco duas matrizes bool ao invés de utilizar uma matriz int para representar sólidos, sitios e ligações (consome menos memória).
bool CSegPorosGargantas3D::Write(string nomeArquivo ) {
	/*
	ofstream fout; //  Abre arquivo disco
	fout.open(nomeArquivo.c_str(), ios::binary);
	int nx, ny, nz;
	if (fout.good()){
		//menores valores para nx, ny e nz.
		nx = pm->NX();
		ny = pm->NY();
		nz = pm->NZ();

		//cabeçalho
		fout << setw (0) << "D5" << '\n' << nx << ' ' << ny << ' ' << nz << '\n' << 3 << '\n';
		if (pi3d != nullptr) {
			fout << "#fatorAmplificacao: " << pi3d->FatorAmplificacao() << '\n';
			fout << "#dimensaoPixel: " << pi3d->DimensaoPixel() << '\n';
			fout << "#numeroPixeisBorda: " << pi3d->NumeroPixelsBorda() << '\n';
		}
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
	*/
	//Assim é mais lento e consome mais memória, porém, qualquer alteração na estrutura da imagem é refletida aqui.
	int nx = pm->NX();
	int ny = pm->NY();
	int nz = pm->NZ();
	TCImagem3D<short int> i3d(nx, ny, nz);
	i3d.SetFormato(EImageType::D5_X_Y_Z_GRAY_BINARY);
	i3d.NumCores(3);
	if (pi3d != nullptr) {
		i3d.FatorAmplificacao(pi3d->FatorAmplificacao());
		i3d.DimensaoPixel(pi3d->DimensaoPixel());
		i3d.NumeroPixelsBorda(pi3d->NumeroPixelsBorda());
	}
	//percorre matrizes e mescla resultados salvando na imagem 3D.
	for (int k = 0; k < nz; ++k) {
		for (int j = 0; j < ny; ++j) {
			for (int i = 0; i < nx; ++i) {
				if( matrizSitios->data3D[i][j][k] == INDICE ) // se o voxel for índice em matrizSitios
					i3d.data3D[i][j][k] = 1; //SÍTIO
				else if( matrizLigacoes->data3D[i][j][k] == INDICE ) // senão, se o voxel for índice em matrizLigacoes
					i3d.data3D[i][j][k] = 2; //LIGACAO
				else // senão, só pode ser fundo
					i3d.data3D[i][j][k] = 0; //FUNDO
			}
		}
	}
	return i3d.Write(nomeArquivo);
}

// Grava em disco, com o nome informado, os objetos identificados.
bool CSegPorosGargantas3D::SalvarListaObjetos(string nomeArquivo){
	if (gerarDetalhesObjetos) {
		return CMatrizObjetoImagem::SalvarListaObjetos(nomeArquivo, pm->NX(), pm->NY(), pm->NZ());
	}
	return false;
}

// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz bool informada como parametro.
void CSegPorosGargantas3D::SalvarResultadosParciaisEmDisco(TCMatriz3D<bool>* &mat, string nomeArquivo){
	if (salvarResultadosParciais) {
		cout << "-->Salvando imagem " << nomeArquivo.c_str() << endl ;
		mat->Write(nomeArquivo);
	}
}

// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz int informada como parametro.
void CSegPorosGargantas3D::SalvarResultadosParciaisEmDisco(TCMatriz3D<int>* &mat, string nomeArquivo){
	if (salvarResultadosParciais) {
		cout << "-->Salvando imagem " << nomeArquivo.c_str() << endl ;
		mat->Write(nomeArquivo);
	}
}

// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz rotulo informada como parametro.
void CSegPorosGargantas3D::SalvarResultadosParciaisEmDisco(CRotuladorIRA3D *&mat, string nomeArquivo){
	if (salvarResultadosParciais) {
		cout << "-->Salvando imagem " << nomeArquivo.c_str() << endl ;
		mat->Write(nomeArquivo);
	}
}

// Executa a segmentação de poros e gargantas de acordo com o modelo informado.
void CSegPorosGargantas3D::Go(int _modelo){
	modelo = _modelo;
	matrizObjetos.clear();
	matrizSitios->Constante(FUNDO);
	matrizLigacoes->Constante(FUNDO);
	switch ( modelo ) {
		case 0:
			Modelo_0();
			break;
		default:
			return Modelo_0();
	}
	matrizObjetos.clear();
	GerarDetalhesMatrizObjetos();
}

void CSegPorosGargantas3D::GerarDetalhesMatrizObjetos() {
	// Se a flag estiver setada
	if (gerarDetalhesObjetos) {
		// Variáveis auxiliares
		map<int,CObjetoImagem>::iterator itt;
		int rip1, rjp1, rkp1;
		int rotulo = 0;
		int i,j,k;

		cout << "==>Rotulando a matrizSitios..." << endl ;
		TCRotulador3D<bool> * mRotulo = new TCRotulador3D<bool>(matrizSitios);
		mRotulo->Go(matrizSitios);

		cout << "==>Determinando IDF da matrizSitios..." << endl ;
		TCFEMMIDFd3453D<bool> pfmf(matrizSitios, INDICE, FUNDO);
		pfmf.Go(matrizSitios);

		// Armazena o número de objetos identificados na rotulagem
		int numObjs = mRotulo->NumeroObjetos();
		int nx = matrizSitios->NX();
		int ny = matrizSitios->NY();
		int nz = matrizSitios->NZ();
		// Cria matriz que irá acumular os rótulos identificados (inicialmente é cópia de matrizRotulo)
		TCMatriz3D<int>* matrizRotulada = new TCMatriz3D<int>( nx, ny, nz );

		cout << "==>Alimentando a matrizObjetos com os objetos identificados na rotulagem dos sítios..." << endl ;
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					rotulo = mRotulo->data3D[i][j][k];
					if ( rotulo != 0) {
						it = matrizObjetos.find(rotulo);
						if(it != matrizObjetos.end()){  // o elemento foi encontrado
							++(it->second);							// incrementa o número de objetos representados
						}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							matrizObjetos[rotulo] = CObjetoImagem( SITIO, 1 );
						}
						it->second.PontoCentral( i, j, k, pfmf.data3D[i][j][k] );
					}
					// matrizRotulada acumula os rótulos identificados (inicialmente é cópia de matrizRotulo).
					matrizRotulada->data3D[i][j][k] = rotulo;
				}
			}
		}

		cout << "==>Rotulando a matrizLigacoes..." << endl ;
		mRotulo->Go(matrizLigacoes);

		cout << "==>Determinando IDF da matrizLigacoes..." << endl ;
		pfmf.Go(matrizLigacoes);

		int numObjs_1 = numObjs-1;
		cout << "==>Alimentando a matrizObjetos com os objetos identificados na rotulagem das ligações..." << endl ;
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					rotulo = mRotulo->data3D[i][j][k];
					if ( rotulo > 0 ) { //só entra se for diferente de fundo, ou seja, neste caso, maior que numObjs
						rotulo = rotulo + numObjs_1;
						mRotulo->data3D[i][j][k] = rotulo; // para que os rótulos sejam sequenciais
						it = matrizObjetos.find(rotulo);
						if(it != matrizObjetos.end()){  // o elemento foi encontrado
							++(it->second);							// incrementa o número de objetos representados
						}else{													// o elemento ainda não existe, então iremos crialo representando 1 objeto.
							matrizObjetos[rotulo] = CObjetoImagem( LIGACAO, 1);
						}
						it->second.PontoCentral( i, j, k, pfmf.data3D[i][j][k] );
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
					rotulo = mRotulo->data3D[i][j][k];
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
		delete mRotulo;
	}
}


/* ========================= Modelo 0 =========================
 * Utiliza IRA
 * Passos do algorítmo: Descrever...
*/
void CSegPorosGargantas3D::Modelo_0() {
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

	cout << "==>Criando IRA...\t\t\t"; cout.flush(); timing = omp_get_wtime();
	TCFEMMIRA3D<bool> *pfira = new TCFEMMIRA3D<bool>( pm, INDICE, FUNDO );
	pmira = pfira->Go();
	delete pfira;
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	// Rotula matrizRotulo;
	cout << "==>Rotulando Imagem...\t\t\t"; cout.flush(); timing = omp_get_wtime();
	matrizRotulo = new CRotuladorIRA3D( pmira, INDICE, FUNDO );
	matrizRotulo->Go( pmira, 0 );
	cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

	numObjetos = nObjetosAntesAbertura = matrizRotulo->NumeroObjetos();

	// Cria matriz que irá armazenar os rótulos identificados nas diversas operações de abertura
	TCMatriz3D<int>* matrizRotulada = new TCMatriz3D<int>( *matrizRotulo );

	cout << "Entrando no looping para indentificar poros e gargantas..." << endl ;
	while ( (numObjetos > 1) and (raioEE <= meioNX) and (raioEE <= raioMaximoElementoEstruturante) ) {
		cout << "==>Executando passo = " << raioEE << endl;
		cout << "-->Num. objetos antes da abertura = " << matrizRotulo->NumeroObjetos() << endl;

		cout << "-->Rotulando matriz abertura...\t\t\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( pmira, raioEE );//rotula nX
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		// Atualiza o número de objetos identificados na imagem após a operação de abertura.
		numObjetos = matrizRotulo->NumeroObjetos();
		// Acumula o número de objeto antes e depois da abertura
		nObjetosDepoisAbertura = nObjetosAntesAbertura + numObjetos - 1; // menos 1 para não contar fundo novamente
		cout << "-->Num. objetos depois da abertura = " << numObjetos << endl;

		// Copia a matriz abertura rotulada (matrizRotulo) para a matrizRotulada,
		// assim, a matrizRotulada terá também a informação dos rótulos dos sítios identificados.
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// Se o pixel analizado é INDICE na matrizAbertura, copia o rótulo acrescidos do número de objetos antes da abertura para a matrizRotulada
					if ( pmira->data3D[i][j][k] > raioEE ) {
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
				}
			}
		}
		// O complemento da abertura corresponde a: (0 < pmira[i][j][k] <= raioEE)
		cout << "-->Rotulando matriz abertura complementar...\t"; cout.flush(); timing = omp_get_wtime();
		matrizRotulo->Go( pmira, -raioEE, raioEE-1, matrizLigacoes ); //quando o raio é negativo rotula o complemento da abertura
		cout << "tempo: " << omp_get_wtime()-timing << " s." << endl;

		//os.str(""); os << "MatrizComplementoAbertura_" << raioEE << ".dbm";
		//SalvarResultadosParciaisEmDisco( matrizAbertura, os.str() );

		// Acumula o número de objetos depois da abertura como o número de objetos do complemento da abertura
		nObjetosAberturaComplementar = nObjetosDepoisAbertura + matrizRotulo->NumeroObjetos() - 1;
		cout << "-->Num. objetos na abertura complementar = " << matrizRotulo->NumeroObjetos() << endl;

		// Copia para a matrizRotulada os rótulos do complemento da matriz abertura, acrescidos do nObjetosDepoisAbertura, de forma que os rótulos sejam sequenciais
		//#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk,it) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// Se o pixel analizado é INDICE na matriz Abertura, copia o rótulo acrescido do número de objetos antes da abertura para a matrizRotulada
					//if ( pmira->data3D[i][j][k] > 0 and pmira->data3D[i][j][k] <= raioEE and matrizLigacoes->data3D[i][j][k] == FUNDO) {
					if ( matrizRotulo->data3D[i][j][k] > 0 ) {
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
		for ( i = borda; i < (nx-borda); i++) {
			for ( j = borda; j < (ny-borda); j++) {
				for ( k = borda; k < (nz-borda); k++) {
					rotuloijk = matrizRotulada->data3D[i][j][k];
					// Só devemos considerar os rotulos criados neste passo, i.e, rotulo >= nObjetosAntesAbertura
					//if ( rotuloijk	>= nObjetosAntesAbertura ) {
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

		// Agora vamos percorrer os objetos anotados como RAMOs_MORTOs e identificar as ligações
		// Uma vez identificadas as ligações, podemos armazenar o resultado na matrizLigacoes.
		cout << "-->Identificando sitios e ligações..." << endl ;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k,rotuloijk) //schedule(dynamic,10)
		for ( i = 0; i < nx; ++i) {
			for ( j = 0; j < ny; ++j) {
				for ( k = 0; k < nz; ++k) {
					// a abertura complementar tem RAMOs_MORTOs e LIGACOES
					//if ( pmira->data3D[i][j][k] > 0 and pmira->data3D[i][j][k] <= raioEE  and matrizLigacoes->data3D[i][j][k] == FUNDO) {
					if ( matrizRotulo->data3D[i][j][k] > 0 ) {
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
				}
			}
		}

		//		os.str(""); os << "MatrizSitios_" << raioEE << ".dbm";
		//		SalvarResultadosParciaisEmDisco( matrizSitios, os.str() );

		//		os.str(""); os << "MatrizLigacoes_" << raioEE << ".dbm";
		//		SalvarResultadosParciaisEmDisco( matrizLigacoes, os.str() );


		// Atualizando o número de objetos antes da abertura para o próximo passo.
		nObjetosAntesAbertura = nObjetosAberturaComplementar;

		// Incrementando raio do Elemento Estruturante
		raioEE += incrementoRaioElementoEstruturante;
	} // fim do While

	// Libera memória.
	delete matrizRotulada;

	/*
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
	*/
	cout << "==>Tempo total de execução: " << (omp_get_wtime()-totaltiming)/60 << " min." << endl;
}
