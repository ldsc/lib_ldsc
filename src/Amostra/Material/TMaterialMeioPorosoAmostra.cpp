
#include <TMaterial/CMaterialMeioPorosoAmostra.h>
// ---------------------------------------------------------------------------
// Sobrecarga operadores cin>> e cout<<  template
ostream & operator<< (ostream & op, CMaterialMeioPorosoAmostra * &c);
istream & operator>> (istream & op, CMaterialMeioPorosoAmostra * &c);

// --------------------------------------------------------------------------
// Este construtor deve criar dois objetos. O material fluido(MF) e o
// material meio poroso(MMP). Os mesmos devem ser eliminados no destrutor.
// Foi modificado, como o objeto material meio poroso ja e criado dinamicamente,
// os objetos todo paste podem ser estaticos.
// Tem a vantagem de armazenar os objetos todo parte em disco, atraves das funcoes
// f_ler_disco e f_salvar_disco.
// Como foi feita esta alteracao, talves nao seja mais necessario os
// ponteiros MF e MMP. (repensar)???
CMaterialMeioPorosoAmostra::CMaterialMeioPorosoAmostra ()
{
  // cout<<",CMaterialMeioPorosoAmostra("
  //                     <<sizeof(CMaterialMeioPorosoAmostra)<<")";
  // MF=new Material_Fluido;
  //     f_memoria(MF," todo parte MF");
  // MMP=new Material_Meio_Poroso;
  //     f_memoria(MMP," todo parte MF e MMP");
  MF = &Estatico_MF;		// armazena em MF o endereco do objeto estatico
  MMP = &Estatico_MMP;		// armazena em MMP o endereco do objeto estatico
  // agora pode-se usar MF e MMP diretamente

  // ??????  P E R I G O  ????
  // sera que no construtor os objetos estativos ja foram criados???
  // Creio que sim, voce nao deve e chamar funcoes virtuais em construtores.
}

// --------------------------------------------------------------------------
// Destrutor. Deve destruir os objetos alocados no construtor.
CMaterialMeioPorosoAmostra::~CMaterialMeioPorosoAmostra ()
{
  // delete MF;     // deleta objeto todo parte material fluido
  // delete MMP;  // deleta objeto todo parte material meio poroso
  // cout<<",CMaterialMeioPorosoAmostra";
}

// --------------------------------------------------------------------------
// Chama as funcoes que inicializam os atributos, os mostram e modificam.
// Obs: esta funcao poderia ser eliminada, se as diferencas em relacao a
// funcao da classe base sejam incluidas em uma das funcoes acessadas
// pela funcao de entrada da classe base.
void
CMaterialMeioPorosoAmostra::f_entrada ()
{
  cout << "\n\tO objeto CMaterialMeioPorosoAmostra e' complexo, "
    << "\n\tconsiste de dois objetos todo parte"
    << "\n\t(material meio poroso MMP e material fluido MF)"
    << "\n\tAlem disso, herda os atributos da classe base material,"
    << "\n\te inclue novos atributos."
    << "\n\n\tPrimeiro obtem dados dos objetos todo parte"
    << "\n\tDepois do objeto CMaterialMeioPorosoAmostra";

  cout << "\n\n\tVai executar as funcoes de entrada do objeto todo"
    << " parte Material_Fluido";
  Pausa ();
  MF->Entrada ();		// entrada classe base material fluido
  // cin>>MF;
  cin.get ();
  Carimbo ();
  cout << "\n\tVai executar as funcoes de entrada do objeto "
    << "\n\ttodo parte Material_Meio_Poroso";
  Pausa ();
  MMP->Entrada ();		// entrada classe base TMaterial meio poroso
  // cin>>MMP;
  cin.get ();

  Carimbo ();
  cout << "\n\tVai executar a funcao de entrada "
    << "do objeto CMaterialMeioPorosoAmostra"
    << "\n\tPrimeiro os atributos da classe base depois os novos.";
  Pausa ();
  TMaterial::Entrada ();	// executa a funcao de entrada da classe base.
}

