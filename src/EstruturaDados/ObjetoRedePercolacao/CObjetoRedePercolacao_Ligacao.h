#ifndef CObjetoRedePercolacao_Ligacao_h
#define CObjetoRedePercolacao_Ligacao_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     Leandro Puerari
@file       CObjetoRedePercolacao_Ligacao.h

@copyright  (C) 2000 by Leandro Puerari
@email      puerari@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// Definição de CObjetoRedePercolacao
#ifndef CObjetoRedePercolacao_h
#include <EstruturaDados/ObjetoRedePercolacao/CObjetoRedePercolacao.h>
#endif

/**
 * @brief Representa uma objeto ligação de uma rede.
 * Tendo uma variável rotulo (herdade de CObjetoGrafo),
 * x (herdada de CParametroSolver), e uma propriedade (herdada de CObjetoRedePercolacao).
 * 
 * Note que temos duas possibilidades para implementar: 
 * A primeira adicionar um segundo vetor de conexões,
 * o problema é a dificuldade de resolver os métodos deletar e principalmente
 * o fato de que não fica uniforme com CObjetoRedePercolacao_Sitio que tem um vetor conexao.
 * Exemplo:
 *   /// Conexões a esquerda
 *   CObjetoRedePercolacao * conexao; // herdado
 *   /// Conexões a direita
 *   CObjetoRedePercolacao * conexaoB;
 * 
 * A segunda possibilidade é manter apenas um vetor de conexões, 
 * ficando uniforme com CObjetoRedePercolacao.
 * A modificação seria feita na função Conetar() que receberia o endereço dos 
 * dois sitios aos quais esta conectado.

 * @author  Leandro Puerari
 * @see     grafos
 * @ingroup  HCObjetoGrafo
 */
class CObjetoRedePercolacao_Ligacao : public CObjetoRedePercolacao
{
// --------------------------------------------------------------Atributos
public:
// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoRedePercolacao_Ligacao () = default;

// --------------------------------------------------------------Destrutor

/// Destrutor
     virtual ~ CObjetoRedePercolacao_Ligacao () = default;

// ----------------------------------------------------------------Métodos
     /// Retorna o tipo de objeto do grafo.
     virtual ETipoObjetoGrafo Tipo () const  override { return ETipoObjetoGrafo::ObjetoRede_Ligacao;  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
};

#endif
