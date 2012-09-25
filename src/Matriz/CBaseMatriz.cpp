/*
	===============================================================================
	PROJETO:          Biblioteca LIB_LDSC
	Ramo: TPadrao_ramo
	===============================================================================

	Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
	@author:          André Duarte Bueno
	File:             CBaseMatriz.cpp
	begin:            Sat Sep 16 2000
	copyright:        (C) 2000 by André Duarte Bueno
	email:            andreduartebueno@gmail.com
*/

//  -----------------------------------------------------------------------
//  Bibliotecas C/C++
//  -----------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
using namespace std;

//  -----------------------------------------------------------------------
//  Bibliotecas LIB_LDSC
//  -----------------------------------------------------------------------
#include <Matriz/CBaseMatriz.h>

//  -----------------------------------------------------------------------
//  Atributo estatico da classe
//  -----------------------------------------------------------------------
//  Largura do campo de salvamento dos dados em disco (2)
int CBaseMatriz::larguraCampo = 2;	// BUG

// Armazena matriz em disco, recebe o nome do arquivo de disco.
// No arquivo header detalhes dos formatos de salvamento em disco.
bool CBaseMatriz::Write(string fileName, int separado) const {
	ofstream fout; //  Abre arquivo disco
	string fullFileName = path + fileName;
	//fout.open (fullFileName.c_str ()); // Abre arquivo de disco formato ASCII para salvar cabeçalho e cores
	if ( formatoImagem == V4_X_BINARY ||
			 formatoImagem == V5_X_GRAY_BINARY ||
			 formatoImagem == V6_X_COLOR_BINARY ||
			 formatoImagem == P4_X_Y_BINARY ||
			 formatoImagem == P5_X_Y_GRAY_BINARY ||
			 formatoImagem == P6_X_Y_COLOR_BINARY ||
			 formatoImagem == D4_X_Y_Z_BINARY ||
			 formatoImagem == D5_X_Y_Z_GRAY_BINARY ||
			 formatoImagem == D6_X_Y_Z_COLOR_BINARY ) {
		fout.open (fullFileName.c_str (), ios::binary ); // Abre arquivo de disco no formato Binario
	} else {
		fout.open (fullFileName.c_str ()); // Abre arquivo de disco formato ASCII
	}
	if (fout.good ()) { //  Testa abertura do arquivo
		//fout.width (larguraCampo); // Define a largura do campo
		//fout.setf (ios::left);
		//fout.fill (' '); // possivel erro na saida de P 2???

		SalvaCabecalho (fout);	// (virtual) Salva dados do cabecalho:
		SalvaCores(fout);
		fout << '\n';
		if (separado)
			SalvaDados (fout); // (virtual) Salva dados com um espaco " " 1 0 0 1
		else
			SalvaDadosColados (fout); // (virtual) Salva dados "colados" sem espaço 1001
		fout << endl;
		fout.close();
		return true;
	} else
		return false;
}

bool CBaseMatriz::SalvaCores (ofstream & fout) const {
	if (fout) { // testa abertura do arquivo
		if ( formatoImagem > 1 && formatoImagem != 4 && formatoImagem != 7 && formatoImagem != 10 && formatoImagem != 13 && formatoImagem != 16 && formatoImagem < 19 )
			fout << "\n" << numCores;
	}
}

// Le os comentários do arquivo para anvançar o ponteiro de leitura
void CBaseMatriz::LeComentarios(ifstream & fin) {
	int pos;
	char aux;
	char linha[256];
	do {
		fin >> skipws >> aux;				//pega o próximo caracter ignorando possíveis espaços
		if(aux == '#'){
			fin.getline(linha, 256);	//vai para a próxima linha
		}else{
			pos = fin.tellg();					//armazena a posição de leitura após o caracter
			pos--;
			fin.seekg(pos, ios::beg);	//reposiciona a leitura. Aqui aux é diferente de #. Logo, sairá do loop.
		}
	} while(aux == '#');					// enquanto encontrar comentário, fica no loop.
}

