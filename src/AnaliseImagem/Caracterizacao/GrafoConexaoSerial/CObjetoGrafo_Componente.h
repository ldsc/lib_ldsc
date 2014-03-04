#ifndef CObjetoGrafo_Componente_h
#define CObjetoGrafo_Componente_h

/**
===============================================================================
@PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CObjetoGrafo_Componente.h
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
// Documentacao Classe: CObjetoGrafo_Componente.h
// ===============================================================================
/**
 * @brief Representa um objeto avançado de um grafo.
 * A característica básica de um CObjetoGrafo_Componente é que este pode ter
 * n conexões, ou seja, estar conectado a n objetos do tipo CObjetoGrafo, mas cada conexão é 1 para m (m garras).
 * Um  CObjetoGrafo_Componente vai ter um vetor de vetor de conexões (matriz de conexões).
 * Assim: enquanto um
 * COG_Sitio   esta conectado a 1(um) conjunto de CObjetoGrafo,
 * COG_Ligacao esta conectada a 2(dois) conjuntos de CObjetoGrafo,
 * CObjetoGrafo_Componente esta conectado a M conjuntos de CObjetoGrafo(ou seja tem M garras).
 *
 * Exemplo:
 * Para sítios (uma garra S--<)
 * sítio-->sítio
 * sítio-->ligação
 *
 * Para ligações (duas garras  >--L--<)
 * lig	<--ligação-->	lig
 * sítio	<--ligação-->	lig
 * lig	<--ligação-->	sítio
 * sítio	<--ligação-->	sítio
 *
 * Para CObjetoGrafo_Componente
 * conexao[i][j]
 * i = número de conexões (vai ter n conexões)
 * j = número de garras de cada conexão (cada conexão com m ligações)
 *
 * Esboco para CObjetoGrafo_Componente com 4 garras
 *                   lig
 *                   /|\...
 * lig <-----CObjetoGrafo_Componente---->lig
 *                   \|/...
 *                   lig
 *
 * Exemplo:
 * Um objeto CObjetoGrafo_Componente esta conectado a 10 outros objetos do tipo CObjetoGrafo
 * int i = 10; 	// 10 conexões
 * Cada conexão ocorre com 3 garras
 *       lig <-----CObjetoGrafo_Componente---->lig
 *                       \|/...
 *                       lig
 * int j = 3;		// cada uma das 10 conexões tem 3 garras
 * ...conexao[5][0];...conexao[5][1];...conexao[5][2];
 *
 * Exemplo:
 * Normalmente tem-se um vetor de  CObjetoGrafo*,
 * vector<CObjetoGrafo*>  conexao;
 *
 * Aqui tenho um vetor de vetores
 * vector < vector<CObjetoGrafo*> *> conexao;
 *
 * Uso
 * conexao[i] acessa o vetor i
 * conexao[i][j] acessa o ponteiro j do vetor i
 *
 * Para adicionar um objeto ao vetor i
 * conexao[i].push_back(CObjetoGrafo*obja);
 *
 * Para adicionar um vetor
 * vector<CObjetoGrafo*> * ptr_para_vetor_do_tipo_CObjetoGrafo;
 * conexao.push_back(ptr_para_vetor_do_tipo_CObjetoGrafo);
 *
 * OBS: classe NÃO TESTADA.
 * @author 	André Duarte Bueno
 * @version
 * @see		grafos
 * @ingroup  HCObjetoGrafo
*/
class CObjetoGrafo_Componente : public CObjetoGrafo 
{
// --------------------------------------------------------------Atributos
public:
     /**
     * @brief Normalmente tem-se um vetor de  CObjetoGrafo*,
      * vector<CObjetoGrafo*>  conexao;
      * Aqui tenho um vetor de vetores conexao é um vetor de objetos do tipo
      * vector<CObjetoGrafo*> */
     std::vector < std::vector < CObjetoGrafo * > > conexao;

// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoGrafo_Componente () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CObjetoGrafo_Componente () = default;

// ----------------------------------------------------------------Métodos
     /**
      * @brief Função de conexão. Note que aqui recebe um vetor de objetos e na classe base um único objeto.
     */
     inline virtual void Conectar ( std::vector < CObjetoGrafo * >obj_vetor ) override;

//      /**
//        * @brief Função que recebe um ponteiro para um CObjetoGrafo,
//        * e o inclue na lista de conexões. Lista dos objetos a quem estou conectado.
//      */
//      virtual void Conectar ( CObjetoGrafo *objA, CObjetoGrafo *objB = nullptr ) override ;

     /// Deleta uma conexão (um vetor inteiro).
     inline virtual void DeletarConeccao ( unsigned int ivetor ) override ;

     /// Deleta uma conexão específica (deleta o link do vetor). ~Matriz[ivetor][link].
     inline /*virtual */void DeletarConeccao ( unsigned int ivetor, unsigned int link );

     /**
     * @brief Deleta os links para objetos que foram marcados para deleção.
     * Recebe um número que identifica os objetos que foram marcados
     * para deleção, se o rótulo dos objetos conectados é igual a este parâmetro
	 * a conexão é eliminada.
     */
     inline virtual bool DeletarConeccoesInvalidadas ( unsigned int deletado ) override ; 

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
// 	   return 0;
// 	 }
// 
//      /**
//       * @brief Função que calcula o fluxo associado as propriedade do objeto
// 	  * e suas conexões.
//       * Ou seja, considera-se que este objeto esta conectado a outros objetos
//       * e que em função das propriedades dos objetos, existe alguma informação 
// 	  * que transita entre os objetos. Esta propriedade é calculada por esta função.
//       * Pode ser fluxo de massa, de calor, de qualquer coisa, ...
//      */
//      virtual long double Fluxo () const  override {
// 	   return 0;
// 	 }

// --------------------------------------------------------------------Get
/// Retorna referência para o vetor de vetor de coneccões
     std::vector < std::vector < CObjetoGrafo * > >  &Coneccao() {
          return   conexao;
     }

// --------------------------------------------------------------------Set
/// Definição do vetor de vetor de coneccões.
     void  Coneccao ( std::vector < std::vector < CObjetoGrafo * > > &_conexao ) {
          conexao = _conexao;
     }

// -----------------------------------------------------------------Friend
 friend ostream& operator<< (ostream& os, CObjetoGrafo_Componente& obj);
// friend istream& operator>> (istream& is, CObjetoGrafo_Componente& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline ostream& operator<< (ostream& os, CObjetoGrafo_Componente& obj);
// istream& operator>> (istream& is, CObjetoGrafo_Componente& obj);
#endif
