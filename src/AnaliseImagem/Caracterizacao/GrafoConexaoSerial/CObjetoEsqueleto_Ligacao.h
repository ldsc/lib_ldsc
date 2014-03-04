#ifndef CObjetoEsqueleto_Ligacao_h
#define CObjetoEsqueleto_Ligacao_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CObjetoEsqueleto_Ligacao.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/
// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// Definição de CObjetoGrafo
#ifndef CObjetoEsqueleto_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoEsqueleto.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoEsqueleto_Ligacao
// ===============================================================================
/**
 * @brief Representa um objeto sítio de um esqueleto.
 * @author:  André Duarte Bueno
 * @see:     grafos
 * @todo:    implementar operadores >>.
 * @ingroup  HCObjetoGrafo
*/

class CObjetoEsqueleto_Ligacao :  public CObjetoRede
{
public:
// --------------------------------------------------------------Atributos
// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoEsqueleto_Ligacao () = default;
// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CObjetoEsqueleto_Ligacao () = default;
// ----------------------------------------------------------------Métodos

   /// Retorna o tipo de objeto do grafo.
   virtual ETipo Tipo () const  override { return ETipo::ObjetoEsqueleto_Ligacao;   }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
};

// Cria o tipo CObjetoEsqueleto_Ligacao_CENTER, que é igual a CObjetoEsqueleto_Ligacao
// typedef CObjetoEsqueleto_Ligacao CObjetoEsqueleto_Ligacao_CENTER;
using CObjetoEsqueleto_Ligacao_CENTER = CObjetoEsqueleto_Ligacao;

#endif
