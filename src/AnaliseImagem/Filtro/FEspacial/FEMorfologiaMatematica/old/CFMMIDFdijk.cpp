//  �tens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
				Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFMMIDFdijk.cpp
Nome da classe:      CFMMIDFdijk
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a fun��o CriaMascara da classe CFMMIDFdijk.
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
using namespace std;

#include "TFiltro\TFEspacial\TFEMorfologiaMatematica\CFMMIDFdijk.h"	//  Classe base
#include "TMascara\TMCDiscreta\CBCdijk.h"	//  ponteiro para

/*
============================================================================
Documentacao 		CriaMascara
============================================================================
Descri��o:        Funcao que cria a mascara de chanfro adequada.
Pr�-condi��es:
Excess�es:        tipos de excessoes
Concorr�ncia:
Tempo processamento(s):
Tamanho(bits):

Comentarios:
Programador:      Andre Duarte Bueno
*/
void
CFMMIDFdijk::CriaMascara (unsigned int _tamanhoMascara)
{
  if (mask)			//  se existe uma mascara
    {
      if (mask->NX () == _tamanhoMascara)//  e � do mesmo  tamanho
	return;			//  sai
      delete mask;		//  se  n�o � do mesmo tamanho apaga
    }				//  e abaixo cria uma nova
  //  se n�o existe a mascara, cria uma nova
  mask = new CBCdijk (_tamanhoMascara, mi, mj, mk, raioBase);
}

/*
============================================================================
Documentacao 		Funcao Go
============================================================================
Descri��o:
Antes realizava um processo mais lento, criava uma mascara da mascara e depois
percorria os valores da imagem usando esta mascara da mascara num processo mais confuso e lento.
Da forma como esta n�o existe mais a necessidade de se definir uma mascara da mascara,
de preencher os valores da mesma e realizar loops de cálculo.
Aqui substitui as chamadas a mascara da mascara pelo pr�prio valor, e substitui o loop pela repeti��o do c�digo
de teste para cada ponto da mascara.
Para a d5711 tem-se 8 testes, o que n�o deixa o c�digo muito extenso e
compensa pela maior velocidade e compreens�o.

Observe que n�o percorre toda a imagem, como s�o acessados valores -2 e +2
deve percorrer a parte interna da imagem, n�o acessando a borda da imagem.
Ou seja desconsidera-se os pontos da borda da imagem.
*/
//  CMatriz2D * CFMMIDFdijk::Go( CMatriz2D *& matriz)
CMatriz2D *
CFMMIDFdijk::Go (CMatriz2D * &matriz, unsigned int /*_tamanhoMascara*/ )
{
  int x, y;			//  Indices para percorrer a matriz
  ExecutadaPorGo (matriz);	//  armazena valores da matriz e _tamanhoMascara

  //  Como esta mascara tem raio 2, abaixo nao calcula a idf para pontos nos planos 0 e 1
  //  observe o for iniciar de 2, deixando de fora 0 e 1.
  //  A funcao abaixo considera os planos 0
//   IDFNosPlanosDeContornoIDA(mi);
  //  Falta considerar os planos em que x=1 e y=x, x=nx-2, y=ny-2, o que fa�o a seguir
  //  por simplificacao fa�o ponto igual a 5, posteriormente calcular corretamente
  for (y = 1; y < ny - 1; y++)
    if (data2D[1][y] != 0)	//  percorre linha x=1
      data2D[1][y] = mi;
  for (x = 2; x < nx - 1; x++)
    if (data2D[x][1] != 0)	//  percorre linha y=1
      data2D[x][1] = mi;

  //  deve considerar adicionalmente a segunda linha, pois abaixo a mesma n�o � verificada.

  //  ida   MinimoIda
  //  Da forma como esta n�o percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
  for (y = 2; y < ny - 2; y++)	//  NY() � igual a ny, ny da matriz idf
    for (x = 2; x < nx - 2; x++)
      if (data2D[x][y] != 0)	//  Testa a imagem, se nao for solido entra
	{			//  
	  minimo = raioMaximo;	//  usa 32000
	  //  -------------------------------------------------------------
	  min (data2D[x - 1][y] + mi);	/*ponto [x][y] */
	  min (data2D[x - 2][y - 1] + mk);
	  min (data2D[x - 1][y - 1] + mj);
	  min (data2D[x][y - 1] + mi);
	  min (data2D[x + 1][y - 1] + mj);
	  min (data2D[x + 2][y - 1] + mk);
	  min (data2D[x - 1][y - 2] + mk);
	  min (data2D[x + 1][y - 2] + mk);
	  //  -------------------------------------------------------------
	  data2D[x][y] = minimo;
	}

//  volta    MinimoVolta
//   IDFNosPlanosDeContornoVOLTA(mi);
  //  for (y=2; y < ny-1 ;y++)
  for (y = ny - 2; y > 1; y--)
    if (data2D[nx - 2][y] != 0)	//  percorre linha x=nx-2
      data2D[nx - 2][y] = mi;
  //  for (x=2; x < nx-2 ;x++)
  for (x = nx - 3; x > 1; x--)
    if (data2D[x][ny - 2] != 0)	//  percorre linha y=ny-2
      data2D[x][ny - 2] = mi;

  //  Da forma como esta n�o percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
  for (y = ny - 3; y > 1; y--)	//  -2 pois come�a do zero e a mascara tem tamanho 1
    for (x = nx - 3; x > 1; x--)
      if (data2D[x][y] != 0)	//  Se nao for solido
	{
	  minimo = data2D[x][y]; //  Armazena valor minimo da ida
	  //  -------------------------------------------------------------
	  min (data2D[x - 1][y + 2] + mk);
	  min (data2D[x + 1][y + 2] + mk);
	  min (data2D[x - 2][y + 1] + mk);
	  min (data2D[x - 1][y + 1] + mj);
	  min (data2D[x][y + 1] + mi);
	  min (data2D[x + 1][y + 1] + mj);
	  min (data2D[x + 2][y + 1] + mk);
	  /*ponto [x][y] */ min (data2D[x + 1][y] + mi);
	  //  -------------------------------------------------------------
	  data2D[x][y] = minimo;
	}
  return pm;
}
