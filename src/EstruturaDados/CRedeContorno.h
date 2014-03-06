#ifndef CRedeContorno_h
#define CRedeContorno_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
        Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
			Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CRedeContorno.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/
// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// #ifndef _LIB_LDSC_CLASS_h
// #include <Base/_LIB_LDSC_CLASS.h>
// #endif

#ifndef CRede_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CRede.h>
#endif

#ifndef CContornoCentro_h
#include <MetNum/Contorno/CContornoCentro.h>
#endif

// ===============================================================================
// Documentacao Classe: CRedeContorno
// ===============================================================================
/**
 * @short: Representa um grafo sujeito a condições de contornos;
 * Herdeira da classe CRede, a classe CRedeContorno acrescenta a idéia de que
 * um grafo pode estar submetido a determinadas condições de contorno.
 * Na prática acrescenta um vetor que armazena os objetos de contorno.
 * Note que existe uma hierarquia para objetos de contorno, este
 * vetor terá ponteiros para objetos da classe base da hierarquia de contornos.
 * @author 	André Duarte Bueno
 * @see		Grafo
 * @ingroup  HCGrafo
*/
class CRedeContorno : public CRede {
   // --------------------------------------------------------------Atributos
public:

   /// Usa-se contorno[i] para obter ponteiro para contorno i.
   std::vector < CContornoCentro * >contorno;

   // -------------------------------------------------------------Construtor
   /// Cria os objetos de contorno e incluí no vetor contorno.
   CRedeContorno( std::string _nomeArquivo, unsigned long int _numero_contornos = 0 )
      :      CRede( _nomeArquivo ) {
      tipoGrafo  =  ETipoGrafo::Grafo_Contorno ;

      for( unsigned long int i = 0; i < _numero_contornos; i++ ) {
            CContornoCentro * obj_cont = new CContornoCentro();
            assert( obj_cont );
            contorno.push_back( obj_cont );
         }
   }

   // --------------------------------------------------------------Destrutor
   /// Delete os objetos de contorno do vetor contorno.
   virtual ~ CRedeContorno()   {
      for( auto contorno_i : contorno )
         if( contorno_i )
            delete contorno_i;
   }


   // ----------------------------------------------------------------Métodos
   // virtual void Write(std::string nomeArquivo);

public:
   // --------------------------------------------------------------------Get
   // --------------------------------------------------------------------Set
   // -----------------------------------------------------------------Friend
   // friend std::ostream &operator<< ( std::ostream &os, const CRedeContorno &obj );
   // friend istream& operator>> (istream& is, CRedeContorno& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// std::ostream &operator<< ( std::ostream &os, const CRedeContorno &obj );
// istream& operator>> (istream& is, CRedeContorno& obj);

#endif

// -----------------------------------------------------------------------
// Função     Construtor
// -----------------------------------------------------------------------
/** Recebe o número de contornos;
 *  Cria os objetos e adiciona ao vetor de contornos.
 *  O nomeArquivo é repassado a classe base.
 */

