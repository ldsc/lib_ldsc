/**
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: Programa de teste da LIB_LDSC
  ===============================================================================
  Desenvolvido por:
			  Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
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
#include "CGrafoConexaoSerial_Teste.h"
#include <Base/COperacao.h>

/**
 * Funcao para teste do metodo de determinação da permeabilidade
 * usando o grafo de conexao serial.
 * @param argc
 * @param argv[]
 * @return
 */
int main ( int argc, char* argv[] )
{
   COperacao::Cabecalho_ProgramasDemo_LIBLDSC ( cout );
//cout<< "\n=========================================================================================="
//    << "\n=>    Laboratório de Engenharia de Exploração e Produção de Petróleo - LENEP/CCT/UENF   <="
//    << "\n=>    Setor de Modelagem Matemática e Computacional                                     <="
//    << "\n=>    Núcleo/Laboratório de Desenvolvimento de Software Científico - NDSC/LDSC          <="
//    << "\n=========================================================================================="
//    << "\n=>                 Programa Aplicativo/Demonstrativo da LIB_LDSC                        <="
//    << "\n=>                 =============================================                        <="
//    << "\n=>                                                                                      <="
   cout <<   "=> Programa para cálculo:                                                               <="
        << "\n=> - do Grafo de Conexão Serial de imagens 3D (reconstruídas ou microtomográficas)      <="
        << "\n=> - da Permeabilidade de imagens 3D                                                    <="
        << "\n=> - da Tortuosidade de imagens 3D                                                      <="
        << "\n=========================================================================================="
        << "\nUso:"
        << "\n./Permeabilidade_GrafoConexaoSerial [lista_img]"
        << "\nonde lista_img é um arquivo ascii com endereço completo das imagens a serem processadas." 
		  << "\nA palavra end finaliza a lista."
        << "\nSe a lista_img não for passada vai solicitar o nome da imagem e os dados da simulação."
        << "\n=========================================================================================="
        << endl;

   // Cria objeto grafo_demo
   CGrafoTest* grafo_demo = nullptr;
   grafo_demo = new CGrafoTest;
   assert ( grafo_demo );

   // Se passou argumento na linha de comando com o nome da imagem chama metodo
   // para determinação da permeabilidade.
   if ( argc == 1 ) {
         //double perm = grafo_demo->Permeabilidade ();
         grafo_demo->Run();
      }
   // Senão chama método global, que solicita dados para o usuário.
   else {
         grafo_demo->ProcessarListaImagens ( argc, argv );
      }

   delete grafo_demo;
   return EXIT_SUCCESS;
}


