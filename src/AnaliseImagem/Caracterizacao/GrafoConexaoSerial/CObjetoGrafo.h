#ifndef CObjetoGrafo_h
#define CObjetoGrafo_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoGrafo.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// Interface de disco
#include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// Definição export/import da classe
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CContorno_h
#include <MetNum/Contorno/CContorno.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoGrafo
// ===============================================================================
/**
 * @brief Representa um objeto básico de um grafo.
 * Um CGrafo representa um grafo, que é composto por um conjunto de objetos do tipo CObjetoGrafo.
 * Tem um conjunto de métodos para:
 * Conectar objetos: Conectar().
 * Deletar objetos conectados: DeletarConeccao().
 * Deletar conecções inválidas: DeletarConeccoesInvalidadas().
 * Retornar o tipo de contorno: Contorno().
 * Salvar o objeto em disco: Write().
 * Ler o objeto do disco: Read(). @todo: implementar.
 * Determinar o fluxo de uma determinada propriedade: Fluxo().
 * Determinar alguma propriedade de interesse: Go().
 *
 * @todo: verificar possibilidade de usar rotulo como long int;
 * se for negativo pode indicar outra propriedade/informação.
 *
 * @author   André Duarte Bueno.
 * @see      grafos.
 * @todo     implementar sobrecarga << e >>.
 * @defgroup HCObjetoGrafo
 * @ingroup  HCObjetoGrafo
*/
class CObjetoGrafo {
// --------------------------------------------------------------Atributos
public:
     /**
     * @brief Todo objeto CObjetoGrafo tem um rotulo, que o identifica;
      * O rótulo é usado para armazenar o objeto em disco, e
	  * para localizar o objeto, seus links e referências.
     */
     unsigned int rotulo {0};                    // assume default  = 0

// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoGrafo ()  = default;

// --------------------------------------------------------------Destrutor

/// Destrutor.
     virtual ~ CObjetoGrafo ()   = default;

// ----------------------------------------------------------------Métodos
     /// Enumeração para o tipo de contorno, por default assume CContorno::CENTER.
     ///  Esboço:
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
     virtual CContorno::ETipoContorno Contorno () const  {
          return CContorno::ETipoContorno::CENTER;
     }

     /**
       * @brief Função que recebe um ponteiro para um CObjetoGrafo,
       * e o inclue na lista de conecções. Lista dos objetos a quem estou conectado.
     */
     virtual void Conectar ( CObjetoGrafo *objA, CObjetoGrafo *objB = nullptr ) {}; //=0

     /// Deleta uma conexão.
     virtual void DeletarConeccao ( unsigned int link )  {}; //=0

     /**
     * @brief Deleta os links para objetos que foram marcados para deleção.
     * Recebe um número que identifica os objetos que foram marcados
     * para deleção, se o rótulo dos objetos conectados é igual a este parâmetro
	 * a conecção é eliminada.
     */
     virtual bool DeletarConeccoesInvalidadas ( int deletado )  { return 1;}; //=0 
	 
     /// @brief Salva atributos do objeto em disco.
     virtual std::ostream &Write ( std::ostream &os ) const ; //=0

//      /// @brief Salva atributos do objeto em disco no formato do Liang
//      virtual std::ostream &Write_Liang_Format ( std::ostream &os ) const  {
//           Write ( os ); // deve ser reescrita nas derivadas.
//           return os;
//      }
     /**
     * @brief Função usada para calcular uma propriedade.
     */
     virtual long double Go ( long double d = 0 ) = 0;

     /**
      * @brief Função que calcula o fluxo associado as propriedade do objeto
	  * e suas conecções.
      * Ou seja, considera-se que este objeto esta conectado a outros objetos
      * e que em função das propriedades dos objetos, existe alguma informação 
	  * que transita entre os objetos. Esta propriedade é calculada por esta função.
      * Pode ser fluxo de massa, de calor, de qualquer coisa, ...
     */
     virtual long double Fluxo () const = 0;

// --------------------------------------------------------------------Get
     /// @brief Retorna o rotulo do objeto.
     unsigned int Rotulo () const  {
          return rotulo;
     }

// --------------------------------------------------------------------Set
     /// @brief Seta o rotulo do objeto.
     void Rotulo ( unsigned int _r ) {
          rotulo = _r;
     }

// -----------------------------------------------------------------Friend
     friend std::ostream &operator<< ( std::ostream &os, const CObjetoGrafo &obj );
     friend std::istream &operator>> ( std::istream &is, CObjetoGrafo &obj );
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream &operator<< ( std::ostream &os, const CObjetoGrafo &obj );
std::istream &operator>> ( std::istream &is, CObjetoGrafo &obj );

#endif
