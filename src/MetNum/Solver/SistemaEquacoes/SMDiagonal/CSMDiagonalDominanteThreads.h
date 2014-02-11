#ifndef CSMDiagonalDominanteThreads_h
#define CSMDiagonalDominanteThreads_h

/*.
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CSMDiagonalDominanteThreads...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CSMDiagonalDominanteThreads.h
@begin      Sun Sep 17 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

//  -----------------------------------------------------------------------
//  Bibliotecas C/C++
//  -----------------------------------------------------------------------
#include <vector>
//aqui 2009 #include <cc++/thread.h>	//  common C++

//  -----------------------------------------------------------------------
//  Bibliotecas LIB_LDSC
//  -----------------------------------------------------------------------
#include <MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDiagonalDominante.h>

class CSMDiagonalDominanteThreads;

/**
 * @brief A classe CSMDiagonalDominanteThreadsExec é utilizada para
 * dividir a solução do sistema de equações em várias threads.
 * Observe que é herira da classe Thread (da common C++).
*/
class CSMDiagonalDominanteThreadsExec //: public ost::Thread
{
/*//aqui 2009 
	  friend class     CSMDiagonalDominanteThreads;

//  ----------------------------------------------------------------Atributos
protected:
	/// Mutex para tipo de erro
  static    ost::Mutex    mutex_erro;

  //  Atritubos estáticos, comuns a todas as threads
  ///  Ponteiro para erro da classe CSMDiagonalDominanteThreads
  static long double *    ptr_erro;

  ///  Ponteiro *X e *obj da classe CSMDiagonalDominanteThreads
  static long double *    X;
  
  /// Vetor de ponteiros para objetos do tipo CSMParametroSolver 
  static    std::vector <  CSMParametroSolver * >*    obj;

  //  Atributos estaticos usados nos calculos em go
  /// Número de processos
  static int    nproc;
  /// fator de relaxação
  static long double    fatorRelaxacao;
  /// fator de relaxação complementar
  static long double    fatorRelaxacaoC;
  /// Limite de erro
  static long double    limiteErro;
  /// Limite de iterações
  static unsigned long int    limiteIteracoes;

  ///  Identifica o processo (recebe no construtor)
  int     iproc;

private:
  int     k; ///< Indice que esta sendo avaliado

//  ----------------------------------------------------------------Métodos
public:
  ///  Construtor
CSMDiagonalDominanteThreadsExec (int _iproc = 0):iproc (_iproc)
  {
    std::cout << "Construtor da THREAD iproc = " << iproc << std::endl;
  }
  
protected:
	/// Executa processo
  virtual void  run ();
  
private:
	/// Ponteiro para classe pai
  CSMDiagonalDominanteThreads * pai;
	//aqui 2009 */
};

/**
 * @brief 	Herdeira da CSMDiagonalDominante, reescreve a função Go.
 * @author 	André Duarte Bueno	
 * @see		solver, solução sistema de equações
*/
class   CSMDiagonalDominanteThreads:   public   CSMDiagonalDominante
{
/* //aqui 2009 
	  friend class    CSMDiagonalDominanteThreadsExec;
  
public:
  ///  Construtor
CSMDiagonalDominanteThreads (unsigned long int _limiteIteracoes, 
long double _limiteErro, unsigned long int _size = 0)
:CSMDiagonalDominante (_limiteIteracoes, _limiteErro,
			_size)
  {
  };

  ///  Construtor
CSMDiagonalDominanteThreads (int _nproc, long double _fatorRelaxacao, 
unsigned long int _limiteIteracoes, long double _limiteErro, 
unsigned long int _size = 0)
:CSMDiagonalDominante (_limiteIteracoes, _limiteErro,	_size)
  {
    CSMDiagonalDominanteThreadsExec::nproc = _nproc;
    CSMDiagonalDominanteThreadsExec::fatorRelaxacao = _fatorRelaxacao;
    CSMDiagonalDominanteThreadsExec::fatorRelaxacaoC = 1.0 - _fatorRelaxacao;
    CSMDiagonalDominanteThreadsExec::limiteErro = _limiteErro;
    CSMDiagonalDominanteThreadsExec::limiteIteracoes = _limiteIteracoes;
    //  erro = 1000000;
    std::cout << "\aCONSTRUTOR ";
    std::cout << "\nerro=" << erro << std::endl;
    CSMDiagonalDominanteThreadsExec::ptr_erro = &erro;
    std::cout << "\n*ptr_erro="
	<< *CSMDiagonalDominanteThreadsExec::      ptr_erro << std::endl;

    std::      cout << "CSMDiagonalDominanteThreadsExec::nproc=" 
			<<      CSMDiagonalDominanteThreadsExec::nproc << std::endl;
    std::      cout << "CSMDiagonalDominanteThreadsExec::fatorRelaxacao= " 
		    <<      CSMDiagonalDominanteThreadsExec::fatorRelaxacao << std::endl;
    std::      cout << "CSMDiagonalDominanteThreadsExec::fatorRelaxacaoC=" 
		    <<      CSMDiagonalDominanteThreadsExec::fatorRelaxacaoC << std::endl;
    std::      cout << "CSMDiagonalDominanteThreadsExec::limiteErro= "
		     <<      CSMDiagonalDominanteThreadsExec::limiteErro << std::endl;
    std::      cout << "CSMDiagonalDominanteThreadsExec::limiteIteracoes ="
		     <<      CSMDiagonalDominanteThreadsExec::limiteIteracoes << std::endl;
    std::cout << std::endl;
  }

  ///  Destrutor
  virtual ~ CSMDiagonalDominanteThreads ()
  {
    if (X)
      delete [] X;
  };

//  ----------------------------------------------------------------Métodos
public:
	/// Executa processamento de Go.
  virtual double  Go (std::vector < CSMParametroSolver * >*objeto );
 //aqui 2009 */

};

#endif
