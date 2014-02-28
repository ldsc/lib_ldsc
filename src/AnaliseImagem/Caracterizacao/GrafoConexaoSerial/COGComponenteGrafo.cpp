/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================

Desenvolvido por:Laboratorio de Desenvolvimento de Software Cientifico
			[LDSC].
@author:          André Duarte Bueno
@file:             COGComponenteGrafo.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <iomanip>
using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGComponenteGrafo.h>

// -------------------------------------------------------------------------
// Função:       Conectar
// -------------------------------------------------------------------------
/** Função: 	Conectar
@see  	:	Adiciona ao vetor conexão o vetor passado
@author:  André Duarte Bueno
@param	:	recebe um vetor de  CObjetoGrafo*
@return : void
*/
void COGComponenteGrafo::Conectar (vector < CObjetoGrafo * >obj_vetor)
{
  coneccao.push_back (obj_vetor);
}

// ------------------------------------------------------------------------------
// Funcao:     Write
// ------------------------------------------------------------------------------
/** Salva o grafo em disco (em fout)
@see  		:
@author:    André Duarte Bueno
@param		: Recebe uma ofstream& fout
@return   : ostream&
*/
ostream & COGComponenteGrafo::Write (ostream & fout) const
{
    // Tipo de contorno
    fout << setw (4) << static_cast<unsigned char>( contorno ) << '\n';

    // Número de vetores coneccoes
    fout << ' ' << setw (4) << coneccao.size ();

    // Para cada vetor percorrer a lista
    for (unsigned long int cont_coneccoes= 0; cont_coneccoes < coneccao.size (); cont_coneccoes++)
      {
        // Para cada vetor percorrer os objetos
        // coneccao[cont_vector] retorna um ponteiro para um vetor
        fout << " " << setw (4) << coneccao[cont_coneccoes].size ();

        // lista dos rótulos
        for ( auto objeto_conectado :  coneccao[cont_coneccoes] )
          fout << " " << setw (4) << objeto_conectado->rotulo;

        // lista das propriedades (condutancias)
        for ( auto objeto_conectado :  coneccao[cont_coneccoes] )
          fout << " " << setw (4) << objeto_conectado->propriedade;
  }
  return fout;
}
