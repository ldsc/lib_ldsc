#ifndef CObjetoRede_Sitio_CC_h
#define CObjetoRede_Sitio_CC_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoRede_Sitio_CC.h
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

#ifndef CObjetoRede_Sitio_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Sitio.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoRede_Sitio_CC
// ===============================================================================
/**
 * @brief       Herdeira de CObjetoRede_Sitio, acrescenta informação associada a conexão.
 * É um sítio, herdeiro de CObjetoRede_Sitio, apenas acrescenta um vetor de long double onde será
 * armazenada a informação da propriedade associada a conecção.
 *
 * PS:
 * Para o estudo da permeabilidade, será armazenada  a condutancia da conecção.
 * Cada classe CGrafo herdeira vai calcular esta condutancia de uma forma diferente.
 *
 * @Assunto:     Grafo.
 * @Superclasse: CParametroSolver->CObjetoGrafo->CObjetoRede_Sitio->CObjetoRede_Sitio_CC.
 * @author       André Duarte Bueno.
 * @see          grafos.
 * @ingroup  HCObjetoGrafo
*/
class CObjetoRede_Sitio_CC : public CObjetoRede_Sitio {
// --------------------------------------------------------------Atributos
public:
     /**
      *@brief  Lista de long doubles que armazena a condutancias das conecções.
      * Na classe CObjetoRede_Sitio criou-se um vetor de conecções, aqui cria vetor
      * para armazenar a informação das condutâncias de cada conexão.
      */
     std::vector < long double > condutancia;

// -------------------------------------------------------------Construtor

/// Construtor
     CObjetoRede_Sitio_CC () = default;
// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CObjetoRede_Sitio_CC ()  = default;

// ----------------------------------------------------------------Métodos
     /**
     * @brief  Função herdada da classe CParametroSolver,
     * usada para calcular o valor de x.
     * Para o estudo da permeabilidade
     * usa a condutancia da conecção para calcular x que é a pressão
     */
     virtual long double Go ( long double d = 0 ) override;

     /// Função que calcula o fluxo associado ao sitio
     virtual long double Fluxo () const  override;

     /// Deleta as conexoes mortas (ramos mortos)
     virtual void DeletarConeccao ( unsigned int link )  override;

     /// Deleta os links para objetos que foram marcados para deleção
     virtual bool DeletarConeccoesInvalidadas ( int deletado )  override;

     /// Deleta os links repetidos, retorna o número de links deletados
     virtual int DeletarConeccoesRepetidas ();

     /// Salva atributos do objeto em disco redefinida
     virtual std::ostream &Write ( std::ostream &os ) const  override;

     /// Salva atributos do objeto em disco no formato antigo, redefinida
//      virtual std::ostream &Write_Liang_Format ( std::ostream &os ) const  override;

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
     /// Sobrecarga do operador <<.
     friend std::ostream &operator<< ( std::ostream &os, CObjetoRede_Sitio_CC &obj );
};

// -----------------------------------------------------------------Friend
/// Declaração de Funções Friend
std::ostream &operator<< ( std::ostream &os, CObjetoRede_Sitio_CC &obj );
// istream& operator>> (istream& is, CObjetoRede_Sitio_CC& obj);

/// Cria o tipo CObjetoRede_Sitio_CCCentro, que é igual a CObjetoRede_Sitio_CC
//typedef CObjetoRede_Sitio_CC CObjetoRede_Sitio_CCCentro;
using CObjetoRede_Sitio_CCCentro = CObjetoRede_Sitio_CC ;

#endif
