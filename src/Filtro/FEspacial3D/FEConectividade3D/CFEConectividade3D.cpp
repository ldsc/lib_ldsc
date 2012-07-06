#include <Filtro/FEspacial3D/FEConectividade3D/CFEConectividade3D.h>
// retorna ponteiro para imagem conectada ou NULL caso a imagem não possua conectividade ou ocorra algum erro.

// Retorna a lista de objetos conectados.
set<int> CFEConectividade3D::GetObjetosConectados ( CImagem3D * pmOrig ){
	return GetObjetosConectados( dynamic_cast<CMatriz3D*>(pmOrig) );
}

// Retorna a lista de objetos conectados.
set<int> CFEConectividade3D::GetObjetosConectados ( CMatriz3D * pmOrig){
   //	if ( ! rotulado )			// verifica se a imagem já foi rotulada
   CRotulador3D::Go ( pmOrig ); // se ainda não rotulada, rotula...
   // Obter os objetos do primeiro plano da imagem rotulada (this).
	int planoy = 0;  // plano y corrente (será o primeiro ou o último plano y da imagem 3D).
	set<int> plano0; // estrutura de dados não duplicados para armazenar os objetos do primeiro plano;
	for (int  i = 0; i < this->NX(); i++ )
		for ( int k = 0; k < this->NZ(); k++ )
         if (this->data3D[i][planoy][k] != FUNDO)
				plano0.insert( plano0.begin(), this->data3D[i][planoy][k]);

	// Obter os objetos do último plano da imagem rotulada (this).
	planoy = ( this->NY() - 1 );
	set<int> ultimoPlano;			// estrutura de dados não duplicados para armazenar os objetos do último plano;
	for ( int i = 0; i < this->NX(); i++ )
		for ( int k = 0; k < this->NZ(); k++ )
         if (this->data3D[i][planoy][k] != FUNDO)
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



bool CFEConectividade3D::isConnected ( CImagem3D * pmOrig) {
	return isConnected( dynamic_cast<CMatriz3D*>(pmOrig) );
}

bool CFEConectividade3D::isConnected ( CMatriz3D * pmOrig) {
	if ( GetObjetosConectados(pmOrig).empty() ) // se não existir objetos em plano0 é porque a imagem não possui conectividade.
		return false;
	else
		return true;
}

// Recebe ponteiro para imagem 3D e retorna uma nova imagem 3D conectada. Se a imgem não for conectada, retorna false (0).
CImagem3D * CFEConectividade3D::GetImagemConectada ( CImagem3D * pmOrig) {
   if ( ! isConnected(pmOrig) ){
      cerr << "Não é conectada em Y" << endl;
      return NULL;
   }

   CImagem3D * pmCon = NULL;
   pmCon = new CImagem3D( * pmOrig );
   if ( ! pmCon ) {
      cerr << "Erro! - CFEConectividade3D::GetImagemConectada (int, int) não conseguiu criar CImagem3D" << endl;
      return NULL; 		// se não conseguiu alocar a matriz 3D retorna NULL
   }

   if ( Go( pmCon ) )
      return pmCon;
   else {
      cerr << "Go return false em CImagem3D * CFEConectividade3D::GetImagemConectada ( CImagem3D * pmOrig)" << endl;
      return NULL;
   }
   /*
 set<int> conectados = GetObjetosConectados( pmOrig );

 if ( conectados.empty() ) { // se não existir objetos em plano0 é porque a imagem não possui conectividade.
  return NULL;
 }

 CImagem3D * pmCon = NULL;
   pmCon = new CImagem3D(pmOrig->NX(), pmOrig->NY(), pmOrig->NZ());
 if ( ! pmCon ) {
  cerr << "Erro! - CFEConectividade3D::GetImagemConectada (int) não conseguiu criar CImagem3D" << endl;
  return NULL; 		// se não conseguiu alocar a matriz 3D retorna NULL
 }
 pmCon->WriteFormat(pmOrig->GetFormat());
 pmCon->NumCores(pmOrig->NumCores());
 pmCon->Path(pmOrig->Path());
 pmCon->fatorAmplificacao = pmOrig->FatorAmplificacao();
 pmCon->numeroPixelsBorda = pmOrig->NumeroPixelsBorda();
 pmCon->sizePixel = pmOrig->SizePixel();

 set<int>::iterator it0;
 // percorre a matriz rotulada (this).
 for ( int i = 0; i < this->NX(); i++ )
  for ( int j = 0; j < this->NY(); j++ )
   for ( int k = 0; k < this->NZ(); k++ ) {
    it0 = conectados.find( this->data3D[i][j][k] );
            if ( it0 != conectados.end() ) // Se o pixel analizado pentencer a um objeto conectado.
     pmCon->data3D[i][j][k] = INDICE;	// Marca o pixel com valor de índice
    else
     pmCon->data3D[i][j][k] = FUNDO;
   }
   return pmCon;
*/
}

CMatriz3D * CFEConectividade3D::GetMatrizConectada ( CMatriz3D * pmOrig) {
   if ( ! isConnected(pmOrig) )
      return NULL;

   CMatriz3D * pmCon = NULL;
   pmCon = new CMatriz3D( * pmOrig );
   if ( ! pmCon ) {
      cerr << "Erro! - CFEConectividade3D::GetImagemConectada (int, int) não conseguiu criar CImagem3D" << endl;
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

 CMatriz3D * pmCon = NULL;
   pmCon = new CMatriz3D(pmOrig->NX(), pmOrig->NY(), pmOrig->NZ());
   if ( ! pmCon ) {
  cerr << "Erro! - CFEConectividade3D::GetImagemConectada (int, int) não conseguiu criar CImagem3D" << endl;
  return NULL; 		// se não conseguiu alocar a matriz 3D retorna NULL
 }
 set<int>::iterator it0;
 // percorre a matriz rotulada (this).
 for ( int i = 0; i < this->NX(); i++ )
  for ( int j = 0; j < this->NY(); j++ )
   for ( int k = 0; k < this->NZ(); k++ ) {
    it0 = plano0.find( this->data3D[i][j][k] );
            if ( it0 != plano0.end() ) // Se o pixel analizado pentencer a um objeto conectado.
     pmCon->data3D[i][j][k] = INDICE;	// Marca o pixel com valor de índice
    else
     pmCon->data3D[i][j][k] = FUNDO;
   }

   return pmCon;
   */
}


// Recebe ponteiro para imagem 3D que será alterada para imagem 3D conectada. Se a imgem não for conectada, retorna false (0).
bool CFEConectividade3D::Go ( CMatriz3D* pmCon ) {
	if ( ! pmCon ) {
		cerr << "Erro! - CFEConectividade3D::Go (CMatriz3D*) recebeu objeto nulo" << endl;
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
					pmCon->data3D[i][j][k] = INDICE;	// Marca o pixel com valor de índice
				else
					pmCon->data3D[i][j][k] = FUNDO;
			}
	return true;
}

// Recebe ponteiro para imagem 3D que será alterada para imagem 3D conectada. Se a imgem não for conectada, retorna false (0).
bool CFEConectividade3D::Go ( CImagem3D* pmCon ) {
	if ( ! pmCon ) {
		cerr << "Erro! - CFEConectividade3D::Go (CMatriz3D*) recebeu objeto nulo" << endl;
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
					pmCon->data3D[i][j][k] = INDICE;	// Marca o pixel com valor de índice
				else
					pmCon->data3D[i][j][k] = FUNDO;
			}
	return true;
}
