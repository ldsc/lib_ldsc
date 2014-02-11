
#ifndef CIntEDBarycentric_h
#define CIntEDBarycentric_h

#include <MetNum/Interpolacao/CInterpolacao.h>

/*
--------------------------------------------------------------------
                          Classe CIntEDBarycentric
--------------------------------------------------------------------
*/
/**
 * @brief Caso particular, quando CIntBarycentric tem valores
 * de x igualmente espaçados
*/
class CIntEDBarycentric : public CInterpolacao
{
public:
	/// Realiza a interpolação.
  virtual double Go (double *xarr, double *yarr, int n, double x)
  {
    return ED_Barycentric (xarr, yarr, n, x);
  }

public:
	/// Função auxiliar.
  static double ED_Barycentric (double *xarr, double *yarr, int n, double x);

};
#endif
