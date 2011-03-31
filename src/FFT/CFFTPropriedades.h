

#ifndef CFFTPropriedades_h
#define CFFTPropriedades_h

#include    "FFT/CFFT.h"	//classe

/**
 * @brief Acrescenta a FFT algumas propriedades da transformada de Fourier.
 * Herdeira de CFFT
*/
class /*_LIB_LDSC_CLASS*/ CFFTPropriedades : public CFFT
{
//Atributos
//Atributos (novos)
protected:
  int NX; ///< Dimensão x
  int NY; ///< Dimensão y
  int NZ; ///< Dimensão z	
  
//Metodos
public:
  /// Construtor
  CFFTPropriedades (int _ndim, int *_dims, double _scaling):CFFT (_ndim,
								  _dims,
								  _scaling)
  {
    //novos  //de fato serão utilizados pela classe filha
    if (_ndim > 0)
      NX = _dims[0];
    if (_ndim > 1)
      NY = _dims[1];
    if (_ndim > 2)
      NZ = _dims[2];
  }
  
  /// Destrutor
  virtual ~ CFFTPropriedades ()
  {
  }

  /// Cálculo de propriedades específicas.
  /// Funções da transformada de Fourier relacionadas a propriedades da transformada de Fourier
  void FFTY_of_ModY (double *Re_data, double *Im_data);	//,int semente);
  
  //Dado o Modulo da Transformada de Fourier de Y ,|Y|,
  //determina Y, gerando angulos randomicamente
  //a função utiliza um objeto para gerar os numeros randomicos
};
#endif
