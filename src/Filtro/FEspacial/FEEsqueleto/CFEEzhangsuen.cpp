// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:       	Anaimp.
	       	Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:     	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
 Termofisicas  dos Materiais.
Programadores:         	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEEZhangSuen.cpp
Nome da classe:      CFEEZhangSuen
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Algoritimo de esqueletização de  Zhang & Suen's thinnning algorithm
Stentiford's pre-processing
Holt's post-processing
*/

/*
----------------------------------------------------------------------------
		BIBLIOTECAS
----------------------------------------------------------------------------
*/
#define TRUE 1

#define FALSE 0

#define NORTH 1

#define SOUTH 3

#define REMOVE_STAIR 1

#include <cmath>
// #include <stdio.h>
using namespace std;

#include "Filtro/FEspacial/FEEsqueleto/CFEEZhangSuen.h"
// #include "TMascara\TMPassaBaixa.h" 
      // O filtro espacial atua sobre a mascara          
      // do passa baixa todos os valores=1  
      // outros filtros usam outras mascaras.

/*
==================================================================================
Documentacao      Go
==================================================================================
Descrição:
Programador:      Andre Duarte Bueno
// nome arquivo no dos
// thnbest <input file> <output file>
// thining best
*/

/*TMatriz2D< int > *CFEEZhangSuen::Go( TMatriz2D< int > *& matriz, unsigned int _tamanhoMascara )
{
	tamanhoMascara=_tamanhoMascara;
   return   Go(matriz);
}  */

TMatriz2D< int > *
CFEEZhangSuen::Go (TMatriz2D< int > * &matriz, unsigned int /*_tamanhoMascara*/ )
{
  // CriaMascara(tamanhoMascara);
  pm = matriz;			// pm é a matriz passada

  TMatriz2D< int > *imAux;		// imAux é uma matriz auxiliar

  unsigned int i, j;		// contadores

  //  Embed input into a slightly larger image
  // Pega a imagem passada, a pm e cria uma imagem imAux maior que pm
  // imAux acrescenta pontos nas bordas
  // imAux = newimage (pm->NX()+2, pm->NY()+2);
  imAux = new TMatriz2D< int > (pm->NX () + 2, pm->NY () + 2);	// cria imagem auxiliar, maior

  for (i = 0; i < pm->NX (); i++)
    for (j = 0; j < pm->NY (); j++)
      if (pm->data2D[i][j])
	imAux->data2D[i + 1][j + 1] = 1;	// copia valores de pm para imAuxiliar
      else
	imAux->data2D[i + 1][j + 1] = 0;

  for (i = 0; i < imAux->NX (); i++)	// a borda assume valor 1
    {
      imAux->data2D[i][0] = 1;
      imAux->data2D[i][imAux->NY () - 1] = 1;
    }

  for (j = 0; j < imAux->NY (); j++)	// a borda assume valor 1
    {
      imAux->data2D[0][j] = 1;
      imAux->data2D[imAux->NX () - 1][j] = 1;
    }

//  Pre_process       // pré-processa a imagem
  pre_smooth (imAux);		// ...
  aae (imAux);

//  Thin
  thnz (imAux);			// executa thining (esqueleto)

  for (i = 0; i < pm->NX (); i++)
    for (j = 0; j < pm->NY (); j++)	// O metodo original calcula o esqueleto sobre a fase branca
      // Aqui, eu verifico se esta é ponto do esqueleto
      if (imAux->data2D[i + 1][j + 1] == 0)	// e pinto de preto na imagem original
	// 
	pm->data2D[i][j] = 1;	// copia valores da matriz
  // original: pm->data2D[i][j] = imAux->data2D[i+1][j+1];// copia valores da matriz
  // esqueleto para a imagem passada
  return pm;
}


