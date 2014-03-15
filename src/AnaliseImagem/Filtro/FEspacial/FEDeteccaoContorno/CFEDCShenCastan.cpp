// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:       	Anaimp.
       		Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:      	Laboratorio de Desenvolvimento de Software Cientifico   
dos Materiais.
Programadores:         	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEDCShenCastan.cpp
Nome da classe:      CFEDCShenCastan
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
----------------------------------------------------------------------------
				BIBLIOTECAS
----------------------------------------------------------------------------
*/
// Algoritimo de detecção de contorno de ShenCastan.
#include <cstdio>
#include <cstring>
#include <cmath>
// #define MAX
// #include "lib.h"
using namespace std;

#define OUTLINE_SC 25		// linha externa

#include "AnaliseImagem/Filtro/FEspacial/FEDeteccaoContorno/CFEDCShenCastan.h"

// #include "TMascara\TMPassaBaixa.h"
#ifndef CImagem_h
#include "AnaliseImagem/Matriz/CImagem.h"	// Por causa f2d
#endif // 

/*
==================================================================================
Documentacao      Construtor
==================================================================================
Descricao:
*/
// CFEDCShenCastan( TCMatriz2D< int > *& matriz, unsigned int _tamanhoMascara );
CFEDCShenCastan::CFEDCShenCastan (TCMatriz2D< int > * &matriz,
				  unsigned int _tamanhoMascara)
                                : CFEDeteccaoContorno (matriz, _tamanhoMascara)
{
  b = 0.9;			// fator de amortecimento entre 0 e 1

  low_thresh = 20;		// nivel de corte para histerese

  high_thresh = 22;		// nivel de corte para histerese

  ratio = 0.99;			// razão

  window_size = 7;		// tamanho da janela

  do_hysteresis = 1;		// 

  				// Parametros do arquivo de disco
    				// Os parametros do arquivo de disco sao:
    				// ratio     0.035   (percentagem de pixel's acima do nivel de corte)
    // b          .91     (verifica se esta no intervalo 0<=b<=1)(fator amortecimento)
    // window_size        10      (tamanho da janela para filtro adaptativo)
    // thinFactor     1   (fator de esqueletização)
    // do_hysteresis  1   (verdadeiro ou falso)(Hysteresis thresholding )
    //   if (b<0) b = 0;
    //     else if (b>1.0) b = 1.0;
}

/*
==================================================================================
Documentacao      Go
==================================================================================
Descricao:
Programador:      Andre Duarte Bueno
*/

/*TCMatriz2D< int > *CFEDCShenCastan::Go( TCMatriz2D< int > *& matriz, unsigned int _tamanhoMascara )
{
	tamanhoMascara=_tamanhoMascara;
   return   Go(matriz);
} */

TCMatriz2D< int > *
CFEDCShenCastan::Go (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara)
{
  // CriaMascara(tamanhoMascara);

  pm = matriz;			// armazena endereço da matriz passada

  TCMatriz2D< int > *res;		// matriz resultado

  // inicia procedimento de cálculo

  embed (pm, OUTLINE_SC);	// chama função embed, passando a imagem e OUTLINE_SC

  res = new TCMatriz2D< int > (pm->NX (), pm->NY ());	// cria matriz resultado

  shen (pm, res);		// processa metodo de shen

  debed (res, OUTLINE_SC);	// chama debed passando, a matriz resultado

  for (unsigned int j = 0; j < pm->NY (); j++)	// faz pm igual a matriz resultado
    for (unsigned int i = 0; i < pm->NX (); i++)
      pm->data2D[i][j] = res->data2D[i][j];

  delete res;			// apaga objeto matriz temporaria res
  return pm;			// retorna a imagem res
}

