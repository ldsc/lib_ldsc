/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
									Ramo: Base
===============================================================================

Desenvolvido por: 
				Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:        Andre Duarte Bueno
@file:          CSolver.cpp
@begin:         Sat Sep 16 2000
@copyright:     (C) 2000 by André Duarte Bueno
@email:         andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>
#include <iostream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Base/CSolver.h>
using namespace std;

// ----------------------------------------------------------------------------
// Sobrecarga operador saida stream
// ----------------------------------------------------------------------------
/**
* Sobrecarga operador <<, escreve os atributos do objeto na ostream (saida para disco).
* @param ostream & os
* @param const CSolver & s
* @return std::ostream & os
*/
ostream & operator<< (ostream & os, const CSolver & s) {
	os << "Limite iterações: "			<< s.limiteIteracoes
		 << "\nIterações realizadas: "	<< s.iteracoes
		 << "\nLimite erro: "						<< s.limiteErro
		 << "\nErro: "									<< s.erro
		 << "\nErro médio: "						<< s.erroMedio;
	return os;
}

// ----------------------------------------------------------------------------
// Sobrecarga operador entrada streams
// ----------------------------------------------------------------------------
/**
* Sobrecarga operador >>, lê os dados do objeto da istream.
* @param istream & is
* @param CSolver & s
* @return std::istream &
*/
istream & operator>> (istream & is, CSolver & s)
{
	is	>> s.limiteIteracoes
			>> s.iteracoes
			>> s.limiteErro
			>> s.erro
			>> s.erroMedio ;
	return is;
}

// ----------------------------------------------------------------------------
// Função: Read
// ----------------------------------------------------------------------------
/** Lê atributos do objeto do arquivo de disco.
 * @param:  nome do arquivo de disco.
 * @return: void.
 */
void
CSolver::Read (string nomeArquivo)
{
	ifstream fin (nomeArquivo.c_str ());
	if (fin.good ())
	{
		fin >> (*this);
		fin.close ();
	}
	else
	{
		// Se falhou, solicita os dados do objeto
		cout << "\nEntre com o  limiteIteracoes: ";
		cin >> limiteIteracoes;
		cin.get ();
		cout << "\nEntre com os atributos do objeto - Class CSolver";
		cout << "\nEntre com o  limiteIteracoes: ";
		cin >> limiteIteracoes;
		cin.get ();
		cout << "\nEntre com o limiteErro: ";
		cin >> limiteErro;
		// cout << "\nEntre com o fatorRelaxacao: ";   	// resolver
		// cin >> fatorRelaxacao;
		// cin.get();
	}
}
