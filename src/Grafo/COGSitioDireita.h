#ifndef COGSitioDireita_h
#define COGSitioDireita_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: COGSitioDireita...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       COGSitioDireita.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andre@lmpt.ufsc.br
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
// Documentacao CLASSE: COGSitioDireita
// ===============================================================================
/** 
 * @brief Representa um sítio conectado a face direita do grafo.
 * 
 * Superclasse:    CParametroSolver->CObjetoGrafo->COGSitio->COGSitioDireita
 * Assume valor de contorno = 2.
 * O valor de contorno é usado no calculo dos fluxo da malha como
 * um todo.Ou seja, um algoritimo externo percorre toda
 * a malha, e se o contorno for igual ao solicitado
 * calcula alguma propriedade neste contorno.
 * @author 	André Duarte Bueno	
 * @see			grafos
*/
class COGSitioDireita : public COGSitio
{
// --------------------------------------------------------------Atributos

public:

// -------------------------------------------------------------Construtor
/// Construtor
	COGSitioDireita ()
  {
  }

// --------------------------------------------------------------Destrutor
  
/// Destrutor
  virtual ~ COGSitioDireita ()
  {
  }

// ----------------------------------------------------------------Métodos

public:
  // Redefinição de funções herdadas
   /// Retorna o tipo de contorno
  virtual CContorno::ETipoContorno Contorno ()const
  {
    return CContorno::EST;
  }

   /**
    * @brief Função herdade da classe CParametroSolver usada para 
    * calcular o valor de x retorna a pressão na fronteira direita
   */
  virtual long double Go (long double d = 0)
  {
    return x;
  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
  // friend ostream& operator<< (ostream& os, COGSitioDireita& obj);
  // friend istream& operator>> (istream& is, COGSitioDireita& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, COGSitioDireita& obj);
// istream& operator>> (istream& is, COGSitioDireita& obj);

#endif
