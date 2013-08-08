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
	// Cria vetor que irá armazenar as mascaras.
	std::map<int, CBCd34* > vmask;
	std::map<int, CBCd34* >::iterator it;

	// Cria Imagem de Distância ao Fundo (IDF)
	TCFEMMIDFd34<T> * idf = new TCFEMMIDFd34<T> (this->pm, this->INDICE, this->FUNDO);

	// Determina a IDF
	idf->Go(this->pm);

	// Cria imagem 2D que representará a imagem Raio Abertura (cópia da IDF)
	//pmra = new TCMatriz2D<int> ( this->pm->NX(), this->pm->NY() );
	pmra = new TCMatriz2D<int> ( * dynamic_cast< TCMatriz2D<int> *>(idf) );

	int i, j, x, y, ip, im, jp, jm;//, i0, j0, in, jn;
	int dft, ra;//, cont;
	int nx = idf->NX();
	int ny = idf->NY();
	int rm; // raio da mascara;
	int df = 0; //distancia ao fundo
	bool atendeu;
	//percorre a imagem
	for (i=0; i<nx; i++) {
		for (j=0; j<ny; j++) {
			if (idf->data2D[i][j] > 0) { //só interessam pixeis que não correspondem a fundo (matriz sólida)
				df = idf->data2D[i][j];
				if (df == 3) {
					// df == 3, logo, o valor de RA será 1 se o pixel analizado:
					// Não tiver vizinho > 3;
					// Pode ter 2 visinhos trasversais iguais a 3 e maior que 3 na quina dos vizinhos.
					im = ( i-1 <=  0) ?  0   : i-1;
					ip = ( i+1 >= nx) ? nx-1 : i+1;
					jm = ( j-1 <=  0) ?  0   : j-1;
					jp = ( j+1 >= ny) ? ny-1 : j+1;
					atendeu = true;
					// Verificando vizinhança com df > 3...
					if ( idf->data2D[im][j] > 3 ||
							 idf->data2D[ip][j] > 3 ||
							 idf->data2D[i][jm] > 3 ||
							 idf->data2D[i][jp] > 3 ) {
						atendeu = false;
					} else {
						// Verifica se os 2 vizinhos transversais são df==3 e se na quina dos vizinhos df > 3
						if ( idf->data2D[im][j] == 3 && idf->data2D[i][jm] == 3 && idf->data2D[im][jm] > 3 ) {
							atendeu = false;
						} else if ( idf->data2D[im][j] == 3 && idf->data2D[i][jp] == 3 && idf->data2D[im][jp] > 3 ) {
							atendeu = false;
						} else if ( idf->data2D[ip][j] == 3 && idf->data2D[i][jm] == 3 && idf->data2D[ip][jm] > 3 ) {
							atendeu = false;
						} else if ( idf->data2D[ip][j] == 3 && idf->data2D[i][jp] == 3 && idf->data2D[ip][jp] > 3 ) {
							atendeu = false;
						}
					}
					if ( atendeu ) {
						pmra->data2D[i][j] = 1;
					}/* else {
						pmra->data2D[i][j] = 2;
					}*/
				} else { //tratamento para df > 3, logo, o valor de
					// RA = 2 se (4  <= df <=  6);
					// RA = 3 se (7  <= df <=  9);
					// RA = 4 se (10 <= df <= 12); ...

					//Calcula valor do raio abertura (ra) de acordo com a df do pixel analizado;
					dft = df-1;
					do {
						++dft;
						ra = ( (dft % 3) == 0 ) ? dft/3 : 0;
					} while ( (dft % 3) != 0 );


					// Verifica se existe mascara de raio ra. Se não existir cria...
					it = vmask.find(ra);
					if( it == vmask.end() ) { // A mascara ainda não foi criada
						vmask[ra] = new CBCd34( (2*ra)+1 );
						it = vmask.find(ra);
					}
					this->mask = it->second;

					// ra corresponde ao raio da mascara, então não preciso calcular.
					rm = ra; //rm = (mask->NX()-1)/2; // raio da mascara

					atendeu = true;

					//percorre mascara de raio rm==ra para comparar as visinhanças (considera simetria)
					for (x=0; x<=rm; x++) {
						im = ((i-rm+x) <=  0) ?  0   : i-rm+x; // tentativa de burlar as bordas sem causar falha de segmentação
						ip = ((i+rm-x) >= nx) ? nx-1 : i+rm-x; // tentativa de burlar as bordas sem causar falha de segmentação
						for (y=0; y<=rm; y++) {
							if ( this->mask->data2D[x][y] != 0 ) {
								jm = ((j-rm+y) <=  0) ?  0   : j-rm+y; // tentativa de burlar as bordas sem causar falha de segmentação
								jp = ((j+rm-y) >= ny) ? ny-1 : j+rm-y; // tentativa de burlar as bordas sem causar falha de segmentação
								// Verifica se na vizinhança exite df > dft
								if ( idf->data2D[im][jm] > dft) {
									atendeu = false;
								} else if ( idf->data2D[ip][jp] > dft) {
									atendeu = false;
								} else if ( idf->data2D[ip][jm] > dft) {
									atendeu = false;
								} else if ( idf->data2D[im][jp] > dft) {
									atendeu = false;
								}
							}
						}
					}
					if ( atendeu ) {
						// Percorre toda a mascara e seta a visinhança com o valor mínio entre ra e o valor do pixel correspondente na pmra;
						for (x=0; x<=rm; x++) {
							im = ((i-rm+x) <=  0) ?  0   : i-rm+x; // tentativa de burlar as bordas sem causar falha de segmentação
							ip = ((i+rm-x) >= nx) ? nx-1 : i+rm-x; // tentativa de burlar as bordas sem causar falha de segmentação
							for (y=0; y<=rm; y++) {
								if ( this->mask->data2D[x][y] != 0 ) {
									jm = ((j-rm+y) <=  0) ?  0   : j-rm+y; // tentativa de burlar as bordas sem causar falha de segmentação
									jp = ((j+rm-y) >= ny) ? ny-1 : j+rm-y; // tentativa de burlar as bordas sem causar falha de segmentação
									pmra->data2D[i][j] = ra;
									pmra->data2D[im][jm] = min( ra, pmra->data2D[im][jm] );
									pmra->data2D[ip][jp] = min( ra, pmra->data2D[ip][jp] );
									pmra->data2D[ip][jm] = min( ra, pmra->data2D[ip][jm] );
									pmra->data2D[im][jp] = min( ra, pmra->data2D[im][jp] );
								}
							}
						}
					}
				}
			}
		}
	}
	delete idf;
	return pmra;
}
