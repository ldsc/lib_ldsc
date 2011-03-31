//Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef TFEEsqueleto3D_cpp
#define TFEEsqueleto3D_cpp
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TFEEEsqueletizacao.cpp
Nome da classe:      TFEEEsqueletizacao
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
			Algoritimo de esqueletização
*/
/*
----------------------------------------------------------------------------
			BIBLIOTECAS
----------------------------------------------------------------------------
*/


#include "TFiltro\TFEspacial3D\TFEEsqueleto3D\TFEEsqueleto3D.h"
//Funcao que procura e elimina pés de galinha
//      int valorEsqueleto;                                                                     //valor final dos pixel's que pertencem ao esqueleto
//      int valorObjeto;                             //valor final dos pixel's que não pertencem ao esqueleto
//Versao simples, se pixel atual ativo e a direita ativo, elimina o da direita
void
TFEEsqueleto3D::EliminaPeGalinha (TMatriz3D * imagem)
{
///////////////////////////////////////////////////////////////////
////////OBS: da forma como esta elimina muito mais pontos.
////////Se for um ponto a ser apagado valor=0
////////Quando um ponto é o final da linha, deve ser marcado como inapagavel= valorEsqueleto+1
////////
////////SÖ USAR PARA ELIMINAR OS PES DE GALINHA< COMO ESQUELETO NAO FUNCIONA
///////////////////////////////////////////////////////////////////
  pm = imagem;
  int nx = pm->GetNX ();
  int ny = pm->GetNY ();
  int nz = pm->GetNY ();
  int x, y, z;
  int ***data = pm->GetData3D ();

  //oeste->leste
  for (z = 0; z < nz; z++)
    for (y = 0; y < ny; y++)
      for (x = 0; x < nx - 1; x++)
	if (data[x][y][z] == valorEsqueleto
	    && data[x + 1][y][z] == valorEsqueleto)
	  data[x][y][z] = 0;

  //leste->oeste
  for (z = 0; z < nz; z++)
    for (y = 0; y < ny; y++)
      for (x = nx - 1; x > 0; x--)
	if (data[x][y][z] == valorEsqueleto
	    && data[x - 1][y][z] == valorEsqueleto)
	  data[x][y][z] = 0;

  //sul->norte
  for (z = 0; z < nz; z++)
    for (x = 0; x < nx; x++)
      for (y = 0; y < ny - 1; y++)
	if (data[x][y][z] == valorEsqueleto
	    && data[x][y + 1][z] == valorEsqueleto)
	  data[x][y][z] = 0;

  //norte->sul
  for (z = 0; z < nz; z++)
    for (x = 0; x < nx; x++)
      for (y = ny - 1; y > 0; y--)
	if (data[x][y][z] == valorEsqueleto
	    && data[x][y - 1][z] == valorEsqueleto)
	  data[x][y][z] = 0;

  //back->front
  for (y = 0; y < ny; y++)
    for (x = 0; x < nx; x++)
      for (z = 0; z < nz - 1; z++)
	if (data[x][y][z] == valorEsqueleto
	    && data[x][y][z + 1] == valorEsqueleto)
	  data[x][y][z] = 0;

  //front ->back
  for (y = 0; y < ny; y++)
    for (x = 0; x < nx; x++)
      for (z = nz - 1; z > 0; z--)
	if (data[x][y][z] == valorEsqueleto
	    && data[x][y][z - 1] == valorEsqueleto)
	  data[x][y][z] = 0;
}
#endif
//while(ptr!=4)...
//     ptr++=0;
