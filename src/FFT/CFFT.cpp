
/*--------------------------------*-C-*---------------------------------*
   Arquivo CFFT.cpp, antigo arquivo fftn.c encapsulado..
           Abaixo os comentários do codigo original.
           Como o arquivo utiliza uma serie de #defines para
           tomada de decisao durante a compilação, este arquivo não deve ser alterado.
           Se desejar fazer alguma alteração, faça uma cópia.

 //Documentação original
 * File:	fftn.c
 *
 * Funcoes publicas:
 *	fft_free ();   //libera memoria alocada
 *	fftn();			//tranformada de fourier para matrizes do tipo double
 * fftnf ();      //tranformada de fourier para matrizes do tipo float
 *
 * Private:
 *	fftradix();    //funções auxiliares utilizadas (para double)
 *	fftradixf();   //funções auxiliares utilizadas (para float)
 *
 * Descricao:
 *	multivariate complex Fourier transform, computed in place
 *	using mixed-radix Fast Fourier Transform algorithm.
 *
 *	Fortran code by:
 *	RC Singleton, Stanford Research Institute, Sept. 1968
 *
 *	translated by f2c (version 19950721).
 * O codigo foi transcrito do fortram para a linguagem C pelo programa f2c.
 *
 * Revisoes:
 * 26 July 95	John Beale
 *	- added maxf and maxp as parameters to fftradix()
 *
 * 28 July 95	Mark Olesen <olesen@me.queensu.ca>
 *	- cleaned-up the Fortran 66 goto spaghetti, only 3 labels remain.
 *
 *	- added fft_free() to provide some measure of control over
 *	  allocation/deallocation.
 *
 *	- added fftn() wrapper for multidimensional FFTs
 *
 *	- use -DFFT_NOFLOAT or -DFFT_NODOUBLE to avoid compiling that
 *	  precision. Note suffix 'f' on the function names indicates
 *	  float precision.
 *
 *	- revised documentation
 *
 * 31 July 95	Mark Olesen <olesen@me.queensu.ca>
 *	- added GNU Public License
 *	- more cleanup
 *	- define SUN_BROKEN_REALLOC to use malloc() instead of realloc()
 *	  on the first pass through, apparently needed for old libc
 *	- removed #error directive in favour of some code that simply
 *	  won't compile (generate an error that way)
 *
 * 1 Aug 95	Mark Olesen <olesen@me.queensu.ca>
 *	- define FFT_RADIX4 to only have radix 2, radix 4 transforms
 *	- made fftradix /fftradixf () static scope, just use fftn()
 *	  instead.  If you have good ideas about fixing the factors
 *	  in fftn() please do so.
 *
 * 8 Jan 95	mj olesen <olesen@me.queensu.ca>
 *	- fixed typo's, including one that broke scaling for scaling by
 *	  total number of matrix elements or the square root of same
 *	- removed unnecessary casts from allocations
 *
 * ============================================================================*
 * NIST Guide to Available Math Software.
 * Source for module FFT from package GO.
 * Retrieved from NETLIB on Wed Jul  5 11:50:07 1995.
 * ============================================================================*
 *
 *-----------------------------------------------------------------------*
 *Prototipo da funcao:
 * int fftn (int ndim, const int dims[], REAL Re[], REAL Im[],
 *	    int iSign, double scaling);

 *Parametros:
 * NDIM = the total number dimensions
 * DIMS = a vector of array sizes
 *	if NDIM is zero then DIMS must be zero-terminated
 *
 * RE and IM hold the real and imaginary components of the data, and return
 * the resulting real and imaginary Fourier coefficients.  Multidimensional
 * data *must* be allocated contiguously.  There is no limit on the number
 * of dimensions.
 *
 * ISIGN = the sign of the complex exponential (ie, forward ISIGN=1 or inverse FFT ISIGN=-1)
 *	the magnitude of ISIGN (normally 1) is used to determine the
 *	correct indexing increment (see below).
 *
 * SCALING = normalizing constant by which the final result is *divided*
 *	if SCALING == -1, normalize by total dimension of the transform
 *	if SCALING <  -1, normalize by the square-root of the total dimension
 *
 * Exemplo:
 * tri-variate transform with Re[n1][n2][n3], Im[n1][n2][n3]
 *
 *	int dims[3] = {n1,n2,n3}
 *	fftn (3, dims, Re, Im, 1, scaling);
 *
 *-----------------------------------------------------------------------*
 * int fftradix (REAL Re[], REAL Im[], size_t nTotal, size_t nPass,
 *		 size_t nSpan, int iSign, size_t max_factors,
 *		 size_t max_perm);
 *
 * RE, IM - see above documentation
 *
 * Although there is no limit on the number of dimensions, fftradix() must
 * be called once for each dimension, but the calls may be in any order.
 *
 * NTOTAL = the total number of complex data values
 * NPASS  = the dimension of the current variable
 * NSPAN/NPASS = the spacing of consecutive data values while indexing the
 *	current variable
 * ISIGN - see above documentation
 *
 * example:
 * tri-variate transform with Re[n1][n2][n3], Im[n1][n2][n3]
 *
 *	fftradix (Re, Im, n1*n2*n3, n1,       n1, 1, maxf, maxp);
 *	fftradix (Re, Im, n1*n2*n3, n2,    n1*n2, 1, maxf, maxp);
 *	fftradix (Re, Im, n1*n2*n3, n3, n1*n2*n3, 1, maxf, maxp);
 *
 * single-variate transform,
 *    NTOTAL = N = NSPAN = (number of complex data values),
 *
 *	fftradix (Re, Im, n, n, n, 1, maxf, maxp);
 *
 * The data can also be stored in a single array with alternating real and
 * imaginary parts, the magnitude of ISIGN is changed to 2 to give correct
 * indexing increment, and data [0] and data [1] used to pass the initial
 * addresses for the sequences of real and imaginary values,
 *
 * Example:
 *	REAL data [2*NTOTAL];
 *	fftradix ( &data[0], &data[1], NTOTAL, nPass, nSpan, 2, maxf, maxp);
 *
 * for temporary allocation:
 *
 * MAX_FACTORS	>= the maximum prime factor of NPASS
 * MAX_PERM	>= the number of prime factors of NPASS.  In addition,
 *	if the square-free portion K of NPASS has two or more prime
 *	factors, then MAX_PERM >= (K-1)
 *
 * storage in FACTOR for a maximum of 15 prime factors of NPASS. if NPASS
 * has more than one square-free factor, the product of the square-free
 * factors must be <= 210 array storage for maximum prime factor of 23 the
 * following two constants should agree with the array dimensions.
 *
 *-----------------------------------------------------------------------*
 * void fft_free ();
 * free-up allocated temporary storage after finished all the Fourier
 * transforms.
 *----------------------------------------------------------------------*/

