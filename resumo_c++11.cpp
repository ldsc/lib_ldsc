/**
===============================================================================
Lista com as principais novidades de C++11
Autor: André Duarte Bueno.
===============================================================================
Lista com as principais novidades de C++11 -> Núcleo da linguagem
=============================================================================== */
// [ ] ---------------------------------------------------------------------> auto 
auto x = 3;

// [ ] ---------------------------------------------------------------------> decltype 
decltype(x) y;

// [ ] ---------------------------------------------------------------------> nullptr
char *pc = nullptr;     // OK
int  *pi = nullptr;     // OK

// [ ] ---------------------------------------------------------------------> rvalue reference
template<typename T> void f(T&& param);                // parâmetro do tipo rvalue reference
template<typename T> void f(std::vector<T>&& param);   // parâmetro do tipo rvalue reference
int main() { int&& var1 = 10;                          // rvalue reference
cout << "\nvar1 = " << var1 ; }

// [ ] ---------------------------------------------------------------------> novas strings
char     s8[]  = u8"UTF-8 cstring. ";  // const char[].
char16_t s16[] = u"UTF-16 cstring. ";  // const char16_t[]. 
char32_t s32[] = U"UTF-32 cstring. ";   // const char32_t[].
cout << u8"This is a Unicode Character: \u2018"       << endl;

// [ ] ---------------------------------------------------------------------> raw strings 
// C++11 fornece a opção de raw strings literal: R("string"), não é interpretada
cout <<  "(\a xx \b yy \t zz \n)" << endl;
cout << R"(\a xx \b yy \t zz \n)" << endl;

// [ ] ---------------------------------------------------------------------> sizeof
// C++11 permite obter o sizeof de membro da classe
cout << "sizeof(CPonto) = "    << sizeof(CPonto)<< endl;
cout << "sizeof(CPonto::x) = " << sizeof(CPonto::x)<< endl;

// [ ] ---------------------------------------------------------------------> enum class
// A vantagem é que a mesma não pode ser convertida para int; 
enum class EDiaSemanaCpp11 { segunda = 2, terca = 3, quarta = 4, quinta = 5, sexta = 6 };

// [ ] ---------------------------------------------------------------------> enum class: tipo
// Também podemos definir o tipo usado pela enumeração
enum class EMesesAnoCpp11: unsigned int { janeiro = 1, fevereiro, marco, abril, 
maio, junho, julho, agosto, setembro, outubro, novembro, dezembro };

// [ ] ---------------------------------------------------------------------> union 
union UMiscelanea {   bool b;  int i;  double d;
  CPonto p;   // C++11 retirou restrição dos tipos aceitos em uniões
  UMisccelanea() { new( &p ) CPonto(); } };

// [ ] ---------------------------------------------------------------------> static_assert 
static_assert (constant-expression, error-message);
static_assert(sizeof(int) <= sizeof(T), "A dimensão de T não é suficiente!");
static_assert(std::is_integral<T>::value, "O parâmetro de f deve ser do tipo integral.");
static_assert((pi < 3.14) && ( pi > 3.15), "Aumentar precisão de pi!");

// [ ] ---------------------------------------------------------------------> constexpr
constexpr int FuncaoConstante() { return 5; }
constexpr int XY (int x, int y) { return x * y; }
constexpr double aceleracaoGravidade = 9.8;

// [ ] ---------------------------------------------------------------------> range based for
int vetor_estilo_c[5] = { 1, 2, 3, 4, 5 };
for (int &elemento_vetor : vetor_estilo_c) cout << elemento_vetor << endl;
vector<int> v(5);
for( auto elemento_vetor : v )             cout << elemento_vetor << endl;

// [ ] ----------------------------------------------------------------> Inicialização uniforme
int x3{5.0}, x4 ={5.3}; // Com inicialização uniforme de C++11 aponta erro
int x5, x6{}; 		// x5 valor indefinido, x6 valor padrão = 0
int *ptr1,*ptr2{};    	// ptr1 valor indefinido, ptr2 em C++11 assume valor nullptr
char cx{14},cy{35000};  // cx OK, 14 é do tipo inteiro; cy Erro, estouro do intervalo
std::vector<int> vx { 0, 1, 2, 4, 5 };    // OK
std::vector<int> vy { 1, 2.3, 4, 5.6 };   // Erro
int vc1[3];   // Cria vetor estilo de C, com 3 elementos [0->2], valores indefinidosint vc2 
[]= {1,2,3};  // Cria vetor estilo de C, com 3 elementos [0->2], valores definidos

