
#include <cmath>

#include <Funcao/CFSomatorioCm2RyM.h>

// Os atributos rz e cm foram recebidos no construtor
// nao usa y
double
CFSomatorioCm2RyM::Go (double x, double y)
{
  double sum = 0;

  for (int m = 0; m < numeroPontosSerie; m++)
    // if(ry!=0.0 && m!=0.0)???? aqui ry é a variavel x
    sum += (cm[m] * cm[m]) * pow (x, m);	// Equacao 3.20

  return (rz - sum);
}
