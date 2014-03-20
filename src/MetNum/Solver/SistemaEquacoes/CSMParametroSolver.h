#ifndef CSMParametroSolver_h
#define CSMParametroSolver_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
						Assunto/Ramo: Solver...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CSMParametroSolver.h
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
// Documentacao Classe: CSMParametroSolver
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
 * criando um vetor de CSMParametroSolver e passando para o objeto solver.
 *
 * Observe que na classe  CSMParametroSolver são criadas sobrecargas para operações básicas como igualar o objeto a um double, 
 * igualar um double ao objeto, comparar e igualar dois objetos.
 *
 * A função Go() do objeto CNo tem uma utilidade especial, éla deve calcular o valor da variável no instante de tempo atual.
 * Assim Go() calcula a propriedade de interesse no instante de tempo atual, com base no tempo anterior, mas sem alterar o valor da propriedade.
 * (ex: calcula nova temperatura sem atualizar a temperatura no nó).
 * 
 * Na prática você deve criar objetos herdeiros de  CSMParametroSolver e acrescentar os atributos e métodos necessários 
 * para o cálculo da  propriedade no instante de tempo anterior e atual.
 *
 * Exemplo:
 * Cria 10 objetos do tipo CSMParametroSolver
 * CSMParametroSolver* obj = new CSMParametroSolver [10];
 * for( int i = 0 ; i < 10; i++}
 * {
 * // usa cast para double
 * valorDaVariavel_tempoAnterior = obj[i];
 *
 * // calcula propriedade atualizada
 * valorDaVariavel_tempoAtual = obj[i]->Go(double parametro);
 * }
 *
 * Obs: Pensar em transformar o CSMParametroSolver em uma função.
 * Ou seja herdeiro da CFuncao. Go(x,y)?
 * Dado um valor/parâmetro, gera um novo valor.
 * @author 	André Duarte Bueno
*/
class CSMParametroSolver {
// --------------------------------------------------------------Atributos
public:
/// Variável a ser calculada
   long double x {0.0};

// -------------------------------------------------------------Construtor
/// Contrutor
   CSMParametroSolver () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
   virtual ~ CSMParametroSolver () = default;

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
   virtual bool operator== ( const CSMParametroSolver& par )  {
      return this->x == par.x;
   }

   /// Sobrecarga operador diferença obj_a != obj_b
   virtual bool operator!= ( const CSMParametroSolver& par ) const {
      return this->x != par.x;
   }

   /// Sobrecarga operador de igualdade Iguala obj_a=obj_b
   virtual CSMParametroSolver& operator= ( const CSMParametroSolver& par ) {
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
                                     const CSMParametroSolver& obj );

   /// Sobrecarga do operador >>
   friend std::istream& operator>> ( std::istream& is,
                                     CSMParametroSolver& obj );
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream& operator<< ( std::ostream& os, const CSMParametroSolver& obj );

std::istream& operator>> ( std::istream& is, CSMParametroSolver& obj );

#endif
