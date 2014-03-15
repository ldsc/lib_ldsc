#ifndef CSimPermeabilidadeGrafo_h
#define CSimPermeabilidadeGrafo_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CSimPermeabilidadeGrafo...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CSimPermeabilidadeGrafo.h
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
#include <AnaliseImagem/Simulacao/Permeabilidade/CSimPermeabilidade.h>
#include <Amostra/Material/CMFluido.h>
#include <MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDiagonalDominante.h>
#include <EstruturaDados/CRedeContorno.h>
#include <Contorno/CContornoCentro.h>

#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial.h>

// ===============================================================================
// Documentacao Classe: CSimPermeabilidadeGrafo
// ===============================================================================
/**@brief	Classe usada para determinar a permeabilidade de uma imagem tridimensional
 * usando o GrafoConexaoSerial.
 * Herda de CSimulacao->CSimPermeabilidade, um conjunto de métodos virtuais que serão reescritos
 * para cálculo da Permeabilidade.
 * Recebe como parâmetros:
 * CGrafo	: Objeto grafo (um grafo montado a partir da análise das seções de uma imagem bidimensional).
 * CMFluido : Objeto fluido, com viscosidade, compressibilidade, densidade.
 * CSolver	: Objeto solver, com propriedades de um solver que resolve o sistemas de equações (as pressões do sistema).
 *
 * Descreve-se a seguir algumas diferenças em relação ao programa desenvolvido pelo Liang Zhirong;
 * Liang determinava a permeabilidade a partir da classificação e geração do grafo sobre o esqueleto 3D de Ma.
 * O programa do Liang:
 * Criava um objeto Node e um objeto Grapho.
 * O Node tinha apenas informações do nó como Código, tipo, grau, correspondência, pressão e condutância.
 * O objeto grafo, tinha as funções de cálculo das condutâncias, da pressão e da permeabilidade.
 * -A funcao de cálculo das condutâncias estava vazia, pois as condutâncias
 * estavam sendo calculadas no programa que identifica os sítios e as ligações (no classify).
 * -A função de cálculo da pressão definia os valores iniciais e depois realizava o cálculo
 * do sistema de equações usando um sistema (interno) de relaxações sucessivas.
 * -A função de cálculo da permeabilidade calculava, para cada nó da interface (tipo 1),
 * os fluxos associados. Ou seja Qij=gij(Pi-Pj), e depois a permeabilidade em sí.
 *
 * O programa do Bueno
 * Alterações realizadas:
 * 1-O programa foi todo documentado.
 *
 * 2-O programa conta com um diagrama OMT (das relações dos diversos objetos).
 * ../LIB_LDSC/lib_lmtp/docs/diagramas -> Atualizado com o uso de javadoc/doxygen, veja libldsc-docs.
 *
 * 3-Criam-se os objetos agregados:
 * grafo	- representa uma estrutura de sítios e ligações.
 * solver	- representa um solucionador de matrizes.
 * fluido	- representa um fluido.
 *
 * 4-Cria-se um objeto de cálculo da permeabilidade CSimPermeabilidadeGrafo,
 * passando-se como parametros os objetos agregados.
 *
 * 5-O objeto grafo tem objetos do tipo CObjetoGrafo (ex: CObjetoRede) que tem as variaveis rótulo (que o identifica),
 * propriedade(é a condutancia) e x(a pressão).
 *
 * 6-Criado o objeto CSimPermeabilidadeGrafo, pode-se chamar a função 
 * virtual void SolucaoSistema() que resolve o sistema como um todo, chamando os métodos:
 * 8.1)		virtual void CriarObjetosAgregados () ;     // Cria objetos agregados
 * 8.2)		virtual void DefinirCondicoesContorno () ;  // Define as condições de contorno
 * 8.2.1)  	void AuxCalcularCondutancias();          	// Chama funcao de cálculo das condutâncias
 * 8.3)		virtual void DefinirCondicoesIniciais();    // Define valores iniciais
 * 8.4)  	virtual double Go ();                      	// Resolve as pressões e a seguir calcula a permeabilidade 
 * 8.4.1)  	double AuxFluxoFronteira(int);           	// Calcula os fluxos (controle erro Permeabilidade)
 *
 * 9 -Redefinido todo o Layout (AOO), com uma concepção moderna e genérica.
 *
 * 10 -Foram criados vários objetos novos, com nomes claros e objetivos claros.
 *
 * 11 -Uso de vector e stl.
 *
 * 12 -Antes as propriedades do fluído e do solver eram passadas através de um único arquivo de
 * dados. Agora é passado um objeto CMFluido e um objeto CSolver. 
 * Os dados de cada objeto estão armazenados em arquivos de disco individuais.
 *
 * 13 -As operações de leitura e escrita para cada objeto, estão no próprio objeto.
 * Ou seja as operações sobre os atributos do objeto são realizadas pelo próprio objeto.
 *
 * 14 -A solução do sistema de equações é realizada por um objeto CSMDiagonalDominante 
 * (um solver para matrizes com diagonal dominante - na realizada gera uma matriz esparsa).
 * Que é um solver genérico, que recebe um vetor de dados que se relacionam e determina
 * os valores deste vetor para um próximo passo de tempo.
 *
 * Para o problema ser resolvido é necessário:
 * Calcular as pressões de cada sítio/ligação, isto é feito chamando SolucaoSistema(), que envolve:
 * - Definir as condições de contorno nas fronteiras esquerda e direita;
 * - Definir as condições iniciais (valores iniciais de pressão em cada sítio), 
 * - Resolver o sistema de equações para as pressões em cada sítio.
 * 
 * Pode-se então chamar Go() para determinar a Permeabilidade.
 * - Resolvidas as pressões de equilíbrio em cada sítio, pode-se determinar o fluxo nas fronteiras 
 * esquerda e direita e a seguir a permeabilidade intrinseca.
 * 
 * @author André Duarte Bueno
*/
class CSimPermeabilidadeGrafo : public CSimPermeabilidade {
// --------------------------------------------------------------Atributos
protected:
     /// Propriedades da imagem! (devem estar armazenadas na imagem...)
     unsigned long int nx{0};					///< Número píxeis na direção x da imagem.
     unsigned long int ny{0};					///< Número píxeis na direção y da imagem.
     unsigned long int nz{0};					///< Número píxeis na direção z da imagem.
     unsigned long int fatorAmplificacao{1};	///< Fator de amplificação usado na reconstrução da imagem.
     long double dimensaoPixel{0};				///< Dimensão do píxel (multiplique por nx e pelo fatorAmplificacao para obter dimensão real da imagem em metros - SI).
     unsigned long int numeroPixeisBorda{0};	///< Número de píxeis a serem descontados (dimensão da borda).

