/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
	[LDSC].
@author:          André Duarte Bueno
@file:             CSMDSOR.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDSOR.h>

using namespace std;

// ---------------------------------------------------------------------------
// Funcao AtualizaX
// ---------------------------------------------------------------------------
// Funcao que atualiza o valor de X, considerando os fatores de  relaxamento.
// Observe que faz uma interpolacao entre o resultado
// de X atual e X0..
   // (*obj) retorna um objeto vetor
   // (*obj)[k] retorna um elemento do objeto vetor,
   // que é um ponteiro para CSMParametroSolver que tem a função Go
   // *(*obj)[k] retorna um objeto CSMParametroSolver que tem sobrecarga para receber um double
/*
Livro Maliska
presao[k,t+1] =fr * pressao[k,t+1] + (1-fr) * pressao[k,t];
com fr= 1,5 a 1,7 ficando
presao[k,t+1] = 1.5 * pressao[k,t+1]  - .5 * pressao[k,t];

*/
void
CSMDSOR::AtualizaX ()
{
  // PASSO FUNDAMENTAL
  // Executa a função Go do objeto k
  // Ex: para determinação da permeabilidade, executa Go de um objeto do grafo
  // calculando os valores de pressão do sítio (objeto do grafo).

  // Go calcula e retorna o valor atual da propriedade (pressão do sítio)
  X[k] = ((*obj)[k])->Go ();	// Gauss Seidel

  // Considera o fator de relaxacao
  X[k] =
    fatorRelaxacao * X[k] + fatorRelaxacaoC * ((long double) (*((*obj)[k])));

  // Atualiza a variável no objeto
  // incluida em 14/2/2001 para deixar igual Liang
  // sem a linha abaixo fica sendo um Jacobi relaxado
  *((*obj)[k]) = X[k];
}

// ---------------------------------------------------------------------------
// Funcao AtualizaObjFinal
// ---------------------------------------------------------------------------
// Neste ponto tenho dois campos para a variável x (ex: pressão)
// em X[k]                                       esta armazenado o campo no tempo atual
// em *((*obj)[kk])      esta armazenado o campo no tempo anterior
// aqui atualiza o campo nos objetos fazendo *((*obj)[kk]) = X[k]
void
CSMDSOR::AtualizaObjFinal ()
{
/*Abaixo é desnecessário, já esta atualizando dentro de AtualizaX
  unsigned int size = obj->size();
	for ( unsigned int kk = 0 ; kk < size ; kk++)
		{
   	// calcula X considerando fator relaxacao
   	// X[kk] = fatorRelaxacao * X[kk] + fatorRelaxacaoC * (    (long double)(*((*obj)[kk]))   );
   	// Atualiza a variável no objeto
   	*((*obj)[kk]) = X[kk];
  	}
*/
}

/*
LIANG usa:
Em AtualizaX, calcula previsao da nova pressao com Go,
depois faz X[k]= média da previsao da pressao com a pressao atual
void  CSMDSOR::AtualizaX()
	X[k] = ((*obj)[k])->Go();
	X[k] = fatorRelaxacao * X[k] + fatorRelaxacaoC * (    (long double)(*((*obj)[k]))   );

Percorre todos os objetos e faz a pressao = X[k]
void  CSMDSOR::AtualizaObjFinal()
	for ( unsigned int kk = 0 ; kk < size ; kk++)
   	*((*obj)[kk]) = X[kk];

*/

// Sobrecarga streans
// Função friend
// Saida dos parametros desta classe

ostream & operator<< (ostream & os, const CSMDSOR & s)
{
//    os << s.fatorRelaxacao << endl;
//    os << s.fatorRelaxacaoC<< endl;
  return os;
}

// Entrada dos parametros desta classe
istream & operator>> (istream & is, CSMDSOR & s)
{
  is >> s.fatorRelaxacao;
  is >> s.fatorRelaxacaoC;
  return is;
}

// #endif