// Algoritimo de detecção de contorno de Shen-Castan
void
CFEDCShenCastan::shen (TCMatriz2D< int > * &im, TCMatriz2D< int > * &res)
{
  register unsigned int i, j;

  float **buffer;		// matrizes float

  float **smoothed_buffer;

  TCMatriz2D< int > *bli_buffer;

  /* Convert the input image to floating point */
  buffer = f2d (im->NX (), im->NY ());

  for (i = 0; i < im->NX (); i++)
    for (j = 0; j < im->NY (); j++)
      buffer[i][j] = (float) (im->data2D[i][j]);

  /* Smooth input image using recursively implemented ISEF filter */
  smoothed_buffer = f2d (im->NX (), im->NY ());

  compute_ISEF (buffer, smoothed_buffer, im->NX (), im->NY ());

  /* Compute bli image band-limited laplacian image from smoothed image */
  // retorna ponteiro para TCMatriz2D< int >
  bli_buffer =
    compute_bli (smoothed_buffer, buffer, im->NX (), im->NY ());

  /* Perform edge detection using bli and gradient thresholding */
  locate_zero_crossings (buffer, smoothed_buffer, bli_buffer, im->NX (),
			 im->NY ());

  free (smoothed_buffer[0]);

  free (smoothed_buffer);

  delete bli_buffer;		// antes era freeimage (bli_buffer);, apaga objeto imagem auxiliar

  threshold_edges (buffer, res, im->NX (), im->NY ());
  for (i = 0; i < im->NX (); i++)
    for (j = 0; j < im->NY (); j++)
      if (res->data2D[i][j] > 0)
	res->data2D[i][j] = 0;
      else
	res->data2D[i][j] = 255;

  free (buffer[0]);
  free (buffer);		// buffer é uma matriz 2D
}

/*	Recursive filter realization of the ISEF
(Shen and Castan CVIGP March 1992)	 */
void
CFEDCShenCastan::compute_ISEF (float **x, float **y, int nrows, int ncols)
{
  float **A, **B;		// matrizes float

  A = f2d (nrows, ncols);	// store causal component

  B = f2d (nrows, ncols);	// store anti-causal component

  /* first apply the filter in the vertical direcion (to the rows) */
  apply_ISEF_vertical (x, y, A, B, nrows, ncols);

  /* now apply the filter in the horizontal direction (to the columns) and */
  /* apply this filter to the results of the previous one */
  apply_ISEF_horizontal (y, y, A, B, nrows, ncols);

  /* free up the memory */
  free (B[0]);

  free (B);			// deleta matrizes alocadas

  free (A[0]);

  free (A);			// 
}

void
CFEDCShenCastan::apply_ISEF_vertical (float **x, float **y,
				      float **A, float **B, int nrows,
				      int ncols)
{
  register int row, col;

  float b1, b2;

  b1 = (1.0 - b) / (1.0 + b);

  b2 = b * b1;

  /* compute boundary conditions */
  for (col = 0; col < ncols; col++)
    {
      /* boundary exists for 1st and last column */
      A[0][col] = b1 * x[0][col];
      B[nrows - 1][col] = b2 * x[nrows - 1][col];
    }

  /* compute causal component */
  for (row = 1; row < nrows; row++)
    for (col = 0; col < ncols; col++)
      A[row][col] = b1 * x[row][col] + b * A[row - 1][col];

  /* compute anti-causal component */
  for (row = nrows - 2; row >= 0; row--)
    for (col = 0; col < ncols; col++)
      B[row][col] = b2 * x[row][col] + b * B[row + 1][col];

  /* boundary case for computing output of first filter */
  for (col = 0; col < ncols - 1; col++)
    y[nrows - 1][col] = A[nrows - 1][col];

  /* now compute the output of the first filter and store in y */
  /* this is the sum of the causal and anti-causal components */
  for (row = 0; row < nrows - 2; row++)
    for (col = 0; col < ncols - 1; col++)
      y[row][col] = A[row][col] + B[row + 1][col];
}


