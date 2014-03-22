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

#include <MetNum/Matriz/TCImagem3D.h>
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd5711.h>
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd3453D.h>

using namespace std;

/**
 * Cria imagem 3D com dois poros esfericos ligados por uma garganta cilindrica.
*/

int main (int argc, char *argv[]) {
	int result;
	cout  << "\n================================================================================"
				<< "\n======== Programa para criar imagem sintética 3D com poros e gargantas ========="
				<< "\n================================================================================" << endl;

	unsigned int nxyz = 0; //tamanho da imagem
	unsigned int desfera1 = 25; //diametro da primeira esfera
	unsigned int desfera2 = 21; //diametro da segunda esfera
	unsigned int dcilindro = 15; //diametro do cilindro
	unsigned int margem = 10; //borda da imagem;
	unsigned int teclado = 0; //valores digitados pelo usuário

	cout << "Tamanho da imagem (par):" << endl;
	cin >>  teclado;
	while ( (teclado % 2) != 0 ){
		cout << "O tamanho da imagem deve ser par!" << endl;
		cin >>  teclado;
	}
	nxyz = teclado;

	cout << "Borda da imagem (de 0 a " << nxyz/10 << "):" << endl;
	cin >>  teclado;
	while ( teclado > nxyz/10 || teclado < 0 ){
		cout << "O tamanho da borda deve ser entre 0 e " << nxyz/10 << "!" << endl;
		cin >> teclado;
	}
	margem = teclado;

	cout << "Diametro da primeira esfera (impar - menor que " << int(nxyz/3) << "):" << endl;
	cin >> teclado;
	while ( ((teclado % 2) == 0) || (teclado >= int(nxyz/3)) ) {
		if ( (teclado % 2) == 0 ) {
			cout << "O diamentro da esfera deve ser impar!" << endl;
		}
		if ( teclado >= int(nxyz/3) ) {
			cout << "A esfera deve ser menor que " << int(nxyz/3) << "):" << endl;
		}
		cin >> teclado;
	}
	desfera1 = teclado;

	cout << "Diametro da segunda esfera (impar - menor que " << int(nxyz/3) << "):" << endl;
	cin >> teclado;
	while ( ((teclado % 2) == 0) || (teclado >= int(nxyz/3)) ) {
		if ( (teclado % 2) == 0 ) {
			cout << "O diamentro da esfera deve ser impar!" << endl;
		}
		if ( teclado >= int(nxyz/3) ) {
			cout << "A esfera deve ser menor que " << int(nxyz/3) << "):" << endl;
		}
		cin >>  teclado;
	}
	desfera2 = teclado;

	int menor_esfera = (desfera1 < desfera2) ? desfera1 : desfera2;

	cout << "Diametro do cilindro (impar - menor que " << menor_esfera <<")" << endl;
	cin >> teclado;
	while ( ((teclado % 2) == 0) || teclado >= menor_esfera ) {
		if ( (teclado % 2) == 0 ) {
			cout << "O diamentro do cilindro deve ser impar!" << endl;
		}
		if ( teclado >= menor_esfera ) {
			cout << "O cilindro deve ser menor que " << menor_esfera << ")" << endl;
		}
		cin >>  teclado;
	}
	dcilindro = teclado;

	cout << "Desenhando imagem..." << endl;

	TCImagem3D<bool>* img = new TCImagem3D<bool>(nxyz, nxyz, nxyz);
	CBCDiscreta* cilindro = new CBCd5711(dcilindro);
	CBCDiscreta3D* esfera1 = new CBCd3453D(desfera1);
	CBCDiscreta3D* esfera2 = new CBCd3453D(desfera2);

	//desenhando primeira esfera
	int x = (nxyz/2)-(desfera1/2); //centraliza a primeira esfera no eixo x da imagem
	int y = x; //centraliza a primeira esfera no eixo y da imagem
	int z = margem; // margem de 10 vozels para o eixo z

	for (int i=0; i < desfera1; i++) {
		for (int j=0; j < desfera1; j++) {
			for (int k=0; k < desfera1; k++) {
				if (esfera1->data3D[i][j][k]!=0) {
					img->data3D[x+i][y+j][z+k] = 1;
				}
			}
		}
	}

	//desenhando segunda esfera
	x = (nxyz/2)-(desfera2/2); //centraliza a segunda esfera no eixo x da imagem
	y = x; //centraliza a segunda esfera no eixo y da imagem
	z = nxyz-margem-desfera2; // margem de 10 vozels para o eixo z

	for (int i=0; i < desfera2; i++) {
		for (int j=0; j < desfera2; j++) {
			for (int k=0; k < desfera2; k++) {
				if (esfera2->data3D[i][j][k]!=0) {
					img->data3D[x+i][y+j][z+k] = 1;
				}
			}
		}
	}

	//desenhando cilindro que interliga as esferas
	x = (nxyz/2)-(dcilindro/2); //centraliza cilindro no eixo x da imagem
	y = x; //centraliza cilindro no eixo y da imagem
	z = margem+(desfera1/2); // inicia cilindro no centro da primeira esfera
	int zf = nxyz-margem-(desfera2/2); //termina cilindro no centro da segunda esfera;

	for (int i=0; i < dcilindro; i++) {
		for (int j=0; j < dcilindro; j++) {
			if (cilindro->data2D[i][j]!=0) {
				for (int k=z; k <= zf; k++) {
					img->data3D[x+i][y+j][k] = 1;
				}
			}
		}
	}

	//Salvando a imagem em disco
	img->SetFormato(D4_X_Y_Z_BINARY);
	ostringstream os;
	os << "imgSintPorosGarganta-" << nxyz << "_p1-" << desfera1 << "_p2-" << desfera2 << "_g-" << dcilindro << ".dbm";
	cout << "Salvando a imagem 3D em disco com o nome " << os.str() << endl;
	img->Write( os.str() );

	cout << "Abrindo imagem no LVP..." << endl;
	string comando = string( "lvp ") + os.str()  + " &";
	result = system( comando.c_str() ) ;

	delete img;
	return 0;
}

