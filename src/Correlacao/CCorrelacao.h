#ifndef CCorrelacao_h
#define CCorrelacao_h

#include <string>

// ---------------------------------------------------------------------
//               DEFINES
// ---------------------------------------------------------------------
// Observe que nesta classe e nas classes derivadas os nomes 
// Re_data, Im_data e NX precisam ser preservados.
#define MatrizReal(x,y)	Re_data [(y)*NX + (x)]
#define MatrizImag(x,y)	Im_data [(y)*NX + (x)]

class CMatriz2D;

/**
 * @brief Classe utilizada para determinação da curva de auto-correlação.
 * Adaptação desenvolvida sobre o código do Liang Zhirong
 * 
 * Tarefa: 
 * Como é um vetor deve ser herdeiro de CVetor.
 * Passar a usar FFT compatível com o formato de CMatriz pm[i][j]
*/
class CCorrelacao
{
   protected:
   //Atributos
	/// Dimensão nx da imagem 2D
	// int NX;
	
	/// Dimensão ny da imagem 2D
	// int NY;
	
	/// Menor dimensao da imagem 2D
	// int dimensaoMinima;
	
	/// Deslocamento máximo da imagem usado para calculo correlacao
	int deslocamentoMaximo;	
	
	/// Matriz 2D da parte real
	// float * Re_data;
	
	/// Matriz 2D da parte imaginária
	// float * Im_data;
	
	/// Ponteiro para Matriz 2D
	// CMatriz2D *pm2d;
	
   public:
   	/// Vetor correlacao
	float *correlacao;
   
   //Métodos
   public:
   	/// Construtor, recebe a dimensao_vetor_correlacao ( default = 320 = 640/2 )
   	CCorrelacao ( int dimensao_vetor_correlacao = 320 );
   	
	/// Destrutor
	virtual ~ CCorrelacao ();
   
  	/// Determina a função autocorrelação. Recebe ponteiro para vetor imagem e as dimensões da imagem
  	virtual bool Go (float *Re_data, int NX, int NY) = 0;
  	
	/// Determina a função autocorrelação. Recebe ponteiro para CMatriz2D.
  	virtual bool Go (CMatriz2D *img) = 0;
  	
	/// Salva em disco os dados de correlação (.cor).
  	bool Write (std::string fileName);
	
	/// Retorna dimensao do vetor correlacao (igual ao deslocamentoMaximo).
	inline int size()		
				{	return deslocamentoMaximo;	}
};

#endif

