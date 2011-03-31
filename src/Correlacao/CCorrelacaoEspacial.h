#ifndef CCorrelacaoEspacial_h
#define CCorrelacaoEspacial_h

// #include <cstring>
// #include <string>
// #include <cstdlib>
// #include <fstream>
// #include <iostream>
// #include <cassert>

#include <Correlacao/CCorrelacao.h>
#include <Matriz/CMatriz2D.h>

/**
 * @brief Recebe uma imagem bidimensional e determina a função autocorrelação através do método do deslocamento.
 * 
 * Recebe uma imagem bidimensional e determina através do método do deslocamento da imagem sobre ela mesma, a função autocorrelação.
 * 
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
  	
	/// Determina a função autocorrelação. Recebe ponteiro para vetor imagem e as dimensões da imagem
	virtual bool Go (float *Re_data, int NX, int NY);
  	
  	/// Determina a função autocorrelação. Recebe ponteiro para CMatriz2D.
	virtual bool Go (CMatriz2D *img);
	
	/// Salva em disco os dados de correlação (extensao .cor).
	// virtual bool Write (std::string fileName);
};

#endif
