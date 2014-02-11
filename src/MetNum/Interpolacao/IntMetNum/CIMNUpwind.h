
#ifndef    CIMNUpwind_h
#define CIMNUpwind_h

#include <MetNum/Interpolacao/IntMetNum/CIntMetNum.h>

// ---------------------------------------------------------------------------
//                           CLASSE CIntMetNumUpwind
// ---------------------------------------------------------------------------
/**
 * @brief 
 * Utiliza aproximacao por CIntMetNumUpwind.
 *  Alfa e beta sao constantes.
 * */
class CIMNUpwind : public CIntMetNum
{
public:
/// Construtor
  CIMNUpwind ();		

  /// Destrutor
  virtual ~ CIMNUpwind ();	

  /// Redefinida, calcula alfa e beta
  virtual void AlfaBeta (double &u, double &dxc, double &ro, double &dif);
};
#endif
