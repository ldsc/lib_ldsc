/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
									Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:
Laboratorio de Desenvolvimento de Software Cientifico 	[LDSC].
@author:          Andre Duarte Bueno
File:             TCMatriz2D.cpp
begin:            Sat Sep 16 2000
copyright:        (C) 2000 by Andre Duarte Bueno
email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef TCMatriz2D_H
#include <Matriz/TCMatriz2D.h>
#endif


// Construtor default. Constroe objeto vazio.
template< typename T >
TCMatriz2D<T>::TCMatriz2D() : CBaseMatriz( P2_X_Y_GRAY_ASCII ) {
	nx = ny = 0;
	data2D.clear();
	numCores = 255;
}

// Construtor que le matriz de arquivo
template< typename T >
TCMatriz2D<T>::TCMatriz2D (string fileName) : CBaseMatriz() {
	nx = ny = 0;
	data2D.clear();
	TCMatriz2D<T>::Read(fileName);
	size_t pos = fileName.rfind("/");
	if (pos!=string::npos)
		path = fileName.substr(0, pos+1);
}

// Construtor le arquivo RAW do disco. Recebe nome do arquivo, largura, altura e tipo da imagem.
template< typename T >
TCMatriz2D<T>::TCMatriz2D(string fileRAW, int _nx, int _ny, EImageType tipo) : CBaseMatriz(tipo) {
	nx = ny = 0;		// será setado em ReadRAW()
	data2D.clear();	// será setado em ReadRAW()
	TCMatriz2D<T>::ReadRAW(fileRAW, _nx, _ny, tipo);
	size_t pos = fileRAW.rfind("/");
	if (pos!=string::npos)
		path = fileRAW.substr(0, pos+1);
}

// Construtor por copia.
template< typename T >
TCMatriz2D<T>::TCMatriz2D (TCMatriz2D<T> & matriz) : CBaseMatriz(matriz.formatoImagem, matriz.path) {
	numCores = matriz.numCores;
	data2D.clear();
	if( TCMatriz2D<T>::AlocaMatriz2D (matriz.nx, matriz.ny) )	{ // Aloca data2D
		int i,j;
#pragma omp parallel for collapse(2) default(shared) private(i,j) // reduction(+:variavel) schedule(static,10)
		for (i = 0; i < nx; i++)	// Copia membro a membro
			for (j = 0; j < ny; j++)
				this->data2D[i][j] = matriz.data2D[i][j];
	}
}

// Cosntrutor aloca matriz vazia. Copia nx e ny, aloca a matriz e preenche a mesma com zeros
template< typename T >
TCMatriz2D<T>::TCMatriz2D (int NX, int NY) : CBaseMatriz( P2_X_Y_GRAY_ASCII ) {
	numCores = 255;
	data2D.clear();
	TCMatriz2D<T>::AlocaMatriz2D (NX, NY);	// aloca data2D
}

// Construtor. Recebe o nome do arquivo de disco e o número do plano a ser lido
template< typename T >
TCMatriz2D<T>::TCMatriz2D (string fileName, int planoZ) : CBaseMatriz() {
	data2D.clear();
	TCMatriz2D<T>::LePlanoZ (fileName, planoZ);
	size_t pos = fileName.rfind("/");
	if (pos!=string::npos)
		path = fileName.substr(0, pos+1);
}

// Construtor. Cria copia com borda extendida de forma que imagem fica centralizada.
template< typename T >
TCMatriz2D<T>::TCMatriz2D (TCMatriz2D<T> & matriz, unsigned int borda) : CBaseMatriz(matriz.formatoImagem, matriz.path) {
	nx = matriz.nx + (2*borda);			// Define dimensoes
	ny = matriz.ny + (2*borda);
	data2D = TCMatriz2D<T>::AlocaMatriz2D(nx, ny);
	if( ! data2D.empty() )	{// Aloca data2D
		//Constante(0); //já está alocando zerada // Zera a imagem criada // posteriormente otimizar, zerando somente as bordas
		int i,j;
#pragma omp parallel for collapse(2) default(shared) private(i,j) // reduction(+:variavel) schedule(static,10)
		for (i = borda; i < nx - borda; i++)	// Copia região central
			for (j = borda; j < ny - borda; j++)
				this->data2D[i][j] = matriz.data2D[i][j];
	}
}

