
// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEBorda.cpp
Nome da classe:      CFEBorda
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
----------------------------------------------------------------------------
                        BIBLIOTECAS
----------------------------------------------------------------------------
*/

#include "Filtro/FEspacial/CFEBorda.h"
// virtual CMatriz2D* Go(CMatriz2D* imagem, int _espessura=1);

CMatriz2D *
CFEBorda::Go (CMatriz2D * &imagem, unsigned int espessura)
{
  int nx = imagem->NX ();
  int ny = imagem->NY ();
  int nx_1 = nx - 1;
  int ny_1 = ny - 1;
  int desloc;

// Teste acesso aos dados da imagem diretamente:
// Na CMatriz2D inline int   ** GetData2D() const    {return data2D;};       // RETORNA data2D
  int **img = imagem->Data2D ();

  for (desloc = 0; desloc < espessura; desloc++)
    {
      for (int x = desloc; x < nx - desloc; x++)
	// Funciona: imagem->data2D[x][desloc]   =     imagem->data2D[x][ny_1-desloc]=valorBorda;
	img[x][desloc] = img[x][ny_1 - desloc] = valorBorda;

      for (int y = desloc + 1; y < ny - desloc - 1; y++)
	// Funciona: imagem->data2D[desloc][y]     =     imagem->data2D[nx_1-desloc][y]=valorBorda;
	img[desloc][y] = img[nx_1 - desloc][y] = valorBorda;
    }
  return imagem;
}

