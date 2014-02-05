#include <gtest/gtest.h>
#include <Matriz/TCMatriz2D.h>

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
	CBCDiscreta3D * bola = NULL;
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
