#ifndef CFFT_h
#define CFFT_h

/**
 * @brief Classe para cálculo da FFT.
 * 
 * 
 * Descrição: O arquivo fftn.h continha a definição das funções do arquivo 
 * fftn.c.
 * Como preciso de um objeto para realizar a transformada de Fourier,
 * simplesmente encapsulei o programa fftn, que determina a transformada de
 * Fourier direta e inversa.
 * Como o procedimento de cálculo é complexo, o codigo não tem nenhuma
 * linha alterada.
 * Simplesmente cria-se um objeto CFFT que basicamente chama
 * a função de cálculo da transformada de Fourier.
 *
 * 
 * Principais parâmetros utilizados: 
 * Re[]:		matriz da parte real
 * Im[]:		matriz da parte imaginaria
 * nTotal:	total number of complex values
 * nPass:	number of elements involved in this pass of transform
 * nSpan:	nspan/nPass = numero de bytes para incremento do ponteiro
 *				in Re[] and Im[]
 * isign:	usado para definir se é para calcular a 
 * transformadfa de fourier ou sua inversa. exponent:
 * +1 = forward   FFT
 * -1 = reverse   Inversa FFT
 * scaling:  constante usada para dividir o resultado final (normalizar)
 * scaling  se == -1, normaliza pela dimensao total da transformada
 * scaling  se <  -1, normaliza pela raiz quadrada da dimensao
 * total da transformada
 *
 * ----------------------------------------------------------------------
 * Maiores detalhes veja comentários no codigo
 */

class /**/ CFFT
{
  //Atributos (antigos)
  int scaling;		///< Fator de multiplicacao

  int iSign;		///< Direção da transformada

  int ndim;		///< Dimensao da matriz
  
  int *dims;		///< Vetor com as dimensoes

  //Metodos
 public:

  //Abaixo funções originais, encapsuladas
	 
	 static int fft_free ();	//libera memoria alocada ok
	 
 /** @brief  Libera memoria alocada.
  * Definida como publica e estática porque é usada pela
  * fftradixf que é global
  * double precision routine	
  * */ 
  static int fftradix (double Re[], double Im[], size_t nTotal, size_t nPass,
		       size_t nSpan, int isign, int max_factors,
		       int max_perm);

 /** @brief  Libera memoria alocada.
   * Definida como publica e estática porque é usada pela
   * fftradixf que é global
   * float precision routine
   * static int fft_free ();	
  * */
  static int fftradix (float Re[], float Im[], size_t nTotal, size_t nPass,
		       size_t nSpan, int isign, int max_factors,
		       int max_perm);

 private:
  /// Transformada de fourier para double
  static int fftn (int ndim, const int dims[], double Re[], double Im[],
		   int isign, double scaling);

  /// Transformada de fourier para float
  //comentei: int fftnf (int ndim, const int dims[], float Re[], float Im[], int isign, double scaling);
  static int fftn (int ndim, const int dims[], float Re[], float Im[],
		   int isign, double scaling);

  /* A função abaixo foi definida como global
     int fftradix (double Re[], double Im[],size_t nTotal, size_t nPass, size_t nSpan, int isign,
     int max_factors, int max_perm);
     int	fftradixf (float Re[], float Im[],size_t nTotal, size_t nPass, size_t nSpan, int isign,
     int max_factors, int max_perm);
  */

  //Abaixo função nova
  //abaixo estava em comentário, coloquei de volta, pois o arquivo funcoesAuxiliaresParaCFFT
  //define esta função como sendo da classe
  //      int FFTN (int ndim, const int dims[],   REAL Re ,REAL Im ,int iSign,    double scaling);

 public:
	 /// Construtor
  CFFT (int _ndim, int *_dims, double _scaling);	

  ///Destrutor
  virtual ~ CFFT ();	

/// Calcula a transformada de Fourier float
  int Go (float *, float *);	
  
/// Calcula a transformada de Fourier double
  int Go (double *, double *);	
  
  /// Calcula a inversa da transformada de Fourier float
  int Go_Inverse (float *, float *);	
  
  /// Calcula a inversa da transformada de Fourier  double
  int Go_Inverse (double *, double *);	
  
  /// Funções de definição dos atributos internos
  void SetScaling (int escala)
  {
    scaling = escala;
  }				//seta fator de multiplicacao
};
#endif
