/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================
Desenvolvido por:
			Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:    André Duarte Bueno
@file:      CSimPermeabilidade.cpp
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
#include <AnaliseImagem/Simulacao/Permeabilidade/CSimPermeabilidade.h>

using namespace std;

// /*
// -------------------------------------------------------------------------
// Função:  operator=
// -------------------------------------------------------------------------
// @short  : OPerador igualdade (atribuição)
// @author : André Duarte Bueno
//  
// @param  : CSimPermeabilidade&
// @return : *this
// */
// CSimPermeabilidade & CSimPermeabilidade::
// operator= (const CSimPermeabilidade & aCSimPermeabilidade)
// {
//   if (this == &aCSimPermeabilidade)
//     return *this;
//
//   permeabilidade = aCSimPermeabilidade.permeabilidade;
//   return *this;
// }

/*
-------------------------------------------------------------------------
Função:  operator==
-------------------------------------------------------------------------
@short  : Operador comparação igualdade
@author : André Duarte Bueno
 
@param  : const CSimPermeabilidade&
@return : bool==1 se iguais, ==0 se diferentes
*/
bool CSimPermeabilidade::operator== ( const CSimPermeabilidade& ori )  const
{
   return this->permeabilidade == ori.permeabilidade;
}

/*
-------------------------------------------------------------------------
Função:  operator!=
-------------------------------------------------------------------------
@short  :  Operador diferença
@author :  André Duarte Bueno
 
@param  :  const CSimPermeabilidade&
@return :  bool==0 se iguais, ==1 se diferentes
*/
bool CSimPermeabilidade::operator!= ( const CSimPermeabilidade& ori )  const{
   return this->permeabilidade != ori.permeabilidade;
}

/*
-------------------------------------------------------------------------
Função: operator<<
-------------------------------------------------------------------------
@short  : Salva atributos do objeto em disco
@author : André Duarte Bueno
 
@param  : ostream& os, const CSimPermeabilidade& p
@return : ostream& os,
*/
ostream& operator<< ( ostream& os, const CSimPermeabilidade& p ){
   os << p.erro << '\n';
   os << p.permeabilidade << '\n';
   return os;
}

/*
-------------------------------------------------------------------------
Função:  operator>>
-------------------------------------------------------------------------
@short  : Sobrecarga operador entrada streams
@author : André Duarte Bueno
 
@param  : istream& is, CSimPermeabilidade& p
@return : istream& is
*/
istream& operator>> ( istream& is, CSimPermeabilidade& p ){
   is >> p.erro;
   is >> p.permeabilidade;
   return is;
}

/*
-------------------------------------------------------------------------
Função:   Read
-------------------------------------------------------------------------
@short  :
	Recebe nome do arquivo de disco e a seguir le atributos do objeto do arquivo
@author : André Duarte Bueno
  Read
@param  : string nomeArquivo
@return : void
*/
void
CSimPermeabilidade::Read ( string nomeArquivo )
{
   // Abre o arquivo de disco
   ifstream fin ( nomeArquivo.c_str () );

   if ( !fin ) {
         cerr << "\nNao abriu arquivo de disco " << nomeArquivo << endl;
         exit ( 0 );
      }

  fin >> erro;
  fin >> permeabilidade;
  fin.close();
}