// [ ] -----------------------------------------------------------------> Lista inicialização
class CLista { private:  vector< float > v;
               public:   CLista( std::initializer_list<float> lista ):v(lista){};
                         vector< float > V()	{ return v; }; };

struct SPonto {  float x;   float y;        // Construtor
SPonto(float _x, float _y): x(_x), y(_y) { std::cout << "Passou pelo construtor de SPonto;";} };
SPonto GetSPonto() { return { 0.0, 0.9 }; } // O objeto é criado sem usarmos o tipo SPonto

void Saida ( std::initializer_list<int> l)  // Note que recebe como parâmetro a lista l.
{    for (auto it = l.begin(); it != l.end(); ++it) std::cout << *it << "\n"; }

int main() {                                // Chama função saida, imprime valores na tela
  Saida ({0,1,2,3,4,5,6,7,8,9});            // Usando initializer_list
  CLista lista{ 5.1, 5.2, 5.3, 5.4 };       // Cria objeto lista
  vector<int> v{ 1,2,3,4,5 };               // Usando initializer_list com biblioteca padrão

  SPonto p2  { 5.1 , 6.1 };                 // Usando inicialização uniforme padrão C++11
  // Criando uma lista de pontos usando inicialização uniforme padrão C++11
  SPonto lista_pontos[4] =  { { 5.2,6.2 }, {5.3,6.3}, {5.4,6.4}, {5.5,6.5} };
}

// [ ] ---------------------------------------------------------------------> Funções lambda
/*auto nomeFuncao = [captura](parametros)->tipoRetorno {corpo da funcao}
[]: Não capturar nada. 
[=]: Todas as variáveis externas são capturadas por valor.
[&]: Todas as variáveis externas são capturadas por referência.
[x, &y]: capturar x por valor(cópia) e y por referência.
[&, x]: Todas as variáveis externas são capturadas por referência, exceto x que é por valor.
[=, &z]: Todas as variáveis externas são capturadas por valor, exceto z que é por referência. */

// Função lambda anônima criada e já executada
[] { std::cout << "Função lambda criada e já executada" << std::endl;   } ();

// Função lambda criada e chamada a seguir
auto l = [] { std::cout << "Função lambda criada e chamada a seguir" << std::endl; };
l();                                                                // Chama função lambda

// Definição de função lambda que não captura nada e que não recebe parâmetros.
auto ptr_funcao = [] () { cout << "Olá mundo!\n"; };
ptr_funcao();

// Definição de função lambda que não captura nada e que recebe os parâmetros x e y.
auto ptr_funcao2 = [](int x, int y) { return x + y; }
cout << " x + y = " << ptr_funcao2(3,4) << endl;

// Usando função lambda com captura por referencia
int soma = 0; 
auto Soma = [&soma]( int x ) { soma += x; cout << "Soma = " << soma << endl; };
Soma(10);

// [ ] ---------------------------------------------------------------> novo formato funções
auto X(int _x) -> void 		{ x = _x; }
auto X() -> int			{ return x; }
auto Set(int _x, int _y) -> void;
auto CPonto::Set(int _x, int _y) -> void{ x = _x;  y = _y; }

// [ ] ---------------------------------------------------------------------> Ponteiro Função
void FC(int x)       { cout << "FC x=" << x << endl; } // // Declara e define função
void (*pFC)(int) = &FC;                       // ponteiro para função C++98
typedef void (*PonteiroFuncao)(double);       // ponteiro para função C++98

using PonteiroFuncao = void (*)(double);      // ponteiro para função C++11
std::function<void(int)> pF11 = &F11;         // ponteiro para função C++11
auto autopF11 = &F11;                         // ponteiro para função C++11

class C {    void F11(int x)   { cout << "F11 x=" << x << endl; } };
void (C::*pF03)(int) = &C::F03;               //  ponteiro para função C++98
std::function<void (C&, int)> pF11_ref = &C::F11; // funciona como referencia
std::function<void (C*, int)> pF11_ptr = &C::F11; // funciona como ponteiro


// [ ] ----------------------------------------------------------------> delegação construtor
explicit CPonto (int _x, int _y):x(_x),y(_y) 	{}
CPonto(int xy) : CPonto(xy,xy) 	{}  // um construtor chama o outro

// [ ] ---------------------------------------------------------------------> default e delete
class NonCopyable { public:  // Diz para o compilador desabilitar o operator= (não criar)
  NonCopyable& operator=(const NonCopyable&) = delete;
  
