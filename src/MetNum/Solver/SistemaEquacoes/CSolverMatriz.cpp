/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
		  	[LDSC].
@author:          André Duarte Bueno
@file:             CSolverMatriz.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cmath>
// #include <math.h>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <MetNum/Solver/SistemaEquacoes/CSolverMatriz.h>
using namespace std;

// -----------------------------------------------------------------------
// Classe TSMComum
// -----------------------------------------------------------------------
// ---------------------------------------------------------------------------
// Funcao estatica de selecao do tipo de solver
// retorna um ponteiro para o ultimo objeto criado
// para usala, cria no programa principal um ponteiro para Solver
// Ex:
//               Solver* ptr_solver=Solver::f_seleciona_solver(n,dimensao);
CSolverMatriz * CSolverMatriz::Selecao (int tipo)
{
/*
  // f_carimbo();
  cout<<"\n\n\t\tQual o  SOLVER a ser utilizado?"
		<<"\n\tCSolverTDMA................1"
		<<"\n\tCSolverJacobi..............2"
		<<"\n\tCSolverGaussSeidel.........3"
		<<"\n\tCSolverSOR.................4"
		<<"\n\tCSolverLU..................5"
		<<"\n\tCSolverGaussJordan.........6:";
  cin>>tipo_de_solver;// =getche();
  cin.get();	// pega o retorno de carro.
  switch(tipo_de_solver)
	 {           // SOLVER(numero_equacoes);
	 case 1:		ptr=new CSolverTDMA(naux);  	 	break;
	 case 2: 	ptr=new CSolverJacobi(naux);	 	break;
	 case 3: 	ptr=new CSolverGaussSeidel(naux);break;
	 case 4:    ptr=new CSolverSOR(naux);			break;
	 case 5:    ptr=new CSolverLU(naux);			break;
	 case 6:    ptr=new CSolverGaussJordan(naux);break;
	 default:	ptr=new CSolverTDMA(naux);	 		break;
	 }
	 f_memoria(ptr,"solver");		// verifica se não o objeto foi alocado
*/
// ptr;                                  // ponteiro para ultimo solver criado
  return (0);
}

// -----------------------------------------------------------------------
// Funcao Determinante
// -----------------------------------------------------------------------
double
CSolverMatriz::Determinante (TCMatriz2D< int > * &A)	// ;// , int numRows);
{
/*
BUG: a matriz deve ser double e não int
  unsigned int i;
  err = LUDecomp(A);// , Index, A.GetNX(), rowSwapFlag);
  if (err != CMath::ok)     		// matErr_None
	 return CMath::RESULTADO_ERRADO;
  determinante = (double)rowSwapFlag;	// Atributo da classe base
  for (i = 0; i < A->GetNX(); i++)		// depois da decomposição de LU
	 determinante *= A->data2D[i][i];    // o determinante é a multiplicação da linha de identidade.
*/
  return determinante;
}

// -----------------------------------------------------------------------
// Funcao Inversa
// -----------------------------------------------------------------------
// Deve pertencer a   TM2DOperacoesBasicas
CMath::EMathError CSolverMatriz::Inversa (TCMatriz2D< int > * &A)	// ;// , int numRows);// MatInverse
{
/*  CVetor colVect(A->GetNX());
  unsigned int i, j;

  err = LUDecomp(A) ;
  if (err != CMath::ok)
	 return err;

  for (j = 0; j < A->GetNX(); j++)
  { for (i = 0; i < A->GetNX(); i++)
		colVect.data1D[i] = 0;
	 colVect.data1D[j] = 1;
	 LUBackSubst(A, &colVect);// , Index, A.GetNX(),
	 for (i = 0; i < A->GetNX(); i++)
		A->data2D[i][j] = colVect.data1D[i];
  }
*/
  return CMath::ok;
}

