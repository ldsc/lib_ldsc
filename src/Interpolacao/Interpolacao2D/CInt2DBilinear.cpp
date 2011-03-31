
#include <Interpolacao/Interpolacao2D/CInt2DBilinear.h>

/*Bilinear interploration*/
//Recebe
   //NX=Dimensão nx
	//NY=dimensão ny
	//Re_data,Ponteiro para matriz 2D
	//posição x
	//posição y
//Retorna
	//valor de z no ponto

#define z(x,y)		matriz2D [(y)*nx + (x)]

double
CInt2DBilinear::Go (const int nx, const int ny,
		    const float *matriz2D, const double x, const double y)
{

  //Usa nx,
  int kx;

  for (int i = 0; i <= nx - 3; i++)
    {
      if (x <= (i + 1))
	{
	  kx = i;
	  break;
	}
      else
	kx = nx - 2;
    }

  //Usa ny,
  int ly;

  for (int j = 0; j <= ny - 3; j++)
    {
      if (y <= (j + 1))
	{
	  ly = j;
	  break;
	}
      else
	ly = ny - 2;
    }

  //Calcula variáveis auxiliares x_kx e y_ly
  float x_kx, y_ly;

  x_kx = (x - kx);		//x_kx
  y_ly = (y - ly);		//y_ly

  //Calcula result em sí
  double result;

  result =
    (1 - x_kx) * (1 - y_ly) * z (kx, ly) 
    + y_ly * (1 - x_kx) * z (kx, ly + 1) +
    x_kx * (1 - y_ly) * z (kx + 1, ly) + x_kx * y_ly * z (kx + 1, ly + 1);

  return (result);
}
