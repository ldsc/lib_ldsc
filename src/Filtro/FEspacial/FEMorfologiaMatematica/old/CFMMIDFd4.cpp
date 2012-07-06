// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
/*
  ----------------------------------------------------------------------------
  PROJETO:		Anaimp
  Analise de Imagens de Meios Porosos
  ----------------------------------------------------------------------------

  Desenvolvido por:		Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
  Programadores:   		Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	CFMMIDFd4.cpp
  Nome da classe:      CFMMIDFd4
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  Descricao:	 Implementa a fun��o CriaMascara da classe CFMMIDFd4.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
using namespace std;

#include "TFiltro\TFEspacial\TFEMorfologiaMatematica\CFMMIDFd4.h"	// Classe base
// #include "TMascara\TMCDiscreta\TMCd4.h"  // ponteiro para


/*
// Mascara de chanfro d4
// 	   1
//  1  p  1
//     1
// CMatriz2D * CFMMIDFd4::Go( CMatriz2D *& matriz)
// Obs
CMatriz2D *CFMMIDFd4::Go( CMatriz2D *& matriz, unsigned int _tamanhoMascara )
{
ExecutadaPorGo(matriz);// ,_tamanhoMascara);
IDFNosPlanosDeContorno(mi);
// verifica se pm->data2D e this->data2D tem as mesmas dimensoes
// preenche os valores de data2D com os valores de pm
int x,y;// ,i;				// Indices para percorrer a matriz
int auxiliar,minimo;                  	// variaveis auxiliares

// ida   MinimoIda
int y_1;				// otimizacao
for (y=1; y < ny ;y++)                // NY() � igual a ny, ny da matriz idf
{
  y_1=y-1;
  for (x=1; x < nx-1 ;x++)
    if (data2D[x][y]!=0)		// Testa a imagem, se nao for solido entra
      {                             // 
	minimo = raioMaximo;	// 64000
	// -----------------------------------------------------------
	min(data2D[x-1][y]+1);  // ponto[x][y]
	min(data2D[x][y_1]+1);
	// -----------------------------------------------------------

	data2D[x][y]=minimo;
      }
 }
// volta    MinimoVolta
 int ym1;                              // otimizacao
 for (y = ny-2; y >= 0; y--)		// -2 pois come�a do zero e a mascara tem tamanho 1
   {ym1=y+1;
     for (x = nx-2; x > 0; x--)
       if (data2D[x][y]!=0)             // Se nao for solido, entra
	 {
	   minimo = data2D[x][y];        // armazena valor minimo ida
	   // -----------------------------------------------------------
	   min(data2D[x][y+1]+1);
	   // ponto[x][y]
	   min(data2D[x+1][y]+1);
	   // -----------------------------------------------------------
	   data2D[x][y]=minimo;
	 }
   }
 return pm;
}
*/
