#include <Filtro/FEspacial3D/FEInversao3D/CFEInversao3D.h>

CImagem3D * CFEInversao3D::Go ( CImagem3D * imagem ) {
	if ( ! imagem )
		return NULL;
	// só aceita imagens P&B
	if ( imagem->GetFormato() != D1_X_Y_Z_ASCII && imagem->GetFormato() != D4_X_Y_Z_BINARY)
		return NULL;
	
	CImagem3D * pi3Dinv = NULL;
	pi3Dinv = new CImagem3D( *imagem);
	if ( ! pi3Dinv )
		return NULL;
	
	// percorre a imagem invertendo os valores.
	for ( int i = 0; i < pi3Dinv->NX(); i++ )
		for ( int j = 0; j < pi3Dinv->NY(); j++ )
			for ( int k = 0; k < pi3Dinv->NZ(); k++ )
				( pi3Dinv->data3D[i][j][k] == 0 ) ? pi3Dinv->data3D[i][j][k] = 1 : pi3Dinv->data3D[i][j][k] = 0;
	return pi3Dinv;
}

TCMatriz3D<int> * CFEInversao3D::Go ( TCMatriz3D<int> * imagem ) {
	if ( ! imagem )
		return NULL;
	// só aceita imagens P&B
	if ( imagem->GetFormato() != D1_X_Y_Z_ASCII && imagem->GetFormato() != D4_X_Y_Z_BINARY)
		return NULL;
	
	TCMatriz3D<int> * pi3Dinv = NULL;
	pi3Dinv = new TCMatriz3D<int>( *imagem);
	if ( ! pi3Dinv )
		return NULL;
	
	// percorre a imagem invertendo os valores.
	for ( int i = 0; i < pi3Dinv->NX(); i++ )
		for ( int j = 0; j < pi3Dinv->NY(); j++ )
			for ( int k = 0; k < pi3Dinv->NZ(); k++ )
				( pi3Dinv->data3D[i][j][k] == 0 ) ? pi3Dinv->data3D[i][j][k] = 1 : pi3Dinv->data3D[i][j][k] = 0;
	return pi3Dinv;
}
