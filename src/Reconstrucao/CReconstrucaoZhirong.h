
#ifndef CReconstrucaoZhirong_h
#define CReconstrucaoZhirong_h

#include <string>
#include <sstream>

// calculo da transformada de fourier (Singleton)
#include    <Reconstrucao/CReconstrucao.h>

// calculo da transformada de fourier (Singleton)
#include    <FFT/CFFTPropriedades.h>

// dada um valor de q calcula u, sendo q a integral da equação normal e
// u o limite inferior da integral.
// extern int NP(double,double,double*);// calcula z=CurvaNormal(area)

#define Real_E1(x,y,z)	Re_data[(z)*NY*NX+(y)*NX+(x)]
#define Imag_E1(x,y,z)	Im_data[(z)*NY*NX+(y)*NX+(x)]
// inline Real_E1(int x,int y,int z){return Re_data[(z)*NY*NX+(y)*NX+(x)]}
// inline Imag_E1(int x,int y,int z){return Im_data[(z)*NY*NX+(y)*NX+(x)]}

// ------------------------------------------------------------------------------
/**
 * @brief Realiza reconstrução tridimensional usando método de liang.
 * */
class CReconstrucaoZhirong:public CReconstrucao
{
// Atributos
public:
	double *Re_data;///<  ponteiros para matrizes parte real 
	double *Im_data;///<  ponteiros para matrizes parte imaginaria
	  
protected:
    CFFTPropriedades * objetoFourier;	///< objeto FFT, realiza Operacoes da Transformada de Fourier

  // int sementeGeradorAleatorio;         // semente utilizada pelo gerador de números aleatórios
  int fatorAmplificacaoNumeroPontos;	///< fator de redução, se =1 pega todos os pontos (a imagem recontruída receberá este valor em comentario).
  double sizePixel;					///< Resolução utilizada na obtenção da imagem pelo microscópio (a imagem reconstruida receberá este valor em comentario).
  // se =2 pega a metade dos pontos.
  int numeroPontosCz;		///< numero total de dados na funcao correlacao
  int NX, NY, NZ;		///< dimensoes da matriz
  double porosidade;		///< porosidade a ser obtida
  double *Ry;			///< vetor autocovariancia normalizada Ry// usar TVetor
  double *Cz;			///< vetor correlação Cz// usar TVetor
  double yc;			///< limite utilizado para definicao da funcao de faze
  int i;			///< contadores
  int  j;			///< contadores
  int  k;			///< contadores
  int numeroPontosLidosArquivoCz;	///< numero de pontos realmente lidos do arquivo de disco
  // int u;                      // deslocamento u
  // double rz,ry;
  // double temp;                // variavel temporaria

  std::string nomeArquivoCz;	///< nome arquivo covariograma z
  std::string nomeArquivoRy;	///< nome arquivo autocorrelacao ry
  std::string fileName;
// FILE *fin;// ponteiro para arquivo de disco
// FILE *foutw,*foutr;// ponteiro arquivo disco ry, fpryw salva, e fpryr le
//   ifstream fin;
//   ofstream fout;
// Metodos
protected:
	/// leitura dos dados iniciais da linha de comando
  void Entrada_dados (int argc, char **argv);	
  // void Calculo_yc();// Calcula yc
  /// lê valores do arquivo Cz
  void LeituraArquivoCz ();	
  
  /// Calcula Ry1D
  virtual void Calculo_Ry1D ();
  	
  	/// Calcula Ry3D
  void Calcula_Ry3D_de_1D ();
  
  /// Determina Raiz Quadrada De Y2
  void RaizQuadradaDeY2 ();
  
  /// Resolve filtro nao linear
  void filtro_nao_linear ();	
  
  /// salva imagem reconstruida em disco
  virtual void SalvarImagemEmDisco (std::string);	

public:
	/// Construtor
	CReconstrucaoZhirong (std::string nomeArquivo = "entrada.cor", int fatorAmp = 3, int npcz = 30, double sp = 2.5, int nx = 100, int ny = 100, int nz = 100);

	/// Destrutor
    virtual ~ CReconstrucaoZhirong ();	
    
     /// Executa a reconstrucao
    virtual int Go ();		
     
      /// Calcula e salva o covariograma
    void Covariograma ();	
    // void MediaECovariancia();   // Verifica media e covariancia
    
    /// Retorna o nome do arquivo (.dbm) reconstruido.
    inline std::string getFileName() { return fileName; }
};
#endif
