#ifndef CFIterativa_h
#define CFIterativa_h

#include "Base/CFuncao.h"

/**
 * @brief Representa função Iterativa.
 * @author André Duarte Bueno
 * */
class CFIterativa : public CFuncao
{
protected:

  double L0, L1, L2;

  double x;

  int i;

protected:
	
/// Função iterativa L1
	virtual void CalculaL1 ();

/// Função iterativa L2
	virtual void CalculaL2 ();

public:
	
	/// Função iterativa, calcula a funcao em si y=f(x) ou z=f(x,y)	
	virtual double Go (double _x = 0, double _y = 0);

};
#endif
