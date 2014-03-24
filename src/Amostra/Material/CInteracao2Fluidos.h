#ifndef CInteracao2Fluidos_h
#define CInteracao2Fluidos_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CInteracao2Fluidos...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CInteracao2Fluidos.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <iostream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

/**
===============================================================================
Documentacao Classe: CInteracao2Fluidos
===============================================================================
@short		Representa a interação entre  dois fluidos.
O atributo molhabilidade é um atributo de ligação, surge da interação de dois fluídos.
@author 	André Duarte Bueno	
*/
class CInteracao2Fluidos
{
// --------------------------------------------------------------Atributos
public:
// -------------------------------------------------------------Construtor
  CInteracao2Fluidos () = default;

// --------------------------------------------------------------Destrutor
  ~ CInteracao2Fluidos () = default;

// ----------------------------------------------------------------Métodos
public:
  /// Recebe dois fluidos, e então define qual é molhante e qual é não molhante.
  /// Vai setar a molhabilidade dos fluidos A e B.
  void Molhabilidade (CMFluido * fluidoA, CMFluido * fluidoB)  = 0;

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set

// -----------------------------------------------------------------Friend
  // Sobrecarga streans
  // friend std::ostream & operator<< (std::ostream & os, const CInteracao2Fluidos & obj);
  // friend std::istream & operator>> (std::istream & is, CInteracao2Fluidos & obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// std::ostream & operator<< (std::ostream & os, const CInteracao2Fluidos & obj);
// std::istream & operator>> (std::istream & is, CInteracao2Fluidos & obj);

#endif
