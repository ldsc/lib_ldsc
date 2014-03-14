#ifndef CPermeabilidadeRede_h
#define CPermeabilidadeRede_h
// Bibliotecas C/C++
#include <iostream>
#include <string>

// Bibliotecas LIB_LDSC
//#include <Base/_LIB_LDSC_CLASS.h>
#include <AnaliseImagem/Caracterizacao/RedePercolacao/CRedeContorno.h>
#include <AnaliseImagem/Simulacao/Permeabilidade/CPermeabilidade.h>
#include <MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDiagonalDominante.h>
#include <Amostra/Material/CMFluido.h>
#include <MetNum/Contorno/CContorno.h>

/**
 * @file		CPermeabilidadeRede.h
 * @author	Leandro Puerari <puerari@gmail.com>
 * @author	André Duarte Bueno <andreduartebueno@gmail.com>
 * @brief		Classe usada para determinar a permeabilidade de uma imagem tridimensional.
 * Recebe como parâmetros:
 *
 * 1-Criam-se os objetos agregados:
 * rede				- representa uma estrutura de sítios e ligações.
 * solver			- representa um solucionador de matrizes (resolve sistemas de equações)
 * fluido			- representa um fluido com viscosidade, compressibilidade, densidade
 *
 * 2-Cria-se um objeto de calculo da permeabilidade CPermeabilidadeRede,
 * passando-se como parametros os objetos agregados.
 *
 * 3-O objeto rede tem objetos do tipo CObjetoImagem (ex: Sitios e Ligações) que
 * tem uma propriedade(condutância) e x (a pressão).
 *
 * 4-Criado o objeto CPermeabilidadeRede, pode-se chamar a função
 * de solução do sistema de equações (SolucaoSistema());
 *
 * Resolve o sistema como um todo,
 * 4.1)	virtual void CriacaoObjetosAgregados () ;// Cria objetos agregados
 * 4.2)	virtual void DefinicaoCondicoesContorno () ;// Define as condições de contorno
 * 4.2.1)  void AuxCalculoCondutancias(); // Chama funcao de calculo das condutancias
 * 4.3)	virtual void DefinicaoValoresIniciais() ;// Define valores iniciais
 * 4.4)  virtual double Go () ;	// Calcula a permeabilidade (e os fluxos)
 * 4.4.1)  double AuxFluxoFronteira(int);// calcula os fluxos
 *
 * 10 -A solução do sistema de equações é realizada por um objeto TSMSOR (um solver).
 * Que é um solver genérico, que recebe um vetor de dados que se relacionam e determina
 * os valores deste vetor para um práximo passo de tempo.
 *
 * O problema a ser resolvido é a determinação das pressões de equilíbrio em cada sítio,
 * dadas as condições de contorno nas fronteiras esquerda e direita.
 * Determinadas as pressões nos sítios das fronteiras, pode-se determinar a permeabilidade intrinseca.
*/
class CPermeabilidadeRede : public CPermeabilidade
{
		// --------------------------------------------------------------Atributos
	protected:
		CMFluido * fluido; ///< Objeto fluido
		CSMDiagonalDominante *solver; ///< Objeto solver
		CRedeContorno *rede; ///< Objeto grafo

		unsigned int nx; ///< Dimensão nx da imagem
		unsigned int ny; ///< Dimensão ny da imagem
		unsigned int nz; ///< Dimensão nz da imagem
		unsigned int fatorAmplificacao;	///< Fator de amplificação
		unsigned int numeroPixelsBorda;	///< Número de píxeis a serem descontados da borda
		unsigned int iteracoes; ///< número de iterações
		long double sizePixel; ///< Dimensão do píxel
		long double fluxoFronteira; ///< Valor do fluxo na fronteira
		long double diferencaPressao; ///< Diferença de pressão entre as faces
		long double dimensaoX; ///< Dimensão x
		long double dimensaoY; ///< Dimensão y
		long double dimensaoZ; ///< Dimensão z
		long double comprimento; ///< comprimento
		long double area; ///< area
		long double erroPermeabilidade; ///< erro na determinação da permeabilidade

