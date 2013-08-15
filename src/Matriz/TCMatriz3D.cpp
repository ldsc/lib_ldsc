/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
===============================================================================
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
	[LDSC].
@author:          Andre Duarte Bueno
File:             TCMatriz3D.cpp
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

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef TCMatriz3D_H
#include <Matriz/TCMatriz3D.h>
#endif

using namespace std;

// Construtor default. Constroe objeto vazio.
template< typename T >
TCMatriz3D<T>::TCMatriz3D () : CBaseMatriz()  {
	nx = ny = nz = 0;
	data3D = NULL;
	formatoImagem = D2_X_Y_Z_GRAY_ASCII;
	numCores = 255;
}

// Construtor que le matriz de arquivo
template< typename T >
TCMatriz3D<T>::TCMatriz3D (string fileName) : CBaseMatriz() {
	nx = ny = nz = 0;
	data3D = NULL;
	TCMatriz3D<T>::Read(fileName);
	size_t pos = fileName.rfind("/");
	if (pos!=string::npos)
		path = fileName.substr(0, pos+1);
}

// Construtor le arquivo RAW do disco. Recebe nome do arquivo, largura, altura, profundidade e tipo (D4_X_Y_Z_BINARY (default), D5_X_Y_Z_GRAY_BINARY ou D6_X_Y_Z_COLOR_BINARY) da imagem.
template< typename T >
TCMatriz3D<T>::TCMatriz3D(string fileRAW, int _nx, int _ny, int _nz, EImageType tipo) : CBaseMatriz(tipo) {
	nx = ny = nz = 0;	// será setado em ReadRAW()
	data3D = NULL;	// será setado em ReadRAW()
	TCMatriz3D<T>::ReadRAW(fileRAW, _nx, _ny, _nz, tipo);
	size_t pos = fileRAW.rfind("/");
	if (pos!=string::npos)
		path = fileRAW.substr(0, pos+1);

}

// Construtor por copia.
template< typename T >
TCMatriz3D<T>::TCMatriz3D (TCMatriz3D<T> & matriz) : CBaseMatriz(matriz.formatoImagem, matriz.path) {
	nx = matriz.nx;
	ny = matriz.ny;
	nz = matriz.nz;
	numCores = matriz.numCores;
	data3D = NULL;
	if(TCMatriz3D<T>::AlocaMatriz3D (nx, ny, nz)) {
		int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
		for (i = 0; i < nx; i++)
			for (j = 0; j < ny; j++)
				for (k = 0; k < nz; k++)
					data3D[i][j][k] = matriz.data3D[i][j][k];
	}
}

// Cosntrutor aloca matriz vazia. Copia nx,ny e nz, aloca a matriz e preenche a mesma com zeros
template< typename T >
TCMatriz3D<T>::TCMatriz3D (int NX, int NY, int NZ) : CBaseMatriz() {
	nx = NX;			// define valores
	ny = NY;			// em aloca garante que sejam positivos
	nz = NZ;
	formatoImagem = D2_X_Y_Z_GRAY_ASCII;
	numCores = 255;
	data3D = NULL;
	TCMatriz3D<T>::AlocaMatriz3D (nx, ny, nz);	// aloca data3D
}

// Aloca memória para a matriz tridimensional
template< typename T >
bool TCMatriz3D<T>::AlocaMatriz3D(int _nx, int _ny, int _nz) {
	nx = _nx;
	ny = _ny;
	nz = _nz;
	if(data3D)
		TCMatriz3D<T>::DesalocaMatriz3D (data3D, nx, ny, nz);
	data3D = NULL;
	data3D = new vector<T> *[nx];							// Aloca dimensão x
	if ( data3D ) {					// se alocou corretamente
		int i, j;
		//analizar... #pragma omp parallel for //collapse(2) schedule(dynamic,10)
		for (i = 0; i < nx; i++){
			data3D[i] = NULL;					// Aloca dimensão y
			data3D[i] = new vector<T> [ny];
			if ( data3D[i] ) {  // Se alocou corretamente
				for (j = 0; j < ny; j++) {
					data3D[i][j].resize(nz,0);// Aloca dimensão y e zera o conteúdo
					if ( data3D[i][j].empty() ) { // Se não alocou corretamente
						TCMatriz3D<T>::DesalocaMatriz3D (data3D, nx, ny, nz); // Desaloca a matriz para evitar vazamento de memoria
						cerr << "Não alocou data3D[i][j] em TCMatriz3D<T>::AlocaMatriz3D" << endl;
						return false;
					}
				}
			} else {	// Se não alocou corretamente
				TCMatriz3D<T>::DesalocaMatriz3D (data3D, nx, ny, nz); // Desaloca a matriz para evitar vazamento de memoria
				cerr << "Não alocou data3D[i] em TCMatriz3D<T>::AlocaMatriz3D" << endl;
				return false;
			}
		}
	} else {
		nx = ny = nz = 0;		// ou o usuario verifica dat ou faz nx=ny=0
		cerr << "Não alocou data3D em TCMatriz3D<T>::AlocaMatriz3D" << endl;
		return false;			// informa retornando 0
	}
	return true;
}

