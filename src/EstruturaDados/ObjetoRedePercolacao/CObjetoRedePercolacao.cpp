/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================

Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:    Leandro Puerari
@file:      CObjetoRedePercolacao.cpp
@begin:     Sat Sep 16 2000
@copyright: (C) 2000 by Leandro Puerari
@email:     puerari@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include<fstream>
// #include<iomanip>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <EstruturaDados/ObjetoRedePercolacao/CObjetoRedePercolacao.h>
using namespace std;

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

    @author :		Leandro Puerari
     
    @param  :   Recebe uma referencia para uma ostream
    @return :		ostream&
*/
ostream& CObjetoRedePercolacao::Write ( ostream& out ) const
{
    CObjetoRede::Write(out);
    // adiciona as coordenadas
    out << ' ' << cx << ' ' << cy << ' ' << cz /*<< '\n'*/;

    return out;
}

// -------------------------------------------------------------------------
// Função:       operator<<
// -------------------------------------------------------------------------
/** Escreve os atributos do objeto em disco.
    @short  :	Sobrecarga operador entrada streams (antiga CObjetoRede_SitioEsqueleto).
    @author :	Leandro Puerari
     
    @param  :	ostream& e CObjetoRede_Sitio&
    @return :	ostream&
*/
ostream& operator<< ( ostream& out, CObjetoRedePercolacao& s )
{
    s.Write ( out );
    return out;
}
