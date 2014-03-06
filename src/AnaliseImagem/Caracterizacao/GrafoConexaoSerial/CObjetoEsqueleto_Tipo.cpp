/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================

Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:    André Duarte Bueno
@file:      CObjetoEsqueleto_Tipo.cpp
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoEsqueleto_Tipo.h>

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
    Lista_dos_rotulos_das_conexões

    @author :		André Duarte Bueno
    @see    :
    @param  :   Recebe uma referencia para uma ostream
    @return :		ostream&
*/
ostream& CObjetoEsqueleto_Tipo::Write ( ostream& out ) const
{
    out.setf ( ios::right );

    // Tipo de contorno
    out << setw ( 5 ) << static_cast<uint8_t> ( Tipo () ) << '\n';

    // Rótulo de this
    out << ' ' << setw ( 5 ) << rotulo;

    // x de this (pressão)
    out << ' ' << setw ( 10 ) << x;

    // propriedade de this (condutancia)
    out << ' ' << setw ( 10 ) << propriedade;

    // Numero de links do sítio
    out << ' ' << setw ( 5 ) << conexao.size ();

    // lista dos rótulos da conexao
    for ( auto objeto_conectado : conexao )
        out << ' ' << setw ( 5 ) << objeto_conectado->rotulo;

    // Lista das propriedades (condutâncias das ligações)
    for ( auto condutancia_i : condutancia )
        out << ' ' << setw ( 10 ) << condutancia_i;

    // coordenadas
    out << ' ' << cx << ' ' << cy << ' ' << cz << ' ';

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
ostream& operator<< ( ostream& out, CObjetoEsqueleto_Tipo& s )
{
    s.Write ( out );
    return out;
}
