/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
                  	[LDSC].
@author:          André Duarte Bueno
@file:             CGrafoContorno.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno
@email:            andreduartebueno@gmail.com
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoContorno.h>

// #ifndef CSitio_h
// #include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio.h>
// #endif
// 
// #ifndef COGSitio_WEST_h
// #include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_WEST.h>
// #endif
// 
// #ifndef COGSitio_EST_h
// #include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_EST.h>
// #endif

using namespace std;

// -----------------------------------------------------------------------
// Função     Construtor
// -----------------------------------------------------------------------
// Recebe o número de contornos,
// Cria os objetos e adiciona ao vetor de contornos.
// O nomeArquivo é repassado a classe base.
CGrafoContorno::CGrafoContorno (string _nomeArquivo,
				unsigned long int _numero_contornos):
		      CGrafo (_nomeArquivo)
		      //, contorno(_numero_contornos, new CContorno())
{
  tipoGrafo  =  ETipoGrafo::grafoContorno ; 
//   contorno.resize(_numero_contornos); // redimensiona (size)
//   generate(contorno.begin(), contorno.end(), new CContorno ());
  for (unsigned long int i = 0; i < _numero_contornos; i++)
    {
      CContorno *obj_cont = new CContorno ();
      assert (obj_cont); ///@todo: static_assert ou try..catch
      contorno.push_back (obj_cont);
    }
}

// -----------------------------------------------------------------------
// Função     Destrutor
// -----------------------------------------------------------------------
// Os objetos contorno criados por esta classe no construtor, precisam ser deletados.
// Ou seja os contornos são objetos todo parte do CGrafoContorno.
CGrafoContorno::~CGrafoContorno ()
{
  for (unsigned long int i = 0; i < contorno.size (); i++)
    if (contorno[i])
      delete contorno[i];
}

/*
-------------------------------------------------------------------------
Função:	operator<<
-------------------------------------------------------------------------
@short  : Salva dados do objeto em "os" (disco);
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
