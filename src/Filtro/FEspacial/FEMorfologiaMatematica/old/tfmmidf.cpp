// Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		LIB_LDSC
----------------------------------------------------------------------------
Desenvolvido por:    Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:       Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...

Copyright @1997:     Todos os direitos reservados.
Nome deste arquivo:  CFMMIDF.cpp
Nome da classe:      CFMMIDF
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa as funções da classe CFMMIDF.
*/

// ----------------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// ----------------------------------------------------------------------------
#include "TFiltro\TFEspacial\TFEMorfologiaMatematica\CFMMIDF.h"	// Classe base
#include "TMascara\TMCDiscreta\CBCDiscreta.h"	// Cria objeto

/*
============================================================================
Atributos estaticos da classe
============================================================================
*/
// Se verdadeira após cada processamento recalcula a idf
// por default, não recalcula a idf apos dilatacao ou fechamento
bool
  CFMMIDF::atualizaIDF = 0;	

/*
============================================================================
Documentacao construtor
============================================================================
Executa o construtor da classe base, o CMatriz2D(NX,NY) que aloca a matriz2D.
Define o valor do raioBola como sendo metade do tamanhoMascara, e o raioMaximo.
*/

// Construtor sobrecarregado, recebe também o tamanho do raio máximo
CFMMIDF::CFMMIDF (CMatriz2D * &matriz, unsigned int _tamanhoMascara,
		  unsigned int _raioMax):
CFMorfologiaMatematica (matriz, _tamanhoMascara),
CMatriz2D (matriz->NX (), matriz->NY ()),
raioBola (_tamanhoMascara),
raioMaximo (_raioMax),
indiceAtivo (1),
indiceInativo (0)
{				// raioMaximo=_raioMax;// raioBola=3;
}

/*
============================================================================
Função    ExecutadaPorGo
============================================================================
Função usada exclusivamente por Go:

1-Quando o usuário quer calcular a idf, chama a funcao Go (pm).
	No caso da imagem pm passada ter dimemsões diferentes da pm passada para o construtor, devo
	desalocar a matris idf, definir os novos valores de nx e ny e realocar a idf.
	Se as dimensões da matriz passada e minhas forem iguais posso continuar.

2-Copia valores da matriz passada para a matriz idf, usando data2D[i][j]=pm->data2D[i][j]

	Esta função foi incluída porque o usuário programador pode chamar Go(pm)
	sendo pm diferente daquela passada par o construtor.	Ou seja cria-se uma segurança a mais.

	Outro fato que justifica esta função é que antes, criava objeto idf, executava Go, e depois
	podia executar a erosao, mas sempre sobre a idf da imagem passada para o construtor
	da classe. Agora posso passar uma nova matriz para a função Go,
	diferente da passada para o construtor.

	O terceiro motivo é que no caso de se realizar uma sequencia de erosoes com a mesma
	mascara, eu precisava criar um objeto idf, calcular go e calcular a erosao,
	criar nova idf (sobre pm erodida), executar go e então realizar a erosao...
	Agora posso recalcular Go a qualquer momento, sem recriar o objeto idf.
	Exemplo:
			cria idf      		pfmm=new TMMIDF(pm,tamanhoMascara);
			executa go    		pfmm->Go(pm,tamanhoMascara);
			executa erosao    pfmm->Erosao(pm,raioBola);       // erosao sobre imagem original
			executa go    		pfmm->Go(pm,tamanhoMascara);
			executa erosao    pfmm->Erosao(pm,raioBola);			// erosao sobre imagem já erodida

Assim, toda funcao Go herdeira da CFMMIDF deve chamar esta funcao:
	 ExecutadaPorGo(matriz);

*/
// Funcao chamada exclusivamente por Go, ou seja depois de executada a idf vai ser recalculada.
void
CFMMIDF::ExecutadaPorGo (CMatriz2D * &matriz)	// ,unsigned int _tamanhoMascara)
{
  pm = matriz;			// armazena endereço matriz
  // verifica se a matriz tem as mesmas dimensoes da idf(this)
  if (this->nx != matriz->NX () || this->ny != matriz->NY ())
    {
      Desaloca ();		// desaloca a matriz existende e depois
      this->nx = matriz->NX ();	// define novo tamanho
      this->ny = matriz->NY ();
      Aloca ();			// aloca de acordo com novo tamanho
    }
  // Agora tenho de armazenar valores de pm na idf

  // substituir por this=matriz;
  register int mi = Mi ();
  for (int j = 0; j < ny; j++)
    for (int i = 0; i < nx; i++)
      if (matriz->data2D[i][j] != 0)	// como a imagem recebida por ser uma outra idf
	this->data2D[i][j] = mi;	// define this com 0 e 1
  // =mi                                           // // // // // // // // // // // // // 
  // AQUI AQUI AQUI AQUI: trocar 1 por mi
  // elimina calculo dos planos de contorno
  // fica mais rapido
  // // // // // // // // // // // // // 
      else
	this->data2D[i][j] = 0;
  // this->data2D[i][j]=matriz->data2D[i][j];
// Write("ExecutadaPorGo.txt");
}

