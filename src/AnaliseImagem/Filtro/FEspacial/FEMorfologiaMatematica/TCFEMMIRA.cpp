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
#include "AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIRA.h"
#endif

#include "AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd34.h"
#include "AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd34.h"

#include <map>

// Construtor
template<typename T>
TCFEMMIRA<T>::TCFEMMIRA ( TCMatriz2D<T> * &matriz, int _indice, int _fundo )
	: TCFEMorfologiaMatematica<T> ( matriz, 0, _indice, _fundo ), pmra(NULL) {
}

template<typename T>
TCMatriz2D<int> * TCFEMMIRA<T>::Go () {
	// Veriáveis auxiliares
	int i, j, x, y, ip, im, jp, jm;
	int dft, ra;
	int rm; // raio da mascara;
	int df = 0; //distancia ao fundo
	bool atendeu;
	int nx = this->pm->NX();
	int ny = this->pm->NY();

	// Cria vetor que irá armazenar as mascaras.
	std::map<int, CBCd34* > vmask;
	std::map<int, CBCd34* >::iterator it;

	// Cria Imagem de Distância ao Fundo (IDF)
	TCFEMMIDFd34<T> * idf = new TCFEMMIDFd34<T> (this->pm, this->INDICE, this->FUNDO);

	// Determina a IDF
	idf->Go(this->pm);

	// Cria imagem 2D que representará a imagem Raio Abertura (cópia da IDF)
	//pmra = new TCMatriz2D<int> ( * dynamic_cast< TCMatriz2D<int> *>(idf) );
	pmra = new TCMatriz2D<int> ( nx, ny );

#pragma omp parallel for collapse(2) default(shared) private(i,j) //schedule(static,10) //reduction(+:variavel)
	for (j=0; j<ny; ++j) {
		for (i=0; i<nx; ++i) {
			if (idf->data2D[i][j] > 0) {
				pmra->data2D[i][j] = -idf->data2D[i][j];
			}
		}
	}

	//percorre a imagem
	for (i=0; i<nx; ++i) {
		for (j=0; j<ny; ++j) {
			if (idf->data2D[i][j] > 0) { //só interessam pixeis que não correspondem a fundo (matriz sólida)
				df = idf->data2D[i][j];
				if (df == 3) {
					// df == 3, logo, o valor de RA será 1 se o pixel analizado:
					// Não tiver vizinho > 3;
					im = ( i-1 <=  0) ? i+1 : i-1;
					ip = ( i+1 >= nx) ? i-1 : i+1;
					jm = ( j-1 <=  0) ? j+1 : j-1;
					jp = ( j+1 >= ny) ? j-1 : j+1;

					// Verificando se toda a vizinhança tem df<=3
					if ( idf->data2D[im][j] <= 3 &&
							 idf->data2D[ip][j] <= 3 &&
							 idf->data2D[i][jm] <= 3 &&
							 idf->data2D[i][jp] <= 3 ) {
						pmra->data2D[i][j] = 1;
					}/* else { // Pode ter 2 visinhos trasversais iguais a 3 e maior que 3 na quina dos vizinhos.
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
						if ( atendeu ) {
							pmra->data2D[i][j] = 1;
						}
					}*/
				} else { //tratamento para df > 3, logo, o valor de
					// RA = 2 se (4  <= df <=  6);
					// RA = 3 se (7  <= df <=  9);
					// RA = 4 se (10 <= df <= 12); ...
					atendeu = true;
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

					// o raio da mascara (rm) corresponde ao raio abertura (ra), então não preciso calcular.
					rm = ra; //rm = (mask->NX()-1)/2; // raio da mascara

					++dft; // dft irá corresponder ao primeiro valor de df de ra+1
					// Percorre mascara de raio rm==ra para comparar as visinhanças (considera simetria)
					// No momento que deixar de atender, sai do loop da mascara (atendeu = false;)
					for (y=0; y<=rm && atendeu; ++y) {
						jm = ((j-rm+y) <=  0) ? j+rm-y : j-rm+y; // tentativa de burlar as bordas sem causar falha de segmentação
						jp = ((j+rm-y) >= ny) ? j-rm+y : j+rm-y; // tentativa de burlar as bordas sem causar falha de segmentação
						for (x=0; x<=rm && atendeu; ++x) {
							if ( this->mask->data2D[x][y] != 0 ) {
								im = ((i-rm+x) <=  0) ? i+rm-x : i-rm+x; // tentativa de burlar as bordas sem causar falha de segmentação
								ip = ((i+rm-x) >= nx) ? i-rm+x : i+rm-x; // tentativa de burlar as bordas sem causar falha de segmentação
								// Verifica se na vizinhança exite df > dft
								// Poderia usar ||, porém, fazendo testes separados fica mais rápido.
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
						for (y=0; y<=rm; ++y) {
							jm = ((j-rm+y) <=  0) ? j+rm-y : j-rm+y; // tentativa de burlar as bordas sem causar falha de segmentação
							jp = ((j+rm-y) >= ny) ? j-rm+y : j+rm-y; // tentativa de burlar as bordas sem causar falha de segmentação
							for (x=0; x<=rm; ++x) {
								if ( this->mask->data2D[x][y] != 0 ) {
									im = ((i-rm+x) <=  0) ? i+rm-x : i-rm+x; // tentativa de burlar as bordas sem causar falha de segmentação
									ip = ((i+rm-x) >= nx) ? i-rm+x : i+rm-x; // tentativa de burlar as bordas sem causar falha de segmentação
									pmra->data2D[im][jm] = (idf->data2D[im][jm] != 0) ? MinGtZero ( ra, pmra->data2D[im][jm] ) : 0;
									pmra->data2D[ip][jp] = (idf->data2D[ip][jp] != 0) ? MinGtZero ( ra, pmra->data2D[ip][jp] ) : 0;
									pmra->data2D[ip][jm] = (idf->data2D[ip][jm] != 0) ? MinGtZero ( ra, pmra->data2D[ip][jm] ) : 0;
									pmra->data2D[im][jp] = (idf->data2D[im][jp] != 0) ? MinGtZero ( ra, pmra->data2D[im][jp] ) : 0;
									//									pmra->data2D[i][j] = ra;
									//									pmra->data2D[im][jm] = min( ra, pmra->data2D[im][jm] );
									//									pmra->data2D[ip][jp] = min( ra, pmra->data2D[ip][jp] );
									//									pmra->data2D[ip][jm] = min( ra, pmra->data2D[ip][jm] );
									//									pmra->data2D[im][jp] = min( ra, pmra->data2D[im][jp] );
									//									pmra->data2D[im][jm] = (idf->data2D[im][jm] != 0) ? MinNotZero ( ra, pmra->data2D[im][jm] ) : 0;
									//									pmra->data2D[ip][jp] = (idf->data2D[ip][jp] != 0) ? MinNotZero ( ra, pmra->data2D[ip][jp] ) : 0;
									//									pmra->data2D[ip][jm] = (idf->data2D[ip][jm] != 0) ? MinNotZero ( ra, pmra->data2D[ip][jm] ) : 0;
									//									pmra->data2D[im][jp] = (idf->data2D[im][jp] != 0) ? MinNotZero ( ra, pmra->data2D[im][jp] ) : 0;
									//									pmra->data2D[im][jm] = (idf->data2D[im][jm] != 0) ? MinNotOne ( ra, pmra->data2D[im][jm] ) : 0;
									//									pmra->data2D[ip][jp] = (idf->data2D[ip][jp] != 0) ? MinNotOne ( ra, pmra->data2D[ip][jp] ) : 0;
									//									pmra->data2D[ip][jm] = (idf->data2D[ip][jm] != 0) ? MinNotOne ( ra, pmra->data2D[ip][jm] ) : 0;
									//									pmra->data2D[im][jp] = (idf->data2D[im][jp] != 0) ? MinNotOne ( ra, pmra->data2D[im][jp] ) : 0;
								}
							}
						}
					}
				}
			}
		}
	}
	// Alguns pixeis estão ficando apagados quando deviam estar acessos, então, estou colocando este loop para corrigir
	// Estudar possibilidade de retirar este loop!
#pragma omp parallel for collapse(2) default(shared) private(i,j) //schedule(static,10) //reduction(+:variavel)
	for ( j=0; j<ny; ++j ) {
		for ( i=0; i<nx; ++i ) {
			if (pmra->data2D[i][j] < 0 ) {
				pmra->data2D[i][j] = -1 * pmra->data2D[i][j];
			}
		}
	}
	/*
#pragma omp parallel for collapse(2) default(shared) private(i,j) //schedule(static,10) //reduction(+:variavel)
	for ( j=0; j<ny; ++j ) {
		for ( i=0; i<nx; ++i ) {
			if (idf->data2D[i][j] > 0 && pmra->data2D[i][j] == 0 ) {
				pmra->data2D[i][j] = 1;
			}
		}
	}
*/
	delete idf;
	return pmra;
}