// ---------------------------------------------------------------------------
// Armazena valores default dos atributos
void
CMaterialMeioPorosoAmostra::Default ()
{
  TMaterial::Default ();	// inicializa atributos da classe base

  // dimensao_x=1.0;     // inicializa atributos novos
  // dimensao_y=1.0;
  // dimensao_z=1.0;
  concentracao_vapor = 1.0;
  massa_saturada = 1730 * 1.33;
  massa_seca = 1730.0;
  volume = 1.0;
  umidade = 0.0;
  dtv = 5.0e-8;
  dtl = 5.0e-8;
  dolv = 5.0e-10;
  doll = 5.0e-10;
  difusividade_massica = 5.0e-10;
  fator_experimental = 1.0;
  temperatura_ensaio_adsorcao = 25.0;
  conteudo_ol = 0.0;		// definido na classe base e redefinido aqui.
  conteudo_wl = 0.0;
  // observe o uso dos objetos todo parte na definicao do polinomio p_k.
  // p_k[0]=0.084742;   p_k[1]=1.0174;// para concreto celular Bellini
  // =0.743  para argamassa           // =13.486 Celso Fernandes
  p_k.Definir (MMP->f_condutividade (), MF->f_condutividade ());	// verificar funcionamento
  p_ads.Definir (1.42785867, 160.5442872, -122.690340, -139.3032187,
		   101.397014919);
  p_des.Definir (1.42785867, 160.5442872, -122.690340, -139.3032187,
		   101.397014919);
  p_dif_termica.Definir (1, 1, 1, 1, 1);
  p_der.Definir (1, 1, 1, 1, 1);
  p_fe.Definir (0.1006429853, -1.8252810641, +19.789457415);
}

// ---------------------------------------------------------------------------
// Mostra os valores dos atributos
void
CMaterialMeioPorosoAmostra::Saida ()
{
  // se na funcao de entrada, permite a entrada dos atributos dos dois objetos
  // estaticos, MF e MMP, aqui deveria mostrar os atributos destes dois objetos
  // para que o usuário possa os alterar?
  // Isto nao e necessario, pois voce executa as funcoes f_entrada dos objetos
  // estaticos, e a funcao f_entrada ja verifica a entrada correta dos dados
  // possibilitando ao usuário a alteracao atributos errados. OK.

  cout <<
    "\n\tObjeto material meio poroso amostra, atributos da classe base.";
  TMaterial::Saida ();
  cout << "\n\tObjeto CMaterialMeioPorosoAmostra, atributos novos:";
  Pausa ();
  // cout<<"\n\tdimensao_x="<<dimensao_x;
  // cout<<"\tdimensao_y="<<dimensao_y;
  // cout<<"\tdimensao_z="<<dimensao_z;
  cout << "\tconcentracao_vapor=" << concentracao_vapor;
  cout << "\n\tmassa_saturada=" << massa_saturada;
  cout << "\tmassa_seca=" << massa_seca;
  cout << "\n\tvolume=" << volume;
  cout << "\n\tumidade=" << umidade;
  cout << "\n\tconteudo_ol=" << conteudo_ol;
  cout << "\tconteudo_wl=" << conteudo_wl;
  cout << "\n\tfator_experimental=" << fator_experimental;
  cout << "\n\ttemperatura_ensaio de adsorcao e dessorcao" <<
    temperatura_ensaio_adsorcao;
  cout << "\n\tdtv=" << dtv;
  cout << "\tdtl=" << dtl;
  cout << "\tdolv=" << dolv;
  cout << "\tdoll=" << doll;
  // f_pausa();

  cout << "\n\tDeseja ver os polinomios do objeto CMaterialMeioPorosoAmostra"
    <<
    "\n\tCondutividade,adsorcao,dessorcao,difusividade,derivada_h_teta e fator exp:";
  cin.get (resp);		// =getche();
  cin.get ();
  if (resp == 's' || resp == 'S')
    {
      cout << "\n\tCoeficientes polinomio condutividade\n";
      cout << &p_k;		// p_k.f_saida();
      cout << "\n\tCoeficientes polinomio adsorcao\n";
      cout << &p_ads;		// p_ads.f_saida();
      cout << "\n\tCoeficientes polinomio dessorcao\n";
      cout << &p_des;		// p_des.f_saida();
      cout << "\n\tCoeficientes polinomio difusividade\n";
      cout << &p_dif_termica;	// p_dif_termica.f_saida();
      cout << "\n\tCoeficientes polinomio para derivada_h_teta\n";
      cout << &p_der;		// p_der.f_saida();
      cout << "\n\tCoeficientes polinomio para fator experimental\n";
      cout << &p_fe;		// p_fe.f_saida();
    }
}

