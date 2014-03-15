
#include <cmath>
#include <iostream>

using namespace std;

#include <Estatistica/CENormal.h>

// ------------------------------------------------------------------------------
// Construtor
CENormal::CENormal ():z (0.), area (0.)
{
  limiteErro = 0.001;		// valor padrão para o limite de erro
}

// ------------------------------------------------------------------------------
// Construtor de cópia
// Operator= Assignment Operator alternative to compiler provided operator=
CENormal & CENormal::operator= (const CENormal & aCENormal)
{
  if (this == &aCENormal)	// se for eu mesmo
    return *this;		// retornar

  z = aCENormal.z;		// copia de cada atributo

  area = aCENormal.area;

  df1 = aCENormal.df1;

  df2 = aCENormal.df2;

  limiteErro = aCENormal.limiteErro;

  return *this;
}

// ====================================================================================
// Descrição:
// Função que retorna a área da cauda esquerda da curva normal dado um valor de z.
// Dado um valor de z retorna a área da cauda esquerda da curva normal
// Existem duas funções de cálculo, a Go1 que á mais rápida pois utiliza um polinômio que
// aproxima a área da curva normal e a Go2 que á mais lenta mas mais precisa
// pois realiza uma integração sobre a curva normal.
double
CENormal::Go (double _z)
{
  if (limiteErro > 0.01)
    return Go1 (_z);		// equação normal usando polinomio aproximação, +erro+rapida

  else
    return Go2 (_z);		// equação normal usando integração numérica da curva,+precisa+lenta
}

// ------------------------------------------------------------------------------
// Usa polinômio para aproximação da Normal
double
CENormal::Go1 (double _z)
{

  z = _z;

  double tempo = 1.0 / (1.0 + 0.2316419 * fabs (z));

  double tempo2 = tempo * tempo;

  tempo =
    0.319381530 * tempo - 0.356563782 * tempo2 +
    1.781477937 * tempo2 * tempo - 1.821255978 * tempo2 * tempo2 +
    1.330274429 * tempo2 * tempo2 * tempo;

  area = (exp (-z * z / 2.0) / 2.506628274631 * tempo);	// sqrt(2*PI)=2.506628274631

  area = (z >= 0.0) ? 1.0 - area : area;

  return area;
}

// ------------------------------------------------------------------------------
// Calcula a equação normal. Dada a cordenada z, calcula a área
// Esta versão foi obtida do programa de reconstrução do Liang,
// calcula a área sobre a curva normal com uma precisão melhor mas é mais lento
double
CENormal::Go2 (double _z)
{
  z = _z;

  int i;

  double y, e, f;

// cout<<"\nGo2";
// cout<<"\tz="<<z;
  y = fabs (z);

  e = 1. / 2.506628274631 * exp (-0.5 * y * y);	// e=1/sqrt(2*M_PI)*exp(-0.5*y*y);       // 

  f = 28.;

  area = 0.;

  if (y > 3.)			// intervalo para z>3
    {
      for (i = 1; i <= 28; i++)
	{
	  area = f / (y + (area));
	  f -= 1;
	}
      area = e / (y + (area));
    }
  else				// intervalo para z entre 0 e 3
    {
      for (i = 1; i <= 28; i++)
	{
	  area = f * y * y / (2. * f + 1. + pow (-1.0, (double) i) * (area));
	  f -= 1;
	}
      area = (0.5 - e * y / (1. - (area)));
    }

  if (z > 0.)			// fazendo z<0 retorna a cauda direita, agora retorna a cauda esquerda
    area = 1. - (area);
// cout<<" area="<<area;

  return (area);
}

// ====================================================================================

// Descrição:
// Calculate the inverse normal.
// Existem duas funções de cálculo a GoInv1 que utiliza um polinomio para aproximar
// a equação normal, sendo mais rápida mas menos precisa.
// A segunda a GoInv2 realiza a integração numérica sobre a curva, sendo
// mais precisa e mais lenta.
double
CENormal::GoInv (double _area)
{
  if (limiteErro > 0.05)
    return GoInv1 (_area);	// equação normal usando polinomio aproximação, +erro+rapida

  else
    return GoInv2 (_area);	// equação normal usando integração numérica da cruva,+precisa+lenta
}

// ------------------------------------------------------------------------------
// Usa polinômio para aproximação da Normal
double
CENormal::GoInv1 (double _area)
{
// cout<<"\nGoInv1";
  area = _area;

  double sum1, sum2;		// variaveis temporarias

  double tempo;

  if (area <= 0.0) //?????
    area = 0.0001;		// verifica intervalo de area

  if (area >= 1.0) //?????
    area = 0.9999;

  if (area <= 0.5)
    tempo = sqrt (log (1.0 / (area * area)));
  else
    tempo = sqrt (log (1.0 / ((1.0 - area) * (1.0 - area))));

  double tempo2 = tempo * tempo;

  sum1 = 2.515517 + 0.802853 * tempo + 0.010328 * tempo2;

  sum2 =
    1.0 + 1.432788 * tempo + 0.189269 * tempo2 + 0.001308 * tempo2 * tempo;

  z = tempo - sum1 / sum2;	// cálculo de z

  z = (area < 0.5) ? -z : z;
  return z;			// retorna resultado
}

// ------------------------------------------------------------------------------
// Esta versão foi obtida do programa de reconstrução do Liang,
// calcula a área sobre a curva normal com uma precisão melhor.
// Retornando um valor de z mais refinado        // testar
double
CENormal::GoInv2 (double _area)
{
  int contadorLoopDo;

  double p, qq, d, dd, r, erroCalculado, y;

  area = _area;

  if (area <= 0.0)  //?????
    area = 0.0000001;		// verifica intervalo de area

  if (area >= 1.0)   //?????
    area = 0.9999999;

  p = area;

  if (area > 0.5)
    p = 1. - area;
  else if (area == 0.5)
    {
      z = 0.;
      // cout<<" z="<<z;
      return (z);
    }
// else  // *****novidade*****este else estava errado
  {
    contadorLoopDo = 0;

    y = -log (p * (1. - p) * 4.);

    z = sqrt (y * (2.0611786 - 5.7262704 / (y + 11.640595)));

    do
      {
	qq = 1. - Go2 (z);	// era qq=Go2(z);pois usava cauda direita

	d = exp (-0.5 * (z) * (z)) * 0.3989422804;

	dd = -(z) * d;

	r = d * d - (p - qq) * dd * 2.;

	if (r <= 0.)
	  erroCalculado = -d / dd;
	else
	  erroCalculado = (p - qq) * 2 / (-d - sqrt (r));

	z += erroCalculado;

	contadorLoopDo += 1;

	if (contadorLoopDo == 10 || fabs (erroCalculado) <= limiteErro)	// entra se erro < limiteErro ou contadorLoopDo == 10
	  {
	    if (_area < 0.5)	// antes era _area>0.5 e retornava a cauda direita,
	      z = -(z);		// agora retorna a cauda esquerda
	    return (z);		// sai retornando z
	  }
      }
    while (fabs (erroCalculado) > limiteErro);	// enquanto o erroCalculado for maior que o limiteErro
  }

  if (_area < 0.5)		// antes era _area>0.5 e retornava a cauda direita,
    z = -(z);			// agora retorna a cauda esquerda

  return (z);
}

