/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
									Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
			[LDSC].
@author:          Andre Duarte Bueno
@file:             CVetor.cpp
@begin:            Sat Sep 16 2000
copyright:        (C) 2000 by Andre Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Matriz/CVetor.h>

/*
-------------------------------------------------------------------------
Funcao:  AlocaVetor
-------------------------------------------------------------------------
@short  :
Aloca um conjunto de numeros inteiros e armazena
endereco do primeiro elemento em data1D.
data1D é um ponteiro para um array de numeros inteiros.
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
int * CVetor::AlocaVetor (int nx)
{
	// TESTE PARA EXCESSOES
	//       try
	//       {

	// STEP 1: aloca eixo x.
	int *dat = new int[nx];
	if (dat != nullptr)
		return dat;
	else
	{
		nx = 0;
		return 0;
	}
	//       }
	//       catch (xalloc)
	//               {
	//               return 0;// exit(-1);
	//               cerr << "Nao conseguiu alocar o Vetor. Tchau ...";
	//               }
	// return 1;
}

/*
-------------------------------------------------------------------------
Funcao:   DesalocaVetor
-------------------------------------------------------------------------
@short  :
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
bool CVetor::DesalocaVetor (int *dat, int nx)
{
	if (dat != nullptr)
	{
		// Passo 1: apaga eixo x
		delete [] dat;
		dat = nullptr;
		// opcional, evita uso do vetor sem verificacao
		nx = 0;
		return 1;
	}
	return 0;
}

/*
-------------------------------------------------------------------------
Funcao:  Construtor vazio
-------------------------------------------------------------------------
@short  :
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CVetor::CVetor ()
{
	nx = 0;
	data1D = nullptr;
}

/*
-------------------------------------------------------------------------
Funcao:  Construtor
-------------------------------------------------------------------------
@short  :Constroe objeto a partir de arquivo de disco
Chama classe base, já chama a funcao que le o Vetor do disco.
A informação da dimensao  é o	primeiro elemento do arquivo de disco.
Se dim=1 vetor, se dim=2 matriz, se dim=3 matriz 3D.
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CVetor::CVetor (string nomeArquivo)
{
	nx = 0;
	data1D = nullptr;
	// A funcao Read aloca o vetor e armazena dados do arquivo de disco no vetor
	CVetor::Read (nomeArquivo);
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
Cria copia deve copiar nx, depois alocar a
Vetor e entao copiar membro a membro
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CVetor::CVetor (CVetor & v)
{
	// Define dimensoes
	nx = v.nx;
	// aloca data1D
	data1D = AlocaVetor (nx);
	// copia membro a membro
	int i;
#pragma omp parallel for default(shared) private(i) // reduction(+:variavel) schedule(static,10)
	for (i = 0; i < nx; i++)
		this->data1D[i] = v.data1D[i];
}

/*
-------------------------------------------------------------------------
Funcao: Construtor
-------------------------------------------------------------------------
@short  :
Copia nx, aloca o Vetor e preenche com zeros

@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CVetor::CVetor (int NX)
{
	this->nx = NX;			// define valores
	data1D = AlocaVetor (nx);	// aloca data1D
}

/*
-------------------------------------------------------------------------
Funcao:  operator+
-------------------------------------------------------------------------
@short  :
Copia nx, aloca o Vetor e preenche com zeros
Sobrecarga operador +
Obs: da forma como esta cria um objeto dinamico
e retorna ponteiro para o programa que chamou, deve
garantir que o objeto aqui criado seja deletado
pelo programa que chamou???
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CVetor & CVetor::operator+ (CVetor & v)
{
	int minx = this->nx > v.nx ? v.nx : this->nx;
	int i;
#pragma omp parallel for default(shared) private(i) // reduction(+:variavel) schedule(static,10)
	for (i = 0; i < minx; i++)
		this->data1D[i] += v.data1D[i]; // Soma membro a membro
	return *this;
}
/*
CVetor* CVetor::operator+(CVetor* v)
{
	 int min = std::min(this->nx, v->nx);

 for (  int i = 0; i < min; i++)
		 // Soma membro a membro
		 this->data1D[i]+= v->data1D[i];
 return this;
}
*/

