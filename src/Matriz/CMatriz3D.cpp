/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
						Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
	[LDSC].
@author:          Andre Duarte Bueno
File:             CMatriz3D.cpp
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
using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Matriz/CMatriz3D.h>

/*
-------------------------------------------------------------------------
Funcao: Aloca
-------------------------------------------------------------------------
@short  :
 Aloca cria nx ponteiros ** e armazena em data3D
 ou seja data3D é um ponteiro ***, que aponta para um vetor de ponteiros **
 Depois aloca um plano de vetores *, e finalmente aloca uma matriz 3D de inteiros
 Com data3D[i][j][k] acessa inteiro
 Com data3D[i][j]    acessa plano de ponteiros *
 Com data3D [i]      acessa ponteiros **
 Com data3D				acessa ponteiro ***
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
int *** CMatriz3D::AlocaMatriz3D (int nx, int ny, int nz) {
	int i, j;			//
	int ***dat = NULL;		// Cria ponteiro*** nulo
	dat = new int **[nx];		// Passo 1: aloca eixo x.
	if (dat) {			// se alocou dat corretamente
		for (i = 0; i < nx; i++)	// Zera todos os ponteiros dat[i]
			dat[i] = NULL;		// porque se a alocacao der errado vai chamar desaloca
		for (i = 0; i < nx; i++) {			//
			dat[i] = new int *[ny];	// Passo 2: aloca linhas y
			if (dat[i]) {		// se alocou corretamente
				for (j = 0; j < ny; j++)	// Zera todos os ponteiros dat[i][j]
					dat[i][j] = NULL;	// porque se a alocacao der errado vai chamar desaloca
				for (j = 0; j < ny; j++) {
					dat[i][j] = new int[nz];	// STEP 3: aloca matriz de dados 3D.
					if (dat[i][j] == NULL) {
						CMatriz3D::DesalocaMatriz3D (dat, nx, ny, nz);
						return 0;
					}
				}
			} else {
				CMatriz3D::DesalocaMatriz3D (dat, nx, ny, nz);
				return 0;
			}
		}
		return dat;
	} else {				// se nao alocou corretamente dat=0
		nx = ny = nz = 0;		// ou o usuario verifica dat ou faz nx=ny=0
		return 0;			// informa retornando 0
	}
}

/*
-------------------------------------------------------------------------
Funcao:  DesalocaMatriz3D
-------------------------------------------------------------------------
@short  : Chama funcao que le a matriz do disco.
	As informacoes das dimensoes da matriz sao os tres primeiros elementos do arquivo de disco
@author : Andre Duarte Bueno
@see    :

@param  :
@return :
*/
bool CMatriz3D::DesalocaMatriz3D (int ***dat, int nx, int ny, int nz)
{
	if (dat != NULL)
	{
		int i, j;
		for (i = 0; i < nx; i++)
			if (dat[i] != NULL)
				for (j = 0; j < ny; j++)	//
					if (dat[i][j] != NULL)
						delete [] dat[i][j];	// Passo 1: apaga planos z
		// else return;
		for (i = 0; i < nx; i++)	// Passo 2: apaga linhas y
			if (dat[i] != NULL)
				delete [] dat[i];
		delete [] dat;		// Passo 3: apaga eixo x
		nx = ny = nz = 0;
		dat = NULL;
		return 0;			// aqui dat
	}
	return 0;
}

/*
==================================================================================
Documentacao  Construtor (vazio) CMatriz3D
==================================================================================
Descrição: Faz data3D apontar para NULL, e valores nx = ny = nz = 0;
*/
CMatriz3D::CMatriz3D ()
{
	nx = ny = nz = 0;
	data3D = NULL;
	formatoSalvamento = D1_X_Y_Z_ASCII;
	numCores = 65535;
}

