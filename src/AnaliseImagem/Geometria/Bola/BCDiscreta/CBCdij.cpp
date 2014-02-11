
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
   Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CBCdij.cpp
Nome da classe:      CBCdij
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa as funções da classe CBCdij.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
// Classe base
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCdij.h>

/*
  ==================================================================================
  Documentacao Construtor
  ==================================================================================
  Descrição:        Chama as funcoes de calculo do raio da bola e depois	chama preenche mascara
  Divide o tamanho da mascara por dois, porque o método atual de calculo
  só usa a metade da mascara :CBCDiscreta((tamanhoMascara-1)/2+1)
  CBCdij(unsigned int tm,int i,int j,int rb):CMascara(tm),TMCDij(i,j,rb)

  Pré-condições:
  Excessões:        tipos de excessoes
  Concorrência:
  Tempo processamento(s):
  Tamanho(bits):
  Comentarios:
  Programador:      Andre Duarte Bueno
*/

// Para mascara d34,  dij correspondem a:  // mi=3   // mj=4
CBCdij::CBCdij (unsigned int tm, int mi, int mj, int /*raioBase */ )
   :CBCDiscreta (tm)		/*TMCdij(i,j,rb), */
{
   // uso de raioBase
   unsigned int dist;		// distancia calculada
   unsigned int raio = RaioX ();	// Como a bola é quadrada uso raio=raioX=raioY

   // calculo raio bola inclusa e tangente
   raioBolaInclusa = mi * (raio);	// 3*(raio);
   raioBolaTangente = mi * (raio + 1);	// 3*(raio + 1);

   // variaveis otimizacao, usadas calculo simetria
   unsigned int posxe, posxd;	// x esquerda e x direita
   unsigned int posys, posyn;	// y sul e y norte
   unsigned int miXraio = mi * raio;
   for (unsigned int xx = 0; xx <= raio; xx++){	// Usa simetria para calculo da bola
      posxe = raio - xx;
      posxd = raio + xx;
      for (unsigned int yy = 0; yy <= raio; yy++) {	// só percorre meia bola
      	// por ser simétrica, calculo distancia
         // Falta o uso do raioBase?
         if (xx == yy) {
            dist = mj * xx;	// dist=4*xx;   // ou 4*yy
         } else if (xx > yy) {
            dist = mi * xx + yy;	// dist=3*xx+yy;
         } else {
            dist = mi * yy + xx;	// dist=3*yy+xx;
         }
         posys = raio - yy;
         posyn = raio + yy;

         if (dist <= miXraio)	{ // 3*raio
            data2D[posxe][posyn] = data2D[posxe][posys] = data2D[posxd][posyn] = data2D[posxd][posys] = dist;	// aqui
         }else { // tread stoped???
            data2D[posxe][posyn] = data2D[posxe][posys] =
            data2D[posxd][posyn] = data2D[posxd][posys] = 0;
         }
      }
   }
   data2D[raio][raio] = 1;
}

/*void CBCdij::CalculaRaioBola()
  {
  unsigned int raio=RaioX();						// Como a bola é quadrada uso raio=raioX=raioY
  raioBolaInclusa= mi*(raio-1);						// mi=3
  raioBolaTangente=mi*(raio-1) +1;    			// mi=3
  } */



/*SIMETRIA
unsigned int posxe,posxd;// x esquerda e x direita
unsigned int posys,posyn;// y sul e y norte
unsigned int miXraio=mi*raio;
for (unsigned int xx=0; xx <= raio; xx++)// Usa simetria para calculo da bola
  {
    posxe=raio - xx;
    posxd=raio + xx;
    for (unsigned int yy=0; yy <= raio; yy++) 	// só percorre meia bola
      {                                      // por ser simétrica
 posys=raio - yy;
 posyn=raio + yy;

 data2D[posxe][posyn]=data2D[posxe][posys]=data2D[posxd][posyn]=data2D[posxd][posys]=;
      }
    */
