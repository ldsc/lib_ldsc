#ifndef CFEDCMarrHildreth_h
#define CFEDCMarrHildreth_h

/*
----------------------------------------------------------------------------
Bibliotecas e Defines
----------------------------------------------------------------------------
*/

// inclue a biblioteca de Matrizzes
#ifndef TCMatriz2D_H
#include <MetNum/Matriz/TCMatriz2D.h>
#endif

#ifndef CFEDeteccaoContorno_h
#include <AnaliseImagem/Filtro/FEspacial/FEDeteccaoContorno/CFEDeteccaoContorno.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
				Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico 
			dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
			Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEDCMarrHildreth.h
Nome da classe:      CFEDCMarrHildreth
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Classe para detecção de contorno usando método de Marr/Hildreth.
*/
class CFEDCMarrHildreth:public CFEDeteccaoContorno
{
// Atributos
// Métodos
public:			
	/// Construtor
  CFEDCMarrHildreth (TCMatriz2D< int > * &matriz,
		     unsigned int  _tamanhoMascara)
                   : CFEDeteccaoContorno (matriz,  _tamanhoMascara)
  {
  }

  /// Destrutor
  ~CFEDCMarrHildreth ()
  {
  }				

  /// Processamento da filtragem
  virtual TCMatriz2D< int > *Go (TCMatriz2D< int > * &matriz, 
			 unsigned int _tamanhoMascara =	 0);


protected:
  /// Método auxiliar convolution
  void convolution (TCMatriz2D< int > * im, float **mask, int nr, int nc,
		    float **res, int NR, int NC);

  /// Método auxiliar gauss
  float gauss (float x, float sigma);

    /// Método auxiliar LoG
  float LoG (float x, float sigma);

    /// Método auxiliar meanGauss
  float meanGauss (float x, float sigma);

    /// Método auxiliar marr
  void marr (float s, TCMatriz2D< int > * im);

    /// Método auxiliar dolap
  void dolap (float **x, int nr, int nc, float **y);

    /// Método auxiliar zero_cross
  void zero_cross (float **lapim, TCMatriz2D< int > * im);

    /// Método auxiliar norm
  float norm (float x, float y);

    /// Método auxiliar distance
  float distance (float a, float b, float c, float d);
};
#endif //  CFEDCMarrHildreth_h
