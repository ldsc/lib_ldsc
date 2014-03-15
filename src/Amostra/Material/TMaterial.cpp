
#include <TMaterial/CMaterial.h>
//#include <TMaterial/CMaterialFluido.h>
//#include <TMaterial/CMaterialMeioPoroso.h>
//#include <TMaterial/CMaterialMeioPorosoAmostra.h>

//--------------------------------------------------------------------------
//atributo estatico precisa ser definido fora da classe
int  CMaterial::tipo_de_material = 1;
CMaterial *  CMaterial::ptr = NULL;	//antes era estatico

//--------------------------------------------------------------------------
//Construtor da classe CMaterial.                  //construtor
CMaterial::CMaterial ()
{
  strcpy (nome_material, "nome_material");	//[0]='\0';//new char [30];
  //cout<<"\n\tCriou  objeto base CMaterial("<<sizeof(CMaterial)<<")";
}

//--------------------------------------------------------------------------
//Destrutor da classe material                    //destrutor
CMaterial::~CMaterial ()
{
  //delete nome_material;
  //cout<<"\n\tDeletou objeto base CMaterial";
}

//---------------------------------------------------------------------------
//Sobrecarga operadores cin>> e cout<< com o uso do template
//O segundo parametro e' um ponteiro para um objeto material.
istream & operator >> (istream & op, CMaterial * &c);
ostream & operator << (ostream & op, const CMaterial * &c);

//---------------------------------------------------------------------------
//Chama definicao dos atributos, mostra os atributos, e os modifica ou nao.
void
CMaterial::Entrada ()
{
  Default ();			//inicializa valores dos atributos
  Carimbo ();
  Saida ();			//apresenta os valores dos atributos
  Pergunta ();		//pergunta se deseja alterar atributos
  //ciclo de modificao dos atributos at�
  //que estejam corretos
  while (resp == 's' || resp == 'S')
    {
      Modificar ();		//modifica atributos
      Carimbo ();
      Saida ();		//mostra atributos
      Pergunta ();		//pergunta se deseja alterar estes dados
    }
}

//---------------------------------------------------------------------------
//Armazena valores default dos atributos
void
CMaterial::Default ()
{
  strcpy (nome_material, "nome_material");
  temperatura = 25.0;
  conteudo_ol = 0.0;		//definido aqui porque o ponteiro usa a classe base.
  densidade = 1700.0;
  calor_especifico = 1000.0;
  condutividade = 0.743;
  difusividade_termica = 0.743 / 1700.0;	//k/ro
  absortividade = 0.6;
  emissividade = 1.0;
}

//---------------------------------------------------------------------------
//Mostra os valores dos atributos
void
CMaterial::Saida ()
{
  //f_carimbo(); nao porque apaga informação do objeto filho
  cout << "\n\tAtributos Classe Base CMaterial:";
  cout << "\n\tnome_material=" << nome_material;
  cout << "\n\ttemperatura=" << temperatura;
  //cout<<"\n\tconteudo_ol="<<conteudo_ol;//definido aqui
  cout << "\n\tdensidade=" << densidade;
  cout << "\n\tcalor_especifico=" << calor_especifico;
  cout << "\n\tcondutividade=" << condutividade;
  cout << "\n\tdifusividade_termica=" << difusividade_termica;
  cout << "\tabsortividade=" << absortividade;
  cout << "\n\temissividade=" << emissividade;

}

//---------------------------------------------------------------------------
//pergunta se deseja alterar os atributos
void
CMaterial::Pergunta ()
{
  cout << "\n\tDeseja alterar estes atributos (s/n)? ";
  cin.get (resp);		//pega a resposta
  cin.get ();			//pega o retorno de carro
}

//---------------------------------------------------------------------------
//Modifica os valores dos atributos
void
CMaterial::Modificar ()
{

  cout << "\n\tAtributos da Classe base CMaterial, Entre com os atributos:";
  cout << "\n\tnome_material(sem espacos):";
  cin.get (nome_material, 29, '\n');	//ate o retorno de carro
  cin.get ();			//pega o retorno de carro
  //cin.get(nome_material,30,'\n');
  //cin.get();//pega o retorno de carro
  cout << "\ttemperatura:";
  cin >> temperatura;
  //cout<<"\tconteudo_ol :";
  //cin>>conteudo_ol;
  cout << "\tdensidade:";
  cin >> densidade;
  cout << "\tcalor_especifico:";
  cin >> calor_especifico;
  cout << "\tcondutividade:";
  cin >> condutividade;
  cout << "\tdifusividade_termica:";
  cin >> difusividade_termica;
  cout << "\tabsortividade:";
  cin >> absortividade;
  cout << "\temissividade:";
  cin >> emissividade;
  cin.get ();
}

