#ifndef CECombinacoes_h
#define CECombinacoes_h

#include <Estatistica/CEFatorial.h>
#include <Base/CMath.h>

/**
 * @brief Classe utilizada para determinação da permutação, arranjo, combinação e fatorial.
 * */
class CECombinacoes
{
public:
  double combinacao;		///< Valor da ultima combinação calculada
  double permutacao;		///< Valor da ultima permutação calculada
  double arranjo;		///< Valor do ultimo arranjo calculado
  double fatorial;		///< Último fatorial calculado
  bool comRepeticao;		///< flag que indica se existe repetição

  CMath::EMathError codigoErro;

public:
  /// Construtor
  CECombinacoes():codigoErro(CMath::ok) {};
	
  /// Destrutor
  virtual ~CECombinacoes(){};

  /// Calculo de combinações m por n.
  double Combinacao (int m, int n);	// 

  /// Cálculo de permutações m por n.
  double Permutacao (int m, int n);	// 

  /// Cálculo de arranjos m por n.
  inline double Arranjo (int m, int n)
  {
	  return arranjo = Permutacao (m, n);
  }
  
  /// Função de cálculo do fatorial.
  double Fatorial (int n)
  {
      CEFatorial obj_fatorial (n);
      return fatorial = obj_fatorial[n];
  }

  
  // Get
  /// Retorna valor da última combinacao calculada.
  double Combinacao() 	{ return combinacao; }
  
  /// Retorna valor da última permutacao calculada.
  double Permutacao() 	{ return permutacao; }
  
  /// Retorna valor do último arranjo calculado.
  double Arranjo() 	{ return arranjo; }
  
  /// Retorna valor do último fatorial calculado.
  double Fatorial() 	{ return fatorial; }
  
};
#endif