void
CFEDCShenCastan::apply_ISEF_horizontal (float **x, float **y,
					float **A, float **B, int nrows,
					int ncols)
{
  register int row, col;

  float b1, b2;

  b1 = (1.0 - b) / (1.0 + b);

  b2 = b * b1;

  /* compute boundary conditions */
  for (row = 0; row < nrows; row++)
    {
      A[row][0] = b1 * x[row][0];
      B[row][ncols - 1] = b2 * x[row][ncols - 1];
    }

  /* compute causal component */
  for (col = 1; col < ncols; col++)
    for (row = 0; row < nrows; row++)
      A[row][col] = b1 * x[row][col] + b * A[row][col - 1];

  /* compute anti-causal component */
  for (col = ncols - 2; col >= 0; col--)
    for (row = 0; row < nrows; row++)
      B[row][col] = b2 * x[row][col] + b * B[row][col + 1];

  /* boundary case for computing output of first filter */
  for (row = 0; row < nrows; row++)
    y[row][ncols - 1] = A[row][ncols - 1];

  /* now compute the output of the second filter and store in y */
  /* this is the sum of the causal and anti-causal components */
  for (row = 0; row < nrows; row++)
    for (col = 0; col < ncols - 1; col++)
      y[row][col] = A[row][col] + B[row][col + 1];
}

/* compute the band-limited laplacian of the input image */
TCMatriz2D< int > *
CFEDCShenCastan::compute_bli (float **buff1, float **buff2, int nrows,
			      int ncols)
{
  register int row, col;

  TCMatriz2D< int > *bli_buffer;

  bli_buffer = new TCMatriz2D< int > (nrows, ncols);
  for (row = 0; row < nrows; row++)
    for (col = 0; col < ncols; col++)
      bli_buffer->data2D[row][col] = 0;

  /* The bli is computed by taking the difference between the smoothed image */
  /* and the original image.  In Shen and Castan's paper this is shown to */
  /* approximate the band-limited laplacian of the image.  The bli is then */
  /* made by setting all values in the bli to 1 where the laplacian is */
  /* positive and 0 otherwise.                                            */
  for (row = 0; row < nrows; row++)
    for (col = 0; col < ncols; col++)
      {
	if (row < OUTLINE_SC || row >= nrows - OUTLINE_SC ||
	    col < OUTLINE_SC || col >= ncols - OUTLINE_SC)
	  continue;
	bli_buffer->data2D[row][col] =
	  ((buff1[row][col] - buff2[row][col]) > 0.0);
      }
  return bli_buffer;
}

void
CFEDCShenCastan::locate_zero_crossings (float **orig, float **smoothed,
					TCMatriz2D< int > * &bli, int nrows,
					int ncols)
{
  register int row, col;

  for (row = 0; row < nrows; row++)
    {
      for (col = 0; col < ncols; col++)
	{

	  /* ignore pixels around the boundary of the image */
	  if (row < OUTLINE_SC || row >= nrows - OUTLINE_SC
	      || col < OUTLINE_SC || col >= ncols - OUTLINE_SC)
	    {
	      orig[row][col] = 0.0;
	    }

	  /* next check if pixel is a zero-crossing of the laplacian  */
	  else if (is_candidate_edge (bli, smoothed, row, col))
	    {
	      /* now do gradient thresholding  */
	      float grad =
		compute_adaptive_gradient (bli, smoothed, row, col);
	      orig[row][col] = grad;
	    }
	  else
	    orig[row][col] = 0.0;
	}
    }
}

void
CFEDCShenCastan::threshold_edges (float **in, TCMatriz2D< int > * &out, int nrows,
				  int ncols)
{
  register int i, j;

  lap = in;

  edges = out;

  nr = nrows;

  nc = ncols;

  estimate_thresh (&low_thresh, &high_thresh, nr, nc);
  if (!do_hysteresis)
    low_thresh = high_thresh;

  for (i = 0; i < nrows; i++)
    for (j = 0; j < ncols; j++)
      edges->data2D[i][j] = 0;

  for (i = 0; i < nrows; i++)
    for (j = 0; j < ncols; j++)
      {
	if (i < OUTLINE_SC || i >= nrows - OUTLINE_SC || j < OUTLINE_SC
	    || j >= ncols - OUTLINE_SC)
	  continue;

	/* only check a contour if it is above high_thresh */
	if ((lap[i][j]) > high_thresh)

	  /* mark all connected points above low thresh */
	  mark_connected (i, j, 0);
      }

  for (i = 0; i < nrows; i++)	/* erase all points which were 255 */
    for (j = 0; j < ncols; j++)
      if (edges->data2D[i][j] == 255)
	edges->data2D[i][j] = 0;
}

