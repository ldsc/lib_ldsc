#ifndef CSFNewton_h
#define CSFNewton_h

#include <SFuncao/CSFuncao.h>

// ------------------------------------------------------------------------------
// Classe CSFNewton
// ------------------------------------------------------------------------------
/**
 * @brief Calcula a raiz pelo metodo de newtow, é um método rápido.
 * */

class CSFNewton : public CSFuncao
{
// Metodos
public:	

	/// Construtor, recebe a funcao
  CSFNewton (CFuncao * &_funcao):CSFuncao (_funcao)
  {
  }
  
/// Destrutor
  virtual ~ CSFNewton ()
  {
  }

/// Realiza a determinacao de x para o qual y=0
  virtual double Go (double semente = 0);
};
#endif
