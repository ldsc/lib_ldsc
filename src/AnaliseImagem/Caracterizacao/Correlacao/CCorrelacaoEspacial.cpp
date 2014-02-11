
#include <AnaliseImagem/Caracterizacao/Correlacao/CCorrelacaoEspacial.h>

using namespace std;

// -----------------------------------------------------------------------
// Construtor:
// -----------------------------------------------------------------------
CCorrelacaoEspacial::CCorrelacaoEspacial ( int dimensao_vetor_correlacao ) 
	:  CCorrelacao ( dimensao_vetor_correlacao ) {
}

// -----------------------------------------------------------------------
// Destrutor:
// -----------------------------------------------------------------------
CCorrelacaoEspacial::~CCorrelacaoEspacial () {
}

// Calculo da correlacao sobre uma imagem float.
// O ponteiro _Re_data representa uma matriz que pode ser acessada usando:
// #define MatrizReal(x,y)	Re_data [(y)*NX + (x)]
// #define MatrizImag(x,y)	Im_data [(y)*NX + (x)]
bool CCorrelacaoEspacial::Go (float *Re_data, int NX, int NY, int indice) {
   if ( ! AlocarOuRealocarCorrelacaoSeNecessarioEZerar(NX/2) ){
      return false;
   }
	// Soma o número de poros para calcular a porosidade (primeiro ponto do vetor autocorrelacao).
	for ( int i = 0; i < NX; i++)
		for ( int j = 0; j < NY; j++)
         if( MatrizReal(i, j) == indice ) // == indice no lugar de 1
				correlacao[0]++;
	correlacao[0] = correlacao[0] / (NX * NY);

   for (int deslocamento = 1; deslocamento < size; deslocamento++) {
		// cout << "->deslocamento = " << deslocamento << endl;
      for (int i = 1; i < (NX - deslocamento); i++) {
         for (int j = 0; j < NY; j++) {
            if ( (MatrizReal(i, j) == indice) && (MatrizReal( i + deslocamento, j) == indice) ) {
					correlacao[ deslocamento ]++;
				}
			}
		}
      // Divide o numero de pixeis acumulados pela área considerada
      correlacao[deslocamento] =  correlacao[deslocamento] / ( (NX - deslocamento) * NY ) ;
	}
   return true;
}

bool CCorrelacaoEspacial::Go(TCMatriz2D< bool > *img, int indice) {
	if ( ! img ) 
		return false;

	int NX = img->NX();
	int NY = img->NY();

   if ( ! AlocarOuRealocarCorrelacaoSeNecessarioEZerar(NX/2) ){
      return false;
   }

	// soma o número de poros para calcular a porosidade.
	for ( int i = 0; i < NX; i++)
		for ( int j = 0; j < NY; j++)
         if( img->data2D[i][j] == indice )
				correlacao[0]++;
	correlacao[0] = correlacao[0] / (NX * NY);

   for (int deslocamento = 1; deslocamento < size; deslocamento++) {
      for (int i = 1; i < (NX - deslocamento) ; i++) {
         for (int j = 0; j < NY; j++) {
            if ( (img->data2D[i][j] == indice) && (img->data2D[ i + deslocamento ][j] == indice) ) {
					correlacao[ deslocamento ]++;
				}
			}
		}
      // Divide o numero de pixeis acumulados pela área considerada
      correlacao[deslocamento] =  correlacao[deslocamento] / ( (NX - deslocamento)*NY ) ;
	}
	return true;
}

// bool CCorrelacaoEspacial::Write (string fileName) 
// {
// 	if ( ! correlacao ) 
// 		return false;
// 	
// 	ofstream fcor ((fileName + ".cor").c_str()); 		//+rapido
// 	
// 	if ( ! fcor.is_open() )
// 		return false;
// 
// 	// Escreve o vetor correlacao em disco
// 	for (int deslocamento = 0; deslocamento < (size); deslocamento++)
// 		fcor << deslocamento << "\t" <<  correlacao[deslocamento]  << "\n";
// 
// 	fcor.close (); 						// Fecha o arquivo de disco
// 	return true;
// }