//---------------------------------------------------------------------------
/*
//Funcao de retorno do atributo temperatura.
double CMaterial::f_temperatura()
{
 return (temperatura);
}
*/

//---------------------------------------------------------------------------
//Calcula e retorna a densidade do material
double
CMaterial::Densidade ()
{				//densidade=
  return (densidade);
}

//---------------------------------------------------------------------------
//Calcula e retorna o calor espec�fico do material
double
CMaterial::Calor_especifico ()
{				//calor_especifico=
  return (calor_especifico);
}

//---------------------------------------------------------------------------
//calcula e  retorna a condutividade do material
double
CMaterial::Condutividade ()
{				//condutividade=p_k[0]+p_k[1]*ol;
  return (condutividade);
}

//---------------------------------------------------------------------------
//Calcula e retorna a difusividade_termica
double
CMaterial::Difusividade_termica ()
{				//difusividade_termica=
  return (difusividade_termica);
}

//---------------------------------------------------------------------------
//Calcula e retorna a difusividade_massica
//Definida aqui por causa do polimorfismo
double
CMaterial::Difusividade_massica ()
{				//difusividade_massica=
  double difusividade_massica = 0.0;
  return (difusividade_massica);
  //Apenas para efeito de polimorfismo, e' definida uma variavel local.
  //verificar porque isto teve de ser feito a luz da POO.
}

//---------------------------------------------------------------------------
//Definida aqui por causa do polimorfismo
double
CMaterial::Pressao_vapor_saturado ()
{
  double pressao_vapor_saturado = 0.0;
  return (pressao_vapor_saturado);

}

//---------------------------------------------------------------------------
//calcula e retorna a absortividade do material
double
CMaterial::Absortividade ()
{
  //absortividade=funcao de algo como temperatura
  return (absortividade);
}

//---------------------------------------------------------------------------
//Calcula e retorna a emissividade do material
double
CMaterial::Emissividade ()
{				//emissividade=
  return (emissividade);
}

//---------------------------------------------------------------------------
//Usada na Classe Material_Fluido
//Definida na classe base para permitir o polimorfismo
//void  CMaterial::f_umidade(double h)
//{
//}

//---------------------------------------------------------------------------
//Executa todas os metodos de cálculo da classe CMaterial
//Observe que pode haver interdependencia entre os metodos
//neste caso voce deve respeitar a sequencia de cálculo.
//v[1]=temperatura, v[0]=conteudo de umidade ol ou umidade
void
CMaterial::Calcula_propriedades_material (double *&v)
{
  //armazena valores das propriedades temperatura e conteudo de umidade
  temperatura = v[1];
  //conteudo_ol=v[0];   //verificar porque foi incluido aqui?

  //Chama as funcoes de calcula das variaveis                                                                                                    //Pense em eliminar esta funcao
  Densidade ();		//fixa ou funcao do conteudo de umidade                   //cada objeto no, chamaria a f_densidade
  Calor_especifico ();	//fixa ou funcao do conteudo de umidade                   //diretamente, so chamando o que fosse
  Condutividade ();		//fixa ou funcao do conteudo de umidade                   //necessario
  Difusividade_termica ();	//idem
  //f_absortividade();  //fixa
  //f_emissividade();   //funcao da temperatura usada no cálculo e*sigma*t^4
}

//--------------------------------------------------------------------------
//Funcao da classe virtual Banco_de_dados
//redefinida para ler objetos com a extensao mat de material.
//Cuidado com o uso de ptr visto que o mesmo � est�tico.
//Se existir um �nico objeto material nao vai ocorrer problema, mas com
//mais de um deve-se adotar uma formulacao diferenciada.
void
CMaterial::Ler_disco ()
{
  strcpy (ext, "mat");
  size_obj = sizeof (CMaterial);
  ptr_obj = ptr;
  TBancoDeDados::f_ler_disco ();	//chama funcao da classe
  //base  Banco_De_Dados
}

//--------------------------------------------------------------------------
//Funcao da classe virtual Banco_de_dados
//redefinida para salvar objetos com a extensao mat de material.
void
CMaterial::Salvar_disco ()
{
  strcpy (ext, "mat");
  size_obj = sizeof (CMaterial);
  ptr_obj = ptr;
  TBancoDeDados::f_salvar_disco ();	//chama funcao da classe base
  //Banco_De_Dados
}

