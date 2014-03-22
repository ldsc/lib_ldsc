#ifndef CObjetoEsqueleto_Sitio_h
#define CObjetoEsqueleto_Sitio_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoEsqueleto_Sitio.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/
// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// Definição de CObjetoGrafo
#ifndef CObjetoEsqueleto_h
#include <EstruturaDados/ObjetoEsqueleto/CObjetoEsqueleto.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoEsqueleto_Sitio
// ===============================================================================
/**
 * @brief Representa um objeto sítio de um esqueleto.
 * @author:  André Duarte Bueno
 * @see:     grafos
 * @ingroup  HCObjetoGrafo
*/

class CObjetoEsqueleto_Sitio :  public CObjetoEsqueleto
{
public:
// --------------------------------------------------------------Atributos
// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoEsqueleto_Sitio () = default;
// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CObjetoEsqueleto_Sitio () = default;
// ----------------------------------------------------------------Métodos

   /// Retorna o tipo de objeto do grafo.
   virtual ETipoObjetoGrafo Tipo () const  override { return ETipoObjetoGrafo::ObjetoEsqueleto_Sitio;   }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
};

// Cria o tipo CObjetoEsqueleto_Sitio_CENTER, que é igual a CObjetoEsqueleto_Sitio
// typedef CObjetoEsqueleto_Sitio CObjetoEsqueleto_Sitio_CENTER;
using CObjetoEsqueleto_Sitio_CENTER = CObjetoEsqueleto_Sitio;

#endif
