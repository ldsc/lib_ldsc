#ifndef CSFuncao_h
#define CSFuncao_h

#include <Base/CFuncao.h>

#include <Base/CSolver.h>

// ------------------------------------------------------------------------------
// Classe CSFuncao
// ------------------------------------------------------------------------------
/**
 * @brief Representa um método numérico para solucao de funcoes nao lineares
 * ou seja dada uma funcao y=f(x), determina o valor de x para o qual y=0.
 * */

class CSFuncao : public CSolver
{
// Atributos
public:
	/// limiteInferior do intervalo onde deve procurar a raiz
	double limiteInferior;	
/// limiteSuperior do intervalo onde deve procurar a raiz
	double  limiteSuperior;	

double x;	// cordenada x
double  yx;	// valor de yx, y associado a x

  double xMenor;	///< Cordenada de x menor
  double yMenor;	///< Cordenada de y menor
  double xMaior;	///< Cordenada de x maior
  double yMaior;	///< Cordenada de y maior

  CFuncao *funcao;	///< Ponteiro para função
  
// Metodos
protected:

	/// Determina o valor de x para o qual y é zero.
    virtual void Determina_x ()
  {
  }
 
public:

	/// Construtor, recebe a funcao
  CSFuncao (CFuncao * &_funcao);	

public:

	/// Seta o limite inferior
  void LimiteInferior (double _limiteInferior)
  {
    xMenor = limiteInferior = _limiteInferior;
  }

  /// Seta o limite superior
  void LimiteSuperior (double _limiteSuperior)
  {
    x = xMaior = limiteSuperior = _limiteSuperior;
  }

  /// Realiza a determinacao de x para o qual y=0
  virtual double Go (double semente = 0);
};
#endif
