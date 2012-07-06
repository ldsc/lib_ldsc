// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEBartlet.cpp
Nome da classe:      CFEBartlet
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
----------------------------------------------------------------------------
		IBLIOTECAS
----------------------------------------------------------------------------
*/

#include "Filtro/FEspacial/CFEBartlet.h"
#include "Mascara/CMBartlet.h"

/*
==================================================================================
Documentacao      CriaMascara
==================================================================================
Descrição:	Se a mascara ja existe deve ser eliminada
Programador:      Andre Duarte Bueno
*/
void
CFEBartlet::CriaMascara (unsigned int _tamanhoMascara)
{
  tamanhoMascara = _tamanhoMascara;
  if (mask)
    delete mask;                           // deleta objeto mask
  mask = new CMBartlet (_tamanhoMascara);
}

