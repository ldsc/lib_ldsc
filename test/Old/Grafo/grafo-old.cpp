/*
  =========================================================================
  PROJETO:    Biblioteca LIB_LMPT
  Assunto/Ramo: Programa de teste da LIB_LMPT
  =========================================================================

  Desenvolvido por:
  Laboratorio de Meios Porosos e Propriedades Termofisicas
  [LMPT].
  @author     Andr� Duarte Bueno
  @file       main.cpp
  @begin      Sat Sep 16 2000
  @copyright  (C) 2000 by Andr� Duarte Bueno
  @email      andre@lmpt.ufsc.br
*/

/*
  Descri��o da fun��o main():
  ===========================
  1-Incluir arquivos basicos
  2-Cria TTeste objeto que cria e testa os diversos objetos da LIB_LMPT
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
//#include "TTeste.h"

// Para teste da permeabilidade
#include <Grafo/CTCGrafo.h>

// Fun��o main da LIB_LMPT
int main(int argc, char *argv[])
{
    cout <<
         "\n=======================================================================================\
     \nUSO:								\
     \n./grafo LISTA & \nonde LISTA � um arquivo *.txt com o		\
     \n  endere�o completo de cada imagem a ser processada, a palavra end finaliza a lista. \
     \n======================================================================================="
         << endl;

    CTCGrafo *grafo = new CTCGrafo;
    assert (grafo);
    if (argc == 1)
    {
        double perm = grafo->Permeabilidade ();
    }
    else
    {
        grafo->ALL (argc, argv);
    }
    delete grafo;
    return EXIT_SUCCESS;
}