  // Diz para o compilador desabilitar o construtor de cópia (não criar)
  NonCopyable(const NonCopyable&) = delete;
  
  // Diz para o compilador criar o construtor default
  NonCopyable() = default; };

// [ ] ---------------------------------------------------------------------> override e final
class CPonto { virtual auto Entrada() -> void;
               virtual auto Saida() -> void; };

class CCirculo: public CPonto {
    virtual void Entrada() override ; // sobrecreve método virtual da classe base
    virtual void Saida() final; };    // última atualização de Saida

// [ ] ---------------------------------------------------------------------> for_each
char s[] = "Olá Mundo!";
for_each( s, s + sizeof(s), [] (char c){ cout << c << endl; });
int vc[] = { 1 , 2 , 3 , 4 , 5 , 6 , 7 };
for_each( begin(vc), end(vc), [](int x) { cout << x << ' ';});
int soma = 0;  
for_each( begin(vc), end(vc), [&soma] (int x) { soma += x;});




===============================================================================
Lista com as principais novidades de C++11 -> Biblioteca std
===============================================================================
// [ ] ---------------------------------------------------------------------> array
#include <array>
std::array<int, 4> array_4_int { {1,2,3,4} };    	// Precisa de duplo {}
array<int, 3> array_3_int = {1, 2, 3};   		// Apos = precisa {} simples
array<string, 2> array_2_string = {"a", "b"} ;
sort(array_4_int.begin(), array_4_int.end());
for(auto& ae: array_4_int)         cout << ae << ' ';

// [ ] --------------------------------------------------------------> all_of any_of none_of
void Teste( vector<int> &v , string msg )
{ cout << "Vetor " << msg << endl;
  if ( all_of(v.begin(), v.end(), [](int ev) { return ev > 0;} ) )   // Todos positivos?
    cout << "Todos positivos\n";
  if ( any_of(v.begin(), v.end(), [](int ev) { return ev > 0;} ) )   // Pelo menos um positivo?
    cout << "Pelo menos um positivo\n"; }
int main() {  vector<int> v1{   1, 2, 3, 4, 5};    Teste( v1 , "v1"  );
              vector<int> v2{ 0,-1, 2, 3, 4, 5};   Teste( v2  , "v2" ); }
              
// [ ] ---------------------------------------------------------------------> unique_ptr
std::unique_ptr<int> ptr_int3( new int(3) );     // Cria ponteiro e objeto
cout << *ptr_int3 << endl;                       // Usa
unique_ptr<int> ptr_int5  = std::move(ptr_int3); // Transfere propriedade
cout << *ptr_int5 << endl;                       // Usa 
ptr_int5.reset();                                // Destrõe
ptr_int3.reset();                                // Não faz nada.      

// [ ] ---------------------------------------------------------------------> shared_ptr
// Use shared_ptr​ quando quizer vários ponteiros apontando para mesmo objeto,
// somente quando o último for deletado o objeto será efetivamente deletado. 
shared_ptr<int> ptr_int6(new int(6));            // Cria ponteiro e objeto
cout << *ptr_int6 << endl;                       // Usa
shared_ptr<int> ptr_int7 = ptr_int6;             // ptr7 aponta mesmo objeto que ptr6
cout << *ptr_int7 << endl;
ptr_int6.reset();                                // Não destrõe e objeto 
cout << *ptr_int7 << endl;                       // Usa
ptr_int7.reset();                                // Agora deleta objeto

// [ ] ---------------------------------------------------------------------> weak_ptr
shared_ptr<int> ptr_int8(new int(8));
cout << *ptr_int8 << endl;
weak_ptr<int> wptr_int8 = ptr_int8;            // ptr_int8 owns the memory. 
  {
  shared_ptr<int> ptr_int9 = wptr_int8.lock(); // Agora p8 e p9 acessam a mesma memória.
  cout << *ptr_int9 << endl;
  if( ptr_int9 )                               // Sempre verifique se a memória 
    cout << *ptr_int9 << endl;                 // Faça algo com ptr_int9
  }     // ptr_int9 é destruído; ptr_int8 volta a ter a propriedade.
  cout << *ptr_int8 << endl; 
  ptr_int8.reset();                            // A memoria é deletada.
}

