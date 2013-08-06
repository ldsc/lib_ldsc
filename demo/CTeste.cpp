/*
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: TPadrao_ramo
  ===============================================================================
  Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
  	[LDSC].
  @author:          Andre Duarte Bueno
  File:             CTeste.cpp
  begin:            Sat Sep 16 2000
  copyright:        (C) 2000 by Andre Duarte Bueno
  email:            andre@lmpt.ufsc.br
*/
// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <iostream>

// Teste alocacao de dados
#include <cassert>

// String de c++
#include <string>

using namespace std;

// Classe CTeste.h
#include "CTeste.h"

// Classes criadas localmente Para teste do grafo e da  permeabilidade
#include <Grafo/CGrafoTest.h>

/*
  -------------------------------------------------------------------------
  Funcao:  	Run
  -------------------------------------------------------------------------
  @short  :
  Funcao principal, executada por main.
  Vai solicitar ao usuario o tipo de objeto a ser criado,
  criar o objeto e passar o controle
  do programa para o objeto criado

  A LIB_LDSC e muito grande, para poder testar o diversos objetos foi
  adotada a seguinte solucao:
  A funcao Main cria um objeto CTeste e executa Run.
  A funcao Run solicita o nome do diretorio a ser testado,
  cria um objeto do diretorio (nome deve ser TTNomeDiretorio)
  e chama a funcao Run TTNomeDiretorio->Run().

  @author :	Andre Duarte Bueno

  @see  		Testes
  @param		nada
  @return   void
*/
void CTeste::Run(unsigned int argc, char *argv[])
{

    cout <<
         "\n================================================================================\
			\n====================== Programa de teste da LIB_LIDC ===========================\
			\n================================================================================" << endl;

    char selecao;
    string traco = "\n------------------------------------------------------------------------";
    string limpaTela="\n\n\n\n\n\n\n\n\n\n";

    /*  do
      {
        // Selecao do objeto a ser testado
        cout << limpaTela <<"Objeto CTeste";
        cout << traco;
        cout <<"\nSelecione o Diretorio com os <<Objetos>> a serem testados.";
        cout << traco;

        // Lista dos diretorios com classes TTNomeDiretorio
        cout << "\nClima.....................................a:";
        cout << "\nConfEq....................................b:";
        cout << "\nConectividade.............................c:";
        cout << "\nCorrelacao..(TFFT,).......................d:";
        cout << "\nEstatistica...............................e:";
        cout << "\nFiltro....................................f:";
        cout << "\nGrafo.....................................g:";
        cout << "\nIntegral..................................h:";
        cout << "\nInterpolacao..............................i:";
        cout << "\nMatriz....................................j:";
        cout << "\nPermeabilidade............................k:";
        cout << "\nReconstrucao..............................l:";
        cout << "\nSMatriz...................................l:";
        cout << "\nQuit.......................................q:" << endl ;
        cin.get(selecao);
        cin.get();			// pega retorno de carro
        cout << "\nSelecionou a opcao ("<<selecao<<")";

        CGrafoTest * grafo;
        double perm;
        switch(selecao)
      {
      case 'a': case 'A':
      case 'b': case 'B':
      case 'c': case 'C':
      case 'd': case 'D':
      case 'e': case 'E':
      case 'f': case 'F':
      case 'h': case 'H':
      case 'i': case 'I':
      case 'j': case 'J':
      case 'l': case 'L':
        cout << "\n Objetos ja existem, mas nao foram incluidos";
        cout << "\n nesta compilacao. Aguarde versao futura."<<endl;
        break;
      case 'k': case 'K':
        cout << "\nCriando o objeto CGrafoTest." <<endl;
        grafo = new CGrafoTest ();
        assert (grafo);
        perm = grafo->Permeabilidade ();
        delete grafo;
        break;
      case 'g': case 'G':
        // Teste do grafo
        cout << "\n Criando o objeto CGrafoTest." <<endl;
        grafo = new CGrafoTest ();
        assert(grafo);
        grafo->Run();
        delete grafo;
        break;
      case 'q': case 'Q':
        cout << "\n Encerrando programa.."<<endl;
        break;
      default:
        cout << "\n Selecao invalida"<<endl;
        break;
      } // switch
      }while(selecao != 'q' && selecao != 'Q');
    */

    /// As linhas abaixo devem ser retiradas quando o do..while for ativado
    CGrafoTest * grafo;
    double perm;
    // Teste do grafo
    cout << "\n Criando o objeto CGrafoTest." <<endl;
    grafo = new CGrafoTest ();
    assert(grafo);
    grafo->Run();
    delete grafo;

    return;
}

