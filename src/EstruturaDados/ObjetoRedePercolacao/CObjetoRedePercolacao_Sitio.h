#ifndef CObjetoRedePercolacao_Sitio_h
#define CObjetoRedePercolacao_Sitio_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     Leandro Puerari
@file       CObjetoRedePercolacao_Sitio.h

@copyright  (C) 2000 by Leandro Puerari
@email      puerari@gmail.com
*/
// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// Definição de CObjetoGrafo
#ifndef CObjetoRedePercolacao_h
#include <EstruturaDados/ObjetoRedePercolacao/CObjetoRedePercolacao.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoRedePercolacao_Sitio
// ===============================================================================
/**
 * @brief Representa um objeto sítio de uma rede.
 * Tendo uma variável rotulo (herdade de CObjetoGrafo)
 * x (herdada de CParametroSolver), e uma propriedade (herdada de CObjetoRedePercolacao).
 * @author:  Leandro Puerari
 * @see:     grafos
 * @ingroup  HCObjetoGrafo
*/

class CObjetoRedePercolacao_Sitio :  public CObjetoRedePercolacao
{
public:
// --------------------------------------------------------------Atributos
// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoRedePercolacao_Sitio () = default;
// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CObjetoRedePercolacao_Sitio () = default;
// ----------------------------------------------------------------Métodos
     /// Retorna o tipo de objeto do grafo.
     virtual ETipoObjetoGrafo Tipo () const  override { return ETipoObjetoGrafo::ObjetoRede_Sitio;  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
};

// Cria o tipo CObjetoRedePercolacao_Sitio_CENTER, que é igual a CObjetoRedePercolacao_Sitio
// typedef CObjetoRedePercolacao_Sitio CObjetoRedePercolacao_Sitio_CENTER;
using CObjetoRedePercolacao_Sitio_CENTER = CObjetoRedePercolacao_Sitio;

#endif
