
#include <cassert>

#include <MetNum/Interpolacao/CIntNewtonDiff.h>
/*
Função de cálculo dos valores auxiliares,
observe que são independentes do valor de x, usam
apenas os vetores x e y.
*/
void
CIntNewtonDiff::GeraTabelaAuxiliar (double h, double *yarr, int n)
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

      //Calculo do vetor diffTable (tabela auxiliar) usa diffTable, h e yarr
      //Copia yarr para tabela
      for (i = 0; i < n; i++)
	*(diffTable + i) = *(yarr + i);
      //Até n-1
      for (i = 0; i < (n - 1); i++)
	{
	  //diferença entre CIntNewtonDiff e CIntNewtonDivDiff a linha abaixo
	  for (j = n - 1; j > i; j--)
	    *(diffTable + j) = (*(diffTable + j - 1) - *(diffTable + j))
	      / (-h * (1 + i));
	}
      //fas buildMatFlag=false (não necessita refazer)
      buildMatFlag = 0;
    }
}

//Função de cálculo em sí.
//Observe que cria duas variáveis x0 e h usadas pelo método original
double
CIntNewtonDiff::Go (double *xarr, double *yarr, int n, double x)
{
  //Pedaço usado para criar as variáveis x0 e h, usadas pelo método
  //original de cálculo.
  if (n < 2)
    return 0;			//Evita acesso a xarr[1] que não existe
  double x0 = xarr[0];
  double h = xarr[1] - xarr[0];

  //Se necessário chama função de cálculo da tabela auxiliar
  if (buildMatFlag)
    GeraTabelaAuxiliar (h, yarr, n);

  //Resultado da interpolação
  double yInterpolado;

  //Inicializa valor interpolado com o valor n-1 da tabela auxiliar
  yInterpolado = *(diffTable + n - 1);

  int i, j;
  //diferença entre CIntNewtonDiff e CIntNewtonDivDiff a linha abaixo
  for (i = n - 2; i >= 0; i--)
    yInterpolado = *(diffTable + i) + (x - (x0 + i * h)) * yInterpolado;

  return yInterpolado;
};

//Função original do C.Shammas
//Em ves de receber o vetor xarr recebe um x0 e h
//foi definida como estatica
double
CIntNewtonDiff::NewtonDiff (double x0, double h,
			    double *yarr,
			    double *diffTable, int buildMatFlag,
			    int n, double x)
{
  double yint;
  int i, j;
  //Calcula a tabela auxiliar
  if (buildMatFlag)
    {
      for (i = 0; i < n; i++)
	*(diffTable + i) = *(yarr + i);
      for (i = 0; i < (n - 1); i++)
	{
	  for (j = n - 1; j > i; j--)
	    *(diffTable + j) = (*(diffTable + j - 1) - *(diffTable + j))
	      / (-h * (1 + i));
	}
    }
  // initialize interpolated value
  yint = *(diffTable + n - 1);
  for (i = n - 2; i >= 0; i--)
    yint = *(diffTable + i) + (x - (x0 + i * h)) * yint;	//diferente entre os dois metodos
  return yint;
}