/*
-------------------------------------------------------------------------
Funcao:  operator-
-------------------------------------------------------------------------
@short  :
GetMaiorValor retorna o maior valor da matriz
GetMenorValor retorna o menor valor da matriz
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CVetor & CVetor::operator- (CVetor & v) {
	//int minx = min( this->nx ,  v.nx);
	int minx = this->nx > v.nx ? v.nx : this->nx;
	int i;
#pragma omp parallel for default(shared) private(i) // reduction(+:variavel) schedule(static,10)
	for (i = 0; i < minx; i++)
		this->data1D[i] -= v.data1D[i]; // subtrai membro a membro
	return *this;
}
/*
CVetor* CVetor::operator-(CVetor* v)
{
	 int minx = menor(this->nx, v->nx);

 for (  int i = 0; i < minx; i++)
		// subtrai membro a membro
		this->data1D[i]-= v->data1D[i];

 return this;
}
*/

/*
-------------------------------------------------------------------------
Funcao: operator=
-------------------------------------------------------------------------
@short  :
Observe que retorna this, isto e importante pois pode-se ter
casos como x=y=z;
Excessões:        No caso dos vetores terem dimensoes diferentes.
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
CVetor & CVetor::operator= (CVetor & v) {
	//int minx = min(this->nx, v.nx);
	int minx = this->nx > v.nx ? v.nx : this->nx;
	int i;
#pragma omp parallel for default(shared) private(i) // reduction(+:variavel) schedule(static,10)
	for (i = 0; i < minx; i++)
		this->data1D[i] = v.data1D[i];
	return *this;
}
/*
CVetor* CVetor::operator=(CVetor* v)
{
	 int min = std::min(this->nx, v->nx);
	for (  int i = 0; i < min; i++)
		this->data1D[i] = v->data1D[i];
 return this;
}
*/

/*
-------------------------------------------------------------------------
Funcao: operator==
-------------------------------------------------------------------------
@short  :
Obs: Para considerar igual considerar somente os dados, desconsidera
o atributo formatoImagem@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
/*
bool CVetor::operator==(CVetor* v)const
{
// if(this->nx!=v->nx)			// se as dimensoes forem diferentes, retorna  0
//    return 0;                // se incluir esta linha, tem de mudar operador !=

		int minx = std::min(this->nx, v->nx);

	for (  int i = 0; i < minx; i++)
		if(this->data1D[i] != v->data1D[i])
							 return 0;
return 1;
}
*/
bool CVetor::operator== (CVetor & v) const {
	//if(this->nx!=v->nx)      // se as dimensoes forem diferentes, retorna  0
	//return 0;                // se incluir esta linha, tem de mudar operador !=
	//int minx = min(this->nx, v.nx);
	int minx = this->nx > v.nx ? v.nx : this->nx;
	for (int i = 0; i < minx; i++)
		if (this->data1D[i] != v.data1D[i])
			return 0;
	return 1;
}
/*
-------------------------------------------------------------------------
Funcao:  operator!=
-------------------------------------------------------------------------
@short  :
Obs: Para considerar igual considerar somente os dados, desconsidera
o atributo formatoImagem.
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
bool CVetor::operator!= (CVetor & v) const {
	return !(CVetor::operator== (v));
}
/*
bool CVetor::operator!=(CVetor* v)const
{
 return !(CVetor::operator==(v));
}
*/

