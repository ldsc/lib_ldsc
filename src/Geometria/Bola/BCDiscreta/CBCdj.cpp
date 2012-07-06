
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCdj.cpp
Nome da classe:      CBCdj
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa as funções da classe CBCdj.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
// Classe base
#include <Geometria/Bola/BCDiscreta/CBCdj.h>


/*
==================================================================================
Documentacao Construtor
==================================================================================
Descrição:        O construtor da CBCdj chama as funções que calculam o raio
  		da bola (tangente e inclusa) e depois preenche a mascara
                  com os valores adequados
Pré-condições:
Excessões:        tipos de excessoes
Concorrência:
Tempo processamento(s):
Tamanho(bits):
Comentarios:
	Divide o tamanho da mascara por dois, porque o método atual de calculo
	só usa a metade da mascara
Programador:      Andre Duarte Bueno
*/

// Semelhante a d8
CBCdj::CBCdj (unsigned int tm, int mj, int /*raioBase */ ):	/*TMCdi(j,rb), */
        CBCDiscreta (tm)
{

    // uso de raioBase

    unsigned int dist;		// distancia calculada

    unsigned int raio = RaioX ();	// Como a bola é quadrada uso raio=raioX=raioY

    // calculo raio bolas inclusa e tangente---

    raioBolaInclusa = (raio);

    raioBolaTangente = (raio + 1);

    // variaveis otimizacao, usadas calculo simetria
    unsigned int posxe, posxd;	// x esquerda e x direita

    unsigned int posyb, posya;	// y baixo e y alto

    for (unsigned int xx = 0; xx <= raio; xx++)	// Usa xx, metade da bola
    {
        posxe = raio - xx;

        posxd = raio + xx;

        for (unsigned int yy = 0; yy <= raio; yy++)	// só percorre meia bola
        {			// por ser simétrica

            // calculo das distancias

            if (xx == yy)
                dist = xx;		// calcula valores de distância
            else if (xx > yy)
                dist = mj * xx;	//  + yy;     // 1*x+y;
            else
                dist = mj * yy;	//  + xx;     // 1*y+x;

            posyb = raio + yy;

            posya = raio - yy;

            if (dist <= raio)	// raioBolaInclusa)????
                data2D[posxe][posya] = data2D[posxe][posyb] =
                                           data2D[posxd][posya] = data2D[posxd][posyb] = dist;
            else
                data2D[posxe][posya] = data2D[posxe][posyb] =
                                           data2D[posxd][posya] = data2D[posxd][posyb] = 0;

        }

    }

    data2D[raio][raio] = 1;
}

/*void CBCdj::CalculaRaioBola()
{
   unsigned int raio=RaioX();
	raioBolaInclusa= (raio-1);
	raioBolaTangente=(raio-1) +1;
}       */
