#ifndef CObjetoGrafo_h
#define CObjetoGrafo_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CObjetoGrafo...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CObjetoGrafo.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// Interface de disco
#include <fstream>

// Classe container para vetores
// #include <vector>
// using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// Definição export/import da classe
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CContorno_h
#include <MetNum/Contorno/CContorno.h>
#endif

// Definição de CSMParametroSolver, classe base
#ifndef CSMParametroSolver_h
#include <MetNum/Solver/SistemaEquacoes/CSMParametroSolver.h>
#endif

// Declara a existencia das classes
/*
class CGrafo;
class CGra3Dby2D;
class CGra3Dby2_M1;
class CGra3Dby2_M2;

class COGSitio;
class COGLigacao;
class COGComponenteGrafo;
*/
// ===============================================================================
// Documentacao CLASSE: CObjetoGrafo
// ===============================================================================
/**  
 * @brief Representa um objeto básico de um grafo
 * CGrafo: 
 * Um CGrafo representa um grafo, um grafo contem
 * um conjunto de objetos do tipo CObjetoGrafo.
 * Um grafo tem varios CObjetoGrafo.
 * 
 * CObjetoGrafo: 
 * Um  CObjetoGrafo é um objeto todo parte do CGrafo,
 * ou seja  é um objeto do CGrafo.
 * 
 * Herança:
 * Um CObjetoGrafo  é herdeiro do CSMParametroSolver,
 * herdando o atributo propriedade
 * que é do tipo double, ou seja um CObjetoGrafo pode armazenar
 * uma propriedade no formato double (ex: pressão).
 * 
 * Atributos:
 * O CObjetoGrafo tem um long int rotulo
 * (usado pelo CGrafo para gerenciar os diversos objetos.
 * Ou seja, o CGrafo identifica os objetos pelos seus rótulos).
 * 
 * O CObjetoGrafo tem uma lista de CObjetoGrafo*,
 * de forma que um CObjetoGrafo tem uma lista de
 * ponteiros para outros objetos do tipo CObjetoGrafo.
 * Ou seja, um CObjetoGrafo tem uma lista dos objetos
 * com quem se relaciona.
 * 
 * Exemplo prático: 
 * Um CGrafo tem n objetos do tipo CObjetoGrafo, e os   identifica pelos seus rótulos.
 * Cada CObjetoGrafo sabe seu rótulo, e tem uma propriedade armazenada(pressão).
 * Como um objeto CObjetoGrafo pode estar   ligado a outros CObjetoGrafo, ele tem um
 * ponteiro para cada CObjetoGrafo a quem esta conectado.
 * Desta forma cada CObjetoGrafo sabe a quem esta relacionado (conectado, ligado).
 * PS: se rotulo for long int pode ser negativo.
 * 
 * @author 	André Duarte Bueno
 * @see		grafos
*/
class CObjetoGrafo : public CSMParametroSolver
{
	
// --------------------------------------------------------------Atributos

public:

   /** 
   * @brief Todo objeto CObjetoGrafo tem um rotulo, que o identifica
    * o rótulo é usado principalmente para armazenar o objeto em disco, ou seja,
    * é um  indice que identifica o objeto.
    * USADO O TEMPO todo para localizar o objeto, seus links, e referências.
   */
  unsigned int rotulo;

  // Todo CObjetoGrafo tem uma propriedade x herdada de CSMParametroSolver
  // long double x;                         // ex: pressão (herdada)

   /// Todo CObjetoGrafo tem uma propriedade // Ex:condutancia
  long double propriedade;

// -------------------------------------------------------------Construtor
/// Construtor
  CObjetoGrafo () : rotulo (0), propriedade (0)
  {
    /*      static int cont=0;
       if(cont==0)
       fout.open("imagens/CObjetoGrafo.log.txt");

       cont++; */

    /*              if(!fout)
       {
       cerr << "Não abriu o arquivo de disco imagens/CObjetoGrafo.log.txt."<<endl;
       exit(0);
       } */
  };

// --------------------------------------------------------------Destrutor

/// Destrutor
  virtual ~ CObjetoGrafo ()
  {
    /*              static int cont=0;
       if(cont==0)

       fout.close();
       cont++; */
  }

// ----------------------------------------------------------------Métodos
public:
  //  0 para sítio a esquerda(oeste),
  //  1 para sítio central e
  //  2 para sitio a direita (leste)
  //  3 para sítio ao norte,
  //  4 para sítio ao sul,
  //  5 para sítio em front e
  //  6 para sítio em back
  //  enum ETipoContorno{CENTER=0, WEST, EST, SUL, NORTH, FRONT, BACK};
  //  Esboço:
  //                         3
  //            5         0  1  2            6
  //                         4
  //                         NORTH
  //            FRONT WEST  CENTER  EST    BACK
  //                         SUL
  // Alterar: usar dynamic_cast e typeid
   /// Retorna o número (identificação) do contorno ao qual esta associado
  virtual CContorno::ETipoContorno Contorno ()const
  {
    return CContorno::CENTER;
  }
	/**
	 * @brief Função que recebe um ponteiro para um CObjetoGrafo,
	 * e o inclue na lista de link's de this
	 * ou seja, conecta o objA a this
	*/
  virtual void Conectar (CObjetoGrafo * objA, CObjetoGrafo * objB = 0) = 0;

	/// Salva atributos do objeto em disco
  virtual std::ostream & Write (std::ostream & os) const = 0;

	/// Salva atributos do objeto em disco no formato antigo
  virtual std::ostream & Write_Liang_Format (std::ostream & os) const
  {
    return os;
  }

    /// Retorna o rotulo de this
  unsigned int Getrotulo () const
  {
    return rotulo;
  }

  /**
   * @brief Função que calcula o fluxo associado ao objeto.
   * Ou seja, considera-se que este objeto esta conectado a outros objetos
   * e que em função das propriedades dos objetos, existe alguma informação que transita
   * entre os objetos. Esta propriedade é calculada por esta função.
   * Pode ser fluxo de massa, de calor, de qualquer coisa, ...
  */
  virtual long double Fluxo () const = 0;

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
  // friend ostream& operator<< (ostream& os, const CObjetoGrafo& obj);const
  // friend istream& operator>> (istream& is, CObjetoGrafo& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CObjetoGrafo& obj);
// istream& operator>> (istream& is, CObjetoGrafo& obj);
#endif
