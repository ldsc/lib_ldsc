// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CBCd4_h
#define CBCd4_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCd4.h
Nome da classe:      CBCd4
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCdi.h>

// ----------------------------------------------------------------------------
// Classe:CBCd4
// ----------------------------------------------------------------------------
/**
 * @brief Representa uma bola discreta gerada usando métrica d4.
 * E' derivada da classe base CBCdi.
 */
class CBCd4:public CBCdi
{

public:
/// Construtor
    CBCd4 (unsigned int tm, int rb = 1):CBCdi (tm, 1, rb)
    {
    }

    /// Destrutor
    virtual ~ CBCd4 ()
    {
    }

    // virtual void CalculaRaioBola(){};

};
#endif //  CBCd4_h