// Desaloca a memória da matriz 3D
template< typename T >
void TCMatriz3D<T>::DesalocaMatriz3D (vector<T>** &dat, int nx, int ny, int nz) {
	if ( dat ) {
		for (int i = 0; i < nx; i++) {
			for (int j = 0; j < ny; j++) {
				dat[i][j].clear();
			}
			delete [] dat[i];
			dat[i]=NULL;
		}
		delete [] dat;
		dat = NULL;
		nx = ny = nz = 0;
	}
}

// Sobrecarga do operador +
template< typename T >
TCMatriz3D<T> & TCMatriz3D<T>::operator+ (TCMatriz3D<T> & m2) {
	int minx = std::min (this->nx, m2.nx);
	int miny = std::min (this->ny, m2.ny);
	int minz = std::min (this->nz, m2.nz);
	int i,j,k;
	// deve somar membro a membro
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for (i = 0; i < minx; i++)
		for (j = 0; j < miny; j++)
			for (k = 0; k < minz; k++)
				this->data3D[i][j][k] += m2.data3D[i][j][k];
	return *this;
}

// Sobrecarga do operador -
template< typename T >
TCMatriz3D<T> & TCMatriz3D<T>::operator- (TCMatriz3D<T> & m2) {
	int minx = std::min (this->nx, m2.nx);
	int miny = std::min (this->ny, m2.ny);
	int minz = std::min (this->nz, m2.nz);
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for (i = 0; i < minx; i++)
		for (j = 0; j < miny; j++)
			for (k = 0; k < minz; k++)
				this->data3D[i][j][k] -= m2.data3D[i][j][k];	// subtrae membro a membro
	return *this;
}

// Sobrecarga do operador =
template< typename T >
TCMatriz3D<T> & TCMatriz3D<T>::operator= (TCMatriz3D & m2) {
	int minx = std::min (this->nx, m2.nx);
	int miny = std::min (this->ny, m2.ny);
	int minz = std::min (this->nz, m2.nz);
	int i,j,k;
	// deve igualar membro a membro
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for (i = 0; i < minx; i++)
		for (j = 0; j < miny; j++)
			for (k = 0; k < minz; k++)
				this->data3D[i][j][k] = m2.data3D[i][j][k];
	return *this;
}

// Sobrecarga do operador ==
template< typename T >
bool TCMatriz3D<T>::operator== (TCMatriz3D<T> & pmatriz) {
	if ( (this->nx != pmatriz.nx) || (this->ny != pmatriz.ny) || (this->nz != pmatriz.nz) )
		return false;
	for (int i=0; i < nx; i++)
		for (int j=0; j < ny; j++)
			for (int k=0; k < nz; k++)
				if (this->data3D[i][j][k] != pmatriz.data3D[i][j][k])	// se houver algum diferente
					return false;		// retorna false
	return true;			// senao retorna true
}

// Sobrecarga do operador !=
template< typename T >
bool TCMatriz3D<T>::operator!= (TCMatriz3D<T> & pmatriz) {
	return !(TCMatriz3D<T>::operator == (pmatriz));
}

