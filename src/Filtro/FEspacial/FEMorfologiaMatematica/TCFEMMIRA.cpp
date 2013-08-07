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
#include "Geometria/Bola/BCDiscreta/CBCd34.h"

#include <map>

// Construtor
template<typename T>
TCFEMMIRA<T>::TCFEMMIRA ( TCMatriz2D<T> * &matriz, int _indice, int _fundo )
	: TCFEMorfologiaMatematica<T> ( matriz, 0, _indice, _fundo ), pmra(NULL) {
}

template<typename T>
TCMatriz2D<int> * TCFEMMIRA<T>::Go () {
	// Cria imagem 2D que representará a imagem Raio Abertura
	pmra = new TCMatriz2D<int> ( this->pm->NX(), this->pm->NY() );

	// Cria vetor que irá armazenar as mascaras.
	std::map<int, CBCd34* > vmask;
	std::map<int, CBCd34* >::iterator it;

	// Cria Imagem de Distância ao Fundo (IDF)
	TCFEMMIDFd34<T> * idf = new TCFEMMIDFd34<T> (this->pm, this->INDICE, this->FUNDO);

	// Determina a IDF
	idf->Go(this->pm);

	int i, j, x, y, i0, j0, in, jn, ipx, imx, jpy, jmy, cont;
	int nx = idf->NX();
	int ny = idf->NY();
	int nxm, nym;
	int df = 0; //distancia ao fundo
	bool atendeu;
	//percorre a imagem
	for (i=0; i<nx; i++) {
		for (j=0; j<ny; j++) {
			if (idf->data2D[i][j] > 0) { //só interessam pixeis que não correspondem a fundo (matriz sólida)
				df = idf->data2D[i][j];
				if (df == 3) {
					// df == 3, logo, o valor de RA == 1 se o pixel analizado:
					// Não tiver vizinho > 3;
					// Não tiver mais de 2 vizinhos com df == 3 (visinhança definida pela máscara);
					// Pode ter 2 visinhos trasversais iguais a 3.
					imx = ( i-1 <=  0) ?  0   : i-1;
					ipx = ( i+1 >= nx) ? nx-1 : i+1;
					jmy = ( j-1 <=  0) ?  0   : j-1;
					jpy = ( j+1 >= ny) ? ny-1 : j+1;
					atendeu = true;
					// Verificando vizinhança com df > 3...
					if ( idf->data2D[imx][ j ] > 3 ||
							 idf->data2D[ipx][ j ] > 3 ||
							 idf->data2D[ i ][jmy] > 3 ||
							 idf->data2D[ i ][jpy] > 3 ) {
						atendeu = false;
					} else {
						/*
						// Verificando número de vizinhos com df==3...
						cont = 0;
						cont += (idf->data2D[imx][ j ] == 3) ? 1 : 0;
						cont += (idf->data2D[ipx][ j ] == 3) ? 1 : 0;
						cont += (idf->data2D[ i ][jmy] == 3) ? 1 : 0;
						cont += (idf->data2D[ i ][jpy] == 3) ? 1 : 0;
						if ( cont >= 3 ) {
							atendeu = false;
						} else if ( cont == 2 ) {
						*/
						// Verifica se os 2 vizinhos com df==3 são paralelos
						if ( idf->data2D[imx][j] == 3 && idf->data2D[i][jmy] == 3 && idf->data2D[imx][jmy] > 3 ) {
							atendeu = false;
						} else if ( idf->data2D[imx][j] == 3 && idf->data2D[i][jpy] == 3 && idf->data2D[imx][jpy] > 3 ) {
							atendeu = false;
						} else if ( idf->data2D[ipx][j] == 3 && idf->data2D[i][jmy] == 3 && idf->data2D[ipx][jmy] > 3 ) {
							atendeu = false;
						} else if ( idf->data2D[ipx][j] == 3 && idf->data2D[i][jpy] == 3 && idf->data2D[ipx][jpy] > 3 ) {
							atendeu = false;
						}
						/* } */
					}
					if ( atendeu ) {
						pmra->data2D[i][j] = 1;
					} else {
						pmra->data2D[i][j] = 2;
					}
				} else { //tratamento para df > 3
					pmra->data2D[i][j] = 2;
					/*
					// df == 3, logo, o valor de RA == 1 se o pixel analizado:
					// Não tiver vizinho > 3;
					// Não tiver mais de 2 vizinhos com df == 3 (visinhança definida pela máscara);
					// Pode ter 2 visinhos trasversais iguais a 3.
					it = vmask.find(1); // Verifica se existe mascara de raio 1. Se não existir cria...
					if( it == vmask.end() ) { // A mascara ainda não foi criada
						vmask[1] = new CBCd34(3);
						it = vmask.find(1);
					}
					mask = it->second;
					nxm = (mask->NX()-1)/2;
					nym = (mask->NY()-1)/2;
					//percorre mascara de raio 1 para comparar as visinhanças (considera simetria)
					for (x=0; x<=nxm; x++) {
						imx = ((i-nxm+x) <=  0) ?  0   : i-nxm+x;
						ipx = ((i+nxm-x) >= nx) ? nx-1 : i+nxm-x;
						for (y=0; y<=nym; y++) {
							if ( mask->data2D[x][y] != 0 ) {
								jmy = ((j-nym+y) <=  0) ?  0   : j-nym+y;
								jpy = ((j+nym-y) >= ny) ? ny-1 : j+nym-y;
								if ( idf->data2D[imx][] < 3) {

								}
							}
						}
					}
					*/
				}
			}
		}
	}

	//
	for (it=vmask.begin(); it!=vmask.end(); ++it) {
		delete it->second;
	}
	delete idf;
	return pmra;
}
