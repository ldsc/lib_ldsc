
#ifndef CReconstrucaoBueno_h
#define CReconstrucaoBueno_h

#include<string>

// calculo da transformada de fourier (Singleton)
#include    <Reconstrucao/CReconstrucaoZhirong.h>

// calculo da transformada de fourier (Singleton)
#include    <FFT/CFFTPropriedades.h>

// ------------------------------------------------------------------------------
/**
 * @brief Reconstrução de imagens tridimensionais, usando método de Zhirong.
 * */
class CReconstrucaoBueno : public CReconstrucaoZhirong
{
   //  Atributos
public:
   int gt;  ///<  Tipo de gaussiana truncada, 1 ou 2

protected:
	/// Calcula Ry1D
  	virtual void Calculo_Ry1D ();

   /// salva imagem reconstruida em disco
	virtual bool SalvarImagemEmDisco (std::string);


public:
   //  Apenas define o valor de gt, padrão = 1
	/// Construtor
   CReconstrucaoBueno (std::string nomeArquivo = "entrada.cor",
                       int fatorAmp =	  3,
                       int npcz = 30,
                       double sp = 2.5,
                       int nx = 100,
                       int ny = 100,
                       int nz = 100,
                       int _gt = 1)
      : CReconstrucaoZhirong (nomeArquivo, fatorAmp, npcz, sp, nx, ny, nz), gt(_gt) {}

   /// Destrutor
   virtual ~ CReconstrucaoBueno (){}
};
#endif