// Sobrecarga do operador <<
template< typename T >
ostream & operator<< (ostream & os, const TCMatriz3D<T> & pm) {
	for (int k = 0; k < pm.NZ (); k++) {
		os << "\n- y! --------------------------------------------------------------------";
		for (int j = pm.NY () - 1; j >= 0; j--) {
			os << "\n| ";
			os.width (pm.larguraCampo);
			os << j << " |";
			for (int i = 0; i < pm.NX (); i++) {
				os.width (pm.larguraCampo);
				os << pm.data3D[i][j][k] << " ";	// ' ';
			}
			// os<<'\n';
		}
		os << "\n|   y |------------------------------ x-> --------------------------------------\n      |";
		for (int i = 0; i < pm.NX (); i++) {
			os.width (pm.larguraCampo);
			os << i << " ";
		}
		os << "\n-------------------------------------------------------------------------------";
		os << "\nz=" << k;
	}
	return os;
}

// Salva em arquivo os dados do cabecalho da matriz (imagem)
template< typename T >
void TCMatriz3D<T>::SalvaCabecalho (ofstream & fout) const {
	if (fout) { // testa abertura do arquivo
		switch ( formatoImagem ) {
			case D1_X_Y_Z_ASCII:
				fout << setw (0) << "D1" << '\n' << nx << ' ' << ny << ' ' << nz;
				break;
			case D2_X_Y_Z_GRAY_ASCII:
				fout << setw (0) << "D2" << '\n' << nx << ' ' << ny << ' ' << nz;
				break;
			case D3_X_Y_Z_COLOR_ASCII:
				fout << setw (0) << "D3" << '\n' << nx << ' ' << ny << ' ' << nz;
				break;
			case D4_X_Y_Z_BINARY:
				fout << setw (0) << "D4" << '\n' << nx << ' ' << ny << ' ' << nz;
				break;
			case D5_X_Y_Z_GRAY_BINARY:
				fout << setw (0) << "D5" << '\n' << nx << ' ' << ny << ' ' << nz;
				break;
			case D6_X_Y_Z_COLOR_BINARY:
				fout << setw (0) << "D6" << '\n' << nx << ' ' << ny << ' ' << nz;
				break;
			default:
				fout << setw (0) << "D1" << '\n' << nx << ' ' << ny << ' ' << nz;
				cerr << "Formato inválido em TCMatriz2D<T>::SalvaCabecalho. Utilizado valor default!" << endl;
				break;
		} // Valor de nCores é salva em CBaseMatriz, depois de chamar SalvaCabecalho.
	}
}

// Salva dados no formato binario
template< typename T >
void TCMatriz3D<T>::SalvaDadosBinarios (ofstream & fout) const {
	if (fout) {
		int x, bit;
		unsigned char c = 0;
		switch(formatoImagem){
			case D4_X_Y_Z_BINARY: // 1 bite por pixel
				for (int k = 0; k < nz; k++) {
					for (int j = 0; j < ny; j++) {
						for (int i = 0; i < nx; i++) {
							x = 7 - i%8;
							bit = (data3D[i][j][k])%2;
							c = c | (bit << x);
							if ( (i+1)%8 == 0 || i == (nx-1) ) {
								fout << c;
								c = 0;
							}
						}
					}
				}
				break;
			case D5_X_Y_Z_GRAY_BINARY: // 8 bits por pixel = 1 Byte - funcional para imagens com variação de tons de 0 a 255.
				for (int k = 0; k < nz; k++) {
					for (int j = 0; j < ny; j++) {
						for (int i = 0; i < nx; i++) {
							fout << (unsigned char) data3D[i][j][k];
						}
					}
				}
				break;
			case D6_X_Y_Z_COLOR_BINARY: // 8 bits red + 8 bits green + 8 bits blue por pixel = 3 Bytes
				cerr << "Formato de arquivo D6_X_Y_Z_COLOR_BINARY não implementado em TCMatriz3D<T>::SalvaDadosBinarios" << endl;
				/* falta implementar matrizes para as cores RGB
				for (int k = 0; k < nz; k++) {
					for (int j = 0; j < ny; j++) {
						for (int i = 0; i < nx; i++) {
							fout << (unsigned char) data3Dr[i][j][k];
							fout << (unsigned char) data3Dg[i][j][k];
							fout << (unsigned char) data3Db[i][j][k];
						}
					}
				}
				*/
				break;
			default: cerr << "Formato de arquivo inválido em TCMatriz3D<T>::SalvaDadosBinarios" << endl;
		}
	}
}

