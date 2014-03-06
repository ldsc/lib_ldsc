#ifndef CObjetoRede_h
#define CObjetoRede_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoRede.h
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

// Definição de CSMParametroSolver, classe base, herda x
#ifndef CSMParametroSolver_h
#include <MetNum/Solver/SistemaEquacoes/CSMParametroSolver.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoRede
// ===============================================================================
/**
 * @brief  Representa um objeto de uma rede é herdeiro de CObjetoGrafo e CSMParametroSolver.
 * Tem um rótulo (herdado de CObjetoGrafo) e uma propriedade x (herdada de CParametroSolver).
 * Acrescenta um long double propriedade, um vetor conexao e um vetor condutancia.
 * Adiciona: nova Conectar que conecta CObjetoRede*.
 * DeletarConeccoesRepetidas_e_SomarCondutanciasParalelo().
 * Sobrescreve: DeletarConexao(), DeletarConexoesInvalidadas(), Write().
 *
 * @author:  André Duarte Bueno
 * @see:     grafos
 * @ingroup  HCObjetoGrafo
*/
class CObjetoRede : /*virtual*/ public CObjetoGrafo, public CSMParametroSolver  {
// --------------------------------------------------------------Atributos
public:
   /// Todo CObjetoGrafo tem uma propriedade// Ex:raio hidraulico ou condutância
   long double propriedade {0.0};

   /**
   * @brief Vetor de ponteiros para objetos do tipo CObjetoRede.
   * Ou seja, ponteiros para objetos da hierarquia CObjetoRede.
   * @todo: verificar vantagens de trocar vector por list (teste desempenho)
   * Note que operações conexao[i] precisam ser trocadas!!
   */
   std::vector < CObjetoRede* >conexao;

   /**
   *@brief  Lista de long doubles que armazena a condutâncias das conexões.
   */
   std::vector < long double > condutancia;

// -------------------------------------------------------------Construtor
/// Construtor
   CObjetoRede () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
   virtual ~ CObjetoRede () = default;

// ----------------------------------------------------------------Métodos
   /**
     * @brief Função nova que recebe um ponteiro para um CObjetoRede,
     * e o inclue na lista de conexões. Lista dos objetos a quem estou conectado.
    * Note que oculta versão que recebe CObjetoGrafo.
   */
   virtual void Conectar ( CObjetoRede* objA, CObjetoRede* objB = nullptr );

   /**
     * @brief Função nova que recebe um ponteiro para um CObjetoRede,
     * e o inclue na lista de conexões. O segundo parâmtro é a condutância.
   */
   virtual void Conectar ( CObjetoRede* objA, long double _condutancia );

   /// Deleta uma conexão.
   /*inline*/ virtual void DeletarConexao ( unsigned int link ) override ;

   /**
    * @brief Deleta as conexões para objetos que foram marcados para deleção.
    * Recebe um número que identifica os objetos que foram marcados
    * para deleção, se o rótulo dos objetos conectados é igual a deletado a conexão é eliminada.
    * NOTA: mesmo código de CObjetoGrafo_1VetorConexoes
    */
   /*inline*/ virtual bool DeletarConexoesInvalidadas ( unsigned int deletado ) override ;

   /// Deleta as conexões repetidas, retorna o número de conexões deletadas.
   virtual unsigned int DeletarConeccoesRepetidas_e_SomarCondutanciasParalelo() override;

   /// @brief Salva atributos do objeto em disco.
   virtual std::ostream& Write ( std::ostream& os ) const override ;

   /**
     * @brief Função usada para calcular novo valor de x (ex: pressão).
     */
   virtual long double Go ( long double d = 0 );

   /**
    * @brief Função que calcula o fluxo associado as propriedade do objeto e suas conexões.
    * Ou seja, considera-se que este objeto esta conectado a outros objetos
    * e que em função das propriedades dos objetos, existe alguma informação
    * que transita entre os objetos. Esta propriedade é calculada por esta função.
    * Pode ser fluxo de massa, de calor, de qualquer coisa, ...
    * No caso do GCS calcula vazão nos pontos de contorno do grafo.
   */
   /*virtual*/ long double Fluxo () const ;

// --------------------------------------------------------------------Get
   /// @brief Retorna cópia da propriedade.
   long double Propriedade () const  {
      return propriedade;
   }
   /// @brief Retorna cópia do vetor de conexões.
   std::vector < CObjetoRede* > Conexao() const  {
      return conexao;
   }
   /// @brief Retorna cópia do vetor de condutâncias.
   std::vector < long double >  Condutancia () const  {
      return condutancia;
   }

// --------------------------------------------------------------------Set
   /// @brief Seta a propriedade.
   void Propriedade ( long double p ) {
      propriedade = p;
   }
   /// @brief Seta vetor de conexões.
   void Conexao ( std::vector < CObjetoRede* >  c) {
      conexao = c;
   }
   /// @brief Seta vetor de condutâncias.
   void Condutancia ( std::vector < long double > c ) {
      condutancia = c;
   }

   // -----------------------------------------------------------------Friend
   friend std::ostream& operator<< ( std::ostream& os, CObjetoRede& obj );
//       friend istream& operator>> (istream& is, CObjetoRede& obj);

protected:
//    /// Função auxiliar que recebe o indice das conexões a serem deletadas e um vetor de conexões.
//    /// criada para reduzir códigos nas herdeiras.
//    bool DeletarConexoesInvalidadas_aux ( unsigned int deletado , std::vector<CObjetoRede*>& conexao );
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline std::ostream& operator<< ( std::ostream& os, CObjetoRede& obj );
// istream& operator>> (istream& is, CObjetoRede& obj);

// Cria o tipo CObjetoRede_CENTER, que é igual a CObjetoRede
// typedef CObjetoRede CObjetoRede_CENTER;
using CObjetoRede_CENTER = CObjetoRede;

#endif
