
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TACFraFunCov.cpp
Nome da classe:      TACFraFunCov
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:          Implementa as funções da classe TACFraFunCov.
*/
//----------------------------------------------------------------------------
//Bibliotecas
//----------------------------------------------------------------------------

#include <TFractal/TACFraFunCov.h>

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
TACFraFunCov::TACFraFunCov ()
{
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
TACFraFunCov::~TACFraFunCov ()
{
}

/*
============================================================================
Documentacao  Funcao CalculaDimensaoFractal
============================================================================
*/
float
TACFraFunCov::CalculaDimensaoFractal (TMatriz2D * &pm)
{
  CalculaVetorDados (pm);
//deve proceder ao calculo da dimensão fractal com o vetor de dados
  return dimensaoFractal;
}

/*
============================================================================
Documentacao CalculaVetorDados
============================================================================
Descrição:        Calcula o vetor de dados para calculo da dimensao fractal
                  Funcionamento:
Programador:      Andre Duarte Bueno                            .
*/
TVetor *
TACFraFunCov::CalculaVetorDados (TMatriz2D * &pm)
{
//implementar
  return vetorDados = NULL;
}
