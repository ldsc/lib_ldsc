#ifndef CSolver_h
#define CSolver_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CSolver...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     Andre Duarte Bueno
@file       CSolver.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by Andre Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>
#include <string>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

/**
 * @brief Classe definida para solução de sistemas (funções,matrizes).
 * 
 * Inclue os atributos:
 * iteracoes 		- número de iterações efetivamente realizadas
 * erro			- erro médio calculado
 * limiteIteracoes	-número máximo de iterações que devem ser realizadas
 *
 * Obs:
 * Tanto a solução de sistemas de equações como de funções podem ter erros,
 * este erro pode ser um enumerated colocado aqui 
 * (usar COperacao).
 * Assim o usuário criaria o objeto solver, executaria o mesmo obtendo a
 * solução final mas para usá-la deveria verificar o atributo de erro.
 *
 * @author 	André Duarte Bueno	
 * @version						
 * @see		Veja assunto
*/
class CSolver
{

// --------------------------------------------------------------Atributos
protected:
  /// Passo de iteracoes
  unsigned long int iteracoes;

  /// Erro calculado nesta iteração
  long double erro;

  /// Número limite de iterações
  unsigned long int limiteIteracoes; // verificar tipo

  /// Erro máximo ao resolver o sistema
  long double limiteErro;

  // Erro médio máximo
  // long double limiteErroMedio;

public:
  /**
   * Construtor, recebe como parâmetros o número limite 
   * de iterações e de erro.
   * */
    CSolver (unsigned long int _limiteIteracoes = 100, long double _limiteErro = 0.00001)
	:iteracoes (0), erro (0.0), limiteIteracoes (_limiteIteracoes), limiteErro (_limiteErro)
    {
    }

// --------------------------------------------------------------Destrutor
    /**
     * Destrutor.
     * @return 
     */
    virtual ~ CSolver ()
      {
      }

// ----------------------------------------------------------------M?todos
public:
  /// Lê atributos internos (mudar para ifstream>>)
    void Read (std::string nomeArquivo); 

// --------------------------------------------------------------------Get
  /// Retorna o limite de erro
  inline long double LimiteErro () const
  {
    return limiteErro;
  }

  /// Retorna o limite de iterações
  inline unsigned long int LimiteIteracoes () const
  {
    return limiteIteracoes;
  }

  /// Retorna o limite de erro
  inline long double Erro () const
  {
    return erro;
  }

  /// Retorna o limite de iterações
  inline unsigned long int Iteracoes () const
  {
    return iteracoes;
  }

// --------------------------------------------------------------------Set
  /// Seta o limite de iterações
  inline void LimiteIteracoes (unsigned int _li)
  {
    limiteIteracoes = _li;
  }

  /// Seta o limite de erros
  inline void LimiteErro (long double _le)
  {
    limiteErro = _le;
  }

/*  inline void SetErro (long double _e)
  {
    erro = _e;
  };   
 */

// -----------------------------------------------------------------Friend
  
  /**
   * Sobrecarga operador <<.
   * @param os 
   * @param obj 
   * @return 
   */
  friend std::ostream & operator << (std::ostream & os, const CSolver & obj);
  
  /**
   * Sobrecarga operador >>.
   * @param is 
   * @param obj 
   * @return 
   */
  friend std::istream & operator >> (std::istream & is, CSolver & obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend

std::ostream & operator << (std::ostream & os, const CSolver & obj);

std::istream & operator >> (std::istream & is, CSolver & obj);

#endif
