#ifndef CImagemVirtual_h
#define CImagemVirtual_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CImagemVirtual...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CImagemVirtual.h
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
#ifndef CImagem_h
#include <AnaliseImagem/Matriz/CImagem.h>
#endif

/**
 * @brief Cria conceito de imagem virtual, baseada no rótulo.
 * 
 * -Não é uma imagem real
 * -Pertence a uma imagem
 * -É um conjunto de píxeis que formam um objeto da imagem
 * 
 * -2 objetos não podem ocupar o mesmo espaço ao mesmo tempo
 * 
 * -Para construir basta passar a matriz imagem, e o rotulo.
 * 
 * -Para usar acessar use Getij(i,j), que retorna 1 se a posição i,j faz
 * parte do objeto ou 0 caso contrario.
 * @author 	André Duarte Bueno	
 * @see		Veja assunto
*/
class CImagemVirtual : public CImagem
{
// --------------------------------------------------------------Atributos
protected:
  /// Rótulo do objeto a ser considerado como 1
  int rotuloObjeto;

  /// Ponteiro para imagem bidimensional
  int **data2D;

  int nx; ///< Dimensão nx
  int ny; ///< Dimensão ny

public:
// -------------------------------------------------------------Construtor
	/// Construtor
    CImagemVirtual ()
  {
  }

// --------------------------------------------------------------Destrutor
  /// Destrutor
  virtual ~ CImagemVirtual ()
  {
  }

// ----------------------------------------------------------------Métodos
  /// Recebe ponteiror para imagem bidimensional e rótulo
  CImagemVirtual (CMatriz2D * imagem, int rotulo) : rotuloObjeto (rotulo)
  {
    nx = imagem->NX ();

    ny = imagem->NY ();
    // bug dois objetos vao apontar para mesma memoria
    // ou copia, ouy trava a imagem.
    data2D = imagem->Data2D ();
  }

  /// Retorna a informação do píxel i e j
  int Getij (int i, int j)
  {
    if (data2D[i][j] == rotuloObjeto)
      return 1;
    else
      return 0;
  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
  // friend ostream& operator<< (ostream& os, CImagemVirtual& obj);
  // friend istream& operator>> (istream& is, CImagemVirtual& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CImagemVirtual& obj);
// istream& operator>> (istream& is, CImagemVirtual& obj);

#endif
