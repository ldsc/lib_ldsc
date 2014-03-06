#ifndef CObjetoGrafo_MatrizConexoes_h
#define CObjetoGrafo_MatrizConexoes_h

/**
===============================================================================
@PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoGrafo_MatrizConexoes.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// ----------------------------------------------------------------------
// Bibliotecas C/C++
// ----------------------------------------------------------------------
#include <vector>

// ----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// ----------------------------------------------------------------------
#ifndef CObjetoGrafo_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoGrafo_MatrizConexoes.h
// ===============================================================================
/**
 * @brief Representa um objeto avançado de um grafo.
 * A característica básica de um CObjetoGrafo_1VetorConexoes é que este pode ter  n conexões,
 * ou seja, estar conectado a n objetos do tipo CObjetoGrafo, mas cada conexão é 1 para m (m garras).
 * Um  CObjetoGrafo_MatrizConexoes vai ter um vetor de vetor de conexões (matriz de conexões).
 * Assim: enquanto um
 * CObjetoGrafo_1VetorConexoes esta conectado a 1(um) conjunto de CObjetoGrafo,
 * CObjetoGrafo_2VetorConexoes esta conectada a 2(dois) conjuntos de CObjetoGrafo,
 * CObjetoGrafo_MatrizConexoes esta conectado a M conjuntos de CObjetoGrafo(ou seja tem M garras).
 *
 * Exemplo:
  * Para sítios (uma garra, use CObjetoGrafo_1VetoresConexoes)
 * sítio-->sítio              // sítio conectado a sítio
 * sítio-->ligação            // sítio conectado a ligação
 *
 * Para conjunto de ligações duplas (duas garras, use CObjetoGrafo_2VetoresConexoes)
 * sítio <--ligação--> lig    // A primeira ligação após um sítio
 * lig <--ligação--> lig      // Ligação entre duas ligações
 * lig <--ligação--> sítio    // A última ligação
 * sítio <--ligação--> sítio  // Conexão entre sítios com uma única ligação
*
 * Para CObjetoGrafo_MatrizConexoes
 * mconexao[i][j]
 * i = número de conexões (vai ter n conexões)
 * j = número de garras de cada conexão (cada conexão com m ligações)
 *
 * Esboco para CObjetoGrafo_MatrizConexoes com 4 garras
 *                   lig
 *                   /|\...
 * lig <-----CObjetoGrafo_MatrizConexoes---->lig
 *                   \|/...
 *                   lig
 *
 * Exemplo:
 * Um objeto CObjetoGrafo_MatrizConexoes esta conectado a 10 outros objetos do tipo CObjetoGrafo
 * int i = 10; 	// 10 conexões
 * Cada conexão ocorre com 3 garras
 *       lig <-----CObjetoGrafo_MatrizConexoes---->lig
 *                       \|/...
 *                       lig
 * int j = 3;		// cada uma das 10 conexões tem 3 garras
 * ...mconexao[5][0];...mconexao[5][1];...mconexao[5][2];
 *
 * Exemplo:
 * Normalmente tem-se um vetor de  CObjetoGrafo*,
 * vector<CObjetoGrafo*>  mconexao;
 *
 * Aqui tenho um vetor de vetores
 * vector < vector<CObjetoGrafo*> *> mconexao;
 *
 * Uso
 * mconexao[i] acessa o vetor i
 * mconexao[i][j] acessa o ponteiro j do vetor i
 *
 * Para adicionar um objeto ao vetor i
 * mconexao[i].push_back(CObjetoGrafo*obja);
 *
 * Para adicionar um vetor
 * vector<CObjetoGrafo*> * ptr_para_vetor_do_tipo_CObjetoGrafo;
 * mconexao.push_back(ptr_para_vetor_do_tipo_CObjetoGrafo);
 *
 * @author 	André Duarte Bueno
 * @version
 * @see		grafos
 * @ingroup  HCObjetoGrafo
 * @todo : testar CObjetoGrafo_MatrizConexoes
*/
class CObjetoGrafo_MatrizConexoes : public CObjetoGrafo {
// --------------------------------------------------------------Atributos
public:
   /**
   * @brief Normalmente tem-se um vetor de  CObjetoGrafo*,
    * vector<CObjetoGrafo*>  mconexao;
    * Aqui tenho um vetor de vetores mconexao é um vetor de objetos do tipo
    * vector<CObjetoGrafo*> */
   std::vector < std::vector < CObjetoGrafo* > > mconexao;

// -------------------------------------------------------------Construtor
/// Construtor
   CObjetoGrafo_MatrizConexoes () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
   virtual ~ CObjetoGrafo_MatrizConexoes () = default;

// ----------------------------------------------------------------Métodos
   /// Retorna o tipo de objeto do grafo.
   virtual ETipoObjetoGrafo Tipo () const  override {
      return ETipoObjetoGrafo::ObjetoGrafo_MatrizConexoes;
   }

   /**
    * @brief Função de conexão. Note que aqui recebe um vetor de objetos e na classe base um único objeto.
   */
   inline virtual void Conectar ( std::vector < CObjetoGrafo* >obj_vetor ) override;

   /// Deleta uma conexão (um vetor inteiro).
   inline virtual void DeletarConexao ( unsigned int ivetor ) override ;

   /// Nova: Deleta uma conexão específica (deleta o link do vetor). ~Matriz[ivetor][link].
   inline /*virtual */void DeletarConexao ( unsigned int ivetor, unsigned int link );

   /**
   * @brief Deleta os links para objetos que foram marcados para deleção.
   * Recebe um número que identifica os objetos que foram marcados
   * para deleção, se o rótulo dos objetos conectados é igual a este parâmetro
   * a conexão é eliminada.
   */
   inline virtual bool DeletarConexoesInvalidadas ( unsigned int deletado ) override ;

   /// @brief Salva atributos do objeto em disco.
   virtual std::ostream& Write ( std::ostream& os ) const override ;

// --------------------------------------------------------------------Get
/// Retorna referência para o vetor de vetor de coneccões
   std::vector < std::vector < CObjetoGrafo* > >&  MConeccao() {
      return   mconexao;
   }

// --------------------------------------------------------------------Set
/// Definição do vetor de vetor de coneccões.
   void  MConeccao ( std::vector < std::vector < CObjetoGrafo* > >& _mconexao ) {
      mconexao = _mconexao;
   }

// -----------------------------------------------------------------Friend
   friend ostream& operator<< ( ostream& os, CObjetoGrafo_MatrizConexoes& obj );
// friend istream& operator>> (istream& is, CObjetoGrafo_MatrizConexoes& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline ostream& operator<< ( ostream& os, CObjetoGrafo_MatrizConexoes& obj );
// istream& operator>> (istream& is, CObjetoGrafo_MatrizConexoes& obj);
#endif
