#include <Distribuicao/CDistribuicao3D.h>

#ifndef TCFEMorfologiaMatematica3D_h
#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMorfologiaMatematica3D.h>
#endif

#ifndef TCFEMMIDFd3453D_h
#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFd3453D.h>
#endif

#ifndef TCFEMMIDFEuclidiana3D_h
#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFEuclidiana3D.h>
#endif

bool CDistribuicao3D::Go( TCMatriz3D<bool> * _pm3D, Tipos _tipo, Metrica3D _metrica, int indice, int fundo ) {
	pm3D = _pm3D;
	Go ( _tipo, _metrica, indice, fundo );
}

bool CDistribuicao3D::Go( Tipos _tipo, Metrica3D _metrica, int indice, int fundo ) {
	if ( ! pm3D ) { // se o ponteiro para a matriz não estiver definido rotorna false
		cerr << "Erro em CDistribuicao3D::Go! Ponteiro para CMatriz3D não definido!" << endl;
		return false;
	}
	tipo = _tipo;
	int raio = 1;
	int cont = 0;
	int nx = pm3D->NX();
	int ny = pm3D->NY();
	int nz = pm3D->NZ();
	int area = nx * ny * nz;
	double obj = 0.0;
	double objt = 0.0;
	double lastobj = 0.0;
	double acumulada = 0.0;
	distribuicao.clear();

	TCMatriz3D<bool> * matriz = NULL;
	matriz = new TCMatriz3D<bool>(nx, ny, nz); // faz uma cópia da matriz
	if ( ! matriz ) { // se a matriz não foi criada corretamente rotorna false
		cerr << "Erro em CDistribuicao3D::Go! Cópia de CMatriz3D não foi criada!" << endl;
		return false;
	}

	TCFEMorfologiaMatematica3D<bool> * filtro = NULL;
	switch (_metrica){
		case espacial:
			filtro = new TCFEMorfologiaMatematica3D<bool>(matriz, raio, indice, fundo);
			break;
		case d345:
			filtro = new TCFEMMIDFd3453D<bool>(matriz, indice, fundo);
			break;
		case euclidiana:
			filtro = new TCFEMMIDFEuclidiana3D<bool>(matriz, indice, fundo);
			break;
	}

	if ( ! filtro ) { // se o filtro não foi criado corretamente rotorna false
		if ( matriz ) {
			delete matriz;
		}
		cerr << "Erro em CDistribuicao3D::Go! O filtro não foi criado!" << endl;
		return false;
	}

	//calcula a área dos objetos
	for ( int i=0; i < nx; i++ ) {
		for ( int j=0; j < ny; j++) {
			for ( int k=0; k < nz; k++) {
				if ( pm3D->data3D[i][j][k] == indice) {
					cont++;
				}
			}
		}
	}
	objetos = obj = 100.0*cont/area;
	cout << "área de objetos (%): " << objetos << endl;
	//calcula a imagem idf.
	while (obj != 0.0) {
		lastobj = obj;
		*matriz=*pm3D; //faz uma cópia da matriz utilizando a sobrecarga de operador de TCMatriz2D< int >
		// calula a idf da imagem
		filtro->Go(matriz, raio);
		// aplica a abertura na imagem
		if ( filtro->Abertura(matriz, raio) == NULL ) { //aplica a abertura na imagem.
			cerr << "Erro em CDistribuicao3D::Go! Método Abertura retornou NULL..." << endl;
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
				for ( int k=0; k < nz; k++) {
					if ( matriz->data3D[i][j][k] == indice) {
						cont++;
					}
				}
			}
		}
		obj = 100.0*cont/area;
		objt = (lastobj-obj)/objetos;
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

bool CDistribuicao3D::Go( TCMatriz3D<bool> * _pm3D, Tipos _tipo, Metrica _metrica, int indice, int fundo, int numPlanos) {
	pm3D = _pm3D;
	Go ( _tipo, _metrica, indice, fundo, numPlanos );
}

bool CDistribuicao3D::Go( Tipos _tipo, Metrica _metrica, int indice, int fundo, int numPlanos) {
	if ( ! pm3D ) { // se o ponteiro para a matriz não estiver definido rotorna false
		cerr << "Erro em CDistribuicao3D::Go! Ponteiro para CMatriz3D não definido!" << endl;
		return false;
	}

	vector<double> dist; //vetor auxiliar que irá acumular os valores de distribuição.
	double obj = 0.0; //variável auxiliar que irá acumular a área dos objetos.

	int nx = pm3D->NX();

	if (numPlanos < 1) {
		numPlanos = (nx/10); // => 10%
	} else if (numPlanos > nx) {
		numPlanos = nx;
	}
	//calcula e acumula as distribuições de cada plano
	for ( int cont = 0; cont < nx; cont += int(nx/numPlanos) ){
		if ( ! CDistribuicao::Go( pm3D->LePlano(cont), _tipo, _metrica, indice, fundo ) ){
			return false;
		}
		//cerr << "Size of distribuicao: " << distribuicao.size() << endl;
		if (dist.size() < distribuicao.size()) {
			dist.resize(distribuicao.size(), 0.0);
		}
		for (int i = 0; i < distribuicao.size(); i++){
			//cerr << dist[i] << "+" << distribuicao[i] << "=";
			dist[i] += distribuicao[i];
			//cerr << dist[i] << endl;
		}
		obj += objetos;
		//cerr << "Size of dist: " << dist.size() << endl;
	}
	//calcula a média das distribuições;
	distribuicao.resize(dist.size());
	for (int i = 0; i < dist.size(); i++){
		distribuicao[i] = dist[i]/numPlanos;
	}
	objetos = obj/numPlanos;

	return true;
}

