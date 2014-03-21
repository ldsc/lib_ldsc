#ifndef CSolverMatrizDiagonalDominante_Threads_h
#define CSolverMatrizDiagonalDominante_Threads_h

/*.
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CSolverMatrizDiagonalDominante_Threads...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CSolverMatrizDiagonalDominante_Threads.h
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
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonalDominante.h>

class CSolverMatrizDiagonalDominante_Threads;

/**
 * @brief A classe CSolverMatrizDiagonalDominante_ThreadsExec é utilizada para
 * dividir a solução do sistema de equações em várias threads.
 * Observe que é herira da classe Thread (da common C++).
*/
class CSolverMatrizDiagonalDominante_ThreadsExec //: public ost::Thread
{
/*//aqui 2009 
	  friend class     CSolverMatrizDiagonalDominante_Threads;

//  ----------------------------------------------------------------Atributos
protected:
	/// Mutex para tipo de erro
  static    ost::Mutex    mutex_erro;

  //  Atritubos estáticos, comuns a todas as threads
  ///  Ponteiro para erro da classe CSolverMatrizDiagonalDominante_Threads
  static long double *    ptr_erro;

  ///  Ponteiro *X e *obj da classe CSolverMatrizDiagonalDominante_Threads
  static long double *    X;
  
  /// Vetor de ponteiros para objetos do tipo CSolverMatriz_ParametroSolver 
  static    std::vector <  CSolverMatriz_ParametroSolver * >*    obj;

  //  Atributos estaticos usados nos cálculos em go
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
CSolverMatrizDiagonalDominante_ThreadsExec (int _iproc = 0):iproc (_iproc)
  {
    std::cout << "Construtor da THREAD iproc = " << iproc << std::endl;
  }
  
protected:
	/// Executa processo
  virtual void  run ();
  
private:
	/// Ponteiro para classe pai
  CSolverMatrizDiagonalDominante_Threads * pai;
	//aqui 2009 */
};

/**
 * @brief 	Herdeira da CSolverMatrizDiagonalDominante, reescreve a função Go.
 * @author 	André Duarte Bueno	
 * @see		solver, solução sistema de equações
*/
class   CSolverMatrizDiagonalDominante_Threads:   public   CSolverMatrizDiagonalDominante
{
/* //aqui 2009 
	  friend class    CSolverMatrizDiagonalDominante_ThreadsExec;
  
public:
  ///  Construtor
CSolverMatrizDiagonalDominante_Threads (unsigned long int _limiteIteracoes, 
long double _limiteErro, unsigned long int _size = 0)
:CSolverMatrizDiagonalDominante (_limiteIteracoes, _limiteErro,
			_size)
  {
  };

  ///  Construtor
CSolverMatrizDiagonalDominante_Threads (int _nproc, long double _fatorRelaxacao, 
unsigned long int _limiteIteracoes, long double _limiteErro, 
unsigned long int _size = 0)
:CSolverMatrizDiagonalDominante (_limiteIteracoes, _limiteErro,	_size)
  {
    CSolverMatrizDiagonalDominante_ThreadsExec::nproc = _nproc;
    CSolverMatrizDiagonalDominante_ThreadsExec::fatorRelaxacao = _fatorRelaxacao;
    CSolverMatrizDiagonalDominante_ThreadsExec::fatorRelaxacaoC = 1.0 - _fatorRelaxacao;
    CSolverMatrizDiagonalDominante_ThreadsExec::limiteErro = _limiteErro;
    CSolverMatrizDiagonalDominante_ThreadsExec::limiteIteracoes = _limiteIteracoes;
    //  erro = 1000000;
    std::cout << "\aCONSTRUTOR ";
    std::cout << "\nerro=" << erro << std::endl;
    CSolverMatrizDiagonalDominante_ThreadsExec::ptr_erro = &erro;
    std::cout << "\n*ptr_erro="
	<< *CSolverMatrizDiagonalDominante_ThreadsExec::      ptr_erro << std::endl;

    std::      cout << "CSolverMatrizDiagonalDominante_ThreadsExec::nproc=" 
			<<      CSolverMatrizDiagonalDominante_ThreadsExec::nproc << std::endl;
    std::      cout << "CSolverMatrizDiagonalDominante_ThreadsExec::fatorRelaxacao= " 
		    <<      CSolverMatrizDiagonalDominante_ThreadsExec::fatorRelaxacao << std::endl;
    std::      cout << "CSolverMatrizDiagonalDominante_ThreadsExec::fatorRelaxacaoC=" 
		    <<      CSolverMatrizDiagonalDominante_ThreadsExec::fatorRelaxacaoC << std::endl;
    std::      cout << "CSolverMatrizDiagonalDominante_ThreadsExec::limiteErro= "
		     <<      CSolverMatrizDiagonalDominante_ThreadsExec::limiteErro << std::endl;
    std::      cout << "CSolverMatrizDiagonalDominante_ThreadsExec::limiteIteracoes ="
		     <<      CSolverMatrizDiagonalDominante_ThreadsExec::limiteIteracoes << std::endl;
    std::cout << std::endl;
  }

  ///  Destrutor
  virtual ~ CSolverMatrizDiagonalDominante_Threads ()
  {
    if (X)
      delete [] X;
  };

//  ----------------------------------------------------------------Métodos
public:
	/// Executa processamento de Go.
  virtual double  Go (std::vector < CSolverMatriz_ParametroSolver * >*objeto );
 //aqui 2009 */

};

#endif
