#include <AnaliseImagem/Filtro/FEspacial/FEInversao/CFEInversao.h>

CImagem * CFEInversao::Go ( CImagem * imagem ) {
	// a imagem deve ser passada no construtor.
	if ( ! imagem )
		return nullptr;
	// só aceita imagens P&B
	if ( imagem->GetFormato() != P1_X_Y_ASCII && imagem->GetFormato() != P4_X_Y_BINARY)
		return nullptr;
	CImagem * piInv = nullptr;
	piInv = new CImagem( *imagem );
	if ( ! piInv )
		return nullptr;

	// percorre a imagem invertendo os valores.
	for ( int i = 0; i < piInv->NX(); i++ )
		for ( int j = 0; j < piInv->NY(); j++ )
			( piInv->data2D[i][j] == 0 ) ? piInv->data2D[i][j] = 1 : piInv->data2D[i][j] = 0;
	return piInv;
}

TCMatriz2D<bool> * CFEInversao::Go ( TCMatriz2D<bool> * imagem ) {
	// a imagem deve ser passada no construtor.
	if ( ! imagem )
		return nullptr;
	// só aceita imagens P&B
	if ( imagem->GetFormato() != P1_X_Y_ASCII && imagem->GetFormato() != P4_X_Y_BINARY)
		return nullptr;
	
	TCMatriz2D<bool> * pmInv = nullptr;
	pmInv = new TCMatriz2D<bool>( *imagem );
	if ( ! pmInv )
		return nullptr;
	
	// percorre a imagem invertendo os valores.
	for ( int i = 0; i < pmInv->NX(); i++ )
		for ( int j = 0; j < pmInv->NY(); j++ )
			( pmInv->data2D[i][j] == 0 ) ? pmInv->data2D[i][j] = 1 : pmInv->data2D[i][j] = 0;
	return pmInv;
}
