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
#include <fstream>
#include <vector>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// #ifndef _LIB_LDSC_CLASS_h
// #include <Base/_LIB_LDSC_CLASS.h>
// #endif

// #ifndef CObjetoGrafo_h
// #include <EstruturaDados/ObjetoGrafo/CObjetoGrafo.h>
// #endif

#ifndef CContorno_h
#include <MetNum/Contorno/CContorno.h>
#endif

// Definição de CSMParametroSolver, classe base, herda x
#ifndef CSMParametroSolver_h
#include <MetNum/Solver/SistemaEquacoes/CSMParametroSolver.h>
#endif

/// Enumeração para os diferentes tipos de grafo.
/// @enum: ETipoGrafo
// Note que sempre que criar classe herdeira tem de colocar aqui a enumeração correspondente.
#ifndef ETipoObjetoGrafo_
#define ETipoObjetoGrafo_
enum class ETipoObjetoGrafo : uint8_t {
   ObjetoGrafo = 0,                  // Objetos da hierarquia de objetos do grafo
   ObjetoGrafo_1VetorConexoes = 1,
   ObjetoGrafo_2VetoresConexoes = 2,
   ObjetoGrafo_MatrizConexoes = 3,

   ObjetoRede = 10,                   // Objetos da hierarquia de objetos da rede
   ObjetoRede_Final = 11, // Inclue o tipo como atributo (final)
   ObjetoRede_Tipo = 15, // Inclue o tipo como atributo (sem herança)
   ObjetoRede_Ligacao = 20,
   ObjetoRede_Ligacao_CENTER = 21,
   ObjetoRede_Ligacao_EST = 22,
   ObjetoRede_Ligacao_WEST = 23,
   ObjetoRede_Sitio = 30,
   ObjetoRede_Sitio_CENTER = 31,
   ObjetoRede_Sitio_EST = 32,
   ObjetoRede_Sitio_WEST = 33,

   ObjetoEsqueleto =  50,              // Objetos da hierarquia de objetos do esqueleto
   ObjetoEsqueleto_Final = 51, // Inclue o tipo como atributo (final)
   ObjetoEsqueleto_Tipo = 55, // Inclue o tipo como atributo (sem herança)
   ObjetoEsqueleto_Ligacao = 60,
   ObjetoEsqueleto_Ligacao_CENTER =61,
   ObjetoEsqueleto_Ligacao_EST = 62,
   ObjetoEsqueleto_Ligacao_WEST = 63,
   ObjetoEsqueleto_Sitio = 70,
   ObjetoEsqueleto_Sitio_CENTER = 71,
   ObjetoEsqueleto_Sitio_EST = 72,
   ObjetoEsqueleto_Sitio_WEST = 73,
};
#endif

// ===============================================================================
// Documentacao Classe: CObjetoRede_Tipo
// ===============================================================================
/**
 * @brief  Representa um objeto completo de uma rede; Não é herdeiro de CObjetoGrafo,
 * apenas de CSMParametroSolver.
 * Todos os atributos e métodos são criados aqui, de forma a não ter métodos virtuais.
 * Comparar com CObjetoRede_Final (que usa final de c++11).
 *
 * Nota importante: o modelo de cálculo das condutâncias (ex: Poiselle ou Koplic ou...)
 * e o cálculo das condutâncias é feito na hierarquia de CGrafo.
 * Ou seja, os objetos da rede não sabem qual modelo foi usado para cálculo da condutância.
 *
 * @author:  André Duarte Bueno
 * @see:     grafos
 * @ingroup  HCObjetoGrafo
 * @todo Criar template T para tipo propriedade ( que pode ser algo simples, um double
 * ou algo mais complexo, como uma classe função )
*/
class CObjetoRede_Tipo : public CSMParametroSolver  {
// --------------------------------------------------------------Atributos
public:
   /**
   * @brief Todo objeto CObjetoGrafo tem um rotulo, que o identifica;  O rótulo é usado para armazenar o objeto em disco, e para localizar o objeto, seus links e referências.
   */
   unsigned int rotulo {0};