// -----------------------------------------------------------------------
// Funcao LUBackSubst
// -----------------------------------------------------------------------
void
CSolverMatriz::LUBackSubst (TCMatriz2D< int > * &A, CVetor * &B)	// ;// CVetor& Index,int A.GetNX(),
{
/*
BUG: a matriz deve ser double e não int
  unsigned int i, j;
  int idx, k = -1;
  double sum;

  for (i = 0; i < A->GetNX(); i++)
  {
	 idx = index->data1D[i];
	 sum = B->data1D[idx];
	 B->data1D[idx] = B->data1D[i];
	 if (k > -1)
		for (j = k; j < i; j++)
		  sum -= A->data2D[i][j] * B->data1D[j];
	 else if (sum != 0)
		k = i;
	 B->data1D[i] = sum;
  }
  for (i = A->GetNX() - 1; i >= 0; i--)
  {
	 sum = B->data1D[i];
	 for (j = i+1; j < A->GetNX(); j++)
		sum -= A->data2D[i][j] * B->data1D[j];
	 B->data1D[i] = sum / A->data2D[i][i];
  }
*/
}

// -----------------------------------------------------------------------
// Funcao LUDecomp
// -----------------------------------------------------------------------
/*La na funcao LUDecomp {antes era LUDecomp(A), Index, numRows, rowSwapFlag);}
CVetor Index(A.GetNX());       		// A.GetNX()= numRows
int rowSwapFlag;
*/
CMath::EMathError CSolverMatriz::LUDecomp (TCMatriz2D< int > * &A)
{
/*
BUG: a matriz deve ser double e não int
  unsigned int i, j, k;// aqui mudei de int para unsigned int verificar
  int iMax;
  double large, sum, z, z2;

  if(index)
	delete index;
  index = new CVetor ( A->GetNX() );
  CVetor scaleVect( A->GetNX() ); // A.GetCols(); conferir se é NX ou NY??

  //  initialize row interchange flag
  rowSwapFlag = 1;

  //  loop to obtain the scaling element
  for (i = 0; i < A->GetNX(); i++)
	{
	 large = 0;
	 for (j = 0; j < A->GetNX(); j++)
	  {
		z2 = fabs( A->data2D[i][j] );
		large = (z2 > large) ? z2 : large;
	  }
	 //  no non-zero large value? then exit with an error code
	 if (large == 0)
		return CMath::erroSingular;// matErr_Singular
	 scaleVect.data1D[i] = 1 / large;
	}
  for (j = 0; j < A->GetNX(); j++)
	{
	for (i = 0; i < j; i++)
	 {	sum = A->data2D[i][j];
		for (k = 0; k < i; k++)
		  sum -= A->data2D[i][k] * A->data2D[k][j];
		A->data2D[i][j] = sum;
	 }
	large = 0;
	for (i = j; i < A->GetNX(); i++)
	 {	sum = A->data2D[i][j];
		for (k = 0; k < j; k++)
		  sum -= A->data2D[i][k] * A->data2D[k][j];
		A->data2D[i][j] = sum;
		z = scaleVect.data1D[i] * fabs(sum);
		if (z >= large)
		{ large = z;
		  iMax = i;
		}
	 }
	if (j != iMax)
	 {
	 for (k = 0; k < A->GetNX(); k++)
	  {
		  z = A->data2D[iMax][k];
		  A->data2D[iMax][k] = A->data2D[j][k];
		  A->data2D[j][k] = z;
	  }
	  rowSwapFlag *= -1;
	  scaleVect.data1D[iMax] = scaleVect.data1D[j];
	 }
	index->data1D[j] = iMax;
	if (A->data2D[j][j] == 0)
		A->data2D[j][j] = CMath::EPSILON;
	if (j != A->GetNX())
	 {
		z = 1 / A->data2D[j][j];
		for (i = j+1; i < A->GetNX(); i++)
		  A->data2D[i][j] *= z;
	 }
  }
*/
  return CMath::ok;
}
