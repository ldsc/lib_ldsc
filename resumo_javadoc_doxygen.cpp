/** Comandos estruturais do javadoc:
 *  ================================
 * \file to document a file.
 * \namespace to document a namespace.
 * \package to document a Java package.
 * \interface to document an IDL interface.
 * 
 * \struct to document a C-struct.
 * \union to document a union.
 * \enum to document an enumeration type.
 * \def to document a #define.
 * \typedef to document a type definition.
 * 
 * \fn to document a function.
 * \var to document a variable or typedef or enum value.
 * 
 * \param para documentar um parâmetro.
 * \warning aviso de cuidado.
 */ 

 /** Criando Listas:
  * ===============
  *  A list of events:
  *    - mouse events
  *         -# mouse move event
  *         -# mouse click event\n
  *            More info about the click event.
  *         -# mouse double click event
  *    - keyboard events
  *         1. key down event
  *         2. key up event
  *
  *  More text here.
  */
 
Exemplo (JAVADOC_AUTOBRIEF = YES ):
===================================
/// \defgroup MetodosNumericos Conjunto de classes relacionados aos métodos numéricos.
...
/** Breve descrição. Descrição detalhada inicia após ponto.
 * \file CTeste.h
 */
/**
 *  Comentário breve da classe. Comentário detalhado após ponto.
 *  \ingroup MetodosNumericos 
 */
class CTeste
{
public:
  /** 
    * Breve descrição enumeração. Descrição detalhada após ponto.
    */
  enum EErro
  {
    divisaoZero,    ///< Descrição de divisaoZero
    notANumber      ///< Descrição de notANumber
  };

/**
  * Breve descrição. Descrição detalhada da função.
  * @param a argumento inteiro.
  * @param b argumento inteiro.
  * @see f2()
  * @return soma de a e b.
  */
  int Soma(int a, int b);
  
/** 
  * Variável pública.
  * Detalhes....
  */
  int var;     
};

Exemplo (JAVADOC_AUTOBRIEF = NO ):
===================================
/** \brief Breve descrição.
 * Breve descrição segunda parte. 
 * (pule uma linha ou ative JAVADOC_AUTOBRIEF para iniciar documentação detalhada)
 *
 *  Descrição detalhada inicia após linha em branco.
 * \file nomeArquivo.h
 */

/** Documentação breve da classe.
 * 
 *  Documentação detalhada da classe.
 */
class Test
{
public:
  /** Documentação breve da enumeração. 
    * 
    *  Documentação detalhada.
    */
  enum EErro
  {
    /// Descrição de divisaoZero
    divisaoZero,     
    /// Descrição de notANumber
    notANumber      
  };

  /** Documentação breve da função. 
    * 
    *  Documentação detalhada da função.
    */
  void member(); 
  
protected:
  /// Descrição da variável.
  int value;       
};

Grupos:
=======
- Módulos permitem informar que determinado conjunto de classes fazem parte de um módulo específico (um módulo cria páginas separadas para seus membros).

/**Para definir um grupo:
\defgroup NomeGrupo
ou
\defgroup NomeGrupo Título página do grupo

Para incluir membros neste grupo:
\ingroup NomeGrupo

Também é possível adicionar um trecho de código num determinado grupo usando \addtogroupt.
*/

/** \addtogroup NomeGrupo
 * @{ */
este código vai fazer parte do grupo NomeGrupo
/** finaliza o bloco do grupo @}*/

Pode-se referenciar um grupo, veja exemplo abaixo.

Exemplo:
========
/** \defgroup IntVariables Global integer variables */
...
/// \ingroup IntVariables
/// Variável A
int VarInA;

/// Estamos referenciando o grupo \ref IntVariables "titulo do link" ...


Subpaging
=========

Information can be grouped into pages using the \page and \mainpage commands. Normally, this results in a flat list of pages, where the "main" page is the first in the list.

Instead of adding structure using the approach described in section modules it is often more natural and convenient to add additional structure to the pages using the \subpage command.

For a page A the \subpage command adds a link to another page B and at the same time makes page B a subpage of A. This has the effect of making two groups GA and GB, where GB is part of GA, page A is put in group GA, and page B is put in group GB.






