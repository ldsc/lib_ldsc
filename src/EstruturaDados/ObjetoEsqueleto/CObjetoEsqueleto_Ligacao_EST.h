#ifndef CObjetoEsqueleto_Ligacao_EST_h
#define CObjetoEsqueleto_Ligacao_EST_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CObjetoEsqueleto_Ligacao_EST.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto_Ligacao.h>

// ===============================================================================
// Documentacao Classe: CObjetoEsqueleto_Ligacao_EST
// ===============================================================================
/**
 * @brief  Representa um objeto do esqueleto do tipo sitio a esquerda.
 * @short  redefine funcao Contorno
 * @author André Duarte Bueno
 * @see		CGrafo
 * @ingroup  HCObjetoGrafo
 */
class CObjetoEsqueleto_Ligacao_EST : public CObjetoEsqueleto_Ligacao {
// --------------------------------------------------------------Atributos
public:
// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoEsqueleto_Ligacao_EST () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CObjetoEsqueleto_Ligacao_EST () = default;

// ----------------------------------------------------------------Métodos

   /// Retorna o tipo de objeto do grafo.
   virtual ETipoObjetoGrafo Tipo () const  override { return ETipoObjetoGrafo::ObjetoEsqueleto_Ligacao_EST;   }

   // Redefinição de funções herdadas
     /// Retorna o tipo de contorno
     inline virtual CContorno::ETipoContorno Contorno () const {
          return CContorno::ETipoContorno::EST;
     }

     /**
      * @brief Função herdada da classe CParametroSolver,
      * usada para calcular o valor de x.
      * Observe que retorna direto o valor de x, ou seja,
      * um objeto da fronteira não recalcula seu fluxo*/
     inline virtual long double Go ( long double d = 0 )  {
          return x;
     }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
     // friend ostream& operator<< (ostream& os, CObjetoEsqueleto_Ligacao_EST& obj);
     // friend istream& operator>> (istream& is, CObjetoEsqueleto_Ligacao_EST& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CObjetoEsqueleto_Ligacao_EST& obj);
// istream& operator>> (istream& is, CObjetoEsqueleto_Ligacao_EST& obj);

#endif
