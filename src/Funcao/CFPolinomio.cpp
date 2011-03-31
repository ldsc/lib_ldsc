#include <iostream>

#include <Base/COperacao.h>

#include <Funcao/CFPolinomio.h>

using namespace std;

// ---------------------------------------------------------------------------
// Atributos de classe (estaticos) devem ser definidos abaixo da classe
double
  CFPolinomio::xx = 0.0;

double
  CFPolinomio::xxx = 0.0;

// ---------------------------------------------------------------------------
// Construtor, aloca espaco grau+1 para p
// incrementa o numero de polinomios criados
CFPolinomio::CFPolinomio (unsigned int n)
{
  grau = n;			// armazena o grau do polinomio

  p = new double[n + 1];	// aloca espaco na memoria para poly

  COperacao::TestaAlocacao (p, "CFPolinomio");	// testa alocacao.

  Zera ();			// inicializa atributos p[ii]
}

// ---------------------------------------------------------------------------
// Destrutor, deleta p
// decrementa o numero de polinomios
CFPolinomio::~CFPolinomio ()
{
  if (p)
    delete [] p;   // deleta vetor
}

// ---------------------------------------------------------------------------
// Inicializa valores de p[ii] em zero
// observe entretanto que p[0] é definido como 1.
void
CFPolinomio::Zera ()
{
  p[0] = 0;
  for (int ii = 1; ii <= grau; ii++)
    p[ii] = 0;
}

// ---------------------------------------------------------------------------
// Mostra os atributos do objeto polinomio, os coeficientes
void
CFPolinomio::Saida ()
{
  cout.setf (ios::scientific);

  cout.width (6);

  cout << "\n\tGrau do polinomio, ou funcao ciclica=" << grau
    << "\n\tp=" << p[0];

  for (int ii = 1; ii <= grau; ii++)
    cout << " +" << p[ii] << ".x^" << ii;
  cout.setf (ios::fixed);
}

// ---------------------------------------------------------------------------
// Permite modificar os coeficientes do polinomio
void
CFPolinomio::Entrada ()
{
  // int y,x;

  char resp;

  cout << "\n\tDeseja alterar o polinomio (s/n)?\n";

  cin.get (resp);		// =getch();

  cin.get ();

  if (resp == 's' || resp == 'S')
    {
      // y=wherey();
      // x=wherex();
      for (int ii = 0; ii <= grau; ii++)
	{
	  cout << "\tp[" << ii << "]=";
	  cin >> p[ii];
	  cin.get ();
	  // gotoxy(x,y);
	}
    }
}

// ---------------------------------------------------------------------------
// Funcao de definicao dos valores de p
void
CFPolinomio::Definir (double p0, double p1, double p2, double p3, double p4, double p5, double p6)	// s
{
  switch (grau)
    {
    case '6':
      p[6] = p6;
    case '5':
      p[5] = p5;
    case '4':
      p[4] = p4;
    case '3':
      p[3] = p3;
    case '2':
      p[2] = p2;
    case '1':
      p[1] = p1;

    case '0':
      p[0] = p0;
    }
}

// ---------------------------------------------------------------------------
// Calcula o resultado do polinomio, recebendo como parametro a variavel
// x.
//       virtual double Go(double x,double y=0); // calcula a funcao em si y=f(x) ou z=f(x,y)
double
CFPolinomio::Go (double x, double y)
{
  resultado = p[0];

  for (int ii = 1; ii <= grau; ii++)
    resultado += p[ii] * pow (x, ii);

  return (resultado);
}

// ---------------------------------------------------------------------------
// construtor
CFPolinomio1g::CFPolinomio1g ():CFPolinomio (1)
{
  // cout<<"\n\tCriou objeto CFPolinomio1g("<<sizeof(CFPolinomio1g)<<")";
  // cout<<" CFPolinomio1g="<<sizeof(CFPolinomio1g);
}

// ---------------------------------------------------------------------------
// destrutor,
CFPolinomio1g::~CFPolinomio1g ()
{
  // cout<<"\n\tDeletou objeto CFPolinomio1g";
}

// ---------------------------------------------------------------------------
// Calcula o resultado do polinomio, recebendo como parametro a variavel
// x.
double
CFPolinomio1g::Go (double x, double y)
{
  resultado = p[0] + p[1] * x;
  return (resultado);
}

// ---------------------------------------------------------------------------
// Construtor
CFPolinomio2g::CFPolinomio2g ():CFPolinomio (2)
{
  // cout<<"\n\tCriou objeto CFPolinomio2g("<<sizeof(CFPolinomio2g)<<")";
  // cout<<" CFPolinomio2g="<<sizeof(CFPolinomio2g);
}

// ---------------------------------------------------------------------------
// Destrutor,
CFPolinomio2g::~CFPolinomio2g ()
{
  // cout<<"\n\tDeletou objeto CFPolinomio2g";
}

// ---------------------------------------------------------------------------
// Calcula o resultado do polinomio, recebendo como parametro a variavel x.
double
CFPolinomio2g::Go (double x, double y)
{
  resultado = p[0] + p[1] * x + p[2] * x * x;

  return (resultado);
}

// ---------------------------------------------------------------------------
// Construtor
CFPolinomio3g::CFPolinomio3g ():CFPolinomio (3)
{
  // cout<<"\n\tCriou objeto CFPolinomio3g("<<sizeof(CFPolinomio3g)<<")";
  // cout<<" CFPolinomio3g="<<sizeof(CFPolinomio3g);
}

// ---------------------------------------------------------------------------
// Destrutor,
CFPolinomio3g::~CFPolinomio3g ()
{
  // cout<<"\n\tDeletou objeto CFPolinomio3g";
}