/*
============================================================================
Função    VerificaImagem
============================================================================
Usada pelas funções Erosao,Dilatacao, fechamento e abertura para verificar se é a mesma imagem.
Se for outra imagem recalcula Go.
Tambem armazena endereço da imagem em pm.
*/
void
CFMMIDF::VerificaImagem (CMatriz2D * &matriz)
{				// se for a mesma imagem e tiver o mesmo tamanho sai
  // pm=matriz;
  if (pm == matriz && nx == matriz->NX () && ny == matriz->NY ())
    return;			// sai
  else
    Go (matriz);		// senão chama Go, que redefine o tamanho da imagem
}				// e calcula valores idf

/*
============================================================================
Função    Erosao
============================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
CMatriz2D *
CFMMIDF::Erosao (CMatriz2D * &matriz, unsigned int _RaioBola)	// 
{
  // -->Padrao
  VerificaImagem (matriz);	// verifica se é a mesma imagem (se diferente recalcula Go)
  tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
  // Deve calcular o tamanhoMascara antes de criar a mascara
  CriaMascara (tamanhoMascara);	// Cria a mascara adequada, do tamanho de tamanhoMascara

  CBCDiscreta *maskd = dynamic_cast < CBCDiscreta * >(mask);	// Cria ponteiro para mascara com acesso a GetraioBolaTangente

  // Processamento da erosao em si
  register int raioBolaInclusa = maskd->GetraioBolaInclusa ();
  register int **pmdata2D = pm->Data2D ();	// otimizacao

  for (int j = 0; j < ny; j++)
    {
      for (int i = 0; i < nx; i++)
	{
	  // se o ponto da idf for maior que a bola tangente, faz ponto=1
	  if (data2D[i][j] > raioBolaInclusa)
	    pmdata2D[i][j] = indiceAtivo;	// seta ponto ativo

	  else
	    pmdata2D[i][j] = indiceInativo;	// seta ponto inativo
	}
    }
  return pm;			// pm é a matriz erodida
}


/*
============================================================================
Função    Abertura
============================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
CMatriz2D *
CFMMIDF::Abertura (CMatriz2D * &matriz, unsigned int _RaioBola)	// 
{
  // -->Padrao
  VerificaImagem (matriz);	// verifica se é a mesma imagem (se diferente recalcula Go)
  tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
  // Deve calcular o tamanhoMascara antes de criar a mascara
  CriaMascara (tamanhoMascara);	// Cria a mascara adequada,do tamanho de tamanhoMascara

  CBCDiscreta *maskd = dynamic_cast < CBCDiscreta * >(mask);	// Cria ponteiro para mascara com acesso a GetraioBolaTangente

  // Processamento da abertura em si
  // Obs:Deve zerar a matriz exceto as bordas(resolver)
  pm->Constante (indiceInativo);	// zera a matriz imagem
  // variáveis auxiliares para otimizacao
  // Otimizacao Mascara
  register int raio = maskd->RaioX ();
  int raioBolaTangente = maskd->GetraioBolaTangente ();
  int raioBolaInclusa = maskd->GetraioBolaInclusa ();
  register int **maskdata2D = maskd->Data2D ();
  // Otimizacao Imagem
  register int **pmdata2D = pm->Data2D ();

  // Variáveis para SIMETRIA Bola   // otimizacao
  unsigned int posxe, posxd;	// x esquerda e x direita
  unsigned int posys, posyn;	// y sul e y norte
  // unsigned int miXraio=mi*raio;
  unsigned int xx, yy;		// posicoes xx e yy da bola
  register int imx;		// i mais x, i+x
  register int rmx;		// raio mais x, raio+x
  for (int j = raio; j < ny - raio; j++)	// Percorre a imagem idf
    for (int i = raio; i < nx - raio; i++)
      {
	// se o ponto da idf for maior que a bola tangente, faz ponto=1
	if (data2D[i][j] > raioBolaTangente)	// p/d34 >3(raio+1)
	  pmdata2D[i][j] = indiceAtivo;	// seta ponto da imagem como pertencente a bola
	// se o ponto for maior que a bola inclusa, percorre a mascara e faz pontos=1
	else if (data2D[i][j] > raioBolaInclusa)	// p/d34 >3(raio)
	  {
/*
// // // // // // // // // // // // // // // // // // // 
// PINTA A BOLA NAO OTIMIZADO: NAO CONSIDERA SIMETRIA
			  for (int xx=-raio; xx<=raio; xx++)  // percorre eixo x da bola
             {
             imx=i+xx; rmx=raio+xx;			  // variaveis otimizacao
				 for (int yy=-raio; yy <=raio;yy++)// percorre eixo y da bola
               if(maskd->data2D[rmx][raio+yy]!=0)// se o ponto da bola é ativo
					pm->data2D[imx][j+yy]=1; 	  // pinta na imagem
             }
// // // // // // // // // // // // // // // // // // // 
*/
// // // // // // // // // // // // // // // // // // // 
// PINTA A BOLA OTIMIZADO: CONSIDERA SIMETRIA
	    for (xx = 0; xx <= raio; xx++)	// Usa simetria para pintura da bola
	      {			// só percorre meia bola
		posxe = i - xx;	// por ser simétrica
		posxd = i + xx;
		rmx = raio + xx;
		for (yy = 0; yy <= raio; yy++)
		  {
		    posys = j - yy;

		    posyn = j + yy;
		    if (maskdata2D[rmx][raio + yy] != 0)	// se o ponto da bola é ativo
		      pmdata2D[posxe][posyn] = pmdata2D[posxe][posys] =
			pmdata2D[posxd][posyn] = pmdata2D[posxd][posys] =
			indiceAtivo;
		  }		// yy
	      }			// xx
