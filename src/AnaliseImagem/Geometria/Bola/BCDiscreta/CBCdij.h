// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CBCdij_h
#define CBCdij_h

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
Nome deste arquivo:	CBCdij.h
Nome da classe:      CBCdij
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------

#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCDiscreta.h>
// #include "CAnaliseImagem/Filtro/Mascara/CMascara.h"
// #include "AnaliseImagem/Filtro/Mascara/MCDiscreta/TMCDij.h"

// ----------------------------------------------------------------------------
// Classe:       CBCdij
// ----------------------------------------------------------------------------
/**
 * @brief Representa uma bola discreta gerada usando métrica dij.
 * E' derivada da classe base CBCDiscreta.
 */
class CBCdij: /*public  TMCdij, *//* public CMascara, */ public CBCDiscreta
{

public:
    /// Construtor
    CBCdij (unsigned int tm, int mi, int mj, int raioBase);

/// Destrutor
    virtual ~ CBCdij ()
    {
    }

    // virtual void CalculaRaioBola();
};
#endif //  CBCdij
