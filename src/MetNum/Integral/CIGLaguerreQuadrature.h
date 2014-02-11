
#ifndef CIGLaguerreQuadrature_h
#define CIGLaguerreQuadrature_h

#include <Integral/CIGauss.h>

// ------------------------------------------------------------------------------
// Objeto CIGLaguerreQuadrature
// ------------------------------------------------------------------------------
/**
 * @brief Calculo da integral pelo método da quadratura de laguerre
 * Herdeira de CIGauss.
 */
class CIGLaguerreQuadrature : public CIGauss
{

public:
	/// Construtor
  CIGLaguerreQuadrature (CFuncao * &_funcao):CIGauss (_funcao)
  {
  }

  /// Destrutor
  virtual ~ CIGLaguerreQuadrature ()
  {
  }

  // virtual double Go();// calcula a integral
/// Calcula a funcao em si y=f(x)
  virtual double Go (double x = 1, double y = 0);	
};
#endif