// Aloca memória para a matriz bidimensional
template< typename T >
bool TCMatriz2D<T>::AlocaMatriz2D(int _nx, int _ny) {
	nx = _nx;
	ny = _ny;
	data2D.resize(nx);								// Aloca dimensão x
	if (! data2D.empty() ) {					// se alocou corretamente
		for (int i = 0; i < nx; i++) {
			data2D[i].resize(ny,0);				// Aloca dimensão y e zera o conteúdo
			if ( data2D[i].empty() ) {		// Se a linha nao foi corretamente alocada
				TCMatriz2D<T>::DesalocaMatriz2D (data2D, nx, ny); // Desaloca a matriz para evitar vazamento de memoria
				cerr << "Não alocou Matriz 2D em TCMatriz2D<T>::AlocaMatriz2D" << endl;
				return false;
			}
		}	// O que nao foi alocado esta com NULL e pode ser deletado
		return true;
	} else { // se nao alocou corretamente
		nx = ny = 0;		// ou verifica data2D ou faz nx=ny=0
		cerr << "Não alocou Matriz 2D em TCMatriz2D<T>::AlocaMatriz2D" << endl;
	}
	return false;
}

// Desaloca a memória da matriz 2D
template< typename T >
void TCMatriz2D<T>::DesalocaMatriz2D ( vector< vector<T> > &dat, int nx, int ny) {
	if ( ! dat.empty() ) {
		for (int i = 0; i < nx; i++)
			dat[i].clear();
		dat.clear();
		nx = ny = 0;				// opcional, evita uso indevido de data2D
	}
}

// Lê uma matriz bidimensional do disco. A matriz é  bidimensional mas o acesso é como em um vetor
// #define Real_E1(x,y)	_reData [(y)*_nx + (x)]
template< typename T >
bool TCMatriz2D<T>::Read2D (std::string inputFile, float * & _reData, int _nx, int _ny) {
	// Abre arquivo disco
	ifstream fin (inputFile.c_str ());
	if (fin.fail ())
		return 0;
	// Lê imagem do disco
	float aux;
	for (int cy = 0; cy < (_ny); cy++)
		for (int cx = 0; cx < (_nx); cx++) {
			fin >> aux;
			// Real_E1(cx,cy) = aux;
			_reData[(cy) * _nx + (cx)] = aux;
		}
	fin.close ();
	return 1;
}

// Salva em arquivo, matriz bidimensional inicialmente armazenada em vetor float.
template< typename T >
bool TCMatriz2D<T>::Write2D (std::string inputFile, float * _reData, int _nx, int _ny) {
	// Abre arquivo disco
	ofstream fout (inputFile.c_str ());
	if ( fout.fail() )
		return false;
	// Lê imagem do disco
	for (int cy = 0; cy < (_ny); cy++) {
		for (int cx = 0; cx < (_nx); cx++) {
			fout << _reData[(cy) * _nx + (cx)];
		}
		fout << "\n";
	}
	fout.close ();
	return true;
}

// Sobrecarga do operador +
template< typename T >
TCMatriz2D<T> & TCMatriz2D<T>::operator+ (TCMatriz2D<T> & matriz) {
	int minx = std::min (this->nx, matriz.nx);
	int miny = std::min (this->ny, matriz.ny);
	int i,j;
#pragma omp parallel for collapse(2) default(shared) private(i,j) // reduction(+:variavel) schedule(static,10)
	for (i = 0; i < minx; i++)
		for (j = 0; j < miny; j++)
			this->data2D[i][j] += matriz.data2D[i][j];
	return *this;
}

// Sobrecarga do operador -
template< typename T >
TCMatriz2D<T> & TCMatriz2D<T>::operator- (TCMatriz2D<T> & matriz) {
	int minx = std::min (this->nx, matriz.nx);
	int miny = std::min (this->ny, matriz.ny);
	int i,j;
#pragma omp parallel for collapse(2) default(shared) private(i,j) // reduction(+:variavel) schedule(static,10)
	for (i = 0; i < minx; i++)
		for (j = 0; j < miny; j++)
			this->data2D[i][j] -= matriz.data2D[i][j];
	return *this;
}

