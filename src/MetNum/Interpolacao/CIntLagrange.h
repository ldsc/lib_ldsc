
#ifndef CIntLagrange_h
#define CIntLagrange_h

#include <MetNum/Interpolacao/CInterpolacao.h>

/*
--------------------------------------------------------------------
                          Classe CIntLagrange
--------------------------------------------------------------------
*/
/**
 * @brief Realiza a interpolação de Lagrange.
 * 
 * A interpolação de Lagrange é a mais popular, embora não seja muito eficiente
 * pois não salva os valores anteriores para obter valores posteriores.
 * Basicamente recebe um número de pontos [pares (x,y)] e ajusta uma curva polinomial
 * para estes pontos.
 * Necessita recalcular todos os termos para cada valor de x.
 * 
 * PS: Lembre-se os métodos são baseados na expansão de Taylor, que
 * define um conjunto de equações para cada valor de x.
 * Ou seja a cada novo calor de x, um novo conjunto de equações.
*/
class CIntLagrange : public CInterpolacao
{
public:

	/// Calculo da interpolação.
  virtual double Go (double *xarr, double *yarr, int n, double x)
  {
    return Lagrange (xarr, yarr, n, x);
  }

public:
	/// Função auxiliar.
  static double Lagrange (double *xarr, double *yarr, int n, double x);

};

#endif

