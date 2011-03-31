#ifndef CFPHermiteRec_h
#define CFPHermiteRec_h

#include "Base/CFuncao.h"
#include "Estatistica/CEFatorial.h"

// ------------------------------------------------------------------------------
// Objeto CFPHermiteRec
// ------------------------------------------------------------------------------
/**
 * @brief Representa função polinômio de PHermiteRec.
 * @author André Duarte Bueno
 * */
class CFPHermiteRec : public CFuncao
{
// atributos
public:
  int m;	///< Parametro da funcao Hermite, passada pela funcao M

private:
	
  double yc;		/// Parametro recebido no construtor,define o uso de cy1 ou cy2
  CEFatorial *fatorial;	///< Objeto externo, determina o fatorial

  double auxiliar1; ///< Atributos aceleradores de performance
  double auxiliar2;	///< Atributos aceleradores de performance

  /// Determina o polinômio de Hermite
  double Hermite (int m, double y);	

public:
	/// Construtor
    CFPHermiteRec (double _porosidade, double _yc, int _numeroPontosSerie);	

    /// Destrutor
    virtual ~ CFPHermiteRec ()
  {
	  delete fatorial;					// deleta objeto fatorial
  }			
  
//       virtual double Go(double x);    
   /// Calcula a funcao em si y=f(x)
  virtual double Go (double x, double y = 0);	
  
  /// Determina o valor de M
  void M (int _m)
  {
    m = _m;
  }
};
#endif
