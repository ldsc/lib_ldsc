#ifndef  _LIB_LDSC_CLASS_h
#define  _LIB_LDSC_CLASS_h

/**
=====================================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: _LIB_LDSC_CLASS...
=====================================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       _LIB_LDSC_CLASS.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andre@lmpt.ufsc.br

*/
/// -----------------------------------------------------------------------
/// Definicao uso dll
/// -----------------------------------------------------------------------
/*
#if defined(_LIB_LDSC_DLL)                /// se dll _export
  #define _LIB_LDSC_CLASS _export

#elif defined(_LIB_LDSC_EXE)
  #define _LIB_LDSC_CLASS _import         /// se app _import

#else
  #define _LIB_LDSC_CLASS                 /// senão default (nada)
#endif
*/

/// Para compilar a lib deixa as duas linhas abaixo
/// Usando namespace  std
/// #include <vector>  
/// using namespace std;
/// 

/// Para desativar o debug, setar abaixo
/// #define NDEBUG

#endif /*  */