// Sobrecarga do operador =
template< typename T >
TCMatriz2D<T> & TCMatriz2D<T>::operator= (TCMatriz2D<T> & matriz) {
	int minx = std::min (this->nx, matriz.nx);
	int miny = std::min (this->ny, matriz.ny);
	int i,j;
#pragma omp parallel for collapse(2) default(shared) private(i,j) // reduction(+:variavel) schedule(static,10)
	for (i = 0; i < minx; i++)
		for (j = 0; j < miny; j++)
			this->data2D[i][j] = matriz.data2D[i][j];	// deve igualar membro a membro
	return *this;
}

// Sobrecarga do operador ==
template< typename T >
bool TCMatriz2D<T>::operator== (TCMatriz2D<T> & pmatriz) {
	int minx = std::min (this->nx, pmatriz.nx);
	int miny = std::min (this->ny, pmatriz.ny);
	for (int i = 0; i < minx; i++)	// percorre as matrizes
		for (int j = 0; j < miny; j++)
			if (this->data2D[i][j] != pmatriz.data2D[i][i])	// se houver algum diferente
				return 0;		// retorna false
	return 1;			// senao retorna true
}

// Sobrecarga do operador !=
template< typename T >
bool TCMatriz2D<T>::operator!= (TCMatriz2D<T> & pmatriz) {
	return !(TCMatriz2D<T>::operator== (pmatriz));
}

// Sobrecarga do operador <<
/*
template< typename T >
ostream & TCMatriz2D<T>::operator<<( ostream &os, const TCMatriz2D<T> &pm ){
	os << "\n- y! --------------------------------------------------------------------";
	for (int j = pm.NY () - 1; j >= 0; j--) {
		os << "\n| ";
		os.width (pm.larguraCampo);
		os << j << " |";
		for (int i = 0; i < pm.NX (); i++)
		{
			os.width (pm.larguraCampo);	// só tem sentido para saida formatada
			os << pm.data2D[i][j] << " ";	// ' ';
		}
		// os<<'\n';
	}
	os << "\n|   y |------------------------------ x-> --------------------------------------\n      |";
	for (int i = 0; i < pm.NX (); i++) {
		os.width (pm.larguraCampo);
		os << i << " ";
	}
	os << "\n-------------------------------------------------------------------------------";
	return os;
}
*/

// Salva em arquivo os dados do cabecalho da matriz (imagem)
template< typename T >
void TCMatriz2D<T>::SalvaCabecalho (ofstream & fout) const {
	if (fout) {	// testa abertura do arquivo
		switch (formatoImagem) {
			case P1_X_Y_ASCII:
				fout << setw (0) << "P1" << '\n' << nx << ' ' << ny;
				break;
			case P2_X_Y_GRAY_ASCII:
				fout << setw (0) << "P2"  << '\n' << nx << ' ' << ny;
				break;
			case P3_X_Y_COLOR_ASCII:
				fout << setw (0) << "P3" << '\n' << nx << ' ' << ny;
				break;
			case P4_X_Y_BINARY:
				fout << setw (0) << "P4" << '\n' << nx << ' ' << ny;
				break;
			case P5_X_Y_GRAY_BINARY:
				fout << setw (0) << "P5" << '\n' << nx << ' ' << ny;
				break;
			case P6_X_Y_COLOR_BINARY:
				fout << setw (0) << "P6" << '\n' << nx << ' ' << ny;
				break;
			default:
				fout << setw (0) << "P1" << '\n' << nx << ' ' << ny;
				cerr << "Formato inválido em TCMatriz2D<T>::SalvaCabecalho. Utilizado valor default!" << endl;
				break;
		} // Valor de nCores é salva em CBaseMatriz, depois de chamar SalvaCabecalho.
	}
}

