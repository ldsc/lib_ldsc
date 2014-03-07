
// �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEEmboss.cpp
Nome da classe:      CFEEmboss
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
----------------------------------------------------------------------------
		BIBLIOTECAS
----------------------------------------------------------------------------
*/
#include "AnaliseImagem/Filtro/FEspacial/CFEEmboss.h"
#include "AnaliseImagem/Filtro/Mascara/CMEmboss.h"

/*
============================================================================
Documentacao      CriaMascara
============================================================================
Descri��o:	Se a mascara ja existe deve ser eliminada
Programador:      Andre Duarte Bueno
*/

void
CFEEmboss::CriaMascara (unsigned int _tamanhoMascara)
{
  tamanhoMascara = _tamanhoMascara;
  if (mask)
    delete mask;
  mask = new CMEmboss (_tamanhoMascara);	// Cria a mascara
  // O construtor da mascara chama a funcao
  // de preenchimento da mascara e de calculo
  // do peso da mascara
}

