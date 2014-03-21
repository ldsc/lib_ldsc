/*
=========================================================================
PROJETO:          Biblioteca LIB_LMPT
                  Ramo: TPadrao_ramo
=========================================================================

Desenvolvido por:	Laboratorio de Meios Porosos
		e Propriedades Termofisicas	[LMPT].
@author:          André Duarte Bueno
File:             CMFluido.cpp
begin:            Sat Sep 16 2000
copyright:        (C) 2000 by André Duarte Bueno
email:            andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------
#include <fstream>
using namespace std;

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
#include <Material/CMFluido.h>

//-----------------------------------------------------------------------
// Fun��es para a classe: CMFluido
//-----------------------------------------------------------------------
/*
-------------------------------------------------------------------------
Fun��o: Construtor de c�pia
-------------------------------------------------------------------------
@short  :O Construtor de c�pia cria uma c�pia do objeto
@author :André Duarte Bueno
@see    :constructor
@param  :Uma refer�ncia para um objeto
@return :sem retorno
*/

CMFluido::CMFluido (const CMFluido & fluido)
{
  viscosidade = fluido.viscosidade;
  densidade = fluido.densidade;
  compressibilidade = fluido.compressibilidade;
  molhabilidade = fluido.molhabilidade;
}

/*
-------------------------------------------------------------------------
Fun��o: OPerador igualdade (atribui��o)
-------------------------------------------------------------------------
@short  :Operador de igualdade, copia todos os atributos do objeto
@author :André Duarte Bueno
@see    :operador=
@param  :Uma refer�ncia para um objeto
@return :o pr�prio objeto (pode haver encadeamento)
*/

CMFluido & CMFluido::operator= (const CMFluido & fluido)
{
  //Verifica se n�o � o pr�prio objeto
  if (this == &fluido)
    return *this;

  //Copia cada atributo
  viscosidade = fluido.viscosidade;
  densidade = fluido.densidade;
  compressibilidade = fluido.compressibilidade;
  molhabilidade = fluido.molhabilidade;

  return *this;
}

/*
-------------------------------------------------------------------------
OPerador compara��o igualdade
-------------------------------------------------------------------------
@short  :Operador de compara��o de igualdade, compara todos os atributos
@author :André Duarte Bueno
@see    :operador==
@param  :Uma refer�ncia para um objeto
@return :bool
*/

bool
CMFluido::operator== (const CMFluido & fluido)  const
{
  return
    ((viscosidade == fluido.viscosidade) &&
     (densidade == fluido.densidade) &&
     (compressibilidade == fluido.compressibilidade) &&
     (molhabilidade == fluido.molhabilidade));
}

/*
-------------------------------------------------------------------------
Sobrecarga operador sa�da stream
-------------------------------------------------------------------------
@short  :Operador de iostream
@author :André Duarte Bueno
@see    :operador<<
@param  :uma ostream e uma refer�ncia para um objeto
@return :ostream
*/

/*
ostream & operator<< (ostream & os, const CMFluido & f)
{
    os << f.viscosidade				<<	endl;
  os << f.densidade					<<	endl;
  os << f.compressibilidade	<<	endl;
  os << f.molhabilidade			<<	endl;
   return os;
}
*/

/*
-------------------------------------------------------------------------
Fun��o Read
-------------------------------------------------------------------------
L� atributos do objeto do arquivo de disco
Recebe nome do arquivo de disco
*/
/*void CMFluido::Read(string nomeArquivo)
{
  ifstream fin(nomeArquivo.c_str()); 	//Abre o arquivo de disco
  if(fin)                               //se ok
  	fin >> this;                    //Envia dados para este objeto
  else
   {
  	cout << "\nEntre com os atributos do objeto - Class CMFluido";
		cout << "\nEntre com a viscosidade: ";
  	cin >> viscosidade;
		cout << "\nEntre com a densidade: ";
  	cin >> densidade;
		cout << "\nEntre com a compressibilidade: ";
  	cin >> compressibilidade;
		cout << "\nEntre com a molhabilidade: ";
  	cin >> molhabilidade;
    cin.get();
   }
}  */

/*
-----------------------------------------------------------------------
Sobrecarga operador entrada streams
-----------------------------------------------------------------------
@short  :Operador>> para istream
@author :André Duarte Bueno
@see    :operador>>
@param  :uma istream e um ponteiro para um objeto
@return :ostream
*/
 /*
istream & operator>> (istream & is, CMFluido & f)
{
  is >> f.viscosidade;
  is >> f.densidade;
  is >> f.compressibilidade;
  is >> f.molhabilidade;

  return is;
}
 */