//       Zhang-Suen with Holt's staircase removal
void
CFEEZhangSuen::thnz (TMatriz2D< int > * imAux)
{
  unsigned int i, j;		// era int i,j;

  int k, again = 1;

  TMatriz2D< int > *tmp;

  // tmp = newimage (imAux->NX(), imAux->NY());
  tmp = new TMatriz2D< int > (imAux->NX (), imAux->NY ());

//  BLACK = 1, WHITE = 0.
  for (i = 0; i < imAux->NX (); i++)
    for (j = 0; j < imAux->NY (); j++)
      {
	if (imAux->data2D[i][j] > 0)
	  imAux->data2D[i][j] = 0;	// define PRETO BRANCO
	else
	  imAux->data2D[i][j] = 1;	// define PRETO BRANCO
	tmp->data2D[i][j] = 0;
      }

//  Mark and delete
  while (again)
    {
      again = 0;

      // Second sub-iteration
      for (i = 1; i < imAux->NX () - 1; i++)
	for (j = 1; j < imAux->NY () - 1; j++)
	  {
	    if (imAux->data2D[i][j] != 1)
	      continue;
	    k = nays8 (imAux, i, j);
	    if ((k >= 2 && k <= 6) && Connectivity (imAux, i, j) == 1)
	      {
		if (imAux->data2D[i][j + 1] * imAux->data2D[i - 1][j] *
		    imAux->data2D[i][j - 1] == 0
		    && imAux->data2D[i - 1][j] * imAux->data2D[i +
							       1][j] *
		    imAux->data2D[i][j - 1] == 0)
		  {
		    tmp->data2D[i][j] = 1;
		    again = 1;
		  }
	      }
	  }

      Delete (imAux, tmp);

      if (again == 0)
	break;

      // First sub-iteration
      for (i = 1; i < imAux->NX () - 1; i++)
	for (j = 1; j < imAux->NY () - 1; j++)
	  {
	    if (imAux->data2D[i][j] != 1)
	      continue;
	    k = nays8 (imAux, i, j);
	    if ((k >= 2 && k <= 6) && Connectivity (imAux, i, j) == 1)
	      {
		if (imAux->data2D[i - 1][j] * imAux->data2D[i][j + 1] *
		    imAux->data2D[i + 1][j] == 0
		    && imAux->data2D[i][j + 1] * imAux->data2D[i +
							       1][j] *
		    imAux->data2D[i][j - 1] == 0)
		  {
		    tmp->data2D[i][j] = 1;
		    again = 1;
		  }
	      }
	  }

      Delete (imAux, tmp);
    }

  // Post_process
  stair (imAux, tmp, NORTH);

  Delete (imAux, tmp);

  stair (imAux, tmp, SOUTH);

  Delete (imAux, tmp);

  //  Restore levels
  for (i = 1; i < imAux->NX () - 1; i++)
    for (j = 1; j < imAux->NY () - 1; j++)
      if (imAux->data2D[i][j] > 0)
	imAux->data2D[i][j] = 0;
      else
	imAux->data2D[i][j] = 255;

  // freeimage (tmp);
      delete tmp;				// deleta objeto imagem tmp
}

//       Delete any pixel in IM corresponding to a 1 in TMP
void
CFEEZhangSuen::Delete (TMatriz2D< int > * imAux, TMatriz2D< int > * tmp)
{
  // Delete pixels that are marked
  for (unsigned int i = 1; i < imAux->NX () - 1; i++)
    for (unsigned int j = 1; j < imAux->NY () - 1; j++)
      if (tmp->data2D[i][j])
	{
	  imAux->data2D[i][j] = 0;
	  tmp->data2D[i][j] = 0;
	}
}

//       Number of neighboring 1 pixels
int
CFEEZhangSuen::nays8 (TMatriz2D< int > * imAux, int r, int c)
{
  int i, j, k = 0;

  for (i = r - 1; i <= r + 1; i++)
    for (j = c - 1; j <= c + 1; j++)
      if (i != r || c != j)
	if (imAux->data2D[i][j] >= 1)
	  k++;
  return k;
}

//       Number of neighboring 0 pixels
int
CFEEZhangSuen::snays (TMatriz2D< int > * imAux, int r, int c)
{
  int i, j, k = 0;

  for (i = r - 1; i <= r + 1; i++)
    for (j = c - 1; j <= c + 1; j++)
      if (i != r || c != j)
	if (imAux->data2D[i][j] == 0)
	  k++;
  return k;
}

