
/*
  ----------------------------------------------------------------------------
  PROJETO:		Anaimp
  Analise de Imagens de Meios Porosos
  ----------------------------------------------------------------------------

  Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
  Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	CBCd5711.cpp
  Nome da classe:      CBCd5711
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  Descricao:Implementa as funções da classe CBCd5711.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
// Classe base
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd5711.h>

/*
  ==================================================================================
  Documentacao Construtor
  ==================================================================================
  Descricao:        O construtor chama as funções de calculo do raio bola e
  depois a funcao de preenchimento da mascara
  Pre-condicoes:
  Excecoes:        tipos de excessoes
  Concorrencia:
  Tempo processamento(s):
  Tamanho(bits):
  Comentarios:
  Programador:      Andre Duarte Bueno
*/

// Divide o tamanho da mascara por dois, porque o metodo atual de calculo
// só usa a metade da mascara // :CBCDiscreta ((tamanhoMascara-1)/2+1)
/*
CBCd5711::CBCd5711(unsigned int  tamanhoMascara):CBCDiscreta (tamanhoMascara)
{
  unsigned int dist;		  	// distancia calculada
  unsigned int raio=GetRaioX();	// Como a bola é quadrada uso raio=raioX=raioY
  // variaveis otimizacao, usadas calculo simetria
  unsigned int posxe,posxd;            // x esquerda e x direita
  unsigned int posyb,posya;	        // y baixo e y alto

  // calculo raio bola inclusa e tangente
  raioBolaInclusa= 5*(raio-1);        // 5
  raioBolaTangente=5*(raio-1) +1;     // 6

  for (unsigned int xx=0; xx <= raio; xx++) // Usa xx, metade da bola
    {
      posxe=raio - xx;
      posxd=raio + xx;
      for (unsigned int yy=0; yy <= raio; yy++) 	// só percorre meia bola
	{ // por ser simétrica
	  // calculo da distancia
     	  if (xx>2*yy)
	    dist=5*xx+yy;		// 6
	  else if (xx==2*yy)
	    dist=11*yy;       // 11
	  else if (xx>yy)
	    dist=4*xx+3*yy;   // 7
	  else if (xx==yy)
	    dist=7*xx;        // 7
	  else if (2*xx>yy)
	    dist=4*yy+3*xx;   // 7
	  else if (2*xx==yy)
	    dist=11*xx;       // 11
	  else if (2*xx<yy)
	    dist=5*yy+xx;     // 6

	  posyb=raio + yy;
	  posya=raio - yy;

	  if ( dist <= 5*raio)// raioBolaInclusa)
	    data2D[posxe][posya]=data2D[posxe][posyb]=data2D[posxd][posya]=data2D[posxd][posyb]=dist;
	  else
	    data2D[posxe][posya]=data2D[posxe][posyb]=data2D[posxd][posya]=data2D[posxd][posyb]=0;
	}
    }
} */
