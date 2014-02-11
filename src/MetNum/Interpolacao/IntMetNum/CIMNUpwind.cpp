
#include <MetNum/Interpolacao/IntMetNum/CIMNUpwind.h>

// ---------------------------------------------------------------------------
// construtor
CIMNUpwind::CIMNUpwind ()
{
  alfa = 0.5;
  beta = 1.0;
  // cout<<", Upwind("<<sizeof(Upwind)<<")";
}

// ---------------------------------------------------------------------------
// destrutor
CIMNUpwind::~CIMNUpwind ()
{
  // cout<<", Upwind";
}

// ---------------------------------------------------------------------------
// funcao de calculo de alfa e beta.
// Esta funcao é redefinida .
// observe que aqui nao realiza nenhum cálculo.
// pois para o upwind alfa e beta sao constantes e foram definidos no construtor
void
CIMNUpwind::AlfaBeta
  (double &u, double &dxc, double &ro, double &dif)
{
}
