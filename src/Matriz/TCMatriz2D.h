#ifndef TCMatriz2D_H
#define TCMatriz2D_H
/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
						Assunto/Ramo: TCMatriz2D...
===============================================================================
Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico - [LDSC].
@author     André Duarte Bueno
@file       TCMatriz2D.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
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
 * PS: Observe que com as funções AlocaMatriz2D, pode-se alocar uma estrutura de dados 2D
 * diretamente, sem criar um objeto matriz.
 * Ex: 	int **m2D = TCMatriz2D::AlocaMatriz2D(nx,ny);
 * @author 	André Duarte Bueno
 * @see		Veja assunto
*/
template< typename T >
class TCMatriz2D : public CBaseMatriz
{
		// --------------------------------------------------------------Atributos
	protected:
		int ny;   	/// Dimensão ny
		int nx;   	/// Dimensão nx

	public:
		T **data2D; /// Ponteiro para matriz 2D
		// -------------------------------------------------------------Construtor
		/// Construtor default, data2D=NULL nx=ny=0;
		TCMatriz2D ();

		/// Construtor le TCMatriz2D do disco.
		TCMatriz2D (std::string fileName);

		/// Construtor le arquivo RAW do disco. Recebe nome do arquivo, largura, altura e tipo (P4_X_Y_BINARY (default), P5_X_Y_GRAY_BINARY ou P6_X_Y_COLOR_BINARY) da imagem.
		TCMatriz2D (std::string fileRAW, int _nx, int _ny, EImageType tipo=P4_X_Y_BINARY );

		/// Construtor le plano de uma matriz 3D o primeiro plano é o z=0
		TCMatriz2D (std::string fileName, int planoZ);

		/// Construtor cria copia TCMatriz2D
		TCMatriz2D (TCMatriz2D < T > &);

		/// Construtor cria matriz extendida, com borda extra. Copia matriz na parte central
		// util pois cria uma borda extra, zerada.
		TCMatriz2D ( TCMatriz2D <T> & matriz, unsigned int borda );

		/// Construtor cria nova TCMatriz2D dados=lixo, para zerar Constante(0);
		TCMatriz2D (int _nx, int _ny);

		// --------------------------------------------------------------Destrutor

		/// Destrutor, chama Desaloca
		virtual ~ TCMatriz2D () {
			TCMatriz2D<T>::DesalocaMatriz2D (data2D, nx, ny);
		}

		// ----------------------------------------------------------------Métodos
	protected:
		/// Alocacao
		virtual bool Aloca () {
			data2D = TCMatriz2D<T>::AlocaMatriz2D (nx, ny);
			return data2D ? 1 : 0;
		}

		/// Desalocacao
		virtual bool Desaloca () {
			TCMatriz2D<T>::DesalocaMatriz2D (data2D, nx, ny);
			return data2D ? 0 : 1;
		}

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
		virtual bool Redimensiona (int NX, int NY = 0, int NZ = 0);

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

		virtual int DimensaoMatriz () const {
			return 2;
		}

		/// Troca todos os valores i por j no vetor, retorna o numero de elementos trocados
		virtual int Replace (int i, int j);

		bool ChecaIndice (int NX, int NY) const {
			return (NX >= 0 && NX < nx && NY >= 0 && NY < ny) ? 1 : 0;
		}

		/// Lê arquivo do tipo PNM (PBM, PGM, PPM). Caso seja arquivo PBM sem espaço entre os valores, o segundo parâmetro deverá ser 0 (zero).
		bool Read (std::string fileName, int separado = 1); // Como é chamada pelo construtor nao pode ser virtual

		/// Lê arquivo binário do tipo RAW. Recebe o nome do arquivo e o tipo (P4_X_Y_BINARY (default), P5_X_Y_GRAY_BINARY ou P6_X_Y_COLOR_BINARY).
		bool ReadRAW (std::string fileName, int _nx, int _ny, EImageType tipo = P4_X_Y_BINARY); // Como é chamada pelo construtor nao pode ser virtual

		// Novidade trazida para cá de COperacao
		/// Lê imagem 2D do disco, usa vetor de dados
		//  static bool Read2D (std::string inputFile, BUG float * _reDdata, int _nx,  int _ny);
		static bool Read2D (std::string inputFile, float * &_reData, int _nx,  int _ny);

		/// Salva imagem 2D no disco, usa vetor de dados
		static bool Write2D (std::string inputFile,  float *_redata, int _nx,  int _ny);

