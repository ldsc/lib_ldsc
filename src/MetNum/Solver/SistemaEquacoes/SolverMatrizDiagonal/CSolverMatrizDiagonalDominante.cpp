/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================
Desenvolvido por:
				Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:        André Duarte Bueno
@file:          CSolverMatrizDiagonalDominante.cpp
@begin:         Sat Sep 16 2000
@copyright:     (C) 2000 by André Duarte Bueno
@email:         andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++.
// -----------------------------------------------------------------------
#include <cmath>
#include <cassert>
#include <fstream>
#include <iomanip>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC.
// -----------------------------------------------------------------------
#include <Base/COperacao.h>

#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonalDominante.h>

using namespace std;

// // ----------------------------------------------------------------------------
// // Sobrecarga operador saida stream
// // ----------------------------------------------------------------------------
// // Escreve os atributos do objeto na ostream (saida para disco)
// ostream & operator<< ( ostream & os, const CSolverMatrizDiagonalDominante & s )
// {
//   os << static_cast<CSolver>(s);
// //    os << s.limiteIteracoes << '\n'
// //       << s.iteracoes << '\n'
// //       << s.limiteErro << '\n'
// //       << s.erro << '\n'
// //       << s.erroMedio << '\n';
//
// //   std::vector < CSolverMatriz_ParametroSolver * >* obj{ nullptr };
// //   unsigned long int dimensaoVetor{0};
// //   long double *vx { nullptr };
//    return os;
// }
//
// // ----------------------------------------------------------------------------
// // Sobrecarga operador entrada streams
// // ----------------------------------------------------------------------------
// // Lê os dados do objeto da istream
// istream & operator>> ( istream & is, CSolverMatrizDiagonalDominante & s )
// {
//   is >> static_cast<CSolver>(s);
// //   is >> s.limiteIteracoes
// //       >> s.iteracoes
// //       >> s.limiteErro;
// //          >> s.erro
// //          >> s.erroMedio ;
// //   std::vector < CSolverMatriz_ParametroSolver * >* obj{ nullptr };
// //   unsigned long int dimensaoVetor{0};
// //   long double *vx { nullptr };
//    return is;
// }

// -----------------------------------------------------------------------
// Funcao Go
// -----------------------------------------------------------------------
/* A função Go() recebe um vetor de objetos, vai percorrer todos os objetos,
 * e atualizar as variáveis x no vetor vx usando a função Go() do objeto.
 *
 * Variáveis herdadas de CSolver
 *  /// Número limite de iterações (número máximo de iterações a serem realizadas).
 *  unsigned long int limiteIteracoes {1000};
 *  /// Número de iterações efetivamente realizadas.
 *  unsigned long int iteracoes{0};
 *  /// Erro máximo ao resolver o sistema.
 *  long double limiteErro{1.0e-8};
 *  /// Erro pontual calculado nesta iteração (de todos os pontos analizados o que apresenta o maior erro).
 *  long double erro{0.0};
 *   // Erro médio (de todos os pontos)
 *  long double erroMedio{0.0};
 *
 * Obs:
 * (*obj) 		é o vetor.
 * (*obj)[k] 	é o objeto k do vetor (que é um ponteiro).
 * *((*obj)[k]) é o objeto k apontado pelo vetor.
 * (*obj)[k].x  é a propriedade x do k'ésimo objeto.
 */
double CSolverMatrizDiagonalDominante::Go( vector < CSolverMatriz_ParametroSolver * > * objeto )
{
// std::cerr 	<< "\nCSolverMatrizDiagonalDominante::Go( vector < CSolverMatriz_ParametroSolver * > * objeto )";
	
    // Seta o atributo obj como sendo o vetor recebido. Necessário pois usa obj em AtualizaX.
    obj = objeto;

    if( vx == nullptr or dimensaoVetorVx < obj->size() ) {
        delete [] vx;							// Se menor, deleta vx.
        vx = nullptr;							// Zera.
        vx = new long double[obj->size()];		// Aloca vx como sendo do tamanho do vetor recebido.
        assert( vx );							// Verifica alocação.
    }

    long double vxk0;							// Valor de x para objeto k no instante anterior 0.
    long double erroPontual { 0.0 };			// erro calculado em cada ponto.
    unsigned long int numeroObjetosUsadosCalculoErroMedio { 0 };
    iteracoes = 0;								// Número de iterações

    // Cada passagem pelo laço do, equivale a um passo de tempo ou uma iteração.
    do {
        erro = 0.0;
        erroMedio = 0.0;
        numeroObjetosUsadosCalculoErroMedio = 0;

        // Resolve o sistema para um dado passo de tempo ou iteração
        for( k = 0; k < obj->size(); k++ ) {	// percorre todos os objeto
            // Pega o valor da propriedade x do objeto [k] e armazena em xk0,
            // xk0 passa a ser o valor de x no instante anterior.
            vxk0 = (*obj)[k]->x; // ( *( ( *obj )[k] ) );
// cerr << "\nvxk0 = (*obj)[" << k << "]->x = " << vxk0;
            // AtualizaX chama obj[k]->Go() que gera nova estimativa para x.
            AtualizaX();
// cerr << " depois de chamar AtualizaX -> vxk0 = " << vxk0 ;

            // O erro é a diferença entre vx[k] e xk0 dividido por xk0 // (valor absoluto)
            if( ( vx[k] - vxk0 ) != 0.0 ) {	// os objetos da borda tem vxk0 == vx[k] logo o erro = 0/0
                erroPontual = vxk0 > vx[k] ? ( vxk0 - vx[k] ) / vxk0 : ( vx[k] - vxk0 ) / vxk0;
                erroMedio += erroPontual;
                numeroObjetosUsadosCalculoErroMedio++;

                // erro assume o maior valor de todos os erros pontuais
                if( erro < erroPontual )
                    erro = erroPontual;
            }
        }
        erroMedio /= numeroObjetosUsadosCalculoErroMedio;

        // Atualiza valores finais nos objeto (necessário somente para o jacobi)
        AtualizaObjFinal();

        // Incrementa a iteração
        iteracoes++;
    }
    while( erro > limiteErro and				// Se o erro calculado é maior que o limiteErro continua.
            iteracoes < limiteIteracoes ); 		// Se o limiteIteracoes ainda não foi atingido continua.
    if( vx ) {
        delete [] vx;							// Deleta vx
        vx = nullptr;							// Zera
    }

    return erro;
}
