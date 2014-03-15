#ifndef CIntegral_h
#define CIntegral_h

#include <Base/CSolver.h>
#include <Base/CFuncao.h>

// ------------------------------------------------------------------------------
// Objeto CIntegral
// ------------------------------------------------------------------------------

/**
 * @brief Calcula a integral numerica de uma funcao passada. 
 * 
 * A funcao SetParametros e utilizada para passagem dos parametros da integral
 * a funcao Go() calcula a integral.
 * 
 * Herdeira de CFuncao e CSolver.
 * */

class CIntegral : public CFuncao, public CSolver
{

  // atributos
protected:

  CFuncao * funcao;		///< Ponteiro para objeto funcao

  double dx;			///< Intervalo de espaço

  double limiteInferior; ///< limite inferior
  double  limiteSuperior; ///< limite superior

  int numeroPontos;		///< Número de pontos a serem utilizados (deve ser par)

  double resultado;		/// Resultado da integral

  // metodos
protected:

	/// Calculo de dx
    virtual void Dx ()
  {
    dx = (limiteSuperior - limiteInferior) / (numeroPontos - 1);
  }

  // virtual void Dx(){dx=(limiteSuperior-limiteInferior)/(numeroPontos);};
  // para simpson n-1 para gauss n?

public:

	/// Construtor
  CIntegral (CFuncao * _funcao);	

  
  /// calcula a integral, funcao pura
  virtual double Go (double x = 1, double y = 0)	// ordem dos parametros ruim???
  {
    limiteInferior = y;

    limiteSuperior = x;

    Dx ();
    // Aqui abaixo, nas classes herdeiras entra o cálculo da integral em sí

    return 1;
  }

  // void SetLimiteInferior(double limInferior){limiteInferior=limInferior;Dx();};
  // void SetLimiteSuperior(double limSuperior){limiteSuperior=limSuperior;Dx();};

  /// Seta número de pontos
  void NumeroPontos (int numPts)
  {
    numeroPontos = numPts;
    if (numeroPontos < 3)
      numeroPontos = 3;		// numero minimo de pontos
    if ((numeroPontos % 2) == 0)
      numeroPontos++;		// preciso ter um numero impar de pontos

    Dx (); // recalcula dx
  }

};
#endif

//       virtual double Go(double x=0,double y=0)=0;     // calcula a funcao em si y=f(x)
