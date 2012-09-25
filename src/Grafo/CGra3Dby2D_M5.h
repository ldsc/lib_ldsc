#ifndef CGra3Dby2D_M5_h
#define CGra3Dby2D_M5_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CGra3Dby2D_M5...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CGra3Dby2D_M5.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// #include <Base/_LIB_LDSC_CLASS.h>
#include <Grafo/CGra3Dby2D_M4.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>

/**
 * @brief Determina o grafo de imagens 3D, modelo 5.
 * Superclasse:    CGrafo -> CGra3Dby2D -> CGra3Dby2D_M2 
 * -> CGra3Dby2D_M5
 * @author 	André Duarte Bueno	
 * @see			grafos
*/
class CGra3Dby2D_M5:public CGra3Dby2D_M4
{
// --------------------------------------------------------------Atributos
protected:
	/// Objeto para saída.
	std::ofstream pixeisDeletados;

  /// Imagem 3D.
  TCMatriz3D<int> *img3D;

public:
// -------------------------------------------------------------Construtor
	/// Construtor
	CGra3Dby2D_M5 (char *_fileName):CGra3Dby2D_M4 (_fileName)
  {
    char arqDisco[256];
      sprintf (arqDisco, "PixeisDeletados_%s", _fileName);
      pixeisDeletados.open (arqDisco);
    if (pixeisDeletados.bad ())
      {
	std::cerr << "\nNão conseguiu abrir arquivo de disco:" 
		  << arqDisco << std::endl;
	exit (0);
      }
  }

// --------------------------------------------------------------Destrutor

	/// Destrutor
    virtual ~ CGra3Dby2D_M5 ()
  {
    pixeisDeletados.close ();
  }

// ----------------------------------------------------------------Métodos
  /// Processa a determinação do grafo.
  CGrafo *Go (TCMatriz3D<int> * _img3D, unsigned long int funcao);

protected:
	/// Adiciona objetos ao grafo
  virtual void AdicionarObjetos
    (CRotulador2DCm * rotulador,
     unsigned long int ultimoRotuloUtilizado,
     CContorno::ETipoContorno tipoContornoObjeto);
     //     unsigned long int tipoContornoObjeto);



// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CGra3Dby2D_M5& obj);
//       friend istream& operator>> (istream& is, CGra3Dby2D_M5& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGra3Dby2D_M5& obj);
// istream& operator>> (istream& is, CGra3Dby2D_M5& obj);

#endif
