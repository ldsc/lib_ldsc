//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
  ----------------------------------------------------------------------------
  PROJETO:      	Anaimp
  Analise de Imagens de Meios Porosos
  ----------------------------------------------------------------------------

  Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
  Termofisicas  dos Materiais.
  Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
  Liang Zirong, Paulo C. Philippi, ...
  Copyright @1997:  	Todos os direitos reservados.
	Nome deste arquivo:	TCFEMMIDFd5711.cpp
	Nome da classe:      TCFEMMIDFd5711
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
	Descricao:	 Implementa a função CriaMascara da classe TCFEMMIDFd5711.
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
//using namespace std;
#ifndef TCFEMMIDFd5711_h
#include "AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd5711.h"
#endif
//  Classe base
//  #include "AnaliseImagem/Filtro/Mascara/MCDiscreta\CMCd5711.h"  //  ponteiro para

/*
  ==================================================================================
  Documentacao 		Funcao Go
  ==================================================================================
  Descrição:
  Antes realizava um processo mais lento, criava uma mascara da mascara e depois
  percorria os valores da imagem usando esta mascara da mascara num processo mais confuso e lento.
  Da forma como esta não existe mais a necessidade de se definir uma mascara da mascara,
  de preencher os valores da mesma e realizar loops de calculo.
  Aqui substitui as chamadas a mascara da mascara pelo próprio valor, e substitui o loop pela repetição do código
  de teste para cada ponto da mascara.
  Para a d5711 tem-se 8 testes, o que não deixa o código muito extenso e
  compensa pela maior velocidade e compreensão.

  Observe que não percorre toda a imagem, como são acessados valores -2 e +2
  deve percorrer a parte interna da imagem, não acessando a borda da imagem.
  Ou seja desconsidera-se os pontos da borda da imagem.
*/
//  TCMatriz2D< int > * TCFEMMIDFd5711::Go( TCMatriz2D< int > *& matriz)
/*
	TCMatriz2D< int > *TCFEMMIDFd5711::Go( TCMatriz2D< int > *& matriz, unsigned int _tamanhoMascara )
  {
  InicializaIDF(matriz,_tamanhoMascara);//  armazena valores da matriz e _tamanhoMascara
  //  verifica se pm->data2D e this->data2D tem as mesmas dimensoes
  //  preenche os valores de data2D com os valores de pm
  IDFNosPlanosDeContorno(mi);				//  considera a primeira linha, fazendo=5
  //  deve considerar adicionalmente a segunda linha, pois abaixo a mesma não é verificada.
  int x,y;	      	//  Indices para percorrer a matriz
  int auxiliar,minimo;      //  variáveis auxiliares

  //  ida   MinimoIda
  //  Da forma como esta não percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
  for (y=2; y < ny ;y++)    	//  NY() é igual a ny, ny da matriz idf
  for (x=2; x < nx-2 ;x++)
  if (data2D[x][y]!=0)		//  Testa a imagem, se nao for solido entra
  {                                      //  
  minimo = raioMaximo;       	//  usa 32000
  //  -------------------------------------------------------------
  min(data2D[x-1][y]+5);     //  ponto [x][y]
  min(data2D[x-2][y-1]+11); min(data2D[x-1][y-1]+7);	
  min(data2D[x][y-1]+5);	min(data2D[x+1][y-1]+7);	
  min(data2D[x+2][y-1]+11);
  min(data2D[x-1][y-2]+11);							
  min(data2D[x+1][y-2]+11);
  //  -------------------------------------------------------------
  data2D[x][y]=minimo;
  }
  //  volta    MinimoVolta
  //  Da forma como esta não percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
  for (y = ny-3; y >= 0; y--)	     	//  -2 pois começa do zero e a mascara tem tamanho 1
  for (x = nx-3; x > 1; x--)
  if (data2D[x][y]!=0)	//  Se nao for solido
  {
  minimo = data2D[x][y];    //  Armazena valor minimo da ida
  //  ---------------------------------------------------------
  min(data2D[x-1][y+2]+11);									
  min(data2D[x+1][y+2]+11);
  min(data2D[x-2][y+1]+11);	
  min(data2D[x-1][y+1]+7);	
  min(data2D[x][y+1]+5);	
  min(data2D[x+1][y+1]+7);	
  min(data2D[x+2][y+1]+11);     //  ponto [x][y]  
  min(data2D[x+1][y]+5);
  //  -------------------------------------------------------------
  data2D[x][y]=minimo;
  }
  return pm;
  }
*/

