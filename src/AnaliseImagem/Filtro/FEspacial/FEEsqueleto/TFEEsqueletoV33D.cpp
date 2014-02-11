//Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef TFEEsqueletoV33D_cpp
#define TFEEsqueletoV33D_cpp
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TFEEEsqueletizacao.cpp
Nome da classe:      TFEEEsqueletizacao
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
							Algoritimo de esqueletização
*/
/*
----------------------------------------------------------------------------
			BIBLIOTECAS
----------------------------------------------------------------------------
*/


#include "base\TOperacao.h"

#include "TFiltro\TFEspacial3D\TFEEsqueleto3D\TFEEsqueletoV33D.h"

#include "TFiltro\TFEspacial3D\TFEMorfologiaMatematica3D\TFMMIDFd3453D.h"

#include "TFiltro\TFEspacial3D\TFEBorda3D.h"
#include "TFiltro\TFEspacial3D\TFEBSubstitueValor3D.h"

//------------------------------------------------------
//Esqueleto Tentativa 1:
//------------------------------------------------------
//Procedimento: Calcular a idf, eliminar pontos em que idf > 4
//Erros:
//Erro 1: Uma conexao do tipo abaixo é eliminada
//333
//363
//333<-eliminado porque tem vizinho >4
//  3
//  3
//   3
//Erro 2: Na imagem final ficam pontos 4 e 3 que poderiam ser eliminados

TMatriz3D *
TFEEsqueletoV33D::Go (TMatriz3D * &imagem, unsigned int /*_tamanhoMascara*/ )
{
  if (idf == NULL)
    {
      idf = new TFMMIDFd3453D (imagem);	//Cria a idf, e testa alocacao
      TOperacao::TestaAlocacao (idf,
				"Erro alocacao idf, funcao Go, objeto TFEEsqueletoV33D");
    }

  int espessuraBorda = 2;	//Espessura da borda (2 para d5711 e 1 para d34)
  {
    int valorBorda = 0;		//Elimina a borda
    TFEBorda3D (valorBorda).Go (imagem, espessuraBorda);
  }

  int i, j;
  idf->Go (imagem);		//Calcula a idf
  int valorMaximo = idf->GetMaiorValor ();	//valor maximo da idf
  int valorMinimo = idf->Getmi ();	//valor minimo da idf

  int numeroVizinhosMaiores;	//variaveis método
  int mi = idf->Getmi ();	//funcao virtual, retorna menor valor da mascara utilizada
//  int mip1=mi+1;//4                          //Variaveis otimizacao
  int im1, i_1, jm1, j_1;	//
  int ***idfdata = idf->GetData3D ();	//captura ponteiro para imagem de dados
  // no v3
  int somaVizinhos;

  //int km1, k_1;
  int k;			//*
  for (int v = valorMinimo; v < valorMaximo; v++)	//Percorre a imagem do valor minimo ao máximo
    {
      //------------
      //ESQUELETO V2
      //------------
      for (k = espessuraBorda; k < idf->GetNZ () - espessuraBorda + 1; k++)	//Percorre direcao z
	{
	  //km1=k+1; k_1=k-1;//*
	  for (j = espessuraBorda; j < idf->GetNY () - espessuraBorda + 1; j++)	//vai de 2 a ny-1 por causa da mascara d5711
	    {
	      jm1 = j + 1;
	      j_1 = j - 1;	//para d34 pode ser de 1 a ny
	      for (i = espessuraBorda; i < idf->GetNX () - espessuraBorda + 1;
		   i++)
		{
		  im1 = i + 1;
		  i_1 = i - 1;
		  //se o ponto da idf for maior que a bola tangente, faz ponto=1
		  if (idfdata[i][j][k] == mi)	//faz a pesquisa no ponto  igual a mi //se 3
		    {
		      //numeroVizinhosMaiores=0;//>4
		      //condicao para esqueleto 2
		      //d4  //se na d4 >=6 deleta  ou >5
		      if (idfdata[i][jm1][k] > 5)
			idfdata[i][j][k] = 0;
		      else if (idfdata[i][j_1][k] > 5)
			idfdata[i][j][k] = 0;
		      else if (idfdata[im1][j][k] > 5)
			idfdata[i][j][k] = 0;
		      else if (idfdata[i_1][j][k] > 5)
			idfdata[i][j][k] = 0;
		      //diagonal
		      else if (idfdata[im1][jm1][k] > 6)
			idfdata[i][j][k] = 0;
		      else if (idfdata[im1][j_1][k] > 6)
			idfdata[i][j][k] = 0;
		      else if (idfdata[i_1][jm1][k] > 6)
			idfdata[i][j][k] = 0;
		      else if (idfdata[i_1][j_1][k] > 6)
			idfdata[i][j][k] = 0;
		    }
		}
	    }
	}
      //-----------
      //ESQUELETO 3
      //-----------
      for (k = espessuraBorda; k < idf->GetNZ () - espessuraBorda + 1; k++)	//Percorre direcao z
	{
	  //km1=k+1; k_1=k-1;//*
	  for (j = espessuraBorda; j < idf->GetNY () - espessuraBorda + 1;
	       j++)
	    {
	      jm1 = j + 1;
	      j_1 = j - 1;
	      for (i = espessuraBorda; i < idf->GetNX () - espessuraBorda + 1;
		   i++)
		{
		  im1 = i + 1;
		  i_1 = i - 1;
		  if (idfdata[i][j][k] == mi)	//agora tem menos pontos 3
		    {
		      //Condicao para esqueleto 3
		      //d4                        //se o viznho >3 soma. se a soma>=10 elimina
		      somaVizinhos = 0;
		      if (idfdata[i][jm1][k] > 3)
			somaVizinhos += idfdata[i][jm1][k];
		      if (idfdata[i][j_1][k] > 3)
			somaVizinhos += idfdata[i][j_1][k];
		      if (idfdata[im1][j][k] > 3)
			somaVizinhos += idfdata[im1][j][k];
		      if (idfdata[i_1][j][k] > 3)
			somaVizinhos += idfdata[i_1][j][k];
		      //diagonal
		      if (idfdata[im1][jm1][k] > 3)
			somaVizinhos += idfdata[im1][jm1][k];
		      if (idfdata[im1][j_1][k] > 3)
			somaVizinhos += idfdata[im1][j_1][k];
		      if (idfdata[i_1][jm1][k] > 3)
			somaVizinhos += idfdata[i_1][jm1][k];
		      if (idfdata[i_1][j_1][k] > 3)
			somaVizinhos += idfdata[i_1][j_1][k];

		      if (somaVizinhos >= 10)
			idfdata[i][j] = 0;	//descasca
		    }
		}
	    }
	}
      idf->Go (idf);		//atualiza a idf após descascamento
    }
  for (k = 0; k < idf->GetNZ (); k++)	//Marca na imagem os pontos que fazem parte do esqueleto
    for (j = 0; j < idf->GetNY (); j++)	//Marca na imagem os pontos que fazem parte do esqueleto
      for (i = 0; i < idf->GetNX (); i++)	//
	{
	  if (imagem->data3D[i][j][k] != 0)	//se faz parte da imagem pintar com  a cor pixelObjeto
	    imagem->data3D[i][j][k] = valorObjeto;
	  if (idfdata[i][j][k] != 0)	//se faz parte do esqueleto pintar com a cor do esqueleto
	    imagem->data3D[i][j][k] = valorEsqueleto;
	}
  //Depois de determinar o esqueleto:
  //eliminar pontos isolados
  //eliminar ramos isolados
  return imagem;
}

#endif