   /// Todo CObjetoGrafo tem uma propriedade// Ex:raio hidraulico ou condutância
   long double propriedade {0.0};

   /**
   * @brief Vetor de ponteiros para objetos do tipo CObjetoRede_Tipo.
   * Ou seja, ponteiros para objetos da hierarquia CObjetoRede_Tipo.
   */
   std::vector < CObjetoRede_Tipo* >conexao;

   /**
   *@brief  Lista de long doubles que armazena a condutâncias das conexões.
   */
   std::vector < long double > condutancia;

   /// Armazena a informação do tipo de objeto do grafo; desta forma não
   /// precisa de um monte de heranças e classes EST WEST.
   /// O problema é que aumenta o consumo de memória em um uint8_t
   ETipoObjetoGrafo tipo;

// -------------------------------------------------------------Construtor
/// Construtor
   CObjetoRede_Tipo ( ETipoObjetoGrafo _tipo ) {
      tipo = _tipo;
   }

// --------------------------------------------------------------Destrutor
/// Destrutor
   /*virtual*/ ~ CObjetoRede_Tipo () = default;

// ----------------------------------------------------------------Métodos
   /// Enumeração para o tipo de contorno, por default assume CContorno::CENTER.
   /// Esboço:
   ///                         3
   ///            5         0  1  2            6
   ///                         4
   ///                         NORTH
   ///            FRONT WEST  CENTER  EST    BACK
   ///                         SUL
   /// @bug : como os valores abaixo foram alterados verificar se não implica em bug!
   ///        ou seja, no código não usar conversão para unsigned char.
   ///  enum class ETipoContorno : unsigned char
   ///  { CENTER = 1, WEST=0, EST=2, SOUTH=4, NORTH=3, FRONT=5, BACK=6 };
   /// @return o número (identificação) do contorno ao qual esta associado
   /*virtual*/ CContorno::ETipoContorno Contorno () const  {
      return CContorno::ETipoContorno::CENTER;
   }

   /**
    * @brief Função que recebe um ponteiro para um CObjetoRede objA e o inclue na lista de conexões.
    * Faz a conexão de this com o objeto recebido. Usada por objetos do tipo Sitio.
    * Note que oculta versão que recebe CObjetoGrafo.
   */
   inline void Conectar ( CObjetoRede_Tipo* objA ) {
      this->conexao.push_back ( objA );
      this->condutancia.push_back ( objA->propriedade );
   }

   /**
   * @brief Função que recebe um ponteiro para um CObjetoRede_Tipo objA e o inclue na lista de conexões.
   * Faz a conexão de this com o objeto recebido. Usada por objetos do tipo Sitio.
   * A condutância entre this e o objA é o segundo parâmetro (a _condutancia é calculada externamente).
   */
   inline void Conectar ( CObjetoRede_Tipo* objA, long double _condutancia ) {
      this->conexao.push_back ( objA );
      this->condutancia.push_back ( _condutancia );
   }

   /**
   * @brief Função que recebe um ponteiro para um CObjetoRede_Tipo objA e um ponteiro para um objeto B
   * e os inclue na lista de conexões. Usada por objetos do tipo Ligação.
   * Faz a conexão de this com os objetos recebidos.
   * Note que oculta versão que recebe CObjetoGrafo.
   * Note que como não recebe a condutância, vai fazer a condutancia igual a
   * propriedade do objeto ao qual estou conectado:
   * conexao[i] = objA;
   * condutancia[i] = objA.propriedade;
   */
   /*inline */virtual void Conectar ( CObjetoRede_Tipo* objA, CObjetoRede_Tipo* objB ) {
      this->conexao.push_back ( objA );
      this->condutancia.push_back ( objA->propriedade );
      this->conexao.push_back ( objB );
      this->condutancia.push_back ( objB->propriedade );
   }

