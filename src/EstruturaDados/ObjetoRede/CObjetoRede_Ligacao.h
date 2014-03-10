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
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// Definição de CObjetoRede
#ifndef CObjetoRede_h
#include <EstruturaDados/ObjetoRede/CObjetoRede.h>
#endif

/**
 * @brief Representa uma objeto ligação de uma rede.
 * Tendo uma variável rotulo (herdade de CObjetoGrafo),
 * x (herdada de CParametroSolver), e uma propriedade (herdada de CObjetoRede).
 * 
 * Note que temos duas possibilidades para implementar: 
 * A primeira adicionar um segundo vetor de conexões,
 * o problema é a dificuldade de resolver os métodos deletar e principalmente
 * o fato de que não fica uniforme com CObjetoRede_Sitio que tem um vetor conexao.
 * Exemplo:
 *   /// Conexões a esquerda
 *   CObjetoRede * conexao; // herdado
 *   /// Conexões a direita
 *   CObjetoRede * conexaoB;
 * 
 * A segunda possibilidade é manter apenas um vetor de conexões, 
 * ficando uniforme com CObjetoRede.
 * A modificação seria feita na função Conetar() que receberia o endereço dos 
 * dois sitios aos quais esta conectado.

 * @author  André Duarte Bueno
 * @see     grafos
 * @ingroup  HCObjetoGrafo
 */
class CObjetoRede_Ligacao : public CObjetoRede
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
     /// Retorna o tipo de objeto do grafo.
     virtual ETipoObjetoGrafo Tipo () const  override { return ETipoObjetoGrafo::ObjetoRede_Ligacao;  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
};

#endif
