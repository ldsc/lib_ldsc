/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
	[LDSC].
@author:          André Duarte Bueno
File:             CPermeabilidade.cpp
begin:            Sat Sep 16 2000
copyright:        (C) 2000 by André Duarte Bueno
email:            andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cstdlib>
#include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Permeabilidade/CPermeabilidade.h>

using namespace std;

/*
-------------------------------------------------------------------------
Função: Construtor de copia
-------------------------------------------------------------------------
@short  : Copia atributos do objeto
@author :	André Duarte Bueno
@see    : Construtores
@param  : CPermeabilidade&
@return : nada
*/
// Construtor de copia
CPermeabilidade::CPermeabilidade (const CPermeabilidade & aCPermeabilidade)
{
  permeabilidade = aCPermeabilidade.permeabilidade;
}

/*
-------------------------------------------------------------------------
Função:  operator=
-------------------------------------------------------------------------
@short  : OPerador igualdade (atribuição)
@author : André Duarte Bueno
@see    :
@param  : CPermeabilidade&
@return : *this
*/
CPermeabilidade & CPermeabilidade::
operator= (const CPermeabilidade & aCPermeabilidade)
{
  if (this == &aCPermeabilidade)
    return *this;

  permeabilidade = aCPermeabilidade.permeabilidade;
  return *this;
}

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
bool CPermeabilidade::operator== (const CPermeabilidade & ori)  const
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
bool CPermeabilidade::operator!= (const CPermeabilidade & ori)  const
{
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
ostream & operator<< (ostream & os, const CPermeabilidade & p)
{
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
istream & operator>> (istream & is, CPermeabilidade & p)
{
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
	void CPermeabilidade::Read(string fileName)
@author : André Duarte Bueno
@see    : Read
@param  : string fileName
@return : void
*/
void
CPermeabilidade::Read (string fileName)
{
  // Abre o arquivo de disco
  ifstream fin (fileName.c_str ());
  if (!fin)
    {
      cerr << "\nNao abriu arquivo de disco " << fileName << endl;
      exit (0);
    }
  // se ok
  // Envia dados para este objeto
  //  if(fin.good())
  //      fin >> (*this);

}
