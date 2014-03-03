/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================

Desenvolvido por:	
			  Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:      André Duarte Bueno
@file:        CObjetoGrafo.cpp
@begin:       Sat Sep 16 2000
@copyright:   (C) 2000 by André Duarte Bueno
@email:       andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>
#include <iomanip>
using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo.h>

// -------------------------------------------------------------------------
// Função:       Write
// -------------------------------------------------------------------------
/** Salva dados do objeto sítio em novo formato.
    @short  : 	
    Formato novo (Andre Format):
    ----------------------------
    NumeroSitios  		// salvo pelo grafo
    Tipo
    Rotulo
    propriedade 		// raio hidraulico ou condutancia
    x           		// pressão
    NumeroConeccoes
    Lista_dos_rotulos_das_coneccoes

    @author :		André Duarte Bueno
    @see    :
    @param  :   Recebe uma referencia para uma ostream
    @return :		ostream&
*/
ostream & CObjetoGrafo::Write (ostream & out) const
{
  out.setf (ios::right);

  // Tipo de contorno
  out << setw (4) << static_cast<unsigned char>( Contorno() ) << '\n';

  // Rótulo de this
  out << ' ' << setw (5) << rotulo;

  return out;
}
/**
-------------------------------------------------------------------------
Função:    operator<<
-------------------------------------------------------------------------
@short  : Escreve em os as propriedades do objeto
@author : André Duarte Bueno
@see    :
@param  : const CContorno &
@return : ostream&
*/
ostream &operator<< ( ostream &os, const CObjetoGrafo &obj )
{
  os.setf (ios::right);
     os << static_cast<unsigned char> ( obj.Contorno() ) << ' '
        << obj.rotulo << ' ';
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
istream  &operator>> ( istream &is,  CObjetoGrafo &obj )
{
  unsigned char contorno; ///@bug: corrigir!
  is >> contorno;
  is >> obj.rotulo;
  return is;
}
