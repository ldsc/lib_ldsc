/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
	[LDSC].
@author:          Andre Duarte Bueno
File:             COperacao.cpp
begin:            Sat Sep 16 2000
copyright:        (C) 2000 by Andre Duarte Bueno
email:            andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <iostream>
//#include <iomanip>
//#include <fstream>
#include <cstdlib>		// exit.
// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Base/COperacao.h>

// -----------------------------------------------------------------------
// Funaao:
// -----------------------------------------------------------------------
/*
double COperacao::ErroPercentual(double& x,double& y)
{
  return (x - y) / x;
}
*/

/**
 * Realiza uma pausa.
 */
void
COperacao::Pausa ()
{
  cout << "\n\tPressione enter para continuar...";
  cin.get ();
}

/**
 * Limpa tela.
 */
void
COperacao::LimpaTela ()
{
  cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
}
/**
 * Se o objeto nao foi alocado corretamente, encerra o programa
 * informando o erro.
 * @param ptr 
 * @param s 
 */
void
COperacao::TestaAlocacao (void *ptr, string s)
{
  if (ptr == NULL)
    {
      cerr << "\n\tFaltou memoria para alocar o objeto " << s << endl;
      exit (0);
    }
}

// Usada para pegar a extensao de um arquivo, recebe o nome do arquivo e
// o tamanho da extensao (default=3)
/*
string   COperacao::GetExtensao(std::string fileName,int tamanhoExtensao)
{
   string  extensao (fileName.substring(?.?));
   // strxfrm( extensao, fileName, strlen(fileName)-tamanhoExtensao);
	
   return extensao;
}
*/

// -----------------------------------------------------------------------
// Funcao:
// -----------------------------------------------------------------------
// Usada para verificar a extensao do arquivo
// Recebe o nome do arquivo e a extensao retorna 1 se a extensao do arquivo
// e a extensao passadas sao as mesmas
/*
bool  COperacao::VerificaExtensaoArquivo(string fileName, char* extensao)
{
   // Pega a extensao do arquivo
   tamanhoExtensao extensaoArquivo = COperacao::GetExtensao (fileName , sizeof(extensao) );

      // Se forem iguais strcmp retorna 0
   return  extensaoArquivo == extensao ;
}
*/
//    strcat(filename,".pbm");

// void COperacao::RunningView()
/**
 * Mostra sinal de que o programa esta rodando.
 */
void COperacao::RunningView ()
{
  static int p = 0;
  switch (p)
  {
    case 0:
    case 4:
      cout << "\r|" << flush;
      break;
    case 1:
    case 5:
      cout << "\r/" << flush;
      break;
    case 2:
    case 6:
      cout << "\r-" << flush;
      break;
    case 3:
    case 7:
      cout << "\r\\" << flush;
      break;
  }
  p++;
  if (p >= 7)
    p = 0;
}

void COperacao::RunningView (string msg)
{
  static int p = 0;
  switch (p)
  {
    case 0:
    case 4:
      cout << "\r| " << msg << "                                                                    " << flush;
      break;
    case 1:
    case 5:
      cout << "\r/ " << msg << "                                                                    " << flush;
      break;
    case 2:
    case 6:
      cout << "\r- " << msg << "                                                                    " << flush;
      break;
    case 3:
    case 7:
      cout << "\r\\ " << msg << "                                                                    " << flush;
      break;
  }
  p++;
  if (p >= 7)
    p = 0;
}
