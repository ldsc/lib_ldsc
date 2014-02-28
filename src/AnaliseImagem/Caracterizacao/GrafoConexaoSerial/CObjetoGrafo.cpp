/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
			[LDSC].
@author:          André Duarte Bueno
@file:             CObjetoGrafo.cpp
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo.h>
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
@todo   : verificar padrão, usar espaço ou '\n'.
*/
ostream & operator<< (ostream & os, const CObjetoGrafo & obj)
{
  os << static_cast<unsigned char>( obj.Contorno() ) << '\n'
     << obj.rotulo << '\n'
     << obj.propriedade << '\n';
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
@todo   : Note que esta armazenando o tipo de contorno em x; corrigir/analizar!
*/
istream&  operator>> (istream& is,  CObjetoGrafo& obj)
{
  is >> obj.x
     >> obj.rotulo 
     >> obj.propriedade ;
  return is;
}