/*	return true if it marked something */
int
CFEDCShenCastan::mark_connected (int i, int j, int level)
{
  int notChainEnd;

  /* stop if you go off the edge of the image */
  if (i >= nr || i < 0 || j >= nc || j < 0)
    return 0;

  /* stop if the point has already been visited */
  if (edges->data2D[i][j] != 0)
    return 0;

  /* stop when you hit an image boundary */
  if (lap[i][j] == 0.0)
    return 0;

  if ((lap[i][j]) > low_thresh)
    {
      edges->data2D[i][j] = 1;
    }
  else
    {
      edges->data2D[i][j] = 255;
    }

  notChainEnd = 0;

  notChainEnd |= mark_connected (i, j + 1, level + 1);
  notChainEnd |= mark_connected (i, j - 1, level + 1);
  notChainEnd |= mark_connected (i + 1, j + 1, level + 1);
  notChainEnd |= mark_connected (i + 1, j, level + 1);
  notChainEnd |= mark_connected (i + 1, j - 1, level + 1);
  notChainEnd |= mark_connected (i - 1, j - 1, level + 1);
  notChainEnd |= mark_connected (i - 1, j, level + 1);
  notChainEnd |= mark_connected (i - 1, j + 1, level + 1);

  if (notChainEnd && (level > 0))
    {
      /* do some contour thinning */
      if (thinFactor > 0)
	if ((level % thinFactor) != 0)
	  {
	    /* delete this point */
	    edges->data2D[i][j] = 255;
	  }
    }
  return 1;
}

/* finds zero-crossings in laplacian (buff)  orig is the smoothed image */
int
CFEDCShenCastan::is_candidate_edge (TCMatriz2D< int > * &buff, float **orig, int row,
				    int col)
{
/* test for zero-crossings of laplacian then make sure that zero-crossing */
/* sign correspondence principle is satisfied.  i.e. a positive z-c must */
/* have a positive 1st derivative where positive z-c means the 2nd deriv */
/* goes from positive to negative as we pass through the step edge */

  if (buff->data2D[row][col] == 1 && buff->data2D[row + 1][col] == 0)	/* positive z-c */
    {
      if (orig[row + 1][col] - orig[row - 1][col] > 0)
	return 1;
      else
	return 0;
    }
  else if (buff->data2D[row][col] == 1 && buff->data2D[row][col + 1] == 0)	/* positive z-c */
    {
      if (orig[row][col + 1] - orig[row][col - 1] > 0)
	return 1;
      else
	return 0;
    }
  else if (buff->data2D[row][col] == 1 && buff->data2D[row - 1][col] == 0)	/* negative z-c */
    {
      if (orig[row + 1][col] - orig[row - 1][col] < 0)
	return 1;
      else
	return 0;
    }
  else if (buff->data2D[row][col] == 1 && buff->data2D[row][col - 1] == 0)	/* negative z-c */
    {
      if (orig[row][col + 1] - orig[row][col - 1] < 0)
	return 1;
      else
	return 0;
    }
  else				/* not a z-c */
    return 0;
}

