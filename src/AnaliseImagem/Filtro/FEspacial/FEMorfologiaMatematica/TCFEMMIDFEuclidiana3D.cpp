/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEMMIDFEuclidiana3D.cpp
Nome da classe:      TCFEMMIDFEuclidiana3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a função CriaMascara da classe TCFEMMIDFEuclidiana3D.
*/
// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#ifndef TCFEMMIDFEuclidiana3D_h
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFEuclidiana3D.h> // Classe base
#endif

#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCEuclidiana3D.h>

/*
==================================================================================
Documentacao 		CriaMascara
==================================================================================
Descrição:      Funcao que cria a mascara de chanfro adequada.
*/
template<typename T>
void TCFEMMIDFEuclidiana3D<T>::CriaMascara (unsigned int _tamanhoMascara) {
	if (this->mask)
		delete this->mask	;
	this->mask = new CBCEuclidiana3D (_tamanhoMascara);	//  valores mi,mj,rb definidos pelo construtor de CBCEuclidiana
}

/*================================================================================
Função    CorrigeAbertura
==================================================================================
Este método corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.

ATENÇÃO: Após mudar para template precisei replicar em todas as classes base de CFEMMIDF pois estava dando erro na compilação. Precisa estudar uma melhor maneira de implementar sem necessidade de replicação.*/
template<typename T>
void TCFEMMIDFEuclidiana3D<T>::CorrigeAbertura (TCMatriz3D<T> * &matriz, int &regiao) {
	// Método - 2
	TCMatriz3D<int> *ptr_mask = static_cast<TCMatriz3D<int> *>(this->mask);
	// calcula idf euclidiana da mascara
	TCFEMMIDFEuclidiana3D<int> *idfMask = new TCFEMMIDFEuclidiana3D<int>(ptr_mask);
	idfMask->Go(ptr_mask);

	/*	//grava em disco a IDF da mascara.
	char fileName[64];
	int max;
	static int contMasK = 1;
	sprintf (fileName, "idfMascara%d.dgm", contMasK++);
	idfMask->SetFormato( WRITEFORM_DI_X_Y_Z_GRAY_ASCII );
		max = idfMask->MaiorValor();
	idfMask->NumCores( (max > 1)? max : max+2 );
	idfMask->Path(matriz->Path());
	idfMask->Write(fileName);
*/

	// calcula idf euclidiana da imagem abertura.
	TCFEMMIDFEuclidiana3D<T> *idfAbertura = new TCFEMMIDFEuclidiana3D<T>(matriz);
	idfAbertura->Go(matriz);

	/*	//grava em disco a IDF da imagem abertura.
 static int contAbertura = 1;
 sprintf (fileName, "idfAbertura%d.dgm", contAbertura++);
 idfAbertura->SetFormato( WRITEFORM_DI_X_Y_Z_GRAY_ASCII );
		max = idfAbertura->MaiorValor();
		idfAbertura->NumCores( (max > 1)? max : max+2 );
 idfAbertura->Path(matriz->Path());
 idfAbertura->Write(fileName);
*/

	// Pega plano central da mascara
	TCMatriz2D<int> * pcm = idfMask->LePlano(this->mask->RaioZ(), EIXO_Z);

	/*	//grava em disco o plano da mascara.
 static int contPlanoMask = 1;
 sprintf (fileName, "planoMascara%d.pgm", contPlanoMask++);
 pcm->SetFormato( WRITEFORM_PI_X_Y_GRAY_ASCII );
		max = pcm->MaiorValor();
		pcm->NumCores( (max > 1)? max : max+2 );
 pcm->Path(matriz->Path());
 pcm->Write(fileName);
*/
	/////////////////////////////////////////////////////////
	// matriz 	-> é a imagem abertura atual.		  	//
	// this 		-> é a IDF da imagem original.	  	//
	// idfAbertura	-> é a IDF da imagem abertura atual. 	//
	// mask 		-> é a máscara atual.			  	//
	// idfMask	-> é a IDF da mascara atual.		  	//
	// pcm 		-> é o plano central de idfMask.		//
	/////////////////////////////////////////////////////////
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
					} else if ( (idfAbertura->data3D[i][j][k] == 3) && (idfAbertura->data3D[i-1][j][k] == 0) && (idfAbertura->data3D[i+1][j][k] == 0)
											&& 	(idfAbertura->data3D[i][j][k-1] == 0) && (idfAbertura->data3D[i][j][k+1] == 0) && (matriz->data3D[i][j-1][k] == regiao)
											&& 	(matriz->data3D[i][j+1][k] == regiao) ) { //fecha pixeis isolados...
						matriz->data3D[i][j][k] = 0;
					}
				}
			}
		}
	}
	delete idfMask;
	delete idfAbertura;
	delete pcm;

	/*	// Método - 1
 TCMatriz3D<int> *ptr_mask = static_cast<TCMatriz3D<int> *>(mask);
 char fileName[64];
 int max;
 // calcula idf d345 da mascara
 CFEMMIDFd3453D *idfMask = NULL;
 idfMask = new CFEMMIDFd3453D(ptr_mask);
 idfMask->Go(ptr_mask);
 //grava em disco a IDF da mascara.
 static int contMasK = 1;
 sprintf (fileName, "idfMascara%d.dgm", contMasK++);
 idfMask->SetFormato( WRITEFORM_DI_X_Y_Z_GRAY_ASCII );
		max = idfMask->MaiorValor();
		idfMask->NumCores( (max > 1)? max : max+2 );
 idfMask->Path(matriz->Path());
 idfMask->Write(fileName);
	// PROVAVELMENTE NÃO SERÁ USADO. SE FOR USAR IDF DA ABERTURA, TERÁ DE CALCULAR A IDF DE CADA PLANO DA IMAGEM ABERTURA E NÃO A IDF DA IMAGEM TODA (3D)
 // calcula idf d345 da imagem abertura.
 CFEMMIDFd3453D *idfAbertura = NULL;
 idfAbertura = new CFEMMIDFd3453D(matriz);
 idfAbertura->Go(matriz);
 //grava em disco a IDF da imagem abertura.
 static int contAbertura = 1;
 sprintf (fileName, "idfAbertura%d.dgm", contAbertura++);
 idfAbertura->SetFormato( WRITEFORM_DI_X_Y_Z_GRAY_ASCII );
		max = idfAbertura->MaiorValor();
		idfAbertura->NumCores( (max > 1)? max : max+2 );
 idfAbertura->Path(matriz->Path());
 idfAbertura->Write(fileName);

 // Pega plano central da mascara
 TCMatriz2D< int > * pcm = NULL;
// 	pcm = idfMask->LePlano(mask->RaioZ(), EIXO_Z);
 pcm = ptr_mask->LePlano(mask->RaioZ(), EIXO_Z);
 //grava em disco o plano da mascara.
	static int contPlanoMask = 1;
 sprintf (fileName, "planoMascara%d.pgm", contPlanoMask++);
 pcm->SetFormato( WRITEFORM_PI_X_Y_GRAY_ASCII );
		max = pcm->MaiorValor();
		pcm->NumCores( (max > 1)? max : max+2 );
 pcm->Path(matriz->Path());
 pcm->Write(fileName);
 /////////////////////////////////////////////////////////
 // matriz 	-> é a imagem abertura atual.		  	//
 // this 		-> é a IDF da imagem original.	  	//
 // idfAbertura	-> é a IDF da imagem abertura atual. 	//
 // mask 		-> é a máscara atual.			  	//
 // idfMask	-> é a IDF da mascara atual.		  	//
 // pcm 		-> é o plano central de mask.			//
 /////////////////////////////////////////////////////////

 //pair<int,int> maiorMenor = pcm->MaiorMenorValorNzero(); //maiorMenor.first = centro da máscara e maiorMenor.second = bordas da máscara
 //int centro = maiorMenor.first;
 //int borda  = maiorMenor.second;
 int raiox = mask->RaioX();
 int raioy = mask->RaioY();
 int raioz = mask->RaioZ();
 bool passa; // Se verdadeiro quer dizer que a mascara passa na região abertura
 for (int j = raioy+2; j < this->ny-raioy-2; j++) {
	for (int k = raioz+2; k < this->nz-raioz-2; k++) {	// Percorre a imagem idf
	 for (int i = raiox+2; i < this->nx-raiox-2; i++) {
		if ( matriz->data3D[i][j][k] == regiao ) { // Se o pixel analizado faz parte da região abertura
		 passa = true;
		 for (int x = 0; x <= 2*raiox; x++) { // percorre a mascara
			for (int y = 0; y <= 2*raioy; y++) {
			 if ( (pcm->data2D[x][y] != 0) && (matriz->data3D[i-raiox+x][j][k-raioy+y] != regiao) ) {
			 // Se o ponto da mascara faz parte da regiao mascara e o ponto correspondente da imagem abertura não pertence a regiao abertura.
				passa = false;
				break;
			 }
			}
			if (!passa) break; // Não passa, vai para o próximo ponto da imagem.
		 }
		 if (passa) { // A mascara passa pela regiao abertura. verificar se os pontos adjacentes a mascara, ou dois ponto adjacentes e dois pontos adjancentes + 1 não fazem parte da região abertura.
			if ( ( (matriz->data3D[i-raiox-1][j][k] != regiao) && (matriz->data3D[i+raiox+1][j][k] != regiao) && (matriz->data3D[i][j][k-raioy-1] != regiao) && (matriz->data3D[i][j][k+raioy+1] != regiao) )
				|| ( (matriz->data3D[i-raiox-1][j][k] != regiao) && (matriz->data3D[i+raiox+2][j][k] != regiao) && (matriz->data3D[i+raiox+1][j][k] == regiao) && (matriz->data3D[i][j][k-raioy-1] != regiao) && (matriz->data3D[i][j][k+raioy+1] == regiao) && (matriz->data3D[i][j][k+raioy+2] != regiao) )
				|| ( (matriz->data3D[i+raiox+1][j][k] != regiao) && (matriz->data3D[i-raiox-2][j][k] != regiao) && (matriz->data3D[i-raiox-1][j][k] == regiao) && (matriz->data3D[i][j][k-raioy-1] != regiao) && (matriz->data3D[i][j][k+raioy+1] == regiao) && (matriz->data3D[i][j][k+raioy+2] != regiao) )
				|| ( (matriz->data3D[i-raiox-1][j][k] != regiao) && (matriz->data3D[i+raiox+2][j][k] != regiao) && (matriz->data3D[i+raiox+1][j][k] == regiao) && (matriz->data3D[i][j][k+raioy+1] != regiao) && (matriz->data3D[i][j][k-raioy-1] == regiao) && (matriz->data3D[i][j][k-raioy-2] != regiao) )
				|| ( (matriz->data3D[i+raiox+1][j][k] != regiao) && (matriz->data3D[i-raiox-2][j][k] != regiao) && (matriz->data3D[i-raiox-1][j][k] == regiao) && (matriz->data3D[i][j][k+raioy+1] != regiao) && (matriz->data3D[i][j][k-raioy-1] == regiao) && (matriz->data3D[i][j][k-raioy-2] != regiao) ) ) {
			 // Os pontos adjacentes a borda da mascara não fazem parte da regiao abertura. verificar a mesma regiao no plano seguinte.
			 passa = true;
			 for (int x = 0; x <= 2*raiox; x++) { // percorre a mascara
				for (int y = 0; y <= 2*raioy; y++) {
				 if ( (pcm->data2D[x][y] != 0) && (matriz->data3D[i-raiox+x][j+1][k-raioy+y] != regiao) ) {
				 // Se o ponto da mascara faz parte da regiao mascara e o ponto correspondente da imagem abertura (no plano seguinte) não pertence a regiao abertura.
					passa = false;
					break;
				 }
				}
				if ( ! passa ) break; // Não passa, vai para o próximo ponto da imagem.
			 }
			 if ( ! passa ) { // A mascara NÃO passa pela regiao abertura (do plano seguinte). Fechar toda a regiao do plano j+raioy
				if ( matriz->data3D[i][j+raioy][k] == regiao )
				 FecharAdjacencias ( i, k, j+raioy, matriz  );
				else if ( matriz->data3D[i-1][j+raioy][k] == regiao )
				 FecharAdjacencias ( i, k, j+raioy, matriz  );
				else if ( matriz->data3D[i+1][j+raioy][k] == regiao )
				 FecharAdjacencias ( i, k, j+raioy, matriz  );
				else if ( matriz->data3D[i][j-raioy][k-1] == regiao )
				 FecharAdjacencias ( i, k, j+raioy, matriz  );
				else if ( matriz->data3D[i][j-raioy][k+1] == regiao )
				 FecharAdjacencias ( i, k, j+raioy, matriz  );
				else if ( matriz->data3D[i-1][j-raioy][k-1] == regiao )
				 FecharAdjacencias ( i, k, j+raioy, matriz  );
				else if ( matriz->data3D[i+1][j-raioy][k+1] == regiao )
				 FecharAdjacencias ( i, k, j+raioy, matriz  );
				else if ( matriz->data3D[i-1][j-raioy][k+1] == regiao )
				 FecharAdjacencias ( i, k, j+raioy, matriz  );
				else if ( matriz->data3D[i+1][j-raioy][k-1] == regiao )
				 FecharAdjacencias ( i, k, j+raioy, matriz  );
			 }
			}
		 }
		}
	 }
	}
		 }
//  	delete idfMask;
//  	delete idfAbertura;
 delete pcm;*/
}
