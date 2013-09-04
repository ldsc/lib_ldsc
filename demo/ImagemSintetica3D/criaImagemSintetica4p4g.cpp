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

#include <Matriz/TCImagem3D.h>
#include <Geometria/Bola/BCDiscreta/CBCd5711.h>
#include <Geometria/Bola/BCDiscreta3D/CBCd3453D.h>

using namespace std;

/**
 * Cria imagem 3D com quatro poros esfericos ligados por quatro gargantas cilindricas.
*/

int main (int argc, char *argv[]) {
	int result;
	cout  << "\n===================================================================================="
				<< "\n======== Programa para criar imagem sintética 3D com 4 poros e 4 gargantas ========="
				<< "\n====================================================================================" << endl;

	unsigned int nxyz = 0; //tamanho da imagem
	unsigned int desfera1 = 25; //diametro da primeira esfera
	unsigned int desfera2 = 21; //diametro da segunda esfera
	unsigned int desfera3 = 15; //diametro da terceira esfera
	unsigned int desfera4 = 15; //diametro da quarta esfera
	unsigned int dcilindro1 = 19; //diametro do cilindro
	unsigned int dcilindro2 = 11; //diametro do cilindro
	unsigned int dcilindro3 = 7; //diametro do cilindro
	unsigned int dcilindro4 = 9; //diametro do cilindro
	unsigned int margem = 10; //borda da imagem;
	unsigned int teclado = 0; //valores digitados pelo usuário
	unsigned int menor_esfera;
	int x,y,z;
	int i,j,k;
	int xf,zf;

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

	cout << "Diametro da terceira esfera (impar - menor que " << int(nxyz/3) << "):" << endl;
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
	desfera3 = teclado;

	cout << "Diametro da quarta esfera (impar - menor que " << int(nxyz/3) << "):" << endl;
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
	desfera4 = teclado;

	// primeiro cilindro
	menor_esfera = (desfera1 < desfera2) ? desfera1 : desfera2;
	cout << "Diametro do cilindro que ligará a primeira e segunda esferas (impar - menor que " << menor_esfera <<")" << endl;
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
	dcilindro1 = teclado;

	//segundo cilindro
	menor_esfera = (desfera2 < desfera3) ? desfera2 : desfera3;
	cout << "Diametro do cilindro que ligará a segunda e tercera esferas (impar - menor que " << menor_esfera <<")" << endl;
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
	dcilindro2 = teclado;

	//terceiro cilindro
	menor_esfera = (desfera3 < desfera4) ? desfera3 : desfera4;
	cout << "Diametro do cilindro que ligará a tercera e quarta esferas (impar - menor que " << menor_esfera <<")" << endl;
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
	dcilindro3 = teclado;

	//quarto cilindro
	menor_esfera = (desfera1 < desfera4) ? desfera1 : desfera4;
	cout << "Diametro do cilindro que ligará a primeira e quarta esferas (impar - menor que " << menor_esfera <<")" << endl;
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
	dcilindro4 = teclado;

	cout << "Desenhando imagem..." << endl;

	TCImagem3D<bool>* img = new TCImagem3D<bool>(nxyz, nxyz, nxyz);
	CBCDiscreta* cilindro1 = new CBCd5711(dcilindro1);
	CBCDiscreta* cilindro2 = new CBCd5711(dcilindro2);
	CBCDiscreta* cilindro3 = new CBCd5711(dcilindro3);
	CBCDiscreta* cilindro4 = new CBCd5711(dcilindro4);

	CBCDiscreta3D* esfera1 = new CBCd3453D(desfera1);
	CBCDiscreta3D* esfera2 = new CBCd3453D(desfera2);
	CBCDiscreta3D* esfera3 = new CBCd3453D(desfera3);
	CBCDiscreta3D* esfera4 = new CBCd3453D(desfera4);

	cout << "Desenhando esfera 1..." << endl;
	x = (nxyz/4)-(desfera1/2); //centraliza a primeira esfera no primeiro quadrante da imagem
	y = (nxyz/2)-(desfera1/2); //centraliza a primeira esfera no eixo y da imagem
	z = (desfera1 >= desfera4) ? margem : (margem+((desfera4-desfera1)/2)); // margem para o eixo z

	for (i=0; i < desfera1; i++) {
		for (j=0; j < desfera1; j++) {
			for (k=0; k < desfera1; k++) {
				if (esfera1->data3D[i][j][k]!=0) {
					img->data3D[x+i][y+j][z+k] = 1;
				}
			}
		}
	}

	cout << "Desenhando esfera 2..." << endl;
	x = (nxyz/4)-(desfera2/2); //centraliza a segunda esfera no segundo quadrante da imagem
	y = (nxyz/2)-(desfera2/2); //centraliza a segunda esfera no eixo y da imagem
	z = (desfera2 >= desfera3) ? (nxyz-margem-desfera2) : (nxyz-margem-(desfera3/2)-(desfera2/2)); // margem para o eixo z

	for (i=0; i < desfera2; i++) {
		for (j=0; j < desfera2; j++) {
			for (k=0; k < desfera2; k++) {
				if (esfera2->data3D[i][j][k]!=0) {
					img->data3D[x+i][y+j][z+k] = 1;
				}
			}
		}
	}

	cout << "Desenhando esfera 3..." << endl;
	x = nxyz - ((nxyz/4)+(desfera3/2)); //centraliza a terceira esfera no terceiro quadrante da imagem
	y = (nxyz/2)-(desfera3/2); //centraliza a terceira esfera no eixo y da imagem
	z = (desfera3 >= desfera2) ? (nxyz-margem-desfera3) : (nxyz-margem-(desfera2/2)-(desfera3)/2); // margem para o eixo z
	//z = (desfera3 >= desfera2) ? (nxyz-margem-desfera3) : (nxyz-margem-desfera3-((desfera2-desfera3)/2)); // margem para o eixo z
	//z = nxyz-margem-desfera3; // margem para o eixo z

	for (i=0; i < desfera3; i++) {
		for (j=0; j < desfera3; j++) {
			for (k=0; k < desfera3; k++) {
				if (esfera3->data3D[i][j][k]!=0) {
					img->data3D[x+i][y+j][z+k] = 1;
				}
			}
		}
	}

	cout << "Desenhando esfera 4..." << endl;
	x = nxyz - ((nxyz/4)+(desfera4/2)); //centraliza a terceira esfera no terceiro quadrante da imagem
	y = (nxyz/2)-(desfera4/2); //centraliza a terceira esfera no eixo y da imagem
	z = (desfera4 >= desfera1) ? margem : (margem+((desfera1-desfera4)/2)); // margem para o eixo z
	//z = margem; // margem para o eixo z

	for (i=0; i < desfera4; i++) {
		for (j=0; j < desfera4; j++) {
			for (k=0; k < desfera4; k++) {
				if (esfera4->data3D[i][j][k]!=0) {
					img->data3D[x+i][y+j][z+k] = 1;
				}
			}
		}
	}

	cout << "Desenhando cilindro entre esferas 1 e 2..." << endl;
	x = (nxyz/4)-(dcilindro1/2); //centraliza cilindro no eixo x da imagem
	y = (nxyz/2)-(dcilindro1/2); //centraliza cilindro no eixo y da imagem
	z = margem+(desfera1/2); // inicia cilindro no centro da primeira esfera
	zf = nxyz-margem-(desfera2/2); //termina cilindro no centro da segunda esfera;

	for (int i=0; i < dcilindro1; i++) {
		for (int j=0; j < dcilindro1; j++) {
			if (cilindro1->data2D[i][j]!=0) {
				for (int k=z; k <= zf; k++) {
					img->data3D[x+i][y+j][k] = 1;
				}
			}
		}
	}

	cout << "Desenhando cilindro entre esferas 2 e 3..." << endl;
	x = (nxyz/4); //centraliza cilindro no eixo x da imagem
	y = (nxyz/2)-(dcilindro2/2); //centraliza cilindro no eixo y da imagem
	z = (desfera2 >= desfera3) ? nxyz-margem-(desfera2/2)-(dcilindro2/2) : nxyz-margem-(desfera3/2)-(dcilindro2/2); // margem para o eixo z
	xf = nxyz-(nxyz/4); //termina cilindro no centro da quarta esfera;

	for (int i=0; i < dcilindro2; i++) {
		for (int j=0; j < dcilindro2; j++) {
			if (cilindro2->data2D[i][j]!=0) {
				for (int k=x; k <= xf; k++) {
					img->data3D[k][y+i][z+j] = 1;
				}
			}
		}
	}

	cout << "Desenhando cilindro entre esferas 3 e 4..." << endl;
	x = nxyz - ((nxyz/4)+(dcilindro3/2)); //centraliza cilindro no eixo x da imagem
	y = (nxyz/2)-(dcilindro3/2); //centraliza cilindro no eixo y da imagem
	z = margem+(desfera4/2); // inicia cilindro no centro da quarta esfera
	zf = nxyz-margem-(desfera3/2); //termina cilindro no centro da terceira esfera;

	for (int i=0; i < dcilindro3; i++) {
		for (int j=0; j < dcilindro3; j++) {
			if (cilindro3->data2D[i][j]!=0) {
				for (int k=z; k <= zf; k++) {
					img->data3D[x+i][y+j][k] = 1;
				}
			}
		}
	}

	cout << "Desenhando cilindro entre esferas 1 e 4..." << endl;
	x = (nxyz/4); //centraliza cilindro no eixo x da imagem
	y = (nxyz/2)-(dcilindro4/2); //centraliza cilindro no eixo y da imagem
	z = (desfera1 >= desfera4) ? margem+(desfera1/2)-(dcilindro4/2) : margem+(desfera4/2)-(dcilindro4/2); // margem para o eixo z
	xf = nxyz-(nxyz/4); //termina cilindro no centro da quarta esfera;

	for (int i=0; i < dcilindro4; i++) {
		for (int j=0; j < dcilindro4; j++) {
			if (cilindro4->data2D[i][j]!=0) {
				for (int k=x; k <= xf; k++) {
					img->data3D[k][y+i][z+j] = 1;
				}
			}
		}
	}

	cout << "Salvando a imagem em disco..." << endl;
	img->SetFormato(D4_X_Y_Z_BINARY);
	ostringstream os;
	os << "imgSintPorosGargantas-" << nxyz << "_p1-" << desfera1 << "_p2-" << desfera2 << "_p3-" << desfera3 << "_p4-" << desfera4 << "_g1-" << dcilindro1 << "_g2-" << dcilindro2 << "_g3-" << dcilindro3 << "_g4-" << dcilindro4 << ".dbm";
	cout << "Salvando a imagem 3D em disco com o nome " << os.str() << endl;
	img->Write( os.str() );

	cout << "Abrindo imagem no LVP..." << endl;
	string comando = string( "lvp ") + os.str()  + " &";
	result = system( comando.c_str() ) ;

	delete img;
	return 0;
}