//       Connectivity by counting black-white transitions on the boundary
// NumeroConexoes
int
CFEEZhangSuen::Connectivity (TMatriz2D< int > * imAux, int r, int c)
{
  // int i, N=0;
  int N = 0;

  if (imAux->data2D[r][c + 1] >= 1 && imAux->data2D[r - 1][c + 1] == 0)
    N++;
  if (imAux->data2D[r - 1][c + 1] >= 1 && imAux->data2D[r - 1][c] == 0)
    N++;
  if (imAux->data2D[r - 1][c] >= 1 && imAux->data2D[r - 1][c - 1] == 0)
    N++;
  if (imAux->data2D[r - 1][c - 1] >= 1 && imAux->data2D[r][c - 1] == 0)
    N++;
  if (imAux->data2D[r][c - 1] >= 1 && imAux->data2D[r + 1][c - 1] == 0)
    N++;
  if (imAux->data2D[r + 1][c - 1] >= 1 && imAux->data2D[r + 1][c] == 0)
    N++;
  if (imAux->data2D[r + 1][c] >= 1 && imAux->data2D[r + 1][c + 1] == 0)
    N++;
  if (imAux->data2D[r + 1][c + 1] >= 1 && imAux->data2D[r][c + 1] == 0)
    N++;

  return N;
}

//       Stentiford's boundary smoothing method
void
CFEEZhangSuen::pre_smooth (TMatriz2D< int > * imAux)
{
  int i, j;

  for (i = 0; i < imAux->NX (); i++)
    for (j = 0; j < imAux->NY (); j++)
      if (imAux->data2D[i][j] == 0)
	if (snays (imAux, i, j) <= 2 && Yokoi (imAux, i, j) < 2)
	  imAux->data2D[i][j] = 2;

  for (i = 0; i < imAux->NX (); i++)
    for (j = 0; j < imAux->NY (); j++)
      if (imAux->data2D[i][j] == 2)
	imAux->data2D[i][j] = 1;
}

//       Stentiford's Acute Angle Emphasis
void
CFEEZhangSuen::aae (TMatriz2D< int > * imAux)
{
  int i, j, again, k;

  again = 0;

  for (k = 5; k >= 1; k -= 2)
    {
      for (i = 2; i < imAux->NX () - 2; i++)
	for (j = 2; j < imAux->NY () - 2; j++)
	  if (imAux->data2D[i][j] == 0)
	    match_du (imAux, i, j, k);

      for (i = 2; i < imAux->NX () - 2; i++)
	for (j = 2; j < imAux->NY () - 2; j++)
	  if (imAux->data2D[i][j] == 2)
	    {
	      again = 1;
	      imAux->data2D[i][j] = 1;
	    }
      if (again == 0)
	break;
    }
}