// [ ] ---------------------------------------------------------------------> function 
#include <functional>
function<double(double)> fx2 = [](double x) { return x*x;};  // funcao f
function<double(double)> f2x = [](double x) { return 2.0*x;};// funcao g
// Cada vez mais perto da notação matemática! Agrupando funções, como g(f(x));
std::function<double(double)> gf(function<double(double)> f, function<double(double)> g ) 
     { return [=](double x) { return g(f(x)); };
// Uso de gf, cria funcao fx4, retorna double, recebe funcao
function<double(double)> fx4 = gf(fx2, fx2);
int main() {    double x = 3;
   cout << "x = " << x << end;
   cout << "fx2 = " << fx2(x) << endl;
   cout << "fx4 = " << fx4(x) << endl; }

 // [ ] ---------------------------------------------------------------------> bind
// Declara função f que recebe dois parâmetros, um int e uma string
void fis( int x, string s) { cout << "int x = " << x << " string s = " << s << endl; }
int main() {
  fis( 2, " oi tudo bem ");
  // Cria ponteiro para função fis que recebe apenas a string
  std::function<void( string )> fs = std::bind(&fis, 3 , std::placeholders::_1);
  fs("Usando fs, passando apenas a string");
  // Cria ponteiro para função alternativa que recebe apenas o inteiro
  function<void( int )> fi = std::bind(&fis, "Usando fi" ,std::placeholders::_2);
  fi(7); }

// [ ] ---------------------------------------------------------------------> pair
// Mostra uso de tie com pair<> e equivalencia de pair com tuple
  pair<double,double> p = make_pair(1.1,2.2);
  cout << "get<0>(p) = " << get<0>(p) 	<< " , get<1>(p) = " 	<< get<1>(p) << endl;
  cout << "p.first   = " << p.first 	<< " , p.second  = " 	<< p.second << endl;

// [ ] ---------------------------------------------------------------------> tuple
// Mostra uso de tuple, get<>, tie, pair 
#include <tuple>
// Cria tuple com 3 doubles
tuple<double, double, double> notasJoao(8.7,4.2,5.7);
cout<< "\nJoao\n"
      << "P1: " << get<0>(notasJoao) << ", "	// Acesso aos elementos da tuple
      << "P2: " << get<1>(notasJoao) << ", "	// usando funcao get<indice>(objeto_tuple)
      << "P3: " << get<2>(notasJoao) << '\n';
std::get<2>(notasJoao) = 6.3; 		        // Nota p3 corrigida, usa referencia.
      
// Mostra uso da funcao tie() para obter, separadamente, os valores da tuple
double n1,n2,n3;
tie(n1, n2, n3) = notasJoao;
cout<< "\nJoao\n" << "n1: " << n1 << ", " << "n2: " << n2 << ", " << "n3: " << n3 << '\n';

// [ ] ---------------------------------------------------------------------> forward_as_tuple 
// forward_as_tuple cria objeto temporario que funciona como uma tupla
// para objetos rvalue (right value). Note que como sao rvalue, nao alocam espaco em disco; 
#include <tuple>	// std::tuple e std::make_tuple
// Note que os parametros da tuple sao right value
void print_pack (std::tuple<std::string&&,double&&> pack) 
{   std::cout << std::get<0>(pack) << ", " << std::get<1>(pack) << std::endl; }
int main() { print_pack (std::forward_as_tuple(string("Joao"), 8.7)); }

// [ ] ---------------------------------------------------------------------> remove_if
bool is_even(int N) { return N % 2 == 0; } // Retorna verdadeiro se for impar
int main() {   vector<int> v{1,2,3,4,5,6};
  for_each (v.begin(),v.end(),[](int ev){ cout << ev << '\t'; });  // Vetor v antes de remove_if
  remove_if(v.begin(),v.end(),is_even);  
  for_each (v.begin(),v.end(),[](int ev){ cout << ev << '\t'; });  // Vetor v depois de remove_if

  // Efetivamente remove elementos no intervalo final do vetor
  v2.erase(remove_if(v2.begin(), v2.end(), is_even), v2.end());  }

// [ ] ---------------------------------------------------------------------> random
// O gerador números randomicos tem duas partes; um motor que gera números randomicos 
// e uma distribuição matemática. 
// Motores: linear_congruential_engine,subtract_with_carry_engine e mersenne_twister_enginee.
// Distribuições: uniform_int_distribution, uniform_real_distribution, 
// bernoulli_distribution, binomial_distribution, geometric_distribution, poisson_distribution,
// normal_distribution, student_t_distribution, chi_squared_distribution, 
// exponential_distribution, gamma_distribution, lognormal_distribution, 
// cauchy_distribution, lognormal_distribution, weibull_distribution, 
// extreme_value_distribution, fisher_f_distribution, negative_binomial_distribution,  
// discrete_distribution, piecewise_constant_distribution, piecewise_linear_distribution.

#include <random>
int main()
{ uniform_int_distribution<int> distribuicao(-20, 20); // Cria distribuição uniforme
  mt19937 motor;                                       // Cria motor "Mersenne twister MT19937"
  int numeroRandomico = distribuicao(motor);           // Gera número aleatório

  std::normal_distribution<double>  normal(0.0,1.0);   // Normal, media 0 e desvio padrao 1
  cout << " media = " << normal.mean() << " desvio padrao = " << normal.stddev() 
       << " max = " << normal.max() << " min = " << normal.min() << endl;
  normal = normal_distribution<double>(12,3);          // Seta media = 12 e desvio padrao = 3

std::default_random_engine motor2;           // Cria motor, usa default
auto Normal = std::bind(normal, motor2);     // Cria gerador de número aleatorio 
vector<double> vna(500);                     // Cria vetor de numeros aleatorios 
for( double &v : vna )	v = geradorNormal(); // Gera números aleatóros
}








// [ ] ---------------------------------------------------------------------> chrono
#include <chrono> // Biblioteca date time no C++11
#include <ctime>  // Biblioteca date time no C
// Cria objeto time_point
  chrono::time_point<chrono::system_clock> start;
// Define valor de start como sendo agora (antes do processamento)
  start = chrono::system_clock::now();
// Chama função com determinado tempo de processamento
  int result = sin(45);  
// Define valor de end como sendo agora (depois do processamento)
  auto end = chrono::system_clock::now();
// count() retorna numero ticks, a diferença é convertida em segundos.
  int elapsed_seconds = chrono::duration_cast<chrono::seconds>(end-start).count();
  time_t end_time = chrono::system_clock::to_time_t(end);
  cout << "Computação terminada em  " << ctime(&end_time)
       << "tempo(s) decorrido : " << elapsed_seconds << "s\n";

// [ ] ---------------------------------------------------------------------> regex (-lregex)
#include <regex>	// regex, replace, match_results
// regex - Classe que representa uma Expressão Regular - ER.
// match_results - representa as ocorrências, casos em que a ER foi encontrada.
// regex_search - função usada para localizar uma ocorrência da ER.
// regex_replace - função que substitue a ocorrência encontrada por outro texto.
// As funções regex_search e regex_replace recebem uma expressão regular e uma stringe escrevem as 
// ocorrências encontradas na estrutura match_results. 
int main(){
if (regex_match ("Palmeiras, Campeão Mundial 1951", regex("r") ) )
 cout << "\nA expressão regular \"(ras)\" foi encontrada em \"Palmeiras, Campeão Mundial 1951\"";

// A procura pela expressao regular er, sera feita em s pela funcao regex_match.
string s ("Palmeiras campeão mundial");		// string a ser pesquisada
regex er ("r)");				// expressao regular usada na pesquisa
if (regex_match (s,er))				// faz a procura
 cout << "\nA expressão regular \"(ras)\" foi encontrada em \"Palmeiras, Campeão Mundial 1951\"";
  
// Faz a procura usando iteradores
if ( regex_match ( s.begin(), s.end(), er ) )    cout << "range matched\n";

// o mesmo que match_results<const char*> cm;
cmatch cm;    
regex_match ("Palmeiras, Campeão Mundial 1951",cm,er);
cout << "string literal with " << cm.size() << " matches\n";

// o mesmo que match_results<string::const_iterator> sm;
smatch sm;    
regex_match (s,sm,er);
cout << "string object with " << sm.size() << " matches\n";

regex_match ( s.cbegin(), s.cend(), sm, er);
cout << "Usando intervalo, foram encontradas " << sm.size() << " ocorrências\n";

// usando os flags de forma explicita:
regex_match ( "subject", cm, er, regex_constants::match_default );
cout << "As ocorrências são: ";
for (unsigned i=0; i<sm.size(); ++i) {    cout << "[" << sm[i] << "] ";  }
}
---
#include <regex> 
int main(){ std::string fnames[] = {"foo.txt", "bar.txt", "zoidberg"}; 
std::regex txt_regex("[a-z]+\\.txt");
for (const auto &fname : fnames) 
        std::cout << fname << ": " << std::regex_match(fname, txt_regex) << '\n';}

// [ ] ---------------------------------------------------------------------> 
