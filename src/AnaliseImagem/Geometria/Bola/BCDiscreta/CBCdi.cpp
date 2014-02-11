
/*
  ----------------------------------------------------------------------------
  PROJETO:		Anaimp
  Analise de Imagens de Meios Porosos
  ----------------------------------------------------------------------------

  Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
  Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	CBCdi.cpp
  Nome da classe:      CBCdi
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  Descricao:Implementa as funções da classe CBCdi.
*/

/*
  ----------------------------------------------------------------------------
  Bibliotecas
  ----------------------------------------------------------------------------
*/
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCdi.h>

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

// CBCdi::CBCdi(unsigned int  tamanhoMascara)            :CBCDiscreta(tamanhoMascara)
CBCdi::CBCdi (unsigned int tm, int mi, int /*raioBase */ )
        :
        CBCDiscreta (tm)		/*TMCdi(i,rb), */
{

    // Uso de raioBase

    unsigned int dist;		// distancia calculada

    unsigned int raio = RaioX ();	// Como a bola é quadrada uso raio=raioX=raioY

    raioBolaInclusa = (raio);	// virtual void CBCdi::CalculaRaioBolaECondicao()

    raioBolaTangente = (raio) + 1;

    // variaveis otimizacao, usadas calculo simetria

    unsigned int posxe, posxd;	// x esquerda e x direita

    unsigned int posys, posyn;	// y sul e y norte

    for (unsigned int xx = 0; xx <= raio; xx++)	// xx, percorre metade da bola (usa simtria)
    {

        posxe = raio - xx;

        posxd = raio + xx;

        for (unsigned int yy = 0; yy <= raio; yy++)	// só percorre meia bola (usa simetria)
        {			// virtual void CBCdi::CalculaDistancia()

            if (xx == yy)
                dist = xx + yy;	// calcula valores de distância
            else if (xx > yy)
                dist = mi * xx + yy;	// 1*x+y;        raioBase*xx + yy
            else
                dist = mi * yy + xx;	// 1*y+x;        raioBase*yy + xx


            posys = raio - yy;

            posyn = raio + yy;

            if (dist <= raioBolaInclusa)
                data2D[posxe][posyn] = data2D[posxe][posys] =
                                           data2D[posxd][posyn] = data2D[posxd][posys] = dist;
            else
                data2D[posxe][posyn] = data2D[posxe][posys] =
                                           data2D[posxd][posyn] = data2D[posxd][posys] = 0;

        }

    }

    data2D[raio][raio] = 1;

}

// calculo raio bolas inclusa e tangente---
/*void CBCdi::CalculaRaioBola()
  {
  raioBolaInclusa= (RaioX());
  raioBolaTangente=(RaioX()) +1;
  } */
/*
  Pode-se criar uma funcao virtual preenche bola generica, que para cada classe
  filha executaria as funcoes abaixo:
  void CBCdi::CalculaRaioBolaECondicao()
{
  raio=GetRaioX();
  raioBolaInclusa= (GetRaioX());
  raioBolaTangente=(GetRaioX()) +1;
  condicao=raio;
}
void CBCdi::CalculaDistancia()
{
  // CalculaDistancias
  if 	  (xx==yy)
   dist=xx + yy; 	 	// calcula valores de distância
  else if (xx>yy)
   dist=xx + yy; 		// 1*x+y;
  else
   dist=yy + xx; 		// 1*y+x;
}
*/