     /// Objetos agregados.
     CMFluido *fluido{nullptr}; 				///< Objeto fluido.
     CSMDiagonalDominante *solver{nullptr};		///< Objeto solver.
     CGrafoConexaoSerial *grafo{nullptr}; 		///< Objeto grafo de conexão serial.

     /// Propriedades da simulação (solver local cálculo permeabilidade).
     bool salvarDadosParciaisPressaoDisco { false }; ///< Se true salva dados parciais de pressão em disco.

     /// Solver interno, calcula fluxo nas fronteiras e determina necessidade de refinar cálculo das pressões
     /// em função do ErroPermeabilidade e do número iterações.
     long double erroRelativo{100.0}; 			///< erro percentual na determinação da permeabilidade. iinicial = 100%.
//long double & erroAbsolutoPermeabilidade = erro; // da classe base?
     long double limiteErroRelativo { 1.0 };  	///< limite erro relativo, default =1%
     unsigned int iteracoes{0};	 	///< número de iterações realizadas no cálculo permeabilidade.
     unsigned long int limiteIteracoes { 5000 };///< limite de iterações.

     /// Variáveis necessárias para cálculo permeabilidade (Lei Darcy).
     long double fluxoFronteira{0.0};			///< Valor do fluxo na fronteira.
     long double diferencaPressao{0.0};			///< Diferença de pressão entre as faces
     long double dimensaoX{0.0}; 				///< Dimensão real da imagem na direção x (desconta as bordas).
     long double dimensaoY{0.0}; 				///< Dimensão real da imagem na direção y (desconta as bordas).
     long double dimensaoZ{0.0}; 				///< Dimensão real da imagem na direção z (desconta as bordas).
     long double comprimento_z{0.0}; 			///< Comprimento.
     long double area{0.0}; 					///< Área da seção considerada Lei Darcy.

public:
// -------------------------------------------------------------Construtor
     /// Construtor, recebe os objetos agregados: fluido, solver, grafo; e as propriedades da imagem.
	/// Propriedades da simulação, solver interno e externo, estão sendo lidas de arquivo de disco.
     CSimPermeabilidadeGrafo ( CMFluido *& _fluido,
                            CSMDiagonalDominante *& _solver,
                            CGrafoConexaoSerial *& _grafo,
                            unsigned long int _nx,
                            unsigned long int _ny,
                            unsigned long int _nz,
                            unsigned long int _fatorAmplificacao,
                            long double _dimensaoPixel,
                            unsigned long int _numeroPixeisBorda = 0 );

// --------------------------------------------------------------Destrutor
     /// Destrutor
     virtual ~ CSimPermeabilidadeGrafo ();

// ----------------------------------------------------------------Métodos
public:

