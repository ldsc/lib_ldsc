
#ifndef CIGLegendreQuadrature_h
#define CIGLegendreQuadrature_h

#include <Integral/CIGauss.h>

// ------------------------------------------------------------------------------
// Objeto CIGLegendreQuadrature
// ------------------------------------------------------------------------------
/**
 * @brief Calculo da integral usando método da quadratura de legendre,
 * CIGLegendreQuadrature.
 * 
 * Herdeira de CIGauss.
 * */
class CIGLegendreQuadrature : public CIGauss
{

protected:	
	/// Calcula a área de um subintervalo
  virtual void CalculaAreaSubIntervalo ();	

public:
	/// Construtor
    CIGLegendreQuadrature (CFuncao * &_funcao):CIGauss (_funcao)
  {
  }

  /// Destrutor
  virtual ~ CIGLegendreQuadrature ()
  {
  }

};
#endif
