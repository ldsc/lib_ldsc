//Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef TFEEsqueletoV23D_cpp
#define TFEEsqueletoV23D_cpp
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
#include <stdio.h>


#include "base\TOperacao.h"

#include "TFiltro\TFEspacial3D\TFEEsqueleto3D\TFEEsqueletoV23D.h"

#include "TFiltro\TFEspacial3D\TFEMorfologiaMatematica3D\TFMMIDFd3453D.h"

#include "TFiltro\TFEspacial3D\TFEBorda3D.h"

#include "TFiltro\TFEspacial3D\TFEBSubstitueValor3D.h"

#include <fstream.h>
//#include <cstring.h>
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
TFEEsqueletoV23D::Go (TMatriz3D * &imagem, unsigned int /*_tamanhoMascara*/ )
{
  if (idf == NULL)
    {
      idf = new TFMMIDFd3453D (imagem);	//Cria a idf, e testa alocacao
      TOperacao::TestaAlocacao (idf,
				"Erro alocacao idf, funcao Go, objeto TFEEsqueletoV23D");
    }

  int espessuraBorda = 2;	//Espessura da borda (2 para d5711 e 1 para d34)
  {
    int valorBorda = 0;		//Elimina a borda
    TFEBorda3D (valorBorda).Go (imagem, espessuraBorda);
  }

  int i, j, k;
  idf->Go (imagem);		//Calcula a idf
  int valorMaximo = idf->GetMaiorValor ();	//valor maximo da idf
  int valorMinimo = idf->Getmi ();	//valor minimo da idf

  int numeroVizinhosMaiores;	//variaveis método
  int mi = idf->Getmi ();	//funcao virtual, retorna menor valor da mascara utilizada
  //int mip1=mi+1;//4                          //Variaveis otimizacao
  int im1, i_1, jm1, j_1;
  //int km1;//,k_1;               //
  int ***idfdata = idf->GetData3D ();	//captura ponteiro para imagem de dados
  int nx = idf->GetNX ();
  int ny = idf->GetNY ();
  int nz = idf->GetNZ ();

  for (int v = valorMinimo; v < valorMaximo; v++)	//Percorre a imagem do valor minimo ao máximo
    {
      for (k = espessuraBorda; k < nz - espessuraBorda + 1; k++)	//Percorre direcao z
	{
	  //km1=k+1; k_1=k-1;//*
	  for (j = espessuraBorda; j < ny - espessuraBorda + 1; j++)	//Percorre direcao y
	    {
	      jm1 = j + 1;
	      j_1 = j - 1;
	      for (i = espessuraBorda; i < nx - espessuraBorda + 1; i++)	//Percorre direcao x
		{
		  im1 = i + 1;
		  i_1 = i - 1;

		  if (idfdata[i][j][k] == mi)	//faz a pesquisa no ponto  //se igual a mi //mi=3 para d34 e d345
		    {		//se obedecer as condicoes abaixo, descasca o ponto
		      //PLANO  K
		      //d4
		      if (idfdata[i][jm1][k] > 5)
			idfdata[i][j][k] = 0;	//4+1
		      else if (idfdata[i][j_1][k] > 5)
			idfdata[i][j][k] = 0;
		      else if (idfdata[im1][j][k] > 5)
			idfdata[i][j][k] = 0;
		      else if (idfdata[i_1][j][k] > 5)
			idfdata[i][j][k] = 0;
		      //diagonal
		      else if (idfdata[im1][jm1][k] > 6)
			idfdata[i][j][k] = 0;	//5+1
		      else if (idfdata[im1][j_1][k] > 6)
			idfdata[i][j][k] = 0;
		      else if (idfdata[i_1][jm1][k] > 6)
			idfdata[i][j][k] = 0;
		      else if (idfdata[i_1][j_1][k] > 6)
			idfdata[i][j][k] = 0;

/*            //PLANO  K - 1
            //d4
            else if (idfdata[i  ][jm1][k] > 5) idfdata[i][j][k]=0;//4+1
            else if (idfdata[i  ][j_1][k] > 5) idfdata[i][j][k]=0;
            else if (idfdata[im1][j  ][k] > 5) idfdata[i][j][k]=0;
            else if (idfdata[i_1][j  ][k] > 5) idfdata[i][j][k]=0;
            //diagonal
            else if (idfdata[im1][jm1][k] > 6) idfdata[i][j][k]=0;//5+1
            else if (idfdata[im1][j_1][k] > 6) idfdata[i][j][k]=0;
            else if (idfdata[i_1][jm1][k] > 6) idfdata[i][j][k]=0;
            else if (idfdata[i_1][j_1][k] > 6) idfdata[i][j][k]=0;

            //PLANO  K  + 1
            //d4
            else if (idfdata[i  ][jm1][k] > 5) idfdata[i][j][k]=0;//4+1
            else if (idfdata[i  ][j_1][k] > 5) idfdata[i][j][k]=0;
            else if (idfdata[im1][j  ][k] > 5) idfdata[i][j][k]=0;
            else if (idfdata[i_1][j  ][k] > 5) idfdata[i][j][k]=0;
            //diagonal
            else if (idfdata[im1][jm1][k] > 6) idfdata[i][j][k]=0;//5+1
            else if (idfdata[im1][j_1][k] > 6) idfdata[i][j][k]=0;
            else if (idfdata[i_1][jm1][k] > 6) idfdata[i][j][k]=0;
            else if (idfdata[i_1][j_1][k] > 6) idfdata[i][j][k]=0;  */
		    }
		}
	    }
//inicio teste      
	  char msg[255];
	  sprintf (msg, "esqueleto-parcial-%d", v);
	  idf->Write (msg);
//fim teste
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
