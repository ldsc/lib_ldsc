//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
/*
  ----------------------------------------------------------------------------
  PROJETO:		Anaimp
  Analise de Imagens de Meios Porosos
  ----------------------------------------------------------------------------

  Desenvolvido por:    	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
 Termofisicas  dos Materiais.
  Programadores:       	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	CFEMMIDFd34.cpp
  Nome da classe:      CFEMMIDFd34
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  Descricao:	 Implementa a função CriaMascara da classe CFEMMIDFd34.
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
//using namespace std;
#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFd34.h"	//  Classe base
//  #include "Mascara/MCDiscreta\CMCd34.h"              //  ponteiro para
//  #include "Mascara/CMascMascDiscretad34.h"            //  Cria objeto

/*
  ==================================================================================
  Documentacao      Go
  ==================================================================================
  Descrição:
  //  Substitui o uso da mascara da mascara, que usava loop's e índices confusos
  //  pelo acesso direto aos valores da mascara.
  //  Ao lado do código o esboço do ponto da mascara que esta sendo considerado.
  //  Da forma como esta o código fica mais rápido e compreensivo.
  Programador:      Andre Duarte Bueno
*/
/*
//  TCMatriz2D< int > * CFEMMIDFd34::Go(TCMatriz2D< int > * matriz )
TCMatriz2D< int > *CFEMMIDFd34::Go( TCMatriz2D< int > *& matriz, unsigned int _tamanhoMascara )
{
InicializaIDF(matriz,_tamanhoMascara);//  armazena valores da matriz e _tamanhoMascara
//  verifica se pm->data2D e this->data2D tem as mesmas dimensoes
//  preenche os valores de data2D com os valores de pm
//  Fim da funcao InicializaGo(matriz,_tamanhoMascara)
IDFNosPlanosDeContorno(mi);

int x,y;		  //  Indices para percorrer a matriz
int auxiliar,minimo;

//  ida   MinimoIda
//  Da forma como esta não percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
for (y=1; y < ny ;y++)             //  NY() é igual a ny, ny da matriz idf
for (x=1; x < nx-1 ;x++)
if (data2D[x][y]!=0)		  //  Testa a imagem, se nao for solido entra
{                                  //  
  minimo = raioMaximo;
  //  -----------------------------------------------------------
  min(data2D[x-1][y]+3);	    //  ponto[x][y]
  min(data2D[x-1][y-1]+4);	 min(data2D[x][y-1]+3);	min(data2D[x+1][y-1]+4);
  //  -----------------------------------------------------------

  data2D[x][y]=minimo;
 }
//  volta    MinimoVolta
//  Da forma como esta não percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
 for (y = ny-2; y >= 0; y--)		 //  -2 pois começa do zero e a mascara tem tamanho 1
   for (x = nx-2; x > 0; x--)
     if (data2D[x][y]!=0)                //  Se nao for solido
       {
	 minimo = data2D[x][y];           //  Armazena valor minimo da ida
	 //  -----------------------------------------------------------
	 min(data2D[x-1][y+1]+4);	min(data2D[x][y+1]+3);	 min(data2D[x+1][y+1]+4);
	 //  ponto[x][y]
	 min(data2D[x+1][y]+3);
	 //  -----------------------------------------------------------
	 data2D[x][y]=minimo;
       }
 return pm;
}
*/
