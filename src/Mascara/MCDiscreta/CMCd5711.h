//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CMCd5711_h
#define CMCd5711_h

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
Nome deste arquivo:	CMCd5711.h
Nome da classe:      CMCd5711
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------

//  #include "TMascara\CMCDiscreta\CMCDiscreta.h"
#include <Mascara/MCDiscreta/CMCdijk.h>

//  ----------------------------------------------------------------------------
//  Classe:       CMCd5711
//  ----------------------------------------------------------------------------
/**
 * @brief Mascara  de chanfro d5711.
 * 
 * Representa uma mascara de chanfro.
 * 
 */
class CMCd5711 : public CMCdijk
{
public:
///  Construtor, recebe como parametro o tamanho da mascara
CMCd5711 (int rb = 2):CMCdijk (5, 7, 11, rb)
  {
  }

///  Destrutor
  virtual ~ CMCd5711 ()
  {
  }

};
#endif //  CMCd5711_h
