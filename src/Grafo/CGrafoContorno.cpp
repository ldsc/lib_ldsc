/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
                  	[LDSC].
@author:          André Duarte Bueno
File:             CGrafoContorno.cpp
begin:            Sat Sep 16 2000
copyright:        (C) 2000 by André Duarte Bueno
email:            andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cstdio>
#include <cassert>
#include <iomanip>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Grafo/CGrafoContorno.h>

#ifndef CSitio_h
#include <Grafo/COGSitio.h>
#endif

#ifndef COGSitioEsquerda_h
#include <Grafo/COGSitioEsquerda.h>
#endif

#ifndef COGSitioDireita_h
#include <Grafo/COGSitioDireita.h>
#endif

using namespace std;

// -----------------------------------------------------------------------
// Função     Construtor
// -----------------------------------------------------------------------
// Recebe o número de contornos,
// Cria os objetos e adiciona ao vetor de contornos
// Um fileName e repassado a classe base
CGrafoContorno::CGrafoContorno (string _fileName,
				unsigned long int _numero_contornos):
CGrafo (_fileName)
{
  for (unsigned long int i = 0; i < _numero_contornos; i++)
    {
      CContorno *obj_cont = new CContorno ();
      assert (obj_cont);
      contorno.push_back (obj_cont);
    }
};

// -----------------------------------------------------------------------
// Função     Destrutor
// -----------------------------------------------------------------------
// Os objetos contorno criados
// por esta classe no construtor, precisam ser deletados.
// Ou seja os contornos são objetos todo parte do CGrafoContorno
CGrafoContorno::~CGrafoContorno ()
{
  for (unsigned long int i = 0; i < contorno.size (); i++)
    if (contorno[i])
      delete contorno[i];
};

/*
-------------------------------------------------------------------------
Função:	operator<<
-------------------------------------------------------------------------
@short  : Salva dados do objeto em os (disco),
Salva o número de objetos que fazem parte de grafo e
a seguir os dados de cada objeto.
					
@author :	André Duarte Bueno
@see    : grafos
@param  :
@return :	ostream&
*/
ostream & operator<< (ostream & os, const CGrafoContorno & grafoC)
{
  // Numero de objetos
  os << setw (5) << grafoC.objeto.size () << endl;

  // Percorre os objetos e salva em disco as informações de cada objeto
  for (unsigned long int i = 0; i < grafoC.objeto.size (); i++)
    grafoC.objeto[i]->Write (os);
  // os << (*(grafoC.objeto[i]));

  return os;
}
