#ifndef CSolverMatrizDiagonal_GaussSeidel_h
#define CSolverMatrizDiagonal_GaussSeidel_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CSolverMatrizDiagonal_GaussSeidel...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CSolverMatrizDiagonal_GaussSeidel.h
@begin      Sun Sep 17 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CMath_h
#include <Base/CMath.h>
#endif

#ifndef CSolverMatrizDiagonalDominante_h
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonalDominante.h>
#endif

/**
 * @brief Implementa o solver de Gauss Seidel.
 * O solver Gauss_Seidel é semelhante ao Jacobi, a diferenca é que ao calcular o valor de vx
 * para a linha k utiliza o valor de vx[k] já calculado no objeto (implicito).
 * Deste modo o sistema, na maioria dos casos, converge mais rapidamente.
 * @ingroup HCSolver
 * @author André Duarte Bueno
*/
class CSolverMatrizDiagonal_GaussSeidel : public CSolverMatrizDiagonalDominante
{
// --------------------------------------------------------------Atributos
public:

// -------------------------------------------------------------Construtor
    /// Construtor, Recebe os parâmetros limite de erro e de iterrações
    CSolverMatrizDiagonal_GaussSeidel (unsigned long int _limiteIteracoes, long double _limiteErro/*, unsigned long int _size = 0*/)
        : CSolverMatrizDiagonalDominante (_limiteIteracoes, _limiteErro/*,_size*/)  {
    }

// --------------------------------------------------------------Destrutor
/// Destrutor
    virtual ~ CSolverMatrizDiagonal_GaussSeidel () = default;

// ----------------------------------------------------------------Métodos
protected:

    /// Função que atualiza os valores de X,herdada
    //   virtual void AtualizaX ();

    /** A classe CSolverMatrizDiagonal_GaussSeidel redefine a funcao AtualizaX, implementando a solução para GaussSeidel.
     *  vx[k] = ((*obj)[k])->Go ();
     * (*obj)[k]->x = vx[k];
     */
    virtual void AtualizaX () override
    {
        // Faz o mesmo que a classe CSolverMatrizDiagonalDominante, calcula novo valor de x e armazena em vx[k].
        vx[k] = (*obj)[k]->Go ();

        // Mas, diferente de CSolverMatrizDiagonalDominante, já atualiza o valor da variável no próprio objeto k.
        (*obj)[k]->x = vx[k];
    }

// -------------------------------------------------------------Sobrecarga

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//  friend std::ostream& operator<< (std::ostream& os, CSolverMatrizDiagonal_GaussSeidel& obj);
//  friend std::istream& operator>> (std::istream& is, CSolverMatrizDiagonal_GaussSeidel& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// std::ostream& operator<< (std::ostream& os, CSolverMatrizDiagonal_GaussSeidel& obj);
// std::istream& operator>> (std::istream& is, CSolverMatrizDiagonal_GaussSeidel& obj);

#endif