// ---------------------------------------------------------------------------
// Calcula o resultado do polinomio de terceiro grau,
// recebendo como parametro a variavel x.
double
CFPolinomio3g::Go (double x, double y)
{
  xx = x * x;

  resultado = p[0] + p[1] * x + p[2] * xx + p[3] * x * xx;

  return resultado;
}

// ---------------------------------------------------------------------------
// Construtor
CFPolinomio4g::CFPolinomio4g ():CFPolinomio (4)
{
  // cout<<"\n\tCriou objeto CFPolinomio4g("<<sizeof(CFPolinomio4g)<<")";
  // cout<<" CFPolinomio4g="<<sizeof(CFPolinomio4g);
}

// ---------------------------------------------------------------------------
// Destrutor,
CFPolinomio4g::~CFPolinomio4g ()
{
  // cout<<"\n\tDeletou objeto CFPolinomio4g";
}

// ---------------------------------------------------------------------------
// Calcula o resultado do polinomio de terceiro grau,
// recebendo como parametro a variavel x.
double
CFPolinomio4g::Go (double x, double y)
{
  xx = x * x;

  resultado = p[0] + p[1] * x + p[2] * xx + p[3] * x * xx + p[4] * xx * xx;

  return (resultado);
}

// ---------------------------------------------------------------------------
// Construtor
CFPolinomio5g::CFPolinomio5g ():CFPolinomio (5)
{
  // cout<<"\n\tCriou objeto CFPolinomio5g("<<sizeof(CFPolinomio5g)<<")";
  // cout<<" CFPolinomio5g="<<sizeof(CFPolinomio5g);
}

// ---------------------------------------------------------------------------
// Destrutor,
CFPolinomio5g::~CFPolinomio5g ()
{
  // cout<<"\n\tDeletou objeto CFPolinomio5g";
}

// ---------------------------------------------------------------------------
// Calcula o resultado do polinomio de terceiro grau,
double
CFPolinomio5g::Go (double x, double y)
{
  xx = x * x;

  xxx = xx * x;

  resultado =
    p[0] + p[1] * x + p[2] * xx + p[3] * xxx + p[4] * xx * xx +
    p[5] * xx * xxx;

  return (resultado);
}

// ---------------------------------------------------------------------------
// Construtor
CFPolinomio6g::CFPolinomio6g ():CFPolinomio (6)
{
  // cout<<"\n\tCriou objeto CFPolinomio6g ("<<sizeof(CFPolinomio6g)<<")";
  // cout<<" CFPolinomio6g="<<sizeof(CFPolinomio6g);
}

// ---------------------------------------------------------------------------
// Destrutor,
CFPolinomio6g::~CFPolinomio6g ()
{
  // cout<<"\n\tDeletou objeto CFPolinomio6g";
}

// ---------------------------------------------------------------------------
// Calcula o resultado do polinomio de terceiro grau,
// recebendo como parametro a variavel x.
double
CFPolinomio6g::Go (double x, double y)
{
  xx = x * x;

  xxx = xx * x;

  resultado = p[0] + p[1] * x + p[2] * xx + p[3] * xxx + p[4] * xx * xx
    + p[5] * xx * xxx;
  +p[6] * xxx * xxx;

  return (resultado);
}
/*
// ---------------------------------------------------------------------------
CSenoide::CSenoide ():CFPolinomio (3)
{
  // cout<<"\n\tCriou objeto Senoide ("<<sizeof(Senoide)<<")";
  // cout<<" Senoide="<<sizeof(Senoide);
}

// ---------------------------------------------------------------------------
CSenoide::~CSenoide ()
{
  // cout<<"\n\tDeletou objeto Senoide";
}

// ---------------------------------------------------------------------------
double
CSenoide::Go (double x, double y)
{
  resultado = p[0] + p[1] * sin (p[2] * x + p[3]);

  return (resultado);
}
*/

/*// ---------------------------------------------------------------------------
// Funcao estatica que cria um objeto polinomio e retorna um ponteiro para
// o objeto criado.
// Sempre que for criada uma nova classe herdeira da classe base polinomio
// voce deve acrescentar aqui o seu nome e a sua alocacao.
CFPolinomio* CFPolinomio::fselecaotipopolinomio()
{
  fcarimbo();
  cout<<"\n\tSelecione o tipo de polinomio:"
		<<"\n\tCFPolinomio de grau n..........0"
		<<"\n\tCFPolinomio1g.................1"
		<<"\n\tCFPolinomio2g.................2"
		<<"\n\tCFPolinomio3g ................3"
		<<"\n\tCFPolinomio4g ................4"
		<<"\n\tCFPolinomio5g ................5"
		<<"\n\tCFPolinomio6g ................6"
		<<"\n\tSenoide......................7:";
  cin>>tipodepolinomio;
  cin.get();

  switch(tipodepolinomio)
  {
	case 0:
		cout<<"\n\t Entre com o grau do polinomio:";
		int grau;
		cin>>grau;
		cin.get();
		ptr=new CFPolinomio(grau);
		break;
	case 1:
		ptr=new CFPolinomio1g();
		break;
	case 2:
		ptr=new CFPolinomio2g();
		break;
	case 3:
		ptr=new CFPolinomio3g();
		break;
	case 4:
		ptr=new CFPolinomio4g();
		break;
	case 5:
		ptr=new CFPolinomio5g();
		break;
	case 6:
		ptr=new CFPolinomio6g();
		break;
	case 7:
		ptr=new CSenoide();
		break;
  }
  fmemoria(ptr,"objeto polinomio");

return(ptr);
}
*/
