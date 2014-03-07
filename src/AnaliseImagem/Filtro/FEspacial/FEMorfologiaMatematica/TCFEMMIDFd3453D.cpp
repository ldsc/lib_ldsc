/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEMMIDFd3453D.cpp
Nome da classe:      TCFEMMIDFd3453D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a função CriaMascara da classe TCFEMMIDFd3453D.
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#ifndef TCFEMMIDFd3453D_h
#include "AnaliseImagem/Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFd3453D.h"	// Classe base
#endif

/*================================================================================
Função    CorrigeAbertura
==================================================================================
Este método corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.

ATENÇÃO: Após mudar para template precisei replicar em todas as classes base de CFEMMIDF pois estava dando erro na compilação. Precisa estudar uma melhor maneira de implementar sem necessidade de replicação. */
template<typename T>
void TCFEMMIDFd3453D<T>::CorrigeAbertura (TCMatriz3D<T> * &matriz, int &regiao) {
	// Método - 2
	TCMatriz3D<int> *ptr_mask = static_cast<TCMatriz3D<int> *>(this->mask);
	// calcula idf d345 da mascara
	TCFEMMIDFd3453D<int> *idfMask = new TCFEMMIDFd3453D<int>(ptr_mask);
	idfMask->Go(ptr_mask);

	// calcula idf d345 da imagem abertura.
	TCFEMMIDFd3453D<T> *idfAbertura = new TCFEMMIDFd3453D<T>(matriz);
	idfAbertura->Go(matriz);

	// Pega plano central da mascara
	TCMatriz2D<int> * pcm = idfMask->LePlano(this->mask->RaioZ(), EIXO_Z);

	/* ********************************************************
	 * matriz 	-> é a imagem abertura atual.
	 * this 		-> é a IDF da imagem original.
	 * idfAbertura	-> é a IDF da imagem abertura atual.
	 * mask 		-> é a máscara atual.
	 * idfMask	-> é a IDF da mascara atual.
	 * pcm 		-> é o plano central de idfMask.
	*/
	pair<int,int> maiorMenor = pcm->MaiorMenorValorNzero(); //maiorMenor.first = centro da máscara e maiorMenor.second = bordas da máscara
	int centro = maiorMenor.first;
	int borda  = maiorMenor.second;
	int raiox = this->mask->RaioX();
	int raioy = this->mask->RaioY();
	int raioz = this->mask->RaioZ();
	int cont, temp;
	for (int j = raioy+2; j < this->ny-raioy-2; j++) {
		for (int k = raioz+2; k < this->nz-raioz-2; k++) {
			for (int i = raiox+2; i < this->nx-raiox-2; i++) { // Percorre a imagem idf
				if ( matriz->data3D[i][j][k] == regiao ) { // Se o pixel analizado faz parte da região abertura
					if ( idfAbertura->data3D[i][j][k] >= centro ) { // Se o ponto da IDF da região abertura for >= ao centro da idfMask
						// Verificacoes para saber se a mascara, centrada no ponto analizado, passa pela regiao abertura e tem suas bordas próximas ao fundo da imagem.
						if ( ( ( idfAbertura->data3D[i-raiox][j][k] == borda ) && ( idfAbertura->data3D[i][j][k-raioz] == borda ) && ( idfAbertura->data3D[i+raiox][j][k] < centro ) && ( idfAbertura->data3D[i+raiox-1][j][k] < centro ) && ( idfAbertura->data3D[i][j][k+raioz] < centro ) && ( idfAbertura->data3D[i][j][k+raioz-1] < centro ) )
								 || ( ( idfAbertura->data3D[i-raiox][j][k] == borda ) && ( idfAbertura->data3D[i][j][k-raioz] == borda ) && ( idfAbertura->data3D[i+raiox+1][j][k] == borda ) && ( idfAbertura->data3D[i][j][k+raioz+1] == borda ) )
								 || ( ( idfAbertura->data3D[i-raiox-1][j][k] == borda ) && ( idfAbertura->data3D[i][j][k-raioz-1] == borda ) && ( idfAbertura->data3D[i+raiox+1][j][k] == borda ) && ( idfAbertura->data3D[i][j][k+raioz+1] == borda ) && ( idfAbertura->data3D[i+raiox+2][j][k] == 0 ) && ( idfAbertura->data3D[i][j][k+raioz+2] == 0 ) )
								 || ( ( idfAbertura->data3D[i-raiox-2][j][k] == borda ) && ( idfAbertura->data3D[i][j][k-raioz-2] == borda ) && ( idfAbertura->data3D[i+raiox+2][j][k] < centro ) && ( idfAbertura->data3D[i][j][k+raioz+2] < centro ) && ( idfAbertura->data3D[i+raiox+2][j][k] != 0 ) && ( idfAbertura->data3D[i][j][k+raioz+2] != 0 ) ) ) {
							cont = 0;
							temp = centro-1;
							while ( cont <= raioy ) { // verificas os planos seguintes, na altura do ponto ora analizado, para verificar até onde a mascara passa.
								if (matriz->data3D[i][j+cont+1][k] != regiao){ // o ponto não pertence a região abertura
									cont = 0;
									break;
								}
								if ( idfAbertura->data3D[i][j+cont+1][k] <= temp ){ // a mascara nao passa pola regiao
									temp = idfAbertura->data3D[i][j+cont+1][k];
									cont++;
								} else { //sai do loop
									break;
								}
							}
							if (cont > 0) {
								int inc = cont; //(cont / 2) + 3; //cerr << inc << endl;
								if ( matriz->data3D[i][j+inc][k] == regiao )
                                    this->FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i-1][j+inc][k] == regiao )
                                    this->FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i+1][j+inc][k] == regiao )
                                    this->FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i][j+inc][k-1] == regiao )
                                    this->FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i][j+inc][k+1] == regiao )
                                    this->FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i-1][j+inc][k-1] == regiao )
                                    this->FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i+1][j+inc][k+1] == regiao )
                                    this->FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i-1][j+inc][k+1] == regiao )
                                    this->FecharAdjacencias ( i, k, j+inc, matriz  );
								else if ( matriz->data3D[i+1][j+inc][k-1] == regiao )
                                    this->FecharAdjacencias ( i, k, j+inc, matriz  );
							}
						}
					} else if ( (idfAbertura->data3D[i][j][k] == 3) &&
											(idfAbertura->data3D[i-1][j][k] == 0) &&
											(idfAbertura->data3D[i+1][j][k] == 0) &&
											(idfAbertura->data3D[i][j][k-1] == 0) &&
											(idfAbertura->data3D[i][j][k+1] == 0) &&
											(matriz->data3D[i][j-1][k] == regiao) &&
											(matriz->data3D[i][j+1][k] == regiao) )
					{ //fecha pixeis isolados...
						matriz->data3D[i][j][k] = 0;
					}
				}
			}
		}
	}
	delete idfMask;
	delete idfAbertura;
	delete pcm;
}


