#ifndef CObjetoGrafo_h
#define CObjetoGrafo_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
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

// ===============================================================================
// Documentacao Classe: CObjetoGrafo
// ===============================================================================
/**
 * @brief Representa um objeto básico de um grafo.
 * Um CGrafo representa um grafo, que é composto por um conjunto de objetos do tipo CObjetoGrafo.
 * 
 * CObjetoGrafo: Um  CObjetoGrafo é uma classe  herdeira de CSMParametroSolver,
 * herdando o atributo propriedade que é do tipo double;
 * ou seja, um CObjetoGrafo pode armazenar uma propriedade no formato double (ex: pressão).
 * 
 * Atributos:
 * O CObjetoGrafo acrescenta um atributo long int rotulo que é usado pelo CGrafo para gerenciar 
 * os diversos objetos. 
 * Também acrescenta um long double propriedade. No caso do Grafo de Conexão Serial, 
 * a propriedade irá representar o raio hidraulico ou condutância.
 * 
 * Exemplo prático: 
 * Um CGrafo tem n objetos do tipo CObjetoGrafo, e os identifica pelos seus rótulos.
 * Cada CObjetoGrafo sabe seu rótulo e tem uma propriedade armazenada(pressão).
 * Como um objeto CObjetoGrafo pode estar ligado a outros CObjetoGrafo, ele tem um
 * ponteiro para cada CObjetoGrafo a quem esta conectado.
 * Desta forma cada CObjetoGrafo sabe a quem esta relacionado (conectado, ligado).
 * 
 * @todo: verificar possibilidade de usar rotulo como long int; 
 * se for negativo pode indicar outra propriedade/informação.
 * 
 * @author   André Duarte Bueno.
 * @see      grafos.
 * @todo     implementar sobrecarga << e >>.
*/
class CObjetoGrafo : public CSMParametroSolver
{
// --------------------------------------------------------------Atributos
public:
   /** 
   * @brief Todo objeto CObjetoGrafo tem um rotulo, que o identifica;
    * o rótulo é usado principalmente para armazenar o objeto em disco, ou seja,
    * é um  indice que identifica o objeto.
    * Usado o tempo todo para localizar o objeto, seus links e referências.
   */
    unsigned int rotulo {0};                    // assume default  = 0

  // Todo CObjetoGrafo tem uma propriedade x herdada de CSMParametroSolver
  // long double x;                         // ex: pressão (herdada)

   /// Todo CObjetoGrafo tem uma propriedade// Ex:raio hidraulico ou condutancia
    long double propriedade {0.0};                // assume default  = 0

// -------------------------------------------------------------Construtor
/// Construtor
  CObjetoGrafo ()  = default;
    /* { static int cont = 0;
       if(cont==0)
        fout.open("imagens/CObjetoGrafo.log.txt");
       cont++; 
     if(!fout)
       {
       cerr << "Não abriu o arquivo de disco imagens/CObjetoGrafo.log.txt."<<endl;
       exit(0);
       } 
  }; */

// --------------------------------------------------------------Destrutor

/// Destrutor.
  virtual ~ CObjetoGrafo ()   = default;
    /* { static int cont=0;
       if(cont==0)
        fout.close();
       cont++; } */

// ----------------------------------------------------------------Métodos
  /// Enumeração para o tipo de contorno, por default assume CContorno::CENTER.
  ///  Esboço:
  ///                         3
  ///            5         0  1  2            6
  ///                         4
  ///                         NORTH
  ///            FRONT WEST  CENTER  EST    BACK
  ///                         SUL
  /// @bug : como os valores abaixo foram alterados verificar se não implica em bug!
  ///        ou seja, no código não usar conversão para unsigned char.
  ///  enum class ETipoContorno : unsigned char 
  ///  { CENTER = 1, WEST=0, EST=2, SOUTH=4, NORTH=3, FRONT=5, BACK=6 };
  /// @return o número (identificação) do contorno ao qual esta associado
  virtual CContorno::ETipoContorno Contorno () const  {
    return CContorno::ETipoContorno::CENTER;
  }
  
  /**
    * @brief Função que recebe um ponteiro para um CObjetoGrafo,
    * e o inclue na lista de link's de this
    * ou seja, conecta o objA a this
  */
  virtual void Conectar (CObjetoGrafo * objA, CObjetoGrafo * objB = nullptr ) = 0;

  /// @brief Salva atributos do objeto em disco
  virtual std::ostream & Write (std::ostream & os) const = 0;

  /// @brief Salva atributos do objeto em disco no formato do Liang
  virtual std::ostream & Write_Liang_Format (std::ostream & os) const  {
    Write(os); // deve ser reescrita nas derivadas.
    return os;
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
  /// @brief Retorna o rotulo.
  unsigned int Rotulo () const  { return rotulo; }
  
  /// @brief Retorna a propriedade.
  long double Propriedade () const  { return propriedade; }
  
// --------------------------------------------------------------------Set
  /// @brief Seta o rotulo.
  void Rotulo (unsigned int _r) { rotulo = _r; }
  
  /// @brief Seta a propriedade.
  void Propriedade (long double _p) { propriedade = _p; };

// -----------------------------------------------------------------Friend
  friend std::ostream& operator<< (std::ostream& os, const CObjetoGrafo& obj);
  friend std::istream& operator>> (std::istream& is, CObjetoGrafo& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream& operator<< (std::ostream& os, const CObjetoGrafo& obj);
std::istream& operator>> (std::istream& is, CObjetoGrafo& obj);
#endif
