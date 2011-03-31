#include <cstdlib>
#include <cmath>

#include <Estatistica/CEstatistica.h>

using namespace std;

// Defines incluídos
#define Real_E1(x,y,z)	Re_data[(z)*NY*NX+(y)*NX+(x)]

#define Imag_E1(x,y,z)	Im_data[(z)*NY*NX+(y)*NX+(x)]

// Função de inicialização do objeto
void
CEstatistica::Inicializa (int _hasMissingData, double _missingCode)	// definir valores padrões
{
  hasMissingData = _hasMissingData;

  missingCode = _missingCode;

  numeroDados = 0;

  sum = 0.0;

  sumX = 0.0;

  sumX2 = 0.0;

  sumX3 = 0.0;

  sumX4 = 0.0;

  media = 0.0;

  desvioPadrao = 0.0;

  media = variancia = desvioPadrao = 0;
}

// Funções da classe CEstatistica
// Funções criadas para dar suporte ao formato do programa do Liang
// Posteriormente devem ser eliminadas
double
CEstatistica::Media (double *Re_data, int NX, int NY, int NZ)
{
  media = 0;
  for (int i = 0; i < (NX); i++)
    for (int j = 0; j < (NY); j++)
      for (int k = 0; k < NZ; k++)
	media += Real_E1 (i, j, k);
  media /= (NX * NY * NZ);

  return media;
}

// Verificar se esta calculando conceito corretamente
double 	CEstatistica::Covariancia (double *Re_data, int NX, int NY, int NZ)
{
	double temp;
	media = covariancia = 0;
	for (int i = 0; i < (NX); i++)
		for (int j = 0; j < (NY); j++)
			for (int k = 0; k < NZ; k++)
	{
		temp = Real_E1 (i, j, k);
		media += temp;
		covariancia += (temp * temp);
	}
	media /= (NX * NY * NZ);
	covariancia /= (NX * NY * NZ);
	covariancia -= (media * media);
	return covariancia;
}


// Obs: O que o Liang chamou de covariancia é outra coisa,
// esta função deve ser eliminada do objeto final
double
CEstatistica::Variancia (double *Re_data, int NX, int NY, int NZ)
{
  double temp;

  media = variancia = 0;

  for (int i = 0; i < (NX); i++)
    for (int j = 0; j < (NY); j++)
      for (int k = 0; k < NZ; k++)
	{
	  temp = Real_E1 (i, j, k);

	  media += temp;

	  variancia += (temp * temp);

	}

  media /= (NX * NY * NZ);

  variancia /= (NX * NY * NZ);

  variancia -= (media * media);

  return variancia;
}

// Criar duas, uma para media, uma para desvio padrão
// e trabalhar com um TVetor
// Antes de chamar o calculo do DesvioPadrao deve zerar as variáveis chamando
// a função Inicializa(..)
double
CEstatistica::DesvioPadrao (double *x, int numeroDados)
{
  int i;			// contador

  double xx;			// um valor especifico do vetor

  double x2;			// o quadrado de x

  if (numeroDados > 1)
    {
      numeroDados += numeroDados;

      if (hasMissingData)
	{
	  for (i = 0; i < numeroDados; i++)
	    xx = *(x + i);	// aqui parece ter um erro, o for deveria englobar o if abaixo?

	  if (xx > missingCode)
	    {
	      x2 = (xx) * (xx);

	      sum++;

	      sumX += xx;

	      sumX2 += x2;

	      sumX3 += xx * x2;

	      sumX4 += (x2) * (x2);
	    }
	}
      else
	{
	  for (i = 0; i < numeroDados; i++)
	    {
	      xx = *(x + i);	// Acessa valor de x

	      x2 = (xx) * (xx);	// calcula o quadrado

	      sum++;		// incrementa o contador

	      sumX += xx;	// Soma de x

	      sumX2 += x2;	// Soma de x^2

	      sumX3 += xx * x2;	// Soma de x^3

	      sumX4 += (x2) * (x2);	// Soma de x^4
	    }
	}
      if (sum > 1)		// Se a soma for maior que 1
	{			// calcula e retorna os resultados
	  media = sumX / sum;	// média e desvio padrão

	  desvioPadrao = sqrt ((sumX2 - (sumX) * (sumX) / sum) / (sum - 1.0));
	}
      else
	return false;
    }
  return false;
}
