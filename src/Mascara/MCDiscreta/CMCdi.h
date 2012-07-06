
//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CMCdi_h
#define CMCdi_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
  Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, 
Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  Todos os direitos reservados.
Nome deste arquivo:	CMCdi.h
Nome da classe:      CMCdi
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/


#include <Mascara/MCDiscreta/CMCDiscreta.h>

//  ----------------------------------------------------------------------------
//  Classe:CMCdi
//  ----------------------------------------------------------------------------
/**
 * @brief Mascara de chanfro di.
 * 
 * Implementa a classe CMCdi,  é derivada da classe CMCDiscreta.
 * Fundamentada na métrica d4.
 * 
 * Representa uma mascara de chanfro.
 * 
 */
class CMCdi : public CMCDiscreta
{
protected:
    int mi; ///< Atributo mi

public:
    ///  Construtor
    CMCdi (int _mi, int rb):CMCDiscreta (rb), mi (_mi) { }

    ///  Destrutor
    virtual ~ CMCdi () { }

public:
	/// Seta mi
    void Mi (int _mi) {
        mi = _mi;
    }

    /// Obtêm mi
    int Mi () const {
        return mi;
    }
};

#endif //   CMCdi_h