// Salva dados "colados" sem espaço (ex.: 00110011110111101010) ou em formato binário
template< typename T >
void TCMatriz3D<T>::SalvaDadosColados (ofstream & fout) const {
	switch(formatoImagem){
		case D1_X_Y_Z_ASCII:
		case D2_X_Y_Z_GRAY_ASCII:
		case D3_X_Y_Z_COLOR_ASCII:
			for (int k = 0; k < nz; k++) {
				for (int j = 0; j < ny; j++) {
					for (int i = 0; i < nx; i++) {
						fout << data3D[i][j][k];
					}
					fout << '\n';
				}
				fout << '\n';
			}
			break;
		case D4_X_Y_Z_BINARY:
		case D5_X_Y_Z_GRAY_BINARY:
		case D6_X_Y_Z_COLOR_BINARY:
			TCMatriz3D<T>::SalvaDadosBinarios(fout);
			break;
		default: cerr << "Formato de arquivo inválido em TCMatriz3D<T>::SalvaDados" << endl;
	}
}

// Salva dados com um espaco (ex.: 0 0 1 1 0 0 1 1 1 1 0 1 1 1 1 0 1 0 1 0) ou em formato binário
template< typename T >
void TCMatriz3D<T>::SalvaDados (ofstream & fout) const {
	switch(formatoImagem){
		case D1_X_Y_Z_ASCII:
		case D2_X_Y_Z_GRAY_ASCII:
		case D3_X_Y_Z_COLOR_ASCII:
			for (int k = 0; k < nz; k++) {
				for (int j = 0; j < ny; j++) {
					for (int i = 0; i < nx; i++) {
						fout << data3D[i][j][k] << ' ';
					}
					fout << '\n';
				}
				//fout << '\n';
			}
			break;
		case D4_X_Y_Z_BINARY:
		case D5_X_Y_Z_GRAY_BINARY:
		case D6_X_Y_Z_COLOR_BINARY:
			TCMatriz3D<T>::SalvaDadosBinarios(fout);
			break;
		default: cerr << "Formato de arquivo inválido em TCMatriz3D<T>::SalvaDados" << endl;
	}
}

// Carrega matriz armazenada em arquivo
template< typename T >
bool TCMatriz3D<T>::Read (string fileName, int separado) {
	ifstream fin;									// Ponteiro para arquivo de disco
	CBaseMatriz::AbreArquivo (fin, fileName);			// Abre o arquivo de disco no formato correto
	//fin.open (fileName.c_str ());		// Abre o arquivo de disco no formato ascii
	if (fin.good ()) { 								// Se o arquivo foi corretamente aberto
		formatoImagem = CBaseMatriz::VerificaFormato(fin); // Obtem o formato de salvamento
		switch (formatoImagem)	{	// Em funcao do formato de salvamento lê os dados referente ao número de cores/tons de cinza
			case D1_X_Y_Z_ASCII:
			case D4_X_Y_Z_BINARY:
				numCores = 0;
				break;
			case D2_X_Y_Z_GRAY_ASCII:
			case D3_X_Y_Z_COLOR_ASCII:
			case D5_X_Y_Z_GRAY_BINARY:
			case D6_X_Y_Z_COLOR_BINARY:
				numCores = 1;
				break;
			default:
				cerr << "Formato de arquivo inválido em  TCMatriz3D<T>::Read" << endl;
				return false;
		}
		//pega os valore de nx, ny e nz ignorando os comentários
		CBaseMatriz::LeComentarios(fin);
		fin >> nx;
		CBaseMatriz::LeComentarios(fin);
		fin >> ny;
		CBaseMatriz::LeComentarios(fin);
		fin >> nz;
		CBaseMatriz::LeComentarios(fin);
		if( numCores ){
			fin >> numCores;					//pega o número de cores do arquivo.
			CBaseMatriz::LeComentarios(fin);
		}
		if ( TCMatriz3D<T>::AlocaMatriz3D(nx, ny, nz) ) {	// Aloca a matriz de dados
			if (separado)																		// Leitura dos dados da matriz
				TCMatriz3D<T>::LeDados (fin);									// Lê os dados separados
			else
				TCMatriz3D<T>::LeDadosColados (fin);					// Lê os dados colados
			fin.close();
			return true;
		}
		fin.close();
		cerr << "Não foi possível alocar matriz 2D em TCMatriz3D<T>::Read()." << endl;
		return false;
	} else {
		cerr << "Não foi possível abrir arquivo " << fileName << " em TCMatriz3D<T>::Read()." << endl;
		return false;
	}
}

