#ifndef CBaseMatriz_h
#define CBaseMatriz_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
						Assunto/Ramo: CMatriz
===============================================================================
Desenvolvido por:	
						Laboratorio de Desenvolvimento de Software Cientifico
						[LDSC].
@author     André Duarte Bueno
@file       CBaseMatriz.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>
#include <string>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

/**
 * @brief EImageType enumera os tipos de imagens aceitas.
 * Quando o arquivo é lido/salvo em disco o mesmo obedece um
 * determinado formato, definido pelo parâmetro formatoImagem
 * que pode assumir um dos valores abaixo
 **/
enum EImageType {
	INVALID_IMAGE_TYPE,		// 0 - FORMATO INVÁLIDO
	// Matriz 1D
	V1_X_ASCII,						// 1 -VBM 1D ASCII
	V2_X_GRAY_ASCII,			// 2 -VGM 1D ASCII
	V3_X_COLOR_ASCII,			// 3 -VPM 1D ASCII
	V4_X_BINARY,					// 4 -VBM 1D BINARY
	V5_X_GRAY_BINARY,			// 5 -VGM 1D BINARY
	V6_X_COLOR_BINARY,		// 6 -VPM 1D BINARY
	// Matriz 2D
	P1_X_Y_ASCII,					// 7 - PBM 2D ASCII
	P2_X_Y_GRAY_ASCII,		// 8 - PGM 2D ASCII
	P3_X_Y_COLOR_ASCII,		// 9 - PPM 2D ASCII
	P4_X_Y_BINARY,				// 10 - PBM 2D BINARY
	P5_X_Y_GRAY_BINARY,		// 11 - PGM 2D BINARY
	P6_X_Y_COLOR_BINARY,	// 12 - PPM 2D BINARY
	// Matriz 3D
	D1_X_Y_Z_ASCII,				// 13 - DBM 3D ASCII
	D2_X_Y_Z_GRAY_ASCII,	// 14 - DGM 3D ASCII
	D3_X_Y_Z_COLOR_ASCII,	// 15 - DPM 3D ASCII
	D4_X_Y_Z_BINARY,			// 16 -DBM 3D BINARY
	D5_X_Y_Z_GRAY_BINARY,	// 17 -DGM 3D BINARY
	D6_X_Y_Z_COLOR_BINARY	// 18 -DPM 3D BINARY
};

const char CR = 13;
const char LF = 10;

//struct sbit0 {
//		bool b : 1;
//};
//struct sbit1 {
//		bool : 1;
//		bool b : 1;
//};
//struct sbit2 {
//		bool : 2;
//		bool b : 1;
//};
//struct sbit3 {
//		bool : 3;
//		bool b : 1;
//};
//struct sbit4 {
//		bool : 4;
//		bool b : 1;
//};
//struct sbit5 {
//		bool : 5;
//		bool b : 1;
//};
//struct sbit6 {
//		bool : 6;
//		bool b : 1;
//};
//struct sbit7 {
//		bool : 7;
//		bool b : 1;
//};
//struct sbyte {
//		sbit0 b0;
//		sbit1 b1;
//		sbit2 b2;
//		sbit3 b3;
//		sbit4 b4;
//		sbit5 b5;
//		sbit6 b6;
//		sbit7 b7;
//		bool operator[](int n) { // sbyte[i]
//			switch ( n ) {
//				case 0: return  b0.b; break;
//				case 1: return  b1.b; break;
//				case 2: return  b2.b; break;
//				case 3: return  b3.b; break;
//				case 4: return  b4.b; break;
//				case 5: return  b5.b; break;
//				case 6: return  b6.b; break;
//				case 7: return  b7.b; break;
//			}
//		}
//};

using namespace std;

// ===============================================================================
// Documentacao CLASSE: CBaseMatriz
// ===============================================================================
/**
 * @brief Classe base para as classes CVetor, TCMatriz2D< int > e CMatriz3D.
 *   Inclue as definições em comum.
 *
 * Obs:
 * A funcao virtual void LeCabecalho(ifstream& fin)=0;
 * Será herdada e  serve para ler o cabecalho do arquivo de disco.
 * Éla chama a funcao LeEfetivamenteCabecalho desta classe.
 * Observe que é uma especie de truque.
 * A funcao desta classe, LeEfetivamenteCabecalho  precisa de alguns parametros
 * que esta classe não tem, mas que as classes herdeiras terão.
 * Entao a funcao Read chama LeCabecalho (virtual e redefinida nas classes filhas)
 * que chama LeEfetivamenteCabecalho passando os atributos corretos como referência.
 * @author 	André Duarte Bueno
 * @see		Matriz
*/
class CBaseMatriz
{
		// --------------------------------------------------------------Atributos
	protected:
		/**
	 * @brief Define como os dados serão salvos.
	 * Quando realiza leitura do arquivo, verifica o formato.
	*/
		EImageType formatoImagem;

		/**
	 * @brief Define a faixa de tons de cinza ou de cores.
	 * Quando realiza leitura do arquivo, verifica a faixa de tons.
	*/
		unsigned int numCores;

	public:
		/**
	* @brief largura do campo de salvamento dos dados em disco (default=2).
	* Observe que se for um número como 255 precisa de uma largura = 4
	*/
		static int larguraCampo;

