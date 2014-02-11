// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CBCd3453D_h
#define CBCd3453D_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
 dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCd3453D.h
Nome da classe:      CBCd3453D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao CLASSE:		CBCd3453D
==================================================================================
Nome Classe:            CBCd3453D
Assunto:                Mascaras
Superclasse:		CMascara
Descrição:             Implementa a classe CBCd3453D.  E' derivada da classe base TMCDiscreta
		      	Fundamentada na métrica d34
Cardinalidade:
Abstrata/Concreta:
Arquivo de documentacao auxiliar:
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCdijk3D.h>

// ----------------------------------------------------------------------------
// Classe:       CBCd3453D
// ----------------------------------------------------------------------------
/**
 * @brief Representa uma bola 3D discreta gerada usando métrica d345.
 * E' derivada da classe base CBCdijk3D.
 */

class CBCd3453D : public CBCdijk3D
{

public:
    /// Construtor
    CBCd3453D (unsigned int tamanhoMascara, unsigned int rb = 1)
            :CBCdijk3D (tamanhoMascara, 3, 4, 5, rb)
    {
    }

    /// Destrutor
    virtual ~ CBCd3453D ()
    {
    }

};
#endif //  CBCd3453D
