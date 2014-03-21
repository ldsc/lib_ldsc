/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================
Desenvolvido por:	
				Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:        André Duarte Bueno
@file:          CSolverMatrizDiagonal_SOR.cpp
@begin:         Sat Sep 16 2000
@copyright:     (C) 2000 by André Duarte Bueno
@email:         andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_SOR.h>

using namespace std;

// // ---------------------------------------------------------------------------
// // Função AtualizaX()
// // ---------------------------------------------------------------------------
// /* Função que atualiza o valor de X, considerando fator de relaxamento.
//  * Observe que faz uma interpolacao entre o resultado de x atual e x0.
//  * Livro Maliska
//  * presao[k,t+1] = fr * pressao[k,t+1] + (1-fr) * pressao[k,t];
//  * com fr= 1,5 a 1,7 ficando
//  * presao[k,t+1] = 1.5 * pressao[k,t+1]  - .5 * pressao[k,t];
// */
// void CSolverMatrizDiagonal_SOR::AtualizaX ()
// {
//   // ((*obj)[k])->Go () calcula nova estimativa de x (pressão do sítio) e armazena em vx[iteracoes]
//   vx[iteracoes] = ((*obj)[iteracoes])->Go ();
// 
//   // Considera o fator de relaxacao
//   vx[iteracoes] = fatorRelaxacao * vx[iteracoes] + fatorRelaxacaoC * (*obj)[iteracoes]->x;
// 
//   // Atualiza a variável no objeto
// 	// Neste ponto tenho dois valores para a variável x (ex: pressão)
// 	// em vx[k] esta armazenada a variável x no tempo atual
// 	// em (*obj)[k]->x  esta armazenada a variável x no tempo anterior
// 	// abaixo faz o valor de x no objeto igual ao valor no tempo atual.
//   (*obj)[iteracoes]->x = vx[iteracoes];
// }

// ---------------------------------------------------------------------------
// Funcao AtualizaObjFinal
// ---------------------------------------------------------------------------
// Neste ponto tenho dois campos para a variável x (ex: pressão)
// em X[k] esta armazenado o campo no tempo atual
// em *((*obj)[kk])  esta armazenado o campo no tempo anterior
// aqui atualiza o campo nos objetos fazendo *((*obj)[kk]) = X[k]

// void CSolverMatrizDiagonal_SOR::AtualizaObjFinal ()
// {
// /*Abaixo é desnecessário, já esta atualizando dentro de AtualizaX
//   unsigned int size = obj->size();
// 	for ( unsigned int kk = 0 ; kk < size ; kk++)
// 		{
//    	// calcula X considerando fator relaxacao
//    	// X[kk] = fatorRelaxacao * X[kk] + fatorRelaxacaoC * (    (long double)(*((*obj)[kk]))   );
//    	// Atualiza a variável no objeto
//    	*((*obj)[kk]) = X[kk];
//   	}
// */
// }

// ----------------------------------------------------------------------------
// Sobrecarga operador saida stream
// ----------------------------------------------------------------------------
// Escreve os atributos do objeto na ostream (saida para disco)
ostream & operator<< (ostream & os, const CSolverMatrizDiagonal_SOR  & s)
{
  os << s.limiteIteracoes 	<< '\n'
     << s.iteracoes 		<< '\n'
	 << s.limiteErro 		<< '\n'
	 << s.erro 				<< '\n'
	 << s.erroMedio 		<< '\n'
     << s.fatorRelaxacao  	<< '\n'
     << s.fatorRelaxacaoC;
  return os;
}

// ----------------------------------------------------------------------------
// Sobrecarga operador entrada streams
// ----------------------------------------------------------------------------
// Lê os dados do objeto da istream
istream & operator>> (istream & is, CSolverMatrizDiagonal_SOR  & s)
{
  is >> s.limiteIteracoes
     >> s.iteracoes
     >> s.limiteErro
	 >> s.erro
	 >> s.erroMedio 
     >> s.fatorRelaxacao
     >> s.fatorRelaxacaoC;
  return is;
}
