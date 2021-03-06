#ifndef TCFEConectividade3D_h
#include <AnaliseImagem/Filtro/FEspacial/FEConectividade/TCFEConectividade3D.h>
#endif
// retorna ponteiro para imagem conectada ou NULL caso a imagem não possua conectividade ou ocorra algum erro.

// Retorna a lista de objetos conectados.
template<typename T>
set<int> TCFEConectividade3D<T>::GetObjetosConectados ( TCImagem3D<T> *pmOrig ){
	return GetObjetosConectados( dynamic_cast<TCMatriz3D<T> *>(pmOrig) );
}

// Retorna a lista de objetos conectados.
template<typename T>
set<int> TCFEConectividade3D<T>::GetObjetosConectados ( TCMatriz3D<T> *pmOrig){
	//	if ( ! rotulado )			// verifica se a imagem já foi rotulada
	TCRotulador3D<T>::Go ( pmOrig ); // se ainda não rotulada, rotula...
	// Obter os objetos do primeiro plano da imagem rotulada (this).
	int planoy = 0;  // plano y corrente (será o primeiro ou o último plano y da imagem 3D).
	set<int> plano0; // estrutura de dados não duplicados para armazenar os objetos do primeiro plano;
	for (int  i = 0; i < this->NX(); i++ )
		for ( int k = 0; k < this->NZ(); k++ )
			if (this->data3D[i][planoy][k] != this->FUNDO)
				plano0.insert( plano0.begin(), this->data3D[i][planoy][k]);

	// Obter os objetos do último plano da imagem rotulada (this).
	planoy = ( this->NY() - 1 );
	set<int> ultimoPlano;			// estrutura de dados não duplicados para armazenar os objetos do último plano;
	for ( int i = 0; i < this->NX(); i++ )
		for ( int k = 0; k < this->NZ(); k++ )
			if (this->data3D[i][planoy][k] != this->FUNDO)
				ultimoPlano.insert(ultimoPlano.begin(), this->data3D[i][planoy][k]);

	set<int>::iterator it0;
	set<int>::iterator it;

	for ( it0 = plano0.begin(); it0 != plano0.end(); ++it0 )	{
		it = ultimoPlano.find( *it0 ); // Busca no ultimoPlano o valor contido em plano0[it0].
		if ( it == ultimoPlano.end() ) // Se não encontrar apaga plano0[it0].
			plano0.erase( it0 );
	} // A partir daqui, plano0 contém somente os objetos conectados.

	return plano0;
}

template<typename T>
bool TCFEConectividade3D<T>::isConnected ( TCImagem3D<T> *pmOrig) {
	return isConnected( dynamic_cast<TCMatriz3D<T> *>(pmOrig) );
}

template<typename T>
bool TCFEConectividade3D<T>::isConnected ( TCMatriz3D<T> *pmOrig) {
	if ( GetObjetosConectados(pmOrig).empty() ) // se não existir objetos em plano0 é porque a imagem não possui conectividade.
		return false;
	else
		return true;
}

// Recebe ponteiro para imagem 3D e retorna uma nova imagem 3D conectada. Se a imgem não for conectada, retorna false (0).
template<typename T>
TCImagem3D<T> *TCFEConectividade3D<T>::GetImagemConectada( TCImagem3D<T> *pmOrig) {
	if ( ! isConnected(pmOrig) ){
		cerr << "Não é conectada em Y" << endl;
		return NULL;
	}

	TCImagem3D<T> * pmCon = NULL;
	pmCon = new TCImagem3D<T>( * pmOrig );
	if ( ! pmCon ) {
		cerr << "Erro! - TCFEConectividade3D::GetImagemConectada (int, int) não conseguiu criar TCImagem3D" << endl;
		return NULL; 		// se não conseguiu alocar a matriz 3D retorna NULL
	}

	if ( Go( pmCon ) )
		return pmCon;
	else {
		cerr << "Go return false em TCImagem3D * TCFEConectividade3D::GetImagemConectada ( TCImagem3D * pmOrig)" << endl;
		return NULL;
	}
	/*
 set<int> conectados = GetObjetosConectados( pmOrig );

 if ( conectados.empty() ) { // se não existir objetos em plano0 é porque a imagem não possui conectividade.
	return NULL;
 }

 TCImagem3D * pmCon = NULL;
	 pmCon = new TCImagem3D(pmOrig->NX(), pmOrig->NY(), pmOrig->NZ());
 if ( ! pmCon ) {
	cerr << "Erro! - TCFEConectividade3D::GetImagemConectada (int) não conseguiu criar TCImagem3D" << endl;
	return NULL; 		// se não conseguiu alocar a matriz 3D retorna NULL
 }
 pmCon->SetFormato(pmOrig->GetFormato());
 pmCon->NumCores(pmOrig->NumCores());
 pmCon->Path(pmOrig->Path());
 pmCon->fatorAmplificacao = pmOrig->FatorAmplificacao();
 pmCon->numeroPixeisBorda = pmOrig->NumeroPixelsBorda();
 pmCon->dimensaoPixel = pmOrig->DimensaoPixel();

 set<int>::iterator it0;
 // percorre a matriz rotulada (this).
 for ( int i = 0; i < this->NX(); i++ )
	for ( int j = 0; j < this->NY(); j++ )
	 for ( int k = 0; k < this->NZ(); k++ ) {
		it0 = conectados.find( this->data3D[i][j][k] );
						if ( it0 != conectados.end() ) // Se o pixel analizado pentencer a um objeto conectado.
		 pmCon->data3D[i][j][k] = this->INDICE;	// Marca o pixel com valor de índice
		else
		 pmCon->data3D[i][j][k] = this->FUNDO;
	 }
	 return pmCon;
*/
}

