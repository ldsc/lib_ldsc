// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
	      		Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:      	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:         	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEDCCanny.cpp
Nome da classe:      CFEDCCanny
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
----------------------------------------------------------------------------
		       	BIBLIOTECAS
----------------------------------------------------------------------------
*/
#include <cmath>
#include <cstdio>
using namespace std;

#include "Filtro/FEspacial/FEDeteccaoContorno/CFEDCCanny.h"

// #include "TMascara\TMPassaBaixa.h"     // O filtro espacial atua sobre a mascara                                                                               // do passa baixa todos os valores=1
       	// outros filtros usam outras mascaras.
/*
==================================================================================
Documentacao      Go
==================================================================================
Descrição:
Programador:      Andre Duarte Bueno
*/

/*
TCMatriz2D< int >* CFEDCCanny::Go( TCMatriz2D< int > *& matriz, unsigned int _tamanhoMascara )
{
	tamanhoMascara=_tamanhoMascara;
   return   Go(matriz);
} */

TCMatriz2D< int > *
CFEDCCanny::Go (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara)
{
  // CriaMascara(tamanhoMascara);
  pm = matriz;

  // CImagem* im=dynamic_cast< CImagem* > (pm);
  CImagem *im = new CImagem (*pm);	// cria imagem nova copia pm e acrescenta dados

  unsigned int i, j;		// ,k,n;

  CImagem *magim;

  CImagem *oriim;

  // Cria imagem local
  magim = new CImagem (im->NX (), im->NY ());

  if (magim == NULL)
    {				// cout<<"Out of storage: Magnitude\n";
      exit (1);
    }

  oriim = new CImagem (im->NX (), im->NY ());
  if (oriim == NULL)
    {				// cout<<"Out of storage: Orientation\n";
      exit (1);
    }

  canny (s, im, magim, oriim);	// aplica o filtro
 
 hysteresis (high, low, im, magim, oriim);	// Hysteresis thresholding of edge pixels
 
 for (i = 0; i < WIDTH; i++)	// WIDTH é calculada por canny
    for (j = 0; j < im->NY (); j++)
      im->data2D[i][j] = 255;

  for (i = im->NX () - 1; i > im->NX () - 1 - WIDTH; i--)
    for (j = 0; j < im->NY (); j++)
      im->data2D[i][j] = 255;

  for (i = 0; i < im->NX (); i++)
    for (j = 0; j < WIDTH; j++)
      im->data2D[i][j] = 255;

  for (i = 0; i < im->NX (); i++)
    for (j = im->NY () - WIDTH - 1; j < im->NY (); j++)
      im->data2D[i][j] = 255;

  // im->Write("canny.m2");
  // fim do codigo do filtro novo
  // delete pm;                    // apaga objeto matriz inicial
  // pm=dynamic_cast< TCMatriz2D< int >* > (im);   // converte a imagem para TCMatriz2D< int >
  // funciona porque im é maior que pm
  // 
  for (j = 0; j < im->NY (); j++)
    for (i = 0; i < im->NX (); i++)
      pm->data2D[i][j] = im->data2D[i][j];

  return pm;
}

/*
==================================================================================
Documentacao      CriaMascara
==================================================================================
Descrição:	Se a mascara ja existe deve ser eliminada
Programador:      Andre Duarte Bueno

void   CFEDCCanny::CriaMascara(unsigned int _tamanhoMascara)
{
 tamanhoMascara=_tamanhoMascara;
 if(mask)
 	delete mask;
 mask=new TMEmboss(_tamanhoMascara);
}
*/


float
CFEDCCanny::norm (float x, float y)
{
  return (float) sqrt ((double) (x * x + y * y));
}

void
CFEDCCanny::canny (float s, CImagem * im, CImagem * mag, CImagem * ori)
{
  int width;

  float **smx, **smy;

  // TCMatriz2D< int >*smx,smy;

  float **dx, **dy;

  // TCMatriz2D< int >*dx,dy;

  int i, j;			// ,k;

  // int n;
  float gau[MAX_MASK_SIZE], dgau[MAX_MASK_SIZE], z;

/* Create a Gaussian and a derivative of Gaussian filter mask */
  for (i = 0; i < MAX_MASK_SIZE; i++)
    {
      gau[i] = meanGauss ((float) i, s);
      if (gau[i] < 0.005)
	{
	  width = i;
	  break;
	}
      dgau[i] = dGauss ((float) i, s);
    }

  // n = width+width + 1;
  WIDTH = width / 2;
  // printf ("Smoothing with a Gaussian (width = %d) ...\n", n);

  smx = f2d (im->NX (), im->NY ());

  // TCMatriz2D< int >*smx=new TCMatriz2D< int >(nr,nc);
  smy = f2d (im->NX (), im->NY ());

/* Convolution of source image with a Gaussian in X and Y directions  */
  seperable_convolution (im, gau, width, smx, smy);

/* Now convolve smoothed data2D with a derivative */
  // printf ("Convolution with the derivative of a Gaussian...\n");
  dx = f2d (im->NX (), im->NY ());

  dxy_seperable_convolution (smx, im->NX (), im->NY (), 
			     dgau, width, dx,    1);
  // delete smx;

  free (smx[0]);

  free (smx);

  dy = f2d (im->NX (), im->NY ());

  dxy_seperable_convolution (smy, im->NX (), im->NY (), 
			     dgau, width, dy,    0);
  // delete smy
  free (smy[0]);

  free (smy);

/* Create an image of the norm of dx,dy */
  for (i = 0; i < im->NX (); i++)
    for (j = 0; j < im->NY (); j++)
      {
	z = norm (dx[i][j], dy[i][j]);
	mag->data2D[i][j] = (unsigned char) (z * MAG_SCALE);
      }

/* Non-maximum suppression - edge pixels should be a local max */
  nonmax_suppress (dx, dy, (int) im->NX (), (int) im->NY (), mag, ori);
  free (dx[0]);

  free (dx);

  free (dy[0]);

  free (dy);
}

