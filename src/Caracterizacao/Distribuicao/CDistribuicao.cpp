#include <Caracterizacao/Distribuicao/CDistribuicao.h>
#ifndef TCFEMorfologiaMatematica_h
#include <Filtro/FEspacial/FEMorfologiaMatematica/TCFEMorfologiaMatematica.h>
#endif
#ifndef TCFEMMIDFd34_h
#include <Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd34.h>
#endif
#ifndef TCFEMMIDFd5711_h
#include <Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd5711.h>
#endif
#ifndef TCFEMMIDFEuclidiana_h
#include <Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFEuclidiana.h>
#endif

bool CDistribuicao::Go( TCMatriz2D<bool> * _pm, Tipos _tipo, Metrica _metrica, int indice, int fundo ) {
	pm = _pm;
	Go ( _tipo, _metrica, indice, fundo );
}

bool CDistribuicao::Go( Tipos _tipo, Metrica _metrica, int indice, int fundo ) {
	if ( ! pm ) { // se o ponteiro para a matriz não estiver definido rotorna false
		cerr << "Erro em CDistribuicao::Go! Ponteiro para TCMatriz2D< int > não definido!" << endl;
		return false;
	}
	tipo = _tipo; //seta o tipo pois será utilizado em Write;
	int raio = 1;
	int cont = 0;
	int nx = pm->NX();
	int ny = pm->NY();
	int area = nx * ny;
	double obj = 0.0;
	double objt = 0.0;
	double lastobj = 0.0;
	double acumulada = 0.0;
	distribuicao.clear();

	TCMatriz2D<bool> * matriz = NULL;
	matriz = new TCMatriz2D<bool>(nx, ny); // faz uma cópia da matriz
	if ( ! matriz ) { // se a matriz não foi criada corretamente rotorna false
		cerr << "Erro em CDistribuicao::Go! Cópia de TCMatriz2D< int > não foi criada!" << endl;
		return false;
	}

	TCFEMorfologiaMatematica<bool> * filtro = NULL;
	switch (_metrica){
		case espacial:
			filtro = new TCFEMorfologiaMatematica<bool>(matriz, raio, indice, fundo);
			break;
		case d34:
			filtro = new TCFEMMIDFd34<bool>(matriz, indice, fundo);
			break;
		case d5711:
			filtro = new TCFEMMIDFd5711<bool>(matriz, indice, fundo);
			break;
		case euclidiana:
			filtro = new TCFEMMIDFEuclidiana<bool>(matriz, indice, fundo);
			break;
	}

	if ( ! filtro ) { // se o filtro não foi criado corretamente rotorna false
		if ( matriz ) {
			delete matriz;
		}
		cerr << "Erro em CDistribuicao::Go! O filtro não foi criado!" << endl;
		return false;
	}

	//calcula a área dos objetos
	for ( int i=0; i < nx; i++ ) {
		for ( int j=0; j < ny; j++) {
			if ( pm->data2D[i][j] == indice) {
				cont++;
			}
		}
	}
	areaObjetos = obj = 100.0*cont/area;
	cout << "área de objetos (%): " << areaObjetos << endl;
	//calcula a imagem idf.
	while (obj != 0.0) {
		lastobj = obj;
		*matriz=*pm; //faz uma cópia da matriz utilizando a sobrecarga de operador de TCMatriz2D< int >
		// calula a idf da imagem
		filtro->Go(matriz, raio);
		// aplica a abertura na imagem
		if ( filtro->Abertura(matriz, raio) == NULL ) { //aplica a abertura na imagem.
			cerr << "Erro em CDistribuicao::Go! Método Abertura retornou NULL..." << endl;
			if ( filtro )
				delete filtro;
			if ( matriz )
				delete matriz;
			return false;
		}
		//calcula a área dos objetos
		cont = 0;
		for ( int i=0; i < nx; i++ ) {
			for ( int j=0; j < ny; j++) {
				if ( matriz->data2D[i][j] == indice) {
					cont++;
				}
			}
		}
		obj = 100.0*cont/area;
		objt = (lastobj-obj)/areaObjetos;
		//if(objt < 0.0000000) objt = 0.0;
		cout << "raio = " << raio << " | lastobj = " << lastobj << " | obj = " << obj << " | (lastobj-obj)/objetos = " << objt << endl;
		acumulada += objt;
		distribuicao.push_back(objt);
		raio ++;
	}
	cout << "área acumulada: " << acumulada << endl;
	if ( filtro )
		delete filtro;
	if ( matriz )
		delete matriz;
	return true;
}
