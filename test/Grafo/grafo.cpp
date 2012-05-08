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
// Para teste da permeabilidade
#include "CGrafoTeste.h"

/**
 * Funcao para teste do metodo de determina��o da permeabilidade
 * usando o grafo de conexao serial.
 * @param argc
 * @param argv[]
 * @return
 */
int main(int argc, char *argv[])
{
    cout  << "\n================================================================================"
          << "\n====================== Programa de teste da LIB_LDSC ==========================="
          << "\n================================================================================" << endl;

    cout <<
         "\n================================================================================\
     \nUSO:\
     \n./grafo LISTA & \
     \nonde LISTA é um arquivo *.txt com o \
     \nendereco completo de cada imagem a ser processada, \
     \na palavra end finaliza a lista.	\
     \n================================================================================"
         << endl;

    // Cria objeto grafo
    CGrafoTest *grafo = new CGrafoTest;
    assert (grafo);

    // Se passou argumento na linha de comando com o nome da imagem chama metodo
    // para determina��o da permeabilidade.
    if (argc == 1)
    {
        double perm = grafo->Permeabilidade ();
    }
    // Senao chama metodo global, que solicita dados para o usu�rio.
    else
    {
        grafo->ALL (argc, argv); // trocar por Run
    }
    delete grafo;
    return EXIT_SUCCESS;
}

