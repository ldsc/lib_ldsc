#ifndef CFEDCCanny_h
#define CFEDCCanny_h

/*
----------------------------------------------------------------------------
Bibliotecas e Defines
----------------------------------------------------------------------------
*/
#ifndef CFEDeteccaoContorno_h
#include <Filtro/FEspacial/FEDeteccaoContorno/CFEDeteccaoContorno.h>
#endif

// inclue biblioteca imagem tem a f2d temporariamente
#ifndef CImagem_h
#include <Matriz/CImagem.h>
#endif

#define MAX_CANNY

/* Scale floating point magnitudes and angles to 8 bits */
#define ORI_SCALE 40.0
#define MAG_SCALE 20.0

/* Biggest possible filter mask */
#define MAX_MASK_SIZE 20

/**
 * @brief Classe para detecção de contorno usando método Canny.
 * 
*/
class CFEDCCanny : public CFEDeteccaoContorno
{

protected:
  int low; ///< Atributo low
  int high; ///< Atributo high
  int WIDTH; ///< Atributo WIDTH
  float s;///< Atributo s
  float ratio;///< Atributo ratio

  // Criar diálogo para entrada de s,low,high parâmetros

// Métodos
public:			
	/// Construtor
    CFEDCCanny (TMatriz2D< int > * &matriz,
		unsigned int _tamanhoMascara)
      : CFEDeteccaoContorno (matriz,  _tamanhoMascara)
  {
    ratio = 0.1;
    WIDTH = 0;
    s = 4.0;
    low = 5, high = 3;		// chamar diálogo para definição
  }

  /// Destrutor
  ~CFEDCCanny ()
  {
  }				

  /// Processamento da filtragem
  virtual TMatriz2D< int > *Go (TMatriz2D< int > * &matriz, unsigned int _tamanhoMascara = 0);

protected:
/// Método auxiliar trace
  int trace (int i, int j, int low, CImagem * im, CImagem * mag,
	     CImagem * ori);

/// Método auxiliar gauss
  float gauss (float x, float sigma);

/// Método auxiliar dGauss
  float dGauss (float x, float sigma);

  /// Método auxiliar meanGauss
  float meanGauss (float x, float sigma);

  /// Método auxiliar hysteresis
  void hysteresis (int high, int low, CImagem * im, CImagem * mag,
		   CImagem * oriim);

  /// Método auxiliar canny
  void canny (float s, CImagem * im, CImagem * mag, CImagem * ori);

  /// Método auxiliar seperable_convolution
  void seperable_convolution (CImagem * im, float *gau, int width,
			      float **smx, float **smy);

  /// Método auxiliar dxy_seperable_convolution
  void dxy_seperable_convolution (float **im, int nr, int nc, float *gau,
				  int width, float **sm, int which);

  /// Método auxiliar nonmax_suppress
  void nonmax_suppress (float **dx, float **dy, int nr, int nc,
			CImagem * mag, CImagem * ori);

  /// Método auxiliar estimate_thresh
  void estimate_thresh (CImagem * mag, int *low, int *hi);

  /// Método auxiliar norm
  float norm (float x, float y);	// não estava listado
};
#endif //  CFEDCCanny_h