		/// rotaciona a imagem 90 graus a direita
		bool Rotacionar90 ();

		// -----------------------------------------------------------------------Sobrecarga de operador
		/// Sobrecarga +
		TCMatriz2D< T > & operator+ (TCMatriz2D< T > & pm2);

		/// Sobrecarga -
		TCMatriz2D< T > & operator- (TCMatriz2D< T > & pm2);

		/// Sobrecarga =
		TCMatriz2D< T > & operator= (TCMatriz2D< T > & pm2);

		// TCMatriz2D* operator*(TCMatriz2D*& m2);
		// Sobrecarga *

		/// Sobrecarga ==
		bool operator== (TCMatriz2D< T > & pm2);

		/// Sobrecarga !=
		bool operator!= (TCMatriz2D< T > & pm2);

		/// Aceita matriz(x,y)
		inline T &operator  () (int x, int y) const {
			return data2D[x][y];
		}

		/// Poderia criar um SetColuna[y], ai teria data2D[x][coluna]
		inline T &operator[] (int x) const {
			return data2D[x][0];
		}

		// --------------------------------------------------------------------Get
		/// Retorna nx
		inline int NX () const {
			return nx;
		}

		/// Retorna ny
		inline int NY () const {
			return ny;
		}

		/// Retorna data2D
		inline T **Data2D () const {
			return data2D;
		}

		// --------------------------------------------------------------------Set
		/// Define nx
		inline void NX (int NX) {
			nx = NX;
		}

		/// Define ny
		inline void NY (int NY) {
			ny = NY;
		}

		// -----------------------------------------------------------------Friend
		/// Sobrecarga operador<<.
		//friend ostream &operator<<( ostream &, const TCMatriz2D<T> & );

		/// Sobrecarga operador>>.
		//friend istream &operator>>( istream &, TCMatriz2D<T> & );
};
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/* Especialização da Classe tamplate para o tipo bool */
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
#include <boost/dynamic_bitset.hpp>
#include <vector>

using namespace boost;

template<>
class TCMatriz2D<bool> : public CBaseMatriz
{
		// --------------------------------------------------------------Atributos
	protected:
		int ny;   	/// Dimensão ny
		int nx;   	/// Dimensão nx

	public:
		/// Matriz 2D com dados do tipo dynamic_bitset
		vector< dynamic_bitset<> > data2D;
		// -------------------------------------------------------------Construtor
		/// Construtor default, data2D=NULL nx=ny=0;
		TCMatriz2D ();

		/// Construtor le TCMatriz2D do disco.
		TCMatriz2D (std::string fileName);

		/// Construtor le arquivo RAW do disco. Recebe nome do arquivo, largura, altura e tipo (P4_X_Y_BINARY (default), P5_X_Y_GRAY_BINARY ou P6_X_Y_COLOR_BINARY) da imagem.
		TCMatriz2D (std::string fileRAW, int _nx, int _ny, EImageType tipo=P4_X_Y_BINARY );

		/// Construtor le plano de uma matriz 3D o primeiro plano é o z=0
		TCMatriz2D (std::string fileName, int planoZ);

		/// Construtor cria copia TCMatriz2D
		TCMatriz2D (TCMatriz2D<bool> &);

		/// Construtor cria matriz extendida, com borda extra. Copia matriz na parte central
		// util pois cria uma borda extra, zerada.
		TCMatriz2D ( TCMatriz2D<bool> & matriz, unsigned int borda );

		/// Construtor cria nova TCMatriz2D dados=lixo, para zerar Constante(0);
		TCMatriz2D (int _nx, int _ny);

		// --------------------------------------------------------------Destrutor

		/// Destrutor, chama Desaloca
		virtual ~ TCMatriz2D () {
			TCMatriz2D<bool>::DesalocaMatriz2D (data2D, nx, ny);
		}

		// ----------------------------------------------------------------Métodos
	protected:
		/// Alocacao
		virtual bool Aloca () {
			data2D = TCMatriz2D<bool>::AlocaMatriz2D (nx, ny);
			return data2D.empty();
		}

		/// Desalocacao
		virtual bool Desaloca () {
			TCMatriz2D<bool>::DesalocaMatriz2D (data2D, nx, ny);
			return data2D.empty();
		}

		/// Le os dados separados por " "
		void LeDados (ifstream & fin);

		/// Le os dados, colados 010111001
		void LeDadosColados (ifstream & fin);

		/// Le os dados gravados em formato binario
		void LeDadosBinarios (ifstream & fin);