/*
	==================================================================================
	Documentacao LeEfetivamenteCabecalho
	==================================================================================
	As classes derivadas chamam esta funcao passando os parametros fin,nx,ny,nz.
	1- Lê o formato
	2- Lê nx,[ny],[nz]
	3- ^Define o formatoImagem e [lê numeroCores]
*/
/*
	void CBaseMatriz::LeEfetivamenteCabecalho(ifstream& fin, unsigned int& nx,
	unsigned int& ny,unsigned int& nz,unsigned int& numeroCores)
	{
	if(fin!=NULL)
	{
	//  Lê o formato e define nx,ny,nz
	char formato[50];
	Desaloca();                    		 //  (virtual)Precisa desalocar pois pode mudar nx,ny, desaloca faz nx=ny=0

	fin>>formato;                      //  Obtem formato do arquivo

	//  Primeiro verifica o caracter de formatacao
	if (formato[0] == 'V' || formato[0] == 'v' )       //  é um vetor
	{fin>>nx;}
	else if (formato[0] == 'P' || formato[0] == 'p' )  //  é uma matriz 2D
	{fin>>nx;	fin>>ny;}
	else if (formato[0] == 'D' || formato[0] == 'd' )  //  é uma matriz 3D
	{fin>>nx;	fin>>ny; fin>>nz;}
	else if (formato[0]>= 48 || formato[0] <=57 )		//  Se for um número, então só são dados
	{                                               //  deve receber nx,ny,nz como parametros.
	formatoImagem=WRITEFORM_ASCII_DATA;       //  Preciso recolocar ponteiro de leitura no inicio do arquivo
	fin.seekg(0,ios::beg);               						//  reposiciona inicio arquivo
	}
	else
	{
	formatoImagem=WRITEFORM_ERROR;
	nx=ny=nz=0;                                   //  zera para evitar acesso a dados errados
	return;
	}
	//  Lê o formato de salvamento
	unsigned int  numeroCores;
	switch (formato[1])
	{
	//  Leitura ASCII
	case '1':       formatoImagem=WRITEFORM_ASCII_PI_N_DATA;
	break;
	case '2':       formatoImagem=WRITEFORM_ASCII_PI_N_256_DATA;
	fin>>numeroCores;
	break;
	case '3':       formatoImagem=WRITEFORM_ASCII_PI_N_MAXCOLOR_DATA;
	fin>>numeroCores; 									//  exceto uchar
	break;
	//  Leitura Binaria
	case '4':       formatoImagem=WRITEFORM_BINARY_PI_N_DATA;
	break;
	case '5':       formatoImagem=WRITEFORM_BINARY_PI_N_256_DATA;
	fin>>numeroCores;
	break;
	case '6':       formatoImagem=WRITEFORM_BINARY_PI_N_MAXCOLOR_DATA;
	fin>>numeroCores;                //  exceto uchar
	break;
	default:        //  ='2'
	formatoImagem=WRITEFORM_ASCII_PI_N_256_DATA;
	fin>>numeroCores;
	break;
	}
	}
	}
*/
/*
	==================================================================================
	Documentacao Read
	==================================================================================
	Descrição:        Le dados da matriz do disco,
	Primeiro lê o cabeçalho, depois aloca a matriz,
	e finalmente lê os dados  armazenados no disco.
	Pré-condições:
	Excessões:        Tipos de excessoes
	Concorrência:
	Tempo processamento(s):fileName
	Tamanho(bits):
	Comentarios:

	Programador:      Andre Duarte Bueno
*/
//  bool Read (const char*& fileName,bool separado=1); //  Le matriz do disco
//  bool CBaseMatriz::Read(const char*& fileName,bool separado=1); //  Le matriz do disco
/*
	bool CBaseMatriz::Read(const char*& fileName,bool separado)
	{
	//  Abertura do arquivo de disco
	ifstream fin;
	//  ->Verifica o formato do arquivo
	fin.open(fileName);   					   				//  Abre arquivo como sendo ASCII

	string aux(50);
	if(fin.good());                           //  Se abriu corretamente o arquivo
	{
	fin>>aux;                          		//  Lê a primeira string ( Pi ou Di ou Vi ou um numero. ->P1,P2,...D1,D5..)
	//  Se nao for um formato conhecido, sai

	//  if(	aux[0]!='V' && aux[0]!='v' &&   //  Se nao é um vetor
	//   		aux[0]!='P' && aux[0]!='p' &&   //  Se nao é uma matriz 2D
	//         aux[0]!='D' && aux[0]!='d' )//  && //  Se nao é uma matriz 3D
	//  formatoImagem != WRITEFORM_ASCII_DATA && //  e se nao sao somente dados
	//  formatoImagem!=WRITEFORM_BINARY_DATA)
	//   return 0;                      //  entao sai

	//  Aqui verifica a correcao, um vetor lendo um vetor, uma matriz2D lendo uma matriz2D,..
	if((GetDimensaoMatriz()==1 && aux[0]!='V') return 0; if (GetDimensaoMatriz()==1 && aux[0]!='v')) return 0;
	if((GetDimensaoMatriz()==2 && aux[0]!='P') return 0; if (GetDimensaoMatriz()==2 && aux[0]!='p')) return 0;
	if((GetDimensaoMatriz()==3 && aux[0]!='D') return 0; if (GetDimensaoMatriz()==3 && aux[0]!='d')) return 0;

	//  se for um V ou P ou D, verifica o segundo índice
	if (aux[1] != '0' && aux[1] != '1' && aux[1] != '2' && aux[1] != '3')
	{                                     //  se i!= 1 e !=2 e !=3, abriu com o formato errado
	fin.close();                         //  Fecha o arquivo e
	fin.open(fileName,ios::binary);				//  Reabre como binario
	}
	else                                    //  Abriu com o formato correto
	//  verificar ifstream                //  Reposiciona ponteiro arquivo, no inicio do arquivo
	fin.seekg(0,ios::beg);              //  vai para posicao 0, a partir do inicio do arquivo
	}
	//  Leitura do cabecalho de dados
	LeCabecalho(fin);//  (virtual)Chama LeEfetivamenteCabecalho(fin,nx,ny,nz,ncores)



	if(fin.good())//  Se o arquivo esta ok
	{
	Aloca();//  (virtual) Aloca a matriz (usa nx,ny,nz lidos do arquivo de disco)
	//  Leitura dos dados da matriz
	if(separado)
	LeDados(fin);//  (virtual) Lê os dados 12 15 21 20 2 5
	else
	LeDadosColados(fin);//  (virtual) Lê os dados colados 0011010101

	return true;//  sucesso

	}
	else
	return 0;//  falhou
	}
*/
/*
//  Aqui verifica a correcao, um vetor lendo um vetor,
if((aux[0]!='V' && aux[0]!='v')
return 0;
*/

