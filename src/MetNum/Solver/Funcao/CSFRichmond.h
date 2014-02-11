#ifndef CSFRichmond_h
#define CSFRichmond_h

#include <MetNum/Solver/Funcao/CSFuncao.h>

// ------------------------------------------------------------------------------
// Classe CSRichmont
// ------------------------------------------------------------------------------
/**
 * @brief Calcula a raiz de uma função pelo metodo de Richmont, que usa uma 
 * aproximação da primeira e da segunda derivada.
 * 
 * Representa um método numérico para solucao de funcoes nao lineares
 * ou seja dada uma funcao y=f(x), determina o valor de x para o qual y=0.
 *  
 * O objeto CSRichmont recebe como parametro uma determinada funcao
 * e alguns parametros internos.
 * Ao chamar a funcao Go(semente), o objeto determina uma raiz para a equacao
 * e retorna a posicao x desta raiz, ou seja y=f(x)
 * é um método rápido.
 * */

class CSFRichmond : public CSFuncao
{

// metodos
public:
	/// Construtor, recebe a funcao
  CSFRichmond (CFuncao * &_funcao):CSFuncao (_funcao)
  {
  }	
/// Destrutor

  virtual ~ CSFRichmond ()
  {
  }
  
/// Realiza a determinacao de x para o qual y=0
  virtual double Go (double semente = 0);	

};
#endif
