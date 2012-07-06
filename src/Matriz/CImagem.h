#ifndef CImagem_h
#define CImagem_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CImagem...
===============================================================================
Desenvolvido por:	
            Lab
            oratorio de Meios Porosos 	
            [LDSC].
@author     André Duarte Bueno
@file       CImagem.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// por causa acesso a disco
#include <fstream>
#include <string>

// por causa abs
#include <cstdlib>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------

#ifndef CMatriz2D_h
#include <Matriz/CMatriz2D.h>
#endif
/* // comentei para poder fazer dynamic_cast
#ifndef CImg_h
#include <Matriz/CImg.h>
#endif
*/
/**
 * 
 * @brief	Representra uma imagem bidimensional.	
 * 
 * É uma matriz bidimensional, herdeira da CMatriz2D
 * Acrescenta os atributos x0 e y0 que representam
 * as coordenadas centrais na matriz de dados.
 * Acrescenta uma função contem que verifica se o ponto passado
 * pertence a imagem.
 * @author 	André Duarte Bueno	
 * @see		Matrizes
*/
class CImagem : public CMatriz2D//, public CImg // comentei para poder fazer dynamic_cast
{
// --------------------------------------------------------------Atributos
public:
  int x0, y0;
// -------------------------------------------------------------Construtor
  /// Constrói uma imagem vazia, não alocada
  CImagem ():CMatriz2D ()
  {
     x0 = y0 = 0;
  }

  /// Constrói a imagem a partir de arquivo de disco
  CImagem (std::string fileName):CMatriz2D (fileName)
  {
    x0 = y0 = 0;
  }

  /// Constrói a imagem bidimensional a partir de plano de imagem tridimensional
  CImagem (std::string fileName, int planoZ):CMatriz2D (fileName, planoZ)
  {
    x0 = y0 = 0;
  }

  /// Construtor de cópia
CImagem (CImagem & img):CMatriz2D (img)
  {
     x0 = y0 = 0;
  }

  /// Constrói imagem nova, com as dimensões passadas
  CImagem (int nx, int ny):CMatriz2D (nx, ny)
  {
     x0 = y0 = 0;
  }

  /// Constrói uma imagem a partir de uma matriz2D, faz x0=y0=0
CImagem (CMatriz2D & matriz):CMatriz2D (matriz)
  {
    x0 = y0 = 0;
  }

// --------------------------------------------------------------Destrutor
  /// Destrutor
  virtual ~ CImagem ()
  {
  }

// ----------------------------------------------------------------Métodos
private:
  // Mover para CImagem
	/// Verifica o formato
  void VerificaFormato (std::ifstream & fin);

public:

  /// Verifica se a imagem contem o ponto, o mesmo que checaIndice
  inline bool Contem (int i, int j) const
  {
  // acusa ser sempre verdadeira?
  if (i < nx && j < ny)
    return 1;
  else
    return 0;
  }

  /// Inverte valores 0-1
  void Inverter ();

  /// Faz a interseção entre this e pm2
  CMatriz2D *Intersecao (CMatriz2D * pm2);

  /// Faz a união entre this e pm2
  CMatriz2D *Uniao (CMatriz2D * pm2);

  // Compara this e pm2
  // CMatriz2D* Igual(CMatriz2D*pm2);                              

// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CImagem& obj);
//       friend istream& operator>> (istream& is, CImagem& obj);

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CImagem& obj);
// istream& operator>> (istream& is, CImagem& obj);

// A partir daqui IMAGE é um ponteiro para CImagem
typedef CImagem *IMAGE;

// A função abaixo, retorna um vetor float**, ou seja, uma imagem bidimensional,
// acessada pelo vetor float**
// foi incluída para dar suporte a alguma função do Liang, posteriormente
// esta classe vai ser templatizada e a mesma será eliminada
float **f2d (int nr, int nc);
#endif