// Lê arquivo binário do tipo RAW. Recebe o nome do arquivo e o tipo
template< typename T >
bool TCMatriz3D<T>::ReadRAW(string fileName, int _nx, int _ny, int _nz, EImageType tipo) {
	ifstream fin (fileName.c_str(), ios::binary); // Ponteiro para arquivo de disco.
	if (fin.good ()) { // Se o arquivo foi corretamente aberto
		nx = _nx;
		ny = _ny;
		nz = _nz;
		if ( TCMatriz3D<T>::AlocaMatriz3D(nx, ny, nz) ) {			// Aloca a matriz de dados
			formatoImagem = D5_X_Y_Z_GRAY_BINARY; // força leitura como tons de cinza
			TCMatriz3D<T>::LeDadosBinarios (fin);			// Lê os dados separados
			formatoImagem = tipo;									// seta o real formato da imagem
			fin.close();
			return true;
		} else {
			nx = ny = nz = 0;
			formatoImagem = INVALID_IMAGE_TYPE;
			fin.close();
			return false;
		}
	} else {
		return false;
	}

}

// Retorna matriz 2D referente ao plano/eixo informados
template< typename T >
TCMatriz2D<T> * TCMatriz3D<T>::LePlano (unsigned int planoZ, E_eixo direcao) {
	if ( ! data3D )
		return NULL;
	TCMatriz2D<T> *pm2D = NULL;
	pm2D = new TCMatriz2D<T>(nx, ny);	// Aloca a matriz de dados
	if ( ! pm2D )
		return NULL;
	if ( LePlano( pm2D, planoZ, direcao) )
		return pm2D;
	else
		return NULL;
}

// Carrega matriz 2D infomada com o conteúdo do plano/eixo informados
template< typename T >
bool TCMatriz3D<T>::LePlano (TCMatriz2D<T> * pm2D, int plano, E_eixo direcao) {
	if ( ! data3D || ! pm2D)
		return false;
	int i, j;
	switch(direcao){
		case EIXO_X:
			//cerr << "aqui é x" << endl;
			if (plano > nx - 1)
				plano = nx - 1;
			pm2D->Redimensiona(nz, ny);							// gira a visão
			//pm2D->Redimensiona(ny, nz);							// gira o eixo
			for (j = 0; j < nz; j++)
				for (i = 0; i < ny; i++)
					pm2D->data2D[nz-j-1][i] = data3D[plano][i][j]; 	// gira a visão
			//pm2D->data2D[i][j] = data3D[plano][i][j];		// gira o eixo
			break;
		case EIXO_Y:
			//cerr << "aqui é y" << endl;
			if (plano > ny - 1)
				plano = ny- 1;
			pm2D->Redimensiona(nx, nz);
			for (j = 0; j < nz; j++)
				for (i = 0; i < nx; i++)
					pm2D->data2D[i][nz-j-1] = data3D[i][plano][j];	// gira a visão
			//pm2D->data2D[i][j] = data3D[i][plano][j];		// gira o eixo
			break;
			// caso direção seja == z ou Z ou k ou K
		case EIXO_Z:
			//cerr << "aqui é z" << endl;
			if (plano > nz - 1)
				plano = nz - 1;
			pm2D->Redimensiona(nx, ny);
			for (j = 0; j < ny; j++)
				for (i = 0; i < nx; i++)
					pm2D->data2D[i][j] = data3D[i][j][plano];
			break;
			// caso direção informada seja incorreta o default é z
		default:
			//cerr << "aqui é default z" << endl;
			if (plano > nz - 1)
				plano = nz - 1;
			pm2D->Redimensiona(nx, ny);
			for (j = 0; j < ny; j++)
				for (i = 0; i < nx; i++)
					pm2D->data2D[i][j] = data3D[i][j][plano];		// o eixo está na mesma direção da visão
	}
	return true;
}

