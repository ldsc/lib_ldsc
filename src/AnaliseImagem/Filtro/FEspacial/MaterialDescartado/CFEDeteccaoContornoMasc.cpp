
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEDeteccaoContornoMasc.cpp
Nome da classe:      CFEDeteccaoContornoMasc
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
----------------------------------------------------------------------------
			BIBLIOTECAS
----------------------------------------------------------------------------
*/
#include "AnaliseImagem/Filtro/FEspacial/CFEDeteccaoContornoMasc.h"
#include "AnaliseImagem/Filtro/Mascara/CMDeteccaoContorno.h"

/*
============================================================================
Documentacao      CriaMascara
============================================================================
Descri��o:	Se a mascara ja existe deve ser eliminada
Programador:    Andre Duarte Bueno
*/

void
CFEDeteccaoContornoMasc::CriaMascara (unsigned int _tamanhoMascara)
{
  tamanhoMascara = _tamanhoMascara;
  if (mask)
    delete mask;
  mask = new CMDeteccaoContorno (_tamanhoMascara);	// Cria a mascara
  // O construtor da mascara chama a funcao
  // de preenchimento da mascara e de cálculo
  // do peso da mascara        
}

