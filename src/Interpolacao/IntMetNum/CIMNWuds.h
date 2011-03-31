
#ifndef CIMNWuds_h
#define CIMNWuds_h

#include <Interpolacao/IntMetNum/CIntMetNum.h>

// ---------------------------------------------------------------------------
//                           CLASSE CIMNWuds
// ---------------------------------------------------------------------------
/**
 * @brief 
 * Utiliza aproximacao CIMNWuds, que 'e baseada na solucao 
 * CIntMetNumCIntMetNumExata do problema.
 * 
 * Entretanto faz uma aproximacao dos coeficientes alfa e beta.
 * 
 * A classe CIntMetNum deve calcular os coeficientes alfa e beta.
 * */
class CIMNWuds : public CIntMetNum
{
public:

/// Construtor
  CIMNWuds ();
  	
  /// Destrutor
  virtual ~ CIMNWuds ();
  	
  /// Redefinida, calcula alfa e beta
  virtual void AlfaBeta (double &u, double &dxc, double &ro, double &dif);
};

#endif
