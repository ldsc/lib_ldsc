#ifndef CObjetoRede_Tipo_h
#define CObjetoRede_Tipo_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoRede_Tipo.h
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
// #ifndef _LIB_LDSC_CLASS_h
// #include <Base/_LIB_LDSC_CLASS.h>
// #endif

// #ifndef CObjetoGrafo_h
// #include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo.h>
// #endif

// Definição de CSMParametroSolver, classe base, herda x
#ifndef CSMParametroSolver_h
#include <MetNum/Solver/SistemaEquacoes/CSMParametroSolver.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoRede_Tipo
// ===============================================================================
/**
 * @brief  Representa um objeto completo de uma rede, Não é herdeiro de CObjetoGrafo,
 * apenas de CSMParametroSolver.
 * Todos os atributos e métodos são criados aqui, de forma a não ter métodos virtuais.
 *
 * @author:  André Duarte Bueno
 * @see:     grafos
 * @todo:    implementar operadores >>.
 * @ingroup  HCObjetoGrafo
*/
class CObjetoRede_Tipo : public CSMParametroSolver  {
// --------------------------------------------------------------Atributos
public:
   /**
   * @brief Todo objeto CObjetoGrafo tem um rotulo, que o identifica;
    * O rótulo é usado para armazenar o objeto em disco, e para localizar o objeto, seus links e referências.
   */
   unsigned int rotulo {0};

   /// Todo CObjetoGrafo tem uma propriedade// Ex:raio hidraulico ou condutância
   long double propriedade {0.0};

   /**
   * @brief Vetor de ponteiros para objetos do tipo CObjetoRede_Tipo.
   * Ou seja, ponteiros para objetos da hierarquia CObjetoRede_Tipo.
   * @todo: verificar vantagens de trocar vector por list (teste desempenho).
   */
   std::vector < CObjetoRede_Tipo* >conexao;

   /**
   *@brief  Lista de long doubles que armazena a condutâncias das conexões.
   */
   std::vector < long double > condutancia;

   /// Enumeração para os diferentes tipos de grafo.
   /// @enum: ETipoGrafo
   // Note que sempre que criar classe herdeira tem de colocar aqui a enumeração correspondente.
   enum class ETipo : uint8_t {
      ObjetoGrafo,                  // Objetos da hierarquia de objetos do grafo
      ObjetoGrafo_1VetorConexoes,
      ObjetoGrafo_2VetoresConexoes,
      ObjetoGrafo_MatrizConexoes,

      ObjetoRede,                   // Objetos da hierarquia de objetos da rede
      ObjetoRede_Ligacao,
      ObjetoRede_Ligacao_EST,
      ObjetoRede_Ligacao_WEST,
      ObjetoRede_Sitio,
      ObjetoRede_Sitio_EST,
      ObjetoRede_Sitio_WEST,
// 	  ObjetoRede_Tipo, Não faz sentido pois tem objeto tipo indicando o tipo

      ObjetoEsqueleto,              // Objetos da hierarquia de objetos do esqueleto
      ObjetoEsqueleto_Ligacao,
      ObjetoEsqueleto_Ligacao_EST,
      ObjetoEsqueleto_Ligacao_WEST,
      ObjetoEsqueleto_Sitio,
      ObjetoEsqueleto_Sitio_EST,
      ObjetoEsqueleto_Sitio_WEST
// 	  ObjetoEsqueleto_Tipo, Não faz sentido pois tem objeto tipo indicando o tipo
   };

   /// Armazena a informação do tipo de objeto do grafo; desta forma não
   /// precisa de um monte de heranças e classes EST WEST.
   /// O problema é que aumenta o consumo de memória em um uint8_t
   ETipo tipo;

// -------------------------------------------------------------Construtor
/// Construtor
   CObjetoRede_Tipo ( ETipo _tipo ) {
      tipo = _tipo;
   }

// --------------------------------------------------------------Destrutor
/// Destrutor
   /*virtual*/ ~ CObjetoRede_Tipo () = default;

// ----------------------------------------------------------------Métodos
   /**
     * @brief Função nova que recebe um ponteiro para um CObjetoRede_Tipo,
     * e o inclue na lista de conexões. Lista dos objetos a quem estou conectado.
    * Note que oculta versão que recebe CObjetoGrafo.
   */
   inline/*virtual*/ void Conectar ( CObjetoRede_Tipo* objA, CObjetoRede_Tipo* objB = nullptr ) ;

