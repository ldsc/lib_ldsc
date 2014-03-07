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
@email      andreduartebueno@gmail.com
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

#ifndef TCMatriz2D_H
#include <AnaliseImagem/Matriz/TCMatriz2D.h>
#endif
/* // comentei para poder fazer dynamic_cast
#ifndef CImg_h
#include <AnaliseImagem/Matriz/CImg.h>
#endif
*/
/**
 *
 * @brief	Representra uma imagem bidimensional.
 *
 * É uma matriz bidimensional, herdeira da TCMatriz2D< int >
 * Acrescenta os atributos x0 e y0 que representam
 * as coordenadas centrais na matriz de dados.
 * Acrescenta uma função contem que verifica se o ponto passado
 * pertence a imagem.
 * @author 	André Duarte Bueno
 * @see		Matrizes
*/
class CImagem : public TCMatriz2D< int >//, public CImg // comentei para poder fazer dynamic_cast
{
		// --------------------------------------------------------------Atributos
	public:
		int x0, y0;
		// -------------------------------------------------------------Construtor
		/// Constrói uma imagem vazia, não alocada.
		CImagem ()
			:	TCMatriz2D< int >(), x0(0), y0(0) {
		}

		/// Constrói a imagem a partir de arquivo de disco
		CImagem (std::string nomeArquivo)
			: TCMatriz2D< int > (nomeArquivo), x0(0), y0(0) {
		}

		/// Constrói a imagem bidimensional a partir de plano de imagem tridimensional
		CImagem (std::string nomeArquivo, int planoZ)
			: TCMatriz2D< int > (nomeArquivo, planoZ), x0(0), y0(0) {
		}

		/// Construtor le arquivo RAW do disco. Recebe nome do arquivo, largura, altura, profundidade e tipo (D4_X_Y_Z_BINARY (default), D5_X_Y_Z_GRAY_BINARY ou D6_X_Y_Z_COLOR_BINARY) da imagem.
		CImagem (std::string fileRAW, int _nx, int _ny, EImageType tipo=P4_X_Y_BINARY )
			:TCMatriz2D<int>(fileRAW, _nx, _ny, tipo), x0(0), y0(0) {
		}

		/// Construtor de cópia
		CImagem (CImagem & img)
			: TCMatriz2D< int > (img), x0(0), y0(0) {
		}

		/// Constrói imagem nova, com as dimensões passadas
		CImagem (int nx, int ny)
			: TCMatriz2D< int > (nx, ny), x0(0), y0(0) {
		}

		/// Constrói uma imagem a partir de uma matriz2D, faz x0=y0=0
		CImagem (TCMatriz2D< int > & matriz)
			: TCMatriz2D< int > (matriz), x0(0), y0(0) {
		}

		// --------------------------------------------------------------Destrutor
		/// Destrutor
		virtual ~ CImagem () {
		}

		// ----------------------------------------------------------------Métodos
	private:
		// Mover para CImagem
		/// Verifica o formato
		void VerificaFormato (std::ifstream & fin);

	public:

		/// Verifica se a imagem contem o ponto, o mesmo que checaIndice
		inline bool Contem (int i, int j) const {
			return (i < nx && j < ny) ? 1 : 0;
		}

		/// Inverte valores 0-1
		void Inverter ();

		/// Faz a interseção entre this e pm2
		TCMatriz2D< int > *Intersecao (TCMatriz2D< int > * pm2);

		/// Faz a união entre this e pm2
		TCMatriz2D< int > *Uniao (TCMatriz2D< int > * pm2);

		// Compara this e pm2
		// TCMatriz2D< int >* Igual(TCMatriz2D< int >*pm2);

		// -----------------------------------------------------------------Friend
		//       friend ostream& operator<< (ostream& os, CImagem& obj);
		//       friend istream& operator>> (istream& is, CImagem& obj);

		// --------------------------------------------------------------------Get
		/// Retorna o valor de x0
		inline int X0 () const {
			return x0;
		}

		/// Retorna o valor de y0
		inline int Y0 () const {
			return y0;
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
// esta classe vai ser templaetizada e a mesma será eliminada
float **f2d (int nr, int nc);
#endif
