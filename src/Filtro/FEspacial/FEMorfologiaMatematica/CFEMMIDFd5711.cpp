//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
  ----------------------------------------------------------------------------
  PROJETO:      	Anaimp
  Analise de Imagens de Meios Porosos
  ----------------------------------------------------------------------------

  Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
  Termofisicas  dos Materiais.
  Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
  Liang Zirong, Paulo C. Philippi, ...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	CFEMMIDFd5711.cpp
  Nome da classe:      CFEMMIDFd5711
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  Descricao:	 Implementa a função CriaMascara da classe CFEMMIDFd5711.
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
//using namespace std;
#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFd5711.h"
//  Classe base
//  #include "Mascara/MCDiscreta\CMCd5711.h"  //  ponteiro para

/*
  ==================================================================================
  Documentacao 		Funcao Go
  ==================================================================================
  Descrição:
  Antes realizava um processo mais lento, criava uma mascara da mascara e depois
  percorria os valores da imagem usando esta mascara da mascara num processo mais confuso e lento.
  Da forma como esta não existe mais a necessidade de se definir uma mascara da mascara,
  de preencher os valores da mesma e realizar loops de calculo.
  Aqui substitui as chamadas a mascara da mascara pelo próprio valor, e substitui o loop pela repetição do código
  de teste para cada ponto da mascara.
  Para a d5711 tem-se 8 testes, o que não deixa o código muito extenso e
  compensa pela maior velocidade e compreensão.

  Observe que não percorre toda a imagem, como são acessados valores -2 e +2
  deve percorrer a parte interna da imagem, não acessando a borda da imagem.
  Ou seja desconsidera-se os pontos da borda da imagem.
*/
//  TMatriz2D< int > * CFEMMIDFd5711::Go( TMatriz2D< int > *& matriz)
/*
  TMatriz2D< int > *CFEMMIDFd5711::Go( TMatriz2D< int > *& matriz, unsigned int _tamanhoMascara )
  {
  InicializaIDF(matriz,_tamanhoMascara);//  armazena valores da matriz e _tamanhoMascara
  //  verifica se pm->data2D e this->data2D tem as mesmas dimensoes
  //  preenche os valores de data2D com os valores de pm
  IDFNosPlanosDeContorno(mi);				//  considera a primeira linha, fazendo=5
  //  deve considerar adicionalmente a segunda linha, pois abaixo a mesma não é verificada.
  int x,y;	      	//  Indices para percorrer a matriz
  int auxiliar,minimo;      //  variáveis auxiliares

  //  ida   MinimoIda
  //  Da forma como esta não percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
  for (y=2; y < ny ;y++)    	//  NY() é igual a ny, ny da matriz idf
  for (x=2; x < nx-2 ;x++)
  if (data2D[x][y]!=0)		//  Testa a imagem, se nao for solido entra
  {                                      //  
  minimo = raioMaximo;       	//  usa 32000
  //  -------------------------------------------------------------
  min(data2D[x-1][y]+5);     //  ponto [x][y]
  min(data2D[x-2][y-1]+11); min(data2D[x-1][y-1]+7);	
  min(data2D[x][y-1]+5);	min(data2D[x+1][y-1]+7);	
  min(data2D[x+2][y-1]+11);
  min(data2D[x-1][y-2]+11);							
  min(data2D[x+1][y-2]+11);
  //  -------------------------------------------------------------
  data2D[x][y]=minimo;
  }
  //  volta    MinimoVolta
  //  Da forma como esta não percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
  for (y = ny-3; y >= 0; y--)	     	//  -2 pois começa do zero e a mascara tem tamanho 1
  for (x = nx-3; x > 1; x--)
  if (data2D[x][y]!=0)	//  Se nao for solido
  {
  minimo = data2D[x][y];    //  Armazena valor minimo da ida
  //  ---------------------------------------------------------
  min(data2D[x-1][y+2]+11);									
  min(data2D[x+1][y+2]+11);
  min(data2D[x-2][y+1]+11);	
  min(data2D[x-1][y+1]+7);	
  min(data2D[x][y+1]+5);	
  min(data2D[x+1][y+1]+7);	
  min(data2D[x+2][y+1]+11);     //  ponto [x][y]  
  min(data2D[x+1][y]+5);
  //  -------------------------------------------------------------
  data2D[x][y]=minimo;
  }
  return pm;
  }
*/
