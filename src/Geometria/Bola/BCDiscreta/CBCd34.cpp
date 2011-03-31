
/*
  ----------------------------------------------------------------------------
  PROJETO:		Anaimp
  Analise de Imagens de Meios Porosos
  ----------------------------------------------------------------------------

  Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
  Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	CBCd34.cpp
  Nome da classe:      CBCd34
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  Descricao:	 Implementa as funções da classe CBCd34.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
// Classe base
#include <Geometria/Bola/BCDiscreta/CBCd34.h>

/*
  ==================================================================================
  Documentacao Construtor
  ==================================================================================
  Descrição:         Chama as funcoes de calculo do raio da bola e depois
  chama preenche mascara
  Pré-condições:
  Excessões:        tipos de excessoes
  Concorrência:
  Tempo processamento(s):
  Tamanho(bits):
  Comentarios:
  Programador:      Andre Duarte Bueno
*/

// Divide o tamanho da mascara por dois, porque o método atual de calculo
// só usa a metade da mascara :CBCDiscreta((tamanhoMascara-1)/2+1)
/*
CBCd34::CBCd34(unsigned int  tamanhoMascara):CBCDiscreta(tamanhoMascara)
{
  unsigned int dist; // distancia calculada
  unsigned int raio=GetRaioX(); // Como a bola é quadrada uso raio=raioX=raioY
  // variaveis otimizacao, usadas calculo simetria
  unsigned int posxe,posxd; // x esquerda e x direita
  unsigned int posyb,posya; // y baixo e y alto

  // calculo raio bola inclusa e tangente
  raioBolaInclusa= 3*(raio-1);
  raioBolaTangente=3*(raio-1) +1;

  for (unsigned int xx=0; xx <= raio; xx++) // Usa xx, metade da bola
    {
      posxe=raio - xx;
      posxd=raio + xx;
      for (unsigned int yy=0; yy <= raio; yy++) // só percorre meia bola
	{                                      // por ser simétrica
	  // calculo distancia
	  if      (xx==yy)	dist=4*xx;           // ou 4*yy
	  else if (xx >yy)	dist=3*xx+yy;
	  else 					dist=3*yy+xx;

	  posyb=raio + yy;
	  posya=raio - yy;

	  if ( dist <= 3*raio)// raioBolaInclusa)
	    data2D[posxe][posya]=data2D[posxe][posyb]=data2D[posxd][posya]=data2D[posxd][posyb]=dist;
	  else
	    data2D[posxe][posya]=data2D[posxe][posyb]=data2D[posxd][posya]=data2D[posxd][posyb]=0;
	}
    }
}
*/
