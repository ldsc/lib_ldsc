/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================

Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:    André Duarte Bueno
@file:      CObjetoRede_Sitio_CC_CM.cpp
@begin:     Sat Sep 16 2000
@copyright: (C) 2000 by André Duarte Bueno
@email:     andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include<fstream>
#include<iomanip>
using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Sitio_CC_CM.h>


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
ostream& CObjetoRede_Sitio_CC_CM::Write ( ostream& out ) const
{
    out.setf ( ios::right );

    // Tipo de contorno
    /// @todo trocar por tipo ojeto grafo!
    out << setw ( 4 ) << static_cast<unsigned char> ( Contorno() ) << '\n';

    // Rótulo de this
    out << ' ' << setw ( 5 ) << rotulo;

    // x de this (pressão)
    out << ' ' << setw ( 10 ) << x;

    // propriedade de this (condutancia)
    out << ' ' << setw ( 10 ) << propriedade;

    // Numero de links do sítio
    out << ' ' << setw ( 4 ) << conexao.size ();

    // lista dos rótulos da conexao
    for ( auto objeto_conectado : conexao )
        out << ' ' << setw ( 4 ) << objeto_conectado->rotulo;

    // Lista das propriedades (condutancias das ligações)
    for ( auto condutancia_i : condutancia )
        out << ' ' << setw ( 12 ) << condutancia_i;

    // coordenadas
    out << cx << ' ' << cy << ' ' << cz << ' ';

    return out;
}

// -------------------------------------------------------------------------
// Função:       operator<<
// -------------------------------------------------------------------------
/** Escreve os atributos do objeto em disco.
    @short  :	Sobrecarga operador entrada streams (antiga CObjetoRede_SitioEsqueleto).
    @author :	André Duarte Bueno
    @see    :
    @param  :	ostream& e CObjetoRede_Sitio&
    @return :	ostream&
*/
ostream& operator<< ( ostream& out, CObjetoRede_Sitio_CC_CM& s )
{
    s.Write ( out );
    return out;
}