/*      Gaussian        */
float
CFEDCCanny::gauss (float x, float sigma)
{
  float xx;

  if (sigma == 0)
    return 0.0;

  xx = (float) exp ((double) ((-x * x) / (2 * sigma * sigma)));

  return xx;
}

float
CFEDCCanny::meanGauss (float x, float sigma)
{
  float z;

  z =
    (gauss (x, sigma) + gauss (x + 0.5, sigma) +
     gauss (x - 0.5, sigma)) / 3.0;

  z = z / (M_PI * 2.0 * sigma * sigma);
  //  z = z / (PI * 2.0 * sigma * sigma);

  return z;
}

/*      First derivative of Gaussian    */
float
CFEDCCanny::dGauss (float x, float sigma)
{
  return -x / (sigma * sigma) * gauss (x, sigma);
}

/*      HYSTERESIS thersholding of edge pixels. Starting at pixels with a
	value greater than the HIGH threshold, trace a connected sequence
	of pixels that have a value greater than the LOW threhsold.        */

void
CFEDCCanny::hysteresis (int high, int low, CImagem * im, CImagem * mag,
			CImagem * oriim)
{
  int i, j;			// ,k;

  // printf ("Beginning hysteresis thresholding...\n");
  for (i = 0; i < im->NX (); i++)
    for (j = 0; j < im->NY (); j++)
      im->data2D[i][j] = 0;

  if (high < low)
    {
      estimate_thresh (mag, &high, &low);
      // printf ("Hysteresis thresholds (from image): HI %d LOW %D\n",       high, low);
    }

/* For each edge with a magnitude above the high threshold, begin
   tracing edge pixels that are above the low threshold.                */

  for (i = 0; i < im->NX (); i++)
    for (j = 0; j < im->NY (); j++)
      if (mag->data2D[i][j] >= high)
	trace (i, j, low, im, mag, oriim);

/* Make the edge black (to be the same as the other methods) */
  for (i = 0; i < im->NX (); i++)
    for (j = 0; j < im->NY (); j++)
      if (im->data2D[i][j] == 0)
	im->data2D[i][j] = 255;
      else
	im->data2D[i][j] = 0;
}

/*      TRACE - recursively trace edge pixels that have a threshold > the low
	edge threshold, continuing from the pixel at (i,j).                     */

int
CFEDCCanny::trace (int i, int j, int low, CImagem * im, CImagem * mag,
		   CImagem * ori)
{
  int n, m;

  char flag = 0;		// usado para encerrar o for

  if (im->data2D[i][j] == 0)
    {
      im->data2D[i][j] = 255;

      flag = 0;

      for (n = -1; n <= 1; n++)
	{
	  for (m = -1; m <= 1; m++)
	    {
	      if (i == 0 && m == 0)
		continue;
	      // abaixo acrescentei a função contem, era range (testar)
	      if (mag->Contem (i + n, j + m)
		  && mag->data2D[i + n][j + m] >= low)
		if (trace (i + n, j + m, low, im, mag, ori))
		  {
		    flag = 1;
		    break;
		  }
	    }
	  if (flag)		// se flag=true encerra for
	    break;
	}
      return (1);
    }
  return (0);
}

void
CFEDCCanny::seperable_convolution (CImagem * im, float *gau, int width,
				   float **smx, float **smy)
      //  TCMatriz2D< int >*smx,TCMatriz2D< int >*smy)
{
  int i, j, k, I1, I2, nr, nc;

  float x, y;

  nr = im->NX ();

  nc = im->NY ();

  for (i = 0; i < nr; i++)
    for (j = 0; j < nc; j++)
      {
	x = gau[0] * im->data2D[i][j];

	y = gau[0] * im->data2D[i][j];

	for (k = 1; k < width; k++)
	  {
	    I1 = (i + k) % nr;
	    I2 = (i - k + nr) % nr;
	    y += gau[k] * im->data2D[I1][j] + gau[k] * im->data2D[I2][j];
	    I1 = (j + k) % nc;
	    I2 = (j - k + nc) % nc;
	    x += gau[k] * im->data2D[i][I1] + gau[k] * im->data2D[i][I2];
	  }

	smx[i][j] = x;

	smy[i][j] = y;
	// smx->data2D[i][j]...
      }
}