/*
	-------------------------------------------------------------------------
	Função:   AbreArquivo
	-------------------------------------------------------------------------
	@short  :
	Esta função abre o arquivo de disco, como sendo normal(ASCII), verifica a primeira string
	se ok posiciona ponteiro get no inicio do arquivo.
	Se o arquivo foi aberto como ASCII e o formato correto é o binário,
	reabre o arquivo como binario
	@author : André Duarte Bueno, modificado por: Leandro Puerari
	@see    :
	@param  :
	@return :
*/

bool CBaseMatriz::AbreArquivo (ifstream & fin, string fileName) { // Abre arquivo formato correto
	fin.open (fileName.c_str ());
	//  char aux[50];
	string aux;
	if (fin.good ()) { // Se carregou corretamente o arquivo
		fin >> aux;		//  Lê a primeira string ( Pi ou Di ou Vi -> P1,P2,...D1,D5...V1,V3...)
		if (aux[0] == 'V' || aux[0] == 'P' || aux[0] == 'D') { //se for arquivo de matriz válida.
			if ( aux.at(1) == '1' || aux.at(1) == '2' || aux.at(1) == '3' ) { // se o formato do dados for ASCII
				fin.seekg (0, ios::beg);	//  vai para posicao 0, a partir do inicio do arquivo
				return true;
			} else if ( aux.at(1) == '4' || aux.at(1) == '5' || aux.at(1) == '6') { // Verifica se o formato é BINÁRIO
				fin.close (); //  Fecha o arquivo e
				fin.open (fileName.c_str (), ios::binary); // Reabre como binario
				if (fin.good ())
					return true;
				else
					return false;
			}
		}
		fin.close (); 		// Arquivo inválido. Fechar...
	}
	return false;			//  nao abriu corretamente retorna erro
}