//       Template matches for acute angle emphasis
void
CFEEZhangSuen::match_du (TMatriz2D< int > * imAux, int r, int c, int k)
{

//  D1
  if (imAux->data2D[r - 2][c - 2] == 0 && imAux->data2D[r - 2][c - 1] == 0 &&
      imAux->data2D[r - 2][c] == 1 && imAux->data2D[r - 2][c + 1] == 0 &&
      imAux->data2D[r - 2][c + 2] == 0 &&
      imAux->data2D[r - 1][c - 2] == 0 && imAux->data2D[r - 1][c - 1] == 0 &&
      imAux->data2D[r - 1][c] == 1 && imAux->data2D[r - 1][c + 1] == 0 &&
      imAux->data2D[r - 1][c + 2] == 0 &&
      imAux->data2D[r][c - 2] == 0 && imAux->data2D[r][c - 1] == 0 &&
      imAux->data2D[r][c] == 0 && imAux->data2D[r][c + 1] == 0 &&
      imAux->data2D[r][c + 2] == 0 &&
      imAux->data2D[r + 1][c - 2] == 0 && imAux->data2D[r + 1][c - 1] == 0 &&
      imAux->data2D[r + 1][c] == 0 && imAux->data2D[r + 1][c + 1] == 0 &&
      imAux->data2D[r + 1][c + 2] == 0 &&
      imAux->data2D[r + 2][c - 1] == 0 &&
      imAux->data2D[r + 2][c] == 0 && imAux->data2D[r + 2][c + 1] == 0)
    {
      imAux->data2D[r][c] = 2;
      return;
    }

//  D2
  if (k >= 2)
    if (imAux->data2D[r - 2][c - 2] == 0 && imAux->data2D[r - 2][c - 1] == 1
	&& imAux->data2D[r - 2][c] == 1 && imAux->data2D[r - 2][c + 1] == 0
	&& imAux->data2D[r - 2][c + 2] == 0
	&& imAux->data2D[r - 1][c - 2] == 0
	&& imAux->data2D[r - 1][c - 1] == 0 && imAux->data2D[r - 1][c] == 1
	&& imAux->data2D[r - 1][c + 1] == 0
	&& imAux->data2D[r - 1][c + 2] == 0 && imAux->data2D[r][c - 2] == 0
	&& imAux->data2D[r][c - 1] == 0 && imAux->data2D[r][c] == 0
	&& imAux->data2D[r][c + 1] == 0 && imAux->data2D[r][c + 2] == 0
	&& imAux->data2D[r + 1][c - 2] == 0
	&& imAux->data2D[r + 1][c - 1] == 0 && imAux->data2D[r + 1][c] == 0
	&& imAux->data2D[r + 1][c + 1] == 0
	&& imAux->data2D[r + 1][c + 2] == 0
	&& imAux->data2D[r + 2][c - 1] == 0 && imAux->data2D[r + 2][c] == 0
	&& imAux->data2D[r + 2][c + 1] == 0)
      {
	imAux->data2D[r][c] = 2;
	return;
      }

//  D3
  if (k >= 3)
    if (imAux->data2D[r - 2][c - 2] == 0 && imAux->data2D[r - 2][c - 1] == 0
	&& imAux->data2D[r - 2][c] == 1 && imAux->data2D[r - 2][c + 1] == 1
	&& imAux->data2D[r - 2][c + 2] == 0
	&& imAux->data2D[r - 1][c - 2] == 0
	&& imAux->data2D[r - 1][c - 1] == 0 && imAux->data2D[r - 1][c] == 1
	&& imAux->data2D[r - 1][c + 1] == 0
	&& imAux->data2D[r - 1][c + 2] == 0 && imAux->data2D[r][c - 2] == 0
	&& imAux->data2D[r][c - 1] == 0 && imAux->data2D[r][c] == 0
	&& imAux->data2D[r][c + 1] == 0 && imAux->data2D[r][c + 2] == 0
	&& imAux->data2D[r + 1][c - 2] == 0
	&& imAux->data2D[r + 1][c - 1] == 0 && imAux->data2D[r + 1][c] == 0
	&& imAux->data2D[r + 1][c + 1] == 0
	&& imAux->data2D[r + 1][c + 2] == 0
	&& imAux->data2D[r + 2][c - 1] == 0 && imAux->data2D[r + 2][c] == 0
	&& imAux->data2D[r + 2][c + 1] == 0)
      {
	imAux->data2D[r][c] = 2;
	return;
      }

//  D4
  if (k >= 4)
    if (imAux->data2D[r - 2][c - 2] == 0 && imAux->data2D[r - 2][c - 1] == 1
	&& imAux->data2D[r - 2][c] == 1 && imAux->data2D[r - 2][c + 1] == 0
	&& imAux->data2D[r - 2][c + 2] == 0
	&& imAux->data2D[r - 1][c - 2] == 0
	&& imAux->data2D[r - 1][c - 1] == 1 && imAux->data2D[r - 1][c] == 1
	&& imAux->data2D[r - 1][c + 1] == 0
	&& imAux->data2D[r - 1][c + 2] == 0 && imAux->data2D[r][c - 2] == 0
	&& imAux->data2D[r][c - 1] == 0 && imAux->data2D[r][c] == 0
	&& imAux->data2D[r][c + 1] == 0 && imAux->data2D[r][c + 2] == 0
	&& imAux->data2D[r + 1][c - 2] == 0
	&& imAux->data2D[r + 1][c - 1] == 0 && imAux->data2D[r + 1][c] == 0
	&& imAux->data2D[r + 1][c + 1] == 0
	&& imAux->data2D[r + 1][c + 2] == 0
	&& imAux->data2D[r + 2][c - 1] == 0 && imAux->data2D[r + 2][c] == 0
	&& imAux->data2D[r + 2][c + 1] == 0)
      {
	imAux->data2D[r][c] = 2;
	return;
      }

//  D5
  if (k >= 5)
    if (imAux->data2D[r - 2][c - 2] == 0 && imAux->data2D[r - 2][c - 1] == 0
	&& imAux->data2D[r - 2][c] == 1 && imAux->data2D[r - 2][c + 1] == 1
	&& imAux->data2D[r - 2][c + 2] == 0
	&& imAux->data2D[r - 1][c - 2] == 0
	&& imAux->data2D[r - 1][c - 1] == 0 && imAux->data2D[r - 1][c] == 1
	&& imAux->data2D[r - 1][c + 1] == 1
	&& imAux->data2D[r - 1][c + 2] == 0 && imAux->data2D[r][c - 2] == 0
	&& imAux->data2D[r][c - 1] == 0 && imAux->data2D[r][c] == 0
	&& imAux->data2D[r][c + 1] == 0 && imAux->data2D[r][c + 2] == 0
	&& imAux->data2D[r + 1][c - 2] == 0
	&& imAux->data2D[r + 1][c - 1] == 0 && imAux->data2D[r + 1][c] == 0
	&& imAux->data2D[r + 1][c + 1] == 0
	&& imAux->data2D[r + 1][c + 2] == 0
	&& imAux->data2D[r + 2][c - 1] == 0 && imAux->data2D[r + 2][c] == 0
	&& imAux->data2D[r + 2][c + 1] == 0)
      {
	imAux->data2D[r][c] = 2;
	return;
      }

//  U1
  if (imAux->data2D[r + 2][c - 2] == 0 && imAux->data2D[r + 2][c - 1] == 0 &&
      imAux->data2D[r + 2][c] == 1 && imAux->data2D[r + 2][c + 1] == 0 &&
      imAux->data2D[r + 2][c + 2] == 0 &&
      imAux->data2D[r + 1][c - 2] == 0 && imAux->data2D[r + 1][c - 1] == 0 &&
      imAux->data2D[r + 1][c] == 1 && imAux->data2D[r + 1][c + 1] == 0 &&
      imAux->data2D[r + 1][c + 2] == 0 &&
      imAux->data2D[r][c - 2] == 0 && imAux->data2D[r][c - 1] == 0 &&
      imAux->data2D[r][c] == 0 && imAux->data2D[r][c + 1] == 0 &&
      imAux->data2D[r][c + 2] == 0 &&
      imAux->data2D[r - 1][c - 2] == 0 && imAux->data2D[r - 1][c - 1] == 0 &&
      imAux->data2D[r - 1][c] == 0 && imAux->data2D[r - 1][c + 1] == 0 &&
      imAux->data2D[r - 1][c + 2] == 0 &&
      imAux->data2D[r - 1][c - 1] == 0 &&
      imAux->data2D[r - 1][c] == 0 && imAux->data2D[r - 1][c + 1] == 0)
    {
      imAux->data2D[r][c] = 2;
      return;
    }

//  U2
  if (k >= 2)
    if (imAux->data2D[r + 2][c - 2] == 0 && imAux->data2D[r + 2][c - 1] == 1
	&& imAux->data2D[r + 2][c] == 1 && imAux->data2D[r + 2][c + 1] == 0
	&& imAux->data2D[r + 2][c + 2] == 0
	&& imAux->data2D[r + 1][c - 2] == 0
	&& imAux->data2D[r + 1][c - 1] == 0 && imAux->data2D[r + 1][c] == 1
	&& imAux->data2D[r + 1][c + 1] == 0
	&& imAux->data2D[r + 1][c + 2] == 0 && imAux->data2D[r][c - 2] == 0
	&& imAux->data2D[r][c - 1] == 0 && imAux->data2D[r][c] == 0
	&& imAux->data2D[r][c + 1] == 0 && imAux->data2D[r][c + 2] == 0
	&& imAux->data2D[r - 1][c - 2] == 0
	&& imAux->data2D[r - 1][c - 1] == 0 && imAux->data2D[r - 1][c] == 0
	&& imAux->data2D[r - 1][c + 1] == 0
	&& imAux->data2D[r - 1][c + 2] == 0
	&& imAux->data2D[r - 2][c - 1] == 0 && imAux->data2D[r - 2][c] == 0
	&& imAux->data2D[r - 2][c + 1] == 0)
      {
	imAux->data2D[r][c] = 2;
	return;
      }

//  U3
  if (k >= 3)
    if (imAux->data2D[r + 2][c - 2] == 0 && imAux->data2D[r + 2][c - 1] == 0
	&& imAux->data2D[r + 2][c] == 1 && imAux->data2D[r + 2][c + 1] == 1
	&& imAux->data2D[r + 2][c + 2] == 0
	&& imAux->data2D[r + 1][c - 2] == 0
	&& imAux->data2D[r + 1][c - 1] == 0 && imAux->data2D[r + 1][c] == 1
	&& imAux->data2D[r + 1][c + 1] == 0
	&& imAux->data2D[r + 1][c + 2] == 0 && imAux->data2D[r][c - 2] == 0
	&& imAux->data2D[r][c - 1] == 0 && imAux->data2D[r][c] == 0
	&& imAux->data2D[r][c + 1] == 0 && imAux->data2D[r][c + 2] == 0
	&& imAux->data2D[r - 1][c - 2] == 0
	&& imAux->data2D[r - 1][c - 1] == 0 && imAux->data2D[r - 1][c] == 0
	&& imAux->data2D[r - 1][c + 1] == 0
	&& imAux->data2D[r - 1][c + 2] == 0
	&& imAux->data2D[r - 2][c - 1] == 0 && imAux->data2D[r - 2][c] == 0
	&& imAux->data2D[r - 2][c + 1] == 0)
      {
	imAux->data2D[r][c] = 2;
	return;
      }

// * U4
  if (k >= 4)
    if (imAux->data2D[r + 2][c - 2] == 0 && imAux->data2D[r + 2][c - 1] == 1
	&& imAux->data2D[r + 2][c] == 1 && imAux->data2D[r + 2][c + 1] == 0
	&& imAux->data2D[r + 2][c + 2] == 0
	&& imAux->data2D[r + 1][c - 2] == 0
	&& imAux->data2D[r + 1][c - 1] == 1 && imAux->data2D[r + 1][c] == 1
	&& imAux->data2D[r + 1][c + 1] == 0
	&& imAux->data2D[r + 1][c + 2] == 0 && imAux->data2D[r][c - 2] == 0
	&& imAux->data2D[r][c - 1] == 0 && imAux->data2D[r][c] == 0
	&& imAux->data2D[r][c + 1] == 0 && imAux->data2D[r][c + 2] == 0
	&& imAux->data2D[r - 1][c - 2] == 0
	&& imAux->data2D[r - 1][c - 1] == 0 && imAux->data2D[r - 1][c] == 0
	&& imAux->data2D[r - 1][c + 1] == 0
	&& imAux->data2D[r - 1][c + 2] == 0
	&& imAux->data2D[r - 2][c - 1] == 0 && imAux->data2D[r - 2][c] == 0
	&& imAux->data2D[r - 2][c + 1] == 0)
      {
	imAux->data2D[r][c] = 2;
	return;
      }

// * U5
  if (k >= 5)
    if (imAux->data2D[r + 2][c - 2] == 0 && imAux->data2D[r + 2][c - 1] == 0
	&& imAux->data2D[r + 2][c] == 1 && imAux->data2D[r + 2][c + 1] == 1
	&& imAux->data2D[r + 2][c + 2] == 0
	&& imAux->data2D[r + 1][c - 2] == 0
	&& imAux->data2D[r + 1][c - 1] == 0 && imAux->data2D[r + 1][c] == 1
	&& imAux->data2D[r + 1][c + 1] == 1
	&& imAux->data2D[r + 1][c + 2] == 0 && imAux->data2D[r][c - 2] == 0
	&& imAux->data2D[r][c - 1] == 0 && imAux->data2D[r][c] == 0
	&& imAux->data2D[r][c + 1] == 0 && imAux->data2D[r][c + 2] == 0
	&& imAux->data2D[r - 1][c - 2] == 0
	&& imAux->data2D[r - 1][c - 1] == 0 && imAux->data2D[r - 1][c] == 0
	&& imAux->data2D[r - 1][c + 1] == 0
	&& imAux->data2D[r - 1][c + 2] == 0
	&& imAux->data2D[r - 2][c - 1] == 0 && imAux->data2D[r - 2][c] == 0
	&& imAux->data2D[r - 2][c + 1] == 0)
      {
	imAux->data2D[r][c] = 2;
	return;
      }
}

