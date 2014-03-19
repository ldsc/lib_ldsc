#include <gtest/gtest.h>
#include <AnaliseImagem/Matriz/TCMatriz2D.h>

#include "../gtest_configure.h"
#include "../fixtures.h"

TEST_F(TestMatriz2D, ConstrutorCopia) {
	TCMatriz2D<int>* pm = new TCMatriz2D<int>(cfg.original2D);
	TCMatriz2D<int>* pm2 = new TCMatriz2D<int>(*pm);

	EXPECT_TRUE(*pm == *pm2);
}
/*
#include <Geometria/Bola/BCDiscreta3D/CBCd3453D.h>
#include <iostream>
TEST_F(TestMatriz2D, bolas) {
	CBCDiscreta3D * bola = nullptr;
	ofstream file("numPixeisBolas.txt", ios_base::out | ios_base::trunc);
	if (file.good()) {
		file << "std::vector<int> numPixeisBolaByRaio {0";
		int numPixeis, i, j, k;
		for (int d=3; d<602; d+=2) {
			numPixeis = 0;
			bola = new CBCd3453D(d);
			for (i=0; i<bola->NX(); ++i) {
				for (j=0; j<bola->NY(); ++j) {
					for (k=0; k<bola->NZ(); ++k) {
						if (bola->data3D[i][j][k]!=0)
							++numPixeis;
					}
				}
			}
			file << "," << numPixeis;
			delete bola;
		}
		file << "};";
		file.close();

		EXPECT_TRUE(true);
	} else {
		EXPECT_TRUE(false);
	}
}
*/
/*
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd34.h>
#include <iostream>
TEST_F(TestMatriz2D, discos) {
	CBCDiscreta * bola = nullptr;
	ofstream file("numPixeisDiscos.txt", ios_base::out | ios_base::trunc);
	if (file.good()) {
		file << "std::vector<int> numPixeisDiscos {0";
		int numPixeis, i, j;
		for (int d=3; d<602; d+=2) {
			numPixeis = 0;
			bola = new CBCd34(d);
			for (i=0; i<bola->NX(); ++i) {
				for (j=0; j<bola->NY(); ++j) {
					if (bola->data2D[i][j]!=0)
						++numPixeis;
				}
			}
			file << "," << numPixeis;
			delete bola;
		}
		file << "};";
		file.close();

		EXPECT_TRUE(true);
	} else {
		EXPECT_TRUE(false);
	}
}
*/
/*
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd34.h>
#include <iostream>
TEST_F(TestMatriz2D, perimetroDiscos) {
	CBCDiscreta * bola = nullptr;
	TCMatriz2D <int> * mat = nullptr;
	ofstream file("perimetroDiscos.txt", ios_base::out | ios_base::trunc);
	if (file.good()) {
		file << "std::vector<int> numPixeisDiscos {0";
		int perimetro, i, j;
		for (int d=3; d<602; d+=2) {
			perimetro = 0;
			bola = new CBCd34(d);
			mat = new TCMatriz2D<int>(bola->NX()+2, bola->NY()+2);
			for (i=0; i<bola->NX(); ++i) {
				for (j=0; j<bola->NY(); ++j) {
					mat->data2D[i+1][j+1] = bola->data2D[i][j];
				}
			}
			for (i=1; i<mat->NX()-1; ++i) {
				for (j=1; j<mat->NY()-1; ++j) {
					if (mat->data2D[i][j] != 0) {
						if (mat->data2D[i+1][j] == 0)
							++perimetro;
						if (mat->data2D[i-1][j] == 0)
							++perimetro;
						if (mat->data2D[i][j+1] == 0)
							++perimetro;
						if (mat->data2D[i][j-1] == 0)
							++perimetro;
					}
				}
			}
			file << "," << perimetro;
			delete bola;
			delete mat;
		}
		file << "};";
		file.close();
		EXPECT_TRUE(true);
	} else {
		EXPECT_TRUE(false);
	}
}
*/
