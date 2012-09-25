/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
	[LDSC].
@author:          André Duarte Bueno
File:             CSMParametroSolver.cpp
begin:            Sat Sep 16 2000
copyright:        (C) 2000 by André Duarte Bueno
email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <SMatriz/CSMParametroSolver.h>
using namespace std;

// -------------------------------------------------------------------------
// Função:               operator<<
// -------------------------------------------------------------------------
/** Salva atributos do objeto em disco.
@author :	André Duarte Bueno
@see    :
@param  :	ostream& os, CSMParametroSolver& obj
@return : ostream& os
*/
ostream & operator<< (ostream & os, const CSMParametroSolver & obj)
{
  os << obj.x << endl;
  return os;
}

// -------------------------------------------------------------------------
// Função:       operator>>
// -------------------------------------------------------------------------
/**
@short  : Lê os atributos do objeto do arquivo de disco
@author :	André Duarte Bueno
@see    :
@param  :	istream& is, CSMParametroSolver& obj
@return :	istream&
*/
istream & operator>> (istream & is, CSMParametroSolver & obj)
{
  is >> obj.x;
  return is;
}