template<typename T>
TCMatriz3D<T> *TCFEConectividade3D<T>::GetMatrizConectada( TCMatriz3D<T> *pmOrig) {
	if ( ! isConnected(pmOrig) )
		return NULL;

	TCMatriz3D<T> * pmCon = NULL;
	pmCon = new TCMatriz3D<T>( * pmOrig );
	if ( ! pmCon ) {
		cerr << "Erro! - TCFEConectividade3D::GetImagemConectada (int, int) não conseguiu criar TCImagem3D" << endl;
		return NULL; 		// se não conseguiu alocar a matriz 3D retorna NULL
	}

	if ( Go( pmCon ) )
		return pmCon;
	else
		return NULL;

	/*
	 set<int> plano0 = GetObjetosConectados( pmOrig );

 if ( plano0.empty() ) { // se não existir objetos em plano0 é porque a imagem não possui conectividade.
	return NULL;
 }

 TCMatriz3D<int> * pmCon = NULL;
	 pmCon = new CMatriz3D(pmOrig->NX(), pmOrig->NY(), pmOrig->NZ());
	 if ( ! pmCon ) {
	cerr << "Erro! - TCFEConectividade3D::GetImagemConectada (int, int) não conseguiu criar TCImagem3D" << endl;
	return NULL; 		// se não conseguiu alocar a matriz 3D retorna NULL
 }
 set<int>::iterator it0;
 // percorre a matriz rotulada (this).
 for ( int i = 0; i < this->NX(); i++ )
	for ( int j = 0; j < this->NY(); j++ )
	 for ( int k = 0; k < this->NZ(); k++ ) {
		it0 = plano0.find( this->data3D[i][j][k] );
						if ( it0 != plano0.end() ) // Se o pixel analizado pentencer a um objeto conectado.
		 pmCon->data3D[i][j][k] = this->INDICE;	// Marca o pixel com valor de índice
		else
		 pmCon->data3D[i][j][k] = this->FUNDO;
	 }

	 return pmCon;
	 */
}


// Recebe ponteiro para imagem 3D que será alterada para imagem 3D conectada. Se a imgem não for conectada, retorna false (0).
template<typename T>
bool TCFEConectividade3D<T>::Go ( TCMatriz3D<T> *pmCon ) {
	if ( ! pmCon ) {
		cerr << "Erro! - TCFEConectividade3D::Go (TCMatriz3D<int> *) recebeu objeto nulo" << endl;
		return false; 		// se não conseguiu alocar a matriz 3D retorna NULL
	}

	set<int> plano0 = GetObjetosConectados( pmCon );

	if ( plano0.empty() ) { // se não existir objetos em plano0 é porque a imagem não possui conectividade.
		return false;
	}
	set<int>::iterator it0;
	// percorre a matriz rotulada (this).
	for ( int i = 0; i < this->NX(); i++ )
		for ( int j = 0; j < this->NY(); j++ )
			for ( int k = 0; k < this->NZ(); k++ ) {
				it0 = plano0.find( this->data3D[i][j][k] );
				if ( it0 != plano0.end() ) 			// O pixel analizado pentence a um objeto conectado.
					pmCon->data3D[i][j][k] = this->INDICE;	// Marca o pixel com valor de índice
				else
					pmCon->data3D[i][j][k] = this->FUNDO;
			}
	return true;
}

// Recebe ponteiro para imagem 3D que será alterada para imagem 3D conectada. Se a imgem não for conectada, retorna false (0).
template<typename T>
bool TCFEConectividade3D<T>::Go ( TCImagem3D<T> *pmCon ) {
	if ( ! pmCon ) {
		cerr << "Erro! - TCFEConectividade3D::Go (TCMatriz3D<int> *) recebeu objeto nulo" << endl;
		return false; 		// se não conseguiu alocar a matriz 3D retorna NULL
	}

	set<int> plano0 = GetObjetosConectados( pmCon );

	if ( plano0.empty() ) { // se não existir objetos em plano0 é porque a imagem não possui conectividade.
		return false;
	}
	set<int>::iterator it0;
	// percorre a matriz rotulada (this).
	for ( int i = 0; i < this->NX(); i++ )
		for ( int j = 0; j < this->NY(); j++ )
			for ( int k = 0; k < this->NZ(); k++ ) {
				it0 = plano0.find( this->data3D[i][j][k] );
				if ( it0 != plano0.end() ) 			// O pixel analizado pentence a um objeto conectado.
					pmCon->data3D[i][j][k] = this->INDICE;	// Marca o pixel com valor de índice
				else
					pmCon->data3D[i][j][k] = this->FUNDO;
			}
	return true;
}
