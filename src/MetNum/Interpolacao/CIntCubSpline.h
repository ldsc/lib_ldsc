
#ifndef CIntCubSpline_h
#define CIntCubSpline_h

#include <MetNum/Interpolacao/CInterpolacao.h>

/**
 * @brief   Utilizado nos casos em que a varição das curvas é acentuada.
 *
 * O método faz uma estimativa da segunda derivada, e usa esta segunda derivada
 * para interpolar os valores.
 * 
 * Requer: x,y, as derivadas, H (), o número de pontos n, e os valores interpolados.
 * 
 * O construtor recebe os vetores deriv e h, que passam a pertencer ao objeto, ou seja
 * serão destruídos quando o objeto for deletado
*/
class CIntCubSpline : public CInterpolacao
{
 private:

  double *deriv;

  double *h;

 public:
  /// Construtor
  CIntCubSpline (double *_deriv, double *_h):deriv (_deriv), h (_h)
    {
    }

    /// Destrutor
    virtual ~ CIntCubSpline ()
      {
	if (deriv)
	  delete [] deriv;
	if (h)
	  delete [] h;
      }

    //Funções set
    //void Setderiv(double* _deriv) { if(deriv) delete []deriv; deriv = _deriv;};
    //void Seth(double* _h) {  if(h) delete []h;  h = _h;};

    /// Função da classe base, realiza a interpolação.
    double Go (double *xarr, double *yarr, int n, double x)
    {
      return Spline (xarr, yarr, this->deriv, this->h, n, x);
    }

 private:
	 /// Função auxiliar.
    static double Spline
      (double *xarr, double *yarr, double *deriv, double *h, int n, double x);

};

#endif