// Rotaciona a matriz 90 graus no eixo informado
template< typename T >
bool TCMatriz3D<T>::Rotacionar90 (E_eixo axis){
	TCMatriz3D<T> * pmtmp = NULL;
	pmtmp = new TCMatriz3D<T>( *this );
	if( ! pmtmp )
		return false;
	int _nx = nx; // precisa pegar os valores das dimensões, pois se a imagem não for um cubo perfeito, seus valores serão alterados.
	int _ny = ny;
	int _nz = nz;
	int i,j,k;
	switch (axis){
		case EIXO_X:
			if (ny != nz){
				if ( ! Redimensiona(nx, nz, ny) )
					return false ;
			}
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
			for (i = 0; i < _nx; i++){
				for (j = 0; j < _ny; j++){
					for (k = 0; k < _nz; k++){
						data3D[i][k][nz-1-j] = pmtmp->data3D[i][j][k];
					}
				}
			}
			break;
		case EIXO_Y:
			if (nx != nz){
				if ( ! Redimensiona(nz, ny, nx) )
					return false ;
			}
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
			for (j = 0; j < _ny; j++){
				for (i = 0; i < _nx; i++){
					for (k = 0; k < _nz; k++){
						data3D[k][j][nz-1-i] = pmtmp->data3D[i][j][k];
					}
				}
			}
			break;
		case EIXO_Z:
			if (nx != ny){
				if ( ! Redimensiona(ny, nx, nz) )
					return false ;
			}
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
			for (k = 0; k < _nz; k++){
				for (j = 0; j < _ny; j++){
					for (i = 0; i < _nx; i++){
						data3D[nx-1-j][i][k] = pmtmp->data3D[i][j][k];
					}
				}
			}
			break;
		default:
			return false;
	}
	delete pmtmp;
	return true;
}

// Cria e retorna uma nova matriz 3D que será um recorte da matriz (this).
template< typename T >
TCMatriz3D<T>* TCMatriz3D<T>::Crop (int startX, int endX, int startY, int endY, int startZ, int endZ) {
	if (startX >= endX || startY >= endY || startZ >= endZ) {
		cerr << "O valor inicial deve ser menor que o falor final" << endl;
		return NULL;
	}

	if (endX > nx || endY > ny || endZ > nz){
		cerr << "O valor final deve ser menor que o tamanho da matriz original (this)" << endl;
		return NULL;
	}

	int _nx = endX-startX;
	int _ny = endY-startY;
	int _nz = endZ-startZ;

	TCMatriz3D<T> * pmtmp = NULL;
	pmtmp = new TCMatriz3D<T>( _nx, _ny, _nz );
	if( ! pmtmp )
		return NULL;
	pmtmp->SetFormato(this->GetFormato());
	pmtmp->NumCores(this->NumCores());
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for (i = 0; i < _nx; i++) {
		for (j = 0; j < _ny; j++) {
			for (k = 0; k < _nz; k++) {
				pmtmp->data3D[i][j][k] = this->data3D[i+startX][j+startY][k+startZ];
			}
		}
	}
	return pmtmp;
}

//Le do arquivo informado os dados da matriz.
template< typename T >
void TCMatriz3D<T>::LeDados (ifstream & fin) {
	switch(formatoImagem){
		case D1_X_Y_Z_ASCII:
		case D2_X_Y_Z_GRAY_ASCII:
			int n;
			for (int k = 0; k < nz; k++) {
				for (int j = 0; j < ny; j++) {
					for (int i = 0; i < nx; i++) {
						if (!fin.eof ()) {	// Se NAO chegou ao fim do arquivo entra
							fin >> n;
							data3D[i][j][k] = n;
						} else {
							data3D[i][j][k] = 0;	// preenche com zeros
						}
					}
				}
			}
			break;
		case D4_X_Y_Z_BINARY:
		case D5_X_Y_Z_GRAY_BINARY:
		case D6_X_Y_Z_COLOR_BINARY:
			TCMatriz3D<T>::LeDadosBinarios(fin);
			break;
		default: cerr << "Formato de arquivo inválido em TCMatriz3D<T>::LeDados" << endl;
	}
}

