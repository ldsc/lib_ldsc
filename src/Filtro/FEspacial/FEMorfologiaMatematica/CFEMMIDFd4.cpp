//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
/*
  ----------------------------------------------------------------------------
  PROJETO:		Anaimp
  Analise de Imagens de Meios Porosos
  ----------------------------------------------------------------------------

  Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
Termofisicas  dos Materiais.
  Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	CFEMMIDFd4.cpp
  Nome da classe:      CFEMMIDFd4
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  Descricao:	 Implementa a função CriaMascara da classe CFEMMIDFd4.
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
//using namespace std;
#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFd4.h"	//  Classe base

//  #include "Mascara/MCDiscreta\CMCd4.h"  //  ponteiro para

/*
//  Mascara de chanfro d4
//  	   1
//   1  p  1
//      1
//  TMatriz2D< int > * CFEMMIDFd4::Go( TMatriz2D< int > *& matriz)
//  Obs
TMatriz2D< int > *CFEMMIDFd4::Go( TMatriz2D< int > *& matriz, unsigned int _tamanhoMascara )
{
ExecutadaPorGo(matriz);//  ,_tamanhoMascara);
IDFNosPlanosDeContorno(mi);
//  verifica se pm->data2D e this->data2D tem as mesmas dimensoes
//  preenche os valores de data2D com os valores de pm
int x,y;//  ,i;				//  Indices para percorrer a matriz
int auxiliar,minimo;                  	//  variaveis auxiliares

//  ida   MinimoIda
int y_1;				//  otimizacao
for (y=1; y < ny ;y++)                //  NY() é igual a ny, ny da matriz idf
{
  y_1=y-1;
  for (x=1; x < nx-1 ;x++)
    if (data2D[x][y]!=0)		//  Testa a imagem, se nao for solido entra
      {                             //  
	minimo = raioMaximo;	//  64000
	//  -----------------------------------------------------------
	min(data2D[x-1][y]+1);  //  ponto[x][y]
	min(data2D[x][y_1]+1);
	//  -----------------------------------------------------------

	data2D[x][y]=minimo;
      }
 }
//  volta    MinimoVolta
 int ym1;                              //  otimizacao
 for (y = ny-2; y >= 0; y--)		//  -2 pois começa do zero e a mascara tem tamanho 1
   {ym1=y+1;
     for (x = nx-2; x > 0; x--)
       if (data2D[x][y]!=0)             //  Se nao for solido, entra
	 {
	   minimo = data2D[x][y];        //  armazena valor minimo ida
	   //  -----------------------------------------------------------
	   min(data2D[x][y+1]+1);
	   //  ponto[x][y]
	   min(data2D[x+1][y]+1);
	   //  -----------------------------------------------------------
	   data2D[x][y]=minimo;
	 }
   }
 return pm;
}
*/
