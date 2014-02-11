
#ifndef CIntBarycentric_h
#define CIntBarycentric_h

#include <MetNum/Interpolacao/CInterpolacao.h>

/*
--------------------------------------------------------------------
                          Classe CIntBarycentric
--------------------------------------------------------------------
*/
/**
 * @brief Utiliza interpolação usando CIntBarycentric.
 * */
class CIntBarycentric : public CInterpolacao
{
private:

  double *wt;

  int calcWtFlag;

public:
  /// Construtor
    CIntBarycentric ():wt (0), calcWtFlag (1)
  {
  }

  /// Destrutor
  virtual ~ CIntBarycentric ()
  {
    if (wt)
      delete [] wt;
  }

  //Funções set
  //void Setwt(        double* _wt)    {       if(wt)  delete wt;      wt = _wt;};

  /// Realiza a interpolação, função da classe base (herdada).
  virtual double Go (double *xarr, double *yarr, int n, double x);

protected:


  /**
 * @brief Função que calcula a tabela auxiliar é diferente da da classe base, 
 * recebe um xarr e não um double h por isso não é virtual.
 * void GeraTabelaAuxiliar(double* xarr, double* yarr,int n);
   * */
  void GeraTabelaAuxiliar (double *xarr, int n);

public:
  /// Função auxiliar	
  static double Barycentric
    (double *xarr, double *yarr, double *wt, int calcWtFlag, int n, double x);

};

#endif