// Lê os dados de um arquivo de disco, Os dados estao "colados"
// Ex: 00011101000101
template< typename T >
void TCMatriz3D<T>::LeDadosColados (ifstream & fin) {
	char ch = 0;
	char matrizChar[30] = " ";
	switch(formatoImagem) {
		case D1_X_Y_Z_ASCII:
			for (int k = 0; k < nz; k++) {
				for (int j = 0; j < ny; j++) {
					for (int i = 0; i < nx; i++) {	// leitura arquivos 00111101010101
						if (!fin.eof ()) {
							cin.get (ch);	// pega o caracter
							if (ch >= 48 && ch <= 57) {	// se for um número válido 48->0 57->1
								matrizChar[0] = ch;	// copia para string
								data3D[i][j][k] = atoi (matrizChar);	// e da string para o inteiro
							} else {
								i--;		// se for um \n ou ' ' desconsidera, e retorna contador
							}
						} else {
							data3D[i][j][k] = 0;	// se chegou ao fim do arquivo, preenche com zeros
						}
					}
				}
			}
			break;
		case D4_X_Y_Z_BINARY:
		case D5_X_Y_Z_GRAY_BINARY:
		case D6_X_Y_Z_COLOR_BINARY:
			TCMatriz3D<T>::LeDadosBinarios(fin);
			break;
		default: cerr << "Formato de arquivo inválido em TCMatriz3D<T>::LeDadosColados" << endl;
	}
}

// Lê os dados de um arquivo de disco
// Os dados estao separados por um " "
template< typename T >
void TCMatriz3D<T>::LeDadosBinarios (ifstream & fin) {
	char c;
	unsigned char c2 = 0;
	int x, bit;
	switch(formatoImagem){
		case D4_X_Y_Z_BINARY: // 1 bite por pixel
			for (int k = 0; k < nz; k++) {
				for (int j = 0; j < ny; j++) {
					for (int i = 0; i < nx; i++) {
						if ( i%8 == 0 ){
							fin.read(&c, 1);
							c2 = (unsigned char) c;
						}
						x = 7 -i%8;
						bit = (c2 >> x)%2;
						data3D[i][j][k] = bit;
					}
				}
			}
			break;
		case D5_X_Y_Z_GRAY_BINARY: // 8 bits por pixel = 1 Byte
            for (int k = 0; k < nz; k++) {
				for (int j = 0; j < ny; j++) {
					for (int i = 0; i < nx; i++) {
						fin.read(&c, 1);
						data3D[i][j][k] = (unsigned char) c;
					}
				}
			}
            break;
		case D6_X_Y_Z_COLOR_BINARY: // 8 bits red + 8 bits green + 8 bits blue por pixel = 3 Bytes
			cerr << "Formato de arquivo D6_X_Y_Z_COLOR_BINARY não implementado em TCMatriz3D<T>::LeDadosBinarios" << endl;
			/* falta implementar matrizes para as cores RGB
			for (int k = 0; k < nz; k++) {
				for (int j = 0; j < ny; j++) {
					for (int i = 0; i < nx; i++) {
						fin.read(&c, 1);
						data3Dr[i][j][k] = (unsigned char) c;
						fin.read(&c, 1);
						data3Dg[i][j][k] = (unsigned char) c;
						fin.read(&c, 1);
						data3Db[i][j][k] = (unsigned char) c;
					}
				}
			}
			*/
			break;
		default: cerr << "Formato de arquivo inválido em TCMatriz3D<T>::LeDados" << endl;
	}
}

// Redimensiona a matriz para as dimensões informadas
template< typename T >
bool TCMatriz3D<T>::Redimensiona(int NX, int NY, int NZ) {
	if( nx != NX || ny != NY || nz != NZ ) {
		TCMatriz3D::DesalocaMatriz3D (data3D, nx, ny, nz);
		nx = NX;
		ny = NY;
		nz = NZ;
		data3D = NULL;
		TCMatriz3D::AlocaMatriz3D (nx, ny, nz);
		if( ! data3D)
			return false;
	}
	return true;
}

// Preenche a matriz com um valor constante
template< typename T >
void TCMatriz3D<T>::Constante (T cte) {
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k)
	for (i = 0; i < nx; i++)
		for (j = 0; j < ny; j++)
			for (k = 0; k < nz; k++)
				data3D[i][j][k] = cte;
}