// ---------------------------------------------------------------------------
// Permite a modificacao dos atributos
void
CMaterialMeioPorosoAmostra::Modificar ()
{
  // cout<<"\n\tVai modificar atributos da classe base do CMaterialMeioPorosoAmostra";
  TMaterial::Modificar ();	// modificacao do objeto material meio poroso
  // As modificacoes dos objetos estaticos ja foram realizadas atraves da funcao f_entrada destes objetos.
  // Verificar se nao agride a uniformidade, do sistema de entrada, alteracao e saida de dados.

  cout << "\n\tEntre os novos atributos do CMaterialMeioPorosoAmostra:";
  Pausa ();
  // cout<<"\n\tdimensao_x=";
  //  cin>>dimensao_x;
  // cout<<"\tdimensao_y=";
  // cin>>dimensao_y;
  // cout<<"\tdimensao_z=";
  //  cin>>dimensao_z;
  cout << "\tmassa_saturada=";
  cin >> massa_saturada;
  cout << "\tmassa_seca=";
  cin >> massa_seca;
  cout << "\tvolume=";
  cin >> volume;
  cout << "\tEntre com a umidade=";
  cin >> umidade;
  cout << "\tconteudo_ol =";
  cin >> conteudo_ol;
  cout << "\tconteudo_wl =";
  cin >> conteudo_wl;
  cout << "\tconcentracao_vapor:";
  cin >> concentracao_vapor;
  cout << "\tdifusividade  dtv=";
  cin >> dtv;
  cout << "\tdifusividade  dtl=";
  cin >> dtl;
  cout << "\tdifusividade  dolv=";
  cin >> dolv;
  cout << "\tdifusividade  doll=";
  cin >> doll;
  cout << "\tfator_experimental=";
  cin >> fator_experimental;
  cout << "\ttempertura_ensaio adsorcao/dessorcao=";
  cin >> temperatura_ensaio_adsorcao;

  Pausa ();
  cout << "\n\tDeseja modificar polinomios para condutividade, adsorcao,"
    <<
    "\n\tdessorcao, difusividade, derivada_h_teta e fator experimental(s/n)?:";
  cin.get (resp);		// =getche();
  cin.get ();
  if (resp == 's' || resp == 'S')
    {
      cout << "\n\tPolinomio para  condutividade";
      // erro cin>>&p_k;
      cout << "\n\tPolinomio para  adsorcao";
      // erro cin>>&p_ads;
      cout << "\n\tPolinomio para  dessorcao(s/n)?:";
      // erro cin>>&p_des;
      cout << "\n\tPolinomio para  difusividade(s/n)?:";
      // erro cin>>&p_dif_termica;
      cout << "\n\tPolinomio para  derivada_h_teta curva adsorcao(s/n)?:";
      //  erro cin>>&p_der;
      cout << "\n\tPolinomio para  fator experimental(s/n)?:";
      // erro cin>>&p_fe;
      cin.get ();
    }
}

// ---------------------------------------------------------------------------
// Calcula a condutividade com base nas condutividades do
// objeto fluido e do objeto meio poroso
double
CMaterialMeioPorosoAmostra::Condutividade ()
{
  // condutividade=MMP->condutividade+MF->condutividade*conteudo_wl;?
  condutividade = p_k.Go (conteudo_wl);	// a variavel independente e' o conteudo massico de umidade.
  return (condutividade);
}

