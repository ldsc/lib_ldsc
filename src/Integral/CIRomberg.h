
#ifndef CIRomberg_h
#define CIRomberg_h

#include   <Integral/CIGauss.h>

// ------------------------------------------------------------------------------
// Objeto CIRomberg
// ------------------------------------------------------------------------------
/**
 * @brief Calculo da integral pelo método de Romberg.
 * 
 * Herdeira de CIGauss.
*/
class CIRomberg : public CIGauss
{
protected:
	/// Método auxiliar
  double intPow (int base, int exponent);
  
public:
	
/// Construtor
    CIRomberg (CFuncao * &_funcao):CIGauss (_funcao)
  {
  }

  /// Destrutor
  virtual ~ CIRomberg ()
  {
  }

/// Calcula a integral
  virtual double Go (double x = 1, double y = 0);
  
};
#endif
