// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CBCdijk3D_h
#define CBCdijk3D_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCdijk3D.h
Nome da classe:      CBCdijk3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao CLASSE:		CBCdijk3D
==================================================================================
Nome Classe:         CBCdijk3D
Assunto:             Mascaras e filtros
Superclasse:         CMascara
Descrição:           Implementa a classe CBCdijk3D.  E'derivada da classe CBCDiscreta
							Fundamentada na métrica d5711
Cardinalidade:
Abstrata/Concreta:
Arquivo de documentacao auxiliar:
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------

// #include "CMascara\CMascara.h"
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCDiscreta3D.h>

//#include <Geometria/Bola/BCDiscreta/CBCdijk.h>
#include <AnaliseImagem/Filtro/Mascara/MCDiscreta/CMCdijk.h>

// ----------------------------------------------------------------------------
// Classe:       CBCdijk3D
// ----------------------------------------------------------------------------
//class CBCdijk3D  : public CBCdijk , /*public CMascara , */ public CBCDiscreta3D
/**
 * @brief Representa uma bola discreta gerada usando métrica dijk3D.
 * E' derivada da classe base CMCdijk e de CBCDiscreta3D.
 */
class CBCdijk3D  : public CMCdijk , /*public CMascara , */ public CBCDiscreta3D
{

public:

/// Construtor
    CBCdijk3D (unsigned int tm, int i, int j, int k, int rb);

    /// Destrutor
    virtual ~ CBCdijk3D ()
    {
    }
    // virtual void CalculaRaioBola();
};
#endif //  CBCdijk3D_h