//------------------------------------------------------------------
//Bibliotecas
//------------------------------------------------------------------
//#include <stdio.h>
#include <cstdlib>		//por causa free
#include <iostream>		//por causa cerr cout
#include <cmath>

//------------------------------------------------------------------
//Bibliotecas LDSC
//------------------------------------------------------------------
#include    "FFT/CFFT.h"	//classe

//-----------------------------------------------
//Parametros utilizados para controle de memoria
//-----------------------------------------------
static size_t SpaceAlloced = 0;
static size_t MaxPermAlloced = 0;

/* temp space, (void *) since both float and double routines use it */
static void *Tmp0 = NULL;	/* temp space for real part */
static void *Tmp1 = NULL;	/* temp space for imaginary part */
static void *Tmp2 = NULL;	/* temp space for Cosine values */
static void *Tmp3 = NULL;	/* temp space for Sine values */
static int *Perm = NULL;	/* Permutation vector */

#define NFACTOR	11
static int factor[NFACTOR];

/*
#if !defined (__FILE__) && !defined (lint)
Error: your compiler is sick!  define __FILE__ yourself (a string)
eg, something like -D__FILE__=\"fftn.c\"
#endif   */

//define variaveis matematicas
#ifndef M_PI
# define M_PI	3.14159265358979323846264338327950288
#endif

#ifndef SIN60
# define SIN60	0.86602540378443865	/* sin(60 deg) */
# define COS72	0.30901699437494742	/* cos(72 deg) */
# define SIN72	0.95105651629515357	/* sin(72 deg) */
#endif


#if defined (FFT_NOFLOAT) && defined (FFT_NODOUBLE) && !defined (lint)
Error:cannot have both - DFFT_NOFLOAT and - DFFT_NODOUBLE
#endif
      //#else	/* _FFTN_C */
