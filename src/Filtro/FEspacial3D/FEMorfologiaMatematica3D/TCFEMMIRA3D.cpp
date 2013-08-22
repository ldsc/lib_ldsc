/*
----------------------------------------------------------------------------
PROJETO:		LIBLDSC
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
Programadores:    Leandro Puerari
Copyright @2013:  Todos os direitos reservados.
Nome deste arquivo:	TCFEMMIRA3D.cpp
Nome da classe:     TCFEMMIRA3D
*/

using namespace std;
#ifndef TCFEMMIRA3D_H
#include "Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIRA3D.h"
#endif

#include "Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFd3453D.h"
#include "Geometria/Bola/BCDiscreta3D/CBCd3453D.h"

#include <map>

// Construtor
template<typename T>
TCFEMMIRA3D<T>::TCFEMMIRA3D ( TCMatriz3D<T> * &matriz, int _indice, int _fundo )
	: TCFEMorfologiaMatematica3D<T> ( matriz, 0, _indice, _fundo ), pmra(NULL) {
}

template<typename T>
TCMatriz3D<int> * TCFEMMIRA3D<T>::Go () {
	// Veriáveis auxiliares
	int i, j, k;								// para percorrer a imagem
	int x, y, z;								// para percorrer a mascara
	int ip, im, jp, jm, kp, km; //para percorrer a imagem de acordo com a mascara
	int df = 0;									//distancia ao fundo
	int dft;										//distancia ao fundo temporária
	int ra;											//raio para a abertura
	int rm;											//raio da mascara;
	bool atendeu;								//flag para controlar atendimento das condições pra setar ra.
	int nx = this->pm->NX();		//número de voxeis em x
	int ny = this->pm->NY();		//número de voxeis em y
	int nz = this->pm->NZ();		//número de voxeis em z

	// Cria vetor que irá armazenar as mascaras.
	std::map<int, CBCd3453D* > vmask;
	std::map<int, CBCd3453D* >::iterator it;

	// Cria Imagem de Distância ao Fundo (IDF)
	TCFEMMIDFd3453D<T> * idf = new TCFEMMIDFd3453D<T> (this->pm, this->INDICE, this->FUNDO);

	// Determina a IDF
	idf->Go(this->pm);

	// Cria imagem 3D que representará a imagem Raio Abertura
	//pmra = new TCMatriz3D<int> ( * dynamic_cast< TCMatriz3D<int> *>(idf) );
	pmra = new TCMatriz3D<int> ( nx, ny, nz );

#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(static,10) //reduction(+:variavel)
	for (k=0; k<nz; ++k) {
		for (j=0; j<ny; ++j) {
			for (i=0; i<nx; ++i) {
				if (idf->data3D[i][j][k] > 0) {
					pmra->data3D[i][j][k] = -idf->data3D[i][j][k];
				}
			}
		}
	}

	//percorre a imagem
	for (k=0; k<nz; ++k) {
		for (j=0; j<ny; ++j) {
			for (i=0; i<nx; ++i) {
				if (idf->data3D[i][j][k] > 0) { //só interessam pixeis que não correspondem a fundo (matriz sólida)
					df = idf->data3D[i][j][k];
					if (df == 3) { // df == 3, logo, o valor de RA será 1 se o pixel analizado não tiver vizinho > 3;
						im = ( i-1 <=  0) ? i+1 : i-1;
						ip = ( i+1 >= nx) ? i-1 : i+1;
						jm = ( j-1 <=  0) ? j+1 : j-1;
						jp = ( j+1 >= ny) ? j-1 : j+1;
						km = ( k-1 <=  0) ? k+1 : k-1;
						kp = ( k+1 >= nz) ? k-1 : k+1;

						// Verificando se toda a vizinhança tem df<=3
						if ( idf->data3D[i][j][km] <= 3 &&	//frente
								 idf->data3D[i][j][kp] <= 3 &&	//tras
								 idf->data3D[i][jm][k] <= 3 &&	//cima
								 idf->data3D[i][jp][k] <= 3 &&	//baixo
								 idf->data3D[im][j][k] <= 3 &&	//esquerda
								 idf->data3D[ip][j][k] <= 3	){	//direira
							pmra->data3D[i][j][k] = 1;
						}
					} else { //tratamento para df > 3, logo, o valor de
						// RA = 2, se (4  <= df <=  6);
						// RA = 3, se (7  <= df <=  9);
						// RA = 4, se (10 <= df <= 12); ...
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
							vmask[ra] = new CBCd3453D( (2*ra)+1 );
							it = vmask.find(ra);
						}
						this->mask = it->second;

						// o raio da mascara (rm) corresponde ao raio abertura (ra), então não preciso calcular.
						rm = ra; //rm = (mask->NX()-1)/2; // raio da mascara

						++dft; // dft irá corresponder ao primeiro valor de df de ra+1
						// Percorre mascara de raio rm para comparar as visinhanças (considera simetria)
						// No momento que deixar de atender, sai do loop da mascara (atendeu = false;)
						for (z=0; z<=rm && atendeu; ++z) {
							km = ((k-rm+z) <=  0) ? k+rm-z : k-rm+z; // tentativa de burlar as bordas sem causar falha de segmentação
							kp = ((k+rm-z) >= nz) ? k-rm+z : k+rm-z; // tentativa de burlar as bordas sem causar falha de segmentação
							for (y=0; y<=rm && atendeu; ++y) {
								jm = ((j-rm+y) <=  0) ? j+rm-y : j-rm+y; // tentativa de burlar as bordas sem causar falha de segmentação
								jp = ((j+rm-y) >= ny) ? j-rm+y : j+rm-y; // tentativa de burlar as bordas sem causar falha de segmentação
								for (x=0; x<=rm && atendeu; ++x) {
									if ( this->mask->data3D[x][y][z] != 0 ) {
										im = ((i-rm+x) <=  0) ? i+rm-x : i-rm+x; // tentativa de burlar as bordas sem causar falha de segmentação
										ip = ((i+rm-x) >= nx) ? i-rm+x : i+rm-x; // tentativa de burlar as bordas sem causar falha de segmentação
										// Verifica se na vizinhança exite df > dft
										// Poderia usar ||, porém, fazendo testes separados fica mais rápido.
										if ( idf->data3D[im][jm][km] > dft) {
											atendeu = false;
										} else if ( idf->data3D[im][jm][kp] > dft) {
											atendeu = false;
										} else if ( idf->data3D[ip][jm][km] > dft) {
											atendeu = false;
										} else if ( idf->data3D[ip][jm][kp] > dft) {
											atendeu = false;
										} else if ( idf->data3D[im][jp][km] > dft) {
											atendeu = false;
										} else if ( idf->data3D[im][jp][kp] > dft) {
											atendeu = false;
										} else if ( idf->data3D[ip][jp][km] > dft) {
											atendeu = false;
										} else if ( idf->data3D[ip][jp][kp] > dft) {
											atendeu = false;
										}
									}
								}
							}
						}
						if ( atendeu ) {
							// Percorre toda a mascara e seta a visinhança com o valor mínio entre ra e o valor do pixel correspondente na pmra;
							for (z=0; z<=rm; ++z) {
								km = ((k-rm+z) <=  0) ? k+rm-z : k-rm+z; // tentativa de burlar as bordas sem causar falha de segmentação
								kp = ((k+rm-z) >= nz) ? k-rm+z : k+rm-z; // tentativa de burlar as bordas sem causar falha de segmentação
								for (y=0; y<=rm; ++y) {
									jm = ((j-rm+y) <=  0) ? j+rm-y : j-rm+y; // tentativa de burlar as bordas sem causar falha de segmentação
									jp = ((j+rm-y) >= ny) ? j-rm+y : j+rm-y; // tentativa de burlar as bordas sem causar falha de segmentação
									for (x=0; x<=rm; ++x) {
										if ( this->mask->data3D[x][y][k] != 0 ) {
											im = ((i-rm+x) <=  0) ? i+rm-x : i-rm+x; // tentativa de burlar as bordas sem causar falha de segmentação
											ip = ((i+rm-x) >= nx) ? i-rm+x : i+rm-x; // tentativa de burlar as bordas sem causar falha de segmentação
											pmra->data3D[im][jm][km] = (idf->data3D[im][jm][km] != 0) ? MinGtZero ( ra, pmra->data3D[im][jm][km] ) : 0;
											pmra->data3D[im][jm][kp] = (idf->data3D[im][jm][kp] != 0) ? MinGtZero ( ra, pmra->data3D[im][jm][kp] ) : 0;
											pmra->data3D[ip][jm][km] = (idf->data3D[ip][jm][km] != 0) ? MinGtZero ( ra, pmra->data3D[ip][jm][km] ) : 0;
											pmra->data3D[ip][jm][kp] = (idf->data3D[ip][jm][kp] != 0) ? MinGtZero ( ra, pmra->data3D[ip][jm][kp] ) : 0;
											pmra->data3D[im][jp][km] = (idf->data3D[im][jp][km] != 0) ? MinGtZero ( ra, pmra->data3D[im][jp][km] ) : 0;
											pmra->data3D[im][jp][kp] = (idf->data3D[im][jp][kp] != 0) ? MinGtZero ( ra, pmra->data3D[im][jp][kp] ) : 0;
											pmra->data3D[ip][jp][km] = (idf->data3D[ip][jp][km] != 0) ? MinGtZero ( ra, pmra->data3D[ip][jp][km] ) : 0;
											pmra->data3D[ip][jp][kp] = (idf->data3D[ip][jp][kp] != 0) ? MinGtZero ( ra, pmra->data3D[ip][jp][kp] ) : 0;
										}
									}
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
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(static,10)
	for ( k=0; k<nz; ++k ) {
		for ( j=0; j<ny; ++j ) {
			for ( i=0; i<nx; ++i ) {
				if ( pmra->data3D[i][j][k] < 0 ) {
					pmra->data3D[i][j][k] *= -1;
				}
			}
		}
	}
	delete idf;
	return pmra;
}
