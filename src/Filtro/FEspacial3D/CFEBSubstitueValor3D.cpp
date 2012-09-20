
// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEBSubstitueValor3D_cpp
#define CFEBSubstitueValor3D_cpp

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp.
		Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEBSubstitueValor3D.cpp
Nome da classe:      CFEBSubstitueValor3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
----------------------------------------------------------------------------
			BIBLIOTECAS
----------------------------------------------------------------------------
 */

#include "Filtro/FEspacial3D/CFEBSubstitueValor3D.h"
// Construtor:   CFEBSubstitueValor3D( int _valorAntigo, int  vB=0,TCMatriz3D<int> * matriz=0, unsigned int _tamanhoMascara=0 )
// Exemplo de uso: CFEBSubstitueValor3D(valorAntigo,antigoValor).Go(imagem);

// Trocar todos os pixel's com (valorBorda) por (valorAntigo)
TCMatriz3D<int> *
CFEBSubstitueValor3D::Go (TCMatriz3D<int> * &imagem, unsigned int espessura /*=1*/ )
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
	  {
	    if (imagem->data3D[x][y][desloc] == valorAntigo)
	      imagem->data3D[x][y][desloc] = valorBorda;
	    if (imagem->data3D[x][y][nz_1 - desloc] == valorAntigo)
	      imagem->data3D[x][y][nz_1 - desloc] = valorBorda;
	  }
      for (x = desloc; x < nx - desloc; x++)
	for (z = desloc; z < nz - desloc; z++)
	  {
	    if (imagem->data3D[x][desloc][z] == valorAntigo)
	      imagem->data3D[x][desloc][z] = valorBorda;
	    if (imagem->data3D[x][ny_1 - desloc][z] == valorAntigo)
	      imagem->data3D[x][ny_1 - desloc][z] = valorBorda;
	  }
      for (z = desloc; z < nz - desloc; z++)
	for (y = desloc; y < ny - desloc; y++)
	  {
	    if (imagem->data3D[desloc][y][z] == valorAntigo)
	      imagem->data3D[desloc][y][z] = valorBorda;
	    if (imagem->data3D[nx_1 - desloc][y][z] == valorAntigo)
	      imagem->data3D[nx_1 - desloc][y][z] = valorBorda;
	  }
    }
  return imagem;
}
#endif
