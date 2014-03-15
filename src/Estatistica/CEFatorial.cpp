

#include <Estatistica/CEFatorial.h>

// Definicao das funcoes da classe

// Construtor,
// Se nMax for positivo, aloca o vetor fatorial e calcula os valores
// se nMax for negativo (valor default), não aloca o vetor. Sempre recalculando o fatorial.
CEFatorial::CEFatorial (int nMax) : nMaximo (nMax)
{
  // nMaximo=nMax;.
  if (nMaximo >= 0)
    {
      fatorial = new double[nMaximo + 1];
      fatorial[0] = 1.0;
      for (unsigned int i = 1; i <= nMaximo; i++)
	fatorial[i] = fatorial[i - 1] * i;
    }
}

// Destrutor
// Destroe o vetor alocado
CEFatorial::~CEFatorial ()
{
  // if(fatorial)
  if (nMaximo >= 0)
    delete [] fatorial;
}

/*
// Função de cálculo do fatorial.
Observe a existencia de 3 possibilidades:
(1)- nMaximo é negativo, o vetor de fatoriais não é alocado.
E o fatorial deve ser sempre calculado.

(2)- nMaximo é positivo, o vetor de fatoriais é alocado e calculado.
E n solicitado esta abaixo de nMaximo.

(3)- nMaximo é positivo, o vetor de fatoriais é alocado e calculado.
Mas  n solicitado esta acima de  nMaximo
*/
// double CEFatorial::Go(unsigned int n) const
double
CEFatorial::Go (double x, double y)
{

  unsigned int i = 0;		// contador

  int n = (int) x;

  if (n <= nMaximo)		// se o valor solicitado esta dentro da faixa previamente calculada
    return fatorial[n];		// (2)       // (dentro do vetor fatorial),retorna o valor do vetor fatorial direto (rápido)
  else
    {

      double fat = 1;		// faz fat igual a 1

      // Como o valor n solicitado esta acima do tamanho alocado pelo vetor
      // pegue o maior valor do vetor já calculado

      if (nMaximo >= 0)		// (3)        // Verifica se nMaximo não é negativo
	{
	  fat = fatorial[nMaximo];	// Se não negativo, pegue o maio valor previamente calculado
	  i = nMaximo + 1;	// faça i= maior valor mais um
	}
      else
	i = 2;			// (1)        // senão, i=2

      for (; i <= n; i++)	// calcule o resto do fatorial até nMaximo
	fat *= i;		// multiplica um a um

      return fat;		// retorna o fatorial
    }
}
