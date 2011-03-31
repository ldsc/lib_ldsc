
#include <cassert>
#include <Interpolacao/CIntBarycentric.h>

//------------------------------------------------------------------------
//Função de calculo da tabela auxilliar
//------------------------------------------------------------------------
//Calculo do vetor de pesos
//Usa n, xarr
void
CIntBarycentric::GeraTabelaAuxiliar (double *xarr, int n)
{
  if ( calcWtFlag )
    {
      //Aloca o vetor da tabela auxiliar (usar vector)
      if (wt != 0)
	delete [] wt;
      wt = new double [n];
      assert (wt);

      //Calculo do vetor auxiliar wt
      double prod;
      int i, j;
      for (i = 0; i < n; i++)
	{
	  prod = 1;
	  for (j = 0; j < n; j++)
	    if (i != j)
	      prod *= (*(xarr + i) - *(xarr + j));
	  *(wt + i) = 1 / prod;
	}
      calcWtFlag = 0;		//não precisa calcular novamente
    }
}

//------------------------------------------------------------------------
//Função Go
//------------------------------------------------------------------------
double
CIntBarycentric::Go (double *xarr, double *yarr, int n, double x)
{
  if (calcWtFlag)
    GeraTabelaAuxiliar (xarr, n);

  double diff, sum1 = 0, sum2 = 0;
  int i, j;

  //loop for each term
  for (i = 0; i < n; i++)
    {
      diff = *(wt + i) / (x - *(xarr + i));
      sum1 += *(yarr + i) * diff;
      sum2 += diff;
    }
  return sum1 / sum2;
}

// Henrici  pages 237-238
//Abaixo a funcao original, é estatica na classe
double
CIntBarycentric::Barycentric
  (double *xarr, double *yarr, double *wt, int calcWtFlag, int n, double x)
{
  double     diff,    sum1 = 0, sum2 = 0;
  int    i,    j;

  //Calculo do vetor de pesos
  //Usa n, xarr
  double    prod;
  if (calcWtFlag)
    {
      for (i = 0; i < n; i++)
	{
	  prod = 1;
	  for (j = 0; j < n; j++)
	    if (i != j)
	      prod *= (*(xarr + i) - *(xarr + j));
	  *(wt + i) = 1 / prod;
	}
    }

  //  loop for each term
  for (i = 0; i < n; i++)
    {
      diff = *(wt + i) / (x - *(xarr + i));
      sum1 += *(yarr + i) * diff;
      sum2 += diff;
    }
  return sum1 / sum2;
}
