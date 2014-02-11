// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CBCd5711_h
#define CBCd5711_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCd5711.h
Nome da classe:      CBCd5711
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCdijk.h>

// ----------------------------------------------------------------------------
// Classe:       CBCd5711
// ----------------------------------------------------------------------------
/**
 * @brief Representa uma bola discreta gerada usando métrica d5711.
 * E' derivada da classe base CBCdijk.
 */
class CBCd5711 : public CBCdijk
{

public:

    /// Construtor
    CBCd5711 (unsigned int tm, int rb = 2)
            :CBCdijk (tm, 5, 7, 11, rb)
    {
    }

    /// Destrutor
    virtual ~ CBCd5711 ()
    {
    }

    // virtual void CalculaRaioBola(){};
};
#endif //  CBCd5711_h
