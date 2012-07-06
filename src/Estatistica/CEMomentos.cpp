
#include <cmath>
using namespace std;

#include <Estatistica/CEMomentos.h>

// Calcula os primeiros momentos e os coeficientes de skewness e kurtosis.
int
CEMomentos::Momentos (double &mediaX, double &varianceX,
		      double &moment3, double &moment4,
		      double &skewnessCoeff, double &kurtosisCoeff)
{
  double mediaSqrd;

  if (sum < 2)
    return false;

  mediaX = sumX / sum;

  mediaSqrd = (mediaX) * (mediaX);

  varianceX = sumX2 / sum - mediaSqrd;

  moment3 =
    sumX3 / sum - 3.0 / sum * mediaX * sumX2 + 2.0 * mediaX * mediaSqrd;

  moment4 =
    sumX4 / sum - 4.0 / sum * mediaX * sumX3 + 6.0 / sum * mediaSqrd * sumX2 -
    3.0 * (mediaSqrd) * (mediaSqrd);

  skewnessCoeff = moment3 / pow (varianceX, 1.5);

  kurtosisCoeff = moment4 / (varianceX) * (varianceX);

  return true;
}
