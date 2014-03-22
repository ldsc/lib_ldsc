#ifndef CSolverMatrizDiagonal_SOR_h
#define CSolverMatrizDiagonal_SOR_h

/**
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: CSolverMatrizDiagonal_SOR...
  ===============================================================================
  Desenvolvido por:
				Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
  @author     	André Duarte Bueno
  @file       	CSolverMatrizDiagonal_SOR.h
  @begin      	Sun Sep 17 2000
  @copyright  	(C) 2000 by André Duarte Bueno
  @email      	andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Base/_LIB_LDSC_CLASS.h>

#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_GaussSeidel.h>

/**
 * @brief Solução de sistema de equações usando sobre-relaxações.
 * É um metodo de solucao de sistema de equações derivado do Gauss_Seidel, isto fica evidenciado pela herança.
 *
 * A diferenca em relacao ao Gauss_Seidel é o uso da funcao de relaxamento.
 * O coeficiente de relaxamento funciona como um acelerador ou retardador da solucao do sistema.
 * Observe, entretanto, que ao acelerar pode ocasionar a divergência do sistema
 * e ao retardar pode tornar a solução muito lenta.
 *
 * Valores acima  de 1 aceleram a solução do sistema.
 * Valores abaixo de 1 retardam a solução do sistema.
 *
 * Obs: Para o caso do grafo de conexão serial o valor do fator de relaxação deve estar entre 1,5 e 1,7.
 * Ex: FR=1.5 fica 1.5*X + (1-1,5)*X0 ou 1.5*X-0.5*X0
 * Ex: FR=0.7 fica 0.7*X + (1-0.7)*X0 ou 0.7*X+0.3*X0
 * 
 * @ingroup HCSolver
 * @author 	André Duarte Bueno
*/
class CSolverMatrizDiagonal_SOR : public CSolverMatrizDiagonal_GaussSeidel
{
    // --------------------------------------------------------------Atributos
private:
    /// Fator de relaxação
    long double fatorRelaxacao {1.0};

    ///  Fator de relaxação complementar (criado para evitar recálculo)
    long double fatorRelaxacaoC { 0.0 };

public:

    // -------------------------------------------------------------Construtor
    /// Construtor, recebe como parâmetro os limites de iterações, de erro e o fator de relaxação.
    CSolverMatrizDiagonal_SOR (unsigned long int _limiteIteracoes, long double _limiteErro,
             long double _fatorRelaxacao/*, unsigned long int _size = 0*/)
        : CSolverMatrizDiagonal_GaussSeidel (_limiteIteracoes, _limiteErro/*, _size*/),
          fatorRelaxacao (_fatorRelaxacao), fatorRelaxacaoC (1.0 - _fatorRelaxacao) {
    }

    // --------------------------------------------------------------Destrutor
    /// Destrutor
    virtual ~ CSolverMatrizDiagonal_SOR () = default;

    // ----------------------------------------------------------------Métodos
protected:

    /// Função que atualiza os valores de X, a diferença é que considera o fator de relaxação.
    virtual void AtualizaX () override
    {
// std::cerr 	<< "\nPassei por CSolverMatrizDiagonal_SOR::AtualizaX. k=" << k;
// std::cerr 	<< "\nantes do ((*obj)[k])->Go () -> (*obj)[k]->x = " << (*obj)[k]->x ;
        // ((*obj)[k])->Go () calcula nova estimativa de x (pressão do sítio) e armazena em vx[iteracoes]
        vx[k] = ((*obj)[k])->Go ();
        // Considera o fator de relaxacao
        vx[k] = fatorRelaxacao * vx[k] + fatorRelaxacaoC * ((*obj)[k]->x);

        // Atualiza a variável no objeto
        // Neste ponto tenho dois valores para a variável x (ex: pressão)
        // em vx[k] esta armazenada a variável x no tempo atual
        // em (*obj)[k]->x  esta armazenada a variável x no tempo anterior
        // abaixo faz o valor de x no objeto igual ao valor no tempo atual.
        (*obj)[k]->x = vx[k];
// std::cerr 	<< "\ndepois do ((*obj)[k])->Go (); -> (*obj)[k]->x = vx[k];=" << vx[k];
    }

    /// Faz obj[k]=X[k]; a diferença é que considera o fator de relaxação
    //virtual void AtualizaObjFinal () override;

public:
    // -------------------------------------------------------------Sobrecarga

    // --------------------------------------------------------------------Get
    /// Retorna o fator de relaxação
    double FatorRelaxacao () const    {
        return fatorRelaxacao;
    }

    // --------------------------------------------------------------------Set
    /// Define o fator de relaxação, e seu complemento
    void FatorRelaxacao (long double _fr)  {
        fatorRelaxacao = _fr;
        fatorRelaxacaoC = 1.0 - _fr;
    }

    // -----------------------------------------------------------------Friend
    /// Sobrecarga operador <<
    friend std::ostream & operator<< (std::ostream & os, const CSolverMatrizDiagonal_SOR & obj);
    /// Sobrecarga operador >>
    friend std::istream & operator>> (std::istream & is, CSolverMatrizDiagonal_SOR & obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream & operator<< (std::ostream & os, const CSolverMatrizDiagonal_SOR & obj);
std::istream & operator>> (std::istream & is, CSolverMatrizDiagonal_SOR & obj);

#endif
