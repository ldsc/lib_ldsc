
#ifndef  CIMNDiferencasCentrais_h
#define  CIMNDiferencasCentrais_h

#include <MetNum/Interpolacao/IntMetNum/CIntMetNum.h>

/*
--------------------------------------------------------------------
            Classe CIMNDiferencasCentrais
--------------------------------------------------------------------
*/
/**
 * @brief Utiliza aproximacao por diferencas centrais. 
 * alfa e beta (sao constantes)
 * */
class CIMNDiferencasCentrais : public CIntMetNum
{
  // Métodos
public:

	/// Construtor
  CIMNDiferencasCentrais ();	

  /// Destrutor
  virtual ~ CIMNDiferencasCentrais ();	

/// Redefinida, calcula alfa e beta
  virtual void AlfaBeta (double &u, double &dxc, double &ro, double &dif);	

};
#endif