// ---------------------------------------------------------------------------
// Calcula a densidade com base nas densidades do fluido e do meio poroso seco
 // No programa do mestrado, voce nao alteraca a densidade.
 // Mas para usar as equacoes gerais de FI, deve-se recalcular a densidade desta forma.
double
CMaterialMeioPorosoAmostra::Densidade ()
{
  densidade = MMP->f_densidade () + MF->f_densidade () * conteudo_ol;	// redefinida
  return (densidade);
}

// ---------------------------------------------------------------------------
// Calcula o Calor especifico do meio umido [J/kg.K]
//  Calor especifico do material meio poroso incluindo a umidade [J/kg.K]
double
CMaterialMeioPorosoAmostra::f_calor_especifico ()
{
  calor_especifico =
    MMP->f_calor_especifico () + MF->f_calor_especifico () * conteudo_wl;
  return (calor_especifico);	// redefinida
}

// ---------------------------------------------------------------------------
// Calcula a concentracao do vapor,
// tem como base os parametros temperatura e umidade_do_fluido
// ML=18, TK=273.16, R=8313 definidos em const.cpp
double
CMaterialMeioPorosoAmostra::Concentracao_vapor ()
{
  concentracao_vapor =
    MF->Pressao_vapor_saturado () * umidade * ML / (R * (TK + temperatura));
  return (concentracao_vapor);
}

				 // a concentracao de vapor chama o metodo
/*
// ---------------------------------------------------------------------------
double  CMaterialMeioPorosoAmostra::f_conteudo_ol()
{
 return conteudo_ol;
}
// ---------------------------------------------------------------------------
double  CMaterialMeioPorosoAmostra::f_conteudo_wl()
{
 return conteudo_wl;
}
*/

// ---------------------------------------------------------------------------
// A isoterma de adsorcao relaciona o conteudo de umidade do material
// (conteudo_wl) com a umidade relativa do gas.

// e necessaria a temperatura em que foi realizado o ensaio de adsorcao
// para corrigir a curva de adsorcao.
// e necessario que ja tenha sido calculada o conteudo de umidade massico wl
//  O objeto material recebe como parametro o ponteiro *&v, que tem a temperatura
// e o conteudo volumetrico de umidade, o objeto material deve calcular entao
// wl em funcao de ol, e depois a umidade pela curva de adsorcao.

// Depois que estiver implementado o banco de dados de materiais
// o usuário podera� selecionar o material onde estao  definidos
// os atributos do material, inclusive os relativos aos ensaios realizados
// A umidade calculada por f_adsorcao � armazenada em umidade
// O cálculo da adsorcao e da dessorcao deve ser realizado com um unico
// polinomio; permitindo seu armazenamento em disco?
// Basta criar dois objetos polinomios.

