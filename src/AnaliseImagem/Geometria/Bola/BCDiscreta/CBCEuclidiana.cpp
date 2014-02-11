/*
----------------------------------------------------------------------------
PROJETO: Anaimp - Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------
Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCEuclidiana.cpp
Nome da classe:         CBCEuclidiana
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao: Implementa as funções da classe CBCEuclidiana.
*/

// ----------------------------------------------------------------------------
// Bibliotecas Ansi C/C++
// ----------------------------------------------------------------------------
#include <cmath>		// por causa sqrt

// ----------------------------------------------------------------------------
// Biblioteca LIB_LDSC
// ----------------------------------------------------------------------------
#include <Base/CMath.h>

#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCEuclidiana.h>	// Classe base

/*
==================================================================================
Documentacao Construtor
==================================================================================
Descrição: Chama as funcoes de calculo do raio da bola, e depois a funcao de preenchimento da mascara.
Pré-condições:
Excessões:        tipos de excessoes
Concorrência:
Tempo processamento(s):
Tamanho(bits):
Comentarios: As mascaras de chanfro discretas d4,d8,d34 e d5711 usam no seu processo de calculo só
				o primeiro quadrante, de forma que o tamanho da mascara é ((size-1)/2)+1, ou seja: GetRaioX()+1.
                Aqui usa o procedimento padrão, usa a mascara em 2D com o tamanho passado pelo usuário.

Programador: Andre Duarte Bueno
*/
// Construtor
//  Se me lembro bem, a multiplicação só é feita porque usa int e não float
CBCEuclidiana::CBCEuclidiana (unsigned int tm) : CBCdij (tm, 100, CMath::MSQRT2 * 100, 100) {
    unsigned int xx, yy;		// contadores
    unsigned int raio = RaioX ();	// raio da bola

    raioBolaTangente = (raio + 1) * 100;	// *100  Multiplica por 100, porque todo o calculo da bola
    raioBolaInclusa = (raio) * 100;	// *100  e realizado

    float dist;			// Distancia calculada
    unsigned int posxe, posxd;	// Variaveis simetria: x esquerda e x direita
    unsigned int posys, posyn;	// Variaveis simetria: y sul e y norte

    for (xx = 0; xx <= raio; xx++) {	// percorre metade da mascara (usa simetria)
        posxe = raio - xx;	// posicoes simetria em x
        posxd = raio + xx;
        for (yy = 0; yy <= raio; yy++) {
            dist = sqrt (xx * xx + yy * yy);	// calcula a distancia
            posys = raio - yy;	// posicoes simetria em y
            posyn = raio + yy;
            if (dist <= raio)	// se pertence a bola
                data2D[posxe][posyn] = data2D[posxe][posys] = data2D[posxd][posyn] = data2D[posxd][posys] = dist * 100;
            else
                data2D[posxe][posyn] = data2D[posxe][posys] = data2D[posxd][posyn] = data2D[posxd][posys] = 0;
        }
        data2D[raio][raio] = 1;
    }
}