// Salva dados no formato binario
template< typename T >
void TCMatriz2D<T>::SalvaDadosBinarios (ofstream & fout) const {
	if (fout) {
		int x, bit;
		unsigned char c = 0;
		switch(formatoImagem){
			case P4_X_Y_BINARY: // 1 bite por pixel
				for (int j = 0; j < ny; j++) {
					for (int i = 0; i < nx; i++) {
						x = 7 - i%8;
						bit = (data2D[i][j])%2;
						c = c | (bit << x);
						if ( (i+1)%8 == 0 || i == (nx-1) ) {
							fout << c;
							c = 0;
						}
					}
				}
				break;
			case P5_X_Y_GRAY_BINARY: // 8 bits por pixel = 1 Byte
				for (int j = 0; j < ny; j++) {
					for (int i = 0; i < nx; i++) {
						fout << (unsigned char) data2D[i][j];
					}
				}
				break;
			case P6_X_Y_COLOR_BINARY: // 8 bits red + 8 bits green + 8 bits blue por pixel = 3 Bytes
				cerr << "Formato de arquivo P6_X_Y_COLOR_BINARY não implementado em TCMatriz2D<T>::SalvaDadosBinarios" << endl;
				/* falta implementar matrizes para as cores RGB
				for (int j = 0; j < ny; j++) {
					for (int i = 0; i < nx; i++) {
						fout << (unsigned char) data2Dr[i][j];
						fout << (unsigned char) data2Dg[i][j];
						fout << (unsigned char) data2Db[i][j];
					}
				}
				*/
				break;
			default: cerr << "Formato de arquivo inválido em TCMatriz2D<T>::SalvaDadosBinarios" << endl;
		}
	}
}

// Salva dados "colados" sem espaço (ex.: 00110011110111101010) ou em formato binário
template< typename T >
void TCMatriz2D<T>::SalvaDadosColados (ofstream & fout) const {
	switch(formatoImagem){
		case P1_X_Y_ASCII:
		case P2_X_Y_GRAY_ASCII:
		case P3_X_Y_COLOR_ASCII:
			for (int j = 0; j < ny; j++) {
				for (int i = 0; i < nx; i++) {
					fout << data2D[i][j];
				}
				fout << '\n';
			}
			break;
		case P4_X_Y_BINARY:
		case P5_X_Y_GRAY_BINARY:
		case P6_X_Y_COLOR_BINARY:
			TCMatriz2D<T>::SalvaDadosBinarios(fout);
			break;
		default: cerr << "Formato de arquivo inválido em TCMatriz2D<T>::SalvaDadosColados" << endl;
	}
}

// Salva dados com um espaco (ex.: 0 0 1 1 0 0 1 1 1 1 0 1 1 1 1 0 1 0 1 0) ou em formato binário
template< typename T >
void TCMatriz2D<T>::SalvaDados (ofstream & fout) const {
	switch(formatoImagem){
		case P1_X_Y_ASCII:
		case P2_X_Y_GRAY_ASCII:
		case P3_X_Y_COLOR_ASCII:
			for (int j = 0; j < ny; j++) {
				for (int i = 0; i < nx; i++) {
					fout << data2D[i][j] << ' ';
				}
				fout << '\n';
			}
			break;
		case P4_X_Y_BINARY:
		case P5_X_Y_GRAY_BINARY:
		case P6_X_Y_COLOR_BINARY:
			TCMatriz2D<T>::SalvaDadosBinarios(fout);
			break;
		default: cerr << "Formato de arquivo inválido em TCMatriz2D<T>::SalvaDados" << endl;
	}
}


// Carrega matriz armazenada em arquivo
template< typename T >
bool TCMatriz2D<T>::Read (string fileName, int separado) {
	ifstream fin; // Ponteiro para arquivo de disco.
	CBaseMatriz::AbreArquivo (fin, fileName); // Abre o arquivo de disco no formato correto.
	//fin.open(fileName.c_str());
	if (fin.good ()) { // Se o arquivo foi corretamente aberto
		formatoImagem = CBaseMatriz::VerificaFormato(fin); // Obtem o formato de salvamento
		switch (formatoImagem)	{	// Em funcao do formato de salvamento lê os dados referente ao número de cores/tons de cinza
			case P1_X_Y_ASCII:
			case P4_X_Y_BINARY:
				numCores = 0;
				break;
			case P2_X_Y_GRAY_ASCII:
			case P3_X_Y_COLOR_ASCII:
			case P5_X_Y_GRAY_BINARY:
			case P6_X_Y_COLOR_BINARY:
				numCores = 1;
				break;
			default:
				cerr << "Formato de arquivo inválido em  TCMatriz2D<T>::Read" << endl;
				return false;
		}
		//pega os valore de nx e ny ignorando os comentários
		CBaseMatriz::LeComentarios(fin);
		fin >> nx;
		CBaseMatriz::LeComentarios(fin);
		fin >> ny;
		CBaseMatriz::LeComentarios(fin);
		if( numCores ){
			fin >> numCores;					//pega o número de cores do arquivo.
			CBaseMatriz::LeComentarios(fin);
		}
		if ( TCMatriz2D<T>::AlocaMatriz2D(nx, ny) ) {			// Aloca a matriz de dados
			if (separado)															// Leitura dos dados da matriz
				TCMatriz2D<T>::LeDados (fin);									// Lê os dados separados
			else
				TCMatriz2D<T>::LeDadosColados (fin);					// Lê os dados colados
			fin.close();
			return true;
		}
		fin.close();
		cerr << "Não foi possível alocar matriz 2D em TCMatriz2D<T>::Read()." << endl;
		return false;
	} else {
		cerr << "Não foi possível abrir arquivo " << fileName << " em TCMatriz2D<T>::Read()." << endl;
		return false;
	}
}

