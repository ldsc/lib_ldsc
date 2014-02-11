#ifndef CSFCordas_h
#define CSFCordas_h

#include <MetNum/Solver/Funcao/CSFuncao.h>

// ------------------------------------------------------------------------------
// Classe CSFCordas
// ------------------------------------------------------------------------------
/**
 * @brief Calcula a raiz pelo metodo das cordas é um metodo funcional.
 * 
 * Representa um método numérico para solucao de funcoes nao lineares
 * ou seja dada uma funcao y=f(x), determina o valor de x para o qual y=0.
 * */
class CSFCordas:public CSFuncao
{
// Metodos
protected:
 /// Determina o valor de x para o qual y=0
  virtual void Determina_x ();

public:
	/// Construtor, recebe a funcao
    CSFCordas (CFuncao * &_funcao):CSFuncao (_funcao)
  {
  }
  
/// Destrutor
  virtual ~ CSFCordas ()
  {
  }
  
};
#endif
