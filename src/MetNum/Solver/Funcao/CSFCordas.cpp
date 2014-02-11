

#include <MetNum/Solver/Funcao/CSFCordas.h>

// y=m*x+b

// m=dy/dx

// y1=m*x1+b     b=y1-m*x1

void
CSFCordas::Determina_x ()
{
  double m, b;			// m é o coeficiente angular e b o linear

  // if(xMaior==xMenor) xMaior+=e-8;     // evita divisao por zero

  m = (yMaior - yMenor) / (xMaior - xMenor);	// a única diferença em relação ao método da bissecao

  b = yMaior - m * xMaior;	// a determinação do valor de x

  x = -b / m;
}