// Lê arquivo binário do tipo RAW. Recebe o nome do arquivo e o tipo (P4_X_Y_BINARY (default), P5_X_Y_GRAY_BINARY ou P6_X_Y_COLOR_BINARY).
template< typename T >
bool TCMatriz2D<T>::ReadRAW(string fileName, int _nx, int _ny, EImageType tipo) {
	ifstream fin (fileName.c_str(), ios::binary); // Ponteiro para arquivo de disco.
	if (fin.good ()) { // Se o arquivo foi corretamente aberto
		nx = _nx;
		ny = _ny;
		if ( TCMatriz2D<T>::AlocaMatriz2D (nx, ny) ) {	// Aloca a matriz de dados
			formatoImagem = P5_X_Y_GRAY_BINARY;			// força leitura como tons de cinza
			TCMatriz2D<T>::LeDadosBinarios (fin);				// Lê os dados separados
			formatoImagem = tipo;										// seta o real formato da imagem
			fin.close();
			return true;
		} else {
			nx = ny = 0;
			formatoImagem = INVALID_IMAGE_TYPE;
			fin.close();
			return false;
		}
	} else {
		return false;
	}
}

// Lê os dados de um arquivo de disco
// Os dados estao separados por um " "
template< typename T >
void TCMatriz2D<T>::LeDados (ifstream & fin) {
	switch(formatoImagem){
		case P1_X_Y_ASCII:
		case P2_X_Y_GRAY_ASCII:
			int n;
			for (int j = 0; j < ny; j++) {
				for (int i = 0; i < nx; i++) {
					if (!fin.eof ()) {	// Se NAO chegou ao fim do arquivo entra
						fin >> n;
						data2D[i][j]=n;
					} else {
						data2D[i][j] = 0;	// preenche com zeros
					}
				}
			}
			break;
		case P4_X_Y_BINARY:
		case P5_X_Y_GRAY_BINARY:
		case P6_X_Y_COLOR_BINARY:
			TCMatriz2D<T>::LeDadosBinarios(fin);
			break;
		default: cerr << "Formato de arquivo inválido em TCMatriz2D<T>::LeDados" << endl;
	}
}

// Lê os dados de um arquivo de disco, Os dados estao "colados"
// Ex: 00011101000101
template< typename T >
void TCMatriz2D<T>::LeDadosColados (ifstream & fin) {
	char ch = 0;
	char matrizChar[30] = " ";
	switch(formatoImagem) {
		case P1_X_Y_ASCII:
			for (int j = 0; j < ny; j++) {
				for (int i = 0; i < nx; i++) {	// leitura arquivos 00111101010101
					if (!fin.eof ()) {
						cin.get (ch);	// pega o caracter
						if (ch >= 48 && ch <= 57) {	// se for um número válido 48->0 57->1
							matrizChar[0] = ch;	// copia para string
							data2D[i][j] = atoi (matrizChar);	// e da string para o inteiro
						} else {
							i--;		// se for um \n ou ' ' desconsidera, e retorna contador
						}
					} else {
						data2D[i][j] = 0;	// se chegou ao fim do arquivo, preenche com zeros
					}
				}
			}
			break;
		case P4_X_Y_BINARY:
		case P5_X_Y_GRAY_BINARY:
		case P6_X_Y_COLOR_BINARY:
			TCMatriz2D<T>::LeDadosBinarios(fin);
			break;
		default: cerr << "Formato de arquivo inválido em TCMatriz2D<T>::LeDadosColados" << endl;
	}
}

