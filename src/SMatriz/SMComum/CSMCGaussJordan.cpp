
#include <cmath>

#include <TSolverMatriz/TSMCGaussJordan.h>

//-----------------------------------------------------------------------
//Funcao Construtora
//-----------------------------------------------------------------------
TSMCGaussJordan::TSMCGaussJordan ():TSolverMatriz ()
				//:Gauss_Seidel(naux)
{
  //tipoSolver=6;
}

//-----------------------------------------------------------------------
//Funcao Destrutora
//-----------------------------------------------------------------------
TSMCGaussJordan::~TSMCGaussJordan ()
{
  deletaAtributos ();
}

//-----------------------------------------------------------------------
//Funcao  deletaAtributos
//-----------------------------------------------------------------------
void
TSMCGaussJordan::deletaAtributos ()
{
  if (colIndex != NULL)
    delete[]colIndex;
  if (rowIndex != NULL)
    delete[]rowIndex;
  if (pivotIndex != NULL)
    delete[]pivotIndex;
}

//-----------------------------------------------------------------------
//Funcao Go
//-----------------------------------------------------------------------
//        virtual TMath::TipoErro Go(TMatriz2D& A,TVetor& B,TVetor& X);
//X não é usado
//TMath::eTipoErro TSMCGaussJordan::Go(TMatriz2D& A, TMatriz2D& B,TVetor& X)
//TMath::eTipoErro TSMCGaussJordan::Go(TMatriz2D& A, TVetor& B,TVetor& X)
TVetor *
TSMCGaussJordan::Go (TMatriz2D * A, TVetor * B)
{
  int i, j, k, n, m;
  int row, col;
  double large, z, oneOverPiv;
  deletaAtributos ();
//Abaixo é novo preenche a matriz BB com o vetor B
  TMatriz2D BB (B->GetNX (), B->GetNX ());
  for (int kk = 0; kk < B->GetNX (); kk++)
    BB.data2D[kk][0] = B->data1D[kk];
//Fim da novidade
  rowIndex = new int[A->GetNX ()];	//conferir->
  colIndex = new int[A->GetNY ()];
  pivotIndex = new int[A->GetNY ()];

  for (i = 0; i < A->GetNX (); i++)	//initialize the row and column indices
    {
      rowIndex[i] = i;
      colIndex[i] = i;
    }

  for (i = 0; i < A->GetNX (); i++)	//initialize the pivot index array
    pivotIndex[i] = -1;

  for (i = 0; i < A->GetNX (); i++)
    {
      large = 0;
      for (j = 0; j < A->GetNX (); j++)
	if (pivotIndex[j] != 0)
	  for (k = 0; k < A->GetNX (); k++)
	    {
	      if (pivotIndex[k] == -1)
		{
		  if (fabs (A->data2D[j][k]) >= large)
		    {
		      large = fabs (A->data2D[j][k]);
		      row = j;
		      col = k;
		    }
		}
	      else if (pivotIndex[k] > 0)
		{
		  deletaAtributos ();
		  //return TMath::erroSingular;
		  cout << "\nErro: Matriz singular";
		  return &TVetor ();
		}
	    }
      pivotIndex[col] += 1;
      if (row != col)
	{
	  for (n = 0; n < A->GetNX (); n++)
	    TOperacao::swapDouble (A->data2D[row][n], A->data2D[col][n]);

	  for (n = 0; n < A->GetNY (); n++)	//?????????
	    TOperacao::swapDouble (BB.data2D[row][n], BB.data2D[row][n]);	//verificar efeito sobre vetor B??
	}
      rowIndex[i] = row;
      colIndex[i] = col;
      if (fabs (A->data2D[col][col]) < 1.e-10)
	{
	  deletaAtributos ();
	  //return TMath::erroSingular;
	  cout << "\nErro: Matriz singular";
	  return &TVetor ();
	}
      oneOverPiv = 1 / A->data2D[col][col];
      A->data2D[col][col] = 1;

      for (n = 0; n < A->GetNX (); n++)
	A->data2D[col][n] *= oneOverPiv;
      for (n = 0; n < A->GetNY (); n++)
	BB.data2D[col][n] *= oneOverPiv;

      for (m = 0; m < A->GetNX (); m++)
	if (m != col)
	  {
	    z = A->data2D[m][col];
	    A->data2D[m][col] = 1;

	    for (n = 0; n < A->GetNX (); n++)
	      A->data2D[m][n] -= A->data2D[col][n] * z;
	    for (n = 0; n < A->GetNY (); n++)
	      BB.data2D[m][n] -= BB.data2D[col][n] * z;
	  }
    }

  for (n = A->GetNX () - 1; n >= 0; n--)
    {
      if (rowIndex[n] != colIndex[n])
	for (k = 0; k < A->GetNX (); k++)
	  TOperacao::swapDouble (A->data2D[k][rowIndex[n]],
				 A->data2D[k][colIndex[n]]);
    }
  deletaAtributos ();
  //return TMath::ok;
  return B;
}
