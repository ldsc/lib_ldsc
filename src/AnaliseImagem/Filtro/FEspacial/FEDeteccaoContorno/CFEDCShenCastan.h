#ifndef CFEDCShenCastan_h
#define CFEDCShenCastan_h

/*
----------------------------------------------------------------------------
Bibliotecas e Defines
----------------------------------------------------------------------------
*/

#ifndef CFEDeteccaoContorno_h
#include <AnaliseImagem/Filtro/FEspacial/FEDeteccaoContorno/CFEDeteccaoContorno.h>
#endif

#ifndef TCMatriz2D_H
#include <Matriz/TCMatriz2D.h>	// inclue a biblioteca de matrizes
#endif // 

// #define MAX                    // MAX é usada na lib
// #include "lib.h"               // verificar efeito de MAX na lib
/*
//  Scale floating point magnitudes and angles to 8 bits
#define ORI_SCALE 40.0
#define MAG_SCALE 20.0
//  Biggest possible filter mask
#define MAX_MASK_SIZE 20
*/

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca de filtros de detecção de contorno
----------------------------------------------------------------------------

Desenvolvido por:      	Laboratorio de Desenvolvimento de Software Cientifico e 
Propriedades Termofisicas
			dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
			Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEDCShenCastan.h
Nome da classe:      CFEDCShenCastan
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Classe para detecção de contorno usando método de Shen-Castan.
 * 
 * Criar diálogo para entrada de parametros do metodo
 * 
 * // Os parametros do arquivo de disco são:
 * // ratio	0.035	(percentagem de pixel's acima do nivel de corte)
 * // b		.91	(verifica se esta no intervalo 0<=b<=1)(fator amortecimento)
 * // window_size	10	(tamanho da janela para filtro adaptativo)
 * // thinFactor     1	(fator de esqueletização)
 * // do_hysteresis  1	(verdadeiro ou falso)(Hysteresis thresholding )
*/

class CFEDCShenCastan : public CFEDeteccaoContorno
{
// Atributos
protected:
  double b;			///< Fator de amortecimento entre 0 e 1

  double low_thresh, high_thresh;	///< Nivel de corte para histerese

  double ratio;			///< razão

  int window_size;		///< Tamanho da janela

  int do_hysteresis;

  float **lap;			///< matriz laplaciano

  int nr; ///< numero linhas  nrows
  int nc;			///< numero  colunas ncols 

  TCMatriz2D< int > *edges;		///< keep track of edge points (thresholded)

  int thinFactor;		///< fator de thining (afinamento)

public:			/// Construtor

  CFEDCShenCastan (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara);

  /// Destrutor
  ~CFEDCShenCastan ()
  {
  }		
  		
  
  /// Processamento da filtragem
  virtual TCMatriz2D< int > *Go (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara = 0);

  /// Seta parâmetros do método.
  void SetParametros (double razao = 0.035, double fatorAmortecimento = .91,
		      int tamanhoJanela = 10, int fatorEsqueletizacao =
		      1, int usarHisterese = true)
  {
    ratio = razao;

    b = fatorAmortecimento;

    window_size = tamanhoJanela;

    thinFactor = fatorEsqueletizacao;

    do_hysteresis = usarHisterese;
  }

protected:
	/// Método auxiliar shen
  void shen (TCMatriz2D< int > * &im, TCMatriz2D< int > * &res);

	/// Método auxiliar compute_ISEF
  void compute_ISEF (float **x, float **y, int nrows, int ncols);

  	/// Método auxiliar apply_ISEF_vertical
  void apply_ISEF_vertical (float **x, float **y, float **A, float **B,
			    int nrows, int ncols);

  	/// Método auxiliar apply_ISEF_horizontal
  void apply_ISEF_horizontal (float **x, float **y, float **A,
			      float **B, int nrows, int ncols);

  	/// Método auxiliar compute_bli
  TCMatriz2D< int > *compute_bli (float **buff1, float **buff2, int nrows, int ncols);

  	/// Método auxiliar locate_zero_crossings
  void locate_zero_crossings (float **orig, float **smoothed,
			      TCMatriz2D< int > * &bli, int nrows, int ncols);

  	/// Método auxiliar threshold_edges
  void threshold_edges (float **in, TCMatriz2D< int > * &out, int nrows, int ncols);

  	/// Método auxiliar mark_connected
  int mark_connected (int i, int j, int level);

  	/// Método auxiliar is_candidate_edge
  int is_candidate_edge (TCMatriz2D< int > * &buff, float **orig, int row, int col);

  	/// Método auxiliar compute_adaptive_gradient
  float compute_adaptive_gradient (TCMatriz2D< int > * &BLI_buffer,
				   float **orig_buffer, int row, int col);

  	/// Método auxiliar estimate_thresh
  void estimate_thresh (double *low, double *hi, int nr, int nc);

  	/// Método auxiliar debed
  void debed (TCMatriz2D< int > * &im, int width);

  	/// Método auxiliar embed
  void embed (TCMatriz2D< int > * &im, int width);
};
#endif //  CFEDCShenCastan
