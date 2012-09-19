// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:	       Anaimp.
		      	Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEDCMarrHildreth.cpp
Nome da classe:      CFEDCMarrHildreth
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
----------------------------------------------------------------------------
     				BIBLIOTECAS
----------------------------------------------------------------------------
*/
#include <cmath>

#include <cstdio>

// #define MAX
// #include "lib.h"
using namespace std;

#include "Filtro/FEspacial/FEDeteccaoContorno/CFEDCMarrHildreth.h"
// #include "TMascara\TMPassaBaixa.h"    // O filtro espacial atua sobre a mascara                                                                // do passa baixa todos os valores=1
		       	// outros filtros usam outras mascaras.
#ifndef CImagem_h
#include "Matriz/CImagem.h"	// inclue biblioteca imagem
#endif // tem a f2d temporariamente

/*
==================================================================================
Documentacao      Go
==================================================================================
Descricao:
Programador:      Andre Duarte Bueno
*/

/*
TCMatriz2D< int > *CFEDCMarrHildreth::Go( TCMatriz2D< int > *& matriz, unsigned int _tamanhoMascara )
{
	tamanhoMascara=_tamanhoMascara;
	return   Go(matriz);
} */

// O arquivo original pode ler os parametros de um arquivo de disco
// Conteudo do arquivo de parametros (mar.par) 2.0       0.3
// Recebe como parametro o desvio padrão
TCMatriz2D< int > *
CFEDCMarrHildreth::Go (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara)
{
  // CriaMascara(tamanhoMascara);
  pm = matriz;

  float s = 1.0;

  TCMatriz2D< int > *pm2;		// matriz pm2 uma copia de pm

  pm2 = new TCMatriz2D< int > (*pm);	// im2 é uma copia de im1

  marr (s - 0.8, pm);		// aplica o filtro de marr em im1 e im2

  marr (s + 0.8, pm2);

  // calcula o resultado final combinando pm e pm2
  for (unsigned int i = 0; i < pm->NX (); i++)
    for (unsigned int j = 0; j < pm->NY (); j++)
      if (pm->data2D[i][j] > 0 && pm2->data2D[i][j] > 0)
	pm->data2D[i][j] = 0;
      else
	pm->data2D[i][j] = 255;

  return pm;			// pm é o resultado
}

// inline
float
CFEDCMarrHildreth::norm (float x, float y)
{
  return (float) sqrt ((double) (x * x + y * y));
}

// inline
float
CFEDCMarrHildreth::distance (float a, float b, float c, float d)
{
  return norm ((a - c), (b - d));
}

void
CFEDCMarrHildreth::marr (float s, TCMatriz2D< int > * im)
{
  int width;

  float **smx;

  int i, j, k, n;

  float **lgau;			// , z;

  /* Create a Gaussian and a derivative of Gaussian filter mask  */
  width = 3.35 * s + 0.33;

  n = width + width + 1;

  // printf ("Smoothing with a Gaussian of size %dx%d\n", n, n);
  lgau = f2d (n, n);

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      lgau[i][j] = LoG (distance ((float) i, (float) j,
				  (float) width, (float) width), s);

  /* Convolution of source image with a Gaussian in X and Y directions  */
  smx = f2d (im->NX (), im->NY ());

  // printf ("Convolution with LoG:\n");
  convolution (im, lgau, n, n, smx, im->NX (), im->NY ());

  /* Locate the zero crossings */
  // printf ("Zero crossings:\n");
  zero_cross (smx, im);

  /* Clear the boundary */
  for (i = 0; i < im->NX (); i++)
    {
      for (j = 0; j <= width; j++)
	im->data2D[i][j] = 0;
      for (j = im->NY () - width - 1; j < im->NY (); j++)
	im->data2D[i][j] = 0;
    }

  for (j = 0; j < im->NY (); j++)
    {
      for (i = 0; i <= width; i++)
	im->data2D[i][j] = 0;
      for (i = im->NX () - width - 1; i < im->NX (); i++)
	im->data2D[i][j] = 0;
    }

  free (smx[0]);

  free (smx);

  free (lgau[0]);

  free (lgau);
}