// Obs Modificar para polinomio, a adsorcao deve poder
// ser posta na forma de um ou dois polinomios.
double
CMaterialMeioPorosoAmostra::Adsorcao ()
{

  // Equacao para curva adsorcao Celso argamassa
  // if(tipo=='c'||tipo=='C')// tipo=c Celso
  if (conteudo_wl > 0.000001 && conteudo_wl <= 0.015671)	// polinomio??
    umidade = 2.735972 + 0.300513 * log (conteudo_wl / 10.0);
  else if (conteudo_wl <= 0.056975)
    umidade = 2.7164 - 57.6623 * conteudo_wl / 10.0
      + 2.2017 * pow (conteudo_wl / 10.0, 3)
      + 0.094077 * log (pow (conteudo_wl / 10.0, 3));
  else				// if (conteudo_wl>0.056975)// 0.1812)
    umidade = 0.95623 + 0.2415 * conteudo_wl;
  // umidade=p_ads.f_calcular(conteudo_wl);
  // mudar para polinomio.
/*
// EQUACAO PARA ADSORCAO PABLO ARGAMASSA
  else if(tipo=='p'||tipo=='P')// tipo=p Pablo
	{t1=30;
	 if (conteudo_wl<=0.0014)
	umidade=5000*conteudo_wl;
	 else if (conteudo_wl<=0.0073)
	umidade=-9.9696+12358*conteudo_wl;
	 else if (conteudo_wl<=0.0216)
	umidade=38.05+7502*conteudo_wl
			-249425*conteudo_wl*conteudo_wl
			+1217914*conteudo_wl*conteudo_wl*conteudo_wl;
	 else // if (conteudo_wl>0.0216)// 0.1704
	umidade=26.8817*conteudo_wl+95.419;
	}
// EQUACAO PARA ADSORCAO ANDRE TELHA
	else if(tipo=='t'||tipo=='T')// tipo=t telha André, d de default
	{t1=30;
	 if (conteudo_wl<=0.0163)
	umidade=3894*conteudo_wl;
	 else if (conteudo_wl<=0.0182)
	umidade=-72.8284+8362*conteudo_wl;
	 else if (conteudo_wl<=0.07846)
	umidade=74.423+271*conteudo_wl;
	 else // if (conteudo_wl>0.07846)// 0.176
	umidade=92.2475+44.048*conteudo_wl;
	}
*/
// Correcao da curva de adsorcao em funcao da temperatura do ensaio
  if (umidade > 0)
    umidade = exp (log (umidade) * (0.0763 - 0.00016835 * temperatura)
		   * temperatura_ensaio_adsorcao / ((0.0763 - 0.00016835
						     *
						     temperatura_ensaio_adsorcao)
						    * temperatura));
  // h2=e^(ln h1*(sigma(t2)*t1)/(sigma(t1)*t2))
  if (umidade >= 1)
    umidade = 1.0;
  if (umidade <= 0)
    umidade = 0;
  return (umidade);
}

// ---------------------------------------------------------------------------
// Isoterma de dessorcao, relaciona o conteudo de umidade massico
// com a umidade relativa.
// Observe o parametro temperatura, para corrigir a curva de dessorcao
double
CMaterialMeioPorosoAmostra:Dessorcao ()
{
  Adsorcao ();		// chama o cálculo da adsorcao (curva de dessorcao nao calculada)
  return (umidade);
}

// ---------------------------------------------------------------------------
//  Calculo da derivada_h_teta de h em relacao a teta
// Substituir por polinomio???
double
CMaterialMeioPorosoAmostra::Derivada_h_teta_h_ol ()
{
  /*
     double h_1,xnumer,denom;
     h_1=f_adsorcao();
     xnumer=p_ads[0]+p_ads[1]*h_1+p_ads[2]*h_1*h_1+
     p_ads[3]*h_1*h_1*h_1+p_ads[4]*h_1*h_1*h_1*h_1;
     denom=p_ads[0]-p_ads[2]*h_1*h_1-2*p_ads[3]*h_1*h_1*h_1-3
     *p_ads[4]*h_1*h_1*h_1*h_1;
     derivada_h_teta=xnumer*xnumer/denom;
   */
  derivada_h_teta = 1.0;	// temporario
  return (derivada_h_teta);
}

