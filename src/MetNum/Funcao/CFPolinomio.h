#ifndef   CFPolinomio_h
#define    CFPolinomio_h
/*
// ----------------------------------------------------------------------
                       framework    CFPolinomio
// ----------------------------------------------------------------------
Cria o conceito de polinomio e de algumas funcoes básicas
 muito utilizadas em cálculos científicos como polinomios
 funcoes senoidais, cossenos, tangentes, entre outras.

Um objeto polinomio deve ter definido o seu grau, e permitir
 a alteracao dos coeficientes do polinomio, o cálculo do polinomio
 e a visualizacao dos valores dos coeficientes dos polinomios.

Exemplos de polinomios
 polinomio 1g	 y=p[0]+p[1]*x
 polinomio 2g  y=p[0]+p[1]*x+p[2]*x
 polinomio 3g  y=p[0]+p[1]*x+p[2]*x*x+p[3]*x*x*x
*/

// ----------------------------------------------------------------------
//                              BIBLIOTECAS
// ----------------------------------------------------------------------
#include <fstream>		// por causa ifstream e ofstream ponteiros arquivos

#include <cmath>		// por causa funãoes matematicas

#include <Base/CFuncao.h>

/*
// ----------------------------------------------------------------------
                             CFPolinomio
// ----------------------------------------------------------------------
*/
/**
 * @brief Representa um polinômio.
 * 
 * Esta classe é utilizada para a manipulacao de polinomios
 * Um polinomio é usado para realizacao de algum tipo de cálculo
 * Aloca espaco para um polinomio de tamanho n
 * inicializa com zeros os coeficientes dos polinomios
 * 
 * Obs:          Algumas Observacoes:
 * 0-Criar classe base para  as funcoes cíclicas (seno, cossenno, tengente)
 * Verificar se vale ou nao a pena.
 * 1-Outras funcoes podem ser agora incluídas, utilizando o mesmo processo
 * ex: cossenos, tangentes, hiperbolicos,...
 * 2-foi incluido o metodo Set(double* ptr ), que permite a entrada dos
 * valores do polinomio, de modo que o programa
 * principal pode criar um objeto polinomio e depois  passar os seus
 * atributos atraves da funcao f_definir.
 * Observe que isto nao impede que os mesmos sejam posteriormente
 * modificados.
 * 
 * 4-Implementar funcoes Write e Read
 * Para salvamento e leitura em disco.
 * 
 * 5- Definir em todas as classes a heranca da interface
 * como virtual, Classe X:virtual public Interface.
 * 
 * 7- A funcao Set pode ter como parametro um ponteiro para doubles
 * e em funcao do grau do polinomio, definir cada coeficiente
 * do polinomio pelo ponteiro passado
 * ex:
 * void Set(double* p)
 * {
 * for(ii=0;ii<=grau;ii++)
 * p[ii]=p[ii];
 * }
 * 
 * A dificuldade é que voce terá de definir um vetor de doubles
 * dentro do programa e entao passar este vetor.
 * Ex:
 * double *vv=new double[n];
 * vv[0]=123; vv[1]=455; vv[2]=654;...
 * polinomio.Set(vv);
 * e como esta
 * polinomio.definir(123,455,654,..) esta ótimo.
 *
 * 
 * 8-	Recebendo como parametro a variavel x.
 * double=objetopolinomio.fcalcular(x);
 * sobrecarga      double<<objetopolinomio;
 * friend double operator<<(double r,Polinomio p)
 * 
 * 9- A tentativa de sobrecarga do operador () para substituir
 * a funcao de calcula nao dá certo.
 * Isto se deve a dois motivos:
 * 1- O operador () só pode ser sobrecarregado como funcao membro
 * 2- A sobrecarga como funcao membro recebe um objeto implicitamente
 * e nao um ponteiro para um objeto.
 * @author André Duarte Bueno
 * */
class CFPolinomio : public CFuncao
{

protected:

  double *p;	///< Ponteiro para vetor de doubles

  int grau;	///< Grau do polinomio

  static double xx, xxx; ///< Valores intermediários

public:
  double resultado;	///< Resultado do polinomio para um dado x

public:
	/// Construtor, recebe n, o grau do polinomio
    CFPolinomio (unsigned int n);	

    /// Destrutor
    virtual ~ CFPolinomio ();	

    /// Inicializa atributos
  virtual void Zera ();		

  /// Mostra o polinomio
  virtual void Saida ();	

  /// Entrada coeficientes
  virtual void Entrada ();

