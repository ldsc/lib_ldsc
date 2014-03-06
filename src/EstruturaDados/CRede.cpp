/**
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
  ===============================================================================
  Desenvolvido por:
                    Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
  @author:          André Duarte Bueno
  @file:            CRede.cpp
  @begin:           Sat Sep 16 2000
  @copyright:       (C) 2000 by André Duarte Bueno
  @email:           andreduartebueno@gmail.com
*/
// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cassert>
#include <iomanip>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef CRede_h
#include <EstruturaDados/CRede.h>
#endif
using namespace std;

// -------------------------------------------------------------------------
// Função:     Write
// -------------------------------------------------------------------------
/** @short  : Salva em disco o grafo.
    Formato (Andre):
    NumeroTotalObjetos 	// salvo pelo grafo
    TipoDoObjeto        // tipo do objeto
    RotuloDoObjeto      // rótulo
    propriedadeDoObjeto	// propriedade (ex: raio hidraulico ou condutancia)
    x           	// propriedade a ser calculada (ex: pressão).
    NumeroConeccoes     // número de objetos conectados.
    Lista_dos_rotulos_das_conexões

    @author : André Duarte Bueno
    @see    : grafos
    @param  : Nome do arquivo de disco (string)
    @return : void
*/
void CRede::Write( std::ostream & out )
{
   out.setf( ios::right );

   // Numero de objetos
   out << setw( 5 ) << objeto.size() << endl;

   // Percorre os objetos e salva em disco as informações de cada objeto.
   for( auto objeto_i :  objeto ) {
         objeto_i->Write( out );	 // out << (*objeto[i]) << '\n';
         out << '\n';
      }
}

// -------------------------------------------------------------------------
// Função:       operator<<
// -------------------------------------------------------------------------
/** @short  : Salva dados do objeto em os (disco).
    Salva o número de objetos que fazem parte de grafo e a seguir os dados de cada objeto.
    @author : André Duarte Bueno
    @see    : grafos
    @param  : Objeto do tipo CRede.
    @return : ostream&
    @test   : Testar/verificar os diferentes tipos de arquivos de grafo gerados.
*/
ostream & operator<< ( ostream & out, CRede & grafo )
{
   grafo.Write( out );
   return out;
}
