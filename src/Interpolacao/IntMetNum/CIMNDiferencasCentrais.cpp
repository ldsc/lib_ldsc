
#include <Interpolacao/IntMetNum/CIMNDiferencasCentrais.h>

// ---------------------------------------------------------------------------
// Construtor
CIMNDiferencasCentrais::CIMNDiferencasCentrais ()
{
  alfa = 0;
  beta = 1.0;
  // cout<<", CIMNDiferencasCentrais("<<sizeof(CIMNDiferencasCentrais)<<")";
}

// ---------------------------------------------------------------------------
// Destrutor
CIMNDiferencasCentrais::~CIMNDiferencasCentrais ()
{
  // cout<<", CIMNDiferencasCentrais";
}

// ---------------------------------------------------------------------------
// funcao de calculo de alfa e beta.
// Esta funcao é redefinida .
// observe que aqui nao realiza nenhum cálculo.
// pois para o CIMNDiferencasCentrais alfa e beta sao constantes
// e foram definidos no construtor
void
CIMNDiferencasCentrais::AlfaBeta
  (double &u, double &dxc, double &ro, double &dif)
{
}
