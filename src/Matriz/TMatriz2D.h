#ifndef TMatriz2D_h
#define TMatriz2D_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
						Assunto/Ramo: TMatriz2D...
===============================================================================
Desenvolvido por:
						Laboratorio de Desenvolvimento de Software Cientifico
						[LDSC].
@author     André Duarte Bueno
@file       TMatriz2D.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno

@email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <iostream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CBaseMatriz_h
#include <Matriz/CBaseMatriz.h>
#endif
using namespace std;
/**
 * @brief	Representa uma matriz bidimensional.
 * Permite criar e usar uma matriz bidimensional.
 * A alocacao e feita dinamicamente
 * Tem tres construtores:
 * 1 - Para criar uma matriz toda nova use
 * TMatriz2D(NX,NY),cria matriz, aloca data2D, zera valores
 * 2 - Para criar uma copia no programa use
 * TMatriz2D(TMatriz2D), cria uma copia
 * TMatriz2D=TMatriz2D;
 * 3 - Para abrir uma matriz do disco use
 * TMatriz2D("nomearquivo")
 * TMatriz2D.Read("nomearquivo")
 * Para salvar a matriz em disco use
 * TMatriz2D.Write(std::string fileName)
 * Para acessar o conteudo da matriz use data2D
 * TMatriz2D.data2D[i][j]=33;
 * int x=TMatriz2D.data2D[i][j];
 *
 * PS: Observe que com as funções AlocaMatriz2D, pode-se alocar uma estrutura de dados 2D
 * diretamente, sem criar um objeto matriz.
 * Ex: 	int **m2D = TMatriz2D::AlocaMatriz2D(nx,ny);
 * @author 	André Duarte Bueno
 * @see		Veja assunto
*/
template< typename T >
class TMatriz2D : public CBaseMatriz
{
		// --------------------------------------------------------------Atributos
	protected:
		int ny;   	/// Dimensão ny
		int nx;   	/// Dimensão nx

	public:
		T **data2D; /// Ponteiro para matriz 2D de inteiros
		// -------------------------------------------------------------Construtor
		/// Construtor default, data2D=NULL nx=ny=0;
		TMatriz2D ();

		/// Construtor le TMatriz2D do disco.
		TMatriz2D (std::string fileName);

		/// Construtor le arquivo RAW do disco. Recebe nome do arquivo, largura, altura e tipo (P4_X_Y_BINARY (default), P5_X_Y_GRAY_BINARY ou P6_X_Y_COLOR_BINARY) da imagem.
		TMatriz2D (std::string fileRAW, int _nx, int _ny, EImageType tipo=P4_X_Y_BINARY );

		/// Construtor le plano de uma matriz 3D o primeiro plano é o z=0
		TMatriz2D (std::string fileName, int planoZ);

		/// Construtor cria copia TMatriz2D
		TMatriz2D (TMatriz2D &);

		/// Construtor cria matriz extendida, com borda extra. Copia matriz na parte central
		// util pois cria uma borda extra, zerada.
		TMatriz2D ( TMatriz2D & matriz, unsigned int borda );

		/// Construtor cria nova TMatriz2D dados=lixo, para zerar Constante(0);
		TMatriz2D (int _nx, int _ny);

		// --------------------------------------------------------------Destrutor

		/// Destrutor, chama Desaloca
		virtual ~ TMatriz2D ()
		{
			TMatriz2D::DesalocaMatriz2D (data2D, nx, ny);
		}

		// ----------------------------------------------------------------Métodos
	protected:
		/// Alocacao
		virtual bool Aloca ()
		{
			data2D = TMatriz2D::AlocaMatriz2D (nx, ny);
			return data2D ? 1 : 0;
		}

		/// Desalocacao
		virtual bool Desaloca ()
		{
			TMatriz2D::DesalocaMatriz2D (data2D, nx, ny);
			return data2D ? 0 : 1;
		}

		// As funções abaixo devem ser movidas para a classe CImagem2D
		// COLOQUEI APENAS ENQUANTO NAO TROCO NO ANAIMP CBaseMatriz POR TIMAGEM
		/// Le os dados separados por " "
		void LeDados (std::ifstream & fin);

		/// Le os dados, colados 010111001
		void LeDadosColados (std::ifstream & fin);

		/// Le os dados gravados em formato binario
		void LeDadosBinarios (std::ifstream & fin);

	public:

		/// Salva dados do cabecalho
		virtual void SalvaCabecalho (std::ofstream & fout) const;

		/// Salva dados em formato binario
		virtual void SalvaDadosBinarios (std::ofstream & fout) const;

		/// Salva dados "colados" sem espaço
		virtual void SalvaDadosColados (std::ofstream & fout) const;

		/// Salva dados com um espaco " "
		virtual void SalvaDados (std::ofstream & fout) const;

		/// lê o plano z, de uma matriz 3D
		bool LePlanoZ (std::string fileName, int planoZ, bool separado = true);