// // // // // // // // // // // // // // // // // // // 
	  }			// else if
      }				// for
//  DilatacaoNosContornos();                                                     // Realiza a dilatacao nos contornos
  return pm;			// pm é a matriz abertura
}

/*
============================================================================
Função    Dilatacao
============================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
CMatriz2D *
CFMMIDF::Dilatacao (CMatriz2D * &matriz, unsigned int _RaioBola)	// 
{
  // -->Padrao
  VerificaImagem (matriz);	// verifica se é a mesma imagem (se diferente recalcula Go)
  tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
  // Deve calcular o tamanhoMascara antes de criar a mascara
  CriaMascara (tamanhoMascara);	// Cria a mascara adequada,do tamanho de tamanhoMascara

  CBCDiscreta *maskd = dynamic_cast < CBCDiscreta * >(mask);	// Cria ponteiro para mascara com acesso a GetraioBolaTangente

  // Processamento da Dilatacao em si
  register int **pmdata2D = pm->Data2D ();
  int mi = Mi ();		// funcao definida na CFMMIDF, redeclarada nas filhas
  for (int j = 0; j < ny; j++)	// percorre toda a idf e
    for (int i = 0; i < nx; i++)	// pinta pontos na imagem
      if (data2D[i][j] >= mi)
	pmdata2D[i][j] = indiceAtivo;
      else
	pmdata2D[i][j] = indiceInativo;

  // Otimizacao Mascara (bola)
  int raio = maskd->RaioX ();
  // int raioBolaInclusa=maskd->GetraioBolaInclusa();
  // int raioBolaTangente=maskd->GetraioBolaTangente();
  register int **maskdata2D = maskd->Data2D ();

  register int imx;		// i mais x, i+x
  register int rmx;		// raio mais x, raio+x
  // Variáveis para SIMETRIA Bola
  int posxe, posxd;		// x esquerda e x direita
  int posys, posyn;		// y sul e y norte
  int xx, yy;			// posicoes xx e yy da bola
  for (int j = raio; j < ny - raio; j++)	// Percorre a imagem idf
    for (int i = raio; i < nx - raio; i++)
      {				// se for maior ou igual ao raio da bola inclusa,
	// e menor igual a bola tangente pinta bola
	if (data2D[i][j] == mi)	//  && data2D[i][j]<=raioBolaInclusa)
	  // PINTA A BOLA: CONSIDERA SIMETRIA
// // // // // // // // // // // // // // // // // // // 

// PINTA A BOLA OTIMIZADO: CONSIDERA SIMETRIA
	  for (xx = 0; xx <= raio; xx++)	// Usa simetria para pintura da bola
	    {			// só percorre meia bola
	      posxe = i - xx;	// por ser simétrica
	      posxd = i + xx;
	      rmx = raio + xx;
	      for (yy = 0; yy <= raio; yy++)
		{
		  posys = j - yy;
		  posyn = j + yy;
		  if (maskdata2D[rmx][raio + yy] != 0)	// se o ponto da bola é ativo
		    pmdata2D[posxe][posyn] = pmdata2D[posxe][posys] =
		      pmdata2D[posxd][posyn] = pmdata2D[posxd][posys] =
		      indiceAtivo;
		}		// yy
	    }			// xx
// // // // // // // // // // // // // // // // // // // 

/*
// // // // // // // // // // // // // // // // // // // 
// NAO OTIMIZADO
			  for (int x=-raio; x<=raio; x++)  // percorre eixo x da bola
             {
             imx=i+x; rmx=raio+x;			  // variaveis otimizacao
				 for (int y=-raio; y <=raio;y++)// percorre eixo y da bola
               if(maskd->data2D[rmx][raio+y]!=0)// se o ponto da bola é ativo
					pm->data2D[imx][j+y]=indiceAtivo; 	  // pinta na imagem
             }
// // // // // // // // // // // // // // // // // // // 
*/
      }

  //  DilatacaoNosContornos();                                    // Realiza a dilatacao nos contornos
  // verifica atualização idf
  if (atualizaIDF)		// verifica o flag de atualizacao da idf após dilatação
    Go (pm);			// se ativo recalcula a idf

  return pm;			// pm é a matriz Dilatacao
}

