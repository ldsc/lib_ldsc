#include <Filtro/FEspacial3D/FEInversao3D/CFEInversao3D.h>

CImagem3D * CFEInversao3D::Go ( CImagem3D * imagem ) {
	if ( ! imagem ) return NULL;
	// só aceita imagens ASCII P&B
	if ( imagem->GetFormat() != WRITEFORM_DI_X_Y_Z_ASCII ) return NULL;
	
	CImagem3D * pi3Dinv = NULL;
	pi3Dinv = new CImagem3D( *imagem);
	if ( ! pi3Dinv ) return NULL;
	
	// percorre a imagem invertendo os valores.
	for ( int i = 0; i < pi3Dinv->NX(); i++ )
		for ( int j = 0; j < pi3Dinv->NY(); j++ )
			for ( int k = 0; k < pi3Dinv->NZ(); k++ )
				( pi3Dinv->data3D[i][j][k] == 0 ) ? pi3Dinv->data3D[i][j][k] = 1 : pi3Dinv->data3D[i][j][k] = 0;
	return pi3Dinv;
}

CMatriz3D * CFEInversao3D::Go ( CMatriz3D * imagem ) {
	if ( ! imagem ) return NULL;
	// só aceita imagens ASCII P&B
	if ( imagem->GetFormat() != WRITEFORM_DI_X_Y_Z_ASCII ) return NULL;
	
	CMatriz3D * pi3Dinv = NULL;
	pi3Dinv = new CMatriz3D( *imagem);
	if ( ! pi3Dinv ) return NULL;
	
	// percorre a imagem invertendo os valores.
	for ( int i = 0; i < pi3Dinv->NX(); i++ )
		for ( int j = 0; j < pi3Dinv->NY(); j++ )
			for ( int k = 0; k < pi3Dinv->NZ(); k++ )
				( pi3Dinv->data3D[i][j][k] == 0 ) ? pi3Dinv->data3D[i][j][k] = 1 : pi3Dinv->data3D[i][j][k] = 0;
	return pi3Dinv;
}
