
#include <cmath>
using namespace std;

#include <Estatistica/CETesteMediaDuasAmostras.h>

#include <Estatistica/CETStudent.h>

// Função que calcula a média de duas amostras
int
CETesteMediaDuasAmostras::TesteMediaDuasAmostras (CEstatistica & B,
						  double testedDifference,
						  double probabilidade,
						  double &calcT,
						  double &tableT,
						  int &passTest)
{
  double df, p, factor1, factor2, n1, n2;

  if (sum < 2 || B.sum < 2)
    return false;

  n1 = sum;

  n2 = B.sum;

  if (probabilidade > 1.0)
    p = 0.50 - probabilidade / 200.0;

  else
    p = 0.50 - probabilidade / 2.0;

  df = n1 + n2 - 2.0;

  factor1 = sqrt (1.0 / n1 + 1.0 / n2);

  factor2 =
    sqrt ((sumX2 - (sumX) * (sumX) / n1 + B.sumX2 -
	   (B.sumX) * (B.sumX) / n2) / df);

  calcT = (sumX / n1 - B.sumX / n2 - testedDifference) / (factor1 * factor2);
  // chamada antiga
  // tableT = TInv(p, df);
  // novo

  CETStudent student;

  student.df1 = df;

  tableT = student.GoInv (p);

  // fim novo
  passTest = (fabs (calcT) <= tableT) ? true : false;

  return true;
}
