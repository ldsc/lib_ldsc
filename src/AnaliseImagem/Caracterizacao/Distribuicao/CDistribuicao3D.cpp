#include <AnaliseImagem/Caracterizacao/Distribuicao/CDistribuicao3D.h>

#ifndef TCFEMorfologiaMatematica3D_h
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMorfologiaMatematica3D.h>
#endif

#ifndef TCFEMMIDFd3453D_h
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd3453D.h>
#endif

#ifndef TCFEMMIDFEuclidiana3D_h
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFEuclidiana3D.h>
#endif

#include <cstring>

/// Construtor (recebe arquivo de distribuição)
CDistribuicao3D::CDistribuicao3D( std::string filename ){
	ifstream is;
	is.open(filename);
	char line[64];
	std::string auxs;
	int auxi;
	double auxd;
	if ( is.is_open() ) {
		cout << "Importing distribution file..." << endl;
		is.getline(line, 64);
		if ( strcmp(line,"# Distribution of Pores Size") != 0 ) {
			tipo = dtp;
		} else if ( strcmp(line,"# Distribution of Throats Size") != 0 ) {
			tipo = dtg;
		} else if ( strcmp(line, "# Distribution of Solids Size") != 0 ) {
			tipo = dts;
		} else {
			std::cerr << "Arquivo de distribuição inválido!" << std::endl;
		}
		is >> auxs; //#
		is >> auxs; //Size:
		is >> auxi;
		cout << "Size: " << auxi << endl;
		distribuicao.resize(auxi);
		is >> auxs; //#
		is >> auxs; //Porosity: ou # Solids:
		is >> auxd;
		cout << "Porosity: " << auxd << endl;
		areaObjetos = auxd;
		is >> auxs; //#
		is >> auxs; //Radius
		is >> auxs; //Value
		for (int i = 0; i < distribuicao.size(); ++i) {
			is >> auxi; //1, 2, 3 ...
			is >> auxd;
			cout << auxi << "\t" << auxd << endl;
			distribuicao[i] = auxd;
		}
		cout << "Distribution file imported!" << endl;
	} else {
		std::cerr << "Não foi possível abrir os arquivos de distribuição de tamanho de poros e gargantas!" << std::endl;
	}
}

bool CDistribuicao3D::Go( TCMatriz3D<bool> * _pm3D, Tipos _tipo, Metrica3D _metrica, int indice, int fundo ) {
	pm3D = _pm3D;
	return Go ( _tipo, _metrica, indice, fundo );
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

	TCMatriz3D<bool> * matriz = nullptr;
	matriz = new TCMatriz3D<bool>(nx, ny, nz); // faz uma cópia da matriz
	if ( ! matriz ) { // se a matriz não foi criada corretamente rotorna false
		cerr << "Erro em CDistribuicao3D::Go! Cópia de CMatriz3D não foi criada!" << endl;
		return false;
	}

	TCFEMorfologiaMatematica3D<bool> * filtro = nullptr;
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
	areaObjetos = obj = 100.0*cont/area;
	cout << "área de objetos (%): " << areaObjetos << endl;
	//calcula a imagem idf.
	while (obj != 0.0) {
		lastobj = obj;
		*matriz=*pm3D; //faz uma cópia da matriz utilizando a sobrecarga de operador de TCMatriz2D< int >
		// calula a idf da imagem
		filtro->Go(matriz, raio);
		// aplica a abertura na imagem
		if ( filtro->Abertura(matriz, raio) == nullptr ) { //aplica a abertura na imagem.
			cerr << "Erro em CDistribuicao3D::Go! Método Abertura retornou nullptr..." << endl;
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

bool CDistribuicao3D::Go( TCMatriz3D<bool> * _pm3D, Tipos _tipo, Metrica _metrica, int indice, int fundo, int numPlanos) {
	pm3D = _pm3D;
	return Go ( _tipo, _metrica, indice, fundo, numPlanos );
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
		obj += areaObjetos;
		//cerr << "Size of dist: " << dist.size() << endl;
	}
	//calcula a média das distribuições;
	distribuicao.resize(dist.size());
	for (int i = 0; i < dist.size(); i++){
		distribuicao[i] = dist[i]/numPlanos;
	}
	areaObjetos = obj/numPlanos;

	return true;
}

