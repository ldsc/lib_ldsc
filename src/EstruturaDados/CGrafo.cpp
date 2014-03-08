/**
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
  ===============================================================================
  Desenvolvido por:
                    Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
  @author:          André Duarte Bueno
  @file:            CGrafo.cpp
  @begin:           Sat Sep 16 2000
  @copyright:       (C) 2000 by André Duarte Bueno
  @email:           andreduartebueno@gmail.com
*/
// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <cassert>
#include <iomanip>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef CGrafo_h
#include <EstruturaDados/CGrafo.h>
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
void CGrafo::Write ( std::ostream& out ) const
{
   out.setf ( ios::right );

   // Tipo de grafo
   out << setw ( 5 ) << static_cast<uint8_t> ( Tipo() ) << '\n';

   // Numero de objetos
   out << setw ( 5 ) << objeto.size() << endl;

   // Percorre os objetos e salva em disco as informações de cada objeto.
   for ( auto objeto_i :  objeto ) {
         objeto_i->Write ( out );	 
		 // out << (*objeto[i]) << '\n';
		 // objeto_i >> out;   <-> objeto_i << in;
		 // objeto_i >> arquivo;   <-> objeto_i << arquivo;
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
    @param  : Objeto do tipo CGrafo.
    @return : ostream&
    @test   : Testar/verificar os diferentes tipos de arquivos de grafo gerados.
*/
ostream& operator<< ( ostream& out, CGrafo& grafo )
{
   grafo.Write ( out );
   return out;
}