   /**
   * @brief Função que recebe um ponteiro para um CObjetoRede_Tipo objA e um ponteiro para um objeto B
   * e os inclue na lista de conexões. Usada por objetos do tipo Ligação.
   * Faz a conexão de this com os objetos recebidos.
   * Note que oculta versão que recebe CObjetoGrafo.
   * Note que como não recebe a condutância, vai fazer a condutancia igual a
   * propriedade do objeto ao qual estou conectado:
   * conexao[i] = objA;
   * condutancia[i] = _condutanciaA;
   */
   /*inline */virtual void Conectar ( CObjetoRede_Tipo* objA, long double _condutanciaA,
                                      CObjetoRede_Tipo* objB , long double _condutanciaB ) {
      this->conexao.push_back ( objA );
      this->condutancia.push_back ( _condutanciaA );
      this->conexao.push_back ( objB );
      this->condutancia.push_back ( _condutanciaB );
   }
   /** Deleta conexões de ramos mortos
     Nota: o código abaixo pode deixar o processo lento se o número de conexões foram
     grande e se este método for muito chamado! Pensar em usar <list>
   */
   void DeletarConexao ( unsigned int pos ) {
      // Deleta a conexão
      this->conexao.erase ( conexao.begin() + pos );
      // e, adicionalmente, deleta a condutancia associada ao objeto pos
      this->condutancia.erase ( condutancia.begin () + pos );
   }

   /**
     @short  : Recebe um vetor com o índice das conexões a serem deletadas.
     (Deleta várias conexões ao mesmo tempo).
   */
   void DeletarConexao ( std::vector<unsigned int> di ) {
      unsigned int marcar_para_delecao = conexao.size();

      for ( unsigned int i = 0; i < di.size (); i++ )
         // Se di[i] for um rótulo inválido para conexao ocorre estouro de pilha!
         conexao[ di[i] ]->rotulo =  marcar_para_delecao;

      // Chama  DeletarConexoesInvalidadas que deleta conexões marcadas para deleção.
      this->DeletarConexoesInvalidadas ( marcar_para_delecao );
   }

   /**
    * @brief Deleta as conexões para objetos que foram marcados para deleção.
    * Recebe um número que identifica os objetos que foram marcados
    * para deleção, se o rótulo dos objetos conectados é igual a deletado a conexão é eliminada.
    * NOTA: mesmo código de CObjetoGrafo_1VetorConexoes
    */
   /*inline*/ /*virtual*/ bool DeletarConexoesInvalidadas ( unsigned int deletado ) ;

   /// Deleta as conexões repetidas, retorna o número de conexões deletadas.
   /*inline*//*virtual*/ unsigned int DeletarConexoesRepetidas_e_SomarCondutanciasParalelo() ;

   /// @brief Salva atributos do objeto em disco.
   /*inline*//*virtual*/ std::ostream& Write ( std::ostream& os ) const ;

   /**
     * @brief Função usada para calcular novo valor de x (ex: pressão).
     */
   /*inline*//*virtual*/ long double Go ( long double d = 0 ) ;

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
   ETipoObjetoGrafo  Tipo () const  {
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
   void Tipo ( ETipoObjetoGrafo _tipo ) {
      tipo = _tipo;
   }

   // -----------------------------------------------------------------Friend
   friend std::ostream& operator<< ( std::ostream& os, CObjetoRede_Tipo& obj );
//       friend istream& operator>> (istream& is, CObjetoRede_Tipo& obj);

};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline std::ostream& operator<< ( std::ostream& os, CObjetoRede_Tipo& obj );
// istream& operator>> (istream& is, CObjetoRede_Tipo& obj);

// Cria o tipo CObjetoRede_Tipo_CENTER, que é igual a CObjetoRede_Tipo
// typedef CObjetoRede_Tipo CObjetoRede_Tipo_CENTER;
// using CObjetoRede_Tipo_CENTER = CObjetoRede_Tipo;

#endif
