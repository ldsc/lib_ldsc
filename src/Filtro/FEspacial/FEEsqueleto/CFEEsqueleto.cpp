// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
 Termofisicas 
 dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEEEsqueletizacao.cpp
Nome da classe:      CFEEEsqueletizacao
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Algoritimo de esqueletização
*/

/*
----------------------------------------------------------------------------
       			BIBLIOTECAS
----------------------------------------------------------------------------
*/

#include "Filtro/FEspacial/FEEsqueleto/CFEEsqueleto.h"

// Funcao que procura e elimina pés de galinha
//       int valorEsqueleto;      
 // valor final dos pixel's que pertencem ao esqueleto
//       int valorObjeto;          
// valor final dos pixel's que não pertencem ao esqueleto
// Versao simples, se pixel atual ativo e a direita ativo, elimina o da direita

void
CFEEsqueleto::EliminaPeGalinha (CMatriz2D * imagem)
{
// // // // // // // // // // // // // // // // // /
// // OBS: da forma como esta elimina muito mais pontos.
// // Se for um ponto a ser apagado valor=0
// // Quando um ponto é o final da linha, deve ser marcado como inapagavel= valorEsqueleto+1
// // 
// // Só USAR PARA ELIMINAR OS PES DE GALINHA< COMO ESQUELETO NAO FUNCIONA
// // // // // // // // // // // // // // // // // /
  pm = imagem;

  int nx = pm->NX ();

  int ny = pm->NY ();

  int x, y;

  int **data = pm->Data2D ();

  // oeste->leste
  for (y = 0; y < ny; y++)
    for (x = 0; x < nx - 1; x++)
      if (data[x][y] == valorEsqueleto && data[x + 1][y] == valorEsqueleto)
	data[x][y] = 0;

  // leste->oeste
  for (y = 0; y < ny; y++)
    for (x = nx - 1; x > 0; x--)
      if (data[x][y] == valorEsqueleto && data[x - 1][y] == valorEsqueleto)
	data[x][y] = 0;

  // sul->norte
  for (x = 0; x < nx; x++)
    for (y = 0; y < ny - 1; y++)
      if (data[x][y] == valorEsqueleto && data[x][y + 1] == valorEsqueleto)
	data[x][y] = 0;

  // norte->sul
  for (x = 0; x < nx; x++)
    for (y = ny - 1; y > 0; y--)
      if (data[x][y] == valorEsqueleto && data[x][y - 1] == valorEsqueleto)
	data[x][y] = 0;

}

// while(ptr!=4)...
//      ptr++=0;
