#ifndef COGLigacao_h
#define COGLigacao_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: COGLigacao...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       COGLigacao.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <vector>
#include <iostream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

// Definição de CObjetoGrafo


#ifndef CObjetoGrafo_h
#include <Grafo/CObjetoGrafo.h>
#endif

/**
 * @brief      Representa uma objeto ligação de um grafo.
 * é herdeiro de CObjetoGrafo,tendo uma propriedade (herdada de 
 * CParametroSolver)  e um rótulo (herdado de CObjetoGrafo).
 * 
 * A característica básica de uma COGLigacao é que este pode ter
 * n coneccao's, ou seja estar conectado a n objetos do tipo
 * CObjetoGrafo, mas cada conecção é 1 para 2 (duas garras),
 * ou seja cada ligação conecta dois objetos do tipo  CObjetoGrafo ao mesmo tempo.
 * 
 * Observe a diferença, um COGSitio pode ter n coneccao's  mas cada conecção a um 
 * único objeto,já uma ligação vai ter n conexões's,  
 * e cada coneccao ocorre em pares.
 * 
 * Exemplo:
 * Para sítios (uma garra)
 * sítio-->sítio		// sítio conectado a sítio
 * sítio-->ligação           // sítio conectado a ligação
 * 
 * Para ligações (duas garras)
 * lig	<--ligação-->	lig   	// Ligação entre duas ligações
 * sítio	<--ligação-->	lig   // A primeira ligação após um sítio
 * lig	<--ligação-->	sítio 	// A última ligação
 * sítio	<--ligação-->	sítio // Conecção entre sítios com uma única ligação
 * 
 * @author 	André Duarte Bueno	
 * @see		grafos
*/
class COGLigacao : public CObjetoGrafo
{
// --------------------------------------------------------------Atributos

protected:
  // Tem uma lista de ponteiros para objetos do tipo CObjetoGrafo que é generico
  // Vetor de ponteiros para CObjetoGrafo

  /// Conecções a esquerda
  std::vector < CObjetoGrafo * >coneccaoA;

  /// Conecções a direita
  std::vector < CObjetoGrafo * >coneccaoB;

public:
// -------------------------------------------------------------Construtor
/// Construtor
	COGLigacao ()
    {
  }

// --------------------------------------------------------------Destrutor

/// Destrutor  
  virtual ~ COGLigacao ()
  {
  }


// ----------------------------------------------------------------Métodos
protected:

/**
* @brief   Função que recebe um ponteiro para um CObjetoGrafo,
 * e o inclue na lista de coneccoes's
 * */
  virtual void Conectar (CObjetoGrafo * objA, CObjetoGrafo * objB);

  /// Determina o fluxo associado a this, Função herdada.
  virtual long double Fluxo () const;

  // Funcao Go

  /// Salva os dados do objeto em disco
  virtual std::ostream & Write (std::ostream & os) const;

  // Salva atributos do objeto em disco no formato antigo
  // virtual ostream&  Write_Liang_Format(ostream& os)     const ;

public:
// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
	/// Sobrecarga do operador <<.
  friend std::ostream & operator<< (std::ostream & os, COGLigacao & obj);
  // friend istream& operator>> (istream& is, COGLigacao& obj);

};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream & operator<< (std::ostream & os, COGLigacao & obj);
// istream& operator>> (istream& is, COGLigacao& obj);

#endif
