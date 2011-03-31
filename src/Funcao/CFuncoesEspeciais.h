
#ifndef CFuncoesEspeciais_h
#define CFuncoesEspeciais_h

#include "Estatistica/CEFatorial.h"

/*
// Criar uma classe CFuncoesEspeciais
// As funções seriam chamadas diretamente
Ex:
	CFuncoesEspeciais* ptr=new CFuncoesEspeciais*();
	double fat=ptr->factorial(n);
*/

/**
 * @brief Representa funções especiais.
 * @author André Duarte Bueno
 * */
class CFuncoesEspeciais
{
	
public:
	
  /// Função Gamma
  double gamma (double x);

  /// Função erro
  double erf (double x);

  
  /// Função Ei
  double Ei (double x);

  /// Função Si
  double Si (double x);

  /// Função Ci
  double Ci (double x);

  /// Função Beta
  double Beta (double z, double w);

  // Funcoes com dois parametros
  /// Função Laguerre
  double Laguerre (double x, int n);

  /// Função Hermite
  double Hermite (double x, int n);

  /// Função Chebyshev
  double Chebyshev (double x, int n);

  /// Função BesselJ
  double BesselJ (double z, int v);

  /// Função BesselY
  double BesselY (double z, int v);

  /// Função Legendre
  double Legendre (double x, double n);
  
  CEFatorial factorial; ///< Objeto para cálculo do fatorial
};
#endif
