//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CMCd8_h
#define CMCd8_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
				Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:		Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   		Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMCd8.h
Nome da classe:      CMCd8
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <Mascara/MCDiscreta/CMCdi.h>

//  ----------------------------------------------------------------------------
//  Classe:       CMCd8
//  ----------------------------------------------------------------------------
/**
 * @brief Mascara de chanfro d8.
 * 
 * Representa uma mascara de chanfro.
 * 
 */
class CMCd8 : public CMCdi
{
public:
				///  Construtor
CMCd8 (int rb = 1)  : CMCdi (1, rb)
  {
  }

				///  Destrutor
  virtual ~ CMCd8 ()
  {
  }

};
#endif //   CMCd8_h

