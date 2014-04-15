#ifndef CGrafo_h
#define CGrafo_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CGrafo.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>
// #include <vector>
#include <cassert>

// ----------------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// ----------------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CBaseGrafoRedeEsqueleto_h
#include <EstruturaDados/CBaseGrafoRedeEsqueleto.h>
#endif

#ifndef CObjetoGrafo_h
#include <EstruturaDados/ObjetoGrafo/CObjetoGrafo.h>
#endif

#ifndef CObjetoGrafo_1VetorConexoes_h
#include <EstruturaDados/ObjetoGrafo/CObjetoGrafo_1VetorConexoes.h>
#endif

#ifndef CObjetoGrafo_2VetoresConexoes_h
#include <EstruturaDados/ObjetoGrafo/CObjetoGrafo_2VetoresConexoes.h>
#endif

#ifndef CObjetoGrafo_MatrizConexoes_h
#include <EstruturaDados/ObjetoGrafo/CObjetoGrafo_MatrizConexoes.h>
#endif

// ===============================================================================
// Documentacao Classe: CGrafo
// ===============================================================================
/**
 * @brief Um CGrafo é uma representação para uma estrutura de dados.
 * Um CGrafo é composto de um vetor de objetos do tipo CObjetoGrafo.
 * A forma como os objetos se relacionam é definida, normalmente, pelo próprio CObjetoGrafo.
 * Assim, existe uma hierarquia de grafos cujo pai é CGrafo e
 * uma hierarquia de objetos de grafo cujo pai é CObjetoGrafo.
 *
 * @author André Duarte Bueno
 * @see    grafos
 * @ingroup  HCGrafo
*/
class  CGrafo : public CBaseGrafoRedeEsqueleto {
   // --------------------------------------------------------------Atributos
public:
   /// Usa-se objeto[i] para obter ponteiro para o objeto i do grafo
   /// @todo: trocar por unique_ptr shared_ptr ??
   std::vector <CObjetoGrafo*> objeto;

   // -------------------------------------------------------------Construtor
   /// Construtor default.
   CGrafo( )  = default;

   // -------------------------------------------------------------Construtor
   /// Constroi o grafo, recebe um nome de arquivo de disco que tem as informações do grafo.
   CGrafo ( std::string _na ) : CBaseGrafoRedeEsqueleto ( _na ) { }

   // --------------------------------------------------------------Destrutor
   /// Destroi o objeto, como o grafo é o proprietário dos objetos deve eliminá-los.
   virtual ~CGrafo()  {
      for ( auto objeto_i :  objeto )
         delete objeto_i;
   }

   // ----------------------------------------------------------------Métodos
protected:
   /**
    * @brief  Função usada para criar os objetos do grafo.
    * @param  Recebe um ETipoObjetoGrafo, que informa o tipo de objeto a ser criado.
    * @return Retorna um objeto herdeiro de CObjetoGrafo, de acordo com o ETipoGrafo.
   */
   /*virtual */
   CObjetoGrafo* CriarObjeto ( ETipoObjetoGrafo tipo ) {
      CObjetoGrafo* data;

      switch ( tipo ) {
         case ETipoObjetoGrafo::ObjetoGrafo_1VetorConexoes :
            data = new CObjetoGrafo_1VetorConexoes();
            break;

         case ETipoObjetoGrafo::ObjetoGrafo_2VetoresConexoes:
            data = new CObjetoGrafo_2VetoresConexoes();
            break;

         case ETipoObjetoGrafo::ObjetoGrafo_MatrizConexoes :
            data = new CObjetoGrafo_MatrizConexoes();
            break;

         default :
            data = new CObjetoGrafo_1VetorConexoes();
            break;
         }

      assert ( data );
      return data;
   }

   /// Deleta um objeto do grafo, considerando a posição no vetor.
   /// Note que pode ser lenta pois implica movimentação dados vetor.
   virtual bool DeletarObjeto ( int i ) override {
			delete objeto[i];
      objeto.erase ( objeto.begin() + i );
			return true;
   }

   /// Deleta considerando o endereço do objeto (nova).
   /// @todo: testar
   virtual bool DeletarObjeto ( CObjetoGrafo* objeto_i )  {
			delete objeto_i;
      int posicao_i = objeto_i - *objeto.data();
      objeto.erase ( objeto.begin() + posicao_i );
			return true;
   }

public:
   /**
    * @brief Função que monta o grafo, genérica.
   */
//    virtual CGrafo * Go( long double, long double ) = 0;

   /**
    * @brief Fun que salva o grafo e seus objetos em disco, recebe a ofstream.
    */
   virtual void Write ( std::ostream& out ) const override;

   // --------------------------------------------------------------------Get
   // --------------------------------------------------------------------Set
   // -----------------------------------------------------------------Friend
   /// Escreve em "os" os dados do objeto grafo e seus agregados
   friend std::ostream& operator<< ( std::ostream& os, const CGrafo& obj );

   // friend istream& operator>> (istream& is, CGrafo& obj);
};

std::ostream& operator<< ( std::ostream& os, const CGrafo& obj );
// istream& operator>> (istream& is, CGrafo& obj);
#endif
