
// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEBSubstitueValor.cpp
Nome da classe:      CFEBSubstitueValor
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
----------------------------------------------------------------------------
			BIBLIOTECAS
----------------------------------------------------------------------------
*/
#include "Filtro/FEspacial/CFEBSubstitueValor.h"
// Construtor:   CFEBSubstitueValor( int _valorAntigo, int  vB=0,CMatriz2D * matriz=0, unsigned int _tamanhoMascara=0 )
// Exemplo de uso: CFEBSubstitueValor(valorAntigo,antigoValor).Go(imagem);

// Trocar todos os pixel's com (valorAntigo) por (valorBorda)
CMatriz2D *
CFEBSubstitueValor::Go (CMatriz2D * &imagem, unsigned int espessura /*=1*/ )
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
	  if (imagem->data2D[x][desloc] == valorAntigo)
	    imagem->data2D[x][desloc] = valorBorda;

	  if (imagem->data2D[x][ny_1 - desloc] == valorAntigo)
	    imagem->data2D[x][ny_1 - desloc] = valorBorda;
	}
      for (int y = desloc + 1; y < ny - desloc - 1; y++)
	{
	  if (imagem->data2D[desloc][y] == valorAntigo)
	    imagem->data2D[desloc][y] = valorBorda;

	  if (imagem->data2D[nx_1 - desloc][y] == valorAntigo)
	    imagem->data2D[nx_1 - desloc][y] = valorBorda;
	}
    }
  return imagem;
}

