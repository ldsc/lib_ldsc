// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CBCDiscreta3D_h
#define CBCDiscreta3D_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCDiscreta3D.h
Nome da classe:      CBCDiscreta3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao Classe:		CBCDiscreta3D
==================================================================================
Assunto: Mascaras e filtros
Superclasse: CMascara
Descrição: Implementa a classe CBCDiscreta3D. É derivada da classe CMascara.
		     Acrescenta o conceito de mascara discreta.
           Acrescenta atributos raioBolaInclusa e raioBolaTangente
Acesso: import
Cardinalidade:
Abstrata/Concreta:
Arquivo de documentacao auxiliar:
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#include <AnaliseImagem/Filtro/Mascara/CMascara3D.h>

// ----------------------------------------------------------------------------
// Classe:       CBCDiscreta3D
// ----------------------------------------------------------------------------
/**
 * @brief Representa uma bola discreta 3D.
 * É derivada da classe base CMascara3D.
 */
class CBCDiscreta3D : public CMascara3D
{

// Atributos
protected:

    unsigned int raioBolaInclusa;	///< Valor do raio da bola inclusa

    unsigned int raioBolaTangente;	///< Valor do raio da  bola tangente

// Metodos
public:

    /// Construtor
    CBCDiscreta3D (int rb) : CMascara3D (rb) {
    }

    /// Destrutor
    virtual ~ CBCDiscreta3D () {
    }
public:

    /// Retorna raio da bola inclusa
    inline unsigned int RaioBolaInclusa () const {
        return raioBolaInclusa;
    }

    /// Recebe raio da bola inclusa
    inline void RaioBolaInclusa (int rbi) {
        raioBolaInclusa = rbi;
    }

    /// Retorna raio da bola tangente
    inline unsigned int RaioBolaTangente () const {
        return raioBolaTangente;
    }

    /// Recebe raio da bola tangente
    inline void RaioBolaTangente (int rbt) {
        raioBolaTangente = rbt;
    }

};
#endif // TMascMascDiscreta
