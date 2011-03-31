#ifndef CFChebyshev_h
#define CFChebyshev_h

#include <Funcao/CFIterativa.h>

/**
 * @brief Representa função de Chebyshev, herdeira de CFIterativa.
 * @author André Duarte Bueno
 * */
class CFChebyshev : public CFIterativa
{
protected:
	
/// Polinômio de Chebyshev, calcula L1
  virtual void CalculaL1 ();	// herdadas de CFIterativa

  /// Polinômio de Chebyshev, calcula L2
  virtual void CalculaL2 ();	// 
};
#endif
