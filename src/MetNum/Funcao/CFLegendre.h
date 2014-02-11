
#ifndef CFLegendre_h
#define CFLegendre_h

#include "MetNum/Funcao/CFIterativa.h"

/**
 * @brief Representa função de Legendre.
 * @author André Duarte Bueno
 * */
class CFLegendre : public CFIterativa
{
//       virtual double Go(double x);
//       virtual double Go(double x,double y=0); // calcula a funcao em si y=f(x) ou z=f(x,y)

protected:
/// Função Legendre, iterativa L1
	virtual void CalculaL1 ();

/// Função Legendre, iterativa L2
	virtual void CalculaL2 ();

};
#endif
