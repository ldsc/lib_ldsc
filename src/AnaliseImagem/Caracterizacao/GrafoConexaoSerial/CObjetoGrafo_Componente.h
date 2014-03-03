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
 * n link's, ou seja, estar conectado a n objetos do tipo
 * CObjetoGrafo, mas cada conecção é 1 para m (m garras).
 * Um  CObjetoGrafo_Componente vai ter um vetor de conexões.
 * Assim: enquanto
 * um  COG_Sitio   esta conectado a um conjunto de CObjetoGrafo,
 * um  COG_Ligacao esta conectada a dois conjuntos de CObjetoGrafo,
 * um  CObjetoGrafo_Componente esta conectado a m conjuntos de CObjetoGrafo(ou seja tem m garras).
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
 * coneccao[i][j]
 * i = número de conecções (vai ter n conecções)
 * j = número de garras de cada conecção (cada conexão com m ligações)
 *
 * Esboco para CObjetoGrafo_Componente com   4 garras
 *                   lig
 *                   /|\...
 * lig <-----CObjetoGrafo_Componente---->lig
 *                   \|/...
 *                   lig
 *
 * Exemplo:
 * // Um objeto CObjetoGrafo_Componente esta conectado a 10 outros objetos do tipo CObjetoGrafo
 * int i = 10; 	// 10 conecções
 * // Cada conecção ocorre com 3 garras
 * //      lig <-----CObjetoGrafo_Componente---->lig
 * //                      \|/...
 * //                      lig
 * int j = 3;		// cada uma das 10 conecções tem 3 garras
 * ...coneccao[5][0];...coneccao[5][1];...coneccao[5][2];
 *
 * Exemplo:
 * Normalmente tem-se um vetor de  CObjetoGrafo*,
 * vector<CObjetoGrafo*>  coneccao;
 *
 * Aqui tenho um vetor de vetores
 * vector < vector<CObjetoGrafo*> *> coneccao;
 *
 * Uso
 * coneccao[i] acessa o vetor i
 * coneccao[i][j] acessa o ponteiro j do vetor i
 *
 * Para adicionar um objeto ao vetor i
 * coneccao[i].push_back(CObjetoGrafo*obja);
 *
 * Para adicionar um vetor
 * vector<CObjetoGrafo*> * ptr_para_vetor_do_tipo_CObjetoGrafo;
 * coneccao.push_back(ptr_para_vetor_do_tipo_CObjetoGrafo);
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
      * vector<CObjetoGrafo*>  coneccao;
      * Aqui tenho um vetor de vetores coneccao é um vetor de objetos do tipo
      * vector<CObjetoGrafo*> */
     std::vector < std::vector < CObjetoGrafo * > > coneccao;

// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoGrafo_Componente () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CObjetoGrafo_Componente () = default;

// ----------------------------------------------------------------Métodos
     /**
      * @brief Função de conecção.
     */
     virtual void Conectar ( std::vector < CObjetoGrafo * >obj_vetor );

     /**
       * @brief Função que recebe um ponteiro para um CObjetoGrafo,
       * e o inclue na lista de conecções. Lista dos objetos a quem estou conectado.
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
     /**
     * @brief Função usada para calcular uma propriedade.
     */
     virtual long double Go ( long double d = 0 )   override {
	   return 0;
	 }

     /**
      * @brief Função que calcula o fluxo associado as propriedade do objeto
	  * e suas conecções.
      * Ou seja, considera-se que este objeto esta conectado a outros objetos
      * e que em função das propriedades dos objetos, existe alguma informação 
	  * que transita entre os objetos. Esta propriedade é calculada por esta função.
      * Pode ser fluxo de massa, de calor, de qualquer coisa, ...
     */
     virtual long double Fluxo () const  override {
	   return 0;
	 }

// --------------------------------------------------------------------Get
/// Retorna referência para o vetor de vetor de coneccões
     std::vector < std::vector < CObjetoGrafo * > >  &Coneccao() {
          return   coneccao;
     }

// --------------------------------------------------------------------Set
/// Definição do vetor de vetor de coneccões.
     void  Coneccao ( std::vector < std::vector < CObjetoGrafo * > > &_coneccao ) {
          coneccao = _coneccao;
     }

// -----------------------------------------------------------------Friend
/// @todo: implementar sobrecarga operadores << e >> para CObjetoGrafo_Componente
// friend ostream& operator<< (ostream& os, CObjetoGrafo_Componente& obj);
// friend istream& operator>> (istream& is, CObjetoGrafo_Componente& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CObjetoGrafo_Componente& obj);
// istream& operator>> (istream& is, CObjetoGrafo_Componente& obj);
#endif
