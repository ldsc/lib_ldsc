

#ifndef __interpol_cpp
#define __interpol_cpp

#include "Interpolacao.h"
//---------------------------------------------------------------------------
//vari veis estaticas precisam ser definidas abaixo da classe
double
  Interpolacao::peclet = 0.0;
double
  Interpolacao::pe2 = 0.0;
double
  Interpolacao::c3 = 0.0;
double
  Interpolacao::c4 = 0.0;
int
  Interpolacao::tipo_de_interpolacao = 1;	//[32];
Interpolacao *
  Interpolacao::ptr = NULL;	//ponteiro para objeto interpolacao

//---------------------------------------------------------------------------
//sobrecarga operadores cin>> e cout<<   template
//informa ao template que  para criar esta funcao para esta classe
ostream & operator << (ostream & op, Interpolacao * &c);
istream & operator >> (istream & op, Interpolacao * &c);

/*
//---------------------------------------------------------------------------
//Com a definicao da funao genrica a partir do template, nao temos
//a necessidade de digitar as funcoes abaixo.
//Funcao sobrecarregada de entrada dos dados (cin>>objetoclima)
//Como  uma funcao friend nao entra o nome da classe::
istream& operator >> (istream& op,Interpolacao*& c)
{
 c->f_entrada();
 return op;
}
//---------------------------------------------------------------------------
//Funcao sobrecarregada de saida dos dados (cout<<objetoclima)
//Como  uma funcao friend nao entra o nome da classe::
ostream& operator << (ostream& op,Interpolacao*& c)
{
 c->f_saida();
 return op;
}
*/
//---------------------------------------------------------------------------
//construtor
Interpolacao::Interpolacao ()
{
  //cout<<"\n\tCriou Objeto base Interpolacao("<<sizeof(Interpolacao)<<")";
}

//---------------------------------------------------------------------------
//destrutor
Interpolacao::~Interpolacao ()
{
  //cout<<"\n\tDeletou Objeto base Interpolacao";
}

//---------------------------------------------------------------------------
//Entrada dos atributos alfa e beta
void
Interpolacao::f_entrada ()
{
  f_carimbo ();
  cout << "\n\tAtributos classe base Interpolacao";
  cout << "\n\talfa:";
  cin >> alfa;
  cout << "\t\tbeta:";
  cin >> beta;
  cin.get ();
}

//---------------------------------------------------------------------------
//Saida dos atributos alfa e beta.
void
Interpolacao::f_saida ()
{
  cout << "\n\ttipo interpolacao=" << tipo_de_interpolacao;
  cout << "\n\talfa=" << alfa;
  cout << "\n\tbeta=" << beta;
}

//ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ                         CLASSE  Diferencas_Centrais                  บ
//ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
//Utiliza aproximacao por diferencas centrais.
//Alfa e beta sao constantes
class Diferencas_Centrais:public Interpolacao
{
  //metodos
public:
  Diferencas_Centrais ();	//Construtor
  virtual ~ Diferencas_Centrais ();	//Destrutor
  virtual void f_calculo_alfa_beta (double &u, double &dxc, double &ro, double &dif);	//redefinida
};
//---------------------------------------------------------------------------
//Construtor
Diferencas_Centrais::Diferencas_Centrais ()
{
  alfa = 0;
  beta = 1.0;
  //cout<<", Diferencas_Centrais("<<sizeof(Diferencas_Centrais)<<")";
}

//---------------------------------------------------------------------------
//Destrutor
Diferencas_Centrais::~Diferencas_Centrais ()
{
  //cout<<", Diferencas_Centrais";
}

//---------------------------------------------------------------------------
//funcao de calculo de alfa e beta.
//Esta funcao  redefinida .
//observe que aqui nao realiza nenhum c lculo.
//pois para o Diferencas_Centrais alfa e beta sao constantes
//e foram definidos no construtor
void
Diferencas_Centrais::f_calculo_alfa_beta
  (double &u, double &dxc, double &ro, double &dif)
{
}

//ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ                          CLASSE Upwind                               บ
//ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
//Utiliza aproximacao por Upwind
//Alfa e beta sao constantes
class
  Upwind:
  public
  Interpolacao
{
  //metodos
public:
  Upwind ();
  virtual ~
  Upwind ();			//construtor
  virtual void
    f_calculo_alfa_beta		//destrutor
  (double &u, double &dxc, double &ro, double &dif);	//redefinida
};
//---------------------------------------------------------------------------
//construtor
Upwind::Upwind ()
{
  alfa = 0.5;
  beta = 1.0;
  //cout<<", Upwind("<<sizeof(Upwind)<<")";
}

//---------------------------------------------------------------------------
//destrutor
Upwind::~Upwind ()
{
  //cout<<", Upwind";
}

//---------------------------------------------------------------------------
//funcao de calculo de alfa e beta.
//Esta funcao  redefinida .
//observe que aqui nao realiza nenhum c lculo.
//pois para o upwind alfa e beta sao constantes e foram definidos no construtor
void
Upwind::f_calculo_alfa_beta (double &u, double &dxc, double &ro, double &dif)
{
}

//ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ                          CLASSE Wuds                                 บ
//ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
//Utiliza aproximacao Wuds, que 'e baseada na solucao exata do problema
//Entretanto faz uma aproximacao dos coeficientes alfa e beta
//classe Interpolacao deve calcular os coeficientes alfa e beta.
class
  Wuds:
  public
  Interpolacao
{
  //metodos
public:
  Wuds ();
  virtual ~
  Wuds ();
  virtual void
  f_calculo_alfa_beta (double &u, double &dxc, double &ro, double &dif);	//redefinida
};
//---------------------------------------------------------------------------
//construtor
Wuds::Wuds ()
{
  alfa = 0;
  beta = 1;
  //cout<<", Wuds("<<sizeof(Wuds)<<")";
}

//---------------------------------------------------------------------------
//destrutor
Wuds::~Wuds ()
{
  //cout<<", Wuds";
}

//---------------------------------------------------------------------------
//funcao de calculo de alfa e beta.
//observe que  definida na classe base e aqui redefinida.
//os parametros sao passados por referencia por ser mais r pido
void
Wuds::f_calculo_alfa_beta (double &u, double &dxc, double &ro, double &dif)
{
  peclet = u * dxc * ro / dif;
  pe2 = peclet * peclet;
  alfa = pe2 / (10.0 + 2.0 * pe2);
  beta = (1.0 + 0.005 * pe2) / (1.0 + 0.05 * pe2);
}

//ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
//บ                          CLASSE Exata                                บ
//ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ
//Utiliza aproximacao exata, que 'e baseada na solucao exata do problema
//Os coeficientes alfa e beta calculados sao exatos.
//A velocidade recebida como parametro nao pode ser zero.
class Exata:public Interpolacao
{
  //metodos
public:
  Exata ();			//Construtor
  virtual ~ Exata ();		//Destrutor
  virtual void f_calculo_alfa_beta (double &u, double &dxc, double &ro, double &dif);	//redefinida
};
//---------------------------------------------------------------------------
//Construtor
Exata::Exata ()
{
  alfa = 0;
  beta = 1;
  //cout<<", Exata("<<sizeof(Exata)<<")";
}

//---------------------------------------------------------------------------
//Destrutor
Exata::~Exata ()
{
  // cout<<", Exata";
}

//---------------------------------------------------------------------------
//observacao:
//  Para interpolacao do tipo exata, a velocidade u nao pode ser zero.
//  Se a velocidade for zero, peclet=0,c3=0 e vai ocorrer um erro de
//  divisao por zero e estouro.
//Pode-se futuramente incluir aqui um tratamento de excessao
//para o estouro da pilha.
void
Exata::f_calculo_alfa_beta (double &u, double &dxc, double &ro, double &dif)
{
  peclet = u * dxc * ro / dif;	//v=0 peclet=0
  c3 = pow (E, peclet) - 1.0;	//c1=e^peclet-1
  c4 = pow (E, (peclet / 2.0));	//c2=e^(peclet/2)
  alfa = 0.5 - (c4 - 1.0) / c3;
  beta = peclet * c4 / c3;
}

//---------------------------------------------------------------------------
//Funcao estatica de selecao do tipo de interpolacao
//cria um objeto interpolacao e retorna um ponteiro para o objeto criado
//o ponteiro ptr aponta para o ultimo objeto criado.
Interpolacao *
Interpolacao::f_selecao_tipo_de_interpolacao ()
{
  f_carimbo ();
  cout << "\n\t\tEntre com o tipo de interpolacao"
    << "\n\tDif.Centrais (default)....(1)"
    << "\n\tUpwind....................(2)"
    << "\n\tWuds......................(3)"
    << "\n\tExponencial...............(4):";
  cin >> tipo_de_interpolacao;	//=getche();
  cin.get ();
  switch (tipo_de_interpolacao)
    {
    case 1:
      ptr = new Diferencas_Centrais;
      break;
    case 2:
      ptr = new Upwind;
      break;
    case 3:
      ptr = new Wuds;
      break;
    case 4:
      ptr = new Exata;
      break;
    default:
      ptr = new Diferencas_Centrais;
    }
  f_memoria (ptr, "interpolacao");
  return (ptr);			//retorna o ponteiro para o objeto criado
}

//-----------------somente para teste de compilacao-----------------
/*
main()
{
  clrscr();
  //cria objetos
  Interpolacao* dc=new Diferencas_Centrais;
  Interpolacao* up=new Upwind;
  Interpolacao* wu=new Wuds;
  Interpolacao* ex=new Exata;

  double u=1;
  double dxc=2;
  double ro=3;
  double dif=4;

  //mostra atributos dos objetos
  cout<<dc;
  cout<<up;
  cout<<wu;
  cout<<ex;
  cin.get();//getche();
  clrscr();

  //calcula os valores de alfa e beta
  dc->f_calculo_alfa_beta(u,dxc,ro,dif);
  up->f_calculo_alfa_beta(u,dxc,ro,dif);
  wu->f_calculo_alfa_beta(u,dxc,ro,dif);
  ex->f_calculo_alfa_beta(u,dxc,ro,dif);

  //mostra os valores de alfa e beta
  cout<<dc;
  cout<<up;
  cout<<wu;
  cout<<ex;

  //deleta objetos
  delete dc;
  delete up;
  delete wu;
  delete ex;
}
*/

#endif //__interpol