/*
============================================================================
Função    Fechamento
============================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
CMatriz2D *
CFMMIDF::Fechamento (CMatriz2D * &matriz, unsigned int _RaioBola)
{
  bool atualizaIDF_old = atualizaIDF;	// armazena valor de atualizaIDF
  atualizaIDF = 1;		// ativa, para que a Dilatacao recalcule  a idf
  Dilatacao (matriz, _RaioBola);	// processa a dilatação, e depois Go
  atualizaIDF = atualizaIDF_old;	// atualizaIDF volta ao estado anterior

  Erosao (matriz, _RaioBola);	// Processa a erosão, considerando imagem idf atualizada
  return pm;
}

/*
============================================================================
Função    Esqueleto.
============================================================================
*/
//         virtual CMatriz2D* Esqueleto(CMatriz2D*& matriz,unsigned int _RaioBola=0);
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 
// Depois que o calculo do esqueleto com o objeto esqueleto
// estiver pronto, copiar aqui?? resolver
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 
CMatriz2D *
CFMMIDF::Esqueleto (CMatriz2D * &matriz, unsigned int /*_RaioBola*/ )
{
  int mi = Mi ();
  // ----------------------------------------
  // -->Padrao
  // ----------------------------------------
  VerificaImagem (matriz);	// verifica se é a mesma imagem (se diferente recalcula Go)
  // Calculou a idf.
  // ----------------------------------------
  // Calcula o raio Maximo
  // ----------------------------------------
  // raioMaximo=GetmaiorValor()/mi;
  for (int j = 0; j < ny; j++)	// percorre toda a idf e
    for (int i = 0; i < nx; i++)	// pinta pontos na imagem
      if (data2D[i][j] > raioMaximo)
	raioMaximo = data2D[i][j];	// é na verdade o maior valor da matriz
  raioMaximo /= mi;		// agora é o raioMaximo  0->raioMaximo

  // ----------------------------------------
  //  Percorre a imagem de mi até raio maximo
  // ----------------------------------------
  pm->Constante (0);		// zera a imagem
  int raioBolaInclusa;
  for (int raio = 1; raio < raioMaximo; raio++)
    {
      raioBolaInclusa = mi * raio;	// 3*1
      for (int j = 2; j < ny - 1; j++)	// vai de 2 a ny-1 por causa d5711
	{			// para d34 pode ser de 1 a ny
	  for (int i = 2; i < nx - 1; i++)
	    {
	      // se o ponto da idf for maior que a bola tangente, faz ponto=1
	      if (data2D[i][j] == raioBolaInclusa)	// pesquisa
		{
		  if (data2D[i][j + 1] > raioBolaInclusa)
		    continue;	// se eu tiver algum vizinho maior que
		  else if (data2D[i][j - 1] > raioBolaInclusa)
		    continue;	// o raio da bola inclusa, eu não sou
		  else if (data2D[i + 1][j + 1] > raioBolaInclusa)
		    continue;	// um ponto do esqueleto (para o raio atual)
		  else if (data2D[i + 1][j] > raioBolaInclusa)
		    continue;	// sai
		  else if (data2D[i + 1][j - 1] > raioBolaInclusa)
		    continue;	// 
		  else if (data2D[i - 1][j + 1] > raioBolaInclusa)
		    continue;	// se eu nao tiver nenhum vizinho
		  else if (data2D[i - 1][j] > raioBolaInclusa)
		    continue;	// maior que o raioBolaInclusa, então
		  else if (data2D[i - 1][j - 1] > raioBolaInclusa)
		    continue;	// sou um ponto do esqueleto
		  else
		    pm->data2D[i][j] = 1;
		}

	    }
	}
    }
  return pm;
}

