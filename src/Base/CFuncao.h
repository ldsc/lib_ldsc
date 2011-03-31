#ifndef CFuncao_h
#define CFuncao_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CFuncao...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     Andre Duarte Bueno.
@file       CFuncao.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by Andre Duarte Bueno.
@email      andre@lmpt.ufsc.br
*/

/// -----------------------------------------------------------------------
/// Bibliotecas C/C++
/// -----------------------------------------------------------------------

/// -----------------------------------------------------------------------
/// Bibliotecas LIB_LDSC
/// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif /*  */

/*
===============================================================================
Documentacao CLASSE: CFuncao
===============================================================================
*/
/** 
 * @brief Um objeto desta classe representa uma função matemática.
 *  
 * Uma função qualquer que recebe dois parametros x,y
 * e calcula z = f(x,y).
 * 
 * A função pode ter uma série de parâmetros que serão passados para o 
 * construtor ou setados utilizando-se métodos próprios.
 * O motivo que levou a criação desta classe foi sua utilização por métodos 
 * de integração.
 * 
 * A função principal de calculo deve receber unicamente as cordenadas x e 
 * y e retornar a cordenada z, ou seja uma funcao z = f(x,y).
 * 
 * Observe que Go é virtual, logo, sua chamada recursiva pode implicar
 * em demoras adicionais. Neste caso, substitua o uso de obj->Go(x,y)
 * por algo como obj.Go(x,y); (chamada direta, sem polimorfismo)

 * Tarefas:
 * Trocar Go pela sobrecarga de ()

@author 	Andre Duarte Bueno	
@version						
@see		Funções
*/
class CFuncao
{

/// --------------------------------------------------------------Atributos
public:
/// -------------------------------------------------------------Construtor
  /**
   * Construtor.
   * @return 
   */
  CFuncao ()
  {
  }

/// --------------------------------------------------------------Destrutor
  /**
   * Destrutor.
   * @return 
   */
  virtual ~ CFuncao ()
  {
  }

/// ----------------------------------------------------------------Métodos
public:
  /**
   * Calcula z=f(x,y);
   * @param x 
   * @param y 
   * @return retorna o resultado de z=f(x,y);
   */
  virtual double Go (double x = 0, double y = 0) = 0;

/// -------------------------------------------------------------Sobrecarga
  /**
   * Sobrecarga operador (), chama Go(x,y).
   * @param x 
   * @param y 
   * @return retorna o resultado de z=f(x,y);
   */
  inline double operator   () (double x, double y = 0)
  {
    return Go (x, y);
  }

/// --------------------------------------------------------------------Get
/// --------------------------------------------------------------------Set
/// -----------------------------------------------------------------Friend
  /// friend ostream& operator<< (ostream& os, CFuncao& obj);
  /// friend istream& operator>> (istream& is, CFuncao& obj);
};

#endif /*  */
