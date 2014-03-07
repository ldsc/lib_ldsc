#include <AnaliseImagem/Filtro/FEspacial3D/FEConectividade3D/CFEConectividade3D.h>

CImagem3D * CFEConectividade3D::GetImagemConectada (int indice, int fundo) {
    int INDICE = indice;
    FUNDO = fundo;				//seta o valor de fundo. (declarado em CRotulador.h)
    int i, j, k;

    if ( ! rotulado )			//verifica se a imagem já foi rotulada
        CRotulador3D::Go (pmOrig);			//se ainda não rotulada, rotula...

    //this->Write("testeConectividade.dbm");
    CImagem3D * pmCon = NULL;
    pmCon = new CImagem3D(pmOrig->NX(), pmOrig->NY(), pmOrig->NZ());

    if ( ! pmCon ) return NULL; 	// se não conseguiu alocar a matriz 3D retorna NULL

    // Obter os objetos do primeiro plano da imagem rotulada (this).
    int planoy = 0;	//plano y corrente (será o primeiro ou o último plano y da imagem 3D).
    set<int> plano0;	//estrutura de dados não duplicados para armazenar os objetos do primeiro plano;
    for ( i = 0; i < this->NX(); i++ )
        for ( k = 0; k < this->NZ(); k++ )
            if (this->data3D[i][planoy][k] != FUNDO)
                plano0.insert( plano0.begin(), this->data3D[i][planoy][k]);

    // Obter os objetos do último plano da imagem rotulada (this).
    planoy = ( this->NY() - 1 );
    set<int> ultimoPlano;	//estrutura de dados não duplicados para armazenar os objetos do último plano;

    for ( i = 0; i < this->NX(); i++ )
        for ( k = 0; k < this->NZ(); k++ )
            if (this->data3D[i][planoy][k] != FUNDO)
                ultimoPlano.insert(ultimoPlano.begin(), this->data3D[i][planoy][k]);

    set<int>::iterator it0;
    set<int>::iterator it;

    for ( it0 = plano0.begin(); it0 != plano0.end(); ++it0 ) {
        it = ultimoPlano.find( *it0 );
        if ( it == ultimoPlano.end() )
            plano0.erase( it0 ); // Busca no ultimoPlano o valor contido em plano0[it0]. Se encontrar apaga...
    } // A partir daqui, plano0 contém somente os objetos conectados.

    if ( plano0.empty() ) { // se não existir objetos em plano0 é porque a imagem não possui conectividade.
        delete pmCon;
        return NULL;
    }

    // percorre a matriz rotulada (this).
    for ( i = 0; i < this->NX(); i++ )
        for ( j = 0; j < this->NY(); j++ )
            for ( k = 0; k < this->NZ(); k++ ) {
                it0 = plano0.find( this->data3D[i][j][k] );
                if ( it0 != plano0.end() ) 			// O pixel analizado pentence a um objeto conectado.
                    pmCon->data3D[i][j][k] = INDICE;	// Marca o pixel com valor de índice
                else
                    pmCon->data3D[i][j][k] = FUNDO;
            }
    return pmCon;
}

/// Recebe imagem 3D e retorna imagem 3D conectada.
// Por padrão fundo == 0.
// Depois adicionar a Direção Gox Goy Goz
// retiramos nova alocação, percorre matriz list.size()
bool CFEConectividade3D::Go (CMatriz3D * imagem, int indice, int fundo /*,char direcao = 'y' */)
{
    int INDICE = indice;
    FUNDO = fundo;			//seta o valor de fundo. (declarado em CRotulador.h)

    CRotulador3D::Go ( imagem );			//se ainda não rotulada, rotula...

    // Obter os objetos do primeiro plano da imagem rotulada (this).
    int planoy = 0;	//plano y corrente (será o primeiro ou o último plano y da imagem 3D).
    set<int> plano0;	//estrutura de dados não duplicados para armazenar os objetos do primeiro plano;
    for ( int i = 0; i < this->NX(); i++ )
        for ( int k = 0; k < this->NZ(); k++ )
            if (this->data3D[i][planoy][k] != FUNDO)
                plano0.insert( plano0.begin(), this->data3D[i][planoy][k] );

    // Obter os objetos do último plano da imagem rotulada (this).
    planoy = ( this->NY() - 1 );
    set<int> ultimoPlano;	//estrutura de dados não duplicados para armazenar os objetos do último plano;
    for ( int i = 0; i < this->NX(); i++ )
        for ( int k = 0; k < this->NZ(); k++ )
            if (this->data3D[i][planoy][k] != FUNDO)
                ultimoPlano.insert( ultimoPlano.begin(), this->data3D[i][planoy][k] );

    set<int>::iterator it0;
    set<int>::iterator it;

    // Busca no ultimoPlano o valor contido em plano0[it0]. Se encontrar apaga...
    for ( it0 = plano0.begin(); it0 != plano0.end(); ++it0 ) {
        it = ultimoPlano.find( *it0 );
        if ( it == ultimoPlano.end() )
            plano0.erase( it0 );
    }
    // A partir daqui, plano0 contém somente os objetos conectados.

    // se não existir objetos em plano0 é porque a imagem não possui conectividade.
    if ( plano0.empty() ) {
        return 0;
    }

    // percorre a matriz rotulada (this).
    int marcador;
    for (it0 = plano0.begin(); it0 != plano0.end(); ++it0)
    {
        marcador = *it0;
        for ( int i = 0; i < this->NX(); i++ )
            for ( int j = 0; j < this->NY(); j++ )
                for ( int k = 0; k < this->NZ(); k++ ) {
                    if ( marcador == this->data3D[i][j][k] )		// O pixel analizado pentence a um objeto conectado.
                        imagem->data3D[i][j][k] = INDICE;	// Marca o pixel com valor de índice
                    else
                        imagem->data3D[i][j][k] = FUNDO;
                }
    }

    return 1;
}