//       Yokoi's connectivity measure
int
CFEEZhangSuen::Yokoi (TMatriz2D< int > * imAux, int r, int c)
{
  int N[9];
  int i, k, i1, i2;		// j

  N[0] = imAux->data2D[r][c] != 0;
  N[1] = imAux->data2D[r][c + 1] != 0;
  N[2] = imAux->data2D[r - 1][c + 1] != 0;
  N[3] = imAux->data2D[r - 1][c] != 0;
  N[4] = imAux->data2D[r - 1][c - 1] != 0;
  N[5] = imAux->data2D[r][c - 1] != 0;
  N[6] = imAux->data2D[r + 1][c - 1] != 0;
  N[7] = imAux->data2D[r + 1][c] != 0;
  N[8] = imAux->data2D[r + 1][c + 1] != 0;

  k = 0;
  for (i = 1; i <= 7; i += 2)
    {
      i1 = i + 1;
      if (i1 > 8)
	i1 -= 8;
      i2 = i + 2;
      if (i2 > 8)
	i2 -= 8;
      k += (N[i] - N[i] * N[i1] * N[i2]);
    }

  return k;
}

//       Holt's staircase removal stuff
void
CFEEZhangSuen::check (int v1, int v2, int v3)
{
  if (!v2 && (!v1 || !v3))
    t00 = TRUE;
  if (v2 && (v1 || v3))
    t11 = TRUE;
  if ((!v1 && v2) || (!v2 && v3))
    {
      t01s = t01;
      t01 = TRUE;
    }
}

