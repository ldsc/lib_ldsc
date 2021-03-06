/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================
Desenvolvido por:
			Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:    André Duarte Bueno
@file:      CMFluido.cpp
@begin:     Sat Sep 16 2000
@copyright: (C) 2000 by André Duarte Bueno
@email:     andreduartebueno@gmail.com
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------
#include <fstream>
using namespace std;

//-----------------------------------------------------------------------
//Bibliotecas LIB_LDSC
//-----------------------------------------------------------------------
#include <Amostra/Material/CMFluido.h>

//-----------------------------------------------------------------------
// Funções para a classe: CMFluido
//-----------------------------------------------------------------------
/*
-------------------------------------------------------------------------
Função: Construtor de cópia
-------------------------------------------------------------------------
@short  :O Construtor de cópia cria uma cópia do objeto
@author :André Duarte Bueno
 constructor
@param  :Uma referência para um objeto
@return :sem retorno
*/
/*
CMFluido::CMFluido (const CMFluido & fluido)
{
  viscosidade = fluido.viscosidade;
  densidade = fluido.densidade;
  compressibilidade = fluido.compressibilidade;
  molhabilidade = fluido.molhabilidade;
}*/

/*
-------------------------------------------------------------------------
Função: OPerador igualdade (atribuição)
-------------------------------------------------------------------------
@short  :Operador de igualdade, copia todos os atributos do objeto
@author :André Duarte Bueno
 operador=
@param  :Uma referência para um objeto
@return :o próprio objeto (pode haver encadeamento)
*//*

CMFluido & CMFluido::operator= (const CMFluido & fluido)
{
  //Verifica se não é o próprio objeto
  if (this == &fluido)
    return *this;

  //Copia cada atributo
  viscosidade = fluido.viscosidade;
  densidade = fluido.densidade;
  compressibilidade = fluido.compressibilidade;
  molhabilidade = fluido.molhabilidade;

  return *this;
}*/

/*
-------------------------------------------------------------------------
OPerador comparação igualdade
-------------------------------------------------------------------------
@short  :Operador de comparação de igualdade, compara todos os atributos
@author :André Duarte Bueno
 operador==
@param  :Uma referência para um objeto
@return :bool
*/
bool CMFluido::operator== (const CMFluido & fluidoB)  const
{
    return
        ((this->compressibilidade 	== fluidoB.viscosidade) 		and
         (this->densidade 			== fluidoB.densidade) 			and
         (this->compressibilidade 	== fluidoB.compressibilidade)	and
         (this->molhabilidade 		== fluidoB.molhabilidade));
}

/*
-------------------------------------------------------------------------
Sobrecarga operador saída stream
-------------------------------------------------------------------------
@short  :Operador de iostream
@author :André Duarte Bueno
 operador<<
@param  :uma ostream e uma referência para um objeto
@return :ostream
*/
ostream & operator<< (ostream & os, const CMFluido & f)
{
		os << "Viscosidade: "					<< f.viscosidade
			 << "\nDensidade: "					<< f.densidade
			 << "\nCompressibilidade: " << f.compressibilidade
			 << "\nMolhabilidade: "			<< f.molhabilidade;
    return os;
}

/*
-------------------------------------------------------------------------
Função Read
-------------------------------------------------------------------------
Lê atributos do objeto do arquivo de disco
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
 operador>>
@param  :uma istream e um ponteiro para um objeto
@return :ostream
*/
istream & operator>> (istream & is, CMFluido & f)
{
    is >> f.viscosidade
       >> f.densidade
       >> f.compressibilidade
       >> f.molhabilidade;
    return is;
}