/*
-------------------------------------------------------------------------
Funcao: operator<<
-------------------------------------------------------------------------
@short  :
Despacha os valores do vetor para a ostream
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
ostream & operator<< (ostream & os, CVetor & v) {
	for (int i = 0; i < v.NX (); i++)
		os << v.data1D[i] << endl;
	return os;
}

/*
-------------------------------------------------------------------------
Funcao: SalvaCabecalho
-------------------------------------------------------------------------
@short  :
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CVetor::SalvaCabecalho (ofstream & fout) const
{
	if (fout.good ()) // Testa abertura do arquivo
	{ 	// fout << setw (0) << 'V' << left << formatoImagem << '\n' << nx;
		switch (formatoImagem){
			case 1:	fout << setw (0) << "V1" << '\n' << nx;
				break;
			case 2:	fout << setw (0) << "V2" << '\n' << nx;
				break;
			case 3:	fout << setw (0) << "V3" << '\n' << nx;
				break;
			case 4:	fout << setw (0) << "V4" << '\n' << nx;
				break;
			case 5:	fout << setw (0) << "V5" << '\n' << nx;
				break;
			case 6:	fout << setw (0) << "V6" << '\n' << nx;
				break;
			default: fout << setw (0) << "V2" << '\n' << nx;
				break;
		}
	}
}

// Salva dados no formato binario
void CVetor::SalvaDadosBinarios (ofstream & fout) const {
	int x, bit;
	unsigned char c = 0;
	if (fout) {
		switch(formatoImagem){
			case V4_X_BINARY: // 1 bite por pixel
				for (int i = 0; i < nx; i++) {
					x = 7 - i%8;
					bit = (data1D[i])%2;
					c = c | (bit << x);
					if ( (i+1)%8 == 0 || i == (nx-1) ) {
						//fout.write( &c, 1 );
						fout << c;
						c = 0;
					}
				}
				break;
			case V5_X_GRAY_BINARY: // 8 bits por pixel = 1 Byte
				for (int i = 0; i < nx; i++) {
					c = data1D[i];
					fout << c;
				}
				break;
			case V6_X_COLOR_BINARY: // 8 bits red + 8 bits green + 8 bits blue por pixel = 3 Bytes
				cerr << "Formato de arquivo V6_X_COLOR_BINARY não implementado em TCMatriz2D< int >::SalvaDadosBinarios" << endl;
				/* falta implementar vetores para as cores RGB
				for (int i = 0; i < nx; i++) {
					fout << (unsigned char) data1Dr[i];
					fout << (unsigned char) data1Dg[i];
					fout << (unsigned char) data1Db[i];
				}
				*/
				break;
			default: cerr << "Formato de arquivo inválido em TCMatriz2D< int >::SalvaDadosBinarios" << endl;
		}
	}
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
Salva dados "colados" sem espaço
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CVetor::SalvaDadosColados (ofstream & fout) const
{
	for (int i = 0; i < nx; i++)
		fout << data1D[i];
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
Salva dados com um espaco " "
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CVetor::SalvaDados (ofstream & fout) const
{
	for (int i = 0; i < nx; i++)
	{
		// fout.width(larguraCampo);
		fout << data1D[i] << '\n';
	}
}

/*
-------------------------------------------------------------------------
Funcao: Constante
-------------------------------------------------------------------------
@short  :
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CVetor::Constante (int cte) {
	int i;
#pragma omp parallel for default(shared) private(i) // reduction(+:variavel) schedule(static,10)
	for (i = 0; i < nx; i++)
		data1D[i] = cte;
/*int *p = data1D;
	for (int i = 0; i < nx; i++) {
		*p = cte;			//  PERFORMANCE USAR MEMCPY
		p++;
	}
*/
}