// ---------------------------------------------------------------------------
// calcula a difusividade global a transferencia de fluido
// Observe que por enquanto esta definida como a curva de difusividade
// da argamassa de cal e cimento
// Posteriormente dever� ser alterada, incluindo as difusividade
// de massa e de temperatura.
// Obs Modificar para polinomio, a difusividade deve poder
// ser posta na forma de um ou dois polinomios.
double
CMaterialMeioPorosoAmostra::Difusividade_massica ()	// redefinida
{
  if (conteudo_ol < 0.0692)
    difusividade_massica =
      9.25e-8 - 2.81e-6 * conteudo_ol + 2.39e-5 * conteudo_ol * conteudo_ol;
  // 2grau
  else if (conteudo_ol <= 0.173)
    difusividade_massica = 7.95e-9 + 6.21e-8 * conteudo_ol;	// reta
  else if (conteudo_ol <= 0.1903)	// item incluido em 250392
    difusividade_massica = -1.135e-7 + 7.734e-7 * conteudo_ol;
  else if (conteudo_ol <= 0.26815)
    difusividade_massica = 3.816766e-8 - 0.000054 * pow (conteudo_ol, 4) + 1.048743 * pow (conteudo_ol, 10);	// 10 grau
  else				// if (conteudo_ol>0.26815)
    difusividade_massica = 2e-6;	// limite para conteudo_ol>0.26815

  if (difusividade_massica <= 1e-8)	// assegura valor mininmo de difusividade_massica
    difusividade_massica = 1e-8;	// porque a regressao nao e perfeita
  if (difusividade_massica > 2e-6)	// assegura valor maximo difusividade_massica
    difusividade_massica = 2e-6;	// alteracao- o correto e fixar conteudo_ol?
  // se conteudo_ol ja foi fixado, entao basta
  // verificar o cálculo da difusividade para a regiao
  // do conteudo_ol.
  return (difusividade_massica);



/*
Esta funcao deveria chamar as funcoes:
	f_cálculo_difusividade_dtl();

	f_cálculo_difusividade_dtv();
	f_cálculo_difusividade_doll();
	f_cálculo_difusividade_dolv();
	f_fator_experimental(); 			// subst. por polinomio p_fe
	f_derivada_h_teta_h_ol();
	e calcular a difusividade massica por
	difusividade_massica=dtv+dtl+doll+dolv;

*/
}

// ---------------------------------------------------------------------------
// Difusividade termica fase liquida
double
CMaterialMeioPorosoAmostra::f_cálculo_difusividade_dtl ()
{
  // dtl=dtv;  // para baixos conteudos desprezo o DTL
  dtl = 1e-12;
  return (dtl);
}

// ---------------------------------------------------------------------------
// Difusividade t�rmica fase vapor
double
CMaterialMeioPorosoAmostra::f_cálculo_difusividade_dtv ()
{				/*
				   double h1;
				   double dif_vapor_ar=1.0;
				   double densidade_vapor_saturado=1.0;
				   double coeficiente_expansao_volumetrica=1.0;
				   double densidade_fluido=1.0;
				   dtv=dif_vapor_ar*fator_experimental*h1*densidade_vapor_saturado
				   *coeficiente_expansao_volumetrica/densidade_fluido;
				 */
  dtv = 1e-12;
  return (dtv);
}

// ---------------------------------------------------------------------------
// Difusividade massica fase liquida
double
CMaterialMeioPorosoAmostra::f_cálculo_difusividade_doll ()
{
  doll = 1e-8;			// dolv;
  return (doll);
}

// ---------------------------------------------------------------------------
// Difusividade massica fase vapor
double
CMaterialMeioPorosoAmostra::Calculo_difusividade_dolv ()
{				/*
				   double Dol2=1.0;
				   // double  dhol=1.0;
				   double dif_vapor_ar=1.0;
				   double densidade_vapor_saturado=1.0;
				   double densidade_fluido=1.0;

				   // dhol=f_cálculo_didufisidade_oll(ol);// ???
				   dolv=dif_vapor_ar*fator_experimental*densidade_vapor_saturado
				   *doll/densidade_fluido;// ?
				   Dol2=64.868427*conteudo_ol*conteudo_ol-11.32096*conteudo_ol-9.075419;
				   if (Dol2>(-7.0)) Dol2=-7.0;
				   Dol2=pow(10,Dol2)*0.7;  // invencao do BELLINI

				   if (dolv<Dol2) dolv=Dol2;
				   else dolv=dolv+2.0*Dol2/3.0;
				 */
  dolv = 1e-8;
  return (dolv);
}

