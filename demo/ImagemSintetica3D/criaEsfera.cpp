/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
===============================================================================
Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     Leandro Puerari
@file       criaImagemSintetica.cpp
@email      puerari@gmail.com
*/
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

#include <Matriz/TCMatriz3D.h>
#include <Geometria/Bola/BCDiscreta3D/CBCd3453D.h>
#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFd3453D.h>

using namespace std;

/**
 * Cria esfera.
*/

int main (int argc, char *argv[]) {
	int desfera1 = 9;
	CBCDiscreta3D* esfera1 = new CBCd3453D(desfera1);
	for (int i=0; i < desfera1; i++) {
		for (int j=0; j < desfera1; j++) {
			for (int k=0; k < desfera1; k++) {
				if (esfera1->data3D[i][j][k]!=0) {
					esfera1->data3D[i][j][k] = 1;
				}
			}
		}
	}
	esfera1->SetFormato(D1_X_Y_Z_ASCII);
	esfera1->Write("esfera.dbm");
	TCMatriz3D<bool>* pme = new TCMatriz3D<bool>("esfera.dbm");
	TCFEMMIDFd3453D<bool>* filtroe = new TCFEMMIDFd3453D<bool>(pme,1,0);
	filtroe->Go(pme);
	filtroe->SetFormato(D2_X_Y_Z_GRAY_ASCII);
	filtroe->NumCores(filtroe->MaiorValor());
	filtroe->Write("idfEsfera.dgm");

	return 0;
}

