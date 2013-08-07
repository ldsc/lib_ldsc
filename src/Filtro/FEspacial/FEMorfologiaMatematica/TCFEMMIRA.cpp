/*
----------------------------------------------------------------------------
PROJETO:		LIBLDSC
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
Programadores:    Leandro Puerari
Copyright @2013:  Todos os direitos reservados.
Nome deste arquivo:	TCFEMMIRA.cpp
Nome da classe:     TCFEMMIRA
*/

using namespace std;
#ifndef TCFEMMIRA_H
#include "Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIRA.h"
#endif

#include "Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd34.h"

// Construtor
template<typename T>
TCFEMMIRA<T>::TCFEMMIRA ( TCMatriz2D<T> * &matriz, int _indice, int _fundo )
	: TCFEMorfologiaMatematica<T> ( matriz, 0, _indice, _fundo ), pmra(NULL) {
}

template<typename T>
TCMatriz2D<int> * TCFEMMIRA<T>::Go () {
	pmra = new TCMatriz2D<int> ( this->pm->NX(), this->pm->NY() );

	// Cria Imagem de Distância ao Fundo (IDF)
	TCFEMMIDFd34<T> * idf = new TCFEMMIDFd34<T> (this->pm, this->INDICE, this->FUNDO);

	// Determina a IDF
	idf->Go(this->pm);

	int i, j;
	int nx = idf->NX();
	int ny = idf->NY();
	int df = 0; //distancia ao fundo
	//percorre a imagem
	for (i=0; i<nx; i++) {
		for (j=0; j<ny; j++) {
			if (idf->data2D[i][j] > 0) { //só interessam pixeis que não correspondem a fundo (matriz sólida)
				df = idf->data2D[i][j];
				if (df == 3) {
					//percorre mascara de raio 1 para comparar as visinhanças
					pmra->data2D[i][j] = 1;
				} else {
						//tratamento para df > 3
				}
			}
		}
	}
	delete idf;
	return pmra;
}
