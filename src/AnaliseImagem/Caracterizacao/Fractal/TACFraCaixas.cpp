
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TACFraCaixas.cpp
Nome da classe:      TACFraCaixas
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao: Implementa as funções da classe TACFraCaixas.
				E' derivada da classe TACFractal.
*/
//----------------------------------------------------------------------------
//Bibliotecas
//----------------------------------------------------------------------------

#include <TFractal/TACFraCaixas.h>

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
TACFraCaixas::TACFraCaixas ()
{
  dimensaoFractal = 2;
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
TACFraCaixas::~TACFraCaixas ()
{
}

/*
============================================================================
Documentacao  Funcao CalculaDimensaoFractal
============================================================================
*/
float
TACFraCaixas::CalculaDimensaoFractal (TMatriz2D * &pm)
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
                  Determina o maior quadro que pode ser incluído na imagem.
                  Depois determina o ponto central.
                  A partir daí calcula o número de pontos Pretos da imagem aumentando progressivamente
                  o tamanho do quadro.   Observe que a cada passo, só percorre a borda do quadro.
                  A variável LMax é o maior tamanho do quadro. As variáveis x e y são usadas
                  para percorrer as linhas e as colunas.
                  Esboco:
                                                     .
                  				*		 *			*      / \  ---->  |
                              *    cx,cy     *       |          |
                              *      *			*       |   <---- \ /
Programador:      Andre Duarte Bueno                            .
*/
TVetor *
TACFraCaixas::CalculaVetorDados (TMatriz2D * &pm)
{				//determina o tamanho do maior quadro,
  //Ex: se NX=5 LMax=2 e se NX=6 LMax=2
  unsigned int LMax = (pm->GetNX () > pm->GetNY ())? (pm->GetNY () - 1) / 2 : (pm->GetNX () - 1) / 2;	//retorna a menor dimensão da imagem (d-1)/2
  unsigned int cx = LMax;	//armazena posição x do ponto central da imagem
  unsigned int cy = LMax;	//armazena posição y do ponto central da imagem
  float ContadorNumeroPontos = 0;	//conta o numero de pontos ativos
  vetorDados = new TVetor (LMax + 1);	//cria vetor onde o índice representa o tamanho do box,
  //e o conteúdo o numero acumulado de pontos
  int L, x, y;			//L representa a metade do box
  if (pm->data2D[cx][cy] > 0)
    ContadorNumeroPontos++;	//Considera ponto central da imagem
  vetorDados->data1D[0] = 0;	//define primeiro ponto do vetor de dados
  for (L = 1; L <= LMax; L++)	//Varia L que representa 1/2 do tamanho da caixa
    {
      for (x = -L; x <= L; x++)	//percorre a direção x
	{
	  if (pm->data2D[cx + x][cy + L] > 0)	//percorre ---->  (borda superior)
	    ContadorNumeroPontos++;
	  if (pm->data2D[cx + x][cy - L] > 0)	//percorre <----  (borda inferior)
	    ContadorNumeroPontos++;
	}
      for (y = -L + 1; y <= L - 1; y++)	//percorre a direção y, descontando o primeiro e ultimo ponto já considerados acima
	{
	  if (pm->data2D[cx + L][cy + y] > 0)	//percorre para baixo ,  (borda direita)
	    ContadorNumeroPontos++;
	  if (pm->data2D[cx - L][cy + y] > 0)	//percorre para cima , (borda esquerda)
	    ContadorNumeroPontos++;
	}
      vetorDados->data1D[L] = ContadorNumeroPontos;
    }

//Agora deve determinar a reta formada por
//cuja tangente=dimensão fractal.
//   . ln(L)   *
//  /|\     *
//   |   *
//   |-------------> ln(1/L)

//dimensaoFractal=.....
  return vetorDados;
}
