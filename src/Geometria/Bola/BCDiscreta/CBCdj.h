// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CBCdj_h
#define CBCdj_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCdj.h
Nome da classe:      CBCdj
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/

/*
==================================================================================
Documentacao CLASSE:		CBCdj
==================================================================================
Nome Classe:           CBCdj
Assunto:               Mascaras e filtros
Superclasse:           CMascara
Descrição:             Implementa a classe CBCdj, é derivada da classe CBCDiscreta
                       Fundamentada na métrica d8
Cardinalidade:
Abstrata/Concreta:
Arquivo de documentacao auxiliar:
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------

#include <Geometria/Bola/BCDiscreta/CBCDiscreta.h>

// ----------------------------------------------------------------------------
// Classe:       TGDd8
// ----------------------------------------------------------------------------
/**
 * @brief Representa uma bola discreta gerada usando métrica dj.
 * E' derivada da classe base CBCDiscreta.
 */
class CBCdj: public CBCDiscreta
{
public:
/// Construtor
    CBCdj (unsigned int tm, int mj, int raioBase);

    /// Destrutor
    virtual ~ CBCdj ()
    {
    }
    // virtual void CalculaRaioBola();

};
#endif //  CBCdj_h
