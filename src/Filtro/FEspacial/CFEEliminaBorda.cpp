
// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEEliminaBorda.cpp
Nome da classe:      CFEEliminaBorda
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
----------------------------------------------------------------------------
	BIBLIOTECAS
----------------------------------------------------------------------------
*/
#include "Filtro/FEspacial/CFEEliminaBorda.h"

CMatriz2D *
CFEEliminaBorda::Go (CMatriz2D * imagem, int espessura)
{
  int nx = imagem->NX ();
  int ny = imagem->NY ();
  int nx_1 = nx - 1;
  int ny_1 = ny - 1;
  int desloc;
  for (desloc = 0; desloc < espessura; desloc++)
    {
      for (int x = desloc; x < nx - desloc; x++)
	{
	  imagem->data2D[x][desloc] = 0;
	  imagem->data2D[x][ny_1 - desloc] = 0;
	}
      for (int y = desloc + 1; y < ny - desloc - 1; y++)
	{
	  imagem->data2D[desloc][y] = 0;
	  imagem->data2D[nx_1 - desloc][y] = 0;
	}
    }
  return imagem;
}

// CFEBorda-->base da hierarquia
// CFEPreencheBorda(valor)-->Preenche a borda com determinado valor
// CFEEliminaBorda-->elimina a borda colocando 0
// CFESubstitueBorda-->substitue os pixels da borda com valor i por j