//---------------------------------------------------------------------------
//Funcao de selecao do tipo de material a ser criado.
CMaterial *
CMaterial::Selecao_tipo_de_material ()
{
  Carimbo ();
  cout << "\n\t\tSelecione o tipo de material"
    << "\n\tMaterial ......................1"
    << "\n\tMaterial Meio Fluido...........2"
    << "\n\tMaterial Meio Poroso...........3"
    << "\n\tMaterial Meio Poroso Amostra...4:";
  cin >> tipo_de_material;
  cin.get ();
  switch (tipo_de_material)
    {
    case 1:
      ptr = new CMaterial;
      break;
    case 2:
      ptr = new CMaterialFluido;
      break;
    case 3:
      ptr = new CMaterialMeioPoroso;
      break;
    case 4:
      ptr = new CMaterialMeioPorosoAmostra;
      break;
    }

  Memoria (ptr, "CMaterial");	//f_memoria testa se ptr foi corretamente alocado
  return ptr;
}


//---------------------------------------------------------------------------
/*
main()
{
//-------------------------------
clrscr();cout<<"\n\tVai Criar objeto material";
CMaterial* 	      	objeto_M=new CMaterial;
cin.get();

clrscr();cout<<"\n\tVai Criar objeto Material_Meio_Poroso";
CMaterial* 		objeto_Mp=new Material_Meio_Poroso;
cin.get();

clrscr();cout<<"\n\tVai Criar objeto CMaterialFluido";
CMaterial* 		objeto_Mf=new CMaterialFluido;
cin.get();

clrscr();cout<<"\n\tVai Criar objeto Material_Meio_Poroso_Amostra";
CMaterial*   		objeto_Mpe=new Material_Meio_Poroso_Amostra;
getch();

//-------------------------------
clrscr();
cout<<"\n\tVai executar funcoes de entrada do objeto material";
cin.get();
cin>>objeto_M;//objeto_M->f_entrada();

clrscr();
cout<<"\n\tVai executar funcoes de entrada do objeto material meio poroso";
cin.get();
cin>>objeto_Mp;//objeto_Mp->f_entrada();

clrscr();
cout<<"\n\tVai executar funcoes de entrada do objeto material fluido";
cin.get();
cin>>objeto_Mf;//objeto_Mf->f_entrada();

clrscr();
cout<<"\n\tfuncoes de entrada do objeto material meio poroso amostra";
cin.get();
cin>>objeto_Mpe;//objeto_Mpe->f_entrada();

//-------------------------------
clrscr();cout<<"\n\tVai deletar os objetos criados";
cin.get();

delete objeto_M;
cin.get();
delete objeto_Mp;
cin.get();
delete objeto_Mf;
cin.get();
delete objeto_Mpe;//deleta classe material meio poroso amostra
		  //deleta objeto material meio poroso +classe base
		  //deleta objeto material fluido      +classe base
		  //deleta entao classe base material
cin.get();
return(0);
}
*/
/*
obs: observe que mesmo que o atributo seja calculado;
	  � importante que o usuário entre com o seu valor
     na funcao f_modificar. O usuário pode estar  definindo os atributos
	  para que os mesmos sejam armazenados   em disco.

palavras chaves para busca:
	mudar=algo que deve ser posteriormente modificado
      analisar=verificar l�gica matematica e de programacao
      atualisar=significa que � uma versao antiga
      matematica?,fisica?=significa que devem existir erros f�sicos

Obs: A tentativa de acessar um atributo de uma classe derivada_h_teta com um ponteiro
	  para a classe base da em erro.
     O ponteiro para a classe base s� tem acesso aos atributos da classe base.
	  E por este motivo que a definicao de atributos das classes derivadas
	  e realizada com uma funcao.
	  Foi criada uma funcao f_umidade para passagem do atributo umidade,
	  como esta funcao nao esta na classe base a mesma nao foi aceita.
	  A solucao adotada foi criar um vetor auxiliar e passar este vetor
	  como parametro na chamada da funcao f_calcula_material
	  do objeto MF.
     A forma mais correta seria definir a funcao f_umidade na classe
     base.

	  ex: na classe CMaterialFluido foi criada a funcao f_umidade
     que � utilizada para entrada da variavel umidade.



Obs:
  //Na classe material meio poroso amostra, foi definido o atributo
  //volume, se referindo ao volume da amostra.
  //O mais correto seria  criar uma classe amostra
  //com definicoes de atributos como
  //origem da amostra, volume, dimensoes X,Y,Z,
  //forma da amostra (quadrada, retangular, cilindrica, irregular...
  //Forma de obtencao da amostra....
  */
	       
