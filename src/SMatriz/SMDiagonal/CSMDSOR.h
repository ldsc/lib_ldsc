#ifndef CSMDSOR_h
#define CSMDSOR_h

/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: CSMDSOR...
  ===============================================================================
  Desenvolvido por:	
  Laboratorio de Desenvolvimento de Software Cientifico 	
  [LDSC].
  @author     André Duarte Bueno
  @file       CSMDSOR.h
  @begin      Sun Sep 17 2000
  @copyright  (C) 2000 by André Duarte Bueno
  @email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Base/_LIB_LDSC_CLASS.h>

#include <SMatriz/SMDiagonal/CSMDGaussSeidel.h>

/**
 * @brief Solução de sistema de equações usando sobre relaxações.
 * 
 * é um metodo de solucao de sistema de equacoes derivado do Gauss_Seidel
 * isto fica evidenciado pela herança.
 * 
 * A diferenca em relacao ao Gauss_Seidel é o uso da funcao de relaxamento.
 * O coeficiente de relaxamento funciona como um acelerador ou retardador
 * da solucao do sistema.
 * Observe entretando que ao acelerar pode ocasionar a divergencia do sistema
 * e ao retardar pode tornar a solucao do sistema muito lenta.
 * 
 * Obs: O valor do fator de relaxacao deve estar entre 1,5 e 1,7.
 * Valores acima  de 1 aceleram a solução do sistema.
 * Valores abaixo de 1 retardam a solução do sistema.
 * 
 * Ex: FR=1.5 fica   1.5*X + (1-1,5)*X0 ou 1.5*X-0.5*X0
 * Ex: FR=0.7 fica   0.7*X + (1-0.7)*X0 ou 0.7*X+0.3*X0
 * 
 * @author 	André Duarte Bueno	
 * @see		SMatriz
*/
class CSMDSOR:public CSMDGaussSeidel
{
  // --------------------------------------------------------------Atributos
 public:

  /// Fator de relaxação 
  long double fatorRelaxacao;

  ///  Fator de relaxação complementar
  long double fatorRelaxacaoC;

 public:

  // -------------------------------------------------------------Construtor
  // Recebe como parâmetro o fator de relação e os
  // limites de erro e de iterações
	 /// Construtor
  CSMDSOR (unsigned long int _limiteIteracoes, long double _limiteErro, 
	   long double _fatorRelaxacao, unsigned long int _size = 0)
    :CSMDGaussSeidel (_limiteIteracoes, _limiteErro, _size),
    fatorRelaxacao (_fatorRelaxacao),
    fatorRelaxacaoC (1.0 - _fatorRelaxacao)
      {
      }

    // --------------------------------------------------------------Destrutor
/// Destrutor   
       virtual ~ CSMDSOR ()
      {
      }

    // ----------------------------------------------------------------Métodos
 protected:

    /// Função que atualiza os valores de X,
    // a diferença é que considera o fator de relaxação
    virtual void AtualizaX ();

    /// Faz obj[k]=X[k];
    // a diferença é que considera o fator de relaxação
    virtual void AtualizaObjFinal ();

 public:
    // -------------------------------------------------------------Sobrecarga

    // --------------------------------------------------------------------Get
    /// Retorna o fator de relaxação
    double FatorRelaxacao () const
    {
      return fatorRelaxacao;
    };

    // --------------------------------------------------------------------Set
    /// Define o fator de relaxação, e seu complemento
    void FatorRelaxacao (long double _fr)
    {
      fatorRelaxacao = _fr;
      fatorRelaxacaoC = 1.0 - _fr;
    };

    // -----------------------------------------------------------------Friend
    /// Sobrecarga operador <<
    friend std::ostream & operator<< (std::ostream & os, const CSMDSOR & obj);
    /// Sobrecarga operador >>
    friend std::istream & operator>> (std::istream & is, CSMDSOR & obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream & operator<< (std::ostream & os, const CSMDSOR & obj);
std::istream & operator>> (std::istream & is, CSMDSOR & obj);

#endif
