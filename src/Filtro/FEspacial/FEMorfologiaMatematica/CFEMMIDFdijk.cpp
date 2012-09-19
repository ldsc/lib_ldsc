//   ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
    Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFdijk.cpp
Nome da classe:      CFEMMIDFdijk
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a função CriaMascara da classe CFEMMIDFdijk.
*/

//   ----------------------------------------------------------------------------
//   Bibliotecas
//   ----------------------------------------------------------------------------
using namespace std;

#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFdijk.h"	//   Classe base
#include "Geometria/Bola/BCDiscreta/CBCdijk.h"	//   ponteiro para

/*
==================================================================================
Documentacao 		CriaMascara
==================================================================================
Descricao:        Funcao que cria a mascara de chanfro adequada.
Pre-condicoes:
Excecoes:        tipos de excessoes
Concorrencia:
Tempo processamento(s):
Tamanho(bits):

Comentarios:
Programador:      Andre Duarte Bueno
*/

void CFEMMIDFdijk::CriaMascara (unsigned int _tamanhoMascara) {
   if (mask) { //   se existe uma mascara
      if (mask->NX () == _tamanhoMascara)	//   e é do mesmo  tamanho
         return;			//   sai
      delete mask;		//   se  nao é do mesmo tamanho apaga objeto mask
   }				//   e abaixo cria uma nova
   //   se nao existe a mascara, cria uma nova
   mask = new CBCdijk (_tamanhoMascara, mi, mj, mk, raioBase);
}

/*
==================================================================================
Documentacao 		Funcao Go
==================================================================================
Descricao:
Antes realizava um processo mais lento, criava uma mascara da mascara e depois
percorria os valores da imagem usando esta mascara da mascara num processo
 mais confuso e lento.
Da forma como esta nao existe mais a necessidade de se definir uma mascara 
da mascara,
de preencher os valores da mesma e realizar loops de calculo.
Aqui substitui as chamadas a mascara da mascara pelo próprio valor, e 
substitui o loop pela repetição do código
de teste para cada ponto da mascara.
Para a d5711 tem-se 8 testes, o que nao deixa o código muito extenso e
compensa pela maior velocidade e compreensao.

Observe que nao percorre toda a imagem, como sao acessados valores -2 e +2
deve percorrer a parte interna da imagem, nao acessando a borda da imagem.
Ou seja desconsidera-se os pontos da borda da imagem.
*/

//   TMatriz2D< int > * CFEMMIDFdijk::Go( TMatriz2D< int > *& matriz)
TMatriz2D< int > * CFEMMIDFdijk::Go (TMatriz2D< int > * &matriz, unsigned int /*_tamanhoMascara*/ ) {
   int x, y;			//   Indices para percorrer a matriz
   ExecutadaPorGo (matriz);	//   armazena valores da matriz e _tamanhoMascara

   //adicionei esta inversão para poder criar imagem IDF informando quem é indice e fundo.
   InverterSeNecessario();

   //   Como esta mascara tem raio 2, abaixo nao calcula a idf para pontos nos planos 0 e 1
   //   observe o for iniciar de 2, deixando de fora 0 e 1.
   //   A funcao abaixo considera os planos 0
   //    IDFNosPlanosDeContornoIDA(mi);
   //   Falta considerar os planos em que x=1 e y=x, x=nx-2, y=ny-2, o que faço a seguir
   //   por simplificacao faço ponto igual a 5, posteriormente calcular corretamente
   for (y = 1; y < ny - 1; y++)
      if (data2D[1][y] != 0)	//   percorre linha x=1
         data2D[1][y] = mi;
   for (x = 2; x < nx - 1; x++)
      if (data2D[x][1] != 0)	//   percorre linha y=1
         data2D[x][1] = mi;

   //   deve considerar adicionalmente a segunda linha, pois abaixo a mesma nao é verificada.

   //   ida   MinimoIda
   //   Da forma como esta nao percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
   for (y = 2; y < ny - 2; y++) {	//   NY() é igual a ny, ny da matriz idf
      for (x = 2; x < nx - 2; x++) {
         if (data2D[x][y] != 0) {	//   Testa a imagem, se nao for solido entra
            minimo = raioMaximo;	//   usa 32000
            //   -------------------------------------------------------------
            min (data2D[x - 1][y] + mi);	/*ponto [x][y] */
            min (data2D[x - 2][y - 1] + mk);
            min (data2D[x - 1][y - 1] + mj);
            min (data2D[x][y - 1] + mi);
            min (data2D[x + 1][y - 1] + mj);
            min (data2D[x + 2][y - 1] + mk);
            min (data2D[x - 1][y - 2] + mk);
            min (data2D[x + 1][y - 2] + mk);
            //   -------------------------------------------------------------
            data2D[x][y] = minimo;
         }
      }
   }

   //   volta    MinimoVolta
   //    IDFNosPlanosDeContornoVOLTA(mi);
   //   for (y=2; y < ny-1 ;y++)
   for (y = ny - 2; y > 1; y--)
      if (data2D[nx - 2][y] != 0)	//   percorre linha x=nx-2
         data2D[nx - 2][y] = mi;
   //   for (x=2; x < nx-2 ;x++)
   for (x = nx - 3; x > 1; x--)
      if (data2D[x][ny - 2] != 0)	//   percorre linha y=ny-2
         data2D[x][ny - 2] = mi;

   //   Da forma como esta nao percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
   for (y = ny - 3; y > 1; y--) {	//   -2 pois começa do zero e a mascara tem tamanho 1
      for (x = nx - 3; x > 1; x--) {
         if (data2D[x][y] != 0) {	//   Se nao for solido
            minimo = data2D[x][y];	//   Armazena valor minimo da ida
            //   -------------------------------------------------------------
            min (data2D[x - 1][y + 2] + mk);
            min (data2D[x + 1][y + 2] + mk);
            min (data2D[x - 2][y + 1] + mk);
            min (data2D[x - 1][y + 1] + mj);
            min (data2D[x][y + 1] + mi);
            min (data2D[x + 1][y + 1] + mj);
            min (data2D[x + 2][y + 1] + mk);
            /*ponto [x][y] */ min (data2D[x + 1][y] + mi);
            //   -------------------------------------------------------------
            data2D[x][y] = minimo;
         }
      }
   }
   return pm;
}