		/**
	* @brief Ao salvar em disco no formato ASCII com dados separados,
	* insere o número de espaços necessários para que os dados fiquem alinhados.
	*	Útil na visualização do código do arquivo (default = false).
	*/
		bool salvarAlinhado;


		/**
	* @brief Path de salvamento dos dados em disco (default = ./).
	*/
		string path;

		// -------------------------------------------------------------Construtor
		/// Construtor, novo default WRITEFORM_ASCII_PI_N_GRAY_DATA
		CBaseMatriz (EImageType _formatoImagem = P1_X_Y_ASCII, string _path="./")
			: formatoImagem (_formatoImagem), salvarAlinhado(false) , path(_path)
		{
		}
		// --------------------------------------------------------------Destrutor
		/// Destrutor
		virtual ~ CBaseMatriz ()
		{
		}

		// ----------------------------------------------------------------Métodos
	protected:
		// Funções de uso interno
		/// Retorna menor valor
		inline int Menor (int a, int b) const
		{
			return (a < b) ? a : b;
		}

		/// Retorna maior valor
		inline int Maior (int a, int b) const
		{
			return (a < b) ? b : a;
		}

	public:
		/**
	 * @brief 	Salva dados do cabecalho.
	 * Para salvar o arquivo em disco, primeiro salva o cabeçalho, depois os dados.
	 */
		virtual void SalvaCabecalho (std::ofstream & fout) const = 0;

		/**
	* @brief	Salva dados em formato binario
	*/
		virtual void SalvaDadosBinarios (std::ofstream & fout) const = 0;

		/**
	* @brief	Salva dados "colados" sem espaço 01100010101.
	*/
		virtual void SalvaDadosColados (std::ofstream & fout) const = 0;

		/**
	* @brief	Salva dados com um espaco " " 0 1 0 1 0 0 1
	*/
		virtual void SalvaDados (std::ofstream & fout) const = 0;

		/**
		* @brief	Lê os comentários do arquivo para avançar o ponteiro de leitura
		*/
		void LeComentarios( std::ifstream & fin);


		// Operacoes com arquivos
		/**
	 * @brief A funcao Write salva a matriz no disco.
	 * Chama algumas funções virtuais definidas nas classes descendentes.
	*/
		bool Write (std::string nomeArquivo, int dadosSeparados = 1) const;

		/**	@brief Grava em arquivo o número de cores da imagem. Deve ser chamada logo após SalvaCabecalho */
		bool SalvaCores (std::ofstream & fout) const;

		/**
	 *  @brief Cada classe herdeira tem uma funcao Read que lê o arquivo de disco.
	 * Esta funcao não é virtual porque é chamada pelo construtor.
	 * A funcao Read chama as duas funcoes abaixo
	*/

		/**
		@brief Abre o arquivo verifica o formato e reabre no formato
		correto (ASCII ou Binário)
		*/
		bool AbreArquivo (std::ifstream & fin, std::string nomeArquivo);

		/**
		@brief Verifica e retorna o formato de salvamento do arquivo. Recebe objeto do tipo std::ifstream
		*/
		EImageType VerificaFormato (std::ifstream & fin);

		/**
		@brief Verifica e retorna o formato de salvamento do arquivo. Recebe o nome do arquivo.
		*/
		EImageType VerificaFormato (std::string & filename);

		/**
		@brief Retona o valor da variável de formato de salvamente (e leitura) de dados
		*/
		inline EImageType GetFormato () const { return formatoImagem; }

		/**
		@brief Define a variável que informa o número de tons de cores ou de cinza
		*/
		void NumCores ( int _cores )
		{
			numCores = _cores;
		}

		/**
		@brief Define o valor do path de salvamento da matriz
		*/
		void Path ( string _path )
		{
			path = _path;
		}
		/**
		@brief Retorna o valor do path de salvamento da matriz
		*/
		string Path ( )
		{
			return path;
		}
		// -----------------------------------------------------------------------
		// Metodos herdados virtuais
		// -----------------------------------------------------------------------
		// Alocacao, Desalocacao
		/**
		@brief Define a variável de formato de salvamente (e leitura) de dados
		*/
		virtual void SetFormato (EImageType _formato)= 0;

		/**
		@brief Funcoes virtuais de alocacao.
		*/
		virtual bool Aloca () = 0;

		/**
		@brief Desalocacao dos dados
		*/
		virtual bool Desaloca () = 0;

		/**
		@brief Redimensiona a matriz ,virtual pura
		*/
		virtual bool Redimensiona (int NX, int NY = 0, int NZ = 0) = 0;

		/**
		@brief Inverte valores (0)-->(1)  (>0) -->0
		*/
		virtual void Inverter () = 0;

		/**
		@brief Calcula e retorna a média
		*/
		virtual double Media () const = 0;

		// --------------------------------------------------------------------Get
		/**
		@brief Retorna o número de tons de cores ou de cinza)
		*/
		inline int NumCores ( ) const { return numCores; }

		/**
		@brief Retorna a dimensão da matriz
		*/
		virtual int DimensaoMatriz () const = 0;

		// --------------------------------------------------------------------Set
		// -----------------------------------------------------------------Friend
		// friend ostream& operator<< (ostream& os, CBaseMatriz& obj);
		// friend istream& operator>> (istream& is, CBaseMatriz& obj);
};

// ostream& operator<< (ostream& os, CBaseMatriz& obj);
// istream& operator>> (istream& is, CBaseMatriz& obj);

#endif
