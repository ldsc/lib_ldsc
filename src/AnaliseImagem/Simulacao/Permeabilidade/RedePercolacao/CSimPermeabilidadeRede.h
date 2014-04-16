#ifndef CSimPermeabilidadeRede_h
#define CSimPermeabilidadeRede_h
// Bibliotecas C/C++
#include <iostream>
#include <string>

// Bibliotecas LIB_LDSC
//#include <Base/_LIB_LDSC_CLASS.h>
#include <AnaliseImagem/Simulacao/Permeabilidade/CSimPermeabilidade.h>
#include <Amostra/Material/CMFluido.h>
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonalDominante.h>
#include <AnaliseImagem/Caracterizacao/RedePercolacao/CContornoRedePercolacao.h>
#include <MetNum/Contorno/CContornoCentro.h>

/**
 * @file		CSimPermeabilidadeRede.h
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
 * 2-Cria-se um objeto de calculo da permeabilidade CSimPermeabilidadeRede,
 * passando-se como parametros os objetos agregados.
 *
 * 3-O objeto rede tem objetos do tipo CObjetoImagem (ex: Sitios e Ligações) que
 * tem uma propriedade(condutância) e x (a pressão).
 *
 * 4-Criado o objeto CSimPermeabilidadeRede, pode-se chamar a função
 * de solução do sistema de equações (SolucaoSistema());
 *
 * Resolve o sistema como um todo,
 * 4.1)	virtual void CriacaoObjetosAgregados () ;// Cria objetos agregados
 * 4.2)	virtual void DefinirCondicoesContorno () ;// Define as condições de contorno
 * 4.2.1)  void AuxCalculoCondutancias(); // Chama funcao de calculo das condutancias
 * 4.3)	virtual void DefinirCondicoesIniciais() ;// Define valores iniciais
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
class CSimPermeabilidadeRede : public CSimPermeabilidade
{
		// --------------------------------------------------------------Atributos
	protected:
		CMFluido * fluido{nullptr}; ///< Objeto fluido
		CSolverMatrizDiagonalDominante *solver{nullptr}; ///< Objeto solver
		CContornoRedePercolacao *rede{nullptr}; ///< Objeto grafo

		unsigned int nx{0}; ///< Dimensão nx da imagem
		unsigned int ny{0}; ///< Dimensão ny da imagem
		unsigned int nz{0}; ///< Dimensão nz da imagem
		unsigned int fatorAmplificacao{1}; ///< Fator de amplificação
		unsigned int numeroPixeisBorda{0}; ///< Número de píxeis a serem descontados da borda
		long double dimensaoPixel{1.0}; ///< Dimensão do píxel (multiplique por nx e pelo fatorAmplificacao para obter dimensão real da imagem em metros - SI).
		// Variáveis necessárias para cálculo permeabilidade (Lei Darcy).
		long double fluxoFronteira{0.0};			///< Valor do fluxo na fronteira.
		long double diferencaPressao{0.0};			///< Diferença de pressão entre as faces
		long double dimensaoX{0.0}; 				///< Dimensão real da imagem na direção x (desconta as bordas).
		long double dimensaoY{0.0}; 				///< Dimensão real da imagem na direção y (desconta as bordas).
		long double dimensaoZ{0.0}; 				///< Dimensão real da imagem na direção z (desconta as bordas).
		long double comprimento_z{0.0}; 			///< Comprimento.
		long double area{0.0}; 					///< Área da seção considerada Lei Darcy.
		// Solver interno, calcula fluxo nas fronteiras e determina necessidade de refinar cálculo das pressões em função do ErroPermeabilidade e do número iterações.
		long double erroRelativo{100.0}; ///< erro percentual na determinação da permeabilidade. inicial = 100%.
		long double limiteErroRelativo{ 1.0 }; ///< limite erro relativo, default =1%
		unsigned int iteracoes{0}; ///< número de iterações realizadas no cálculo permeabilidade.
		unsigned long int limiteIteracoes{ 5000 }; ///< limite de iterações.

	public:
		// -------------------------------------------------------------Construtor
		/// Construtor
		CSimPermeabilidadeRede (
				CMFluido * &_fluido,
				CSolverMatrizDiagonalDominante *& _solver,
				CContornoRedePercolacao *& _rede,
				unsigned int _nx,
				unsigned int _ny,
				unsigned int _nz,
				unsigned int _fatorAmplificacao = 1,
				long double _dimensaoPixel = 1.0,
				unsigned int _numeroPixeisBorda = 0 );

		// --------------------------------------------------------------Destrutor
		/// Destrutor
		virtual ~ CSimPermeabilidadeRede ();

		// ----------------------------------------------------------------Métodos
	public:
		/// Go executa a solução do sistema e depois o calculo da permeabilidade em sí
		virtual long double Go () override;

		/// Executa um passo do solver interno para as permeabilidades
		long double Next ();

	protected:
		/// Cria objetos agregados
		virtual void CriarObjetosAgregados () override;

		/// Define as condições de contorno e adicionalmente chama funcao de calculo das condutancias
		virtual void DefinirCondicoesContorno () override;

		/// Define valores iniciais
		virtual void DefinirCondicoesIniciais () override;

		/// Resolve o sistema de equações
		virtual void SolucaoSistemaEquacoes () override;

	private:
		/// Calcula o fluxo na fronteira. Recebe como parâmetro a identificação da fronteira
		long double FluxoFronteira (CContorno::ETipoContorno fronteira);

	public:
		// --------------------------------------------------------------------Get
		/// Retorna ponteiro para objeto fluido
		CMFluido *GetFluido () const {
			return fluido;
		}

		/// Retorna ponteiro para objeto solver
		CSolverMatrizDiagonalDominante *GetSolver () const {
			return solver;
		}

		/// Retorna ponteiro para objeto rede
		CContornoRedePercolacao *GetRede () const {
			return rede;
		}

		/// Retorna numero pixeis da borda
		unsigned int GetNumeroPixeisBorda () const {
			return numeroPixeisBorda;
		}

		/// Retorna nx
		unsigned long int GetNx () const {
			return nx;
		}

		/// Retorna ny
		unsigned long int GetNy () const {
			return ny;
		}

		/// Retorna nz
		unsigned long int GetNz () const {
			return nz;
		}

		/// Retorna dimensão do pixel
		long double GetDimensaoPixel () const {
			return dimensaoPixel;
		}

		/// Retorna o fator de amplificacao
		unsigned long int GetFatorAmplificacao () const {
			return fatorAmplificacao;
		}

		/// Retorna o erro permeabilidade
		long double GetErroRelativo () const {
			return erroRelativo;
		}

		/// Retorna o numero de iteracoes
		unsigned long int GetIteracoes () const {
			return iteracoes;
		}

	public:
		// --------------------------------------------------------------------Set
		/// Define o fluido
		void SetFluido (CMFluido * _p) {
			if (fluido)
				delete fluido;
			fluido = _p;
		}

		/// Define o solver
		void SetSolver (CSolverMatrizDiagonalDominante * _p) {
			if (solver)
				delete solver;
			solver = _p;
		}

		/// Define o grafo
		void SetRede (CContornoRedePercolacao * _p) {
			if (rede)
				delete rede;
			rede = _p;
		}

		/// Define o numero de píxeis da borda
		void SetNumeroPixeisBorda (unsigned int _npb) {
			numeroPixeisBorda = _npb;
		}

		/// Define a dimensão nx
		void SetNx (unsigned int _nx) {
			nx = _nx;
		}

		/// Define a dimensão ny
		void SetNy (unsigned int _ny) {
			ny = _ny;
		}

		/// Define a dimensão nz
		void SetNz (unsigned int _nz) {
			nz = _nz;
		}

		/// Define a dimensão do pixel
		void SetDimensaoPixel (long double _dimensaoPixel) {
			dimensaoPixel = _dimensaoPixel;
		}

		/// Define o fator amplificacao
		void SetFatorAmplificacao (unsigned int _fatorAmplificacao) {
			fatorAmplificacao = _fatorAmplificacao;
		}

		// -----------------------------------------------------------------Friend
		friend std::ostream & operator<< (std::ostream & os, const CSimPermeabilidadeRede & obj);
		// friend istream& operator>> (istream& is, CSimPermeabilidadeRede& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream & operator<< (std::ostream & os, const CSimPermeabilidadeRede & obj);
// istream& operator>> (istream& is, CSimPermeabilidadeRede& obj);

#endif
