
#ifndef CIMNExata_h
#define CIMNExata_h

#include <Interpolacao/IntMetNum/CIntMetNum.h>

// ---------------------------------------------------------------------------
//                           CLASSE CIMNExata
// ---------------------------------------------------------------------------
/**
 * @brief  Utiliza aproximacao CIMNExata,  que é baseada na solucao 
 * CIMNExata do problema.
 * 
 * Os coeficientes alfa e beta calculados sao exatos.
 * 
 * A velocidade recebida como parametro nao pode ser zero.
 * 
 * */
class CIMNExata : public CIntMetNum
{
public:

/// Construtor
  CIMNExata ();			

  /// Destrutor
  virtual ~ CIMNExata ();	
  
  /// Redefinida, calcula alfa e beta
  virtual void AlfaBeta (double &u, double &dxc, double &ro, double &dif);	

};
#endif
