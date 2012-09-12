/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
									Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:
Laboratorio de Desenvolvimento de Software Cientifico 	[LDSC].
@author:          Andre Duarte Bueno
File:             CMatriz2D.cpp
begin:            Sat Sep 16 2000
copyright:        (C) 2000 by Andre Duarte Bueno
email:            andre@lmpt.ufsc.br
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
#include <Matriz/CMatriz2D.h>

/**
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
 data2D é um ponteiro **, que aponta para um vetor de ponteiros *
 e finalmente aloca uma matriz 2D de inteiros
 Com data2D[i][j] acessa inteiro
 Com data2D[i]    acessa vetor de ponteiros *
 Com data2D       acessa ponteiro **

Pode ser usada externamente, retorna ponteiro 2D
Exemplo de uso externo: int** data=CMatriz2D::Aloca(50,50); if(data)...

PS:
No futuro, alocar um vetor grandão [nx*ny] e definir
os vetores data[i] calculando a posicao correta
Desta forma a matriz vai funcionar tanto com o metodo linear como no metodo de
ponteiro para ponteiro
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
int **CMatriz2D::AlocaMatriz2D (int nx, int ny) {
	int i;			//
	int **dat = NULL;		// Cria ponteiro nulo
	dat = new int *[nx];		// Passo 1: aloca eixo x
	if (dat) {			// se alocou dat corretamente
		for (i = 0; i < nx; i++)	// Zera todos os ponteiros dat[i]
			dat[i] = NULL;		// porque se a alocacao der errado vai chamar desaloca
		for (i = 0; i < nx; i++) {
			dat[i] = new int[ny];	// Passo 2: aloca linhas y
			if (dat[i] == NULL) {	// Se a linha nao foi corretamente alocada
				CMatriz2D::DesalocaMatriz2D (dat, nx, ny); // Para evitar vazamento de memoria
				return 0;
			}
			// Desaloca toda a matriz ja alocada dat=null,nx=ny=0 e retorna.
		}	// O que nao foi alocado esta com NULL e pode ser deletado
		return dat;
	} else { // se nao alocou corretamente dat=0
		nx = ny = 0;		// ou verifica data2D ou faz nx=ny=0
		return 0;			// informa retornando 0
	}
}

/**
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :desaloca a matriz
Exemplo uso externo:
int** ptr=CMatriz2D::Aloca(nx,ny);.....;CMatriz2D::Desaloca(ptr,nx);
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
bool CMatriz2D::DesalocaMatriz2D (int **&dat, int nx, int ny)
{
	if (dat != NULL)
	{
		for (int i = 0; i < nx; i++)
			if ( dat[i] )
				delete [] dat[i];	//  Passo 1: apaga linhas y
		delete [] dat;				//  Passo 2: apaga eixo x
		dat = NULL;
		nx = ny = 0;				// opcional, evita uso indevido de data2D
		return 1;
	}
	return 0;
}


/**
// -----------------------------------------------------------------------
// Funcao: Read2D
// -----------------------------------------------------------------------
// @short  : Lê uma matriz bidimensional do disco
// A matriz é  bidimensional mas o acesso é como em um vetor
// #ifndef Real_E1(x,y)
// @author : Andre Duarte Bueno
// @see    :
// @param  : O nome do arquivo, um vetor de float's e as dimensões nx e ny da matriz
// @return : true/false
*/
#define Real_E1(x,y)	_reData [(y)*_nx + (x)]

//bool CMatriz2D::Read2D (std::string inputFile, float * _reData, int _nx, int _ny);
bool CMatriz2D::Read2D (std::string inputFile, float* & _reData, int _nx, int _ny)
{
	// Abre arquivo disco
	ifstream fin (inputFile.c_str ());
	if (fin.fail ())
		return 0;
	// Lê imagem do disco
	int aux;
	for (int cy = 0; cy < (_ny); cy++)
		for (int cx = 0; cx < (_nx); cx++)
		{
			fin >> aux;
			// Real_E1(cx,cy) = aux;
			_reData[(cy) * _nx + (cx)] = aux;
		}
	fin.close ();
	return 1;
}