void
CFEDCCanny::dxy_seperable_convolution (float **im, int nr, int nc, float *gau,
				       int width, float **sm, int which)
{
  int i, j, k, I1, I2;

  float x;

  for (i = 0; i < nr; i++)
    for (j = 0; j < nc; j++)
      {
	x = 0.0;
	for (k = 1; k < width; k++)
	  {
	    if (which == 0)
	      {
		I1 = (i + k) % nr;
		I2 = (i - k + nr) % nr;
		x += -gau[k] * im[I1][j] + gau[k] * im[I2][j];
	      }
	    else
	      {
		I1 = (j + k) % nc;
		I2 = (j - k + nc) % nc;
		x += -gau[k] * im[i][I1] + gau[k] * im[i][I2];
	      }
	  }
	sm[i][j] = x;
      }
}

// Obs: nr e nc não são utilizados
void
CFEDCCanny::nonmax_suppress (float **dx, float **dy, int nr, int nc,
			     CImagem * mag, CImagem * ori)
{
  int i, j;			// ,k,n,m;

  // int  bottom;// ,top, left, right;
  float xx, yy, g2, g1, g3, g4, g, xc, yc;

  for (i = 1; i < mag->NX () - 1; i++)
    {
      for (j = 1; j < mag->NY () - 1; j++)
	{
	  mag->data2D[i][j] = 0;

/* Treat the x and y derivatives as components of a vector */
	  xc = dx[i][j];
	  yc = dy[i][j];
	  if (fabs (xc) < 0.01 && fabs (yc) < 0.01)
	    continue;

	  g = norm (xc, yc);

/* Follow the gradient direction, as indicated by the direction of
   the vector (xc, yc); retain pixels that are a local maximum. */

	  if (fabs (yc) > fabs (xc))
	    {

/* The Y component is biggest, so gradient direction is basically UP/DOWN */
	      xx = fabs (xc) / fabs (yc);

	      yy = 1.0;

	      g2 = norm (dx[i - 1][j], dy[i - 1][j]);

	      g4 = norm (dx[i + 1][j], dy[i + 1][j]);

	      if ( (xc * yc) > 0.0)
		{
		  g3 = norm (dx[i + 1][j + 1], dy[i + 1][j + 1]);
		  g1 = norm (dx[i - 1][j - 1], dy[i - 1][j - 1]);
		}
	      else
		{
		  g3 = norm (dx[i + 1][j - 1], dy[i + 1][j - 1]);
		  g1 = norm (dx[i - 1][j + 1], dy[i - 1][j + 1]);
		}

	    }
	  else
	    {

/* The X component is biggest, so gradient direction is basically LEFT/RIGHT */
	      xx = fabs (yc) / fabs (xc);

	      yy = 1.0;

	      g2 = norm (dx[i][j + 1], dy[i][j + 1]);
	      g4 = norm (dx[i][j - 1], dy[i][j - 1]);
	      if ((xc * yc) > 0.0)
		{
		  g3 = norm (dx[i - 1][j - 1], dy[i - 1][j - 1]);
		  g1 = norm (dx[i + 1][j + 1], dy[i + 1][j + 1]);
		}
	      else
		{
		  g1 = norm (dx[i - 1][j + 1], dy[i - 1][j + 1]);
		  g3 = norm (dx[i + 1][j - 1], dy[i + 1][j - 1]);
		}
	    }

/* Compute the interpolated value of the gradient magnitude */
	  if ((g > (xx * g1 + (yy - xx) * g2)) &&
	      (g > (xx * g3 + (yy - xx) * g4)))
	    {
	      if ( (g * MAG_SCALE) <= 255)
		mag->data2D[i][j] = (unsigned char) (g * MAG_SCALE);
	      else
		mag->data2D[i][j] = 255;
	      ori->data2D[i][j] = atan2 (yc, xc) * ORI_SCALE;
	    }
	  else
	    {
	      mag->data2D[i][j] = 0;
	      ori->data2D[i][j] = 0;
	    }

	}
    }
}

void
CFEDCCanny::estimate_thresh (CImagem * mag, int *hi, int *low)
{
  int i, j, k, hist[256], count;

/* Build a histogram of the magnitude image. */
  for (k = 0; k < 256; k++)
    hist[k] = 0;

  for (i = WIDTH; i < mag->NX () - WIDTH; i++)
    for (j = WIDTH; j < mag->NY () - WIDTH; j++)
      hist[mag->data2D[i][j]]++;

/* The high threshold should be > 80 or 90% of the pixels
	j = (int)(ratio*mag->NX()*mag->NY());
*/
  j = mag->NX ();
  if (j < mag->NY ())
    j = mag->NY ();
  j = (int) (0.9 * j);
  k = 255;

  count = hist[255];
  while (count < j)
    {
      k--;
      if (k < 0)
	break;
      count += hist[k];
    }
  *hi = k;

  i = 0;
  while (hist[i] == 0)
    i++;

  *low = (*hi + i) / 2.0;
}