// ---------------------------------------------------------------------------
// Chama as funcoes de cálculo desta classe.
// Observe que a ordem pode ser importante.
// os dois parametros recebidos sao v1 temperatura e v2 conteudo_ol
// Obs: a temperatura do nodo e a temperatura do meio poroso
// e a temperatura do fluido sao as mesmas.
// O conteudo_ol s� tem sentido para esta classe
// Observe ainda que para cálculo das propriedades do fluido � necess�ria
// a umidade, assim com o conteudo_ol calcula a umidade e joga como
// parametro para o objeto material fluido.
void
CMaterialMeioPorosoAmostra::Calcula_propriedades_material (double *&v)
{
  // primeiro passo, armazena os parametros passados pelo no
  // e necessario pois oa atributos do material dependem de T e ol.
  conteudo_ol = v[0];		// calculado externamente, conteudo do nodo
  temperatura = v[1];		// calculado externamente,temperatura do nodo

  // Executa funcoes do objeto todo parte material fluido
  // Precisamos passar a umidade para o objeto fluido MF
  // para tal usa-se a funcao f_umidade, que recebe a umidade.
//  MF->f_umidade(umidade);      // passa para objeto MF a umidade (sem utilidade)
  MF->Calcula_propriedades_material (v);	// a funcao de cálculo da condutividade
  // do meio poroso amostra, necessita das propriedades do fluido

  // executa funcoes do  objeto todo parte TMaterial Meio Poroso (A seco)
  MMP->Calcula_propriedades_material (v);	// a funcao de cálculo da condutividade
  // do meio poroso amostra, necessita das propriedades do MMP

  // Depois calcula conteudo_wl (usado no cálculo da umidade, funcao f_adsorcao)
  conteudo_wl = conteudo_ol * MF->Densidade () / MMP->Densidade ();

  // Depois calcula a umidade chamando f_adsorcao
  // observe que o atributo umidade e definido no objeto todo parte
  // MF e nesta classe.
  Adsorcao ();		// calcula a umidade em funcao do conteudo_wl
  // f_dessorcao(); nao implementada
  Difusividade_massica ();	// como esta calcula a difusividade massica
  // como funcao somente do conteudo_ol.
  // deve chamar as funcoes intermediarias.

  Concentracao_vapor ();	// calcula a concentracao do vapor funcao da temperatura e umidade
  // temperatura foi dado entrado, a umidade calculada por f_adsorcao.
  // precisa da pressao do vapor saturado, valor calculado no objeto MF

  // Executa funcoes definidas na classe base f_calcula_propriedades_material
  TMaterial::Calcula_propriedades_material (v);
  // densidade     funcao de MMP e MF
  // calor especifico funcao de MMP e MF
  // condutividade funcao de MMP e MF
  // difusividade termica funcao da condutividade
  // Executa funcoes novas desta classe.
  // Os metodos abaixo devem posteriormente serem verificados e implementados

  // Calcula das curvas de difusividade
  // A funcao de cálculo da difusividade e' que deve chamar estas funcoes
  // na ordem correta
  // f_difusividade();????
  // f_cálculo_difusividade_dtl();
  // f_cálculo_difusividade_dtv();
  // f_cálculo_difusividade_doll();
  // f_cálculo_difusividade_dolv();

  // metodos novos desta classe ainda nao calculados
  // fator_experimental=p_fe.f_calcular(conteudo_ol);
}

// --------------------------------------------------------------------------
// Funcao da classe base Banco_de_dados, redefinida
// cuidado com o uso de ptr.
void
CMaterialMeioPorosoAmostra::Ler_disco ()
{
  strcpy (ext, "mpa");		// extensao
  size_obj = sizeof (CMaterialMeioPorosoAmostra);
  ptr_obj = ptr;		// ptr_obj
  CBancoDeDados::Ler_disco ();	// chama funcao da classe
  // base  Banco_De_Dados
}

// --------------------------------------------------------------------------
// Funcao da classe base Banco_de_dados, redefinida
// cuidado com o uso de ptr.
// Com o uso de dois objetos est�ticos, Estatico_MF e Estatico_MMP
// os dois sao automaticamente salvos ao se salvar o MMPA.
void
CMaterialMeioPorosoAmostra::Salvar_disco ()
{
  strcpy (ext, "mpa");
  size_obj = sizeof (CMaterialMeioPorosoAmostra);
  ptr_obj = ptr;
  CBancoDeDados::Salvar_disco ();	// chama funcao da classe base
  // Banco_De_Dados
}
