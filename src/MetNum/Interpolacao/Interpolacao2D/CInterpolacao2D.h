#ifndef CInterpolacao2D_h
#define CInterpolacao2D_h

#include <Base/CMath.h>

/*
--------------------------------------------------------------------
                          Classe TInterpolacao2D
--------------------------------------------------------------------
*/
/**
 * @brief Interpolação em matrizes e imagens bidimensionais.
 * */
class CInterpolacao2D : virtual public CMath
{

public:
//Recebe
  //NX=Dimensão nx
  //NY=dimensão ny
  //Re_data,Ponteiro para matriz 2D
  //posição x
  //posição y
//Retorna
  /// Valor estimado de z no ponto (x,y).
  virtual double Go (const int nx, const int ny, const float *matriz2D,
		     const double x, const double y) = 0;
  //virtual double Go(int nx, int ny, double* z_data, double x, double y)=0;
  //virtual double Go(TCMatriz2D matriz, double x, double y);
};

#endif
