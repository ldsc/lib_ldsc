// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CBCd8_h
#define CBCd8_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCd8.h
Nome da classe:      CBCd8
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/

/*
==================================================================================
Documentacao Classe:		CBCd8
==================================================================================
Nome Classe:           CBCd8
Assunto:               Mascaras e filtros
Superclasse:           CMascara
Descrição:             Implementa a classe CBCd8, é derivada da classe CBCDiscreta
                       Fundamentada na métrica d8
Cardinalidade:
Abstrata/Concreta:
Arquivo de documentacao auxiliar:
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCdj.h>

// ----------------------------------------------------------------------------
// Classe:       CBCd8
// ----------------------------------------------------------------------------
/**
 * @brief Representa uma bola discreta gerada usando métrica d8.
 * E' derivada da classe base CBCdj.
 */
class CBCd8 : public CBCdj
{

public:

    /// Construtor
    CBCd8 (unsigned int tm, int rb = 1):CBCdj (tm, 1, rb)
    {
    }

/// Destrutor
    virtual ~ CBCd8 ()
    {
    }

    // virtual void CalculaRaioBola(){};
};
#endif //  CBCd8_h
