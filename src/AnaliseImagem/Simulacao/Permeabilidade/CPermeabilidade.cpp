/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================
Desenvolvido por:
			Laboratorio de Desenvolvimento de Software Cientifico	[LDSC].
@author:    André Duarte Bueno
@file:      CPermeabilidade.cpp
@begin:     Sat Sep 16 2000
@copyright: (C) 2000 by André Duarte Bueno
@email:     andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <cstdlib>
#include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Simulacao/Permeabilidade/CPermeabilidade.h>

using namespace std;


// /*
// -------------------------------------------------------------------------
// Função:  operator=
// -------------------------------------------------------------------------
// @short  : OPerador igualdade (atribuição)
// @author : André Duarte Bueno
// @see    :
// @param  : CPermeabilidade&
// @return : *this
// */
// CPermeabilidade & CPermeabilidade::
// operator= (const CPermeabilidade & aCPermeabilidade)
// {
//   if (this == &aCPermeabilidade)
//     return *this;
//
//   permeabilidade = aCPermeabilidade.permeabilidade;
//   return *this;
// }

/*
-------------------------------------------------------------------------
Função:  operator==
-------------------------------------------------------------------------
@short  : Operador comparação igualdade
@author : André Duarte Bueno
@see    :
@param  : const CPermeabilidade&
@return : bool==1 se iguais, ==0 se diferentes
*/
bool CPermeabilidade::operator== ( const CPermeabilidade& ori )  const
{
   return this->permeabilidade == ori.permeabilidade;
}

/*
-------------------------------------------------------------------------
Função:  operator!=
-------------------------------------------------------------------------
@short  :  Operador diferença
@author :  André Duarte Bueno
@see    :
@param  :  const CPermeabilidade&
@return :  bool==0 se iguais, ==1 se diferentes
*/
bool CPermeabilidade::operator!= ( const CPermeabilidade& ori )  const{
   return this->permeabilidade != ori.permeabilidade;
}

/*
-------------------------------------------------------------------------
Função: operator<<
-------------------------------------------------------------------------
@short  : Salva atributos do objeto em disco
@author : André Duarte Bueno
@see    :
@param  : ostream& os, const CPermeabilidade& p
@return : ostream& os,
*/
ostream& operator<< ( ostream& os, const CPermeabilidade& p ){
   os << p.permeabilidade << endl;
   return os;
}

/*
-------------------------------------------------------------------------
Função:  operator>>
-------------------------------------------------------------------------
@short  : Sobrecarga operador entrada streams
@author : André Duarte Bueno
@see    :
@param  : istream& is, CPermeabilidade& p
@return : istream& is
*/
istream& operator>> ( istream& is, CPermeabilidade& p ){
   is >> p.permeabilidade;
   return is;
}

/*
-------------------------------------------------------------------------
Função:   Read
-------------------------------------------------------------------------
@short  :
	Lê atributos do objeto do arquivo de disco
	Recebe nome do arquivo de disco
	void CPermeabilidade::Read(string nomeArquivo)
@author : André Duarte Bueno
@see    : Read
@param  : string nomeArquivo
@return : void
*/
void
CPermeabilidade::Read ( string nomeArquivo )
{
   // Abre o arquivo de disco
   ifstream fin ( nomeArquivo.c_str () );

   if ( !fin ) {
         cerr << "\nNao abriu arquivo de disco " << nomeArquivo << endl;
         exit ( 0 );
      }

  fin >> permeabilidade;
  fin.close();
}
