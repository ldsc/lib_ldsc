
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCdijk.cpp
Nome da classe:      CBCdijk
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:Implementa as funções da classe CBCdijk.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
// Classe base
#include <Geometria/Bola/BCDiscreta/CBCdijk.h>

/*
==================================================================================
Documentacao Construtor
==================================================================================
Descrição:        O construtor chama as funções de calculo do raio bola e
  		depois a funcao de preenchimento da mascara
Pré-condições:
Excessões:        tipos de excessoes
Concorrência:
Tempo processamento(s):
Tamanho(bits):
Comentarios:
Programador:      Andre Duarte Bueno
*/
// Divide o tamanho da mascara por dois, porque o método atual de calculo
// só usa a metade da mascara // :CBCDiscreta ((tamanhoMascara-1)/2+1)

// Mascara dijk  comparando com a d5711 fica da forma:
// mi=5 mj=7 mk=11    0<= raioBase <= 5-1, raiobase fica entre 0 e 4, sendo o default=2
CBCdijk::CBCdijk (unsigned int tm, int mi, int mj, int mk, int raioBase):
CBCDiscreta (tm)		/*TMCdijk(i,j,k,rb), */
{
  unsigned int dist;		// distancia calculada

  unsigned int raio = RaioX ();	// Como a bola é quadrada uso raio=raioX=raioY

  // variaveis otimizacao, usadas calculo simetria

  unsigned int posxe, posxd;	// x esquerda e x direita

  unsigned int posys, posyn;	// y sul e y norte

  // calculo raio bola inclusa e tangente

  raioBolaInclusa = mi * (raio);	// d4, raio=1, mi=1,raioBolaInclusa=1

  raioBolaTangente = mi * (raio + 1);	// d4, raio=1, mi=1,raioBolaTangente=2


  for (unsigned int xx = 0; xx <= raio; xx++)	// Usa xx, metade da bola
    {

      posxe = raio - xx;

      posxd = raio + xx;

      for (unsigned int yy = 0; yy <= raio; yy++)	// só percorre meia bola
	{			// por ser simétrica
	  // calculo da distancia
	  // Falta generalizar (valores 3 e 4), e o uso do raio base
	  if (xx > raioBase * yy)
	    dist = mi * xx + yy;	//       // 6
	  else if (xx == raioBase * yy)
	    dist = mk * yy;	//         // 11
	  else if (xx > yy)
	    dist = 4 * xx + 3 * yy;	//     // 7
	  else if (xx == yy)
	    dist = mj * xx;	//     // 7
	  else if ( (raioBase * xx) > yy)
	    dist = 4 * yy + 3 * xx;	//     // 7  definir substitutos para 3 e 4
	  else if ( (raioBase * xx) == yy)
	    dist = mk * xx;	//     // 11
	  else if ( (raioBase * xx) < yy)
	    dist = mi * yy + xx;	//     // 6


	  posys = raio + yy;

	  posyn = raio - yy;

	  if (dist <= mi * raio)	// raioBolaInclusa)
	    data2D[posxe][posyn] = data2D[posxe][posys] =
	      data2D[posxd][posyn] = data2D[posxd][posys] = dist;
	  else
	    data2D[posxe][posyn] = data2D[posxe][posys] =
	      data2D[posxd][posyn] = data2D[posxd][posys] = 0;

	}

    }

  data2D[raio][raio] = 1;
}

/*void CBCdijk::CalculaRaioBola()
{
   unsigned int raio=RaioX();
// Como a bola é quadrada uso raio=raioX=raioY
	raioBolaInclusa= mj*(raio-1);       // 5
	raioBolaTangente=mj*(raio-1) +1;    // 6
} */
