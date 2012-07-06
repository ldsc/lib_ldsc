// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CBCdijk_h
#define CBCdijk_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCdijk.h
Nome da classe:      CBCdijk
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#include <Geometria/Bola/BCDiscreta/CBCDiscreta.h>
// #include "CMascara/CMascara.h"
// #include "Mascara/MCDiscreta/TMCDijk.h"

// ----------------------------------------------------------------------------
// Classe:       CBCdijk
// ----------------------------------------------------------------------------
/**
 * @brief Representa uma bola discreta gerada usando métrica dijk.
 * E' derivada da classe base CBCDiscreta.
 */
class CBCdijk: /*public  TMCdijk, *//*public CMascara , */ public CBCDiscreta
{

public:

    /// Construtor
    CBCdijk (unsigned int tm, int mi, int mj, int mk, int raioBase);

/// Destrutor
    virtual ~ CBCdijk ()
    {
    }

    // virtual void CalculaRaioBola();
};
#endif //  CBCdijk_h
