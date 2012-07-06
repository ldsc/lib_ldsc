
#include <cmath>

#include <TSolverMatriz/TSMCGaussSeidel.h>

//---------------------------------------------------------------------------
//Construtor
//---------------------------------------------------------------------------
TSMCGaussSeidel::TSMCGaussSeidel (double ld):
TSolverMatriz ()
{
  limiteDenominador = ld;	//Atributo desta classe
  //cout<<", TSolverGaussSeidel("<<sizeof(TSolverGaussSeidel)<<")";
  tipoSolver = 6;		//Atributos das classes bases
  limiteIteracoes = 50;
  limiteErro = 0.0001;		//eps2
}

//---------------------------------------------------------------------------
//Destrutor
//---------------------------------------------------------------------------
TSMCGaussSeidel::~TSMCGaussSeidel ()
{
  //cout<<", TSolverGaussSeidel";
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
//        virtual TMath::TipoErro Go(TMatriz2D& A,TVetor& B,TVetor& X);
//TMath::eTipoErro TSMCGaussSeidel::Go(TMatriz2D& A,TVetor& B,TVetor& X)
//        virtual TVetor* Go(TMatriz2D*& A=0,TVetor*& B=0);    //Calcula e retorna a solução X do sistema
TVetor *
TSMCGaussSeidel::Go (TMatriz2D * A, TVetor * B)
{
  enum opType
  { opContinue, opConverge, opSingular, opError };
  opType operType = opContinue;	//enum opType operType = opContinue;
  TVetor Xold (A->GetNX ());	//TVetor Xold(numRows);
  double denom, dev, devMax;
  int i, j, iter = 0;
  //abaixo é novo
  TVetor *X = new TVetor (B->GetNX ());	//Cria vetor X do mesmo tamanho de B
  X->Constante (0);
  //fim da novidade
  // Normaliza a matriz A e o vetor B
  for (i = 0; i < A->GetNX (); i++)
    {
      denom = A->data2D[i][i];
      if (denom < limiteDenominador)	//eps1=limiteDenominador
	//return TMath::erroSingular;
	cout << "\nErro: matriz singular";
      return &TVetor ();
      B->data1D[i] /= denom;
      for (j = 0; j < A->GetNX (); j++)
	A->data2D[i][j] /= denom;
    }

  // Realiza as iteracoes de  Gauss-Seidel
  while (operType == opContinue)
    {
      for (i = 0; i < A->GetNX (); i++)
	{
	  Xold.data1D[i] = X->data1D[i];
	  X->data1D[i] = 0;
	  for (j = 0; j < A->GetNX (); j++)
	    if (j != i)
	      X->data1D[i] -= A->data2D[i][j] * X->data1D[j];
	  X->data1D[i] += B->data1D[i];
	}

      //Checa a convergencia
      devMax = fabs (Xold.data1D[0] - X->data1D[0]) / X->data1D[0];
      for (i = 1; i < A->GetNX (); i++)
	{
	  dev = fabs (Xold.data1D[i] - X->data1D[i]) / X->data1D[i];
	  devMax = (dev > devMax) ? dev : devMax;
	}
      if (devMax <= limiteErro)	//eps2=limiteErro
	operType = opConverge;
      else
	{
	  iter++;
	  if (iter > limiteIteracoes)
	    operType = opError;
	}
    }
  switch (operType)
    {
      //case opConverge:             return TMath::ok;
    case opConverge:
      return X;
      //case opSingular:             return TMath::erroSingular;
    case opSingular:
      cout << "\nErro: Matriz singular";
      X->Constante (0);
      return X;
      //case opError:                        return TMath::erroLimiteIteracoes;
    case opError:
      cout << "\nErro: opError";
      X->Constante (0);
      return X;
    default:
      return X;
    }
}