/*	Gaussian	*/
// inline
float
CFEDCMarrHildreth::gauss (float x, float sigma)
{
  return (float) exp ((double) ((-x * x) / (2 * sigma * sigma)));
}

// inline
float
CFEDCMarrHildreth::meanGauss (float x, float sigma)
{
  float z;

  z =
    (gauss (x, sigma) + gauss (x + 0.5, sigma) +
     gauss (x - 0.5, sigma)) / 3.0;

  z = z / (M_PI * 2.0 * sigma * sigma);	// M_PI PI

  return z;
}

// inline
float
CFEDCMarrHildreth::LoG (float x, float sigma)
{
  float x1;

  x1 = gauss (x, sigma);

  return (x * x - 2 * sigma * sigma) / (sigma * sigma * sigma * sigma) * x1;
}

/*
float ** CFEDCMarrHildreth::f2d (int nr, int nc)
{
	float **x, *y;
	int i;

	x = (float **)calloc ( nr, sizeof (float *) );
	y = (float *) calloc ( nr*nc, sizeof (float)  );
	if ( (x==0) || (y==0) )
	{
	  fprintf (stderr, "Out of storage: F2D.\n");
	  exit (1);
	}
	for (i=0; i<nr; i++)
	  x[i] = y+i*nc;
	return x;
}
*/

void
CFEDCMarrHildreth::convolution (TCMatriz2D< int > * im, float **mask, int nr, int nc,
				float **res, int NR, int NC)
{
  int i, j, ii, jj, n, m;
  int k, kk;
  float x;			// , y;

  k = nr / 2;
  kk = nc / 2;
  for (i = 0; i < NR; i++)
    for (j = 0; j < NC; j++)
      {
	x = 0.0;
	for (ii = 0; ii < nr; ii++)
	  {
	    n = i - k + ii;
	    if (n < 0 || n >= NR)
	      continue;
	    for (jj = 0; jj < nc; jj++)
	      {
		m = j - kk + jj;
		if (m < 0 || m >= NC)
		  continue;
		x += mask[ii][jj] * (float) (im->data2D[n][m]);
	      }
	  }
	res[i][j] = x;
      }
}

void
CFEDCMarrHildreth::zero_cross (float **lapim, TCMatriz2D< int > * im)
{
  // int i,j,k,n,m, dx, dy;
  unsigned int i, j;

  // float x, y, z;
  // int xi,xj,yi,yj, count = 0;
  // TCMatriz2D< int >* deriv;

  for (i = 1; i < im->NX () - 1; i++)
    for (j = 1; j < im->NY () - 1; j++)
      {
	im->data2D[i][j] = 0;
	if (lapim[i - 1][j] * lapim[i + 1][j] < 0)
	  {
	    im->data2D[i][j] = 255;
	    continue;
	  }
	if (lapim[i][j - 1] * lapim[i][j + 1] < 0)
	  {
	    im->data2D[i][j] = 255;
	    continue;
	  }
	if (lapim[i + 1][j - 1] * lapim[i - 1][j + 1] < 0)
	  {
	    im->data2D[i][j] = 255;
	    continue;
	  }
	if (lapim[i - 1][j - 1] * lapim[i + 1][j + 1] < 0)
	  {
	    im->data2D[i][j] = 255;
	    continue;
	  }
      }
}

/*	An alternative way to compute a Laplacian	*/
void
CFEDCMarrHildreth::dolap (float **x, int nr, int nc, float **y)
{
  // int k,n,m; declarava mas nao usava?
  float u, v;

  // o compilador acusou erro, possivel uso de u,v sem definição
  // tinha um erro, usava u sem definir, acrescentei v=u=o
  u = v = 0;

  for (int i = 1; i < nr - 1; i++)
    for (int j = 1; j < nc - 1; j++)
      {
	y[i][j] =
	  (x[i][j + 1] + x[i][j - 1] + x[i - 1][j] + x[i + 1][j]) -
	  4 * x[i][j];
	if (u > y[i][j])
	  u = y[i][j];
	if (v < y[i][j])
	  v = y[i][j];
      }
}

