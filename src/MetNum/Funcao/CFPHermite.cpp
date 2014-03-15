
#include <cmath>

#include <MetNum/Funcao/CFPHermite.h>

// Construtor
// Deve calcular os parametros que sao utilizados na funcao
CFPHermite::CFPHermite (double _porosidade, double _yc,
			int _numeroPontosSerie)
{
  // numeroPontosSerie=_numeroPontosSerie;
  // cria objeto fatorial que ja calcula um vetor de fatoriais
  fatorial = new CEFatorial (_numeroPontosSerie);

  // parametro utilizado para tomada de decisao na funcao Go
  yc = _yc;			// ypsilon de corte

  // parametros utilizados na funcao, calculados em funcao da porosidade
  // cy1=(_porosidade-1)/sqrt(_porosidade*(1-_porosidade));                // equacao 3.22
  // cy2=_porosidade/sqrt(_porosidade*(1-_porosidade));        // equacao 3.23

  // aceleradores de performance, o que é constante é calculado agora
  auxiliar1 = (_porosidade - 1) / sqrt (_porosidade * (1 - _porosidade));	// equacao 3.22

  auxiliar2 = _porosidade / sqrt (_porosidade * (1 - _porosidade));	// equacao 3.23
}

double
CFPHermite::Go (double x, double y)
{
  double resultado;

  if (x <= yc)			// 2.0                          // 2.0
    resultado =
      (double) exp (-x * x / 2.0) * Hermite (m, x) * auxiliar1 / sqrt (2.0 *
								       M_PI *
								       fatorial->
								       Go
								       (m));
  else
    resultado =
      (double) exp (-x * x / 2.0) * Hermite (m,
					     x) * auxiliar2 / sqrt (2.0 *
								    M_PI *
								    fatorial->
								    Go (m));
  return (resultado);
}

// Calcula o polinomio de hermite eq.3.24
// funcao repetida diversas vezes
// Aceleracao do cálculo
// Se m e y assumirem valores repetidos, montar uma matriz de Hermite para acelerar o cálculo
double
CFPHermite::Hermite (int mm, double y)
{
  double a, b, c;

  int i;

  a = 1;

  b = y;

  switch (mm)
    {
    case 0:
      c = a;
      break;

    case 1:
      c = b;
      break;

    default:
      for (i = 1; i < mm; i++)
	{
	  c = y * b - i * a;
	  a = b;
	  b = c;
	}
      break;

    }
  return (c);
}
