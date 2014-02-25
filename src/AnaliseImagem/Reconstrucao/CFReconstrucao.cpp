#include <AnaliseImagem/Reconstrucao/CFReconstrucao.h>

TCMatriz3D<int> * CFReconstrucao::Go (TCMatriz2D< int > * matriz2D, Eeixo _eixo) {
	int i, j, k;
	int nxyz = matriz2D->NX(); // Dimensões das matrizes (quadrada ou cúbica).
			
	TCMatriz3D<int> * pm3D = nullptr;
	pm3D = new TCMatriz3D<int>(nxyz, nxyz, nxyz);
	
	if ( ! pm3D ) { // se não conseguiu alocar a matriz 3D exibe erro e retorna nullptr
		cerr << "CFReconstrucao::Go(TCMatriz2D< int > * matriz2D) não conseguiu alocar CMatriz3D!" << endl; 
		return nullptr; 	
	}
	//int umterco = nxyz / 3;
	switch (_eixo){
		case E_EIXO_X:
			// Só está rotacionando em Y! Precisa modificar o códico abaixo para rotacionar em X...
			for ( i = 0; i < nxyz; i++ ) {
				for ( j = 0; j < nxyz; j++ ) {
					for ( k = 0; k < nxyz; k++ ) { // [k][i] - [j][k] - [i][k] - [j][i]
						//if ( (i >= umterco && i <= ( 2 * umterco)) || (k >= umterco && k <= ( 2 * umterco)) )
							if ( (matriz2D->data2D[k][j] == 1 || matriz2D->data2D[i][j] == 1) ){
								pm3D->data3D[i][j][k] = 1;
								pm3D->data3D[nxyz - i - 1][j][k] = 1;
								//pm3D->data3D[i][nxyz - j - 1][k] = 1;
								pm3D->data3D[i][j][nxyz - k - 1] = 1;
							}
					}
				}
			}
		break;
		case E_EIXO_Y:
			for ( i = 0; i < nxyz; i++ ) {
				for ( j = 0; j < nxyz; j++ ) {
					for ( k = 0; k < nxyz; k++ ) { // [k][i] - [j][k] - [i][k] - [j][i]
						//if ( (i >= umterco && i <= ( 2 * umterco)) || (k >= umterco && k <= ( 2 * umterco)) )
                        if ( (matriz2D->data2D[k][j] == 1 || matriz2D->data2D[i][j] == 1) ){
                            pm3D->data3D[i][j][k] = 1;
                            pm3D->data3D[nxyz - i - 1][j][k] = 1;
                            //pm3D->data3D[i][nxyz - j - 1][k] = 1;
                            pm3D->data3D[i][j][nxyz - k - 1] = 1;
                        }
					}
				}
			}
		break;
	}
	return pm3D;
}

TCMatriz3D<int> * CFReconstrucao::Go(string arquivo, Eeixo _eixo) {
	TCMatriz2D< int > * pm = nullptr;
	pm = new TCMatriz2D< int >(arquivo);
	if( ! pm ) {
		cerr << "CFReconstrucao::G0(string arquivo) não conseguiu alocar TCMatriz2D< int >!" << endl; 
		return nullptr;
	}
	return Go(pm, _eixo);	
}
