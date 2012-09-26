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

@email      andreduartebueno@gmail.com
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
#ifndef TCMatriz3D_h
#include <Matriz/TCMatriz3D.h>
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
template<typename T>
class CImagem3D : public TCMatriz3D<T> //, public CImg //comentei para poder fazer dynamic_cast
{

		// --------------------------------------------------------------Atributos
	public:
		int x0, y0, z0;									/// Coordenadas x0, y0 e z0 da imagem 3D
		unsigned int fatorAmplificacao; /// Fator de amplificação utilizado na recontrução da imagem
		double sizePixel;               /// Resolução utilizada na obtenção da imagem pelo microscópio
		unsigned int numeroPixelsBorda; /// Número de pixels que representam a borda da imagem (normalmente = 0).


		// -------------------------------------------------------------Construtor
		/// Constrói imagem 3D vazia, não alocada.
		CImagem3D ()
			: TCMatriz3D<T> ()/*, CImg()*/, x0(0), y0(0), z0(0), fatorAmplificacao (0), sizePixel (0), numeroPixelsBorda (0) {
		}

		/// Constróe imagem 3D a partir de imagem 3D no disco
		CImagem3D (std::string fileName)
			: TCMatriz3D<T> (fileName),/* CImg(),*/ x0(0), y0(0), z0(0) {
			LeInformacoesRecontrucao(fileName);
		}

		/// Construtor le arquivo RAW do disco. Recebe nome do arquivo, largura, altura, profundidade e tipo (D4_X_Y_Z_BINARY (default), D5_X_Y_Z_GRAY_BINARY ou D6_X_Y_Z_COLOR_BINARY) da imagem.
		CImagem3D (std::string fileRAW, int _nx, int _ny, int _nz, EImageType tipo=D4_X_Y_Z_BINARY )
			:TCMatriz3D<T>(fileRAW, _nx, _ny, _nz, tipo), x0(0), y0(0), z0(0), fatorAmplificacao (0), sizePixel (0), numeroPixelsBorda (0) {
		}

		/// Construtor de cópia
		CImagem3D (CImagem3D & img)
			: TCMatriz3D<T> (img),/* CImg(),*/ x0(img.x0), y0(img.y0), z0 (img.z0), fatorAmplificacao (img.fatorAmplificacao), sizePixel (img.sizePixel), numeroPixelsBorda (img.numeroPixelsBorda) {
		}

		/// Constróe imagem vazia, com as dimensões nx,ny,nz
		CImagem3D (int nx, int ny, int nz)
			: TCMatriz3D<T> (nx, ny, nz),/* CImg(),*/ x0(0), y0(0), z0(0), fatorAmplificacao (0), sizePixel (0), numeroPixelsBorda (0) {

		}

		/// Construtor de cópia  a partir de uma matriz 3D
		CImagem3D (TCMatriz3D<T> & matriz)
			: TCMatriz3D<T> (matriz), x0(0), y0(0), z0(0), fatorAmplificacao (0), sizePixel (0), numeroPixelsBorda (0) {
		}

		// --------------------------------------------------------------Destrutor
		/// Destrutor
		virtual ~ CImagem3D () {
		}

		// ----------------------------------------------------------------Métodos
		/// Verifica se a imagem contem o ponto   // checaIndice
		inline bool Contem ( int i, int j, int k) const {
			return (i < TCMatriz3D<T>::nx && j < TCMatriz3D<T>::ny && k < TCMatriz3D<T>::nz) ? 1 : 0;
		}

		/// Armazena no arquivo .dbm, em forma de comentário, as informações de reconstrução da imagem. Deve ser chamado após CMatriz3D::SalvaCabecalho().
		void SalvaInformacoesRecontrucao (ofstream & fout) const;

		/// Lê de arquivo .dbm as informações de reconstrução da imagem. Recebe ponteiro para arquivo.
		bool LeInformacoesRecontrucao (ifstream & fin);

		/// Lê de arquivo .dbm as informações de reconstrução da imagem. Recebe o nome do arquivo.
		bool LeInformacoesRecontrucao (string fileName);

		/// Salva dados do cabecalho
		virtual void SalvaCabecalho (std::ofstream & fout) const;


		// --------------------------------------------------------------------Get
		/// Retorna o valor de x0
		inline int X0 () const {
			return x0;
		}

		/// Retorna o valor de y0
		inline int Y0 () const {
			return y0;
		}

		/// Retorna o valor de z0
		inline int Z0 () const {
			return z0;
		}

		/// Retorna o valor de fatorAmplificacao
		inline unsigned int FatorAmplificacao () const {
			return fatorAmplificacao;
		}

		/// Retorna o valor de sizePixel
		inline double SizePixel () const {
			return sizePixel;
		}

		/// Retorna o valor de numeroPixelsBorda
		inline unsigned int NumeroPixelsBorda () const {
			return numeroPixelsBorda;
		}

		// --------------------------------------------------------------------Set
		/// Seta o valor de x0
		inline void X0 (int _x0) {
			x0 = _x0;
		}

		/// Seta o valor de y0
		inline void Y0 (int _y0) {
			y0 = _y0;
		}

		/// Seta o valor de z0
		inline void Z0 (int _z0) {
			z0 = _z0;
		}

		/// Seta o valor de fatorAmplificacao
		inline void FatorAmplificacao ( unsigned int _fatorAmplificacao) {
			fatorAmplificacao = _fatorAmplificacao;
		}

		/// Seta o valor de sizePixel
		inline void SizePixel ( double _sizePixel) {
			sizePixel = _sizePixel;
		}

		/// Seta o valor de numeroPixelsBorda
		inline void NumeroPixelsBorda ( unsigned int _numeroPixelsBorda) {
			numeroPixelsBorda = _numeroPixelsBorda;
		}

		/// Seta os valores de x0 y0 e z0 simultaneamente
		inline void X0Y0Z0 (int _x0, int _y0, int _z0) {
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
		// A partir daqui IMAGE3D é um ponteiro para CImagem3D
		typedef CImagem3D<T> *IMAGE3D;
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CImagem3D& obj);
// istream& operator>> (istream& is, CImagem3D& obj);

#include <Matriz/CImagem3D.cpp>

#endif
