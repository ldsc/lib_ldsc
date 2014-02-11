#ifndef TSMCGaussJordan_h
#define TSMCGaussJordan_h

#include <TSolverMatriz/TSolverMatriz.h>

//#include "TOperacao.h"
//#include "TMath.h"

//---------------------------------------------------------------------------
//º                             CLASSE TSMCGaussJordan
//---------------------------------------------------------------------------
//Resolve o sistema de equacoes utilizando a rotina de Jacobi.
//Jacobi é um sistema iterativo, lento, que calcula o valor da variavel var
//com base nos coeficientes da linha.
//Como resolve linha a linha, é necessária a realizacao de iteracoes
//até que o sistema convirja.
//Nos computadores mais novos com o processamento paralelo, voltou a ser
//utilizado.
//As iteracoes sao controladas por limiteIteracoes e limiteErro.
class TSMCGaussJordan:public TSolverMatriz
{
  //Atributos
protected:
  int *rowIndex;
  int *colIndex;
  int *pivotIndex;

  //Métodos
private:
  void deletaAtributos ();
public:
    TSMCGaussJordan ();		//construtor
    virtual ~ TSMCGaussJordan ();	//destrutor
  //protected:
  //  MatErrType GaussJordan(Matrix& A, Matrix& B,int numRows, int numCols);
  //virtual TMath::eTipoErro Go(TMatriz2D& A, TMatriz2D& B);
  //virtual TMath::eTipoErro Go(TMatriz2D& A,TVetor& B,TVetor& X);
  //virtual TMath::eTipoErro Go(TMatriz2D& A,TVetor& B,TVetor& X=TVetor()); //Executa
  virtual TVetor *Go (TMatriz2D * A = 0, TVetor * B = 0);	//Calcula e retorna a solução X do sistema

};
#endif
