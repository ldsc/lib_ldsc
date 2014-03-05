/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:          André Duarte Bueno
@file:             CContorno.cpp
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

#include <MetNum/Contorno/CContorno.h>
using namespace std;

/**
-------------------------------------------------------------------------
Função:    operator<<
-------------------------------------------------------------------------
@short  : Escreve em os as propriedades do objeto
@author : André Duarte Bueno
@see    : 
@param  : const CContorno & 
@return : ostream&
@todo   : verificar padrão, não deveria ter o endl!
*/
ostream & operator<< (ostream & os, const CContorno & obj)
{
  os << obj.valorContorno /*<< endl*/;
  return os;
}

/**
-------------------------------------------------------------------------
Função:    operator>>
-------------------------------------------------------------------------
@short  : Lê as propriedades do objeto
@author : André Duarte Bueno
@see    : 
@param  : CContorno & 
@return : ostream&
*/
istream&  operator>> (istream& is,  CContorno& obj)
{
  is >> obj.valorContorno ;
  return is;
}