float
CFEDCShenCastan::compute_adaptive_gradient (TCMatriz2D< int > * &BLI_buffer,
					    float **orig_buffer, int row,
					    int col)
{
  register int i, j;

  float sum_on, sum_off;

  float avg_on, avg_off;

  int num_on, num_off;

  sum_on = sum_off = 0.0;

  num_on = num_off = 0;

  for (i = (-window_size / 2); i <= (window_size / 2); i++)
    {
      for (j = (-window_size / 2); j <= (window_size / 2); j++)
	{
	  if (BLI_buffer->data2D[row + i][col + j])
	    {
	      sum_on += orig_buffer[row + i][col + j];
	      num_on++;
	    }
	  else
	    {
	      sum_off += orig_buffer[row + i][col + j];
	      num_off++;
	    }
	}
    }

  if (sum_off)
    avg_off = sum_off / num_off;
  else
    avg_off = 0;

  if (sum_on)
    avg_on = sum_on / num_on;
  else
    avg_on = 0;

  return (avg_off - avg_on);
}

void
CFEDCShenCastan::estimate_thresh (double *low, double *hi, int nr, int nc)
{
  float vmax, vmin, scale, x;

  int i, j, k, hist[256], count;

  /* Build a histogram of the Laplacian image. */
  // Calcula o histograma da imagem laplaciana
  // lap é a matriz laplaciano [aqui]
  vmin = vmax = fabs ((float) (lap[20][20]));

  for (i = 0; i < nr; i++)
    for (j = 0; j < nc; j++)
      {
	if (i < OUTLINE_SC || i >= nr - OUTLINE_SC || j < OUTLINE_SC
	    || j >= nc - OUTLINE_SC)
	  continue;
	x = lap[i][j];
	if (vmin > x)
	  vmin = x;
	if (vmax < x)
	  vmax = x;
      }

  for (k = 0; k < 256; k++)
    hist[k] = 0;

  scale = 256.0 / (vmax - vmin + 1);

  for (i = 0; i < nr; i++)
    for (j = 0; j < nc; j++)
      {
	if (i < OUTLINE_SC || i >= nr - OUTLINE_SC || j < OUTLINE_SC
	    || j >= nc - OUTLINE_SC)
	  continue;
	x = lap[i][j];
	k = (int) ((x - vmin) * scale);
	hist[k] += 1;
      }

  /* The high threshold should be > 80 or 90% of the pixels */
  k = 255;

  j = (int) (ratio * nr * nc);

  count = hist[255];

  while (count < j)
    {
      k--;
      if (k < 0)
	break;
      count += hist[k];
    }

  *hi = (double) k / scale + vmin;

  *low = (*hi) / 2;
}

// conferir:
// Cria matriz temporaria, maior que a passada, realiza procedimento
// de cálculo. Depois,
// apaga a matriz passada e iguala ponteiro im a matriz temporaria
void
CFEDCShenCastan::embed (TCMatriz2D< int > * &im, int width)
{
  unsigned int i, j;

  int I, J;

  TCMatriz2D< int > *nova;

  width += 2;

  nova =
    new TCMatriz2D< int > (im->NX () + width + width,
		   im->NY () + width + width);
  for (i = 0; i < nova->NX (); i++)
    for (j = 0; j < nova->NY (); j++)
      {
	I = (i - width + im->NX ()) % im->NX ();
	J = (j - width + im->NY ()) % im->NY ();
	nova->data2D[i][j] = im->data2D[I][J];
      }

  delete im; // deleta objeto imagem auxiliar
  im = nova;
}

void
CFEDCShenCastan::debed (TCMatriz2D< int > * &im, int width)
{
  TCMatriz2D< int > *old;

  width += 2;

  old =
    new TCMatriz2D< int > (im->NX () - width - width,
		   im->NY () - width - width);
  for (unsigned int i = 0; i < old->NX () - 1; i++)
    {
      for (unsigned int j = 1; j < old->NY (); j++)
	{
	  old->data2D[i][j] = im->data2D[i + width][j + width];
	  old->data2D[old->NX () - 1][j] = 255;
	}
      old->data2D[i][0] = 255;
    }

    delete im;    // deleta objeto imagem auxiliar
  im = old;
}

