// Funcoes com dois parametros
#ifndef CFLaguerre_h
#define CFLaguerre_h

#include "MetNum/Funcao/CFIterativa.h"

/**
 * @brief Representa função de Laguerre.
 * @author André Duarte Bueno
 * */
class CFLaguerre : public CFIterativa
{
	
protected:
	
/// Função Laguerre, iterativa L1
	virtual void CalculaL1 ();

/// Função Laguerre, iterativa L2
	virtual void CalculaL2 ();
};

#endif
