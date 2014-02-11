
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
   Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCEuclidiana3D.cpp
Nome da classe:      CBCEuclidiana3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao: Implementa as funções da classe CBCEuclidiana3D.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
// por causa sqrt
#include <cmath>
#include <Base/CMath.h>
// Classe base
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCEuclidiana3D.h>

/*
==================================================================================
Documentacao Construtor
==================================================================================
Descrição:        Chama as funcoes de calculo do raio da bola,
    e depois a funcao de preenchimento da mascara.
Pré-condições:
Excessões:        tipos de excessoes
Concorrência:
Tempo processamento(s):
Tamanho(bits):
Comentarios:	Obs: as mascaras de chanfro discretas d4,d8,d34, e d5711
                  usam no seu processo de calculo só o primeiro quadrante,
                  de forma que o tamanho da mascara é (size-1)/2+1. =GetRaioX()+1
                  Aqui usa o procedimento padrão, usa a mascara em 2D
                  com o tamanho passado pelo usuário.

Programador:      Andre Duarte Bueno
*/

CBCEuclidiana3D::CBCEuclidiana3D (unsigned int tamanhoMascara)
   : CBCdijk3D (tamanhoMascara, 100, (int)CMath::MSQRT2 * 100, (int)CMath::MSQRT3 * 100, 100) {
   unsigned int i, j, k;		// contadores
   unsigned int ixi, jxj;	// otimizacao
   unsigned int raio = RaioX ();	// raio da bola
   raioBolaTangente = (raio + 1) * 100;	// *100  Multiplica por 100, porque todo o calculo da bola
   raioBolaInclusa = (raio) * 100;	// é realizado assim
   float dist;			// Distancia calculada
   unsigned int posxe, posxd;	// Variaveis simetria: x esquerda e x direita  // rebatimento x
   unsigned int posys, posyn;	// Variaveis simetria: y baixo e y alto        // rebatimento y
   unsigned int poszf, poszb;	// z front , e z back
   for (i = 0; i <= raio; i++) {	// percorre a mascara (metade de x)
      posxe = raio - i;		// posicoes simetria em x
      posxd = raio + i;
      ixi = i * i;
      for (j = 0; j <= raio; j++) {
         posys = raio - j;	// posicoes simetria em y
         posyn = raio + j;
         jxj = j * j;
         for (k = 0; k <= raio; k++) {
            poszf = raio + k;
            poszb = raio - k;
            dist = sqrt (ixi + jxj + k * k);	// calcula a distancia
            if (dist <= raio) {	// se pertence a bola
               data3D[posxe][posyn][poszf] =
                     data3D[posxe][posys][poszf] =
                     data3D[posxd][posyn][poszf] =
                     data3D[posxd][posys][poszf] =
                     data3D[posxe][posyn][poszb] =
                     data3D[posxe][posys][poszb] =
                     data3D[posxd][posyn][poszb] =
                     data3D[posxd][posys][poszb] = (int)dist * 100;
            } else {
               data3D[posxe][posyn][poszf] =
                     data3D[posxe][posys][poszf] =
                     data3D[posxd][posyn][poszf] =
                     data3D[posxd][posys][poszf] =
                     data3D[posxe][posyn][poszb] =
                     data3D[posxe][posys][poszb] =
                     data3D[posxd][posyn][poszb] =
                     data3D[posxd][posys][poszb] = 0;

            }
         }// k
      }// j
   }// i
   data3D[raio][raio][raio] = 1;
}


/*
void CBCEuclidiana3D::PreencheMascara()	// sem usar simetria
{
  unsigned int i,j,k;  		  // contadores
  float dist;                      // Distancia calculada
  float dx,dy,dz;		  // deslocamentos calculados nas direcoes x e y
  float rx,ry,rz;                 // raios da mascara
  rx=GetRaioX();             // calcula os raios
  ry=GetRaioY();
  rz=GetRaioZ();
  peso=0;                          // o peso é a soma dos valores da mascara
  for (i=0; i < GetNX(); i++)      // percorre a mascara
     for (j=0; j < GetNY(); j++)   //
      for (k=0; k < GetNZ(); k++)   //
   {
         dx=i-rx;
         dy=j-ry;
         dz=k-rz;
         dist= sqrt( dx*dx + dy*dy  +dz*dz);
       if (dist <= GetRaioX())
        {
              data3D[i][j][k]=1;      // se esta dentro da bola=1
              peso++;
              }

         else
        data3D[i][j][k]=0;      // se esta fora =0
   }
// CalculaPeso(); calculado dentro do for acima descrito
}
*/
/*
==================================================================================
Documentacao 		CalculaRaioBola
==================================================================================
*/
void CBCEuclidiana3D::CalculaRaioBola () {
   unsigned int raio = RaioX ();
   raioBolaTangente = raio + 1;
   raioBolaInclusa = raio;
}
