
#include <Estatistica/CEChiQuadrado.h>

CEChiQuadrado::CEChiQuadrado ()
{
}

// Operator= Assignment Operator alternative to compiler provided operator=
CEChiQuadrado & CEChiQuadrado::operator= (const CEChiQuadrado & aCEChiQuadrado)
{
  if (this == &aCEChiQuadrado)
    return *this;

  CENormal::operator= (aCEChiQuadrado);

  return *this;
}

// Retorna probabilidade ou area a partir de ChiQuadrado a partir de  z
double
CEChiQuadrado::Go (double _z)
{
  z = _z;

  double aux = 0.222222222222222 / df1;	// aux = 2.0 / 9.0 / df1;

  z = (pow ((z / df1), 0.33333) - (1.0 - aux)) / sqrt (aux);

  area = CENormal::Go (z);

  return area;
}

// Retorna z a partir da probabilidade ou area
double
CEChiQuadrado::GoInv (double _area)
{
  area = _area;

  if (area <= 0.0)		// ????
    area = 0.0001;		// verifica os limites de area

  if (area >= 1.0)		// ????
    area = 0.9999;


  double aux = 0.222222222222222;// aux=2.0/9.0;

  z = CENormal::GoInv (area);

  z = 1.0 - aux / df1 + z * sqrt (aux / df1);

  z = df1 * z * z * z;		// determina z

  return z;			// retorna
}

