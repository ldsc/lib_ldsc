#ifndef COGComponenteGrafo_h
#define COGComponenteGrafo_h

/*
===============================================================================
@PROJETO:    Biblioteca LIB_LDSC
@Assunto/Ramo: Grafo
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       COGComponenteGrafo.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// ----------------------------------------------------------------------
// Bibliotecas C/C++
// ----------------------------------------------------------------------
#include <vector>

// ----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// ----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CObjetoGrafo_h

#include <Grafo/CObjetoGrafo.h>
#endif

#ifndef CContorno_h
#include <Contorno/CContorno.h>
#endif

// ===============================================================================
// Documentacao CLASSE: COGComponenteGrafo.h
// ===============================================================================
/** 
 * @brief Representa um objeto avançado de um grafo.
 * 
 * é herdeiro de CObjetoGrafo, tendo uma propriedade 
 * (herdada de CParametroSolver)
 * e um rótulo (herdado de CObjetoGrafo).
 * A característica básica de uma COGComponenteGrafo é que este pode ter
 * n link's, ou seja, estar conectado a n objetos do tipo
 * CObjetoGrafo, mas cada conecção é 1 para m (m garras).
 * Assim: enquanto
 * um  COGSitio   esta conectado a um único CObjetoGrafo,
 * uma COGLigacao esta conectada a dois CObjetoGrafo,
 * um  COGComponenteGrafo esta conectado a m objetos (ou seja tem m garras).
 * 
 * Um  COGComponenteGrafo vai ter um vetor de conexões.
 *
 * Exemplo:
 * Para sítios (uma garra S--<)
 * sítio-->sítio
 * sítio-->ligação
 * 
 * Para ligações (duas garras  >--L--<)
 * lig	<--ligação-->	lig   	
 * sítio	<--ligação-->	lig
 * lig	<--ligação-->	sítio
 * sítio	<--ligação-->	sítio
 *
 * 
 * Para COGComponenteGrafo
 * coneccao[i][j]
 * i = número de conecções (vai ter n conecções)
 * j = número de garras de cada conecção (cada conexão com m ligações)
 * 
 * Esboco para COGComponenteGrafo com   4 garras
 *                   lig
 *                   /|\...
 * lig <-----COGComponenteGrafo---->lig
 *                   \|/...
 *                   lig
 * 
 *     Exemplo:
 * // Um objeto COGComponenteGrafo esta conectado a 10 outros objetos do tipo CObjetoGrafo
 * int i = 10; 	// 10 conecções
 * // Cada conecção ocorre com 3 garras
 * //      lig <-----COGComponenteGrafo---->lig
 * //                      \|/...
 * //                      lig
 * int j = 3;		// cada uma das 10 conecções tem 3 garras
 * ...coneccao[5][0];...coneccao[5][1];...coneccao[5][2];
 *
 * Exemplo:
 * Normalmente tem-se um vetor de  CObjetoGrafo*,
 * vector<CObjetoGrafo*>  coneccao;
 * 
 * Aqui tenho um vetor de vetores
 * 
 * vector < vector<CObjetoGrafo*> *> coneccao;
 * 
 * Uso
 * coneccao[i] acessa o vetor i
 * coneccao[i][j] acessa o ponteiro j do vetor i	// Testar
 *
 * 
 * Para adicionar um objeto ao vetor i
 * coneccao[i].push_back(CObjetoGrafo*obja);
 * 
 * Para adicionar um vetor
 * vector<CObjetoGrafo*> * ptr_para_vetor_do_tipo_CObjetoGrafo;
 * coneccao.push_back(ptr_para_vetor_do_tipo_CObjetoGrafo);
 * 
 * OBS:
 * classe NÃO TESTADA.	

@author 	André Duarte Bueno	
@version		
@see		grafos
*/
class COGComponenteGrafo : public CObjetoGrafo
{
// --------------------------------------------------------------Atributos
protected:

  /**
  * @brief Normalmente tem-se um vetor de  CObjetoGrafo*,
   * vector<CObjetoGrafo*>  coneccao;
   * Aqui tenho um vetor de vetores coneccao é um vetor de objetos do tipo 
   * vector<CObjetoGrafo*> */
  std::vector < std::vector < CObjetoGrafo * > > coneccao;

   /**
   * @brief Informação do tipo de contorno a que pertenão.
    * Da forma como esta eu sei a que tipo de contorno pertenão
   */
  CContorno::ETipoContorno contorno;

public:
// -------------------------------------------------------------Construtor
/// Construtor
  COGComponenteGrafo ()
  {
  }

// --------------------------------------------------------------Destrutor
/// Destrutor
  virtual ~ COGComponenteGrafo ()
  {
  }

// ----------------------------------------------------------------Métodos
  /**
   * @brief Função de conecção.
   * Deveria receber um vetor com m garras
   * virtual void Conectar (vector<CObjetoGrafo*> * ptr);
   * INCOMPLETA:
   * Observe que vai ter de receber um vetor com a lista de objetos
   * Pensar em melhorar isto.     Reimplementa da função da classe base*/
  virtual void Conectar (std::vector < CObjetoGrafo * >obj_vetor);

  // Redefinida
  /*   virtual unsigned long int Contorno() const
     {
     return contorno;
     } */

 /// Retorna o tipo de contorno a que pertence
  virtual CContorno::ETipoContorno Contorno () const	// Tipo()
  {
    return contorno;
  }

/// Salva atributos do objeto em disco
  virtual std::ostream & Write (std::ostream & os) const;

/// Função herdada, determina o fludo associado ao objeto INCOMPLETA
  virtual long double Fluxo () const
  {
    return 0;
  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
/// Definição do contorno
  void Setcontorno (CContorno::ETipoContorno _contorno)
  {
    contorno = _contorno;
  }

// -----------------------------------------------------------------Friend
  // friend ostream& operator<< (ostream& os, COGComponenteGrafo& obj);
  // friend istream& operator>> (istream& is, COGComponenteGrafo& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, COGComponenteGrafo& obj);
// istream& operator>> (istream& is, COGComponenteGrafo& obj);

#endif