/**
// -----------------------------------------------------------------------
// Funcao:
// -----------------------------------------------------------------------
// @short  : Salva a matriz bidimensional em disco
// @author  :Andre Duarte Bueno
// @see    : TMatriz
// @param  :     nome do arquivo, ponteiro para matriz e dimensões nx e ny
// @return : true/false
*/
bool CMatriz2D::Write2D (std::string inputFile, float * _reData, int _nx, int _ny) {
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


/**
-------------------------------------------------------------------------
Funcao: Construtor
-------------------------------------------------------------------------
@short  : constroe objeto vazio
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CMatriz2D::CMatriz2D ()
{
	nx = 0;
	ny = 0;
	data2D = NULL;
	formatoSalvamento = P2_X_Y_GRAY_ASCII;
	numCores = 65535;
}

/**
-------------------------------------------------------------------------
Funcao: construtor
-------------------------------------------------------------------------
@short  : Chama funcao que le a matriz do disco.
 As informacoes das dimensoes da matriz sao os tres primeiros elementos do arquivo de disco.
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CMatriz2D::CMatriz2D (string fileName)
{
	nx = ny = 0;
	data2D = NULL;
	CMatriz2D::Read(fileName);
	size_t pos = fileName.rfind("/");
	if (pos!=string::npos)
		path = fileName.substr(0, pos+1);
}

/**
-------------------------------------------------------------------------
Funcao: construtor
-------------------------------------------------------------------------
@short  : recebe o nome do arquivo de disco e o número do plano a ser lido
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CMatriz2D::CMatriz2D (string fileName, int planoZ)
{
	nx = ny = 0;
	data2D = NULL;
	CMatriz2D::LePlanoZ (fileName, planoZ);
	size_t pos = fileName.rfind("/");
	if (pos!=string::npos)
		path = fileName.substr(0, pos+1);
}

/**
-------------------------------------------------------------------------
Funcao: Construtor novo
-------------------------------------------------------------------------
@short  : Cria copia com borda extendida, a imagem fica centralizada.
Deve alocar nx+borda,ny+borda depois copiar a matriz para a regiao central
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CMatriz2D::CMatriz2D (CMatriz2D & matriz, unsigned int borda)
{
	nx = matriz.nx + borda;			// Define dimensoes
	ny = matriz.ny + borda;
	data2D = AlocaMatriz2D (nx, ny);	// Aloca data2D
	path = matriz.path;

	Constante(0); // Zera a imagem criada // posteriormente otimizar, zerando somente as bordas
	for (int i = borda; i < nx - borda; i++)	// Copia região central
		for (int j = borda; j < ny - borda; j++)
			this->data2D[i][j] = matriz.data2D[i][j];
}

/**
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  : Cria copia deve copiar nx,ny depois alocar a matriz e entao copiar membro a membro
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CMatriz2D::CMatriz2D (CMatriz2D & matriz)
{
	nx = matriz.nx;			// Define dimensoes
	ny = matriz.ny;
	data2D = AlocaMatriz2D (nx, ny);	// Aloca data2D
	path = matriz.path;
	for (int i = 0; i < nx; i++)	// Copia membro a membro
		for (int j = 0; j < ny; j++)
			this->data2D[i][j] = matriz.data2D[i][j];
}
/*CMatriz2D::CMatriz2D(CMatriz2D* matriz)
{
 nx = matriz->nx;				// Define dimensoes
 ny = matriz->ny;
 data2D = AlocaMatriz2D(nx,ny);   	// Aloca data2D

 for (  int i = 0; i < nx; i++)  	// Copia membro a membro
 for (  int j = 0; j < ny; j++)
		this->data2D[i][j] = matriz->data2D[i][j];
}
*/
/**
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  : Copia nx,ny, aloca a matriz e preenche a mesma com zeros
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CMatriz2D::CMatriz2D (int NX, int NY)
{
	nx = NX;			// define valores
	ny = NY;
	formatoSalvamento = P2_X_Y_GRAY_ASCII;
	numCores = 65535;
	data2D = AlocaMatriz2D (nx, ny);	// aloca data2D
}

/**
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CMatriz2D & CMatriz2D::operator+ (CMatriz2D & matriz)
{
	int minx = std::min (this->nx, matriz.nx);
	int miny = std::min (this->ny, matriz.ny);
	for (int i = 0; i < minx; i++)
		for (int j = 0; j < miny; j++)
			this->data2D[i][j] += matriz.data2D[i][j];
	return *this;
}

/*
CMatriz2D* CMatriz2D::operator+(CMatriz2D* matriz)
{
	 int minx = std::min(this->nx,matriz->nx);
	 int miny = std::min(this->ny,matriz->ny);

 for (  int i = 0; i < minx; i++)
	for (  int j = 0; j < miny; j++)
		 this->data2D[i][j] +=  matriz->data2D[i][j];
 return this;
}
*/

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
/*
CMatriz2D* CMatriz2D::operator-(CMatriz2D* matriz)
{
	 int minx = std::min(this->nx,matriz->nx);
	 int miny = std::min(this->ny,matriz->ny);

 for (  int i = 0; i < minx; i++)
		for (  int j = 0; j < miny; j++)
				 this->data2D[i][j] -= matriz->data2D[i][j];

 return this;
}
*/
CMatriz2D & CMatriz2D::operator- (CMatriz2D & matriz)
{
	int minx = std::min (this->nx, matriz.nx);
	int miny = std::min (this->ny, matriz.ny);
	for (int i = 0; i < minx; i++)
		for (int j = 0; j < miny; j++)
			this->data2D[i][j] -= matriz.data2D[i][j];
	return *this;
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
/*
CMatriz2D* CMatriz2D::operator=(CMatriz2D* matriz)
{
	 int minx = std::min(this->nx,matriz->nx);
	 int miny = std::min(this->ny,matriz->ny);

 for (  int i = 0; i < minx; i++)
	 for (  int j = 0; j < miny; j++)
		 this->data2D[i][j] = matriz->data2D[i][j];     // deve igualar membro a membro

 return this;
}
*/
CMatriz2D & CMatriz2D::operator= (CMatriz2D & matriz)
{
	int minx = std::min (this->nx, matriz.nx);
	int miny = std::min (this->ny, matriz.ny);
	for (int i = 0; i < minx; i++)
		for (int j = 0; j < miny; j++)
			this->data2D[i][j] = matriz.data2D[i][j];	// deve igualar membro a membro
	return *this;
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
 Todos os valores de this e pmatriz devem ser iguais,
 caso contrario retorna 0 (false)
*/
/*
bool CMatriz2D::operator==(CMatriz2D* pmatriz)
{
	 int minx = std::min(this->nx,pmatriz->nx);
	 int miny = std::min(this->ny,pmatriz->ny);
 for (  int i = 0; i < minx; i++)// percorre as matrizes
	for (  int j = 0; j < miny; j++)
	if(this->data2D[i][j] != pmatriz->data2D[i][i])// se houver algum diferente
	 return 0;// retorna false
return 1;// senao retorna true
}
*/
bool CMatriz2D::operator== (CMatriz2D & pmatriz)
{
	int minx = std::min (this->nx, pmatriz.nx);
	int miny = std::min (this->ny, pmatriz.ny);
	for (int i = 0; i < minx; i++)	// percorre as matrizes
		for (int j = 0; j < miny; j++)
			if (this->data2D[i][j] != pmatriz.data2D[i][i])	// se houver algum diferente
				return 0;		// retorna false
	return 1;			// senao retorna true
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
 Todos os valores de this e pmatriz devem ser diferentes,
 caso contrario retorna 0 (false)
*/
bool CMatriz2D::operator!= (CMatriz2D & pmatriz)
{
	return !(CMatriz2D::operator== (pmatriz));
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
@author :Andre Duarte Bueno
@see    :
@param  :
@return :

==================================================================================
Documentacao operator<<
==================================================================================
// Funcao Friend
Deve sair os dados da mesma forma que Write
Deve sair os dados da mesma forma que o CVetor , e CMatriz3D
*/
ostream & operator<< (ostream & os, const CMatriz2D & pm)
{
	/*
			os<<"\n- y! --------------------------------------------------------------------";
			for (int j = pm.NY()-1; j >=0 ; j--)
				{
			 os<<"\n| ";
				os.width(pm.larguraCampo);
				os << j <<" |";

			 for (int i = 0; i < pm.NX(); i++)
			 {
			 os.width(pm.larguraCampo);      // só tem sentido para saida formatada
			 os << pm.data2D[i][j] << " ";// ' ';
			 }
			 // os<<'\n';
			 }
			os<<"\n|   y |------------------------------ x-> --------------------------------------\n      |";
			for (int i = 0; i < pm.NX(); i++)
			{
			os.width(pm.larguraCampo);
			os<<i<<" ";
			}
		 os << "\n-------------------------------------------------------------------------------";
		 return os;
		*/
	/*
			os<<"\n- y! --------------------------------------------------------------------";
			for (int j = pm->NY()-1; j >=0 ; j--)
				{
			 os<<"\n| ";
				os.width(pm->larguraCampo);
				os << j <<" |";
			 for (  int i = 0; i < pm->NX(); i++)
			 {
			 os.width(pm->larguraCampo);      // só tem sentido para saida formatada
			 os << pm->data2D[i][j] << " ";// ' ';
			 }
			 // os<<'\n';
			 }
			os<<"\n|   y |------------------------------ x-> --------------------------------------\n      |";
			for (  int i = 0; i < pm->NX(); i++)
			{
			os.width(pm->larguraCampo);
			os<<i<<" ";
			}
		 os << "\n-------------------------------------------------------------------------------";
		 return os;
		*/
	os <<
				"\n- y! --------------------------------------------------------------------";
	for (int j = pm.NY () - 1; j >= 0; j--)
	{
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
	os <<
				"\n|   y |------------------------------ x-> --------------------------------------\n      |";
	for (int i = 0; i < pm.NX (); i++)
	{
		os.width (pm.larguraCampo);
		os << i << " ";
	}
	os <<
				"\n-------------------------------------------------------------------------------";
	return os;
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :Salva dados do cabecalho
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CMatriz2D::SalvaCabecalho (ofstream & fout) const {
	if (fout) {	// testa abertura do arquivo
		//fout << setw (0) << 'P' << left << formatoSalvamento << '\n' << nx << ' ' << ny;
		switch (formatoSalvamento) {
			case 7:
				fout << setw (0) << "P1" << '\n' << nx << ' ' << ny;
				break;
			case 8:
				fout << setw (0) << "P2"  << '\n' << nx << ' ' << ny;
				break;
			case 9:
				fout << setw (0) << "P3" << '\n' << nx << ' ' << ny;
				break;
			case 10:
				fout << setw (0) << "P4" << '\n' << nx << ' ' << ny;
				break;
			case 11:
				fout << setw (0) << "P5" << '\n' << nx << ' ' << ny;
				break;
			case 12:
				fout << setw (0) << "P6" << '\n' << nx << ' ' << ny;
				break;
			default:
				fout << setw (0) << "P1" << '\n' << nx << ' ' << ny;
				break;
		}
		// Valor de nCores é salva em CBaseMatriz, depois de chamar SalvaCabecalho.
	}
}

// Salva dados no formato binario
void CMatriz2D::SalvaDadosBinarios (ofstream & fout) const {
	int x, bit;
	unsigned char c = 0;
	if (fout) {
		switch(formatoSalvamento){
			case P4_X_Y_BINARY: // 1 bite por pixel
				for (int j = 0; j < ny; j++) {
					for (int i = 0; i < nx; i++) {
						x = 7 - i%8;
						bit = (data2D[i][j])%2;
						c = c | (bit << x);
						if ( (i+1)%8 == 0 || i == (nx-1) ) {
							//fout.write( &c, 1 );
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
				cerr << "Formato de arquivo P6_X_Y_COLOR_BINARY não implementado em CMatriz2D::SalvaDadosBinarios" << endl;
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
			default: cerr << "Formato de arquivo inválido em CMatriz2D::SalvaDadosBinarios" << endl;
		}
	}
}

// Salva dados "colados" sem espaço (ex.: 00110011110111101010)
void CMatriz2D::SalvaDadosColados (ofstream & fout) const {
	switch(formatoSalvamento){
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
			SalvaDadosBinarios(fout);
			break;
		case P5_X_Y_GRAY_BINARY:
		case P6_X_Y_COLOR_BINARY:
			SalvaDadosBinarios(fout);
			break;
		default: cerr << "Formato de arquivo inválido em CMatriz2D::SalvaDadosColados" << endl;
	}
}

// Salva dados com um espaco (ex.: 0 0 1 1 0 0 1 1 1 1 0 1 1 1 1 0 1 0 1 0)
void CMatriz2D::SalvaDados (ofstream & fout) const {
	switch(formatoSalvamento){
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
			SalvaDadosBinarios(fout);
			break;
		case P5_X_Y_GRAY_BINARY:
		case P6_X_Y_COLOR_BINARY:
			SalvaDadosBinarios(fout);
			break;
		default: cerr << "Formato de arquivo inválido em CMatriz2D::SalvaDados" << endl;
	}
}


/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :Abre arquivo de disco e lê os dados.
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
bool CMatriz2D::Read (string fileName, int separado) {
	ifstream fin; // Ponteiro para arquivo de disco.
	CBaseMatriz::AbreArquivo (fin, fileName); // Abre o arquivo de disco no formato correto.
	//fin.open(fileName.c_str());
	int pos;			// posição de leitura do arquivo.
	char aux;			// auxiliar.
	char linha[256];
	//	string str;
	if (fin.good ()) { // Se o arquivo foi corretamente aberto
		formatoSalvamento = CBaseMatriz::VerificaFormato(fin); // Obtem o formato de salvamento
		//pega os valore de nx e ny ignorando os comentários
		do {
			pos = fin.tellg();				//guarda a posição de leitura no arquivo.
			fin >> skipws >> aux;			//pega o primeiro caracter ignorando possíveis espaços
			if(aux == '#'){
				fin.seekg(pos, ios::beg);	//reposiciona a leitura
				fin.getline(linha, 256);	  	//vai para a próxima linha
			}else{
				fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
			}
		} while(aux == '#'); 				// enquanto encontrar comentário, fica no loop.
		fin >> nx;
		do {
			pos = fin.tellg();				//guarda a posição de leitura no arquivo.
			fin >> skipws >> aux;			//pega o primeiro caracter ignorando possíveis espaços
			if(aux == '#'){
				fin.seekg(pos, ios::beg);	//reposiciona a leitura
				fin.getline(linha, 256);	  	//vai para a próxima linha
			}else{
				fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
			}
		} while(aux == '#'); 				// enquanto encontrar comentário, fica no loop.
		fin >> ny;
		do {
			pos = fin.tellg();				//guarda a posição de leitura no arquivo.
			fin >> skipws >> aux;			//pega o primeiro caracter ignorando possíveis espaços
			if(aux == '#'){
				fin.seekg(pos, ios::beg);	//reposiciona a leitura
				fin.getline(linha, 256);	  	//vai para a próxima linha
			}else{
				fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
			}
		} while(aux == '#'); 				// enquanto encontrar comentário, fica no loop.

		switch (formatoSalvamento)	{	// Em funcao do formato de salvamento lê os dados referente ao número de cores/tons de cinza
			case P2_X_Y_GRAY_ASCII:
			case P3_X_Y_COLOR_ASCII:
			case P5_X_Y_GRAY_BINARY:
			case P6_X_Y_COLOR_BINARY:
				do {
					pos = fin.tellg();				//guarda a posição de leitura no arquivo.
					fin >> skipws >> aux;			//pega o primeiro caracter ignorando possíveis espaços
					if(aux == '#'){
						fin.seekg(pos, ios::beg);	//reposiciona a leitura
						fin.getline(linha, 256);	  	//vai para a próxima linha
					}else{
						fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
					}
				} while(aux == '#'); 				//enquanto encontrar comentário, fica no loop.
				fin >> numCores;					//pega o número de cores do arquivo.
				do {
					pos = fin.tellg();				//guarda a posição de leitura no arquivo.
					fin >> skipws >> aux;			//pega o primeiro caracter ignorando possíveis espaços
					if(aux == '#'){
						fin.seekg(pos, ios::beg);	//reposiciona a leitura
						fin.getline(linha, 256);	  	//vai para a próxima linha
					}else{
						fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
					}
				} while(aux == '#'); 				// enquanto encontrar comentário, fica no loop.
				break;
			case INVALID_IMAGE_TYPE:
				return false;
		}/*
		switch (formatoSalvamento)	{	// Verifica a necessidade de reabrir o arquivo em formato binário
			case P4_X_Y_BINARY:
			case P5_X_Y_GRAY_BINARY:
			case P6_X_Y_COLOR_BINARY:
				fin.close();
				fin.open (fileName.c_str (), ios::binary);		// Abre o arquivo de disco no formato ascii
				if ( fin.good ())
					fin.seekg(pos, ios::beg); //reposiciona o ponteiro de leitura
				else
					return false;
			case INVALID_IMAGE_TYPE:
				return false;
		}*/
		data2D = AlocaMatriz2D (nx, ny);			// Aloca a matriz de dados
		if (separado != 0)							// Leitura dos dados da matriz
			CMatriz2D::LeDados (fin);				// Lê os dados separados
		else
			CMatriz2D::LeDadosColados (fin);			// Lê os dados colados
		return true;
	}
	else
		return false;
}

// Lê os dados de um arquivo de disco
// Os dados estao separados por um " "
void CMatriz2D::LeDados (ifstream & fin) {
	switch(formatoSalvamento){
		case P1_X_Y_ASCII:
		case P2_X_Y_GRAY_ASCII:
			cerr << "Entrou em CMatriz2D::LeDados como P1_X_Y_ASCII" << endl;
			for (int j = 0; j < ny; j++) {
				for (int i = 0; i < nx; i++) {
					if (!fin.eof ()) {	// Se NAO chegou ao fim do arquivo entra
						fin >> data2D[i][j];
					} else {
						data2D[i][j] = 0;	// preenche com zeros
					}
				}
			}
			break;
		case P4_X_Y_BINARY:
		case P5_X_Y_GRAY_BINARY:
		case P6_X_Y_COLOR_BINARY:
			CMatriz2D::LeDadosBinarios(fin);
			break;
		default: cerr << "Formato de arquivo inválido em CMatriz2D::LeDados" << endl;
	}
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
Lê os dados de um arquivo de disco, Os dados estao "colados"
Ex: 00011101000101
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CMatriz2D::LeDadosColados (ifstream & fin) {
	char ch = 0;
	char matrizChar[30] = " ";
	switch(formatoSalvamento) {
		case P1_X_Y_ASCII:
			cerr << "Entrou em CMatriz2D::LeDadosColados como P1_X_Y_ASCII" << endl;
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
			CMatriz2D::LeDadosBinarios(fin);
			break;
		default: cerr << "Formato de arquivo inválido em CMatriz2D::LeDadosColados" << endl;
	}
}

// Lê os dados de um arquivo de disco
// Os dados estao separados por um " "
void CMatriz2D::LeDadosBinarios (ifstream & fin) {
	char c;
	unsigned char c2;
	int x, bit;
	switch(formatoSalvamento){
		case P4_X_Y_BINARY: // 1 bit por pixel
			cerr << "Entrou em CMatriz2D::LeDadosBinarios como P4_X_Y_BINARY" << endl;
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
			cerr << "Formato de arquivo P6_X_Y_COLOR_BINARY não implementado em CMatriz2D::LeDadosBinarios" << endl;
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
		default: cerr << "Formato de arquivo inválido em CMatriz2D::LeDados" << endl;
	}
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
Le um plano de uma matriz tridimensional.
O objetivo é ter acesso a uma matriz 3D usando uma matriz2D.
Muitas vesez preciso acessar um plano de uma matriz 3D.
Posso abrir a matriz 3D e acessar o plano, mas toda a matriz 3D fica
na memoria, e se a matriz 3D for muito grande pode nao ser possivel.
Portanto em alguns casos é melhor só acessar o plano.
Obs: O valor de planoZ deve iniciar de 0.
O primeiro plano na direcao z é o plano 0.
Se nz=1, uma plano na direcao z, plano 0.
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
bool CMatriz2D::LePlanoZ (string fileName, int planoZ, bool separado) {
	int nz;
	ifstream fin;							// Ponteiro para arquivo de disco
	CBaseMatriz::AbreArquivo (fin, fileName);	// Abre o arquivo de disco no formato correto
	int pos;								// posição de leitura do arquivo.
	char aux;								// auxiliar.
	char linha[256];
	if (fin.good ()) {						// Se o arquivo foi corretamente aberto
		// Obtem o formato de salvamento
		formatoSalvamento = CBaseMatriz::VerificaFormato(fin);
		switch (formatoSalvamento)	{				// Em funcao do formato de salvamento lê os dados do cabecalho
			case 13:
			case 16:
				do {
					pos = fin.tellg();				//guarda a posição de leitura no arquivo.
					fin >> skipws >> aux;			//pega o primeiro caracter ignorando possíveis espaços
					if (aux == '#') {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura
						fin.getline(linha, 256);	  	//vai para a próxima linha
					} else {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
					}
				} while (aux == '#'); 				// enquanto encontrar comentário, fica no loop.
				fin >> nx;
				do {
					pos = fin.tellg();				//guarda a posição de leitura no arquivo.
					fin >> skipws >> aux;			//pega o primeiro caracter ignorando possíveis espaços
					if (aux == '#') {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura
						fin.getline(linha, 256);	  	//vai para a próxima linha
					} else {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
					}
				} while (aux == '#'); 				// enquanto encontrar comentário, fica no loop.
				fin >> ny;
				do {
					pos = fin.tellg();				//guarda a posição de leitura no arquivo.
					fin >> skipws >> aux;			//pega o primeiro caracter ignorando possíveis espaços
					if (aux == '#') {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura
						fin.getline(linha, 256);	  	//vai para a próxima linha
					} else {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
					}
				} while (aux == '#'); 				// enquanto encontrar comentário, fica no loop.
				fin >> nz;
				do {
					pos = fin.tellg();				//guarda a posição de leitura no arquivo.
					fin >> skipws >> aux;			//pega o primeiro caracter ignorando possíveis espaços
					if (aux == '#') {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura
						fin.getline(linha, 256);	  	//vai para a próxima linha
					} else {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
					}
				} while (aux == '#'); 				// enquanto encontrar comentário, fica no loop.
				break;
			case 14:
			case 15:
			case 17:
			case 18:
				do {
					pos = fin.tellg();				//guarda a posição de leitura no arquivo.
					fin >> skipws >> aux;			//pega o primeiro caracter ignorando possíveis espaços
					if (aux == '#') {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura
						fin.getline(linha, 256);	  	//vai para a próxima linha
					} else {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
					}
				} while (aux == '#'); 				// enquanto encontrar comentário, fica no loop.
				fin >> nx;
				do {
					pos = fin.tellg();				//guarda a posição de leitura no arquivo.
					fin >> skipws >> aux;			//pega o primeiro caracter ignorando possíveis espaços
					if (aux == '#') {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura
						fin.getline(linha, 256);	  	//vai para a próxima linha
					} else {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
					}
				} while (aux == '#'); 				// enquanto encontrar comentário, fica no loop.
				fin >> ny;
				do {
					pos = fin.tellg();				//guarda a posição de leitura no arquivo.
					fin >> skipws >> aux;			//pega o primeiro caracter ignorando possíveis espaços
					if (aux == '#') {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura
						fin.getline(linha, 256);	  	//vai para a próxima linha
					} else {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
					}
				} while (aux == '#'); 				// enquanto encontrar comentário, fica no loop.
				fin >> nz;
				do {
					pos = fin.tellg();				//guarda a posição de leitura no arquivo.
					fin >> skipws >> aux;			//pega o primeiro caracter ignorando possíveis espaços
					if (aux == '#') {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura
						fin.getline(linha, 256);	  	//vai para a próxima linha
					} else {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
					}
				} while (aux == '#'); 				// enquanto encontrar comentário, fica no loop.
				fin >> numCores;					// pega o número de cores do arquivo.
				do {
					pos = fin.tellg();				//guarda a posição de leitura no arquivo.
					fin >> skipws >> aux;			//pega o primeiro caracter ignorando possíveis espaços
					if (aux == '#') {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura
						fin.getline(linha, 256);	  	//vai para a próxima linha
					} else {
						fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
					}
				} while (aux == '#'); 				// enquanto encontrar comentário, fica no loop.
				break;
			default:
				return 0;
		}
		data2D = AlocaMatriz2D (nx, ny);				// Aloca a matriz de dados
		int i, j, k, temp;
		if (planoZ > nz - 1)						// O plano a ser lido nao pode ser maior que nz-1
			planoZ = nz - 1;						// se nz=5 (0,1,2,3,4) planoZ <= 4
		if (separado) {
			for (k = 0; k < planoZ; k++)				// Aqui posiciona o ponteiro
				for (j = 0; j < ny; j++)				// devo ler os elementos desnecessarios
					for (i = 0; i < nx; i++)			// ate encontrar o plano desejado.
						fin >> temp;				// armazena em temp, depois
			CMatriz2D::LeDados (fin);				// Lê os dados do plano Z selecionado
		} else {
			char ch;
			for (k = 0; k < planoZ; k++)				// Aqui posiciona o ponteiro
				for (j = 0; j < ny; j++)				// devo ler os elementos desnecessarios
					for (i = 0; i < nx; i++) {			// ate encontrar o plano desejado.
						cin.get (ch);
						if (ch == ' ' || ch == '\n')	// Precisa considerar '\n' e ' '
							cin.get (ch);			// testar
					}
			CMatriz2D::LeDadosColados (fin);			// Lê os dados como colados
		}
		return true;								// sucesso
	}
	return false;									// falha
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :Preenche a matriz com um valor constante
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CMatriz2D::Constante (int cte) {
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			data2D[i][j] = cte;
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
Deixar as funcoes abaixo apenas enquanto nao mudar no Anaimp o uso
de tmatriz por timagem
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CMatriz2D::Inverter () {
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			if (data2D[i][j] == 0)
				data2D[i][j] = 1;
			else
				data2D[i][j] = 0;
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :calcula a media
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
double CMatriz2D::Media () const {
	double media = 0.0;
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			media += data2D[i][j];
	return media /= (nx * ny);
}

/*
-------------------------------------------------------------------------
Funcao:   MaiorValor
-------------------------------------------------------------------------
@short  :MaiorValor retorna o maior valor da matriz
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
int CMatriz2D::MaiorValor () const {
	int maior = data2D[0][0];
	for (int i = 0; i < nx; i++)	// percorre a matriz de dados
		for (int j = 0; j < ny; j++)
			if (data2D[i][j] > maior)	// se o valor de data2D for maior
				maior = data2D[i][j];	// fazer maior=data2D
	return maior;
}

/*
-------------------------------------------------------------------------
Funcao:   MenorValor
-------------------------------------------------------------------------
@short  :MenorValor retorna o maior valor da matriz
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
int CMatriz2D::MenorValor () const {
	int menor = data2D[0][0];
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			if (data2D[i][j] < menor)
				menor = data2D[i][j];
	return menor;
}

/*
-------------------------------------------------------------------------
Funcao:   MenorValorNzero
-------------------------------------------------------------------------
@short  :MenorValorNzero retorna o menor valor da matriz diferente de zero
@author :Leandro Puerari
*/
int CMatriz2D::MenorValorNzero () const {
	int menor = 9999999;
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			if (data2D[i][j] < menor && data2D[i][j] != 0)
				menor = data2D[i][j];
	return menor;
}
/*
-------------------------------------------------------------------------
Funcao:   MaiorMenorValorNzero
-------------------------------------------------------------------------
@short  :MaiorMenorValorNzero retorna um par correspondente ao maior e menor valor (respectivamente) da matriz diferente de zero
@author :Leandro Puerari
*/
pair<int,int> CMatriz2D::MaiorMenorValorNzero() const {
	//int menor = 9999999999;
	//int maior = data2D[0][0];
	pair<int,int> maiorMenor;
	maiorMenor.first = data2D[0][0];
	maiorMenor.second = 999999999;
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++) {
			if (data2D[i][j] < maiorMenor.second && data2D[i][j] != 0)
				maiorMenor.second = data2D[i][j];
			if (data2D[i][j] > maiorMenor.first)
				maiorMenor.first = data2D[i][j];
		}
	return maiorMenor;
}

/*
-------------------------------------------------------------------------
Funcao:  Replace
-------------------------------------------------------------------------
@short  :A funcao Replace, troca todos os valores i no vetor por j
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
int CMatriz2D::Replace (int i, int j) {
	int contador = 0;
	for (int k = 0; k < nx; k++)	// Pesquisa toda a matriz a procura de i
		for (int l = 0; l < ny; l++)
			if (data2D[k][l] == i)	// se existe algum valor i
			{
				data2D[k][l] = j;	// trocar por j
				contador++;		// acumula o numero de trocas realizadas
			}
	return contador;		// retorna o numero de trocas realizadas

}

bool CMatriz2D::Rotacionar90 () {
	CMatriz2D * pmtmp = NULL;
	pmtmp = new CMatriz2D( *this );
	if ( ! pmtmp )
		return false;
	int _nx = nx; // precisa pegar os valores das dimensões, pois se a imagem não for um cubo perfeito, seus valores serão alterados.
	int _ny = ny;
	if (nx != ny) {
		if ( ! Redimensiona(ny, nx) )
			return false ;
	}
	for (int i = 0; i < _nx; i++) {
		for (int j = 0; j < _ny; j++) {
			data2D[_ny-1-j][i] = pmtmp->data2D[i][j];
		}
	}
	delete pmtmp;
	return true;
}

/*
-------------------------------------------------------------------------
Funcao:   Propriedades
-------------------------------------------------------------------------
@short  :Retorna para os as propriedades da matriz
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CMatriz2D::Propriedades (ofstream & os) const {
	CBaseMatriz::Propriedades (os);
	os << "\nDimensoes: nx=" << nx << " ny=" << ny << endl;
}
