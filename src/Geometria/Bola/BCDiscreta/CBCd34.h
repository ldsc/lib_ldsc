// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CBCd34_h
#define CBCd34_h

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
Nome deste arquivo:	CBCd34.h
Nome da classe:      CBCd34
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#include <Geometria/Bola/BCDiscreta/CBCdij.h>

// ----------------------------------------------------------------------------
// Classe:       CBCd34
// ----------------------------------------------------------------------------
/**
 * @brief Representa uma bola discreta gerada usando métrica d34.
 * E' derivada da classe base CBCdij.
 */
class CBCd34 : public CBCdij
{

public:
    /// Construtor,
    CBCd34 (unsigned int tm, int rb = 1):CBCdij (tm, 3, 4, rb)
    {
    }

    /// Destrutor
    virtual ~ CBCd34 ()
    {
    }

    // virtual void CalculaRaioBola(){};
};
#endif //  CBCd34