     /// Go() verifica se o sistema já foi resolvido, sistemaResolvido==true?, senão foi resolvido chama
	 /// SolucaoSistema() que resolve o sistema como um todo; depois Go() realiza o cálculo da permeabilidade em sí.
     virtual long double Go () override;

     /// Executa um passo do solver interno para cálculo das permeabilidades.
	 /// Depois de iniciada a simulação (cálculo da primeira estimativa das pressões),
	 /// pode-se calcular o fluxo nas fronteiras, e estimar o erro da Permeabilidade.
	 /// Caso este erro seja maior do que o aceitável, pode-se chamar Next() para refinar a solução das pressões.
	 /// Next() chama a SolucaoSistemaEquacoes() melhorando a estimativa das pressões e a seguir calcula
	 /// os fluxos em cada fronteira. Next() inclui a Lei de Darcy.
	 /// Retorna estimativa permeabilidade.
     virtual long double Next () /*override*/;

protected:
     // Sequencia usual de CSimulador::SolucaoSistema()
     /// Criar objetos agregados
     virtual void CriarObjetosAgregados () override;

     /// Definir as condições de contorno  (chama funcao de cálculo das condutâncias)
     virtual void DefinirCondicoesContorno () override;

     /// Para todos os objetos do grafo associa valores iniciais de pressão
     virtual void DefinirCondicoesIniciais () override;

     /// Resolver o sistema de equações (neste caso a solução das pressões)
     virtual void SolucaoSistemaEquacoes () override;

private:

     // Funções internas, auxiliares, próprias da classe  CSimPermeabilidadeGrafo.
     /// Calcula o fluxo na fronteira; recebe como parâmetro a identificação da fronteira (esquerda ou direita).
     long double FluxoFronteira ( CContorno::ETipoContorno fronteira );

// -------------------------------------------------------------Sobrecarga
// --------------------------------------------------------------------Get
public:
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
     CGrafoConexaoSerial *Grafo () const {
          return grafo;
     }

     /// Retorna número píxeis da borda
     unsigned long int NumeroPixelsBorda () const {
          return numeroPixeisBorda;
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
     long double DimensaoPixel () const {
          return dimensaoPixel;
     }

     /// Retorna o fator de amplificacao
     unsigned long int FatorAmplificacao () const {
          return fatorAmplificacao;
     }

     /// Retorna o erro permeabilidade
     long double ErroRelativo () const  {
          return erroRelativo;
     }

     /// Retorna o número de iteracoes
     unsigned long int Iteracoes () const  {
          return iteracoes;
     }

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

     /// Define o número de píxeis da borda
     void NumeroPixelsBorda ( unsigned long int _npb )  {
          numeroPixeisBorda = _npb;
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
     void DimensaoPixel ( long double _dimensaoPixel ) {
          dimensaoPixel = _dimensaoPixel;
     }
     /// Define o fator amplificacao
     void FatorAmplificacao ( unsigned long int _fatorAmplificacao ) {
          fatorAmplificacao = _fatorAmplificacao;
     }

// -----------------------------------------------------------------Friend
     friend std::ostream &operator<< ( std::ostream &os,
                                       const CSimPermeabilidadeGrafo &obj );
     // friend istream& operator>> (istream& is, CSimPermeabilidadeGrafo& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream &operator<< ( std::ostream &os,
                           const CSimPermeabilidadeGrafo &obj );
// istream& operator>> (istream& is, CSimPermeabilidadeGrafo& obj);
#endif
