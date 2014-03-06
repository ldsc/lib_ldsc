#ifndef CPermeabilidadeGrafo_h
#define CPermeabilidadeGrafo_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
						Assunto/Ramo: CPermeabilidadeGrafo...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CPermeabilidadeGrafo.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <iostream>
#include <string>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Base/_LIB_LDSC_CLASS.h>
#include <AnaliseImagem/Simulacao/Permeabilidade/CPermeabilidade.h>
#include <Amostra/Material/CMFluido.h>
#include <MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDiagonalDominante.h>
#include <EstruturaDados/CRedeContorno.h>
#include <Contorno/CContorno.h>
#include <Contorno/CContornoCentro.h>

#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial.h>

// ===============================================================================
// Documentacao Classe: CPermeabilidadeGrafo
// ===============================================================================
/**@brief	Classe usada para determinar a permeabilidade
 * de uma imagem tridimensional.
 *
 * Recebe como parâmetros:
 * CGrafo		: Objeto grafo (uma representação de objetos que compõem o grafo)
 * CMFluido 	: Objeto fluido com viscosidade, compressibilidade, densidade
 * TSolver	: Objeto solver, que resolve sistemas de equações
 *
 * Descreve-se a seguir algumas diferenças em relação ao programa desenvolvido pelo Liang Zhirong, e que
 * determinava a permeabilidade a partir da classificação e geração do grafo sobre o esqueleto 3D de Ma.
 *
 * O programa antigo do Liang:
 * Criava um objeto Node e um objeto Grapho
 * O Node tinha apenas informações do nó como Codigo, tipo, grau, correspondencia, pressao e condutancia.
 * O objeto grafo, tinha as funções de calculo das condutâncias, da pressao e da permeabilidade.
 * -A funcao de calculo das condutâncias estava vazia, pois as condutâncias
 * estavam sendo calculadas no programa que identifica os sítios e as ligações (no classify).
 * -A função de calculo da pressao definia os valores iniciais e depois realizava o calculo
 * do sistema de equações usando um sistema (interno) de relaxações sucessivas.
 * -A função de calculo da permeabilidade calculava para cada nó da interface (tipo 1),
 * os fluxos associados. Ou seja Qij=gij(Pi-Pj), e depois a permeabilidade em sí.
 *
 * O programa do André
 * Alterações realizadas:
 * 1-O programa foi todo documentado.
 *
 * 2-O programa conta com um diagrama OMT (das relações dos diversos objetos).
 * ../LIB_LDSC/lib_lmtp/docs/diagramas
 *
 * 3-Criam-se os objetos agregados:
 * grafo			- representa uma estrutura de sítios e ligações.
 * solver			- representa um solucionador de matrizes
 * fluido			- representa um fluido
 *
 * 4-Cria-se um objeto de calculo da permeabilidade CPermeabilidadeGrafo,
 * passando-se como parametros os objetos agregados.
 *
 * 5-O objeto grafo tem objetos do tipo TObjetoGrafo (ex: TSitios) que
 * tem as variaveis rótulo (que o identifica),
 * propriedade(é a condutancia) e x(a pressão).
 *
 * 6-Criado o objeto CPermeabilidadeGrafo, pode-se chamar a função
 * de solução do sistema de equações (SolucaoSistema()) que executa a sequência:
 * virtual void SolucaoSistema();
 *
 * Resolve o sistema como um todo,
 * 8.1)	virtual void CriarObjetosAgregados () ;// Cria objetos agregados
 * 8.2)	virtual void DefinirCondicoesContorno () ;// Define as condições de contorno
 * 8.2.1)  void AuxCalcularCondutancias(); // Chama funcao de calculo das condutâncias
 * 8.3)	virtual void DefinirValoresIniciais() ;// Define valores iniciais
 * 8.4)  virtual double Go () ;	// Calcula a permeabilidade (e os fluxos)
 * 8.4.1)  double AuxFluxoFronteira(int);// calcula os fluxos
 *
 * 9 -Redefinido todo o Layout (AOO), com uma concepção moderna e genérica
 *
 * 10 -Foram criados vários objetos novos, com nomes claros e objetivos claros.
 *
 * 11	-Uso de vector e stl
 *
 * 12 -Antes as propriedades do fluído  e do solver eram passadas através de um único arquivo de
 * dados. Agora é passado um objeto CMFluido e um objeto solver.
 *
 * 13 -As operações de leitura e escrita para cada objeto, estão no próprio objeto.
 * Ou seja as operações sobre os atributos do objeto são	realizadas pelo próprio objeto.
 *
 * 14 -A solução do sistema de equações é realizada por um objeto TSMSOR (um solver).
 * Que é um solver genérico, que recebe um vetor de dados que se relacionam e determina
 * os valores deste vetor para um práximo passo de tempo.
 *
 * O problema a ser resolvido é a determinação das pressões de equilíbrio em cada sítio,
 * dadas as condições de contorno nas fronteiras esquerda e direita.
 * Determinadas as pressões nos sítios das fronteiras, pode-se
 * determinar a permeabilidade intrinseca.
 * @author André Duarte Bueno
 * Superclasse:    CSimulacao -> CPermeabilidade -> CPermeabilidadeGrafo
*/
class CPermeabilidadeGrafo : public CPermeabilidade {
// --------------------------------------------------------------Atributos
protected:
     CMFluido *fluido; 				///< Objeto fluido
     CSMDiagonalDominante *solver; 		///< Objeto solver
     CGrafoConexaoSerial *grafo; 				///< Objeto grafo

		unsigned long int nx;				///< Dimensão nx da imagem
		unsigned long int  ny;				///< Dimensão ny da imagem
		unsigned long int  nz;				///< Dimensão nz da imagem