int
CFEEZhangSuen::edge (TMatriz2D< int > * imAux, int r, int c)
{
  if (imAux->data2D[r][c] == 0)
    return 0;

  t00 = t01 = t01s = t11 = FALSE;

  //  CHECK(vNW, vN, vNE)
  check (imAux->data2D[r - 1][c - 1], imAux->data2D[r - 1][c],
	 imAux->data2D[r - 1][c + 1]);

  //  CHECK (vNE, vE, vSE)
  check (imAux->data2D[r - 1][c + 1], imAux->data2D[r][c + 1],
	 imAux->data2D[r + 1][c + 1]);

  //  CHECK (vSE, vS, vSW)
  check (imAux->data2D[r + 1][c + 1], imAux->data2D[r + 1][c],
	 imAux->data2D[r + 1][c - 1]);

  //  CHECK (vSW, vW, vNW)
  check (imAux->data2D[r + 1][c - 1], imAux->data2D[r][c - 1],
	 imAux->data2D[r - 1][c - 1]);

  return t00 && t11 && !t01s;
}

void
CFEEZhangSuen::stair (TMatriz2D< int > * imAux, TMatriz2D< int > * tmp, int dir)
{
  int i, j;

  int N, S, E, W, NE, NW, SE, SW, C;

  if (dir == NORTH)
    for (i = 1; i < imAux->NX () - 1; i++)
      for (j = 1; j < imAux->NY () - 1; j++)
	{
	  NW = imAux->data2D[i - 1][j - 1];
	  N = imAux->data2D[i - 1][j];
	  NE = imAux->data2D[i - 1][j + 1];
	  W = imAux->data2D[i][j - 1];
	  C = imAux->data2D[i][j];
	  E = imAux->data2D[i][j + 1];
	  SW = imAux->data2D[i + 1][j - 1];
	  S = imAux->data2D[i + 1][j];
	  SE = imAux->data2D[i + 1][j + 1];

	  if (dir == NORTH)
	    {
	      if (C && !(N &&
			 ((E && !NE && !SW && (!W || !S)) ||
			  (W && !NW && !SE && (!E || !S)))))
		tmp->data2D[i][j] = 0;	//  Survives
	      else
		tmp->data2D[i][j] = 1;
	    }
	  else if (dir == SOUTH)
	    {
	      if (C && !(S &&
			 ((E && !SE && !NW && (!W || !N)) ||
			  (W && !SW && !NE && (!E || !N)))))
		tmp->data2D[i][j] = 0;	//  Survives
	      else
		tmp->data2D[i][j] = 1;
	    }
	}
}
