/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
===============================================================================
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:          Leandro Puerari
File:             TCRotuladorIRA3D.cpp
email:            puerari@gmail.com
*/

#ifndef TCRotuladorIRA3D_h
#include <Rotulador/TCRotuladorIRA3D.h>
#endif

/* Executa toda a sequencia de rotulagem considerando o raio para abertura (ra) informado.*/
// bool TCRotuladorIRA3D::Go(TCMatriz3D<int> *& matriz, int _rotuloInicial)
template<typename T>
bool TCRotuladorIRA3D<T>::Go (TCMatriz3D<T> *matriz, int _ra) {
	ra = _ra;
	return TCRotulador3D<T>::Go(matriz);
}

/*
-------------------------------------------------------------------------
Função: PreparaImagem
-------------------------------------------------------------------------
@short  :
 Funcao chamada por Go, para verificar a imagem passada.
	 Ou seja antes de inicializar o procedimento de rotulagem,
 faz algumas verificações com a imagem.
	 Armazena o ponteiro para a imagem e verifica se a imagem tem as mesmas dimensões
	 caso contrário, realoza a imagem.
@author :André Duarte Bueno
@see    :rotulagem
@param  :CMatriz3D
@return :bool
*/
template<typename T>
bool TCRotuladorIRA3D<T>::PreparaImagem (TCMatriz3D<T> *matriz) {
	this->pm = matriz;			// Armazena endereço matriz
	// Verifica se a matriz tem as mesmas dimensoes do rotulador
	if (this->pm == NULL)
		return false;

	// Se o objeto rotulador3D foi criado sem passar a matriz 3D, ou nx,ny,nz
	// a matriz3D definida por data3D não foi criada, abaixo já Aloca a matriz3D
	if (this->nx != this->pm->NX () || this->ny != this->pm->NY () || this->nz != this->pm->NZ ()) {
		this->Desaloca ();		// Desaloca a matriz existende e depois
		this->nx = this->pm->NX ();	// Define novo tamanho
		this->ny = this->pm->NY ();
		this->nz = this->pm->NZ ();
		this->Aloca ();			// Aloca de acordo com novo tamanho
	}


	// Seta a matriz this de acordo com a imagem original
	int i, j, k;
	if (ra >= 0) {
		cerr << "ra= " << ra << endl;
		#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
		for ( k = 0; k < this->nz; k++ )
			for ( j = 0; j < this->ny; j++ )
				for ( i = 0; i < this->nx; i++ )
					if (this->pm->data3D[i][j][k] > ra)
						this->data3D[i][j][k] = 1;
					else // Define this com 0 e 1
						this->data3D[i][j][k] = 0;
	} else { // ra negativo - irá considerar o complemento da abertura.
		int rat = -1 * ra; //multiplica por -1 para ficar positivo
		#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
		for ( k = 0; k < this->nz; k++ )
			for ( j = 0; j < this->ny; j++ )
				for ( i = 0; i < this->nx; i++ )
					if (this->pm->data3D[i][j][k] > 0 && this->pm->data3D[i][j][k] <= rat)
						this->data3D[i][j][k] = 1;	// Define this com 0 e 1
					else
						this->data3D[i][j][k] = 0;
	}

	// NOVO Versão 7
	// Como pode chamar Go mais de uma vez, preciso apagar os vetores area e perimetro
	// que foram alocados na chamada anterior a Go
	if (this->areaObjetos)
		delete this->areaObjetos;
	this->areaObjetos = NULL;

	if (this->perimetroObjetos)
		delete this->perimetroObjetos;
	this->perimetroObjetos = NULL;

	this->SetFormato(D1_X_Y_Z_ASCII);
	this->Write("PreparaImagem.dbm");
	return true;
}
