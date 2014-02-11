

#ifndef CFHermite_h
#define CFHermite_h

#include "MetNum/Funcao/CFIterativa.h"

/**
 * @brief Representa função de Hermite, herdadas de CFIterativa.
 * @author André Duarte Bueno
 * */
class CFHermite : public CFIterativa
{
//   virtual double Go(double x);
//   virtual double Go(double x,double y=0); // calcula a funcao em si y=f(x) ou z=f(x,y)
protected:

/// Função polinômio de Hermite L1, calcula a funcao em si y=f(x) ou z=f(x,y)
  virtual void CalculaL1 ();	

/// Função polinômio de Hermite L2, calcula a funcao em si y=f(x) ou z=f(x,y)
  virtual void CalculaL2 ();	 

};
#endif
