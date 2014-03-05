#ifndef CObjetoEsqueleto_Ligacao_WEST_h
#define CObjetoEsqueleto_Ligacao_WEST_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CObjetoEsqueleto_Ligacao_WEST.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoEsqueleto_Ligacao.h>

// ===============================================================================
// Documentacao Classe: CObjetoEsqueleto_Ligacao_WEST
// ===============================================================================
/**
 * @brief Representa um objeto esqueleto conectado a face esquerda do grafo.
 *
 * Assume valor de contorno = CContorno::WEST.
 * O valor de contorno é usado no calculo dos fluxo da malha como
 * um todo. Ou seja, um algoritimo externo percorre todo
 * o grafo, e se o contorno for igual ao solicitado
 * calcula alguma propriedade neste contorno.
 * @see:        Grafo
 * @author 	André Duarte Bueno
 * @ingroup  HCObjetoGrafo
*/
class CObjetoEsqueleto_Ligacao_WEST : public CObjetoEsqueleto_Ligacao {
// --------------------------------------------------------------Atributos
public:
// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoEsqueleto_Ligacao_WEST () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CObjetoEsqueleto_Ligacao_WEST () = default;

// ----------------------------------------------------------------Métodos
   /// Retorna o tipo de objeto do grafo.
   virtual ETipo Tipo () const  override { return ETipo::ObjetoEsqueleto_Ligacao_WEST;   }

     /// Retorna o tipo
     inline virtual CContorno::ETipoContorno Contorno () const   {
          return CContorno::ETipoContorno::WEST;
     }

     /**
      * @brief Função herdade da classe CParametroSolver usada para
      * calcular o valor de x como é um objeto da fronteira,
      * não altera seu valor de x
     */
     inline virtual long double Go ( long double d = 0 )  {
          return x;
     }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend

//       friend ostream& operator<< (ostream& os, CObjetoEsqueleto_Ligacao_WEST& obj);
//       friend istream& operator>> (istream& is, CObjetoEsqueleto_Ligacao_WEST& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CObjetoEsqueleto_Ligacao_WEST& obj);
// istream& operator>> (istream& is, CObjetoEsqueleto_Ligacao_WEST& obj);

#endif