   /// Deleta uma conexão.
   inline /*virtual*/ void DeletarConexao ( unsigned int link ) ;

   /**
    * @brief Deleta as conexões para objetos que foram marcados para deleção.
    * Recebe um número que identifica os objetos que foram marcados
    * para deleção, se o rótulo dos objetos conectados é igual a deletado a conexão é eliminada.
    * NOTA: mesmo código de CObjetoGrafo_1VetorConexoes
    */
   inline /*virtual*/ bool DeletarConexoesInvalidadas ( unsigned int deletado ) ;

   /// Deleta as conexões repetidas, retorna o número de conexões deletadas.
   inline/*virtual*/ unsigned int DeletarConeccoesRepetidas_e_SomarCondutanciasParalelo() ;

   /// @brief Salva atributos do objeto em disco.
   inline/*virtual*/ std::ostream& Write ( std::ostream& os ) const ;

   /**
     * @brief Função usada para calcular novo valor de x (ex: pressão).
     */
   inline/*virtual*/ long double Go ( long double d = 0 ) ;

   /**
    * @brief Função que calcula o fluxo associado as propriedade do objeto e suas conexões.
    * Ou seja, considera-se que este objeto esta conectado a outros objetos
    * e que em função das propriedades dos objetos, existe alguma informação
    * que transita entre os objetos. Esta propriedade é calculada por esta função.
    * Pode ser fluxo de massa, de calor, de qualquer coisa, ...
    * No caso do GCS calcula vazão nos pontos de contorno do grafo.
   */
   inline/*virtual*/ long double Fluxo () const ;

// --------------------------------------------------------------------Get
   /// @brief Retorna cópia da propriedade.
   long double Propriedade () const  {
      return propriedade;
   }
   /// @brief Retorna cópia do vetor de conexões.
   std::vector < CObjetoRede_Tipo* > Conexao() const  {
      return conexao;
   }
   /// @brief Retorna cópia do vetor de condutâncias.
   std::vector < long double >  Condutancia () const  {
      return condutancia;
   }
   /// @brief Retorna o tipo de objeto.
   ETipo  Tipo () const  {
      return tipo;
   }


// --------------------------------------------------------------------Set
   /// @brief Seta a propriedade.
   void Propriedade ( long double p ) {
      propriedade = p;
   }
   /// @brief Seta vetor de conexões.
   void Conexao ( std::vector < CObjetoRede_Tipo* >  c ) {
      conexao = c;
   }
   /// @brief Seta vetor de condutâncias.
   void Condutancia ( std::vector < long double > c ) {
      condutancia = c;
   }
   /// @brief Seta vetor de condutâncias.
   void Tipo ( ETipo _tipo ) {
      tipo = _tipo;
   }

   // -----------------------------------------------------------------Friend
   friend std::ostream& operator<< ( std::ostream& os, CObjetoRede_Tipo& obj );
//       friend istream& operator>> (istream& is, CObjetoRede_Tipo& obj);

protected:
//    /// Função auxiliar que recebe o indice das conexões a serem deletadas e um vetor de conexões.
//    /// criada para reduzir códigos nas herdeiras.
//    bool DeletarConexoesInvalidadas_aux ( unsigned int deletado , std::vector<CObjetoRede_Tipo*>& conexao );
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline std::ostream& operator<< ( std::ostream& os, CObjetoRede_Tipo& obj );
// istream& operator>> (istream& is, CObjetoRede_Tipo& obj);

// Cria o tipo CObjetoRede_Tipo_CENTER, que é igual a CObjetoRede_Tipo
// typedef CObjetoRede_Tipo CObjetoRede_Tipo_CENTER;
// using CObjetoRede_Tipo_CENTER = CObjetoRede_Tipo;

#endif
