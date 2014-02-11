
#include <cmath>
using namespace std;

#include <MetNum/Interpolacao/IntMetNum/CIMNExata.h>

// ---------------------------------------------------------------------------
// Construtor
CIMNExata::CIMNExata ()
{
  alfa = 0;
  beta = 1;
  // cout<<", CIntMetNumExata("<<sizeof(CIntMetNumExata)<<")";
}

// ---------------------------------------------------------------------------
// Destrutor
CIMNExata::~CIMNExata ()
{
  //  cout<<", CIntMetNumExata";
}

// ---------------------------------------------------------------------------
// observacao:
//   Para interpolacao do tipo CIntMetNumExata, a velocidade u nao pode ser zero.
//   Se a velocidade for zero, peclet=0,c3=0 e vai ocorrer um erro de
//   divisao por zero e estouro.
// Pode-se futuramente incluir aqui um tratamento de excessao
// para o estouro da pilha.
void
CIMNExata::AlfaBeta (double &u, double &dxc, double &ro,
				double &dif)
{
  peclet = u * dxc * ro / dif;	// v=0 peclet=0
  c3 = pow (M_E, peclet) - 1.0;	// c1=e^peclet-1
  c4 = pow (M_E, (peclet / 2.0));	// c2=e^(peclet/2)
  alfa = 0.5 - (c4 - 1.0) / c3;
  beta = peclet * c4 / c3;
}
