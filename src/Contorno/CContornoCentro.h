#ifndef CContornoCentro_h
#define CContornoCentro_h

/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: CContornoCentro...
  ===============================================================================
  Desenvolvido por:	
  Laboratorio de Desenvolvimento de Software Cientifico 	
  [LDSC].
  @author     André Duarte Bueno
  @file       CContornoCentro.h
  @begin      Wed Oct 4 2000
  @copyright  (C) 2000 by André Duarte Bueno
  @email      andreduartebueno@gmail.com
  
  Tarefas:
  
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CContorno_h
#include <Contorno/CContorno.h>
#endif

/**
 * @brief Determina propriedade no contorno.
 * 
 * Classe herdeira de CContorno, a principal diferença
 * é a implementação da função Go que calcula uma estimativa de
 * valor de contorno baseada numa reta do tipo
 * y = a + b.x
 * Go recebe x e retorna y.
 * @author 	André Duarte Bueno	
 * @version		
 * @see		contornos
 * 
 * Tarefa:
 * Verificar se o nome nao deveria ser CCondicaoInicialCentro?
*/

class CContornoCentro : public CContorno
{
  // --------------------------------------------------------------Atributos
 protected:
  /// Coeficiente da reta  y = a + b.x
  long double a;

  /// Coeficiente da reta  y = a + b.x
  long double b;

 public:

  // -------------------------------------------------------------Construtor
 /// Construtor
  CContornoCentro (long double _a = 0, long double _b = 0):a (_a), b (_b)
    {
    }

    // --------------------------------------------------------------Destrutor
 /// Destrutor
    virtual ~ CContornoCentro ()
      {
      }

    // ----------------------------------------------------------------Métodos
 public:

    /// Calcula, atualiza valor da propriedade no contorno.
    // x=k, e y=numeroObjetos
    virtual long double Go (long double x = 0, long double y = 0)
    {
      return (a + b * x);
    }

    // grafo->objeto[k]->x =  ( k / numeroObjetos ) * ( (*contorno[0]) -  (*contorno[1]) );

    // -------------------------------------------------------------Sobrecarga
    // --------------------------------------------------------------------Get
    // --------------------------------------------------------------------Set

    /// Seta valor de a
    void A (long double _a)
    {
	    a = _a;
    }

    /// Seta valor de b
    void B (long double _b)
    {
	    b = _b;
    }

    /// Obtem o valor de a
    long double  A ()
    {
	return a;
    }

    /// Obtem o valor de b
    long double  B ()
    {
	return b;
    }

    // -----------------------------------------------------------------Friend
    //       friend ostream& operator<< (ostream& os, const CContornoCentro& obj);
    //       friend istream& operator>> (istream& is, CContornoCentro& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, const CContornoCentro& obj);
// istream& operator>> (istream& is, CContornoCentro& obj);

#endif
