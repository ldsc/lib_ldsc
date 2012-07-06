//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CMCd4_h
#define CMCd4_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
 Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, 
Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CMCd4.h
Nome da classe:      CMCd4
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <Mascara/MCDiscreta/CMCdi.h>

//  ----------------------------------------------------------------------------
//  Classe:CMCd4
//  ----------------------------------------------------------------------------
/**
 * @brief  Mascara de chanfro d4.
 * 
 * Representa uma mascara de chanfro.
 * 
 */
class CMCd4 : public CMCdi
{
public:
				///  Construtor
CMCd4 (int rb = 1)
   : CMCdi (1, rb)
  {
  }

				///  Destrutor
  virtual ~ CMCd4 ()
  {
  }
};

#endif //   CMCd4_h

