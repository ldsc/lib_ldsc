//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
/*
  ----------------------------------------------------------------------------
	PROJETO: LibLDSC
  Analise de Imagens de Meios Porosos
  ----------------------------------------------------------------------------

	Desenvolvido por:    	Laboratorio de Desenvolvimento de Software Cientifico.
	Programadores:       	Andre D.Bueno, Leandro Puerari.
	Copyright @ 2009:  	Todos os direitos reservados.
  Nome deste arquivo:	CFEMMIDFd34.cpp
  Nome da classe:      CFEMMIDFd34
	Descricao:	 Implementa a função CorrigeAbertura para corrigir o erro físico observado nas configurações de equilíbrio.
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
//using namespace std;
#ifndef TCFEMMIDFd34_h
#include "AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd34.h"	//  Classe base
#endif
//  #include "AnaliseImagem/Filtro/Mascara/MCDiscreta\CMCd34.h"              //  ponteiro para
//  #include "AnaliseImagem/Filtro/Mascara/CMascMascDiscretad34.h"            //  Cria objeto

/*================================================================================
 * Função    CorrigeAbertura
 * ==================================================================================
 * Este método corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.
 *
 * Após mudar para template precisei replicar em todas as classes base de CFEMMIDF pois estava dando erro na compilação. Precisa estudar uma melhor maneira de implementar sem necessidade de replicação.
*/
template<typename T>
void TCFEMMIDFd34<T>::CorrigeAbertura ( TCMatriz2D<T> * &matriz, int &regiao ) {
	 // calcula idf d34 da mascara
	 TCMatriz2D<int> *ptr_mask = static_cast<TCMatriz2D<int>*> ( this->mask );
	 TCFEMMIDFd34<int> *idfMask = new TCFEMMIDFd34<int> ( ptr_mask );
	 idfMask->Go ( ptr_mask );

	 // calcula idf d34 da imagem abertura.
	 TCFEMMIDFd34<T> *idfAbertura = new TCFEMMIDFd34<T> ( matriz );
	 idfAbertura->Go ( matriz );

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

