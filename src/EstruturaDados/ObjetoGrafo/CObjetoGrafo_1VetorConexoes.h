#ifndef CObjetoGrafo_1VetorConexoes_h
#define CObjetoGrafo_1VetorConexoes_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CObjetoGrafo_1VetorConexoes.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// Interface disco
#include <fstream>

// Classe container para vetores
#include <vector>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CObjetoGrafo_h
#include <EstruturaDados/ObjetoGrafo/CObjetoGrafo.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoGrafo_1VetorConexoes
// ===============================================================================
/**
 * @brief  Representa um objeto de um grafo que tem uma lista de conexões (Sitio).
 *
 * É herdeiro de CObjetoGrafo, tendo um rótulo (herdado de CObjetoGrafo).
 * A característica básica de um sítio é que este pode ter n conexao's,
 * ou seja pode estar conectado a n objetos do tipo CObjetoGrafo,
 * mas esta conexão é 1 para 1 (uma garra).
 *
 * Exemplo:
 * Para sítios (n conexões com uma garra)
 * sítio-->sítio              // sítio conectado a sítio
 * sítio-->ligação            // sítio conectado a ligação
 *
 * Para ligações (n conexões com duas garras - conexaoA e conexaoB)
 * sítio <--ligação--> lig    // A primeira ligação após um sítio
 * lig <--ligação--> lig      // Ligação entre duas ligações
 * lig <--ligação--> sítio    // A última ligação
 * sítio <--ligação--> sítio  // Conexão entre sítios com uma única ligação
 *
 * @author:  André Duarte Bueno
 * @see:     grafos
  * @ingroup  HCObjetoGrafo
*/
class CObjetoGrafo_1VetorConexoes : public CObjetoGrafo {
// --------------------------------------------------------------Atributos
public:
   /**
   * @brief Lista de ponteiros para objetos do tipo CObjetoGrafo.
   * Ou seja, ponteiros para objetos da hierarquia CObjetoGrafo.
   * O vetor conexao é o vetor dos objetos a quem estou conectado.
   * @test: verificar vantagens de trocar vector por list.
   */
   std::vector < CObjetoGrafo* >conexao;

// -------------------------------------------------------------Construtor
/// Construtor
   CObjetoGrafo_1VetorConexoes () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
   virtual ~ CObjetoGrafo_1VetorConexoes () = default;

// ----------------------------------------------------------------Métodos
   /// Retorna o tipo de objeto do grafo.
   virtual ETipoObjetoGrafo Tipo () const  override {
      return ETipoObjetoGrafo::ObjetoGrafo_1VetorConexoes;
   }

   /**
     * @brief Função que recebe um ponteiro para um CObjetoGrafo,
     * e o inclue na lista de conexões. Lista dos objetos a quem estou conectado.
   */
   /*inline*/ virtual void Conectar ( CObjetoGrafo* objA, CObjetoGrafo* objB = nullptr ) override ;

   /// Deleta uma conexão.
   /*inline*/ virtual void DeletarConexao ( unsigned int pos ) override ;

   /**
   * @brief Deleta os links para objetos que foram marcados para deleção.
   * Recebe um número que identifica os objetos que foram marcados
   * para deleção, se o rótulo dos objetos conectados é igual a este parâmetro
   * a conexão é eliminada.
   */
   /*inline*/ virtual bool DeletarConexoesInvalidadas ( unsigned int deletado ) override ;

   /// @brief Salva atributos do objeto em disco.
   virtual std::ostream& Write ( std::ostream& os ) const override ;

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
   friend std::ostream& operator<< ( std::ostream& os, CObjetoGrafo_1VetorConexoes& obj );
//       friend istream& operator>> (istream& is, CObjetoGrafo_1VetorConexoes& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline std::ostream& operator<< ( std::ostream& os, CObjetoGrafo_1VetorConexoes& obj );
// istream& operator>> (istream& is, CObjetoGrafo_1VetorConexoes& obj);

using COG_Sitio = CObjetoGrafo_1VetorConexoes ;

#endif