// Lê os dados de um arquivo de disco
// Os dados estao separados por um " "
template< typename T >
void TCMatriz2D<T>::LeDadosBinarios (ifstream & fin) {
	char c;
	unsigned char c2 = 0;
	int x, bit;
	switch(formatoImagem){
		case P4_X_Y_BINARY: // 1 bit por pixel
			for (int j = 0; j < ny; j++) {
				for (int i = 0; i < nx; i++) {
					if ( i%8 == 0 ){
						fin.read(&c, 1);
						c2 = (unsigned char) c;
					}
					x = 7 -i%8;
					bit = (c2 >> x)%2;
					data2D[i][j] = bit;
				}
			}
			break;
		case P5_X_Y_GRAY_BINARY: // 8 bits por pixel = 1 Byte
			for (int j = 0; j < ny; j++) {
				for (int i = 0; i < nx; i++) {
					fin.read(&c, 1);
					data2D[i][j] = (unsigned char) c;
				}
			}
			break;
		case P6_X_Y_COLOR_BINARY: // 8 bits red + 8 bits green + 8 bits blue por pixel = 3 Bytes
			cerr << "Formato de arquivo P6_X_Y_COLOR_BINARY não implementado em TCMatriz2D<T>::LeDadosBinarios" << endl;
			/* falta implementar matrizes para as cores RGB
			for (int j = 0; j < ny; j++) {
				for (int i = 0; i < nx; i++) {
					fin.read(&c, 1);
					data2Dr[i][j] = (unsigned char) c;
					fin.read(&c, 1);
					data2Dg[i][j] = (unsigned char) c;
					fin.read(&c, 1);
					data2Db[i][j] = (unsigned char) c;
				}
			}
			*/
			break;
		default: cerr << "Formato de arquivo inválido em TCMatriz2D<T>::LeDados" << endl;
	}
}

