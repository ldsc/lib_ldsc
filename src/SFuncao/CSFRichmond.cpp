
#include <cmath>

#include <iostream>

#include <SFuncao/CSFRichmond.h>

// Usa primeira e segunda derivada
// Diferenca=(f(x)*f'(x)) / (f'(x)^2 -0.5*f(x)*f''(x))
// x=x-diferenca
double
CSFRichmond::Go (double semente)
{

  double dx;			// m é a derivada,

  double diferenca;		// diferença entre x1 e x2

  double yxa;			// y para (x-dx)

  double yxp;			// y para (x+dx)

  double fd1, fd2;		// primeiraDerivada e segundaDerivada

  iteracoes = 0;
  // x=semente; x= limiteSuperior

  do
    {
      dx = (fabs (x) > 1) ? 0.01 * x : 0.01;	// calcula dx

      yx = funcao->Go (x);	// calcula funcao em x
// cout<<"\nf("<<x<<")="<<yx;

      yxa = funcao->Go (x - dx);	// calcula funcao em x-dx

      yxp = funcao->Go (x + dx);	// calcula funcao em x+dx

      fd1 = (yxp - yxa) / 2 * dx;	// calcula primeiraDerivada

      fd2 = (yxp - 2 * yx + yxa) / dx * dx;	// calcula segundaDerivada

      diferenca = (yx * fd1) / (fd1 * fd1 - 0.5 * yx * fd2);	// diferença entre x anterior e proximo

      x -= diferenca;		// determina novo x

      erro = fabs (diferenca);	// padronizar calculo do erro em x

      iteracoes++;		// acumula iterações

    }

  while (erro > limiteErro && iteracoes <= limiteIteracoes);

  return x;
}
