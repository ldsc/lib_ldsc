#ifndef TSMCGaussSeidel_h
#define TSMCGaussSeidel_h

#include <TSolverMatriz/TSolverMatriz.h>

//#include "TOperacao.h"
//#include "TMath.h"

//---------------------------------------------------------------------------
//             CLASSE   TSolverGaussSeidel
//---------------------------------------------------------------------------
//O Gauss_Seidel ‚ semelhante ao Jacobi, a diferenca ‚ que ao calcular
//o valor de var para a linha j utiliza o valor de var[j] j  calculado.
//Deste modo o sistema converge mais rapidamente.
class TSMCGaussSeidel:public TSolverMatriz
{
  //Métodos
  double limiteDenominador;
public:
  double SetLimiteDenominador (double ld)
  {
    return limiteDenominador = ld;
  };

public:
  TSMCGaussSeidel (double ld = 0.001);	//???
  virtual ~ TSMCGaussSeidel ();
  //virtual TMath::eTipoErro Go(TMatriz2D& A,TVetor& B);//,TVetor& X);
  //virtual TMath::eTipoErro Go(TMatriz2D& A,TVetor& B,TVetor& X=TVetor()); //Executa
  //virtual void Go();
  virtual TVetor *Go (TMatriz2D * A = 0, TVetor * B = 0);	//Calcula e retorna a solução X do sistema
};
#endif
