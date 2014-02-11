// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CBCdi_h
#define CBCdi_h

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
Nome deste arquivo:	CBCdi.h
Nome da classe:      CBCdi
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao CLASSE:		CBCdi
==================================================================================
Assunto:            Mascaras e filtros
Superclasse:        CMascara
Descrição:          Implementa a classe CBCdi,  é derivada da classe CBCDiscreta
                    fundamentada na métrica d4
Arquivo de documentacao auxiliar:
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCDiscreta.h>

// #include "Mascara/MCDiscreta/TMCdi.h"

// #include "CMascara/CMascara.h"

// ----------------------------------------------------------------------------
// Classe:CBCdi
// ----------------------------------------------------------------------------
/**
 * @brief Representa uma bola discreta gerada usando métrica di.
 * E' derivada da classe base CBCDiscreta.
 */
class CBCdi: /*public  TMCdi, */ public CBCDiscreta	/*,public CMascara */
{

public:

/// Construtor
    CBCdi (unsigned int tm, int mi, int raioBase);

    ///   Destrutor
    virtual ~ CBCdi ()
    {
    }

    // virtual void CalculaRaioBola();

};
#endif //  CBCdi_h
