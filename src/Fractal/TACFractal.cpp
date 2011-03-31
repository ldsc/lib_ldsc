
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TACFractal.cpp
Nome da classe:
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao: Implementa as funções da classe TACFractal.  E'a classe base do assunto TACFractal.
----------------------------------------------------------------------------
*/
//----------------------------------------------------------------------------
//Bibliotecas
//----------------------------------------------------------------------------

#include <TFractal/TACFractal.h>

/*
============================================================================
Documentacao Construtor
============================================================================
Descrição:
Pré-condições:
Excessões:        tipos de excessoes
Concorrência:
Tempo processamento(s):
Tamanho(bits):
Comentarios:
Programador:      Andre Duarte Bueno
*/
TACFractal::TACFractal ()
{
  dimensaoFractal = 2;
  vetorDados = NULL;
}

/*
============================================================================
Documentacao Destrutor
============================================================================
Descrição:
Pré-condições:
Excessões:        tipos de excessoes
Concorrência:
Tempo processamento(s):
Tamanho(bits):
Comentarios:
Programador:      Andre Duarte Bueno
*/
TACFractal::~TACFractal ()
{
  if (vetorDados)
    delete vetorDados;
}

/*
============================================================================
Documentacao DimensaoFractal
============================================================================
Descrição:      Calcula a dimensao fractal
Pré-condições:
Excessões:        tipos de excessoes
Concorrência:
Tempo processamento(s):
Tamanho(bits):
Comentarios:
Programador:      Andre Duarte Bueno
*/
/*float TACFractal::DimensaoFractal(TMatriz2D *& pm)
{
//dimensaoFractal=.....
return(dimensaoFractal);
}   */
