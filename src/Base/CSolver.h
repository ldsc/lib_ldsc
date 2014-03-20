#ifndef CSolver_h
#define CSolver_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CSolver...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
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
 * @brief Classe solver, base para solução de sistemas (funções, matrizes).
 *
 * Inclue os atributos:
 * limiteIteracoes	- número máximo de iterações que devem ser realizadas.
 * iteracoes 		- número de iterações efetivamente realizadas.
 * limiteErro		- limite de erro.
 * erro				- erro calculado (maior valor de erro de todos os pontos/nós).
 * erroMedio		- erro médio (média de todos os pontos).
 *
 * Obs:
 * Tanto a solução de sistemas de equações como de funções podem ter diferentes tipos de erros,
 * estes erros podem ser relatados através de uma enumeração colocada aqui.
 * Assim o usuário criaria o objeto solver, executaria o mesmo obtendo a
 * solução final mas para usá-la deveria verificar o atributo de erro.
 *
 * @author 	André Duarte Bueno
*/
class CSolver {
// --------------------------------------------------------------Atributos
protected:
public:
   /// Número limite de iterações (número máximo de iterações a serem realizadas).
   unsigned long int limiteIteracoes {1000};

   /// Número de iterações efetivamente realizadas.
   unsigned long int iteracoes {0}; // k

   /// Erro máximo ao resolver o sistema.
   long double limiteErro {1.0e-8};

   /// Erro pontual calculado nesta iteração (de todos os pontos analizados o que apresenta o maior erro).
   long double erro {0.0};

   // Erro médio (de todos os pontos)
   long double erroMedio {0.0};

public:
// --------------------------------------------------------------Construtores
   /**
    * Construtor default
    */
   CSolver() = default;

   /**
    * Construtor sobrecarregado, recebe como parâmetros o limite de iterações e o limite de erro.
    */
   CSolver( unsigned long int _limiteIteracoes , long double _limiteErro  )
      : limiteIteracoes( _limiteIteracoes ), limiteErro( _limiteErro )   {
   }

// --------------------------------------------------------------Destrutor
   /**
    * Destrutor.
    */
   virtual ~ CSolver() = default;

// ----------------------------------------------------------------Métodos
public:
   /// Lê atributos internos (mudar para ifstream>>)
   void Read( std::string nomeArquivo );

// --------------------------------------------------------------------Get
   /// Retorna o limite de iterações.
   inline unsigned long int LimiteIteracoes() const  {
      return limiteIteracoes;
   }
   /// Retorna o número de iterações.
   inline unsigned long int Iteracoes() const {
      return iteracoes;
   }
   /// Retorna o limite de erro.
   inline long double LimiteErro() const {
      return limiteErro;
   }
   /// Retorna o erro (de todos os pontos analizados o que apresenta o maior erro).
   inline long double Erro() const {
      return erro;
   }
   /// Retorna o erro médio (média de todos os pontos).
   inline long double ErroMedio() const {
      return ErroMedio();
   }

// --------------------------------------------------------------------Set
   /// Seta o limite de iterações
   inline void LimiteIteracoes( unsigned int _li ) {
      limiteIteracoes = _li;
   }

   /// Seta o limite de erros
   inline void LimiteErro( long double _le ) {
      limiteErro = _le;
   }

// -----------------------------------------------------------------Friend
   /**
    * Sobrecarga operador <<.
    * @param os
    * @param obj
    * @return std::ostream & os
    */
   friend std::ostream & operator << ( std::ostream & os, const CSolver & obj );

   /**
    * Sobrecarga operador >>.
    * @param is
    * @param obj
    * @return std::istream &
    */
   friend std::istream & operator >> ( std::istream & is, CSolver & obj );
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend

std::ostream & operator << ( std::ostream & os, const CSolver & obj );

std::istream & operator >> ( std::istream & is, CSolver & obj );

#endif
