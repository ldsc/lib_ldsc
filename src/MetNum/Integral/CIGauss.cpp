
// #include <iostream.h>

#include <cmath>

#include <MetNum/Integral/CIGauss.h>

// ------------------------------------------------------------------------------
// Usa o método de CIGauss.
// Divide a região a ser calculada em um conjunto de subintervalos,
// para cada subintervalo calcula a área

double
CIGauss::Go (double x, double y)
{

  limiteInferior = y;

  limiteSuperior = x;

  Dx ();

  resultado = 0;

  inicioSubIntervalo = limiteInferior;

  for (int i = 1; i <= numeroSubIntervalos; i++)	// Percorre os subintervalos
    {

      areaSubIntervalo = 0;

      fimSubIntervalo = inicioSubIntervalo + dx;

      dxDiv2 = dx / 2.0;

      CalculaAreaSubIntervalo ();	// calcula area dos subintervalos

      resultado += dxDiv2 * areaSubIntervalo;

      inicioSubIntervalo = fimSubIntervalo;

    }
  return resultado;
}