		/// Redimensiona a matriz
		virtual bool Redimensiona (int NX, int NY = 0, int NZ = 0)
		{
			if( nx != NX || ny != NY ) {
				TMatriz2D::DesalocaMatriz2D (data2D, nx, ny);
				nx = NX;
				ny = NY;
				NZ = 0; // evitar warning
				data2D = TMatriz2D::AlocaMatriz2D (nx, ny);
				return data2D ? 1 : 0;
			}
			return true; // não precisou redimensionar...
		}

		/// Aloca uma matriz de dados 2D qualquer.
		static T** AlocaMatriz2D(int nx, int ny);

		/// Desaloca dat
		static bool DesalocaMatriz2D (T **&dat, int nx, int ny);

		/// Preenche com valor constante
		virtual void Constante (T cte);

		/// Inverte valores (0)-->(1)  (>0)-->(0)
		virtual void Inverter ();

		/// Retorna para os as propriedades da matriz
		virtual void Propriedades (std::ofstream & os) const;
		// virtual void        Propriedades(std::ostream& os)const;

		/// Retorna o maior valor da matriz
		virtual int MaiorValor() const;

		/// Retorna o menor valor da matriz
		virtual int MenorValor() const;

		/// Retorna o menor valor da matriz (diferente de zero). Se a matriz só tiver zeros, irá retornar 9999999999
		virtual T MenorValorNzero() const;

		/// Retorna o um par correspondente ao maior e ao menor valor da matriz (diferente de zero). Se a matriz só tiver zeros, irá retornar 0 e 9999999999
		virtual pair<T, T> MaiorMenorValorNzero() const;

		/// Calcula e retorna a média
		virtual double Media () const;

		virtual int DimensaoMatriz () const
		{
			return 2;
		}

		/// Troca todos os valores i por j no vetor, retorna o numero de elementos trocados
		virtual int Replace (int i, int j);

		bool ChecaIndice (int NX, int NY) const
		{
			return (NX >= 0 && NX < nx && NY >= 0 && NY < ny) ? 1 : 0;
		}

		/// Lê arquivo do tipo PNM (PBM, PGM, PPM). Caso seja arquivo PBM sem espaço entre os valores, o segundo parâmetro deverá ser 0 (zero).
		bool Read (std::string fileName, int separado = 1); // Como é chamada pelo construtor nao pode ser virtual

		/// Lê arquivo binário do tipo RAW. Recebe o nome do arquivo e o tipo (P4_X_Y_BINARY (default), P5_X_Y_GRAY_BINARY ou P6_X_Y_COLOR_BINARY).
		bool ReadRAW (std::string fileName, int _nx, int _ny, EImageType tipo = P4_X_Y_BINARY); // Como é chamada pelo construtor nao pode ser virtual

		// Novidade trazida para cá de COperacao
		/// Lê imagem 2D do disco, usa vetor de dados
		//  static bool Read2D (std::string inputFile, BUG float * _reDdata, int _nx,  int _ny);
		static bool Read2D (std::string inputFile, T * &_reData, int _nx,  int _ny);

		/// Salva imagem 2D no disco, usa vetor de dados
		static bool Write2D (std::string inputFile,  T *_redata, int _nx,  int _ny);

		/// rotaciona a imagem 90 graus a direita
		bool Rotacionar90 ();

		// -----------------------------------------------------------------------Sobrecarga de operador
		/// Sobrecarga +
		TMatriz2D< T > & operator+ (TMatriz2D< T > & pm2);

		/// Sobrecarga -
		TMatriz2D< T > & operator- (TMatriz2D< T > & pm2);

		/// Sobrecarga =
		TMatriz2D< T > & operator= (TMatriz2D< T > & pm2);

		// TMatriz2D* operator*(TMatriz2D*& m2);
		// Sobrecarga *

		/// Sobrecarga ==
		bool operator== (TMatriz2D< T > & pm2);

		/// Sobrecarga !=
		bool operator!= (TMatriz2D< T > & pm2);

		/// Aceita matriz(x,y)
		inline T &operator  () (int x, int y) const
		{
			return data2D[x][y];
		}

		/// Poderia criar um SetColuna[y], ai teria data2D[x][coluna]
		inline T &operator[] (int x) const
		{
			return data2D[x][0];
		}

		// --------------------------------------------------------------------Get
		/// Retorna nx
		inline int NX () const
		{
			return nx;
		}

		/// Retorna ny
		inline int NY () const
		{
			return ny;
		}

		/// Retorna data2D
		inline T **Data2D () const
		{
			return data2D;
		}

		// --------------------------------------------------------------------Set
		/// Define nx
		inline void NX (int NX)
		{
			nx = NX;
		}

		/// Define ny
		inline void NY (int NY)
		{
			ny = NY;
		}

		// -----------------------------------------------------------------Friend
		/// Sobrecarga operador<<.
		friend ostream& operator<< (ostream& os, const TMatriz2D<T> & pm);

		/// Sobrecarga operador>>.
		friend istream& operator>> (istream& is, TMatriz2D<T> & pm);
};
#endif
