/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================
Desenvolvido por:	
				Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:        André Duarte Bueno
@file:          CSMDSOR.cpp
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
#include <MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDSOR.h>

using namespace std;

// ---------------------------------------------------------------------------
// Função AtualizaX()
// ---------------------------------------------------------------------------
/* Função que atualiza o valor de X, considerando fator de relaxamento.
 * Observe que faz uma interpolacao entre o resultado de X atual e X0.
 * Livro Maliska
 * presao[k,t+1] = fr * pressao[k,t+1] + (1-fr) * pressao[k,t];
 * com fr= 1,5 a 1,7 ficando
 * presao[k,t+1] = 1.5 * pressao[k,t+1]  - .5 * pressao[k,t];
*/
void
CSMDSOR::AtualizaX ()
{
  // ((*obj)[k])->Go () calcula nova estimativa de x (pressão do sítio) e armazena em X[iteracoes]
  vx[iteracoes] = ((*obj)[iteracoes])->Go ();

  // Considera o fator de relaxacao
  vx[iteracoes] = fatorRelaxacao * vx[iteracoes] + fatorRelaxacaoC * (*((*obj)[iteracoes]));

  // Atualiza a variável no objeto
	// Neste ponto tenho dois campos para a variável x (ex: pressão)
	// em X[k] esta armazenado o campo no tempo atual
	// em *((*obj)[k])  esta armazenado o campo no tempo anterior
	// aqui atualiza o campo nos objetos fazendo *((*obj)[kk]) = X[k]
  *((*obj)[iteracoes]) = vx[iteracoes];
}

// ---------------------------------------------------------------------------
// Funcao AtualizaObjFinal
// ---------------------------------------------------------------------------
// Neste ponto tenho dois campos para a variável x (ex: pressão)
// em X[k] esta armazenado o campo no tempo atual
// em *((*obj)[kk])  esta armazenado o campo no tempo anterior
// aqui atualiza o campo nos objetos fazendo *((*obj)[kk]) = X[k]

void CSMDSOR::AtualizaObjFinal ()
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

// ----------------------------------------------------------------------------
// Sobrecarga operador saida stream
// ----------------------------------------------------------------------------
// Escreve os atributos do objeto na ostream (saida para disco)
ostream & operator<< (ostream & os, const CSMDSOR  & s)
{
  os << s.limiteIteracoes 	<< '\n'
     << s.iteracoes 		<< '\n'
	 << s.limiteErro 		<< '\n'
	 << s.erro 				<< '\n'
	 << s.erroMedio 		<< '\n'
     << s.fatorRelaxacao  	<< '\n'
     << s.fatorRelaxacaoC 	<< endl;
  return os;
}

// ----------------------------------------------------------------------------
// Sobrecarga operador entrada streams
// ----------------------------------------------------------------------------
// Lê os dados do objeto da istream
istream & operator>> (istream & is, CSMDSOR  & s)
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
