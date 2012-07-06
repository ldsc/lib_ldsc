
#include <iostream>
using namespace std;

#include <Interpolacao/IntMetNum/CIntMetNum.h>

// ---------------------------------------------------------------------------
// Variáveis estaticas precisam ser definidas abaixo da classe

	double  CIntMetNum::peclet = 0.0;

	 double  CIntMetNum::pe2 = 0.0;

	 double  CIntMetNum::c3 = 0.0;

	 double  CIntMetNum::c4 = 0.0;

	 int  CIntMetNum::tipo_de_interpolacao = 1;	// [32];

// ---------------------------------------------------------------------------
// sobrecarga operadores cin>> e cout<<   template
// informa ao template que é para criar esta funcao para esta classe
// ostream& operator <<(ostream&op,CIntMetNum*&c);
// istream& operator >>(istream&op,CIntMetNum*&c);

/*
// ---------------------------------------------------------------------------
// Com a definicao da função genérica a partir do template, nao temos
// a necessidade de digitar as funcoes abaixo.
// Funcao sobrecarregada de entrada dos dados (cin>>objetoclima)
// Como é uma funcao friend nao entra o nome da classe::
istream& operator >> (istream& op,CIntMetNum*& c)
{
 c->f_entrada();
 return op;
}
// ---------------------------------------------------------------------------
// Funcao sobrecarregada de saida dos dados (cout<<objetoclima)
// Como é uma funcao friend nao entra o nome da classe::
ostream& operator << (ostream& op,CIntMetNum*& c)
{
 c->f_saida();
 return op;
}
*/
// ---------------------------------------------------------------------------
// construtor
CIntMetNum::CIntMetNum ()
{
  // cout<<"\n\tCriou Objeto base CIntMetNum("<<sizeof(CIntMetNum)<<")";
}

// ---------------------------------------------------------------------------
// destrutor
CIntMetNum::~CIntMetNum ()
{
  // cout<<"\n\tDeletou Objeto base CIntMetNum";
}

// ---------------------------------------------------------------------------
// Entrada dos atributos alfa e beta
void CIntMetNum::Entrada ()
{
  // f_carimbo();
  cout << "\n\tAtributos classe base CIntMetNum"
   << "\n\talfa:";
  cin >> alfa;
  cout << "\t\tbeta:";
  cin >> beta;
  cin.get ();
}

// ---------------------------------------------------------------------------
// Saida dos atributos alfa e beta.
void CIntMetNum::Saida ()
{
  cout 
	<< "\n\ttipo interpolacao=" << tipo_de_interpolacao
   << "\n\talfa=" << alfa
   << "\n\tbeta=" << beta;
}
