#ifndef CFPHermite_h
#define CFPHermite_h

#include "Base/CFuncao.h"

#include "Estatistica/CEFatorial.h"

// ------------------------------------------------------------------------------
// Objeto CFPHermite
// ------------------------------------------------------------------------------
/**
 * @brief Representa função de Bessel.
 * @author André Duarte Bueno
 * */
class CFPHermite : public CFuncao
{

// atributos
public:
  int m;	///< Parametro da funcao Hermite, passada pela funcao SetM

private:
  // double cy1,cy2;          // parametros utilizados pela funcao calculados aqui

  double yc;	///< Parametro recebido no construtor,define o uso de cy1 ou cy2

  CEFatorial *fatorial;		///< Objeto externo utilizado aqui, determina o fatorial

  // atributos aceleradores de performance
  double auxiliar1;  ///< auxiliar calcula o divisor comum, para qualquer y 
  double auxiliar2;  ///< auxiliar calcula o divisor comum, para qualquer y 

// metodos
  /// Funcao auxiliar, determina o polinômio de Hermite.
  double Hermite (int m, double y);	

public:

	/// Construtor
    CFPHermite (double _porosidade, double _yc, int _numeroPontosSerie);	

    /// Destrutor
    virtual ~ CFPHermite ()
  {
	  delete fatorial; 		// deleta objeto fatorial
  }

//       virtual double Go(double x);     
  /// Calcula a funcao em si y=f(x)
  virtual double Go (double x, double y = 0);	// calcula a funcao em si y=f(x) ou z=f(x,y)
  
  /// Seta o atributo M
  void M (int _m)
  {
    m = _m;
  }
};
#endif
