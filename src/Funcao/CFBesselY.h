

#ifndef CFBesselY_h
#define CFBesselY_h

#include <Base/CFuncao.h>

/**
 * @brief Representa função de BesselY.
 * @author André Duarte Bueno
 * */
class CFBesselY : public CFuncao
{
/// Polinômio de BesselY, calcula a funcao em si y=f(x) ou z=f(x,y)
	virtual double Go (double x = 0, double y = 0);	// calcula a funcao em si y=f(x) ou z=f(x,y)

};
#endif
