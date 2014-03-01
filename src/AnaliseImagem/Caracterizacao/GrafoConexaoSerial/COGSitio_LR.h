#ifndef COGSitio_CC_h
#define COGSitio_CC_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       COGSitio_LR.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio.h>
#endif

// ===============================================================================
// Documentacao CLASSE: COGSitio_LR
// ===============================================================================
/** 
 * @brief       Herdeira de COGSitio, acrescenta informação associada a conexão.
 * É um sítio, herdeiro de COGSitio, apenas acrescenta um vetor de long double onde será
 * armazenada a informação da propriedade associada a conecção.
 * 
 * PS:
 * Para o estudo da permeabilidade, será armazenada  a condutancia da conecção.
 * Cada classe CGrafo herdeira vai calcular esta condutancia de uma forma diferente.
 *
 * @Assunto:     Grafo.
 * @Superclasse: CParametroSolver->CObjetoGrafo->COGSitio->COGSitio_LR.
 * @author       André Duarte Bueno.
 * @see          grafos.
*/
class COGSitio_LR : public COGSitio
{
// --------------------------------------------------------------Atributos
public:
/**
 *@brief  Lista de long doubles que armazena a condutancias das conecções.
 * Na classe COGSitio criou-se um vetor de conecções, aqui cria vetor
 * para armazenar a informação das condutâncias de cada conexão.
 */
  std::vector < long double > condutancia;

// -------------------------------------------------------------Construtor

/// Construtor
  COGSitio_LR () = default;
// --------------------------------------------------------------Destrutor
/// Destrutor
  virtual ~ COGSitio_LR ()  = default;
    
// ----------------------------------------------------------------Métodos
  /**
  * @brief  Função herdada da classe CParametroSolver,
  * usada para calcular o valor de x.
  * Para o estudo da permeabilidade
  * usa a condutancia da conecção para calcular x que é a pressão
  */
  virtual long double Go (long double d = 0) override;

  /// Função que calcula o fluxo associado ao sitio
  virtual long double Fluxo () const  override;

  /// Deleta as conexoes mortas (ramos mortos)
  virtual void DeletarConeccao (unsigned int link)  override;

  /// Deleta os links para objetos que foram marcados para deleção
  virtual bool DeletarConeccoesInvalidadas (int deletado)  override;

  /// Deleta os links repetidos, retorna o número de links deletados
  virtual int DeletarConeccoesRepetidas ();

  /// Salva atributos do objeto em disco redefinida
  virtual std::ostream & Write (std::ostream & os) const  override;

  /// Salva atributos do objeto em disco no formato antigo, redefinida
  virtual std::ostream & Write_Liang_Format (std::ostream & os) const  override;

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
  /// Sobrecarga do operador <<.
  friend std::ostream & operator<< (std::ostream & os, COGSitio_LR & obj);
};

// -----------------------------------------------------------------Friend
/// Declaração de Funções Friend
std::ostream & operator<< (std::ostream & os, COGSitio_LR & obj);
// istream& operator>> (istream& is, COGSitio_LR& obj);

/// Cria o tipo COGSitio_LRCentro, que é igual a COGSitio_LR
//typedef COGSitio_LR COGSitio_LRCentro;
using COGSitio_LRCentro = COGSitio_LR ;

#endif
