#ifndef CObjetoRede_Ligacao_h
#define CObjetoRede_Ligacao_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoRede_Ligacao.h
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Sitio.h>
#endif

/**
 * @brief Representa uma objeto ligação de uma rede.
 * Tendo uma variável rotulo (herdade de CObjetoGrafo)
 * x (herdada de CParametroSolver), e uma propriedade (herdada de CObjetoRede).
 * Note que temos duas possibilidades para implementar, a primeira criar duas conexões,
 * o problema é a dificuldade de resolver os métodos deletar e principalmente
 * o fato de que não fica uniforme com CObjetoRede_Sitio que tem vetor conexao.
 *   /// Conexões a esquerda
 *   CObjetoRede * conexaoA;
 *   /// Conexões a direita
 *   CObjetoRede * conexaoB;
 * A segunda é fazer o mesmo que CObjetoRede, criar um vetor conexao mas com apenas duas conexões.

 * @author  André Duarte Bueno
 * @see     grafos
 * @ingroup  HCObjetoGrafo
 */
class CObjetoRede_Ligacao : public CObjetoRede_Sitio
{
// --------------------------------------------------------------Atributos
public:
// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoRede_Ligacao () = default;

// --------------------------------------------------------------Destrutor

/// Destrutor
     virtual ~ CObjetoRede_Ligacao () = default;

// ----------------------------------------------------------------Métodos
     /**
       * @brief Função que recebe um ponteiro para um CObjetoRede,
       * e o inclue na lista de conexões. Lista dos objetos a quem estou conectado.
     */
     inline virtual void Conectar ( CObjetoRede *objA, CObjetoRede *objB = nullptr ) override ;

     /**
	  * @brief Deleta as conexões.
	  * */
     inline virtual void DeletarConeccao ( unsigned int link ) override ;

     /**
     * @brief Deleta as conexões.
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
     friend std::ostream &operator<< ( std::ostream &os, CObjetoRede_Ligacao &obj );
     // friend istream& operator>> (istream& is, CObjetoRede_Ligacao& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline std::ostream &operator<< ( std::ostream &os, CObjetoRede_Ligacao &obj );
// istream& operator>> (istream& is, CObjetoRede_Ligacao& obj);

#endif
