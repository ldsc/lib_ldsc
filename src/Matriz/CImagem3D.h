#ifndef CImagem3D_h
#define CImagem3D_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CImagem3D...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CImagem3D.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno

@email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// Por causa acesso a disco
#include <fstream>
#include <string>
#include <iostream>
#include<iomanip>
// Por causa abs
#include <cstdlib>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef CMatriz3D_h
#include <Matriz/CMatriz3D.h>
#endif
/* // comentei para poder fazer dynamic_cast
#ifndef CImg_h
#include <Matriz/CImg.h>
#endif
*/
using namespace std;

/**
 * @brief 	Representra uma imagem tridimensional.
 *
 * É uma matriz tridimensional, herdeira da CMatriz3D e da CImg
 * Acrescenta os atributos:
 * x0, y0, z0  - representam as coordenadas centrais na matriz de dados.
 * fatorAmplificacao - representa o fator de amplificação utilizado para recontruir a imagem
 * sizePixel - resolução utilizada na obtenção da imagem pelo microscópio
 * numeroPixelsBorda - número de pixels que representam a borda da imagem (normalmente = 0).
 *
 * Acrescenta a funcao Contem (int i, int j, int k) que verifica
 * se o ponto passado pertence a imagem.
 * @author 	André Duarte Bueno
 * @see		Matrizes
*/
class CImagem3D : public CMatriz3D//, public CImg //comentei para poder fazer dynamic_cast
{

// --------------------------------------------------------------Atributos
public:
    int x0, y0, z0; 				/// Coordenada z0 da imagem 3D
    unsigned int fatorAmplificacao; /// Fator de amplificação utilizado na recontrução da imagem
    double sizePixel;               /// Resolução utilizada na obtenção da imagem pelo microscópio
    unsigned int numeroPixelsBorda; /// Número de pixels que representam a borda da imagem (normalmente = 0).


// -------------------------------------------------------------Construtor
    /// Constróe imagem 3D vazia
    CImagem3D ():CMatriz3D ()/*, CImg()*/, z0 (0), fatorAmplificacao (0), sizePixel (0), numeroPixelsBorda (0)
    {
    }

    /// Constróe imagem 3D a partir de imagem 3D no disco
    CImagem3D (std::string fileName): CMatriz3D (fileName),/* CImg(),*/ z0 (0)
    {
        LeInformacoesRecontrucao(fileName);
    }

    /*  CImagem3D(CImagem3D* img)
    	:CMatriz3D(dynamic_cast< CMatriz3D* > (img)),z0(0)	{}; */

    /// Construtor de cópia
    CImagem3D (CImagem3D & img):CMatriz3D (img),/* CImg(),*/ z0 (img.z0), fatorAmplificacao (img.fatorAmplificacao),
            sizePixel (img.sizePixel), numeroPixelsBorda (img.numeroPixelsBorda)
    {
    }

    /// Constróe imagem vazia, com as dimensões nx,ny,nz
    CImagem3D (int nx, int ny, int nz):CMatriz3D (nx, ny, nz),/* CImg(),*/ z0 (0), fatorAmplificacao (0), sizePixel (0), numeroPixelsBorda (0)
    {

    }
    /*  CImagem3D(CMatriz3D* matriz)
    :CMatriz3D(matriz),z0(0)
    {};*/

    /// Construtor de cópia  a partir de uma matriz 3D
    CImagem3D (CMatriz3D & matriz):CMatriz3D (matriz), z0 (0), fatorAmplificacao (0), sizePixel (0), numeroPixelsBorda (0)
    {
    }

// --------------------------------------------------------------Destrutor
    /// Destrutor
    virtual ~ CImagem3D ()
    {
    }

// ----------------------------------------------------------------Métodos
    /// Verifica se a imagem contem o ponto   // checaIndice
    inline bool Contem (int i, int j, int k) const;

    /// Armazena no arquivo .dbm, em forma de comentário, as informações de reconstrução da imagem. Deve ser chamado após CMatriz3D::SalvaCabecalho().
    void SalvaInformacoesRecontrucao (ofstream & fout);

    /// Lê de arquivo .dbm as informações de reconstrução da imagem. Recebe ponteiro para arquivo.
    bool LeInformacoesRecontrucao (ifstream & fin);

    /// Lê de arquivo .dbm as informações de reconstrução da imagem. Recebe o nome do arquivo.
    bool LeInformacoesRecontrucao (string fileName);

    bool Write(string fileName, int separado = 1);


// --------------------------------------------------------------------Get
    /// Retorna o valor de z0
    inline int Z0 () const
    {
        return z0;
    }

    /// Retorna o valor de fatorAmplificacao
    inline unsigned int FatorAmplificacao () const
    {
        return fatorAmplificacao;
    }

    /// Retorna o valor de sizePixel
    inline double SizePixel () const
    {
        return sizePixel;
    }

    /// Retorna o valor de numeroPixelsBorda
    inline unsigned int NumeroPixelsBorda () const
    {
        return numeroPixelsBorda;
    }

// --------------------------------------------------------------------Set
    /// Seta o valor de z0
    inline void Z0 (int _z0)
    {
        z0 = _z0;
    }

    /// Seta o valor de fatorAmplificacao
    inline void FatorAmplificacao ( unsigned int _fatorAmplificacao)
    {
        fatorAmplificacao = _fatorAmplificacao;
    }

    /// Seta o valor de sizePixel
    inline void SizePixel ( double _sizePixel)
    {
        sizePixel = _sizePixel;
    }

    /// Seta o valor de numeroPixelsBorda
    inline void NumeroPixelsBorda ( unsigned int _numeroPixelsBorda)
    {
        numeroPixelsBorda = _numeroPixelsBorda;
    }

    /// Seta os valores de x0 y0 e z0 simultaneamente
    inline void X0Y0Z0 (int _x0, int _y0, int _z0)
    {
        x0 = _x0;
        y0 = _y0;
        z0 = _z0;
    }

    // inline          void Setbpp(unsigned char& _bpp){bpp=_bpp;};    //
    // inline          void SetX0(unsigned short int& X0){x0=X0;};
    // inline          void SetY0(unsigned short int& Y0){y0=Y0;};
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CImagem3D& obj);
//       friend istream& operator>> (istream& is, CImagem3D& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CImagem3D& obj);
// istream& operator>> (istream& is, CImagem3D& obj);

// A partir daqui IMAGE3D é um ponteiro para CImagem3D
typedef CImagem3D *IMAGE3D;

#endif
