
#include <Estatistica/CECombinacoes.h>

// combinacao (m,n)=                     m!
//		                       n!*(m-n)!
double
CECombinacoes::Combinacao (int m, int n)
{
  if (m <= n || m < 0 || n < 0)		// m deve ser maior, e m,n nao devem ser negativos
  {
	  codigoErro = CMath::erroIntervalo;
	  return -1;			// se forem retorna codigo de erro
  }

  CEFatorial fatorial (m);		// Cria vetor de fatoriais até m

  return combinacao = fatorial[m] / ( fatorial[n] * fatorial[m - n] );
}

// Permutacao (m,n)=                     m!
//		                        (m-n)!
double
CECombinacoes::Permutacao (int m, int n)
{

  if (m <= n || m < 0 || n < 0)		// m deve ser maior, e m,n nao devem ser negativos
  {
	  codigoErro = CMath::erroIntervalo;
	  return -1;			// se forem retorna codigo de erro
  }
  
  CEFatorial fatorial (m);		// Cria vetor de fatoriais até m

  return combinacao = fatorial[m] / fatorial[m - n];
}

