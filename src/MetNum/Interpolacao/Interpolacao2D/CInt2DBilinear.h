#ifndef CInt2DBilinear_h
#define CInt2DBilinear_h

#include <MetNum/Interpolacao/Interpolacao2D/CInterpolacao2D.h>

/*
--------------------------------------------------------------------
                          Classe CInt2DBilinear
--------------------------------------------------------------------
*/
/**
 * @brief Interpolação em matrizes e imagens bidimensionais.
 * */
class CInt2DBilinear : public CInterpolacao2D
{

public:
	/// Valor estimado de z no ponto (x,y).
  virtual double Go (const int nx, const int ny, const float *matriz2D,
		     const double x, const double y);
  //virtual double Go(int nx, int ny, double* z_data, double x, double y);
  //virtual double Go(TCMatriz2D matriz, double x, double y);
};

#endif
