// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CBCDiscreta_h
#define CBCDiscreta_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
Termofisicas  dos Materiais.
Programadores:  	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCDiscreta.h
Nome da classe:      CBCDiscreta
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#include <Mascara/CMascara.h>

// ----------------------------------------------------------------------------
// Classe:       CBCDiscreta
// ----------------------------------------------------------------------------
/**
 * @brief Representa uma bola discreta gerada usando uma métrica qualquer.
 * E' derivada da classe base CMascara.
 */
class CBCDiscreta : public CMascara
{
// Atributos novos
protected:
    unsigned int raioBolaInclusa;	///< Valor do raio da bola inclusa
    unsigned int raioBolaTangente;	///< Valor do raio da  bola tangente

// Metodos
public:
    /// Construtor, recebe tamanho da mascara.
    CBCDiscreta (int tm) : CMascara (tm)
    {
    }

    /// Destrutor
    virtual ~ CBCDiscreta ()
    {
    }

public:

    /// Retorna raio da bola inclusa
    inline unsigned int RaioBolaInclusa () const
    {
        return raioBolaInclusa;
    }

    /// Recebe raio da bola inclusa
    inline void RaioBolaInclusa (int rbi)
    {
        raioBolaInclusa = rbi;
    }

    /// Retorna raio da bola tangente
    inline unsigned int RaioBolaTangente () const
    {
        return raioBolaTangente;
    }

    /// Recebe raio da bola tangente
    inline void RaioBolaTangente (int rbt)
    {
        raioBolaTangente = rbt;
    }
};
#endif // TMascMascDiscreta
