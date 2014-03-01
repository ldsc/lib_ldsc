#ifndef COGComponenteGrafo_h
#define COGComponenteGrafo_h

/*
===============================================================================
@PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo.h>
#endif

#ifndef CContorno_h
#include <Contorno/CContorno.h>
#endif

// ===============================================================================
// Documentacao Classe: COGComponenteGrafo.h
// ===============================================================================
/** 
 * @brief Representa um objeto avançado de um grafo.
 * É herdeiro de CObjetoGrafo, tendo uma variável x (herdada de CParametroSolver),
 * um rótulo e uma propriedade (herdados de CObjetoGrafo).
 * A característica básica de uma COGComponenteGrafo é que este pode ter
 * n link's, ou seja, estar conectado a n objetos do tipo
 * CObjetoGrafo, mas cada conecção é 1 para m (m garras).
 * Assim: enquanto
 * um  COGSitio   esta conectado a um conjunto de CObjetoGrafo,
 * uma COGLigacao esta conectada a dois conjuntos de CObjetoGrafo,
 * um  COGComponenteGrafo esta conectado a m conjuntos de CObjetoGrafo(ou seja tem m garras).
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
 * vector < vector<CObjetoGrafo*> *> coneccao;
 * 
 * Uso
 * coneccao[i] acessa o vetor i
 * coneccao[i][j] acessa o ponteiro j do vetor i
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
  COGComponenteGrafo () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
  virtual ~ COGComponenteGrafo () = default;

// ----------------------------------------------------------------Métodos
  /**
   * @brief Função de conecção.
   * Deveria receber um vetor com m garras
   * virtual void Conectar (vector<CObjetoGrafo*> * ptr);
   * INCOMPLETA:
   * Observe que vai ter de receber um vetor com a lista de objetos
   * Pensar em melhorar isto.     Reimplementa da função da classe base
*/
  virtual void Conectar (std::vector < CObjetoGrafo * >obj_vetor);

/// Salva atributos do objeto em disco
  virtual std::ostream & Write (std::ostream & os) const override;

/// Função herdada, determina o fludo associado ao objeto INCOMPLETA
/// @todo: implementar virtual long double COGComponenteGrafo::Fluxo ()!
  virtual long double Fluxo () const  override {
    return 0;
  }

// --------------------------------------------------------------------Get
/// Retorna o tipo de contorno a que pertence.
virtual CContorno::ETipoContorno Contorno () const override {
     return contorno;
   }

/// Retorna referência para o vetor de vetor de coneccões
std::vector < std::vector < CObjetoGrafo * > >&  Coneccao() {
    return   coneccao;
}

// --------------------------------------------------------------------Set
/// Definição do contorno
void Contorno (CContorno::ETipoContorno _contorno) {
    contorno = _contorno;
}

/// Definição do vetor de vetor de coneccões.
void  Coneccao(std::vector < std::vector < CObjetoGrafo * > >& _coneccao) {
    coneccao = _coneccao;
}

// -----------------------------------------------------------------Friend
/// @todo: implementar sobrecarga operadores << e >> para COGComponenteGrafo
// friend ostream& operator<< (ostream& os, COGComponenteGrafo& obj);
// friend istream& operator>> (istream& is, COGComponenteGrafo& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, COGComponenteGrafo& obj);
// istream& operator>> (istream& is, COGComponenteGrafo& obj);
#endif
