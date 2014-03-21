#ifndef CSolverMatriz_ParametroSolver_h
#define CSolverMatriz_ParametroSolver_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
						Assunto/Ramo: Solver...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CSolverMatriz_ParametroSolver.h
@begin      Sun Sep 17 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <iostream>
#include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

// ===============================================================================
// Documentacao Classe: CSolverMatriz_ParametroSolver
// ===============================================================================
/**
 * @brief
 * Classe que representa as propriedades e funções que um objeto deve ter
 * para poder ser resolvido por um objeto solver (do tipo CSDiagonalDominante).
 *
 * Basicamente, quando voce utiliza um solver voce quer resolver um sistema de equações.
 * Se o seu objeto for um volume de controle e voce estiver resolvendo
 * um problema de transferência de calor, a variável a ser resolvida é a temperatura. 
 * Neste caso, você terá um nó com os diferentes coeficientes Ai.
 * Você cria um objeto solver, e passa para ele a matriz (ou vetor) de objetos volumes de controle. 
 * Estes objetos vão ser acessados e utilizados pelo solver, através das funções virtuais declaradas nesta classe.
 *
 * Exemplo: (algo como:)
 * CNo* no = new CNo[nx][ny][nz];...
 * solver->Go(no);...
 *
 * Obs: Se um solver tradicional trabalha sobre um vetor, basta fazer com que esta classe se comporte como um CVetor, 
 * criando um vetor de CSolverMatriz_ParametroSolver e passando para o objeto solver.
 *
 * Observe que na classe  CSolverMatriz_ParametroSolver são criadas sobrecargas para operações básicas como igualar o objeto a um double, 
 * igualar um double ao objeto, comparar e igualar dois objetos.
 *
 * A função Go() do objeto CNo tem uma utilidade especial, éla deve calcular o valor da variável no instante de tempo atual.
 * Assim Go() calcula a propriedade de interesse no instante de tempo atual, com base no tempo anterior, mas sem alterar o valor da propriedade.
 * (ex: calcula nova temperatura sem atualizar a temperatura no nó).
 * 
 * Na prática você deve criar objetos herdeiros de  CSolverMatriz_ParametroSolver e acrescentar os atributos e métodos necessários 
 * para o cálculo da  propriedade no instante de tempo anterior e atual.
 *
 * Exemplo:
 * Cria 10 objetos do tipo CSolverMatriz_ParametroSolver
 * CSolverMatriz_ParametroSolver* obj = new CSolverMatriz_ParametroSolver [10];
 * for( int i = 0 ; i < 10; i++}
 * {
 * // usa cast para double
 * valorDaVariavel_tempoAnterior = obj[i];
 *
 * // calcula propriedade atualizada
 * valorDaVariavel_tempoAtual = obj[i]->Go(double parametro);
 * }
 *
 * Obs: Pensar em transformar o CSolverMatriz_ParametroSolver em uma função.
 * Ou seja herdeiro da CFuncao. Go(x,y)?
 * Dado um valor/parâmetro, gera um novo valor.
 * @ingroup HCSolver
 * @author 	André Duarte Bueno
*/
class CSolverMatriz_ParametroSolver {
// --------------------------------------------------------------Atributos
public:
/// Variável a ser calculada
   long double x {0.0};

// -------------------------------------------------------------Construtor
/// Contrutor
   CSolverMatriz_ParametroSolver () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
   virtual ~ CSolverMatriz_ParametroSolver () = default;

// ----------------------------------------------------------------Métodos
public:
   /**
    * @brief A função Go(), deve utilizar o parametro d para calcular uma nova estimativa do valor da variável x, 
	 * mas Go() não deve alterar o valor da variável.
    *
    * Ou seja dado ti posso calcular ti+1 mas não altero ti.
    * Se o usuário desejar alterar o valor da variável deverá fazer:
    * obj = obj->Go(d);
    * Vai ser reescrita nas classes herdeiras.
   */
   virtual long double Go ( long double d = 0 ) = 0;

// -------------------------------------------------------------Sobrecarga
   // Operadores sobrecarregados
   /// Sobrecarga operador de Comparação obj_a == obj_b
   virtual bool operator== ( const CSolverMatriz_ParametroSolver& par )  {
      return this->x == par.x;
   }

   /// Sobrecarga operador diferença obj_a != obj_b
   virtual bool operator!= ( const CSolverMatriz_ParametroSolver& par ) const {
      return this->x != par.x;
   }

   /// Sobrecarga operador de igualdade Iguala obj_a=obj_b
   virtual CSolverMatriz_ParametroSolver& operator= ( const CSolverMatriz_ParametroSolver& par ) {
      x = par.x;
      return *this;
   }

   /**Recebe um double: double x=5.1; obj=x;*/
   long double& operator= ( const long double& d ) {
      return x = d;
   }

/// Converte para double (cast): double x=obj;
   operator  long double () const {
      return x;
   }

// --------------------------------------------------------------------Get
  long double& X() { return x; }

// --------------------------------------------------------------------Set
  void X( long double& _x ) { x = _x; }

// -----------------------------------------------------------------Friend
   /// Sobrecarga do operador <<
   friend std::ostream& operator<< ( std::ostream& os,
                                     const CSolverMatriz_ParametroSolver& obj );

   /// Sobrecarga do operador >>
   friend std::istream& operator>> ( std::istream& is,
                                     CSolverMatriz_ParametroSolver& obj );
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream& operator<< ( std::ostream& os, const CSolverMatriz_ParametroSolver& obj );

std::istream& operator>> ( std::istream& is, CSolverMatriz_ParametroSolver& obj );

#endif