// Inverte valores da matriz binária
template< typename T >
void TCMatriz3D<T>::Inverter () {
	if ( formatoImagem == D1_X_Y_Z_ASCII || formatoImagem == D4_X_Y_Z_BINARY ){
		int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
		for (i = 0; i < nx; i++) {
			for (j = 0; j < ny; j++) {
				for (k = 0; k < nz; k++) {
					// se garantir que o tipo do dado é bool, pode utilizar data3D[i][j][k].flip()
					if (data3D[i][j][k] == 0)
						data3D[i][j][k] = 1;
					else
						data3D[i][j][k] = 0;
				}
			}
		}
	}
}

// Calcula a media dos valores armazenados na matriz
template< typename T >
double TCMatriz3D<T>::Media () const {
	double media = 0.0;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) reduction(+:media) schedule(static,10)
	for (i = 0; i < nx; i++)
		for (j = 0; j < ny; j++)
			for (k = 0; k < nz; k++)
				media += data3D[i][j][k];
	return media /= (nx * ny * nz);
}

// Retorna o maior valor encontrado na matriz
template< typename T >
int TCMatriz3D<T>::MaiorValor () const {
	int maior = data3D[0][0][0];
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			for (int k = 0; k < nz; k++)
				if (data3D[i][j][k] > maior)	// se o valor de data2D for maior
					maior = data3D[i][j][k];	// fazer maior=data2D
	return maior;
}

//Retorna o menor valor encontrado na matriz
template< typename T >
int TCMatriz3D<T>::MenorValor () const {
	int menor = data3D[0][0][0];
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			for (int k = 0; k < nz; k++)
				if (data3D[i][j][k] < menor)
					menor = data3D[i][j][k];
	return menor;
}

// Retorna o menor valor, diferente de zero, encontrado na matriz
template< typename T >
int TCMatriz3D<T>::MenorValorNzero () const {
	int menor = 999999999;
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			for (int k = 0; k < nz; k++)
				if (data3D[i][j][k] < menor && data3D[i][j][k] != 0)
					menor = data3D[i][j][k];
	return menor;
}

// Retorna o maior valor, diferente de zero, encontrado na matriz
template< typename T >
pair<T, T> TCMatriz3D<T>::MaiorMenorValorNzero() const {
	//int menor = 9999999999;
	//int maior = data2D[0][0];
	pair<T,T> maiorMenor;
	maiorMenor.first = data3D[0][0][0];
	maiorMenor.second = 999999999;
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			for (int k = 0; k < nz; k++) {
				if (data3D[i][j][k] < maiorMenor.second && data3D[i][j][k] != 0)
					maiorMenor.second = data3D[i][j][k];
				if (data3D[i][j][k] > maiorMenor.first)
					maiorMenor.first = data3D[i][j][k];
			}
	return maiorMenor;
}

// Troca todos os valores i por j no vetor, retorna o numero de elementos trocados
template< typename T >
int TCMatriz3D<T>::Replace (T i, T j) {
	int contador = 0;
	int k,l,m;
#pragma omp parallel for collapse(3) default(shared) private(k,l,m) reduction(+:contador) schedule(static,10)
	for (k = 0; k < nx; k++)	// Pesquisa toda a matriz a procura de i
		for (l = 0; l < ny; l++)
			for (m = 0; m < nz; m++)
				if (data3D[k][l][m] == i) {	// se existe algum valor i
					data3D[k][l][m] = j;	// trocar por j
					contador++;		// acumula o numero de trocas realizadas
				}
	return contador;		// retorna o numero de trocas realizadas
}

// Seta o formatoImagem da matriz
template< typename T >
void TCMatriz3D<T>::SetFormato(EImageType _formato){
	if(formatoImagem != _formato){
		switch(_formato){
			case D1_X_Y_Z_ASCII:
			case D2_X_Y_Z_GRAY_ASCII:
			case D3_X_Y_Z_COLOR_ASCII:
			case D4_X_Y_Z_BINARY:
			case D5_X_Y_Z_GRAY_BINARY:
			case D6_X_Y_Z_COLOR_BINARY:
				formatoImagem = _formato;
				break;
			default:
				cerr << "Foi informado um formato inválido em TCMatriz3D<T>::SetFormato()" << endl;
				break;
		}
	}
}
