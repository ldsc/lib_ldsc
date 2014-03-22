/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:
				Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:  	André Duarte Bueno
<<<<<<< HEAD:src/MetNum/Solver/SistemaEquacoes/CSMParametroSolver.cpp
@file:    	CSMParametroSolver.cpp
=======
@file:    	CSolverMatriz_ParametroSolver.cpp
>>>>>>> renomeada hierarquia solvers matriz diagonal; criado @defgroup HCSolver para doxygen.:src/MetNum/Solver/SistemaEquacoes/CSolverMatriz_ParametroSolver.cpp
@begin:   	Sat Sep 16 2000
@copyright: (C) 2000 by André Duarte Bueno
@email:     andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <MetNum/Solver/SistemaEquacoes/CSolverMatriz_ParametroSolver.h>

using namespace std;

// -------------------------------------------------------------------------
// Função:               operator<<
// -------------------------------------------------------------------------
/** Salva atributos do objeto em disco.
@author :	André Duarte Bueno
 
@param  :	ostream& os, CSolverMatriz_ParametroSolver& obj
@return : ostream& os
*/
ostream & operator<< (ostream & os, const CSolverMatriz_ParametroSolver & obj)
{
  os << obj.x << endl;
  return os;
}

// -------------------------------------------------------------------------
// Função:       operator>>
// -------------------------------------------------------------------------
/**
@short  : Lê os atributos do objeto do arquivo de disco
@author :	André Duarte Bueno
 
@param  :	istream& is, CSolverMatriz_ParametroSolver& obj
@return :	istream&
*/
istream & operator>> (istream & is, CSolverMatriz_ParametroSolver & obj)
{
  is >> obj.x;
  return is;
}
