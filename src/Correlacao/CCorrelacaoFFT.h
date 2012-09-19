#ifndef CCorrelacaoFFT_h
#define CCorrelacaoFFT_h

#include   <Correlacao/CCorrelacao.h>

/**
 * @brief Recebe uma imagem bidimensional e determina através do uso da FFT a função autocorrelação.
 * 
 * Recebe uma imagem bidimensional (em um vetor float*, juntamente com NX e NY) 
 * e determina através do uso da FFT, a função autocorrelação.
 * 
 * O objeto tem: as dimensões da imagem NX,NY, os vetores para as imagens (Re_data e Im_data), 
 * um vetor float* para a função autocorrelação. E um nome de arquivo (sem extensão).
*/

class CCorrelacaoFFT : public CCorrelacao
{
protected:
	/// Matriz 2D da parte real
	float * Re_data;
	
	/// Matriz 2D da parte imaginária IM_data
	// Alocada e destruida internamente.
	float * Im_data;

	/// Dimensao NX da imagem IM_data
	int NX;
	
	/// Dimensao NY da imagem IM_data
	int NY;
	
	
public:
	/// Construtor
	CCorrelacaoFFT (  int dimensao_vetor_correlacao = 320  );
	
	/// Destrutor
	virtual ~ CCorrelacaoFFT (); 	

	// Métodos
	/** 
 * @brief Recebe parâmetros, carrega dados, aloca matrizez e depois chama Go, Write e Writerzf. */
	int GoMain (int argc, char **argv);

	/// Determina a função autocorrelação.
   virtual bool Go (float *_Re_data, int _NX, int _NY, int indice=1);
	
	/// Determina a função autocorrelação.
   virtual bool Go (TCMatriz2D< int > *img, int indice=1);
	
	/// Salva correlação em disco
	//virtual bool Write (std::string fname);

	/// Salva correlação e dados adicionais em disco (formato Liang)
	bool Writerzf (std::string fname);
	
private:
	/// Recebe a propria matriz (Im_data ou Re_data) e a nova dimensao,
	/// a matriz é realocada se o ponteiro for null ou se houve mudanca de dimensao. 
	bool RealocarMatrizSeNecessario( float*& matriz, int novoNX, int novoNY );
};

#endif
