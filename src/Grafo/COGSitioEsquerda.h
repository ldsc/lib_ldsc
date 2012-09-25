#ifndef COGSitioEsquerda_h
#define COGSitioEsquerda_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: COGSitioEsquerda...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       COGSitioEsquerda.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
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

#include <Grafo/COGSitio.h>

// ===============================================================================
// Documentacao CLASSE: COGSitioEsquerda
// ===============================================================================
/** 
 * @brief Representa um sítio conectado a face esquerda do grafo.
 * Assunto:        CGrafo
 * Superclasse:    CParametroSolver->CObjetoGrafo->COGSitio->COGSitioEsquerda
 * 
 * O 	valor de contorno é usado no calculo dos fluxo da
 * malha como  um todo.
 * Ou seja, um algoritimo externo percorre toda
 * a malha, e se o contorno for igual ao solicitado
 * calcula alguma propriedade neste contorno.
 * @author 	André Duarte Bueno		
 * @see		CGrafo
*/
class COGSitioEsquerda : public COGSitio
{
// --------------------------------------------------------------Atributos
public:
// -------------------------------------------------------------Construtor
/// Construtor
	COGSitioEsquerda ()
  {
  }

// --------------------------------------------------------------Destrutor

/// Destrutor
    virtual ~ COGSitioEsquerda ()
  {
  }

// ----------------------------------------------------------------Métodos
// --------------------------------------------------------------Atributos
public:
   /// Retorna o tipo de contorno a que pertence
  virtual CContorno::ETipoContorno Contorno ()const	// Tipo()
  {
    return CContorno::WEST;
  }

   /**
    * @brief Função herdada da classe CParametroSolver,
    * usada para calcular o valor de x retorna a pressão na fronteira direita
   */
  virtual long double Go (long double d = 0)
  {
    return x;
  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, COGSitioEsquerda& obj);
//       friend istream& operator>> (istream& is, COGSitioEsquerda& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, COGSitioEsquerda& obj);
// istream& operator>> (istream& is, COGSitioEsquerda& obj);

#endif
