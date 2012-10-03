//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
/*
  ----------------------------------------------------------------------------
  PROJETO:		Anaimp
  Analise de Imagens de Meios Porosos
  ----------------------------------------------------------------------------

  Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
Termofisicas  dos Materiais.
  Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
  Copyright @1997:  	Todos os direitos reservados.
	Nome deste arquivo:	TCFEMMIDFd4.cpp
	Nome da classe:      TCFEMMIDFd4
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
	Descricao:	 Implementa a função CriaMascara da classe TCFEMMIDFd4.
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
//using namespace std;
#ifndef TCFEMMIDFd4_h
#include "Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd4.h"	//  Classe base
#endif
//  #include "Mascara/MCDiscreta\CMCd4.h"  //  ponteiro para

/*
//  Mascara de chanfro d4
//  	   1
//   1  p  1
//      1
//  TCMatriz2D< int > * TCFEMMIDFd4::Go( TCMatriz2D< int > *& matriz)
//  Obs
TCMatriz2D< int > *TCFEMMIDFd4::Go( TCMatriz2D< int > *& matriz, unsigned int _tamanhoMascara )
{
ExecutadaPorGo(matriz);//  ,_tamanhoMascara);
IDFNosPlanosDeContorno(mi);
//  verifica se pm->data2D e this->data2D tem as mesmas dimensoes
//  preenche os valores de data2D com os valores de pm
int x,y;//  ,i;				//  Indices para percorrer a matriz
int auxiliar,minimo;                  	//  variaveis auxiliares

//  ida   MinimoIda
int y_1;				//  otimizacao
for (y=1; y < ny ;y++)                //  NY() é igual a ny, ny da matriz idf
{
  y_1=y-1;
  for (x=1; x < nx-1 ;x++)
    if (data2D[x][y]!=0)		//  Testa a imagem, se nao for solido entra
      {                             //  
	minimo = raioMaximo;	//  64000
	//  -----------------------------------------------------------
	min(data2D[x-1][y]+1);  //  ponto[x][y]
	min(data2D[x][y_1]+1);
	//  -----------------------------------------------------------

	data2D[x][y]=minimo;
      }
 }
//  volta    MinimoVolta
 int ym1;                              //  otimizacao
 for (y = ny-2; y >= 0; y--)		//  -2 pois começa do zero e a mascara tem tamanho 1
   {ym1=y+1;
     for (x = nx-2; x > 0; x--)
       if (data2D[x][y]!=0)             //  Se nao for solido, entra
	 {
	   minimo = data2D[x][y];        //  armazena valor minimo ida
	   //  -----------------------------------------------------------
	   min(data2D[x][y+1]+1);
	   //  ponto[x][y]
	   min(data2D[x+1][y]+1);
	   //  -----------------------------------------------------------
	   data2D[x][y]=minimo;
	 }
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
void TCFEMMIDFd4<T>::CorrigeAbertura ( TCMatriz2D<T> * &matriz, int &regiao ) {
	 // calcula idf d4 da mascara
	 TCMatriz2D<int> *ptr_mask = static_cast<TCMatriz2D<int>*> ( this->mask );
	 TCFEMMIDFd4<int> *idfMask = new TCFEMMIDFd4<int> ( ptr_mask );
	 idfMask->Go ( ptr_mask );
	 // calcula idf d4 da imagem abertura.
	 TCFEMMIDFd4<T> *idfAbertura = new TCFEMMIDFd4<T> ( matriz );
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
