
#ifndef CIGChebyshevQuadrature_h
#define CIGChebyshevQuadrature_h

#include <Integral/CIGauss.h>

// ------------------------------------------------------------------------------
// Objeto CIGChebyshevQuadrature
// ------------------------------------------------------------------------------
/**
 * @brief Calcula a integral usando método da quadratura de chebyshev.
 * 
 * */
class CIGChebyshevQuadrature : public CIGauss
{

public:
	/// Construtor
  CIGChebyshevQuadrature (CFuncao * &_funcao):CIGauss (_funcao)
  {
  }

  /// Destrutor
  virtual ~ CIGChebyshevQuadrature ()
  {
  }

/// Calcula a integral
  virtual double Go (double x = 1, double y = 0);
  
};
#endif
