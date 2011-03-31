
#ifndef CIGHermiteQuadrature_h
#define CIGHermiteQuadrature_h

#include <Integral/CIGauss.h>

// ------------------------------------------------------------------------------
// Objeto CIGHermiteQuadrature
// ------------------------------------------------------------------------------
/**
 * @brief Calculo da integral pelo método da quadratura de Hermite.
 * Herdeira de CIGauss.
 */

class CIGHermiteQuadrature : public CIGauss
{
public:
	
/// Construtor
  CIGHermiteQuadrature (CFuncao * &_funcao):CIGauss (_funcao)
  {
  }

  /// Destrutor
  virtual ~ CIGHermiteQuadrature ()
  {
  }

/// Calcula a funcao em si y=f(x)
  virtual double Go (double x = 1, double y = 0);	

};
#endif