/*
============================================================================
Função    DilatacaoNosContornos
============================================================================
Como a mascara nao pode ser encaixada nos pontos de contorno, estes são
desconsiderados durante a diltacao. Aqui pinta os pontos desconsiderados
*/

void
CFMMIDF::DilatacaoNosContornos ()	// usada pela abertura
{
  // Indices para percorrer a matriz
  for (int y = 0; y < ny; y++)	// plano back z=0
    {
      if (data2D[0][y] != 0)
	pm->data2D[0][y] = 1;
      if (data2D[nx - 1][y] != 0)
	pm->data2D[nx - 1][y] = 1;
    }
  for (int x = 1; x < nx; x++)
    {
      if (data2D[x][0] != 0)
	pm->data2D[x][0] = 1;
      if (data2D[x][ny - 1] != 0)
	pm->data2D[x][ny - 1] = 1;
    }
}

/*
// 	void IDFNosPlanosDeContornoIda(int& base);     // Calcula a idf nos planos de contorno
// 	void IDFNosPlanosDeContornoVolta(int& base);     // Calcula a idf nos planos de contorno
void CFMMIDF::IDFNosPlanosDeContornoVOLTA(int& base)
{
 													// Indices para percorrer a matriz
	 for (int x=nx-2; x > 0 ;x--)			// nx-1
		 if (data2D[x][0]!=0)
				data2D[x][0]=base;
	 for (int y=ny-1; y > 0 ;y--)			// plano back z=0
		 if (data2D[0][y]!=0)
				data2D[0][y]=base;
}
void CFMMIDF::IDFNosPlanosDeContornoIDA(int& base)
{
 													// Indices para percorrer a matriz
	 for (int x=0; x < nx ;x++)
			 if (data2D[x][ny-1]!=0)
				data2D[x][ny-1]=base;
	 for (int y=0; y < ny ;y++)			// plano back z=0
			 if (data2D[nx-1][y]!=0)
				data2D[nx-1][y]=base;
}
*/
// Funcoes inline
//       inline void min(int& t);
void
CFMMIDF::min (const int &t)
{
  if (t < minimo)
    minimo = t;
}
