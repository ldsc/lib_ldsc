#ifndef COGSitioDirLR_h
#define COGSitioDirLR_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: COGSitioDirLR...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       COGSitioDirLR.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Grafo/COGSitioLR.h>

// ===============================================================================
// Documentacao CLASSE: COGSitioDirLR
// ===============================================================================
/** 
 * @brief Representa um sítio que armazena a informação associada a conexão.
 * 
 * Assume valor de contorno = 2.
 * O valor de contorno é usado no calculo dos fluxo da malha como
 * um todo.Ou seja, um algoritimo externo percorre toda
 * a malha, e se o contorno for igual ao solicitado
 * calcula alguma propriedade neste contorno.
 * @author 	André Duarte Bueno	
 * @see			grafos
 * @Superclasse:    CParametroSolver->CObjetoGrafo->COGSitio->COGSitioLR->COGSitioDirLR
 */
class COGSitioDirLR : public COGSitioLR
{
// --------------------------------------------------------------Atributos

public:

// -------------------------------------------------------------/**Construtor*/
/// Construtor
	COGSitioDirLR ()
  {
  }

// --------------------------------------------------------------/**Destrutor*/
 
/// Destrutor
   virtual ~ COGSitioDirLR ()
  {
  }

// ----------------------------------------------------------------Métodos

public:
  // Redefinição de funções herdadas

   /// Retorna o tipo de contorno
  virtual CContorno::ETipoContorno Contorno ()const	// Tipo()
  {
    return CContorno::EST;
  }

   /**
    * @brief Função herdada da classe CParametroSolver usada para 
    * calcular o valor de x retorna a pressão na fronteira direita
    * Observe que retorna direto o valor de x, ou seja,
    * um objeto da fronteira nao recalcula seu fluxo
   */
  virtual long double Go (long double d = 0)
  {
    return x;
  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
  // friend ostream& operator<< (ostream& os, COGSitioDirLR& obj);
  // friend istream& operator>> (istream& is, COGSitioDirLR& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, COGSitioDirLR& obj);
// istream& operator>> (istream& is, COGSitioDirLR& obj);

#endif
