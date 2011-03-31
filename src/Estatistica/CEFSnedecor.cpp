
#include <Estatistica/CEFSnedecor.h>	// CENormal

// Construtor

CEFSnedecor::CEFSnedecor ()	// : df1(0),   df2(0)
{
}

// Operador de copia
// Operator= Assignment Operator alternative to compiler provided operator=
CEFSnedecor & CEFSnedecor::operator= (const CEFSnedecor & aCEFSnedecor)
{
  if (this == &aCEFSnedecor)
    return *this;

  CENormal::operator= (aCEFSnedecor);	// Usa o operador da classe ancestral

  // para copiar atributos.
  return *this;
}

//  Function will return the probability of obtaining an
//       F statistic, z, at df degrees of freedom.
// double CEFSnedecor::Go(double _z, double _df1, double _df2)
double
CEFSnedecor::Go (double _z)
{
  z = _z;

  // df1=_df1;
  // df2=_df2;

  double k = 0.222222222222222;	//          k = 2.0 / 9.0;

  double f1 = pow (z, 0.333334) * (1.0 - k / df2) - (1.0 - k / df1);

  double f2 = sqrt (k / df1 + pow (z, 0.666667) * k / df2);

  area = 1.0 - CENormal::Go (f1 / f2);

  return area;
}

//  Function will return the value of the F statistic for a
//   given probability, z, and df degrees of freedom.
// double CEFSnedecor::GoInv(double _area, double _df1, double _df2)
double
CEFSnedecor::GoInv (double _area)
{

  area = _area;
  // df1=_df1;
  // df2=_df2;
  if (area <= 0.0)
    area = 0.0001;		// checa os limites de area

  if (area >= 1.0)
    area = 0.9999;

  z = CENormal::GoInv (area);

  double k = 0.222222222222222;	// k = 2.0 / 9.0;

  double alpha = (1.0 - k / df2);

  double beta = (1.0 - k / df1);

  double gamma = 1.0 - beta;

  double delta = 1.0 - alpha;

  double A = alpha * alpha - z * z * delta;

  double B = -2.0 * alpha * beta;

  double C = beta * beta - z * z * gamma;

  z = (-B + sqrt (B * B - 4.0 * A * C)) / (2.0 * A);	// equação da parabola

  z = z * z * z;

  return z;
}
