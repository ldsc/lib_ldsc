#ifndef CIGauss_h
#define CIGauss_h

#include <MetNum/Integral/CIntegral.h>

// ------------------------------------------------------------------------------
// Objeto CIGauss
// ------------------------------------------------------------------------------
/**
 * @brief Calculo de integral pelo método de Gauss.
 * 
 *  A funcao Go() calcula a integral de uma funcao passada. 
 * Inclui o atributo  numeroSubIntervalos, que é
 * utilizado para se dividir a região a ser calculada em uma série de subintervalos.
 * A função AreaSubintervalo determina a área de um subintervalo, é reescrita
 * para as diversas classes herdeiras.
 * 
 * */

class CIGauss : public CIntegral
{

// atributos

protected:

  int numeroSubIntervalos;	///< número de sub-intervalos

  double areaSubIntervalo;	///< area de cada Sub Intervalo

  double inicioSubIntervalo;   ///< inicio do subintervalo
  double fimSubIntervalo;	///< fim do subintervalo

  double dxDiv2;		///< dx dividido por dois 

// metodos
protected:

	/// Calcula dx
    virtual void Dx ()
  {
    dx = (limiteSuperior - limiteInferior) / (numeroPontos);
  }

  /// Calcula a área de um subintervalo
  virtual void CalculaAreaSubIntervalo ()
  {
  }				

public:

	/// Construtor
CIGauss (CFuncao * &_funcao):CIntegral (_funcao)
  {
    numeroSubIntervalos = 10;
  }

  /// Destrutor
  virtual ~ CIGauss ()
  {
  }				

/// Calcula a integral
  virtual double Go (double x = 1, double y = 0);	

  /// Seta o número de subintervalos
  void NumeroSubIntervalos (int nsi)
  {
    numeroSubIntervalos = nsi;
  }

};
#endif