     unsigned long int fatorAmplificacao;	///< Fator de amplificação
     long double dimensaoPixel;				///< Dimensão do píxel
     unsigned long int numeroPixelsBorda;	///< Número de píxeis a serem descontados da borda
     long double fluxoFronteira;			///< Valor do fluxo na fronteira

		/*<fatores necessários para calculo permeabilidade */
		long double diferencaPressao; 		///< Diferença de pressão entre as faces
		long double dimensaoX; 			///< Dimensão x
		long double dimensaoY; 			///< Dimensão y
		long double dimensaoZ; 			///< Dimensão z
		long double comprimento; 			///< comprimento
		long double area; 				///< area
		long double erroPermeabilidade; 		///< erro na determinação da permeabilidade
		unsigned int iteracoes; 			///< número de iterações

public:
// -------------------------------------------------------------Construtor
     /// Construtor
     CPermeabilidadeGrafo ( CMFluido*&_fluido,
                            CSMDiagonalDominante  *&_solver,
                            CGrafoConexaoSerial  *&_grafo,
                            unsigned long int _nx,
                            unsigned long int _ny,
                            unsigned long int _nz,
                            unsigned long int _fatorAmplificacao,
                            long double _dimensaoPixel,
                            unsigned long int _numeroPixelsBorda = 0 );

		// --------------------------------------------------------------Destrutor
		/// Destrutor
		virtual ~ CPermeabilidadeGrafo ();

		// ----------------------------------------------------------------Métodos
	private:

		// Funções internas auxiliares, próprias da classe  CPermeabilidadeGrafo
		/// Calcula o fluxo na fronteira,
		/// recebe como parâmetro a identificação da fronteira
		long double FluxoFronteira (CContorno::ETipoContorno fronteira);

	protected:

     // Sequencia usual de um TSimulador
     /// Cria objetos agregados
     virtual void CriarObjetosAgregados ();

     /// Define as condições de contorno
     // e adicionalmente chama funcao de calculo das condutâncias
     virtual void DefinirCondicoesContorno ();

     /// Define valores iniciais
     virtual void DefinirValoresIniciais ();

		/// Resolve o sistema de equações
		virtual void SolucaoSistemaEquacoes ();

	public:

		/// Go executa a solução do sistema e depois o calculo da permeabilidade em sí
		virtual long double Go ();

		/// Executa um passo do solver interno para as permeabilidades
		virtual long double Next ();

// -------------------------------------------------------------Sobrecarga
// --------------------------------------------------------------------Get
     // Funções de obtenção dos atributos internos
     /// Retorna ponteiro para objeto fluido
     CMFluido *Fluido () const {
          return fluido;
     }

     /// Retorna ponteiro para objeto solver
     CSMDiagonalDominante *Solver () const {
          return solver;
     }

     /// Retorna ponteiro para objeto grafo
     CGrafoConexaoSerial *Getgrafo () const {
          return grafo;
     }

     /// Retorna numero pixeis da borda
     unsigned long int GetnumeroPixelsBorda () const {
          return numeroPixelsBorda;
     }

     /// Retorna nx
     unsigned long int Nx () const  {
          return nx;
     }

     /// Retorna ny
     unsigned long int Ny () const   {
          return ny;
     }

     /// Retorna nz
     unsigned long int Nz () const   {
          return nz;
     }

     /// Retorna dimensão do pixel
     long double SizePixel () const {
          return dimensaoPixel;
     }

     /// Retorna o fator de amplificacao
     unsigned long int FatorAmplificacao () const {
          return fatorAmplificacao;
     }

     /// Retorna o erro permeabilidade
     long double ErroPermeabilidade () const  {
          return erroPermeabilidade;
     }

     /// Retorna o numero de iteracoes
     unsigned long int Iteracoes () const  {
          return iteracoes;
     }

public:
// --------------------------------------------------------------------Set
     // Funções de setagem dos atributos internos
     /// Define o fluido
     void Fluido ( CMFluido *_p )  {
          if ( fluido ) {
               delete fluido;
          }
          fluido = _p;
     }

     /// Define o solver
     void Solver ( CSMDiagonalDominante *_p )  {
          if ( solver ) {
               delete solver;
          }
          solver = _p;
     }

     /// Define o grafo
     void Grafo ( CGrafoConexaoSerial *_p ) {
          if ( grafo ) {
               delete grafo;
          }
          grafo = _p;
     }

     /// Define o numero de píxeis da borda
     void NumeroPixelsBorda ( unsigned long int _npb )  {
          numeroPixelsBorda = _npb;
     }

     /// Define a dimensão nx

     void Nx ( unsigned long int _nx ) {
          nx = _nx;
     }
     /// Define a dimensão ny
     void Ny ( unsigned long int _ny ) {
          ny = _ny;
     }
     /// Define a dimensão nz
     void Nz ( unsigned long int _nz ) {
          nz = _nz;
     }
     /// Define a dimensão do pixel
     void SizePixel ( long double _dimensaoPixel ) {
          dimensaoPixel = _dimensaoPixel;
     }
     /// Define o fator amplificacao
     void FatorAmplificacao ( unsigned long int _fatorAmplificacao ) {
          fatorAmplificacao = _fatorAmplificacao;
     }

// -----------------------------------------------------------------Friend
     friend std::ostream &operator<< ( std::ostream &os,
                                       const CPermeabilidadeGrafo &obj );
     // friend istream& operator>> (istream& is, CPermeabilidadeGrafo& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream &operator<< ( std::ostream &os,
                           const CPermeabilidadeGrafo &obj );
// istream& operator>> (istream& is, CPermeabilidadeGrafo& obj);

#endif
