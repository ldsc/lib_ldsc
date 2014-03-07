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
#include <EstruturaDados/ObjetoGrafo/CObjetoGrafo.h>
#endif

/// Agrega o conjunto de classes que estão diretamente relacionadas a hierarquia HCGrafo.
/// @defgroup HCGrafo

// ===============================================================================
// Documentacao Classe: CBaseGrafoRedeEsqueleto
// ===============================================================================
/**
 * @brief Um CBaseGrafoRedeEsqueleto é uma representação para uma estrutura de dados.
 * Um CBaseGrafoRedeEsqueleto é composto de uma lista de objetos do tipo CObjetoGrafo.
 * A forma como os objetos se relacionam é definida, normalmente, pelo próprio CObjetoGrafo.
 * Assim, existe uma hierarquia de grafos cujo pai é CBaseGrafoRedeEsqueleto e
 * uma hierarquia de objetos de grafo cujo pai é CObjetoGrafo.
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
      BaseGrafoRedeEsqueleto,///< representa classe base virtual
      Grafo,                 ///< representa grafo criado por CGrafo
      Grafo_Contorno,        ///< representa grafo criado por CGrafo_Contorno
      GrafoConexaoSerial,    ///< representa grafo criado por CGrafoConexaoSerial
      GrafoConexaoSerial_M1, ///< representa grafo criado por CGrafoConexaoSerial_M1
      GrafoConexaoSerial_M2, ///< representa grafo criado por CGrafoConexaoSerial_M2
      GrafoConexaoSerial_M3, ///< representa grafo criado por CGrafoConexaoSerial_M3
      GrafoConexaoSerial_M4, ///< representa grafo criado por CGrafoConexaoSerial_M4
      GrafoConexaoSerial_M5, ///< representa grafo criado por CGrafoConexaoSerial_M5
      GrafoConexaoSerial_M6  ///< representa grafo criado por CGrafoConexaoSerial_M6
   };

   /// Identifica o tipo de grafo, deve ser definido no construtor.
   ETipoGrafo tipoGrafo { ETipoGrafo::Grafo } ;

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
   /**
    * @brief  Função usada para criar os objetos do grafo.
    * @param  Recebe um ETipoObjetoGrafo, que informa o tipo de objeto a ser criado.
    * @return Retorna um objeto herdeiro de CObjetoGrafo, de acordo com o ETipoGrafo.
   */
   virtual CObjetoGrafo * CriarObjetoGrafo( ETipoObjetoGrafo tipo ) = 0;

   /// Deleta um objeto do grafo, considerando a posição no vetor.
   virtual bool DeletarObjeto( int i ) = 0 ;

   /// Deleta considerando o endereço do objeto.
   /// @todo: testar
   virtual bool DeletarObjeto( CObjetoGrafo * objeto_i )  = 0;

public:
   /**
    * @brief Função que monta o grafo, genérica.
   */
   virtual CBaseGrafoRedeEsqueleto * Go( long double, long double ) {};

   /**
    * @brief Fun que salva o grafo e seus objetos em disco.
    */
   void Write() {
      std::ofstream out( NomeGrafo().c_str() );

      if( out.fail() )  {
            std::cerr << " Não conseguiu abrir o arquivo de disco " << nomeArquivo;
            return;
         }

      Write( out );
      out.close();
   }
   /**
    * @brief Fun que salva o grafo e seus objetos em disco, recebe a ofstream.
    */
   virtual void Write( std::ostream & out ) const = 0;

   // --------------------------------------------------------------------Get
   /// Retorna o nome do grafo que inclui, como extensão, o tipo grafo
   std::string NomeGrafo()  const {
      return nomeArquivo + "." + TipoGrafoString();
   }

   /// Retorna o tipo de grafo
   ETipoGrafo TipoGrafo( ) {
      return tipoGrafo;
   }

   /// Retorna o tipo do grafo como uma string, útil para gerar nomes arquivos saída.
   std::string TipoGrafoString( ) const {
      switch( tipoGrafo ) {
         case ETipoGrafo::Grafo          :
            return {"Grafo"};
         case ETipoGrafo::Grafo_Contorno  :
            return {"Grafo_Contorno"};
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
   }

   // --------------------------------------------------------------------Set
   // -----------------------------------------------------------------Friend
   /// Escreve em "os" os dados do objeto grafo e seus agregados
   friend std::ostream & operator<< ( std::ostream & out, const CBaseGrafoRedeEsqueleto & grafo );

   // friend istream& operator>> (istream& is, CBaseGrafoRedeEsqueleto& obj);
};

std::ostream & operator<< ( std::ostream & out, const CBaseGrafoRedeEsqueleto & grafo )
{
   grafo.Write( out );
   return out;
}
// istream& operator>> (istream& is, CBaseGrafoRedeEsqueleto& obj);
#endif

