
#include <cmath>

#include <iostream>

#include <SFuncao/CSFuncao.h>

// Construtor: Somente armazena o endereço do objeto função
CSFuncao::CSFuncao (CFuncao * &_funcao)
{

  limiteErro = 1.0e-8;

  limiteIteracoes = 100;

  xMenor = limiteInferior = -10;

  x = xMaior = limiteSuperior = +10;

  funcao = _funcao;

}

double
CSFuncao::Go (double semente)
{

  iteracoes = 0;

  yMenor = funcao->Go (xMenor);	// determina os y para posicoes limites

  yMaior = funcao->Go (xMaior);	// 

// cout<<"\nSolver..xMenor="<<xMenor<<" xMaior="<<xMaior;

  do

    {

      Determina_x ();		// determina o valor de x

      yx = funcao->Go (x);	// determina y=f(x)

// cout<<"\nf("<<x<<")="<<yx;

      if ( (yx * yMaior) > 0)	// se maior que zero yx
	{			// passa a ser yMaior

	  xMaior = x;

	  yMaior = yx;
	}
      else
	{

	  xMenor = x;
	  // yMenor=yx;                // não é usado
	}

      iteracoes++;

      erro = fabs (xMaior - xMenor);	// o erro é a diferença de x anterior e x atual
// cout<<"\n y("<<x<<")="<<yx<<" erro="<<erro<<" iter="<<iter;
    }
  // sai quando erro<limiteErro, ou atingiu numero maximo de iteracoes

  while (erro > limiteErro && iteracoes <= limiteIteracoes);

  return x;
}
