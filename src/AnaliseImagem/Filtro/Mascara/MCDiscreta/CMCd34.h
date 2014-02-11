//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#if !defined CMCd34_h
#define CMCd34_h

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
Nome deste arquivo:	CMCd34.h
Nome da classe:      CMCd34
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao CLASSE:		CMCd34
==================================================================================
Nome Classe:            CMCd34
Assunto:                Mascaras
Superclasse:				TMascara
Descrição:              Implementa a classe CMCd34.
	E' derivada da classe base CMCDiscreta
	Fundamentada na métrica d34
Cardinalidade:
Abstrata/Concreta:
Arquivo de documentacao auxiliar:
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------

#include <AnaliseImagem/Filtro/Mascara/MCDiscreta/CMCdij.h>

//  ----------------------------------------------------------------------------
//  Classe:       CMCd34
//  ----------------------------------------------------------------------------
/**
 * @brief Mascara de chanfro d34.
 * 
 * Representa uma mascara de chanfro.
 * 
 */
class CMCd34 : public CMCdij
{

public:

	///  Construtor, dimensao da mascara
CMCd34 (int rb = 1):CMCdij (3, 4, rb)
  {
  }
	///  Destrutor
  virtual ~ CMCd34 ()
  {
  }
};
#endif //   CMCd34
