
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
Descricao:          Implementa as fun��es da classe TACFraFunCov.
*/
//----------------------------------------------------------------------------
//Bibliotecas
//----------------------------------------------------------------------------

#include <TFractal/TACFraFunCov.h>

/*
============================================================================
Documentacao Construtor
============================================================================
Descri��o:
Pr�-condi��es:
Excess�es:        tipos de excessoes
Concorr�ncia:
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
Descri��o:
Pr�-condi��es:
Excess�es:        tipos de excessoes
Concorr�ncia:
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
//deve proceder ao cálculo da dimens�o fractal com o vetor de dados
  return dimensaoFractal;
}

/*
============================================================================
Documentacao CalculaVetorDados
============================================================================
Descri��o:        Calcula o vetor de dados para cálculo da dimensao fractal
                  Funcionamento:
Programador:      Andre Duarte Bueno                            .
*/
TVetor *
TACFraFunCov::CalculaVetorDados (TMatriz2D * &pm)
{
//implementar
  return vetorDados = NULL;
}
