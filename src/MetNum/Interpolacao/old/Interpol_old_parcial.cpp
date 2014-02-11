//---------------------------------------------------------------------------
//Funcao estatica de selecao do tipo de interpolacao
//cria um objeto interpolacao e retorna um ponteiro para o objeto criado
//o ponteiro ptr aponta para o ultimo objeto criado.
TIntMetNum *
TIntMetNum::f_selecao_tipo_de_interpolacao ()
{
  f_carimbo ();
  cout << "\n\t\tEntre com o tipo de interpolacao"
    << "\n\tDif.Centrais (default)....(1)"
    << "\n\tTIntMetNumUpwind....................(2)"
    << "\n\tTIntMetNumWuds......................(3)"
    << "\n\tExponencial...............(4):";
  cin >> tipo_de_interpolacao;	//=getche();
  cin.get ();
  switch (tipo_de_interpolacao)
    {
    case 1:
      ptr = new Diferencas_Centrais;
      break;
    case 2:
      ptr = new TIntMetNumUpwind;
      break;
    case 3:
      ptr = new TIntMetNumWuds;
      break;
    case 4:
      ptr = new TIntMetNumExata;
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
  TIntMetNum* dc=new Diferencas_Centrais;
  TIntMetNum* up=new TIntMetNumUpwind;
  TIntMetNum* wu=new TIntMetNumWuds;
  TIntMetNum* ex=new TIntMetNumExata;

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

//#endif        //__interpol
