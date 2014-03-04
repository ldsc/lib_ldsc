#ifndef CObjetoRede_LigacaoMultipla_h
#define CObjetoRede_LigacaoMultipla_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoRede_LigacaoMultipla.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <vector>
#include <iostream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

// Definição de CObjetoRede
#ifndef CObjetoRede_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede.h>
#endif

/**
 * @brief Representa uma objeto ligação de uma rede.
 * Tendo uma variável rotulo (herdade de CObjetoGrafo)
 * x (herdada de CParametroSolver), e uma propriedade (herdada de CObjetoRede).
 *
 * @author  André Duarte Bueno
 * @see     grafos
 * @ingroup  HCObjetoGrafo
 */
class CObjetoRede_LigacaoMultipla : public CObjetoRede
{
// --------------------------------------------------------------Atributos
public:
     // Tem uma lista de ponteiros para objetos do tipo CObjetoRede que é genérico
     // Vetor de ponteiros para CObjetoRede.

     /// Conexões a esquerda
     std::vector < CObjetoRede * >conexaoA;

     /// Conexões a direita
     std::vector < CObjetoRede * >conexaoB;

// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoRede_LigacaoMultipla () = default;

// --------------------------------------------------------------Destrutor

/// Destrutor
     virtual ~ CObjetoRede_LigacaoMultipla () = default;

// ----------------------------------------------------------------Métodos
     /**
       * @brief Função que recebe um ponteiro para um CObjetoRede,
       * e o inclue na lista de conexões. Lista dos objetos a quem estou conectado.
     */
     inline virtual void Conectar ( CObjetoRede *objA, CObjetoRede *objB = nullptr ) override ;

     /// Deleta uma conexão.
     inline virtual void DeletarConeccao ( unsigned int link ) override ;

     /**
     * @brief Deleta os links para objetos que foram marcados para deleção.
     * Recebe um número que identifica os objetos que foram marcados
     * para deleção, se o rótulo dos objetos conectados é igual a este parâmetro
	 * a conexão é eliminada.
     */
     inline virtual bool DeletarConeccoesInvalidadas ( unsigned int deletado ) override ; 

     /// @brief Salva atributos do objeto em disco.
     virtual std::ostream &Write ( std::ostream &os ) const override ;

     /**
     * @brief Função usada para calcular uma propriedade.
     */
     virtual long double Go ( long double d = 0 ) override;

	 /// Determina o fluxo associado a this, função herdada.
     virtual long double Fluxo () const override;

public:
// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
     /// Sobrecarga do operador <<.
     friend std::ostream &operator<< ( std::ostream &os, CObjetoRede_LigacaoMultipla &obj );
     // friend istream& operator>> (istream& is, CObjetoRede_LigacaoMultipla& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline std::ostream &operator<< ( std::ostream &os, CObjetoRede_LigacaoMultipla &obj );
// istream& operator>> (istream& is, CObjetoRede_LigacaoMultipla& obj);

#endif
