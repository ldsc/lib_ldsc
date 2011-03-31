#ifndef CImg_h
#define CImg_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CImg...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CImg.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

// ===============================================================================
// Documentacao CLASSE: CImg
// ===============================================================================
/**  
 * @brief Representra uma generalizacao para CImg
@author 	André Duarte Bueno	
@version	Matriz	
*/
class CImg
{
// --------------------------------------------------------------Atributos
public:
    /// Coordenadas do ponto inicial
  int x0, y0;

	/// número de cores em bpp (8=256 cores, 16=64000cores)
  int bpp;

  static int PRETO;		/// Definção do valor da cor PRETO 	// 0
  static int BRANCO;	/// Definção do valor da cor  BRANCO	// 255

  //double size_pixel; // resoluicao
// -------------------------------------------------------------Construtor
/// Construtor
  CImg (int _x0 = 0, int _y0 = 0):x0 (_x0), y0 (_y0)
  {
  }

// --------------------------------------------------------------Destrutor
/// Destrutor
  virtual ~ CImg ()
  {
  }

// ----------------------------------------------------------------Métodos
public:
// --------------------------------------------------------------------Get
  /// Retorna a cordenada x0
  inline int X0 () const
  {
    return x0;
  }

  /// Retorna a cordenada Y0
  inline int Y0 () const
  {
    return y0;
  }

  /// Retorna o número de bits por píxel
  inline int Bpp () const
  {
    return bpp;
  }

// --------------------------------------------------------------------Set
public:
  // Funções de definição dos atributos internos
	/// Define x0
  inline void X0 (int _x0)
  {
    x0 = _x0;
  }

	/// Define y0
  inline void Y0 (int _y0)
  {
    y0 = _y0;
  }

  	/// Define bpp
  inline void Bpp (int _bpp)
  {
    bpp = _bpp;
  }

  	/// Define x0,y0
  inline void X0Y0 (int _x0, int _y0)
  {
    x0 = _x0;
    y0 = _y0;
  }

// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CImg& obj);
//       friend istream& operator>> (istream& is, CImg& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CImg& obj);
// istream& operator>> (istream& is, CImg& obj);

#endif
