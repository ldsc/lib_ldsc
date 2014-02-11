
// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEBorda3D_cpp
#define CFEBorda3D_cpp

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEBorda3D.cpp
Nome da classe:      CFEBorda3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
----------------------------------------------------------------------------
			BIBLIOTECAS
----------------------------------------------------------------------------
 */

#include <AnaliseImagem/Filtro/FEspacial/CFEBorda3D.h>

// virtual TCMatriz3D<int> * Go(TCMatriz3D<int> * imagem, int _espessura=1);
TCMatriz3D<int> *
CFEBorda3D::Go (TCMatriz3D<int> * &imagem, unsigned int espessura)
{
  int nx = imagem->NX ();

  int ny = imagem->NY ();

  int nz = imagem->NZ ();
  int nx_1 = nx - 1;

  int ny_1 = ny - 1;

  int nz_1 = nz - 1;

  int x, y, z;

  for (int desloc = 0; desloc < espessura; desloc++)
    {
      for (x = desloc; x < nx - desloc; x++)
	for (y = desloc; y < ny - desloc; y++)
	  imagem->data3D[x][y][desloc] = imagem->data3D[x][y][nz_1 - desloc] =
	    valorBorda;

      for (x = desloc; x < nx - desloc; x++)
	for (z = desloc; z < nz - desloc; z++)
	  imagem->data3D[x][desloc][z] = imagem->data3D[x][ny_1 - desloc][z] =
	    valorBorda;

      for (z = desloc; z < nz - desloc; z++)
	for (y = desloc; y < ny - desloc; y++)
	  imagem->data3D[desloc][y][z] = imagem->data3D[nx_1 - desloc][y][z] =
	    valorBorda;
    }
  return imagem;
}

#endif
