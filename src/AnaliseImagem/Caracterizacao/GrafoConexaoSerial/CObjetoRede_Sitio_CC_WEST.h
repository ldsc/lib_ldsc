#ifndef CObjetoRede_Sitio_CC_WEST_h
#define CObjetoRede_Sitio_CC_WEST_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoRede_Sitio_CC_WEST.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Sitio_CC.h>

// ===============================================================================
// Documentacao Classe: CObjetoRede_Sitio_CC_WEST
// ===============================================================================
/**
 * @brief Representa um sítio conectado a face esquerda do grafo e que
 * armazena a informação da conexão.
 *
 * Superclasse:    CParametroSolver->CObjetoGrafo->CObjetoRede_Sitio->CObjetoRede_Sitio_CC->CObjetoRede_Sitio_CC_WEST
 * Assume valor de contorno = 0.
 * O 	valor de contorno é usado no calculo dos fluxo da malha como
 * um todo.Ou seja, um algoritimo externo percorre toda
 * a malha, e se o contorno for igual ao solicitado
 * calcula alguma propriedade neste contorno.
 * @author 	André Duarte Bueno
 * @see:        CGrafo
 * @todo renomear CObjetoRede_Sitio_CC_WEST -> CObjetoRede_Sitio_CCEsq
 * @ingroup  HCObjetoGrafo
 */
class CObjetoRede_Sitio_CC_WEST : public CObjetoRede_Sitio_CC {
// --------------------------------------------------------------Atributos

public:
// -------------------------------------------------------------Construtor
/// Destrutor
     CObjetoRede_Sitio_CC_WEST () = default;

// --------------------------------------------------------------Destrutor
/// Construtor
     virtual ~ CObjetoRede_Sitio_CC_WEST () = default;

// ----------------------------------------------------------------Métodos
// --------------------------------------------------------------Atributos
     /// Retorna o tipo
     inline virtual CContorno::ETipoContorno Contorno () const override  {
          return CContorno::ETipoContorno::WEST;
     }

     /**
     * @brief Função herdade da classe CParametroSolver usada para calcular
      * o valor de x retorna a pressão na fronteira direita
      * Oberve que nao altera o valor de x.
     */
     inline virtual long double Go ( long double d = 0 ) override  {
          return x;
     }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend

//       friend ostream& operator<< (ostream& os, CObjetoRede_Sitio_CC_WEST& obj);
//       friend istream& operator>> (istream& is, CObjetoRede_Sitio_CC_WEST& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CObjetoRede_Sitio_CC_WEST& obj);
// istream& operator>> (istream& is, CObjetoRede_Sitio_CC_WEST& obj);
#endif
