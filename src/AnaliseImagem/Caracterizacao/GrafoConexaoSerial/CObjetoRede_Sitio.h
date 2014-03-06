#ifndef CObjetoRede_Sitio_h
#define CObjetoRede_Sitio_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CObjetoRede_Sitio.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/
// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// Definição de CObjetoGrafo
#ifndef CObjetoRede_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoRede_Sitio
// ===============================================================================
/**
 * @brief Representa um objeto sítio de uma rede.
 * Tendo uma variável rotulo (herdade de CObjetoGrafo)
 * x (herdada de CParametroSolver), e uma propriedade (herdada de CObjetoRede).
 * @author:  André Duarte Bueno
 * @see:     grafos
 * @ingroup  HCObjetoGrafo
*/

class CObjetoRede_Sitio :  public CObjetoRede
{
public:
// --------------------------------------------------------------Atributos
// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoRede_Sitio () = default;
// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CObjetoRede_Sitio () = default;
// ----------------------------------------------------------------Métodos
     /// Retorna o tipo de objeto do grafo.
     virtual ETipoObjetoGrafo Tipo () const  override { return ETipoObjetoGrafo::ObjetoRede_Sitio;  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
};

// Cria o tipo CObjetoRede_Sitio_CENTER, que é igual a CObjetoRede_Sitio
// typedef CObjetoRede_Sitio CObjetoRede_Sitio_CENTER;
using CObjetoRede_Sitio_CENTER = CObjetoRede_Sitio;

#endif
