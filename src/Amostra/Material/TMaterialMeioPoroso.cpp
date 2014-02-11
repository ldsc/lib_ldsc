
#include <TMaterial/CMaterialMeioPoroso.h>

// ---------------------------------------------------------------------------
// Sobrecarga operadores cin>> e cout<<     template
ostream & operator << (ostream & op, CMaterialMeioPoroso * &c);
istream & operator >> (istream & op, CMaterialMeioPoroso * &c);

// --------------------------------------------------------------------------
// Construtor
CMaterialMeioPoroso::CMaterialMeioPoroso ()
{
  // cout<<",TMaterial Meio Poroso("
  //                      <<sizeof(CMaterialMeioPoroso)<<")";
}

// --------------------------------------------------------------------------
// Destrutor
CMaterialMeioPoroso::~CMaterialMeioPoroso ()
{
  // cout<<",CMaterialMeioPoroso";
}

// ---------------------------------------------------------------------------
// Inicializa atributos
void
CMaterialMeioPoroso::Default ()
{
  TMaterial::Default ();	// funcao da classe base
  densidade = 1730;		// atributo redefinido
  calor_especifico = 1000;	// atributo redefinido
  porosidade = 0.33;		// atributo novo desta classe
}

// ---------------------------------------------------------------------------
// Mostra os atributos
void
CMaterialMeioPoroso::Saida ()
{
  // cout<<"\n\tObjeto CMaterialMeioPoroso (atributos da classe base e novos).";
  TMaterial::Saida ();	// funcao da classe base
  cout << "\n\n\tAtributos novos da classe CMaterialMeioPoroso:";
  cout << "\n\tporosidade=" << porosidade;
}

// ---------------------------------------------------------------------------
// modifica os atributos
void
CMaterialMeioPoroso::Modificar ()
{
  // cout<<"\n\tObjeto CMaterialMeioPoroso atributos da classe base.";
  TMaterial::Modificar ();	// funcao da classe base
  cout << "\n\tAtributos novos da Classe CMaterialMeioPoroso:";
  cout << "\n\tporosidade:";
  cin >> porosidade;
  cin.get ();
}

// ---------------------------------------------------------------------------
// O m‚todo f_porosidade calcula a porosidade e a retorna
double
CMaterialMeioPoroso::Porosidade ()
{
  // porosidade=funcao de algo
  return (porosidade);
}

// ---------------------------------------------------------------------------
// Executa as funcoes da classe CMaterialMeioPoroso
// observe que a sequencia pode ser importante
// foi redefinida para poder chamar a funcao de calculo da porosidade
void
CMaterialMeioPoroso::Calcula_propriedades_material (double *&v)
{
  // calcula propriedades da classe base
  TMaterial::Calcula_propriedades_material (v);


  // calcula propriedades novas definidas nesta classe
  Porosidade ();
}

// --------------------------------------------------------------------------
// Funcao da classe virtual Banco_de_dados
// redefinida para ler objetos com a extensao mp de material.
void
CMaterialMeioPoroso::Ler_disco ()
{
  strcpy (ext, "mp");
  size_obj = sizeof (CMaterialMeioPoroso);
  ptr_obj = ptr;
  CBancoDeDados::Ler_disco ();	// chama funcao da classe
  // base  Banco_De_Dados
}

// --------------------------------------------------------------------------
// Funcao da classe virtual Banco_de_dados
// redefinida para salvar objetos com a extensao mp de material.
void
CMaterialMeioPoroso::Salvar_disco ()
{
  strcpy (ext, "mp");
  size_obj = sizeof (CMaterialMeioPoroso);
  ptr_obj = ptr;
  TBancoDeDados::Salvar_disco ();	// chama funcao da classe base
  // Banco_De_Dados
}
