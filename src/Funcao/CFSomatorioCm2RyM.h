#ifndef CFSomatorioCm2RyM_h
#define CFSomatorioCm2RyM_h

#include <Base/CFuncao.h>

// ------------------------------------------------------------------------------
// Objeto CFSomatorioCm2RyM
// ------------------------------------------------------------------------------
// Descrição:
// Funcao Rz=SomatorioCm^2*Ry^m
// Recebe como parametros rz, o numero de pontos do somatorio, e o vetor cm[m]
// 
/**
 * @brief Representa função de Bessel.
 * @author André Duarte Bueno
 * */
class CFSomatorioCm2RyM : public CFuncao
{
  unsigned int numeroPontosSerie;

  double rz;		///< Variavel independente de x,

  double *cm;		///< Vetor cm

public:
	/// Construtor
    CFSomatorioCm2RyM (double _rz, unsigned int _numeroPontosSerie,
		       double *_cm):rz (_rz),
    numeroPontosSerie (_numeroPontosSerie), cm (_cm)
  {
  }

/// Determina função em sí. 
   virtual double Go (double x = 0, double y = 0);
};
#endif
