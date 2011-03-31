#ifndef COGSitioLR_h
#define COGSitioLR_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CGrafo
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       COGSitioLR.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

/*
-------------------------------------------------------------------------
Bibliotecas ANSI C++
-------------------------------------------------------------------------
*/
#include <vector>

/*
-------------------------------------------------------------------------
Bibliotecas LIB_LDSC
-------------------------------------------------------------------------
*/
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef COGSitio_h
#include <Grafo/COGSitio.h>
#endif


// ===============================================================================
// Documentacao CLASSE: COGSitioLR
// ===============================================================================
/** 
 * @brief       Acrescenta informação associada a conexão.
 * 
 * é um sítio, herdeiro de COGSitio, apenas
 * acrescenta um vetor de long double onde será
 * armazenada a informação da propriedade associada a ligação.
 * 
 * PS:
 * Para o estudo da permeabilidade, será
 * armazenada  a condutancia da ligação.
 * Cada classe CGrafo herdeira vai calcular esta
 * condutancia de uma forma diferente.
 *
 * @Assunto:        Grafo
 * @Superclasse:   CParametroSolver->CObjetoGrafo->COGSitio->COGSitioLR
 * @author 	André Duarte Bueno	
 * @see		grafos
*/
class COGSitioLR : public COGSitio
{
// --------------------------------------------------------------Atributos

public:
	/**
 *@brief  Lista de long doubles para condutancias das ligações.
	 * Na classe COGSitio criou-se um vetor de conexões, aqui cria vetor
	 * para armazenar a informação das condutâncias de cada conexão.
	*/
  std::vector < long double >condutancia;

// -------------------------------------------------------------/**Construtor*/
/// Construtor
  COGSitioLR ()
  {
  }

// --------------------------------------------------------------/**Destrutor*/

/// Destrutor
    virtual ~ COGSitioLR ()
  {
  }

// ----------------------------------------------------------------Métodos
	/**
	* @brief  Função herdada da classe CParametroSolver,
	 * usada para calcular o valor de x
	 * Para o estudo da permeabilidade
	 * usa a condutancia da ligação para calcular x que é a pressão
	*/
  virtual long double Go (long double d = 0);

	/// Função que calcula o fluxo associado ao sitio
  virtual long double Fluxo () const;

	/// Deleta as conexoes mortas (ramos mortos)
  virtual void DeletarConeccao (unsigned int link);

    /// Deleta os links para objetos que foram marcados para deleção
  virtual bool DeletarConeccoesInvalidadas (int deletado);	

	/// Deleta os links repetidos, retorna o número de links deletados
  virtual int DeletarConeccoesRepetidas ();

	/// Salva atributos do objeto em disco redefinida
  virtual std::ostream & Write (std::ostream & os) const;

	/// Salva atributos do objeto em disco no formato antigo, redefinida
  virtual std::ostream & Write_Liang_Format (std::ostream & os) const;

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
  /// Sobrecarga do operador <<.
  friend std::ostream & operator<< (std::ostream & os, COGSitioLR & obj);

};

// -----------------------------------------------------------------Friend
/// Declaração de Funções Friend
std::ostream & operator<< (std::ostream & os, COGSitioLR & obj);
// istream& operator>> (istream& is, COGSitioLR& obj);

/// Cria o tipo COGSitioLRCentro, que é igual a COGSitioLR
typedef COGSitioLR COGSitioLRCentro;

#endif
