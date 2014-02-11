
#include <cassert>

#include <MetNum/Interpolacao/CIntNewtonDivDiff.h>

/*
Função de calculo dos valores auxiliares,
observe que são independentes do valor de x, usam
apenas os vetores x e y.
*/
void
CIntNewtonDivDiff::GeraTabelaAuxiliar (double *xarr, double *yarr, int n)
{
//se buildMatFlag=1 monta a tabela auxiliar
  if (buildMatFlag)
    {
      int i, j;

      //Aloca o vetor da tabela auxiliar (usar vector)
      if (diffTable != 0)
	delete [] diffTable;
      diffTable = new double [n];
      assert (diffTable);

      //Calculo do vetor diffTable (tabela auxiliar)
      //Copia yarr para tabela
      for (i = 0; i < n; i++)
	*(diffTable + i) = *(yarr + i);

      //Até n-1
      for (i = 0; i < (n - 1); i++)
	{
	  //diferença entre CIntNewtonDiff e CIntNewtonDivDiff a linha abaixo
	  for (j = n - 1; j > i; j--)
	    *(diffTable + j) = (*(diffTable + j - 1) - *(diffTable + j))
	      / (*(xarr + j - 1 - i) - *(xarr + j));
	}
      //fas buildMatFlag=false (não necessita refazer)
      buildMatFlag = 0;
    }
}

/*
Função de calculo em sí
se necessário, chama antes o calculo dos valores auxiliares
*/
double
CIntNewtonDivDiff::Go (double *xarr, double *yarr, int n, double x)
{
  if (buildMatFlag)
    GeraTabelaAuxiliar (xarr, yarr, n);

  //resultado da interpolação
  double yInterpolado;

  int i, j;

  //Inicializa valor interpolado com o valor n-1 da tabela auxiliar
  yInterpolado = *(diffTable + n - 1);

  //De n-2 até 0
  //faz yInterpolado =  valor da tabela + dx * yInterpolado
  for (i = n - 2; i >= 0; i--)
    //diferença entre CIntNewtonDiff e CIntNewtonDivDiff a linha abaixo
    yInterpolado = *(diffTable + i) + (x - *(xarr + i)) * yInterpolado;

  return yInterpolado;
};


//Abaixo a Função original
//foi definida como estatica
//see Kohn, page 140
double
CIntNewtonDivDiff::NewtonDivDiff
  (double *xarr, double *yarr,
   double *diffTable, int buildMatFlag, int n, double x)
{
  double    yInterpolado;
  int    i,    j;
  if (buildMatFlag)
    {
      for (i = 0; i < n; i++)
	*(diffTable + i) = *(yarr + i);

      for (i = 0; i < (n - 1); i++)
	{
	  for (j = n - 1; j > i; j--)
	    *(diffTable + j) = (*(diffTable + j - 1) - *(diffTable + j))
	      / (*(xarr + j - 1 - i) - *(xarr + j));
	}
    }
  yInterpolado = *(diffTable + n - 1);

  for (i = n - 2; i >= 0; i--)
    yInterpolado = *(diffTable + i) + (x - *(xarr + i)) * yInterpolado;

  return yInterpolado;
}