/*
-------------------------------------------------------------------------
Funcao: Inverter
-------------------------------------------------------------------------
@short  :
Deixar as funcoes abaixo apenas enquanto nao mudar no Anaimp o uso
de tmatriz por timagem
// pouco sentido, eliminar daqui
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CVetor::Inverter () {
	int i;
#pragma omp parallel for default(shared) private(i) // reduction(+:variavel) schedule(static,10)
	for (i = 0; i < nx; i++) {
		if (data1D[i] == 0) {
			data1D[i] = 1;
		} else {
			data1D[i] = 0;
		}
	}
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :

retorna o maior valor do vetor
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
int CVetor::MaiorValor () const {
	int maximo = data1D[0];
	for (int i = 1; i < nx; i++)
		if (data1D[i] > maximo)
			maximo = data1D[i];
	return maximo;
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
retorna o menor valor do vetor
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
int CVetor::MenorValor () const {
	int minimo = data1D[0];
	for (int i = 1; i < nx; i++)
		if (data1D[i] < minimo)
			minimo = data1D[i];
	return minimo;
}
/*
-------------------------------------------------------------------------
Funcao:   MenorValorNzero
-------------------------------------------------------------------------
@short  :MenorValorNzero retorna o menor valor da matriz diferente de zero
@author :Leandro Puerari
*/
int CVetor::MenorValorNzero () const {
	int menor = 9999999;
	for (int i = 0; i < nx; i++)
		if (data1D[i] < menor && data1D[i] != 0)
			menor = data1D[i];
	return menor;
}
/*
-------------------------------------------------------------------------
Funcao:   MaiorMenorValorNzero
-------------------------------------------------------------------------
@short  :MaiorMenorValorNzero retorna um par correspondente ao maior e menor valor (respectivamente) da matriz diferente de zero
@author :Leandro Puerari
*/
pair<int,int> CVetor::MaiorMenorValorNzero() const {
	pair<int,int> maiorMenor;
	maiorMenor.first = data1D[0];
	maiorMenor.second = 999999999;
	for (int i = 0; i < nx; i++){
		if (data1D[i] < maiorMenor.second && data1D[i] != 0)
			maiorMenor.second = data1D[i];
		if (data1D[i] > maiorMenor.first)
			maiorMenor.first = data1D[i];
	}
	return maiorMenor;
}
/*
-------------------------------------------------------------------------
Funcao:  Media
-------------------------------------------------------------------------
@short  :calcula a media
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
double
CVetor::Media () const {
	double media = 0.0;
	int i;
#pragma omp parallel for default(shared) private(i) reduction(+:media) schedule(static,10)
	for (i = 1; i < nx; i++)
		media += data1D[i];
	return media /= nx;
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  :
A funcao Replace, troca todos os valores i no vetor por j
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
int CVetor::Replace (int i, int j) {
	int contador = 0;
	int k;
#pragma omp parallel for default(shared) private(k) reduction(+:contador) schedule(static,10)
	// Pesquisa todo o vetor a procura de i se existe algum valor i
	for (k = 0; k < nx; k++) {
		if (data1D[k] == i) {
			data1D[k] = j; // Trocar por j
			contador++; // acumula o numero de trocas realizadas
		}
	}
	return contador; // Retorna o numero de trocas realizadas
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  : Abre arquivo de disco e lê os dados.
@author : Andre Duarte Bueno
@see    :
@param  :
@return :
*/
bool CVetor::Read (string nomeArquivo, bool separado) {
	ifstream fin;									// Ponteiro para arquivo de disco
	CBaseMatriz::AbreArquivo (fin, nomeArquivo); 			// Abre o arquivo de disco no formato correto
	int pos;										// posição de leitura do arquivo.
	char aux;										// auxiliar.
	char linha[256];
	if (fin.good ()) 								// Se o arquivo foi corretamente aberto
	{											// Obtem o formato de salvamento
		formatoImagem = CBaseMatriz::VerificaFormato(fin);
		switch (formatoImagem)					// Em funcao do formato de salvamento lê os dados do cabecalho
		{
			case 1:
			case 4:
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
				break;
			case 2:
			case 3:
			case 5:
			case 6:
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
				fin >> numCores;					// pega o número de cores do arquivo.
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
			default:
				return 0;
		}
		data1D = AlocaVetor (nx);					// Aloca o vetor de dados
		if (separado)								// Leitura dos dados da matriz
			CVetor::LeDados (fin);					// Lê os dados separados
		else
			CVetor::LeDadosColados (fin);				// Lê os dados colados
		return 1;
	}
	else
		return 0;
}

/*
-------------------------------------------------------------------------
Funcao:   LeDados
-------------------------------------------------------------------------
@short  :Le os dados separados por " "
@author :Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CVetor::LeDados (ifstream & fin)
{
	for (int i = 0; i < nx; i++)
		if (!fin.eof ())		// se NAO chegou ao fim do arquivo,
			fin >> data1D[i];		// lê dado do arquivo e armazena em data1D.
		else
			data1D[i] = 0;		// Senão, preenche com zeros
}

/*
-------------------------------------------------------------------------

Funcao: LeDadosColados
-------------------------------------------------------------------------
@short  : Le os dados, colados 010111001
@author : Andre Duarte Bueno
@see    :
@param  :
@return :
*/
void CVetor::LeDadosColados (ifstream & fin) {
	char ch;
	char matrizChar[30] = " ";
	for (int i = 0; i < nx; i++) {
		if (!fin.eof ()) {
			cin.get (ch); 				// Pega o caracter
			if (ch >= 48 && ch <= 57) {	// se for um número válido 48->0 57->9
				matrizChar[0] = ch;		// copia para string
				data1D[i] = atoi (matrizChar); // e da string para o inteiro
			} else { // se for um \n ou ' ' desconsidera, e retorna contador ????
				i--;
			}
		} else { // se chegou ao fim do arquivo, preenche com zeros
			data1D[i] = 0;
		}
	}
}

void CVetor::SetFormato(EImageType _formato){
	if(formatoImagem != _formato){
		switch(_formato){
			case V1_X_ASCII:
			case V2_X_GRAY_ASCII:
			case V3_X_COLOR_ASCII:
			case V4_X_BINARY:
			case V5_X_GRAY_BINARY:
			case V6_X_COLOR_BINARY:
				formatoImagem=_formato;
				break;
			default:
				cerr << "Foi informato um formato inválido em CVetor::SetFormato()" << endl;
				break;
		}
	}
}
/*
ptr
(*ptr) = (*ptr) + (*ptr);
obj
obj = obj + obj;
*/
