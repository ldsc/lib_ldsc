
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCd4.cpp
Nome da classe:      CBCd4
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:Implementa as funções da classe CBCd4.
*/

/*
----------------------------------------------------------------------------
Bibliotecas
----------------------------------------------------------------------------
*/

#include <Geometria/Bola/BCDiscreta/CBCd4.h>

/*
==================================================================================
Documentacao Construtor
==================================================================================
Descrição: 			Constroe a mascara de chanfro d4.
Pré-condições:
Excessões:        tipos de excessoes
Concorrência:
Tempo processamento(s):
Tamanho(bits):
Comentarios:
Programador:      Andre Duarte Bueno
*/

/*
CBCd4::CBCd4(unsigned int  tamanhoMascara)	:CBCDiscreta(tamanhoMascara)
	    // :CBCDiscreta((tamanhoMascara-1)/2+1)
{
  unsigned int dist;			// distancia calculada
  unsigned int raio=GetRaioX();		// Como a bola é quadrada uso raio=raioX=raioY
  // variaveis otimizacao, usadas calculo simetria
  unsigned int posxe,posxd;             // x esquerda e x direita
  unsigned int posyb,posya;	        // y baixo e y alto

  // calculo raio bolas inclusa e tangente---
  raioBolaInclusa= (raio);
  raioBolaTangente=(raio) +1;

  for (unsigned int xx=0; xx <= raio; xx++)		// Usa xx, metade da bola
    {
      posxe=raio - xx;
      posxd=raio + xx;
      for (unsigned int yy=0; yy <= raio; yy++) 	// só percorre meia bola
	{                                        		// por ser simétrica
	  // calculo das distancias
	  if 	  (xx==yy)	dist=xx + yy; 	 	// calcula valores de distância
	  else if (xx>yy) 	dist=xx + yy; 		// 1*x+y;
	  else 					dist=yy + xx; 		// 1*y+x;

	  posyb=raio + yy;
	  posya=raio - yy;

	  if ( dist <= raio)// raioBolaInclusa)
	    data2D[posxe][posya]=data2D[posxe][posyb]=data2D[posxd][posya]=data2D[posxd][posyb]=dist;
	  else
	    data2D[posxe][posya]=data2D[posxe][posyb]=data2D[posxd][posya]=data2D[posxd][posyb]=0;
	}
    }
}     */