  // sobrecarga operadores >>e<<
  // virtual ostream& operator <<(ostream& op,CFPolinomio*& c);// sobrecarga de cout<<
  // virtual istream& operator >>(istream&op,CFPolinomio*& c);// sobrecarga de cin>>
  // friend  ostream& operator <<(ostream&,CFPolinomio*&);
  // friend  istream& operator >> (istream&,CFPolinomio*&);
  /// Passa para o polinomio um vetor de coeficientes, do mesmo tamanho do polinomio
  void Set (double *ptr);	

  /// A funcao definir é usada para passar mais facilmente os parametros para o polinomio de grau até 6
  void Definir (double x0, double x1 = 0, double x2 = 0, double x3 =
		0, double x4 = 0, double x5 = 0, double x6 = 0);

  /// Calcula a funcao em si y=f(x) ou z=f(x,y)
  virtual double Go (double x = 0, double y = 0);	
};

// ----------------------------------------------------------------------
//                              CFPolinomio1g
// ----------------------------------------------------------------------
/**
 * @brief Cria um polinomio de 1 grau.
 * A diferença em relacao a classe polinomio é que a funcao de cálculo
 * é mais rapida por ser definida e nao necessitar de um for.
 * */
class CFPolinomio1g : public CFPolinomio
{

public:
  /// Construtor
	CFPolinomio1g ();		
  
	/// Destrutor
	virtual ~ CFPolinomio1g ();	
  
	/// calcula a funcao em si y=f(x) ou z=f(x,y)
	virtual double Go (double x = 0, double y = 0);	
};

// ----------------------------------------------------------------------
//                              CFPolinomio2g
// ----------------------------------------------------------------------
/**
 * @brief Cria um polinomio de 2 grau.
 * A diferença em relacao a classe polinomio é que a funcao de cálculo
 * é mais rapida por ser definida e nao necessitar de um for.
 * */
class CFPolinomio2g:public CFPolinomio
{
  
public:
	/// Construtor
  CFPolinomio2g ();		
  
  /// Destrutor
  virtual ~ CFPolinomio2g ();	
  
  /// Calcula a funcao em si y=f(x) ou z=f(x,y)
  virtual double Go (double x = 0, double y = 0);
};

// ----------------------------------------------------------------------
//                              CFPolinomio3g
// ----------------------------------------------------------------------
/**
 * @brief Cria um polinomio de 3 grau.
 * A diferença em relacao a classe polinomio é que a funcao de cálculo
 * é mais rapida por ser definida e nao necessitar de um for.
 * */
class CFPolinomio3g:public CFPolinomio
{
	
public:
	/// Construtor
  CFPolinomio3g ();		
  
  /// Destrutor
  virtual ~ CFPolinomio3g ();	
  
  /// Calcula a funcao em si y=f(x) ou z=f(x,y)
  virtual double Go (double x = 0, double y = 0);	
};

// ----------------------------------------------------------------------
//                              CFPolinomio4g
// ----------------------------------------------------------------------
/**
 * @brief Cria um polinomio de 4 grau.
 * A diferença em relacao a classe polinomio é que a funcao de cálculo
 * é mais rapida por ser definida e nao necessitar de um for.
 * */
class CFPolinomio4g:public CFPolinomio
{

public:
	/// Construtor
  CFPolinomio4g ();
  /// Destrutor
  virtual ~ CFPolinomio4g ();
  
  /// Calcula a funcao em si y=f(x) ou z=f(x,y)
    virtual double Go (double x = 0, double y = 0);	
};

// ----------------------------------------------------------------------
//                              CFPolinomio5g
// ----------------------------------------------------------------------
/**
 * @brief Cria um polinomio de 5 grau.
 * A diferença em relacao a classe polinomio é que a funcao de cálculo
 * é mais rapida por ser definida e nao necessitar de um for.
 * */
class CFPolinomio5g:public CFPolinomio
{
	
public:
	/// Construtor
  CFPolinomio5g ();
  
  /// Destrutor
  virtual ~ CFPolinomio5g ();
  
  
  /// Calcula a funcao em si y=f(x) ou z=f(x,y)
  virtual double Go (double x = 0, double y = 0);
};

// ----------------------------------------------------------------------
//                              CFPolinomio6g
// ----------------------------------------------------------------------
/**
 * @brief Cria um polinomio de 6 grau.
 * A diferença em relacao a classe polinomio é que a funcao de cálculo
 * é mais rapida por ser definida e nao necessitar de um for.
 * */
class CFPolinomio6g:public CFPolinomio
{
 
public:
	/// Construtor
  CFPolinomio6g ();
  
   /// Destrutor
  virtual ~ CFPolinomio6g ();
  
   /// Calcula a funcao em si y=f(x) ou z=f(x,y)
  virtual double Go (double x = 0, double y = 0);
};

#endif
