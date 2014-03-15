
#include <TMaterial/CMaterialFluido.h>

//---------------------------------------------------------------------------
//Sobrecarga operadores cin>> e cout<<   template
//Com o prototipo da funcao confirma a criacao da mesma.
ostream & operator << (ostream & op, CMaterialFluido * &c);
istream & operator >> (istream & op, CMaterialFluido * &c);

//--------------------------------------------------------------------------
//Construtor
CMaterialFluido::CMaterialFluido ()
{
  //cout<<",CMaterialFluido("<<sizeof(CMaterialFluido)<<")";
}

//--------------------------------------------------------------------------
//Destrutor
CMaterialFluido::~CMaterialFluido ()
{
  //cout<<",CMaterialFluido";
}

//---------------------------------------------------------------------------
void
CMaterialFluido::Default ()	//armazena valores default dos atributos
{
  CMaterial::Default ();	//classe base definicoes
  condutividade = 13.486;	//valor redefinido (analisar)
  densidade = 1000.0;		//valor redefinido
  calor_especifico = 4187.0;	//valor redefinido

// umidade=0.0;                     //atributos novos
  calor_latente = 2256685.0;	//calor latente de vaporizacao
  viscosidade = 1.0;
  viscosidade_cinematica = 1.0;
  pressao_vapor_saturado = 1.0;
  dif_vapor_ar = 1.0;
  coef_exp_vol = 1.0;
  p_cl.Definir (1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
  //p_cp.Definir(4217.8,-1.7245,0.03398,-0.002534);
  p_expvol.Definir (1, 0, 0, 0);
}

//---------------------------------------------------------------------------
//Mostra os valores dos atributos
void
CMaterialFluido::Saida ()
{
  //cout<<"\n\tObjeto CMaterialFluido (atributos da classe base e novos).";
  CMaterial::Saida ();
  cout << "\n\n\tAtributos novos da Classe CMaterialFluido:";
  Pausa ();
// cout<<"\n\tumidade="<<umidade;
  cout << "\tcalor latente=" << calor_latente;
  cout << "\n\tviscosidade=" << viscosidade;
  cout << "\tviscosidade_cinematica=" << viscosidade_cinematica;
  cout << "\n\tpressao_vapor_saturado=" << pressao_vapor_saturado;
  cout << "\n\tdif_vapor_ar=" << dif_vapor_ar;
  cout << "\tcoef_exp_vol=" << coef_exp_vol;
  cout << "\n\n\tPolinomio para o calor latente";
  cout << &p_cl;		//p_cl.f_saida();
  //cout<<"\n\tPolinomio para o calor especifico";
  //cout<<&p_cp;
  cout << "\n\n\tPolinomio para expansao volumetrica";
  cout << &p_expvol;
}

//---------------------------------------------------------------------------
//Modifica os valores dos atributos
void
CMaterialFluido::Modificar ()
{
  //cout<<"\n\tObjeto CMaterialFluido (atributos da classe base e novos).";
  CMaterial::Modificar ();
  cout << "\n\tAtributos novos da Classe CMaterialFluido:";
  Pausa ();
  cout << "\n\tcalor latente:";
  cin >> calor_latente;
  cout << "\tviscosidade:";
  cin >> viscosidade;
  cout << "\tviscosidade_cinematica:";
  cin >> viscosidade_cinematica;
  cout << "\tpressao_vapor_saturado:";
  cin >> pressao_vapor_saturado;
  cout << "\tdif_vapor_ar:";
  cout << "\n\tPolinomio para calor latente....erro";
//erro  cin>>&p_cl;

  //cout<<"\n\tPolinomio para calor especifico";
  //    cin>>&p_cp      //p_cl.f_modificar();

  cout << "\n\tPolinomio para  expansao volumetrica...erro";
  //erro        cin>>&p_expvol;
//  cout<<"\n\tumidade do fluido:";
//      cin>>umidade;
  cin >> dif_vapor_ar;
  cout << "\tcoef_exp_vol:";
  cin >> coef_exp_vol;
  cin.get ();
}

//---------------------------------------------------------------------------
//Calcula a pressao do vapor saturado para a �gua
//Observe que usa para o cálculo a temperatura
//Esta funcao foi retirado do manual do CIBS
double
CMaterialFluido::Pressao_vapor_saturado ()
{
  double t;
  t = temperatura + TK;		//a temperatura ja foi recebida.
  pressao_vapor_saturado =
    1000 * pow10 (30.5905 - 8.2 * log10 (t) + 2.4804e-3 * t - 3142.32 / t);
  return (pressao_vapor_saturado);
}


//---------------------------------------------------------------------------
//Coeficientes de expansao volumetrica do vapor T(C)  "Alfa"
// derivada_h_teta de rovs em relacao a T  "drovs"
//Para a linguagem C log significa logaritmo neperiano.
//util para o cálculo de DTV
double
CMaterialFluido::Coef_exp_vol ()
{
  //coef_exp_vol=(log(10.0))*(p_exp[0]+2*p_exp[1]*temperatura
  //           +3*p_exp[2]*temperatura*temperatura
  //           +4*p_exp[3]*temperatura*temperatura*temperatura
  //           +5*p_exp[4]*temperatura*temperatura*temperatura*temperatura);
  coef_exp_vol = p_expvol.Go (temperatura);
  return (coef_exp_vol);
}

//---------------------------------------------------------------------------
//Calcula o  Calor especifico da agua  [J/kg.K]
//temperatura=temperatura da agua?
double
CMaterialFluido::Calor_especifico ()
{
  calor_especifico = 4187.0;
  //calor_especifico=4217.8-1.7245*temperatura+0.03398*temperatura*temperatura
  //                    -0.002534*temperatura*temperatura*temperatura;
  //calor_especifico=p_cp.f_calcular(temperatura);
  return (calor_especifico);
}

//---------------------------------------------------------------------------
//Calcula a Difusividade do vapor d'agua no ar [m^2/s] - De Vries P[Pa], T[C]
double
CMaterialFluido::Dif_vapor_ar ()	//corrigir
{
  dif_vapor_ar = 2.4e5;		//0.0000217*(101325.0/P)*pow(((temperatura+273.16)/273.16),1.88);
  return (dif_vapor_ar);
}

 /*
    //---------------------------------------------------------------------------
    //Esta funcao so e' utilizada para esta classe
    //e' usada para receber a variavel umidade da classe material fluido.
    //A mesma foi definida na classe base para poder ser acessada
    //pelo ponteiro MF.
    void  CMaterialFluido::f_umidade(double h)          //redefinida
    {
    umidade=h;
    }
  */

//---------------------------------------------------------------------------
//Executa as funcoes da classe clima
//Observe que a ordem pode ser importante
// v[1]=temperatura, e v[0]=conteudo_ol
//a umidade � passada na classe Material_Meio_Poroso_Amostra.
//observe que a umidade � usada no cálculo da concentracao de vapor
void
CMaterialFluido::Calcula_propriedades_material (double *&v)
{
  //Chama funcao da classe base
  //O parametro v1 representa a temperatura que � armazenada.
  CMaterial::f_calcula_propriedades_material (v);	//classe base


// f_pressao_vapor_saturado(); //Nao e necessario calcular aqui?
  //pois o objeto CMaterial meio poroso amostra
  //chama a funcao de cálculo f_pressao_vapor_saturado diretamente ??

  //f_dif_vapor_ar();        //calcula a difusao do vapor no ar
  //f_coef_exp_vol();        //calcula o coeficiente de expansao volumetrica

  //calor_latente=p_cl.f_calcular(temperatura);//calor_latente =2256685,0;

  //f_calor_especifico();    //� chamada pela classe base
  //calor_especifico=p_cp.f_calcular(temperatura);
}

//--------------------------------------------------------------------------
//funcao da classe base Banco_de_dados redefinida
//Obs: o uso de ptr, que � um ponteiro para a classse base material
//esta de acordo se o objeto material fluido foi o ultimo
//objeto criado com a funcao Selecao_tipo_de_material.
void
CMaterialFluido::Ler_disco ()
{
  strcpy (ext, "mf");		//extensao
  size_obj = sizeof (CMaterialFluido);
  ptr_obj = ptr;		//ptr_obj
  CBancoDeDados::Ler_disco ();	//chama funcao da classe
  //base  Banco_De_Dados
}

//--------------------------------------------------------------------------
//funcao da classe base Banco_de_dados redefinida
void
CMaterialFluido::Salvar_disco ()
{
  strcpy (ext, "mf");
  size_obj = sizeof (CMaterialFluido);
  ptr_obj = ptr;
  CBancoDeDados::Salvar_disco ();	//chama funcao da classe base
  //Banco_De_Dados
}
