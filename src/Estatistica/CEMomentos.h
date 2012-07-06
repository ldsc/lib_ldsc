#ifndef CEMomentos_h
#define CEMomentos_h

#include <Estatistica/CEstatistica.h>

/**
 * @brief Determina momentos, coeficiente de skewness e kurtosis.
 * Herdeira de CEstatistica.
 * */
//                 + First four moments
class CEMomentos : public CEstatistica
{

public:
	
/// Determina os momentos, coeficiente de skewness e kurtosis.
  int Momentos (double &meanX, double &varianceX, double &moment3,
		double &moment4, double &skewnessCoeff,
		double &kurtosisCoeff);

};
#endif
