/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: Base
===============================================================================

Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico 	
[LDSC].
@author:          Andre Duarte Bueno
File:             CSolver.cpp
begin:            Sat Sep 16 2000
copyright:        (C) 2000 by André Duarte Bueno
email:            andreduartebueno@gmail.com
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
// Escreve os atributos do objeto na ostream (saida para disco)
ostream & operator<< (ostream & os, const CSolver & s)
{
  os << s.limiteErro << '\n'
     << s.limiteIteracoes << '\n';
  return os;
}

// ----------------------------------------------------------------------------
// Sobrecarga operador entrada streams
// ----------------------------------------------------------------------------
// Lê os dados do objeto da istream
istream & operator>> (istream & is, CSolver & s)
{
  is >> s.limiteErro;
  is >> s.limiteIteracoes;
  return is;
}

// ----------------------------------------------------------------------------
// Função: Read
// ----------------------------------------------------------------------------
// Lê atributos do objeto do arquivo de disco
// Recebe nome do arquivo de disco
// Retorna: void
void
CSolver::Read (string fileName)
{
  ifstream fin (fileName.c_str ());
  if (fin.good ())
    {
      fin >> (*this);
      fin.close ();
    }
  else
    {
      // Se falhou, solicita os dados do objeto
      cout << "\nEntre com os atributos do objeto - Class CSolver";
      cout << "\nEntre com o limiteErro: ";
      cin >> limiteErro;
      cin.get ();
      cout << "\nEntre com o  limiteIteracoes: ";
      cin >> limiteIteracoes;
      cin.get ();				// Pega o retorno de carro

      // cout << "\nEntre com o fatorRelaxacao: ";   	// resolver
      // cin >> fatorRelaxacao;
      // cin.get();
    }
}