/*================================================================================
 * Função    CorrigeAbertura
 * ==================================================================================
 * Este método corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.
 *
 * Após mudar para template precisei replicar em todas as classes base de CFEMMIDF pois estava dando erro na compilação. Precisa estudar uma melhor maneira de implementar sem necessidade de replicação.
*/
template<typename T>
void TCFEMMIDFd5711<T>::CorrigeAbertura ( TCMatriz2D<T> * &matriz, int &regiao ) {
		// calcula idf d5711 da mascara
	 TCMatriz2D<int> *ptr_mask = static_cast<TCMatriz2D<int>*> ( this->mask );
	 TCFEMMIDFd5711<int> *idfMask = new TCFEMMIDFd5711<int> ( ptr_mask );
	 idfMask->Go ( ptr_mask );
	 // calcula idf d5711 da imagem abertura.
	 TCFEMMIDFd5711<T> *idfAbertura = new TCFEMMIDFd5711<T> ( matriz );
	 idfAbertura->Go ( matriz );

	 // Método - 1; O melhor até agora!
	 pair<int,int> maiorMenor = idfMask->MaiorMenorValorNzero(); //maiorMenor.first = centro da máscara e maiorMenor.second = bordas da máscara
	 int centro = maiorMenor.first;
	 int borda  = maiorMenor.second;
	 int raiox = TCFEspacial<T>::mask->RaioX();
	 int raioy = TCFEspacial<T>::mask->RaioY();
	 for ( int j = raioy; j < this->ny - raioy; j++ ) {	// Percorre a imagem idf
			for ( int i = raiox; i < this->nx - raiox; i++ ) {
				 if ( matriz->data2D[i][j] == regiao ) { // se o ponto analizado faz parte da região abertura
						if ( idfAbertura->data2D[i][j] == centro ) {
							 if ( ( idfAbertura->data2D[i-raiox][j] == borda )  && ( idfAbertura->data2D[i][j+raioy] < centro ) ) {
									if ( ( matriz->data2D[i][j+raioy+1] == regiao )
											 && ( matriz->data2D[i-1][j+raioy] != regiao )
											 && ( matriz->data2D[i+raiox+1][j+raioy] != regiao ) ) {
										 for ( int x = 0; x <= raiox; x++ )
												if ( matriz->data2D[i+x][j+raioy] == regiao )
													 matriz->data2D[i+x][j+raioy] = 0;
												else
													 break;
									}
							 } else if ( ( idfAbertura->data2D[i+raiox][j] == borda )  && ( idfAbertura->data2D[i][j+raioy] < centro ) ) {
									if ( ( matriz->data2D[i][j+raioy+1] == regiao )
											 && ( matriz->data2D[i+raiox][j+raioy] != regiao )
											 && ( matriz->data2D[i-raiox-1][j+raioy] != regiao ) ) {
										 for ( int x = -raiox; x <= raiox; x++ )
												if ( matriz->data2D[i+x][j+raioy-1] != regiao )
													 matriz->data2D[i+x][j+raioy] = 0;
									}
							 }
						} else if ( ( idfAbertura->data2D[i][j] < centro ) && ( idfAbertura->data2D[i][j] > borda ) ) {
							 if ( ( idfAbertura->data2D[i-raiox][j] == borda )  && ( idfAbertura->data2D[i][j+raioy] < centro ) ) {
									int raio = 1;
									int teste = 4;
									//cerr << "raio = " << raio << " teste = " << teste << " ponto = " << idfAbertura->data2D[i][j] << endl;
									while ( idfAbertura->data2D[i][j] > teste ) { //quando sair do loop, ten-se o raio correspondente a bola cujo valor de IDF (d34) no ponto central, seja igual ao valor do ponto analizado
										 raio++;
										 teste += 3;
										 //cerr << "raio = " << raio << " teste = " << teste << " ponto = " << idfAbertura->data2D[i][j] << endl;
									}
									if ( ( matriz->data2D[i][j+raio+1] == regiao )
											 && ( matriz->data2D[i-1][j+raio] != regiao )
											 && ( matriz->data2D[i+raio+1][j+raio] != regiao ) ) {
										 for ( int x = 0; x <= raio; x++ )
												if ( matriz->data2D[i+x][j+raio] == regiao )
													 matriz->data2D[i+x][j+raio] = 0;
												else
													 break;
									}
							 }
						}
				 }
			}
	 }
}

