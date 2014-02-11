
#include <MetNum/Interpolacao/IntMetNum/CIMNWuds.h>

// ---------------------------------------------------------------------------
// construtor
CIMNWuds::CIMNWuds ()
{
  alfa = 0;
  beta = 1;
  // cout<<", CIMNWuds("<<sizeof(CIMNWuds)<<")";
}

// ---------------------------------------------------------------------------
// destrutor
CIMNWuds::~CIMNWuds ()
{
  // cout<<", CIMNWuds";
}

// ---------------------------------------------------------------------------
// funcao de calculo de alfa e beta.
// observe que é definida na classe base e aqui redefinida.
// os parametros sao passados por referencia por ser mais rápido
void
CIMNWuds::AlfaBeta (double &u, double &dxc, double &ro,
			       double &dif)
{
  peclet = u * dxc * ro / dif;
  pe2 = peclet * peclet;
  alfa = pe2 / (10.0 + 2.0 * pe2);
  beta = (1.0 + 0.005 * pe2) / (1.0 + 0.05 * pe2);
}
