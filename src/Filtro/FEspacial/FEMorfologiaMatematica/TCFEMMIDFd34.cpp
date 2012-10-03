//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
/*
  ----------------------------------------------------------------------------
  PROJETO:		Anaimp
  Analise de Imagens de Meios Porosos
  ----------------------------------------------------------------------------

  Desenvolvido por:    	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
 Termofisicas  dos Materiais.
  Programadores:       	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	CFEMMIDFd34.cpp
  Nome da classe:      CFEMMIDFd34
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  Descricao:	 Implementa a função CriaMascara da classe CFEMMIDFd34.
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
//using namespace std;
#ifndef TCFEMMIDFd34_h
#include "Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd34.h"	//  Classe base
#endif
//  #include "Mascara/MCDiscreta\CMCd34.h"              //  ponteiro para
//  #include "Mascara/CMascMascDiscretad34.h"            //  Cria objeto

/*
  ==================================================================================
  Documentacao      Go
  ==================================================================================
  Descrição:
  //  Substitui o uso da mascara da mascara, que usava loop's e índices confusos
  //  pelo acesso direto aos valores da mascara.
  //  Ao lado do código o esboço do ponto da mascara que esta sendo considerado.
  //  Da forma como esta o código fica mais rápido e compreensivo.
  Programador:      Andre Duarte Bueno
*/
/*
//  TCMatriz2D< int > * CFEMMIDFd34::Go(TCMatriz2D< int > * matriz )
TCMatriz2D< int > *CFEMMIDFd34::Go( TCMatriz2D< int > *& matriz, unsigned int _tamanhoMascara )
{
InicializaIDF(matriz,_tamanhoMascara);//  armazena valores da matriz e _tamanhoMascara
//  verifica se pm->data2D e this->data2D tem as mesmas dimensoes
//  preenche os valores de data2D com os valores de pm
//  Fim da funcao InicializaGo(matriz,_tamanhoMascara)
IDFNosPlanosDeContorno(mi);

int x,y;		  //  Indices para percorrer a matriz
int auxiliar,minimo;

//  ida   MinimoIda
//  Da forma como esta não percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
for (y=1; y < ny ;y++)             //  NY() é igual a ny, ny da matriz idf
for (x=1; x < nx-1 ;x++)
if (data2D[x][y]!=0)		  //  Testa a imagem, se nao for solido entra
{                                  //  
  minimo = raioMaximo;
  //  -----------------------------------------------------------
  min(data2D[x-1][y]+3);	    //  ponto[x][y]
  min(data2D[x-1][y-1]+4);	 min(data2D[x][y-1]+3);	min(data2D[x+1][y-1]+4);
  //  -----------------------------------------------------------

  data2D[x][y]=minimo;
 }
//  volta    MinimoVolta
//  Da forma como esta não percorre a borda (pontos 0,0 e 0,n ) pois causaria estouro por acesso a pontos inexistentes.
 for (y = ny-2; y >= 0; y--)		 //  -2 pois começa do zero e a mascara tem tamanho 1
   for (x = nx-2; x > 0; x--)
     if (data2D[x][y]!=0)                //  Se nao for solido
       {
	 minimo = data2D[x][y];           //  Armazena valor minimo da ida
	 //  -----------------------------------------------------------
	 min(data2D[x-1][y+1]+4);	min(data2D[x][y+1]+3);	 min(data2D[x+1][y+1]+4);
	 //  ponto[x][y]
	 min(data2D[x+1][y]+3);
	 //  -----------------------------------------------------------
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
void TCFEMMIDFd34<T>::CorrigeAbertura ( TCMatriz2D<T> * &matriz, int &regiao ) {
	 //char fileName[64];
	 // calcula idf d34 da mascara
	 TCMatriz2D<int> *ptr_mask = static_cast<TCMatriz2D<int>*> ( this->mask );
	 TCFEMMIDFd34<int> *idfMask = new TCFEMMIDFd34<int> ( ptr_mask );
	 idfMask->Go ( ptr_mask );
	 //grava em disco a IDF da mascara.
	 /*
		 *    static int contMasK = 1;
		 *    sprintf (fileName, "idfMascara%d.pgm", contMasK++);
		 *    idfMask->SetFormato( WRITEFORM_PI_X_Y_GRAY_ASCII );
		 *      idfMask->NumCores( idfMask->MaiorValor()+1 );
		 *    idfMask->Write(fileName);
		 */
	 // calcula idf d34 da imagem abertura.
	 TCFEMMIDFd34<T> *idfAbertura = new TCFEMMIDFd34<T> ( matriz );
	 idfAbertura->Go ( matriz );
	 //grava em disco a IDF da imagem abertura.
	 /*
	 static int contAbertura = 1;
	 sprintf ( fileName, "idfAbertura%d.pgm", contAbertura++ );
	 idfAbertura->SetFormato ( P2_X_Y_GRAY_ASCII );
	 idfAbertura->NumCores ( idfAbertura->MaiorValor() +1 );
	 idfAbertura->Write ( fileName );
	*/
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
	 /*
		 *    // Método - 2
		 *    pair<int,int> maiorMenor = idfMask->MaiorMenorValorNzero(); //maiorMenor.first = centro da máscara e maiorMenor.second = bordas da máscara
		 *    int centro = maiorMenor.first;
		 *    int borda  = maiorMenor.second;
		 *    cerr << "maior: " << maiorMenor.first << endl;
		 *    cerr << "menor: " << maiorMenor.second << endl;
		 *    int raiox = CFEspacial<T>::mask->RaioX();
		 *    int raioy = CFEspacial<T>::mask->RaioY();
		 *    cerr << "raiox: " << raiox << endl;
		 *    cerr << "raioy: " << raioy << endl << endl;
		 *    bool achou;
		 *    for (int j = raioy; j < this->ny - raioy; j++) {	// Percorre a imagem idf
		 *    		for (int i = raiox; i < this->nx - raiox; i++) {
		 *    			achou = false;
		 *    			if ( ( data2D[i][j] == borda ) && ( data2D[i][j-raioy] == centro ) && ( matriz->data2D[i][j] == regiao ) ) {
		 *    				for (int x = 1; x <= raiox; x++) {
		 *    					if(data2D[i+x][j] >= centro) {
		 *    						achou = true;
		 *    						break;
		 }
		 }
		 if( ! achou ) {
			for (int x = 0; x <= raiox; x++) {
			 if(matriz->data2D[i+x][j] == regiao) {
				matriz->data2D[i+x][j] = 0;
		 }
		 }
		 }
		 }
		 }
		 }
		 */
	 /*// Método - 3 Mão acançou bons resultados
		 *    pair<int,int> maiorMenor = idfMask->MaiorMenorValorNzero(); //maiorMenor.first = centro da máscara e maiorMenor.second = bordas da máscara
		 *    int centro = maiorMenor.first;
		 *    int borda  = maiorMenor.second;
		 *    int raiox = CFEspacial<T>::mask->RaioX();
		 *    int diametrox  = (2 * raiox) + 1;
		 *    int raioy = CFEspacial<T>::mask->RaioY();
		 *    int cont;
		 *    bool flag;
		 *    for (int j = raioy; j < this->ny - raioy; j++) {	// Percorre a imagem idf
		 *    	for (int i = raiox; i < this->nx - raiox; i++) {
		 *    		cont = 0;
		 *    		flag = true;
		 *    		if ( matriz->data2D[i][j] == regiao ) { // se o ponto analizado faz parte da região abertura
		 *    		if ( idfAbertura->data2D[i][j] == borda ) {
		 *    			cont++;
		 *    			while (flag){
		 *    				if ( (i + cont) < this->nx ) { // evitar estouro.
		 *    					if ( matriz->data2D[i+cont][j] == regiao ){
		 *    						if (idfAbertura->data2D[i+cont][j] != borda) {
		 *    							cont++;
		 } else {
			cont++;
			flag = false;
		 }
		 } else {
			flag = false;
		 }
		 } else {
			flag = false;
		 }
		 }
		 if ( (cont < diametrox) && (cont > 2) ) {
			for (int x = 0; x < cont; x++) {
			 matriz->data2D[i+x][j] = 0;
		 }
		 } else if (cont == 2 && matriz->data2D[i-1][j] != regiao && matriz->data2D[i+2][j] != regiao) {
			matriz->data2D[i][j] = 0;
			matriz->data2D[i+1][j] = 0;
		 }
		 }
		 }
		 }
		 }
//			    int maior = idfMask->MaiorValor();
//			    int menor = idfMask->MenorValorNzero();
//			    int raiox = CFEspacial<T>::mask->RaioX();
//			    int raioy = CFEspacial<T>::mask->RaioY();
//			    for (int j = raioy; j < this->ny - raioy; j++) {	// Percorre a imagem idf
//			    	for (int i = raiox; i < this->nx - raiox; i++) {
//			    		if ( ( data2D[i][j] 		< 	maior )
//			    		  && ( data2D[i-raiox][j] 	< 	menor )
//			    		  && ( data2D[i+raiox][j] 	<	menor )
//			    		  && ( matriz->data2D[i][j]	== 	regiao ) )
//			    		{
//			    			matriz->data2D[i][j] = 0;
//			}
//			}
//			}

				pair<int,int> maiorMenor = idfMask->MaiorMenorValorNzero(); //maiorMenor.first = centro da máscara e maiorMenor.second = bordas da máscara
				cerr << "maior: " << maiorMenor.first << endl;
				cerr << "menor: " << maiorMenor.second << endl;
				int raiox = CFEspacial<T>::mask->RaioX();
				int raioy = CFEspacial<T>::mask->RaioY();
				cerr << "raiox: " << raiox << endl;
				cerr << "raioy: " << raioy << endl;
				for (int j = raioy; j < this->ny - raioy; j++) {	// Percorre a imagem idf
						for (int i = raiox; i < this->nx - raiox; i++) {
							if ( ( data2D[i][j] 		<= 	maiorMenor.first )
								&& ( data2D[i-raiox][j] 	<= 	maiorMenor.second )
								&& ( data2D[i+raiox][j] 	<=	maiorMenor.second )
								&& ( matriz->data2D[i][j]	== 	regiao )
								&& ( data2D[i][j-raioy] 	< 	maiorMenor.first ) )
							{
								matriz->data2D[i][j-raioy] = 0;
		 }
		 }
		 }


				pair<int,int> maiorMenor = idfMask->MaiorMenorValorNzero(); //maiorMenor.first = centro da máscara e maiorMenor.second = bordas da máscara
				int raiox = CFEspacial<T>::mask->RaioX();
				int raioy = CFEspacial<T>::mask->RaioY();
				for (int j = raioy; j < this->ny - raioy; j++) {	// Percorre a imagem idf
					for (int i = raiox; i < this->nx - raiox; i++) {
						if ( ( data2D[i][j] 		<= 	maiorMenor.first )
							&& ( matriz->data2D[i][j]	== 	regiao )
							&& ( data2D[i-raiox][j] 	<= 	maiorMenor.second )
							&& ( data2D[i+raiox][j] 	<=	maiorMenor.second ) )
						{
							for(int x = -raiox; x <= raiox; x++)
								matriz->data2D[i+x][j] = 0;
		 }
		 }
		 }
		 */
}