/*
Le um plano de uma matriz tridimensional.
O objetivo é ter acesso a uma matriz 3D usando uma matriz2D.
Muitas vesez preciso acessar um plano de uma matriz 3D.
Posso abrir a matriz 3D e acessar o plano, mas toda a matriz 3D fica
na memoria, e se a matriz 3D for muito grande pode nao ser possivel.
Portanto em alguns casos é melhor só acessar o plano.
Obs: O valor de planoZ deve iniciar de 0.
O primeiro plano na direcao z é o plano 0.
Se nz=1, uma plano na direcao z, plano 0.
*/
template< typename T >
bool TCMatriz2D<T>::LePlanoZ (string fileName, int planoZ, bool separado) {
	int nz;
	ifstream fin;							// Ponteiro para arquivo de disco
	CBaseMatriz::AbreArquivo (fin, fileName);	// Abre o arquivo de disco no formato correto
	if (fin.good ()) {						// Se o arquivo foi corretamente aberto
		// Obtem o formato de salvamento
		formatoImagem = CBaseMatriz::VerificaFormato(fin);
		switch (formatoImagem)	{				// Em funcao do formato de salvamento lê os dados do cabecalho
			case D1_X_Y_Z_ASCII:
				formatoImagem = P1_X_Y_ASCII;
				numCores = 0; //não buscará do arquivo
				break;
			case D4_X_Y_Z_BINARY:
				formatoImagem = P4_X_Y_BINARY;
				numCores = 0; //não buscará do arquivo
				break;
			case D2_X_Y_Z_GRAY_ASCII:
				formatoImagem = P2_X_Y_GRAY_ASCII;
				numCores = 1; //buscará do arquivo
				break;
			case D3_X_Y_Z_COLOR_ASCII:
				formatoImagem = P3_X_Y_COLOR_ASCII;
				numCores = 1; //buscará do arquivo
				break;
			case D5_X_Y_Z_GRAY_BINARY:
				formatoImagem = P5_X_Y_GRAY_BINARY;
				numCores = 1; //buscará do arquivo
				break;
			case D6_X_Y_Z_COLOR_BINARY:
				formatoImagem = P6_X_Y_COLOR_BINARY;
				numCores = 1; //buscará do arquivo
				break;
			default:
				cerr << "Formato de arquivo inválido em TCMatriz2D<T>::LePlanoZ" << endl;
				return false;
		} // se não retornou false é porque o arquivo armazena uma matriz 3D. Posso ler nx, ny e nz.
		CBaseMatriz::LeComentarios(fin);
		fin >> nx;
		CBaseMatriz::LeComentarios(fin);
		fin >> ny;
		CBaseMatriz::LeComentarios(fin);
		fin >> nz;
		CBaseMatriz::LeComentarios(fin);
		if( numCores == 1 ){ //se foi setado como 1 no switch, busca o número de cores no arquivo
			fin >> numCores;
			CBaseMatriz::LeComentarios(fin);
		}
		if( ! data2D.empty() )
			TCMatriz2D<T>::DesalocaMatriz2D(data2D, nx, ny);
		if (TCMatriz2D<T>::AlocaMatriz2D (nx, ny)) { // Aloca a matriz de dados
			int i, j, k, temp;
			if (planoZ > nz - 1)						// O plano a ser lido nao pode ser maior que nz-1
				planoZ = nz - 1;							// se nz=5 (0,1,2,3,4) planoZ <= 4
			if (separado) {
				for (k = 0; k < planoZ; k++)	// Aqui posiciona o ponteiro
					for (j = 0; j < ny; j++)		// devo ler os elementos desnecessarios
						for (i = 0; i < nx; i++)	// ate encontrar o plano desejado.
							fin >> temp;						// armazena em temp, depois
				TCMatriz2D<T>::LeDados (fin);			// Lê os dados do plano Z selecionado
			} else {
				char ch;
				for (k = 0; k < planoZ; k++)				// Aqui posiciona o ponteiro
					for (j = 0; j < ny; j++)					// devo ler os elementos desnecessarios
						for (i = 0; i < nx; i++) {			// ate encontrar o plano desejado.
							cin.get (ch);
							if (ch == ' ' || ch == '\n')	// Precisa considerar '\n' e ' '
								cin.get (ch);	// testar
						}
				TCMatriz2D<T>::LeDadosColados (fin);		// Lê os dados como colados
			}
			fin.close();
			return true;	// sucesso
		}
		fin.close();
		return false;		// falha
	}
	return false;			// falha
}

// Redimensiona a matriz para as dimensões informadas
template< typename T >
bool TCMatriz2D<T>::Redimensiona(int NX, int NY, int NZ) {
	if( nx != NX || ny != NY ) {
		TCMatriz2D<T>::DesalocaMatriz2D (data2D, nx, ny);
		nx = NX;
		ny = NY;
		NZ = 0; // evitar warning
		data2D.clear();
		TCMatriz2D<T>::AlocaMatriz2D (nx, ny);
		return ! data2D.empty();
	}
	return true; // não precisou redimensionar...
}

// Preenche a matriz com um valor constante
template< typename T >
void TCMatriz2D<T>::Constante (T cte) {
	int i,j;
#pragma omp parallel for collapse(2) default(shared) private(i,j) // reduction(+:variavel) schedule(static,10)
	for (i = 0; i < nx; i++)
		for (j = 0; j < ny; j++)
			data2D[i][j] = cte;
}

// Inverte valores da matriz binária
template< typename T >
void TCMatriz2D<T>::Inverter () {
	if ( formatoImagem == P1_X_Y_ASCII || formatoImagem == P4_X_Y_BINARY ) {
		int i,j;
#pragma omp parallel for collapse(2) default(shared) private(i,j) // reduction(+:variavel) schedule(static,10)
		for (i = 0; i < nx; i++) {
			for (j = 0; j < ny; j++) {
				// se garantir que o tipo do dado é bool, pode utilizar data2D[i][j].flip()
				if (data2D[i][j] == 0) {
					data2D[i][j] = 1;
				} else {
					data2D[i][j] = 0;
				}
			}
		}
	}
}