	public:
		/// Salva dados do cabecalho
		virtual void SalvaCabecalho (ofstream & fout) const;

		/// Salva dados em formato binario
		virtual void SalvaDadosBinarios (ofstream & fout) const;

		/// Salva dados "colados" sem espaço
		virtual void SalvaDadosColados (ofstream & fout) const;

		/// Salva dados com um espaco " "
		virtual void SalvaDados (ofstream & fout) const;

		/// lê o plano z, de uma matriz 3D
		bool LePlanoZ (string fileName, int planoZ, bool separado = true);

		/// Redimensiona a matriz
		virtual bool Redimensiona (int NX, int NY = 0, int NZ = 0);

		/// Aloca uma matriz de dados 2D qualquer.
		static vector< dynamic_bitset<> > & AlocaMatriz2D(int nx, int ny);

		/// Desaloca dat
		static bool DesalocaMatriz2D (vector< dynamic_bitset<> > &dat, int nx, int ny);

		/// Preenche com valor constante
		virtual void Constante (bool cte);

		/// Inverte valores (0)-->(1)  (>0)-->(0)
		virtual void Inverter ();

		/// Retorna para os as propriedades da matriz
		virtual void Propriedades (ofstream & os) const;
		// virtual void        Propriedades(std::ostream& os)const;

		virtual int DimensaoMatriz () const {
			return 2;
		}

		bool ChecaIndice (int NX, int NY) const {
			return (NX >= 0 && NX < nx && NY >= 0 && NY < ny) ? 1 : 0;
		}

		/// Lê arquivo do tipo PNM (PBM, PGM, PPM). Caso seja arquivo PBM sem espaço entre os valores, o segundo parâmetro deverá ser 0 (zero).
		bool Read (string fileName, int separado = 1); // Como é chamada pelo construtor nao pode ser virtual

		/// Lê arquivo binário do tipo RAW. Recebe o nome do arquivo e o tipo (P4_X_Y_BINARY (default), P5_X_Y_GRAY_BINARY ou P6_X_Y_COLOR_BINARY).
		bool ReadRAW (string fileName, int _nx, int _ny, EImageType tipo = P4_X_Y_BINARY); // Como é chamada pelo construtor nao pode ser virtual

		// Novidade trazida para cá de COperacao
		/// Lê imagem 2D do disco, usa vetor de dados
		//  static bool Read2D (std::string inputFile, BUG float * _reDdata, int _nx,  int _ny);
		static bool Read2D (string inputFile, bool * &_reData, int _nx,  int _ny);

		/// Salva imagem 2D no disco, usa vetor de dados
		static bool Write2D (string inputFile,  bool *_redata, int _nx,  int _ny);

		/// rotaciona a imagem 90 graus a direita
		bool Rotacionar90 ();

		// -----------------------------------------------------------------------Sobrecarga de operador
		/// Sobrecarga operador +
		TCMatriz2D<bool> & operator+ (TCMatriz2D<bool> & pm2);

		/// Sobrecarga operador -
		TCMatriz2D<bool> & operator- (TCMatriz2D<bool> & pm2);

		/// Sobrecarga operador =
		TCMatriz2D<bool> & operator= (TCMatriz2D<bool> & pm2);

		// TCMatriz2D* operator*(TCMatriz2D*& m2);
		// Sobrecarga *

		/// Sobrecarga operador ==
		bool operator== (TCMatriz2D<bool> & pm2);

		/// Sobrecarga operador !=
		bool operator!= (TCMatriz2D<bool> & pm2);

		/// Sobrecarga operador (). Aceita matriz(x,y)
		inline bool operator  () (int x, int y) const {
			return data2D[x][y];
		}

		// --------------------------------------------------------------------Get
		/// Retorna nx
		inline int NX () const {
			return nx;
		}

		/// Retorna ny
		inline int NY () const {
			return ny;
		}

		/// Retorna data2D
		inline vector< dynamic_bitset<> > Data2D () const {
			return data2D;
		}

		// --------------------------------------------------------------------Set
		/// Define nx
		inline void NX (int NX) {
			nx = NX;
		}

		/// Define ny
		inline void NY (int NY) {
			ny = NY;
		}

		// -----------------------------------------------------------------Friend
		/// Sobrecarga operador<<.
		//friend ostream &operator<<( ostream &, const TCMatriz2D<T> & );

		/// Sobrecarga operador>>.
		//friend istream &operator>>( istream &, TCMatriz2D<T> & );
};

#include <Matriz/TCMatriz2D.cpp>

#endif