	public:
		// -------------------------------------------------------------Construtor
		/// Construtor
		CPermeabilidadeRede (
				CMFluido * &_fluido,
				CSMDiagonalDominante *& _solver,
				CRedeContorno *& _rede,
				unsigned int _nx,
				unsigned int _ny,
				unsigned int _nz,
				unsigned int _fatorAmplificacao = 1,
				long double _sizePixel = 1.0,
				unsigned int _numeroPixelsBorda = 0
		);

		// --------------------------------------------------------------Destrutor
		/// Destrutor
		virtual ~ CPermeabilidadeRede ();

		// ----------------------------------------------------------------Métodos
	private:
		/// Calcula o fluxo na fronteira. Recebe como parâmetro a identificação da fronteira
		long double FluxoFronteira (CContorno::ETipoContorno fronteira);

	protected:
		/// Cria objetos agregados
		virtual void CriacaoObjetosAgregados () {}

		/// Define as condições de contorno e adicionalmente chama funcao de calculo das condutancias
		virtual void DefinicaoCondicoesContorno ();

		/// Define valores iniciais
		virtual void DefinicaoValoresIniciais ();

		/// Resolve o sistema de equações
		virtual void SolucaoSistemaEquacoes ();

	public:
		/// Go executa a solução do sistema e depois o calculo da permeabilidade em sí
		virtual long double Go ();

		/// Executa um passo do solver interno para as permeabilidades
		virtual long double Next ();

		// --------------------------------------------------------------------Get
		/// Retorna ponteiro para objeto fluido
		CMFluido *Getfluido () const {
			return fluido;
		}

		/// Retorna ponteiro para objeto solver
		CSMDiagonalDominante *Getsolver () const {
			return solver;
		}

		/// Retorna ponteiro para objeto rede
		CRedeContorno *GetRede () const {
			return rede;
		}

		/// Retorna numero pixeis da borda
		unsigned int GetnumeroPixelsBorda () const {
			return numeroPixelsBorda;
		}

		/// Retorna nx
		unsigned long int Getnx () const {
			return nx;
		}

		/// Retorna ny
		unsigned long int Getny () const {
			return ny;
		}

		/// Retorna nz
		unsigned long int Getnz () const {
			return nz;
		}

		/// Retorna dimensão do pixel
		long double GetsizePixel () const {
			return sizePixel;
		}

		/// Retorna o fator de amplificacao
		unsigned long int GetfatorAmplificacao () const {
			return fatorAmplificacao;
		}

		/// Retorna o erro permeabilidade
		long double GeterroPermeabilidade () const {
			return erroPermeabilidade;
		}

		/// Retorna o numero de iteracoes
		unsigned long int Getiteracoes () const {
			return iteracoes;
		}

	public:
		// --------------------------------------------------------------------Set
		/// Define o fluido
		void Setfluido (CMFluido * _p) {
			if (fluido)
				delete fluido;
			fluido = _p;
		}

		/// Define o solver
		void Setsolver (CSMDiagonalDominante * _p) {
			if (solver)
				delete solver;
			solver = _p;
		}

		/// Define o grafo
		void SetRede (CRedeContorno * _p) {
			if (rede)
				delete rede;
			rede = _p;
		}

		/// Define o numero de píxeis da borda
		void SetnumeroPixelsBorda (unsigned int _npb) {
			numeroPixelsBorda = _npb;
		}

		/// Define a dimensão nx
		void Setnx (unsigned int _nx) {
			nx = _nx;
		}

		/// Define a dimensão ny
		void Setny (unsigned int _ny) {
			ny = _ny;
		}

		/// Define a dimensão nz
		void Setnz (unsigned int _nz) {
			nz = _nz;
		}

		/// Define a dimensão do pixel
		void SetsizePixel (long double _sizePixel) {
			sizePixel = _sizePixel;
		}

		/// Define o fator amplificacao
		void SetfatorAmplificacao (unsigned int _fatorAmplificacao) {
			fatorAmplificacao = _fatorAmplificacao;
		}

		// -----------------------------------------------------------------Friend
		friend std::ostream & operator<< (std::ostream & os, const CPermeabilidadeRede & obj);
		// friend istream& operator>> (istream& is, CPermeabilidadeRede& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream & operator<< (std::ostream & os, const CPermeabilidadeRede & obj);
// istream& operator>> (istream& is, CPermeabilidadeRede& obj);

#endif
