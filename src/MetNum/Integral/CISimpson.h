#ifndef CISimpson_h
#define CISimpson_h

#include <MetNum/Integral/CIntegral.h>

// ------------------------------------------------------------------------------
// Objeto CISimpson
// ------------------------------------------------------------------------------
/** 
 * @brief Calcula a integral de simpsom de uma funcao passada.
 * 
 * A funcao SetParametros e utilizada para passagem dos parametros da integral
 * a funcao Go() calcula a integral.
 * 
 * Herdeira de CIntegral.
 * */

class CISimpson : public CIntegral
{

public:
	
 /// Construtor
  CISimpson (CFuncao * _funcao):CIntegral (_funcao)  {  }

 /// Calcula a integral
  virtual double Go (double x = 1, double y = 0);

};
#endif
