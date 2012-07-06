#ifndef CMath_h
#define CMath_h

/**
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: CMath...
  ===============================================================================
  Desenvolvido por:	
  Laboratorio de Desenvolvimento de Software Cientifico 	
  [LDSC].
  @author     Andre Duarte Bueno
  @file       CMath.h
  @begin      Sat Sep 16 2000
  @copyright  (C) 2000 by Andre Duarte Bueno
  @email      andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//Bibliotecas LIB_LDSC
//-----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif /*  */


/**
 * @brief Classe definida com constantes matematicas comuns, ...
 * enumerated com tipos de erros, metodos inline com funcoes usuais.
 *
 * A utilizacao de variaveis do tipo #define nao realiza verificacao
 * de tipo, apenas realiza um search and replace.
 *
 * O mesmo acontece com as macros do C, em que o search and replace 
 * se processa de uma maneira mais inteligente, permitindo a construcao 
 * de "funacoes". Mas novamente sem realizar nenhuma verificacao de tipo.
 * 
 * A vantagem de se utilizar a classe CMath a a verificacao de tipo que ela
 * proporciona.
 * 
 * Quando voca passa para uma funcao uma variavel de CMath voce esta passando
 * um parametro que e constante e que tem o tipo long double. 
 * 
 * Se a funcao espera um double tudo ok, mas se ala espera um int, 
 * vai ocorrar uma conversao de double para int, e o compilador vai emitir 
 * um warning lhe informando da perda da parte arredondada.
 * Ou seja o compilador trabalha a seu favor reduzindo a possibilidade 
 * de bugs, que normalmente sao extremamente dificeis de serem localizados
 * quando se usa define.
 * 
 * Exemplo de uso:
 * #include <Base/CMath>
 * // Para acessar a variavel PI.
 * CMath::PI
 * 
 * Tarefas:
 * Verificar como ficou organizada a nova biblioteca do C++0x 
 * Se estas funcoes foram incluidas, colocar esta classe em comentário,
 * e, a seguir atualizar códigos considerando C++0x.
 * 
 * Criar CFisica para constantes fisicas
 
 * @author 	Andre Duarte Bueno	
 * @version	
 * @see		Matematica
 * 
*/

class CMath
{
  //--------------------------------------------------------------Atributos
 public:
	 
  /**
   * Variaveis estaticas definidas para uso externo
  */
  static const long double EPSILON = 1.0e-15;
  static const long double RESULTADO_ERRADO = -1.0e30;
  static const long double DADO_ERRADO = -1.0e30;

  /// Constantes matematicas
  static const long double E =      2.71828182845904523536;
  static const long double LOG2E =  1.44269504088896340736;
  static const long double LOG10E = 0.434294481903251827651;
  static const long double LN2=     0.693147180559945309417;
  static const long double LN10 =   2.30258509299404568402;
  static const long double PI  =    3.14159265358979323846;
  static const long double PI_2=    1.57079632679489661923;
  static const long double PI_4 =   0.785398163397448309616;
  static const long double M1PI =   0.318309886183790671538;
  static const long double M2PI =   0.636619772367581343076;
  static const long double M1SQRTPI=0.564189583547756286948;
  static const long double M2SQRTPI=1.12837916709551257390;
  static const long double MSQRT2 = 1.41421356237309504880;
  static const long double MSQRT3 = 1.73205080756887729352744634150587;
  static const long double MSQRT_2= 0.707106781186547524401;

  //Tipos enumerados
  /**
  * O EMathError define uma enumeracao com diversos tipos de erros.
  */
  enum EMathError               // ETipoErro
    {
      ok=0,			/// 1   matErr_None
      erroNoTamanho,		/// 2
      erroSingular,		/// 3   matErr_Singular
      erroCondicoes,		/// 4
      erroLimiteIteracoes,	/// 5   matErr_IterLimit
      erroLimiteErro,		/// 6
      erroConvergencia,		/// 7
      erroGenerico,		/// 8
      erroIntervalo		/// 9  Erro no intervalo, como valor negativo
    };

  //-------------------------------------------------------------Construtor
  /**
   * Construtor
   * @return 
   */
  CMath ()
    {
    }

  //--------------------------------------------------------------Destrutor
  /**
   * Destrutor
   * @return 
   */
  virtual ~ CMath ()
    {
    }

  //----------------------------------------------------------------Matodos
  /**
   * Retorna o quadrado
   * @param x 
   * @return 
   */
    template<typename T>
    inline T Sqr (T &x) const
    {
      return x * x;
    }


  /**
   * Retorna o cubo
   * @param x 
   * @return 
   */
    template<typename T>
    inline T Cubo (T &x) const
    {
      return x * x * x;
    }

  /**
     * Retorna o inverso
     * @param x 
     * @return 
   */
    template<typename T>
  inline T Inverso (T &x) const
    {
      return 1.0 / x;
    }

};

// Criar objeto CFisica com apelido CFM
// Constantes fisicas (manual Kurt Gieck)
//      static double ML;               // massa molecular da agua    []
//      static double R;                // Cte molar dos gazes        [J/mol.K]
//      static double TK;               // temperatura Kelvin         [K]
//      static double ATM;              // pressao atmosferica        [N/m^2]
//      static double SIGMA;            // Stefan-Boltzmann           [W/m^2.K]

#endif /*  */
