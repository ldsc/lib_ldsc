#ifndef CGra3Dby2D_M2_h
#define CGra3Dby2D_M2_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CGra3Dby2D_M2...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CGra3Dby2D_M2.h
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

#ifndef CGra3Dby2D_h
#include <Grafo/CGra3Dby2D.h>
#endif

/**
 * @brief 	Determinação do grafo de imagens 3D usando modelo 2.
 * Para cada objeto uma ligação
 * Condutancia média entre os dois objetos
 *
 * @author 	André Duarte Bueno	
 * @see			grafos
*/
class CGra3Dby2D_M2 : public CGra3Dby2D
{
// --------------------------------------------------------------Atributos
protected:
/// Rotulador para imagem intermediária
  CRotulador2DCm * rotInt;

public:
// -------------------------------------------------------------Construtor
	/// Construtor
  CGra3Dby2D_M2 (std::string _fileName) : CGra3Dby2D (_fileName),  rotInt (NULL) {
  }

// --------------------------------------------------------------Destrutor
	/// Destrutor
  virtual ~ CGra3Dby2D_M2 () {
  }

// ----------------------------------------------------------------Métodos
protected:
  /**
   * @brief  conecta os objetos considerando um plano intermediário, 
   * que é criado para eliminar a repetição dos links.
  */
  virtual void DeterminarConeccoesObjetos (unsigned long int
					   maiorRotuloUtilizado);

   /**
   * @brief  Determina o grafo usando imagem 3D.
   * Aqui, apenas cria o rotulador intermediário e chama Go da classe base
   */
  virtual CGrafo *Go (CMatriz3D * _img3D, unsigned long int _tamanhoMascara =   1);

/**
 * @brief  Determina o grafo lendo a imagem do disco,	plano a plano.
 * Aqui, apenas cria o rotulador intermediário e chama Go da classe base
*/
  virtual CGrafo *Go (std::string fileName, unsigned long int funcao);

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CGra3Dby2D_M2& obj);
//       friend istream& operator>> (istream& is, CGra3Dby2D_M2& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGra3Dby2D_M2& obj);
// istream& operator>> (istream& is, CGra3Dby2D_M2& obj);

#endif


/*  O calculo do fluxo de massa considera a diferença de pressão entre
  os dois objetos e a condutancia média, ou seja.
  Fluxo = (c1+c2)*DP / 2
    */
