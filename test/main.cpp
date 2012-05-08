/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: Programa de teste da LIB_LDSC
  ===============================================================================

  Desenvolvido por:
  Laboratorio de Desenvolvimento de Software Cientifico
  [LDSC].
  @author     Andre Duarte Bueno
  @file       main.cpp
  @begin      Sat Sep 16 2000
  @copyright  (C) 2000 by Andre Duarte Bueno
  @email      andre@lmpt.ufsc.br
*/

/*
  Descricao da funcao main():
  ===========================
  1-Incluir arquivos basicos
  2-Cria TTeste objeto que cria e testa os diversos objetos da LIB_LDSC
*/

// ----------------------------------------------------------------------
// Bibliotecas C/C++
// ----------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <cassert>
using namespace std;

// ----------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------
#include "CTeste.h"

// Para teste da permeabilidade
#include <Grafo/CGrafoTest.h>

// Funcao main da LIB_LDSC
int main(int argc, char *argv[])
{
    // Cria classe de teste
    CTeste * teste = new CTeste ();

    // Testa alocacao
    assert(teste);

    // Executa classe de teste
    teste->Run(argc,argv);

    return EXIT_SUCCESS;
}
