
#include <cmath>

#include <iostream>

#include <MetNum/Solver/Funcao/CSFNewton.h>

// y=m.x+b

// y1=m.x1+b             x1=(y1-b)/m

// y2=0=m.x2+b   x2=-b/m

// m=(y(x+dx)-y(x-dx))/2*dx

// diferenca=x1-x2=(y1-b)/m --b/m =((y1-b)+b)/m =y1/m

// x2=-y1/m=-diferenca

double CSFNewton::Go (double semente)
{

  double m, diferenca, dx;	// m é o coeficiente angular e b o linear

  iteracoes = 0;

  do
    {

      dx = (fabs (x) > 1) ? 0.01 * x : 0.01;	// determina valor para dx

      m = (funcao->Go (x + dx) - funcao->Go (x - dx)) / 2.0 * dx;	// determina coeficiente angular

      yx = funcao->Go (x);

// cout<<"\nf("<<x<<")="<<yx;

      diferenca = yx / m;	// diferenca entre x1(anterior) e x2(novo)

      x -= diferenca;

      erro = fabs (diferenca);	// calcula erro na determinação de x

      iteracoes++;

    }

  while (erro > limiteErro && iteracoes <= limiteIteracoes);

  return x;
}