// Calcula a media dos valores armazenados na matriz
template< typename T >
double TCMatriz2D<T>::Media () const {
	double media = 0.0;
	int i,j;
#pragma omp parallel for collapse(2) default(shared) private(i,j) reduction(+:media) schedule(static,10)
	for (i = 0; i < nx; i++)
		for (j = 0; j < ny; j++)
			media += data2D[i][j];
	return media /= (nx * ny);
}

// Retorna o maior valor encontrado na matriz
template< typename T >
T TCMatriz2D<T>::MaiorValor() const {
	T maior = data2D[0][0];
	for (int i = 0; i < nx; i++)	// percorre a matriz de dados
		for (int j = 0; j < ny; j++)
			if (data2D[i][j] > maior)	// se o valor de data2D for maior
				maior = data2D[i][j];	// fazer maior=data2D
	return maior;
}

//Retorna o menor valor encontrado na matriz
template< typename T >
T TCMatriz2D<T>::MenorValor() const {
	T menor = data2D[0][0];
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			if (data2D[i][j] < menor)
				menor = data2D[i][j];
	return menor;
}

// Retorna o menor valor, diferente de zero, encontrado na matriz
template< typename T >
T TCMatriz2D<T>::MenorValorNzero () const {
	T menor = data2D[0][0];
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			if (data2D[i][j] < menor && data2D[i][j] != 0)
				menor = data2D[i][j];
	return menor;
}

// Retorna o maior valor, diferente de zero, encontrado na matriz
template< typename T >
pair<T,T> TCMatriz2D<T>::MaiorMenorValorNzero() const {
	pair<T,T> maiorMenor;
	maiorMenor.first = data2D[0][0];
	maiorMenor.second = data2D[0][0];
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++) {
			if (data2D[i][j] < maiorMenor.second && data2D[i][j] != 0)
				maiorMenor.second = data2D[i][j];
			if (data2D[i][j] > maiorMenor.first)
				maiorMenor.first = data2D[i][j];
		}
	return maiorMenor;
}

// Troca todos os valores i por j no vetor, retorna o numero de elementos trocados
template< typename T >
int TCMatriz2D<T>::Replace (T i, T j) {
	int contador = 0;
	int k,l;
#pragma omp parallel for collapse(2) default(shared) private(k,l) reduction(+:contador) schedule(static,10)
	for (k = 0; k < nx; k++)	// Pesquisa toda a matriz a procura de i
		for (l = 0; l < ny; l++)
			if (data2D[k][l] == i) {	// se existe algum valor i
				data2D[k][l] = j;	// trocar por j
				contador++;		// acumula o numero de trocas realizadas
			}
	return contador;		// retorna o numero de trocas realizadas
}

// Rotaciona a matriz 90 graus no eixo informado
template< typename T >
bool TCMatriz2D<T>::Rotacionar90 () {
	TCMatriz2D<T> * pmtmp = NULL;
	pmtmp = new TCMatriz2D< T >( *this );
	if ( ! pmtmp )
		return false;
	int _nx = nx; // precisa pegar os valores das dimensões, pois se a imagem não for um cubo perfeito, seus valores serão alterados.
	int _ny = ny;
	if (nx != ny) {
		if ( ! Redimensiona(ny, nx) )
			return false ;
	}
	int i,j;
#pragma omp parallel for collapse(2) default(shared) private(i,j) // reduction(+:variavel) schedule(static,10)
	for (i = 0; i < _nx; i++) {
		for (j = 0; j < _ny; j++) {
			data2D[_ny-1-j][i] = pmtmp->data2D[i][j];
		}
	}
	delete pmtmp;
	return true;
}

// Seta o formatoImagem da matriz
template< typename T >
void TCMatriz2D<T>::SetFormato(EImageType _formato){
	if(formatoImagem != _formato){
		switch(_formato){
			case P1_X_Y_ASCII:
			case P2_X_Y_GRAY_ASCII:
			case P3_X_Y_COLOR_ASCII:
			case P4_X_Y_BINARY:
			case P5_X_Y_GRAY_BINARY:
			case P6_X_Y_COLOR_BINARY:
				formatoImagem = _formato;
				break;
			default:
				cerr << "Foi informato um formato inválido em TCMatriz2D<T>::SetFormato()" << endl;
				break;
		}
	}
}