CMatriz3D::CMatriz3D (string fileName)
{
	nx = ny = nz = 0;
	data3D = NULL;
	CMatriz3D::Read (fileName);
	size_t pos = fileName.rfind("/");
	if (pos!=string::npos)
		path = fileName.substr(0, pos+1);
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  : Cria copia deve copiar nx,ny,nz, depois alocar a matriz e entao copiar membro a membro
@author : Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CMatriz3D::CMatriz3D (CMatriz3D & matriz)
{
	formatoSalvamento = matriz.formatoSalvamento;
	nx = matriz.nx;
	ny = matriz.ny;
	nz = matriz.nz;
	numCores = matriz.numCores;
	path = matriz.path;

	data3D = CMatriz3D::AlocaMatriz3D (nx, ny, nz);

	if (data3D)
		for (int i = 0; i < nx; i++)
			for (int j = 0; j < ny; j++)
				for (int k = 0; k < nz; k++)
					data3D[i][j][k] = matriz.data3D[i][j][k];
}
/*
CMatriz3D::CMatriz3D(CMatriz3D* matriz)		// : CMatriz2D()
{
 formatoSalvamento = matriz->formatoSalvamento;
 nx = matriz->nx; 			   // Define dimensoes
 ny = matriz->ny;
 nz = matriz->nz;

 data3D = CMatriz3D::AlocaMatriz3D(nx, ny,nz);// Aloca data3D

 if(data3D)
 for (  int i = 0; i < nx; i++)  // Copia membro a membro
 for (  int j = 0; j < ny; j++)
	for (  int k = 0; k < nz; k++)
	 data3D[i][j][k] = matriz->data3D[i][j][k];

}
*/

/*
-------------------------------------------------------------------------
Funcao: Construtor
-------------------------------------------------------------------------
@short  : Copia nx,ny,nz, aloca a matriz e preenche a mesma com zeros
@author : Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CMatriz3D::CMatriz3D (int NX, int NY, int NZ)
{
	nx = NX;			// define valores
	ny = NY;			// em aloca garante que sejam positivos
	nz = NZ;
	formatoSalvamento = D1_X_Y_Z_ASCII;
	numCores = 65535;
	data3D = CMatriz3D::AlocaMatriz3D (nx, ny, nz);	// aloca data3D
}

/*
-------------------------------------------------------------------------
Funcao:  operator+
-------------------------------------------------------------------------
@short  :
@author :Andre Duarte Bueno
@see    :
@param  :

@return :
*/
CMatriz3D & CMatriz3D::operator+ (CMatriz3D & m2)
{
	int
			minx = std::min (this->nx, m2.nx);
	int
			miny = std::min (this->ny, m2.ny);
	int
			minz = std::min (this->nz, m2.nz);
	// ja aloca data3D
	// deve somar membro a membro
	for (int i = 0; i < minx; i++)
		for (int j = 0; j < miny; j++)
			for (int k = 0; k < minz; k++)
				this->data3D[i][j][k] += m2.data3D[i][j][k];
	return *this;
}
/*
CMatriz3D* CMatriz3D::operator+(CMatriz3D* m2)
{
	int minx = std::min(this->nx,m2->nx);
	int miny = std::min(this->ny,m2->ny);
	int minz = std::min(this->nz,m2->nz);
	// ja aloca data3D
	// deve somar membro a membro
	for (  int i = 0; i < minx; i++)
	for (  int j = 0; j < miny; j++)
	for (  int k = 0; k < minz; k++)
	this->data3D[i][j][k] += m2->data3D[i][j][k];
return this;
}
*/

/*
-------------------------------------------------------------------------
Funcao:  operator-
-------------------------------------------------------------------------
@short  :sobrecarga operador -
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CMatriz3D & CMatriz3D::operator- (CMatriz3D & m2)
{
	int
			minx = std::min (this->nx, m2.nx);
	int
			miny = std::min (this->ny, m2.ny);
	int
			minz = std::min (this->nz, m2.nz);

	for (int i = 0; i < minx; i++)
		for (int j = 0; j < miny; j++)
			for (int k = 0; k < minz; k++)
				this->data3D[i][j][k] -= m2.data3D[i][j][k];	// subtrae membro a membro
	return *this;
}

/*
-------------------------------------------------------------------------
Funcao: operator=
-------------------------------------------------------------------------
@short  :sobrecarga operador =
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CMatriz3D & CMatriz3D::operator= (CMatriz3D & m2)
{
	int
			minx = std::min (this->nx, m2.nx);
	int
			miny = std::min (this->ny, m2.ny);
	int
			minz = std::min (this->nz, m2.nz);
	// deve igualar membro a membro
	for (int i = 0; i < minx; i++)
		for (int j = 0; j < miny; j++)
			for (int k = 0; k < minz; k++)
				this->data3D[i][j][k] = m2.data3D[i][j][k];

	return *this;
}
/*
CMatriz3D* CMatriz3D::operator=(CMatriz3D* m2)
{
	int minx = std::min(this->nx,m2->nx);
	int miny = std::min(this->ny,m2->ny);
	int minz = std::min(this->nz,m2->nz);
							// deve igualar membro a membro
 for (  int i = 0; i < minx; i++)
	for (  int j = 0; j < miny; j++)
	 for (  int k = 0; k < minz; k++)
		this->data3D[i][j][k] = m2->data3D[i][j][k];

return this;
}
*/

/*
-------------------------------------------------------------------------
Funcao: operator==
-------------------------------------------------------------------------
@short  :
Todos os valores de this e pm2 devem ser iguais,
caso contrario retorna 0 (false)
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
bool CMatriz3D::operator== (CMatriz3D & pmatriz)
{
	int
			minx = std::min (this->nx, pmatriz.nx);
	int
			miny = std::min (this->ny, pmatriz.ny);
	int
			minz = std::min (this->nz, pmatriz.nz);
	for (int i = 0; i < minx; i++)
		for (int j = 0; j < miny; j++)
			for (int k = 0; k < minz; k++)
				if (this->data3D[i][j][k] != pmatriz.data3D[i][i][k])	// se houver algum diferente
					return 0;		// retorna false
	return 1;			// senao retorna true
}
/*
bool CMatriz3D::operator==(CMatriz3D* pmatriz)
{
	int minx = std::min(this->nx,pmatriz->nx);
	int miny = std::min(this->ny,pmatriz->ny);
	int minz = std::min(this->nz,pmatriz->nz);
	for (  int i = 0; i < minx; i++)
	for (  int j = 0; j < miny; j++)
	for (  int k = 0; k < minz; k++)
	if(this->data3D[i][j][k] != pmatriz->data3D[i][i][k])	// se houver algum diferente
	return 0;						// retorna false
 return 1;                    				// senao retorna true
}
*/

/*
-------------------------------------------------------------------------
Funcao: operator!=
-------------------------------------------------------------------------
@short  :operator!=
Todos os valores de this e pm2 devem ser diferentes,
caso contrario retorna 0 (false)
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
bool CMatriz3D::operator!= (CMatriz3D & pmatriz)
{
	// abaixo compara o endereco dos ponteiros, e nao seu conteudo
	// return ! (this==pmatriz);

	return !(CMatriz3D::operator == (pmatriz));
}

/*
-------------------------------------------------------------------------
Funcao: operator<<
-------------------------------------------------------------------------
@short  :Deve sair com os dados da mesma forma que write
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
ostream & operator<< (ostream & os, const CMatriz3D & pm)
{
	/* for (int k = 0; k < pm.NZ(); k++)
 {
	os << "\n- y! --------------------------------------------------------------------";
	for (int j = pm.NY()-1; j >=0 ; j--)
	 {
	os << "\n| ";
	 os.width(pm.larguraCampo);
	 os << j<<" |";
	for (int i = 0; i < pm.NX(); i++)
	{
	os.width(pm.larguraCampo);
	os << pm.data3D[i][j][k]<<" ";// ' ';
	}
	// os<<'\n';
	}
	os <<"\n|   y |------------------------------ x-> --------------------------------------\n      |";
	for (int i = 0; i < pm.NX(); i++)
	{
	os.width(pm.larguraCampo);
	os << i<<" ";
	}
	os << "\n-------------------------------------------------------------------------------";
	os << "\nz=" << k;
 }
 return os;
 */
	/*  for (  int k = 0; k < pm->NZ(); k++)
 {
	os << "\n- y! --------------------------------------------------------------------";
	for (  int j = pm->NY()-1; j >=0 ; j--)
	 {
	os << "\n| ";
	 os.width(pm->larguraCampo);
	 os << j<<" |";
	for (  int i = 0; i < pm->NX(); i++)
	{
	os.width(pm->larguraCampo);
	os << pm->data3D[i][j][k]<<" ";// ' ';
	}
	// os<<'\n';
	}
	os <<"\n|   y |------------------------------ x-> --------------------------------------\n      |";
	for (  int i = 0; i < pm->NX(); i++)
	{
	os.width(pm->larguraCampo);
	os << i<<" ";
	}
	os << "\n-------------------------------------------------------------------------------";
	os << "\nz=" << k;
 }
 return os;
*/
	for (int k = 0; k < pm.NZ (); k++)
	{
		os <<
					"\n- y! --------------------------------------------------------------------";
		for (int j = pm.NY () - 1; j >= 0; j--)
		{
			os << "\n| ";
			os.width (pm.larguraCampo);
			os << j << " |";
			for (int i = 0; i < pm.NX (); i++)
			{
				os.width (pm.larguraCampo);
				os << pm.data3D[i][j][k] << " ";	// ' ';
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
		os << "\nz=" << k;
	}
	return os;

}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  : Salva dados do cabecalho
@author : Andre Duarte Bueno, modificada por: Leandro Puerari
@see    :
@param  :
@return :
*/
void CMatriz3D::SalvaCabecalho (ofstream & fout) const
{
	if (fout) { // testa abertura do arquivo
		switch ( formatoSalvamento ) {
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
				break;
		} // Valor de nCores é salva em CBaseMatriz, depois de chamar SalvaCabecalho.
	}
}

// Salva dados no formato binario
void CMatriz3D::SalvaDadosBinarios (ofstream & fout) const {
	int x, bit;
	unsigned char c = 0;
	if (fout) {
		switch(formatoSalvamento){
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
			case D5_X_Y_Z_GRAY_BINARY: // 8 bits por pixel = 1 Byte
				for (int k = 0; k < nz; k++) {
					for (int j = 0; j < ny; j++) {
						for (int i = 0; i < nx; i++) {
							fout << (unsigned char) data3D[i][j][k];
						}
					}
				}
				break;
			case D6_X_Y_Z_COLOR_BINARY: // 8 bits red + 8 bits green + 8 bits blue por pixel = 3 Bytes
				cerr << "Formato de arquivo D6_X_Y_Z_COLOR_BINARY não implementado em CMatriz3D::SalvaDadosBinarios" << endl;
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
			default: cerr << "Formato de arquivo inválido em CMatriz3D::SalvaDadosBinarios" << endl;
		}
	}
}

// Salva dados "colados" sem espaço (ex.: 00110011110111101010) ou em formato binário
void CMatriz3D::SalvaDadosColados (ofstream & fout) const {
	switch(formatoSalvamento){
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
			SalvaDadosBinarios(fout);
			break;
		default: cerr << "Formato de arquivo inválido em CMatriz3D::SalvaDados" << endl;
	}
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :Salva dados com um espaco " "
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CMatriz3D::SalvaDados (ofstream & fout) const {
	switch(formatoSalvamento){
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
				fout << '\n';
			}
			break;
		case D4_X_Y_Z_BINARY:
		case D5_X_Y_Z_GRAY_BINARY:
		case D6_X_Y_Z_COLOR_BINARY:
			SalvaDadosBinarios(fout);
			break;
		default: cerr << "Formato de arquivo inválido em CMatriz3D::SalvaDados" << endl;
	}
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :Abre arquivo de disco e le os dados.
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
bool CMatriz3D::Read (string fileName, int separado) {
	ifstream fin;									// Ponteiro para arquivo de disco
	CBaseMatriz::AbreArquivo (fin, fileName);			// Abre o arquivo de disco no formato correto
	//fin.open (fileName.c_str ());		// Abre o arquivo de disco no formato ascii
	if (fin.good ()) { 								// Se o arquivo foi corretamente aberto
		formatoSalvamento = CBaseMatriz::VerificaFormato(fin); // Obtem o formato de salvamento
		//pega os valore de nx, ny e nz ignorando os comentários
		CBaseMatriz::LeComentarios(fin);
		fin >> nx;
		CBaseMatriz::LeComentarios(fin);
		fin >> ny;
		CBaseMatriz::LeComentarios(fin);
		fin >> nz;
		CBaseMatriz::LeComentarios(fin);
		switch (formatoSalvamento)	{	// Em funcao do formato de salvamento lê os dados referente ao número de cores/tons de cinza
			case D2_X_Y_Z_GRAY_ASCII:
			case D3_X_Y_Z_COLOR_ASCII:
			case D5_X_Y_Z_GRAY_BINARY:
			case D6_X_Y_Z_COLOR_BINARY:
				CBaseMatriz::LeComentarios(fin);
				fin >> numCores;					//pega o número de cores do arquivo.
				CBaseMatriz::LeComentarios(fin);
				break;
			case INVALID_IMAGE_TYPE:
				cerr << "Formato de arquivo inválido em  CMatriz3D::Read" << endl;
				return false;
		}
		if ( data3D = AlocaMatriz3D (nx, ny, nz) ) {	// Aloca a matriz de dados
			if (separado != 0)							// Leitura dos dados da matriz
				CMatriz3D::LeDados (fin);				// Lê os dados separados
			else
				CMatriz3D::LeDadosColados (fin);			// Lê os dados colados
			fin.close();
			return true;
		}
		fin.close();
		return false;
	}
	else
		return false;
}

CMatriz2D* CMatriz3D::LePlano (unsigned int planoZ, E_eixo direcao)
{
	if ( ! data3D )
		return NULL;

	CMatriz2D *pm2D = NULL;
	pm2D = new CMatriz2D (nx, ny);	// Aloca a matriz de dados

	if ( ! pm2D )
		return NULL;
	if ( LePlano( pm2D, planoZ, direcao) )
		return pm2D;
	else
		return NULL;
}

bool CMatriz3D::LePlano (CMatriz2D * pm2D, unsigned int plano, E_eixo direcao)
{
	if ( ! data3D || ! pm2D)
		return false;
	int i, j;
	switch(direcao){
		case EIXO_X:
		{	//cerr << "aqui é x" << endl;
			if (plano > nx - 1)
				plano = nx - 1;
			pm2D->Redimensiona(nz, ny);							// gira a visão
			//pm2D->Redimensiona(ny, nz);							// gira o eixo
			for (j = 0; j < nz; j++)
				for (i = 0; i < ny; i++)
					pm2D->data2D[nz-j-1][i] = data3D[plano][i][j]; 	// gira a visão
			//pm2D->data2D[i][j] = data3D[plano][i][j];		// gira o eixo
			break;
		}
		case EIXO_Y:
		{	//cerr << "aqui é y" << endl;
			if (plano > ny - 1)
				plano = ny- 1;
			pm2D->Redimensiona(nx, nz);
			for (j = 0; j < nz; j++)
				for (i = 0; i < nx; i++)
					pm2D->data2D[i][nz-j-1] = data3D[i][plano][j];	// gira a visão
			//pm2D->data2D[i][j] = data3D[i][plano][j];		// gira o eixo
			break;
		} // caso direção seja == z ou Z ou k ou K
		case EIXO_Z:
		{	//cerr << "aqui é z" << endl;
			if (plano > nz - 1)
				plano = nz - 1;
			pm2D->Redimensiona(nx, ny);
			for (j = 0; j < ny; j++)
				for (i = 0; i < nx; i++)
					pm2D->data2D[i][j] = data3D[i][j][plano];
			break;
		} // caso direção informada seja incorreta o default é z
		default:
		{	//cerr << "aqui é default z" << endl;
			if (plano > nz - 1)
				plano = nz - 1;
			pm2D->Redimensiona(nx, ny);
			for (j = 0; j < ny; j++)
				for (i = 0; i < nx; i++)
					pm2D->data2D[i][j] = data3D[i][j][plano];		// o eixo está na mesma direção da visão
		}
	}
	return true;
}

bool CMatriz3D::Rotacionar90 (E_eixo axis){
	CMatriz3D * pmtmp = NULL;
	pmtmp = new CMatriz3D( *this );
	if( ! pmtmp ) return false;
	int _nx = nx; // precisa pegar os valores das dimensões, pois se a imagem não for um cubo perfeito, seus valores serão alterados.
	int _ny = ny;
	int _nz = nz;
	switch (axis){
		case EIXO_X:
			if (ny != nz){
				if ( ! Redimensiona(nx, nz, ny) ) return false ;
			}
			for (int i = 0; i < _nx; i++){
				for (int j = 0; j < _ny; j++){
					for (int k = 0; k < _nz; k++){
						data3D[i][k][nz-1-j] = pmtmp->data3D[i][j][k];
					}
				}
			}
			break;
		case EIXO_Y:
			if (nx != nz){
				if ( ! Redimensiona(nz, ny, nx) ) return false ;
			}
			for (int j = 0; j < _ny; j++){
				for (int i = 0; i < _nx; i++){
					for (int k = 0; k < _nz; k++){
						data3D[k][j][nz-1-i] = pmtmp->data3D[i][j][k];
					}
				}
			}
			break;
		case EIXO_Z:
			if (nx != ny){
				if ( ! Redimensiona(ny, nx, nz) ) return false ;
			}
			for (int k = 0; k < _nz; k++){
				for (int j = 0; j < _ny; j++){
					for (int i = 0; i < _nx; i++){
						data3D[nx-1-j][i][k] = pmtmp->data3D[i][j][k];
					}
				}
			}
			break;
		default: return false;
	}
	delete pmtmp;
	return true;
}

void CMatriz3D::LeDados (ifstream & fin) {
	switch(formatoSalvamento){
		case D1_X_Y_Z_ASCII:
		case D2_X_Y_Z_GRAY_ASCII:
			for (int k = 0; k < nz; k++) {
				for (int j = 0; j < ny; j++) {
					for (int i = 0; i < nx; i++) {
						if (!fin.eof ()) {	// Se NAO chegou ao fim do arquivo entra
							fin >> data3D[i][j][k];
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
			CMatriz3D::LeDadosBinarios(fin);
			break;
		default: cerr << "Formato de arquivo inválido em CMatriz3D::LeDados" << endl;
	}
}

// Lê os dados de um arquivo de disco, Os dados estao "colados"
// Ex: 00011101000101
void CMatriz3D::LeDadosColados (ifstream & fin) {
	char ch = 0;
	char matrizChar[30] = " ";
	switch(formatoSalvamento) {
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
			CMatriz3D::LeDadosBinarios(fin);
			break;
		default: cerr << "Formato de arquivo inválido em CMatriz3D::LeDadosColados" << endl;
	}
}

// Lê os dados de um arquivo de disco
// Os dados estao separados por um " "
void CMatriz3D::LeDadosBinarios (ifstream & fin) {
	char c;
	unsigned char c2;
	int x, bit;
	switch(formatoSalvamento){
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
			cerr << "Formato de arquivo D6_X_Y_Z_COLOR_BINARY não implementado em CMatriz3D::LeDadosBinarios" << endl;
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
		default: cerr << "Formato de arquivo inválido em CMatriz3D::LeDados" << endl;
	}
}

/*
-------------------------------------------------------------------------


Funcao: Constante
-------------------------------------------------------------------------
@short  :Preenche a matriz com um valor constante
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CMatriz3D::Constante (int cte)
{
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			for (int k = 0; k < nz; k++)
				data3D[i][j][k] = cte;
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
void CMatriz3D::Inverter ()
{
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			for (int k = 0; k < nz; k++)
				if (data3D[i][j][k] == 0)
					data3D[i][j][k] = 1;
				else
					data3D[i][j][k] = 0;
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  : Calcula a media
@author : Andre Duarte Bueno
@see    :
@param  :
@return :
*/
double CMatriz3D::Media () const
{
	double media = 0.0;
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			for (int k = 0; k < nz; k++)
				media += data3D[i][j][k];
	return media /= (nx * ny * nz);
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
MaiorValor retorna o maior valor da matriz
@author : Andre Duarte Bueno
@see    :
@param  :
@return :
*/
int CMatriz3D::MaiorValor () const
{
	int maior = data3D[0][0][0];
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			for (int k = 0; k < nz; k++)
				if (data3D[i][j][k] > maior)	// se o valor de data2D for maior
					maior = data3D[i][j][k];	// fazer maior=data2D
	return maior;
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
MenorValor retorna o menor valor da matriz
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
int CMatriz3D::MenorValor () const
{
	int menor = data3D[0][0][0];
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			for (int k = 0; k < nz; k++)
				if (data3D[i][j][k] < menor)
					menor = data3D[i][j][k];
	return menor;
}
/*
-------------------------------------------------------------------------
Funcao:   MenorValorNzero
-------------------------------------------------------------------------
@short  :MenorValorNzero retorna o menor valor da matriz diferente de zero
@author :Leandro Puerari
*/
int CMatriz3D::MenorValorNzero () const
{
	int menor = 9999999;
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			for (int k = 0; k < nz; k++)
				if (data3D[i][j][k] < menor && data3D[i][j][k] != 0)
					menor = data3D[i][j][k];
	return menor;
}
/*
-------------------------------------------------------------------------
Funcao:   MaiorMenorValorNzero
-------------------------------------------------------------------------
@short  :MaiorMenorValorNzero retorna um par correspondente ao maior e menor valor (respectivamente) da matriz diferente de zero
@author :Leandro Puerari
*/
pair<int,int> CMatriz3D::MaiorMenorValorNzero() const
{
	//int menor = 9999999999;
	//int maior = data2D[0][0];
	pair<int,int> maiorMenor;
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
/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :A funcao Replace, troca todos os valores i no vetor por j
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
int CMatriz3D::Replace (int i, int j)
{
	int contador = 0;
	for (int k = 0; k < nx; k++)	// Pesquisa toda a matriz a procura de i
		for (int l = 0; l < ny; l++)
			for (int m = 0; m < nz; m++)
				if (data3D[k][l][m] == i)	// se existe algum valor i
				{
					data3D[k][l][m] = j;	// trocar por j
					contador++;		// acumula o numero de trocas realizadas
				}
	return contador;		// retorna o numero de trocas realizadas
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :Retorna para os as propriedades da matriz
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CMatriz3D::Propriedades (ofstream & os) const
{
	CBaseMatriz::Propriedades (os);
	os << "\nDimensoes: nx=" << nx << " ny=" << ny << " nz=" << nz;
}
