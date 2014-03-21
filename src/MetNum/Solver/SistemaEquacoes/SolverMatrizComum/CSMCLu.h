#ifndef TSMCLU_h
#define TSMCLU_h

#include <TSolverMatriz/TSolverMatriz.h>

//#include "TOperacao.h"

//---------------------------------------------------------------------------
//º                             CLASSE TSMCLU
//---------------------------------------------------------------------------
//Resolve o sistema de equacoes utilizando a rotina de Jacobi.
//Jacobi é um sistema iterativo, lento, que calcula o valor da variavel var
//com base nos coeficientes da linha.
//Como resolve linha a linha, é necessária a realizacao de iteracoes
//até que o sistema convirja.
//Nos computadores mais novos com o processamento paralelo, voltou a ser
//utilizado.
//As iteracoes sao controladas por limiteIteracoes e limiteErro.
class TSMCLU:public TSolverMatriz
{
  //Métodos
public:
  //virtual void  Go();
  TSMCLU ();			//Construtor
  virtual ~ TSMCLU ();		//Destrutor

public:			//funções específicas

//Antes de chamar a LUDeterminant deve definir o valor do atributo rowSwapFlag
//        virtual  void LUInverse(TMatriz2D& A, TMatriz2D& InvA);//,TVetor& Index, int numRows);
//        virtual  double LUDeterminant(TMatriz2D& A);//, int numRows, int rowSwapFlag);
  double Determinante (TMatriz2D * &A);	//, int numRows); //colocar em TM2DOperacoesBasicas
    TMath::eTipoErro Inversa (TMatriz2D * &A);	//, int numRows);//MatInverse //colocar em TM2DOperacoesBasicas

  //virtual TMath::eTipoErro Go(TMatriz2D& A,TVetor& B,TVetor& X=TVetor()); //Executa
  virtual TVetor *Go (TMatriz2D * A = 0, TVetor * B = 0);	//Calcula e retorna a solução X do sistema
};
#endif
