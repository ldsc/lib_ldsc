#ifndef CSFBissecao_h
#define CSFBissecao_h

#include <SFuncao/CSFuncao.h>

// ------------------------------------------------------------------------------
// Classe CSFBissecao
// ------------------------------------------------------------------------------
/**
 * @brief Calcula a raiz pelo metodo da bissecao é um metodo lento, mas funcional.
 * 
 * Representa um método numérico para solucao de funcoes nao lineares
 * ou seja dada uma funcao y=f(x), determina o valor de x para o qual y=0.
 * */

class CSFBissecao : public CSFuncao
{
// Atributos
// Metodos
protected:
	/// Determina o valor de x para o qual y=0
  virtual void Determina_x ();

public:
	/// Construtor, recebe a funcao
    CSFBissecao (CFuncao * _funcao):CSFuncao (_funcao)
  {
  }
  
/// Destrutor
  virtual ~ CSFBissecao ()
  {
  }
};
#endif
