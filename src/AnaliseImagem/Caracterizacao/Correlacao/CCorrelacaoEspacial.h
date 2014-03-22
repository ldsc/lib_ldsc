#ifndef CCorrelacaoEspacial_h
#define CCorrelacaoEspacial_h

#include <AnaliseImagem/Caracterizacao/Correlacao/CCorrelacao.h>
#include <MetNum/Matriz/TCMatriz2D.h>

/**
 * @brief Recebe uma imagem bidimensional e determina através do método do deslocamento da imagem sobre ela mesma, a função autocorrelação.
 * O objeto tem as dimensões da imagem NX, NY, os vetores para as imagens (Re_data e Im_data),
 * uma matriz para a imagem (pm2d) e um vetor float * para a função autocorrelação.
*/

class CCorrelacaoEspacial : public CCorrelacao
{
 public:
	/// Construtor
	CCorrelacaoEspacial ( int dimensao_vetor_correlacao = 320 );
	
	/// Destrutor
	virtual ~ CCorrelacaoEspacial ();
  	
   /// Determina a função autocorrelação. Recebe ponteiro para vetor imagem, as dimensões da imagem e o valore que representa índice na imagem, ou seja, o valor que será correlacionado.
   virtual bool Go (float *Re_data, int NX, int NY, int indice=1);

   /// Determina a função autocorrelação. Recebe ponteiro para TCMatriz2D< int > e o valore que representa índice na imagem, ou seja, o valor que será correlacionado.
	 virtual bool Go (TCMatriz2D< bool > *img, int indice=1);

};

#endif
