#ifndef CBaseGrafoRedeEsqueleto_h
#define CBaseGrafoRedeEsqueleto_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CBaseGrafoRedeEsqueleto.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>

// ----------------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// ----------------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CObjetoGrafo_h
#include <EstruturaDados/ObjetoGrafo/CObjetoGrafo.h> // ETipoObjetoGrafo
#endif

/// Agrega o conjunto de classes que estão diretamente relacionadas a hierarquia HCGrafo.
/// @defgroup HCGrafo Hierarquia para diferentes tipos de  grafos

// ===============================================================================
// Documentacao Classe: CBaseGrafoRedeEsqueleto
// ===============================================================================
/**
 * @brief Um CBaseGrafoRedeEsqueleto é uma classe base para as classes
 * CBaseGrafoRedeEsqueleto->CGrafo CBaseGrafoRedeEsqueleto->CRede->CEsqueleto, 
 * que são representações para estruturas de dados.
 *
 * @author André Duarte Bueno
 * @see    grafos
 * @ingroup  HCGrafo
*/
class  CBaseGrafoRedeEsqueleto {
   // --------------------------------------------------------------Atributos
protected:
   /// Nome do arquivo de disco (nome do arquivo do grafo).
   std::string nomeArquivo {"nomeArquivoIndefinido"};

   /// Enumeração para os diferentes tipos de grafo.
   /// @enum: ETipoGrafo
   // Note que sempre que criar classe herdeira tem de colocar aqui a enumeração correspondente.
   enum class ETipoGrafo : uint8_t {
      BaseGrafoRedeEsqueleto = 101,///< representa classe base virtual
      Grafo = 102,                 ///< representa grafo criado por CGrafo
      Rede = 103,                  ///< representa rede  criada por CRede
      Rede_Contorno = 104,         ///< representa rede  criada por CRede_Contorno
      GrafoConexaoSerial =200,    ///< representa grafo criado por CGrafoConexaoSerial (é uma rede!)
      GrafoConexaoSerial_M1 = 201, ///< representa grafo criado por CGrafoConexaoSerial_M1
      GrafoConexaoSerial_M2 = 202, ///< representa grafo criado por CGrafoConexaoSerial_M2
      GrafoConexaoSerial_M3 = 203, ///< representa grafo criado por CGrafoConexaoSerial_M3
      GrafoConexaoSerial_M4 = 204, ///< representa grafo criado por CGrafoConexaoSerial_M4
      GrafoConexaoSerial_M5 = 205, ///< representa grafo criado por CGrafoConexaoSerial_M5
      GrafoConexaoSerial_M6 = 206  ///< representa grafo criado por CGrafoConexaoSerial_M6
   };

   /// Identifica o tipo de grafo, nas herdeiras deve ser definido no construtor.
   ETipoGrafo tipoGrafo { ETipoGrafo::BaseGrafoRedeEsqueleto } ;

public:
   // -------------------------------------------------------------Construtor
   /// Construtor default.
   CBaseGrafoRedeEsqueleto( )  = default;

   // -------------------------------------------------------------Construtor
   /// Constroi o grafo, recebe um nome de arquivo de disco que tem as informações do grafo.
   CBaseGrafoRedeEsqueleto( std::string _na ) : nomeArquivo( _na ) { }

   // --------------------------------------------------------------Destrutor
   /// Destroi o objeto, como o grafo é o proprietário dos objetos deve eliminá-los.
   virtual ~CBaseGrafoRedeEsqueleto()  = default;

   // ----------------------------------------------------------------Métodos
protected:

   /// Deleta um objeto do grafo, considerando a posição no vetor.
   virtual bool DeletarObjeto( int i ) = 0 ;


public:
   /**
    * @brief Função que salva o grafo e seus objetos em disco.
    */
   void Write() const {
      std::ofstream out( NomeGrafo().c_str() );
      if( out.fail() )  {
            std::cerr << " Não conseguiu abrir o arquivo de disco " << nomeArquivo;
            return;
         }
      Write( out );
      out.close();
   }

   /**
    * @brief Função que salva o grafo e seus objetos em disco, recebe a ostream.
    */
   virtual void Write( std::ostream & out ) const = 0;

   // --------------------------------------------------------------------Get
   /// Retorna o nome do grafo que inclui, como extensão, o tipo grafo
   std::string NomeGrafo()  const {
      return nomeArquivo + "." + TipoString();
   }

   /// Retorna o tipo de grafo
   ETipoGrafo Tipo( ) const {
      return tipoGrafo;
   }

   /// Retorna o tipo do grafo como uma string, útil para gerar nomes arquivos saída.
   std::string TipoString( ) const {
      switch( tipoGrafo ) {
         case ETipoGrafo::BaseGrafoRedeEsqueleto :
            return {"BaseGrafoRedeEsqueleto"};
         case ETipoGrafo::Grafo          :
            return {"Grafo"};
         case ETipoGrafo::Rede    :
            return {"Rede"};
         case ETipoGrafo::Rede_Contorno  :
            return {"Rede_Contorno"};
         case ETipoGrafo::GrafoConexaoSerial    :
            return {"GrafoConexaoSerial"};
         case ETipoGrafo::GrafoConexaoSerial_M1 :
            return {"GrafoConexaoSerial_M1"};
         case ETipoGrafo::GrafoConexaoSerial_M2 :
            return {"GrafoConexaoSerial_M2"};
         case ETipoGrafo::GrafoConexaoSerial_M3 :
            return {"GrafoConexaoSerial_M3"};
         case ETipoGrafo::GrafoConexaoSerial_M4 :
            return {"GrafoConexaoSerial_M4"};
         case ETipoGrafo::GrafoConexaoSerial_M5 :
            return {"GrafoConexaoSerial_M5"};
         case ETipoGrafo::GrafoConexaoSerial_M6 :
            return {"GrafoConexaoSerial_M6"};
         }
   return {"Grafo"}; // eliminar warning
   }

   // --------------------------------------------------------------------Set
   // -----------------------------------------------------------------Friend
   /// Escreve em "os" os dados do objeto grafo e seus agregados
   friend std::ostream & operator<< ( std::ostream & out, const CBaseGrafoRedeEsqueleto & grafo );
   // friend istream& operator>> (istream& is, CBaseGrafoRedeEsqueleto& obj);
};
// istream& operator>> (istream& is, CBaseGrafoRedeEsqueleto& obj);
#endif

