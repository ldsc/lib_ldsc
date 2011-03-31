#include <iostream>

#include <Integral/CISimpson.h>

double
CISimpson::Go (double x, double y)
{

// [n=5]
  limiteInferior = y;

  limiteSuperior = x;

  Dx ();

  resultado = 0.5 * (funcao->Go (limiteInferior)	// ponto 0      // [0]
		     + funcao->Go (limiteSuperior));	// ponto n      // [4]

  for (int i = 1; i < (numeroPontos - 2); i += 2)
    {
      resultado += 2.0 * funcao->Go (limiteInferior + i * dx)	// ponto 1,...,n-3 // [1]
	+ funcao->Go (limiteInferior + (i + 1) * dx);	// ponto 2,...,n-2 // [2]
    }

  resultado += 2.0 * funcao->Go (limiteSuperior - dx);	// ponto n-1            // [3]

  resultado *= 2.0 * dx / 3.0;

  return resultado;

}

/*
Abaixo saida para tela dos calculos
cout<<"\nFunção CISimpson::Go()";
cout<<"\nda="<<dx;
cout<<"\n0.5*f(liminf="<<limiteInferior<<")="<<(0.5*funcao->Go(limiteInferior));
cout<<"\n0.5*f(limSup="<<limiteSuperior<<")="<<(0.5*funcao->Go(limiteSuperior));
cout<<"\n+2*f("<<(limiteInferior+(i)*dx)<<")="<<(2*funcao->Go(limiteInferior+(i)*dx));
cout<<" +f("<<(limiteInferior+(i+1)*dx)<<")="<<funcao->Go(limiteInferior+(i+1)*dx);
cout<<" +2*f("<<(limiteSuperior-dx)<<")="<<2*funcao->Go(limiteSuperior-dx);
cout<<"\nResultado="<<resultado;
*/