//--------------------------------------------------
//Implementação das funções
//--------------------------------------------------
//Construtor
//--------------------------------------------------
CFFT::CFFT (int _ndim, int *_dims, double _scaling = -1)
{
//antigos
  ndim = _ndim;
  dims = _dims;
  scaling = _scaling;
/*
//novos	
  //de fato serão utilizados pela classe filha
if(_ndim>0)  NX=dims[0];
if(_ndim>1)  NY=dims[1];
if(_ndim>2)  NZ=dims[2];
*/
}

//--------------------------------------------------
//Destrutor
//--------------------------------------------------
//libera memoria alocada
CFFT::~CFFT ()
{
  fft_free ();
}

//libera memoria alocada
int
CFFT::fft_free ()
{
  SpaceAlloced = MaxPermAlloced = 0;
  if (Tmp0 != NULL)
    {
      free (Tmp0);
      Tmp0 = NULL;
    }
  if (Tmp1 != NULL)
    {
      free (Tmp1);
      Tmp1 = NULL;
    }
  if (Tmp2 != NULL)
    {
      free (Tmp2);
      Tmp2 = NULL;
    }
  if (Tmp3 != NULL)
    {
      free (Tmp3);
      Tmp3 = NULL;
    }
  if (Perm != NULL)
    {
      free (Perm);
      Perm = NULL;
    }
  return 1;
}


//Chama a função de calculo da transformada de Fourier formato double
int
CFFT::Go (double *real, double *imaginaria)
{
  iSign = 1;
  //fftn (int ndim, const int dims[], double Re[], double Im[], int isign, double scaling);
  return fftn (ndim, dims, real, imaginaria, iSign, scaling);
}

//Chama a função de calculo da transformada inversa de Fourier formato double
int
CFFT::Go_Inverse (double *real, double *imaginaria)
{
  iSign = -1;
  return fftn (ndim, dims, real, imaginaria, iSign, scaling);
}

//Chama a função de calculo da transformada de Fourier formato float
int
CFFT::Go (float *real, float *imaginaria)
{
  iSign = 1;
  //  return fftnf (ndim, dims, real, imaginaria, iSign, scaling);
  return fftn (ndim, dims, real, imaginaria, iSign, scaling);
}

//Chama a função de calculo da transformada inversa de Fourier formato float
int
CFFT::Go_Inverse (float *real, float *imaginaria)
{
  iSign = -1;
  //  return fftnf (ndim, dims, real, imaginaria, iSign, scaling);
return fftn (ndim, dims, real, imaginaria, iSign, scaling);
}

//Na classe foram declaradas as funções   fftn e fftnf, uma versão para double e outra
//para double.
//Como a definição das funções é igual, só muda o tipo das variáveis, as funções foram
//escritas de modo genérico e armazenadas no arquivo funcoesAuxiliaresParaCFFT.cpp
//Abaixo seta algumas para compilar para float e inclue funcoesAuxiliaresParaCFFT.cpp
//depois seta as mesmas variaveis para double e inclue funcoesAuxiliaresParaCFFT.cpp
// futuramente atualizar para template??

//-----------------------------------------------------------------------
//versao para float (segunda passagem)
//-----------------------------------------------------------------------
#undef REAL
//aqui #undef FFTN
#undef FFTNS
//aqui #undef FFTRADIX
#undef FFTRADIXS

#ifndef FFT_NOFLOAT
# define REAL		float	//define para float
//aqui # define FFTN            fftnf                           /* trailing 'f' for float */
# define FFTNS		"fftnf"	/* name for error message */
//aqui # define FFTRADIX        fftradixf               /* trailing 'f' for float */
# define FFTRADIXS	"fftradixf"	/* name for error message */
#include    <FFT/FuncoesAuxiliaresParaCFFT.h>	/* inclue definição das funções */
#endif

//-----------------------------------------------------------------------
//versao para double (segunda passagem)
//-----------------------------------------------------------------------
#undef REAL
//aqui #undef FFTN
#undef FFTNS
//aqui #undef FFTRADIX
#undef FFTRADIXS

#ifndef FFT_NODOUBLE
# define REAL		double	//define para double
//aqui # define FFTN            fftn
# define FFTNS		"fftn"	//for messages
//aqui # define FFTRADIX        fftradix
# define FFTRADIXS	"fftradix"	//for messages
#include    <FFT/FuncoesAuxiliaresParaCFFT.h>	/* inclue definição das funções */
#endif
