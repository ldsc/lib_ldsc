#ifndef CRede_h
#define CRede_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CRede.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/
// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>
// #include <vector>
// #include <cassert>

// ----------------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// ----------------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CBaseGrafoRedeEsqueleto_h
#include <EstruturaDados/CBaseGrafoRedeEsqueleto.h>
#endif

#ifndef CObjetoRede_h
#include <EstruturaDados/ObjetoRede/CObjetoRede.h>
#endif

#ifndef CObjetoRede_Ligacao_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Ligacao.h>
#endif

#ifndef CObjetoRede_Ligacao_EST_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Ligacao_EST.h>
#endif

#ifndef CObjetoRede_Ligacao_WEST_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Ligacao_WEST.h>
#endif

#ifndef CObjetoRede_Sitio_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Sitio.h>
#endif

#ifndef CObjetoRede_Sitio_EST_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Sitio_EST.h>
#endif

#ifndef CObjetoRede_Sitio_WEST_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Sitio_WEST.h>
#endif

#ifndef CObjetoRede_Final_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Final.h>
#endif

#ifndef CObjetoRede_Tipo_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Tipo.h>
#endif

// #include <iostream>

#ifdef OTIMIZAR_VELOCIDADE_PROCESSAMENTO // usar template!
   using value_type_objeto = CObjetoRede_Final;
// std::cout << "debug: using value_type_objeto = CObjetoRede_Final;\n";
#else
   using value_type_objeto = CObjetoRede;
// std::cout << "debug:    using value_type_objeto = CObjetoRede;\n";
#endif
// ===============================================================================
// Documentacao Classe: CRede
// ===============================================================================
/**
 * @brief Um CRede é uma representação para uma estrutura de dados.
 * Um CRede é composto de uma lista de objetos do tipo CObjetoRede.
 * A forma como os objetos se relacionam é definida, normalmente, pelo próprio CObjetoRede.
 * Assim, existe uma hierarquia de grafos cujo pai é CRede e
 * uma hierarquia de objetos de grafo cujo pai é CObjetoRede.
 * Nota: a criação dos objetos e suas ligações é feita aqui; também é em CRede que calculamos
 * as condutâncias entre os objetos da rede.
 * 
 * Note que CObjetoRede_Tipo não é herdeiro de CObjetoRede, seu uso requer
 * modificação do vetor de objetos (uso de #ifdef OTIMIZAR_VELOCIDADE_PROCESSAMENTO).
 *
 * @author André Duarte Bueno
 * @see    grafos
 * @ingroup  HCGrafo
*/
class  CRede : public CBaseGrafoRedeEsqueleto {
   // --------------------------------------------------------------Atributos
public:
   /// Usa-se objeto[i] para obter ponteiro para o objeto i da rede
   /// @todo: trocar por unique_ptr shared_ptr ??
   //    std::vector <CObjetoRede *> objeto;

   /// Vetor de ponteiros para objetos da rede, normalmente CObjetoRede.
   std::vector < value_type_objeto* > objeto;

   // -------------------------------------------------------------Construtor
   /// Construtor default.
   CRede( )  = default;

   // -------------------------------------------------------------Construtor
   /// Constroi o grafo, recebe um nome de arquivo de disco que tem as informações do grafo.
   CRede ( std::string _na ) : CBaseGrafoRedeEsqueleto ( _na ) { }

   // --------------------------------------------------------------Destrutor
   /// Destroi o objeto, como o grafo é o proprietário dos objetos deve eliminá-los.
   virtual ~CRede()  {
      for ( auto objeto_i :  objeto )
         delete objeto_i;
   }

   // ----------------------------------------------------------------Métodos
protected:
   /**
    * @brief  Função usada para criar os objetos (aqui sempre cria objetos herdeiros de CObjetoRede).
    * @param  Recebe um ETipoObjetoGrafo, que informa o tipo de objeto a ser criado.
    * @return Retorna um objeto herdeiro de CObjetoGrafo, de acordo com o ETipoGrafo.
	* Note que a criação e destruição dos objetos da rede é de responsabilidade da rede;
	* Os objetos tem funções para deletar as conexões, nunca deletam os objetos.
   */
   value_type_objeto* CriarObjeto ( ETipoObjetoGrafo tipo ) ;

   /// Deleta um objeto do grafo, considerando a posição no vetor.
   /// Pode ser lenta em Função da necessidade de mover muitos dados!
   virtual bool DeletarObjeto ( int i ) override {
      delete objeto[i];                      // deleta o objeto
      objeto.erase ( objeto.begin() + i );   // apaga o ponteiro para o objeto deletado
   }

   /// Deleta considerando o endereço do objeto.
   /// @todo: testar
   virtual bool DeletarObjeto ( value_type_objeto* objeto_i ) { /*override*/
      delete objeto_i;
      // Para calcular valor i, preciso diminuir o ponteiro para objeto_i
      // do ponteiro para início do vetor, que é dado por objeto.data();
      int posicao_i = objeto_i - ( *objeto.data() );
      objeto.erase ( objeto.begin() + posicao_i );
   }

//    /// Deleta um conjunto de objetos (recebe vector com lista dos índices a serem deletados).
//    virtual bool DeletarObjeto( std::vector<unsigned int> i ) override ;
   
public:
   /**
    * @brief Função que monta o grafo, genérica.
	* Deve ser reescrita nas classes herdeiras.
   */
    virtual CRede * Go( long double, long double ) {};

   /**
    * @brief Fun que salva o grafo e seus objetos em disco, recebe a ofstream.
    */
   virtual void Write ( std::ostream& out ) const override;

   // --------------------------------------------------------------------Get
   // --------------------------------------------------------------------Set
   // -----------------------------------------------------------------Friend
   /// Escreve em "os" os dados do objeto grafo e seus agregados
   friend std::ostream& operator<< ( std::ostream& os, const CRede& obj );

   // friend istream& operator>> (istream& is, CRede& obj);
   // Vai criar apelido
};

std::ostream& operator<< ( std::ostream& os, const CRede& obj );
// istream& operator>> (istream& is, CRede& obj);
#endif
