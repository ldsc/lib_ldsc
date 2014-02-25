#include <AnaliseImagem/Filtro/FEspacial/FEInversao/CFEInversao3D.h>

TCImagem3D<bool> *CFEInversao3D::Go( TCImagem3D<bool> *imagem ) {
	if ( ! imagem )
		return nullptr;
	// só aceita imagens P&B
	if ( imagem->GetFormato() != D1_X_Y_Z_ASCII && imagem->GetFormato() != D4_X_Y_Z_BINARY)
		return nullptr;
	
	TCImagem3D<bool> * pi3Dinv = nullptr;
	pi3Dinv = new TCImagem3D<bool>( *imagem);
	if ( ! pi3Dinv )
		return nullptr;
	
	// percorre a imagem invertendo os valores.
	for ( int i = 0; i < pi3Dinv->NX(); i++ )
		for ( int j = 0; j < pi3Dinv->NY(); j++ )
			for ( int k = 0; k < pi3Dinv->NZ(); k++ )
				( pi3Dinv->data3D[i][j][k] == 0 ) ? pi3Dinv->data3D[i][j][k] = 1 : pi3Dinv->data3D[i][j][k] = 0;
	return pi3Dinv;
}

TCMatriz3D<bool> * CFEInversao3D::Go ( TCMatriz3D<bool> * imagem ) {
	if ( ! imagem )
		return nullptr;
	// só aceita imagens P&B
	if ( imagem->GetFormato() != D1_X_Y_Z_ASCII && imagem->GetFormato() != D4_X_Y_Z_BINARY)
		return nullptr;
	
	TCMatriz3D<bool> * pi3Dinv = nullptr;
	pi3Dinv = new TCMatriz3D<bool>( *imagem);
	if ( ! pi3Dinv )
		return nullptr;
	
	// percorre a imagem invertendo os valores.
	for ( int i = 0; i < pi3Dinv->NX(); i++ )
		for ( int j = 0; j < pi3Dinv->NY(); j++ )
			for ( int k = 0; k < pi3Dinv->NZ(); k++ )
				( pi3Dinv->data3D[i][j][k] == 0 ) ? pi3Dinv->data3D[i][j][k] = 1 : pi3Dinv->data3D[i][j][k] = 0;
	return pi3Dinv;
}
