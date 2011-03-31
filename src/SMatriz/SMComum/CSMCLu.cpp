
#include <TSolverMatriz/TSMCLU.h>

//-----------------------------------------------------------------------
//Funcao Construtora
//-----------------------------------------------------------------------
TSMCLU::TSMCLU ():TSolverMatriz ()
{
  tipoSolver = 5;
}

//-----------------------------------------------------------------------
//Funcao Destrutora
//-----------------------------------------------------------------------
TSMCLU::~TSMCLU ()
{
}

//-----------------------------------------------------------------------
//Funcao LUInverse
//-----------------------------------------------------------------------
TMath::eTipoErro TSMCLU::Inversa (TMatriz2D * &A)	//, int numRows);//MatInverse //colocar em TM2DOperacoesBasicas
//void TSMCLU::LUInverse(TMatriz2D& A, TMatriz2D& InvA)//,TVetor& Index, int numRows)
{				//Criar InvA
  TVetor
  colVect (A->GetNX ());
  int
    i,
    j;
  for (j = 0; j < A->GetNX (); j++)
    {
      for (i = 0; i < A->GetNX (); i++)
	colVect.data1D[i] = 0;
      colVect.data1D[j] = 1;
      //Obs: Abaixo colVect é passado como referencia
      LUBackSubst (A, &colVect);	//LUBackSubst(A, Index, numRows, colVect);
      for (i = 0; i < A->GetNX (); i++)
	//InvA.data2D[i][j] = colVect.data1D[i];
	A->data2D[i][j] = colVect.data1D[i];
    }
  //fazer A=InvA
}

//-----------------------------------------------------------------------
//Funcao LUDeterminant
//-----------------------------------------------------------------------
//Antes de chamar a LUDeterminant deve definir o valor do atributo rowSwapFlag
//double TSMCLU::LUDeterminant(TMatriz2D& A)//, int numRows,int rowSwapFlag)
double
TSMCLU::Determinante (TMatriz2D * &A)	//, int numRows); //colocar em TM2DOperacoesBasicas
{
  double result = (double) rowSwapFlag;
  for (int i = 0; i < A->GetNX (); i++)	//for (int i = 0; i < numRows; i++)
    result *= A->data2D[i][i];
  return result;
}

//Executa
TVetor *
TSMCLU::Go (TMatriz2D * A, TVetor * B)
//TMath::eTipoErro TSMCLU::Go(TMatriz2D& A,TVetor& B,TVetor& X)
{
  err = LUDecomp (A);		//realiza a decomposição
  if (err == TMath::ok)		//se ok
    LUBackSubst (A, B);		//LUBackSubst(MatA, Index, numRows, BB);conferir
  return B;			//retornava o erro
/*Codigo original
  Err = MatOp.LUDecomp(MatA, Index, numRows, dFlag);
  if (Err == MatrixOp::matErr_None)
  {
	 MatOp.LUBackSubst(MatA, Index, numRows, B2);
*/
}
