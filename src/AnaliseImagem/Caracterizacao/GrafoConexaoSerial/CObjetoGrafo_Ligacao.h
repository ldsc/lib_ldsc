#ifndef CObjetoGrafo_Ligacao_h
#define CObjetoGrafo_Ligacao_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoGrafo_Ligacao.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <vector>
#include <iostream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

// Definição de CObjetoGrafo
#ifndef CObjetoGrafo_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo.h>
#endif

/**
 * @brief Representa uma objeto ligação de um grafo, é herdeiro de CObjetoGrafo.
 * Tendo uma variável rótulo (herdada de CObjetoGrafo).
 *
 * A característica básica de um CObjetoGrafo_Ligacao é que este tem 2 vetores de coneccao (duas garras).
 * Cada conecção é 1 para 2 (duas garras).
 * ou seja a coneccaoA[i] e coneccaoB[i] apontam para objetos do tipo CObjetoGrafo.
 *
 * Observe a diferença,
 * um CObjetoRede_Sitio pode ter n coneccao's  mas cada conecção a um único objeto,
 * já um CObjetoGrafo_Ligacao vai ter n conexões's, e cada coneccao ocorre em pares.
 *
 * Exemplo:
 * Para sítios (uma garra)
 * sítio-->sítio              // sítio conectado a sítio
 * sítio-->ligação            // sítio conectado a ligação
 *
 * Para ligações (duas garras)
 * sítio <--ligação--> lig    // A primeira ligação após um sítio
 * lig <--ligação--> lig      // Ligação entre duas ligações
 * lig <--ligação--> sítio    // A última ligação
 * sítio <--ligação--> sítio  // Conexão entre sítios com uma única ligação
 *
 * @author  André Duarte Bueno
 * @see     grafos
 * @ingroup  HCObjetoGrafo
 */
class CObjetoGrafo_Ligacao : public CObjetoGrafo {
// --------------------------------------------------------------Atributos
public:
     /// Conexões a esquerda
     std::vector < CObjetoGrafo * >coneccaoA;

     /// Conexões a direita
     std::vector < CObjetoGrafo * >coneccaoB;

// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoGrafo_Ligacao () = default;

// --------------------------------------------------------------Destrutor

/// Destrutor
     virtual ~ CObjetoGrafo_Ligacao () = default;

// ----------------------------------------------------------------Métodos
     /**
       * @brief Função que recebe um ponteiro para um CObjetoGrafo,
       * e o inclue na lista de conexões. Lista dos objetos a quem estou conectado.
     */
     virtual void Conectar ( CObjetoGrafo *objA, CObjetoGrafo *objB = nullptr ) override ;

     /// Deleta uma conexão.
     virtual void DeletarConeccao ( unsigned int link ) override ;

     /**
     * @brief Deleta os links para objetos que foram marcados para deleção.
     * Recebe um número que identifica os objetos que foram marcados
     * para deleção, se o rótulo dos objetos conectados é igual a este parâmetro
      * a conecção é eliminada.
     */
     virtual bool DeletarConeccoesInvalidadas ( int deletado ) override ;

     /// @brief Salva atributos do objeto em disco.
     virtual std::ostream &Write ( std::ostream &os ) const override ;

//      /// @brief Salva atributos do objeto em disco no formato do Liang
//      virtual std::ostream &Write_Liang_Format ( std::ostream &os ) const  {
//           Write ( os ); // deve ser reescrita nas derivadas.
//           return os;
//      }
//      /**
//      * @brief Função usada para calcular uma propriedade.
//      */
//      virtual long double Go ( long double d = 0 )   override {
//           return 0;
//      }
//
//      /**
//       * @brief Função que calcula o fluxo associado as propriedade do objeto
//        * e suas conecções.
//       * Ou seja, considera-se que este objeto esta conectado a outros objetos
//       * e que em função das propriedades dos objetos, existe alguma informação
//        * que transita entre os objetos. Esta propriedade é calculada por esta função.
//       * Pode ser fluxo de massa, de calor, de qualquer coisa, ...
//      */
//      virtual long double Fluxo () const  override {
//           return 0;
//      }

public:
// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
     /// Sobrecarga do operador <<.
     friend std::ostream &operator<< ( std::ostream &os, CObjetoGrafo_Ligacao &obj );
     // friend istream& operator>> (istream& is, CObjetoGrafo_Ligacao& obj);

};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream &operator<< ( std::ostream &os, CObjetoGrafo_Ligacao &obj );
// istream& operator>> (istream& is, CObjetoGrafo_Ligacao& obj);

#endif