/*
	-------------------------------------------------------------------------
	Função:     VerificaFormato
	-------------------------------------------------------------------------
	@short  :VerificaFormatoSalvamento
	@author :André Duarte Bueno
	@see    :
	@param  :
	@return :

*/
EImageType CBaseMatriz::VerificaFormato(ifstream & fin) {
	if (fin.good ()) { //  Se abriu corretamente o arquivo
		char aux[255];
		//  fin >> aux;            //  Lê a primeira string ( Pi ou Di ou Vi ou um numero. ->P1,P2,...D1,D5..)
		fin.getline (aux, 255);
		//  0123456789
		/*if (aux[0]>= 48 && aux[0] <=57 )                            //  Se for um número, então só são dados
 {                                      //  deve receber nx,ny,nz como parametros.
 formatoImagem=WRITEFORM_ASCII_DATA;//  Preciso recolocar ponteiro de leitura no inicio do arquivo
 fin.seekg(0,ios::beg);                     //  reposiciona inicio arquivo
 return formatoImagem;
 }
 else */ //retorna o formato de salvamento
		switch (aux[0]) {
			case 'V':
				switch (aux[1]) {
					case '1':
						return formatoImagem = V1_X_ASCII;
					case '2':
						return formatoImagem = V2_X_GRAY_ASCII;
					case '3':
						return formatoImagem = V3_X_COLOR_ASCII;
					case '4':
						return formatoImagem = V4_X_BINARY;
					case '5':
						return formatoImagem = V5_X_GRAY_BINARY;
					case '6':
						return formatoImagem = V6_X_COLOR_BINARY;
					default:
						return formatoImagem = INVALID_IMAGE_TYPE;
				}
			case 'P':
				switch (aux[1]) {
					case '1':
						return formatoImagem = P1_X_Y_ASCII;
					case '2':
						return formatoImagem = P2_X_Y_GRAY_ASCII;
					case '3':
						return formatoImagem = P3_X_Y_COLOR_ASCII;
					case '4':
						return formatoImagem = P4_X_Y_BINARY;
					case '5':
						return formatoImagem = P5_X_Y_GRAY_BINARY;
					case '6':
						return formatoImagem = P6_X_Y_COLOR_BINARY;
					default:
						return formatoImagem = INVALID_IMAGE_TYPE;
				}
			case 'D':
				switch (aux[1]) {
					case '1':
						return formatoImagem = D1_X_Y_Z_ASCII;
					case '2':
						return formatoImagem = D2_X_Y_Z_GRAY_ASCII;
					case '3':
						return formatoImagem = D3_X_Y_Z_COLOR_ASCII;
					case '4':
						return formatoImagem = D4_X_Y_Z_BINARY;
					case '5':
						return formatoImagem = D5_X_Y_Z_GRAY_BINARY;
					case '6':
						return formatoImagem = D6_X_Y_Z_COLOR_BINARY;
					default:
						return formatoImagem = INVALID_IMAGE_TYPE;
				}
			default:
				return formatoImagem = INVALID_IMAGE_TYPE;
		}
	} else
		return formatoImagem = INVALID_IMAGE_TYPE;
}

/*
	-------------------------------------------------------------------------
	Função: Propriedades
	-------------------------------------------------------------------------
	@short  :
	@author :André Duarte Bueno
	@see    :
	@param  :
	@return :
*/
void CBaseMatriz::Propriedades (ofstream & os) const {
	os << "\nPropriedades objeto :"
		 << "\nDimensao da Matriz=" << DimensaoMatriz ()
		 << "\nMaior valor=" << MaiorValor ()
		 << "\nMenor valor=" << MenorValor ()
		 << "\nMedia=" << Media ()
		 << "\nFormato de salvamento=" << formatoImagem << endl;
}
