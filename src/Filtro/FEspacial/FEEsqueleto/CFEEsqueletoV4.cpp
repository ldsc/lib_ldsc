// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
		       	Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:      	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
 Termofisicas  dos Materiais.
Programadores:         	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, 
Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEEEsqueletizacao.cpp
Nome da classe:      CFEEEsqueletizacao
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
      	Algoritimo de esqueletização
*/

/*
----------------------------------------------------------------------------
				BIBLIOTECAS
----------------------------------------------------------------------------
*/

#include "Base/COperacao.h"

#include "Filtro/FEspacial/FEEsqueleto/CFEEsqueletoV4.h"

#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFd34.h"

#include "Filtro/FEspacial/CFEBorda.h"

#include "Filtro/FEspacial/CFEBSubstitueValor.h"

// ------------------------------------------------------
// Esqueleto Tentativa 1:
// ------------------------------------------------------
// Procedimento: Calcular a idf, eliminar pontos em que idf > 4
// Erros:
// Erro 1: Uma conexao do tipo abaixo é eliminada
// 333
// 363
// 333<-eliminado porque tem vizinho >4
//   3
//   3
//    3
// Erro 2: Na imagem final ficam pontos 4 e 3 que poderiam ser eliminados

TCMatriz2D< int > *
CFEEsqueletoV4::Go (TCMatriz2D< int > * &imagem, unsigned int /*_tamanhoMascara*/ )
{
  if (idf == NULL)
    {
			idf = new CFEMMIDFd34<int> (imagem);	// Cria a idf, e testa alocacao
      COperacao::TestaAlocacao (idf,
				"Erro alocacao idf, funcao Go, objeto CFEEsqueletoV4");
    }

  int espessuraBorda = 2;	// Espessura da borda (2 para d5711 e 1 para d34)
  {
    int valorBorda = 0;		// Elimina a borda
    CFEBorda (valorBorda).Go (imagem, espessuraBorda);
  }

  int i, j;
  idf->Go (imagem);		// Calcula a idf
  int valorMaximo = idf->MaiorValor ();	// valor maximo da idf
  int valorMinimo = idf->Mi ();	// valor minimo da idf

  int numeroVizinhosMaiores;	// variaveis método
  int mi = idf->Mi ();		// funcao virtual, retorna menor valor da mascara utilizada
//   int mip1=mi+1;// 4                          // Variaveis otimizacao
  int im1, i_1, jm1, j_1;	// 
	Matriz2D(int) idfdata = idf->Data2D ();	// captura ponteiro para imagem de dados

  int somaVizinhos;
  for (int v = valorMinimo; v < valorMaximo; v++)	// Percorre a imagem do valor minimo ao máximo
    {
      // -----------
      // ESQUELETO 4
      // -----------
      for (j = espessuraBorda; j < idf->NY () - espessuraBorda + 1; j++)
	{
	  jm1 = j + 1;
	  j_1 = j - 1;
	  for (i = espessuraBorda; i < idf->NX () - espessuraBorda + 1; i++)
	    {
	      im1 = i + 1;
	      i_1 = i - 1;
	      if (idf->data2D[i][j] == mi)	// agora tem menos pontos 3
		{
		  // Condicao para esqueleto 3
		  // d4                        // se o viznho >3 soma. se a soma>=10 elimina
		  somaVizinhos = 0;
		  if (idfdata[i][jm1] > 3)
		    somaVizinhos += idfdata[i][jm1];
		  if (idfdata[i][j_1] > 3)
		    somaVizinhos += idfdata[i][j_1];
		  if (idfdata[im1][j] > 3)
		    somaVizinhos += idfdata[im1][j];
		  if (idfdata[i_1][j] > 3)
		    somaVizinhos += idfdata[i_1][j];
		  // diagonal
		  if (idfdata[im1][jm1] > 3)
		    somaVizinhos += idfdata[im1][jm1];
		  if (idfdata[im1][j_1] > 3)
		    somaVizinhos += idfdata[im1][j_1];
		  if (idfdata[i_1][jm1] > 3)
		    somaVizinhos += idfdata[i_1][jm1];
		  if (idfdata[i_1][j_1] > 3)
		    somaVizinhos += idfdata[i_1][j_1];

		  if (somaVizinhos >= 10)
		    idfdata[i][j] = 0;	// descasca
		}
	    }
	}


      // novo colocado em 2007
      //      CFEMMIDFd34* ptrImg2D = idf;
      TCMatriz2D< int >* pMatriz = dynamic_cast<TCMatriz2D< int >*>( idf );
      idf->Go (pMatriz,0);		// atualiza a idf após descascamento
      // idf->Go (idf);		// FORMATO ANTIGO

    }
  for (j = 0; j < idf->NY (); j++)	// Marca na imagem os pontos que fazem parte do esqueleto
    for (i = 0; i < idf->NX (); i++)	// 
      {
	if (imagem->data2D[i][j] != 0)	// se faz parte da imagem pintar com  a cor pixelObjeto
	  imagem->data2D[i][j] = valorObjeto;
	if (idfdata[i][j] != 0)	// se faz parte do esqueleto pintar com a cor do esqueleto
	  imagem->data2D[i][j] = valorEsqueleto;
      }
  // Depois de determinar o esqueleto:
  // eliminar pontos isolados
  // eliminar ramos isolados
  return imagem;
}


