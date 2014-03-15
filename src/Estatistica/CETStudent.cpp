
#include <Estatistica/CETStudent.h>

CETStudent::CETStudent ()
{
}

CETStudent & CETStudent::operator= (const CETStudent & aCETStudent)
{
  if (this == &aCETStudent)		// se for eu mesmo sair
    return *this;

  CENormal::operator= (aCETStudent);	// chama função da classe base

  return *this;				// retorna eu
}

// Calcula e retorna a area sobre a curva CETStudent
double
CETStudent::Go (double _z)
{
  z = _z;

  double xt = z * (1.0 - 0.25 / df1) / sqrt (1.0 + z * z / 2.0 / df1);

  area = CENormal::Go (xt);

  return area;
}

// Dada a área retorna o valor de z
double
CETStudent::GoInv (double _area)
{
  area = _area;			// armazena valores de z e df1

  double Pwr[10];		// vetor das potencias

  double termo[4];		// vetor dos termos


  if (area <= 0.0) 		//????
    area = 0.0001;		// checa os limites de area

  if (area >= 1.0)		//????
    area = 0.9999;

  z = CENormal::GoInv (area);	// usa integral da equação normal

  Pwr[1] = z;

  Pwr[2] = z * z;

  Pwr[3] = z * Pwr[2];

  Pwr[5] = Pwr[2] * Pwr[3];

  Pwr[7] = Pwr[2] * Pwr[5];

  Pwr[9] = Pwr[2] * Pwr[7];

  // cálculo dos quatro termos
  termo[0] = 0.25 * (Pwr[3] + Pwr[1]);	// 3,1

  termo[1] = (5. * Pwr[5] + 16. * Pwr[3] + 3. * Pwr[1]) / 96.;	// 5,3,1

  termo[2] = (3. * Pwr[7] + 19. * Pwr[5] + 17. * Pwr[3] - 15. * Pwr[1]) / 384.;	// 7,5,3,1

  termo[3] = (79. * Pwr[9] + 776. * Pwr[7] + 1482. * Pwr[5] - 1920. * Pwr[3] - 945. * Pwr[1]) / 92160.0;	// 9,7,5,3,1

  // cálculo de z
  z =
    z + termo[0] / df1 + termo[1] / (df1 * df1) +
    termo[2] / (df1 * df1 * df1) + termo[3] / (df1 * df1 * df1 * df1);

  return z;
}
