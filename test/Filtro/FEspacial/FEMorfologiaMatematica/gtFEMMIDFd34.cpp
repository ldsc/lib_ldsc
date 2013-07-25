#include <gtest/gtest.h>
#include <omp.h>
#include <Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd34.h>
#include <Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd5711.h>

TEST(FilterTest_IDF_2D, TestIDFd34_1_0) {
	//carrega a imagem original
	TCMatriz2D<bool>* pm = new TCMatriz2D<bool>("../../data/images/2v5i7_640x480_BIN_1-0.pbm");
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz2D<int>* pm2 = new TCMatriz2D<int>("../../data/images/result_IDFd34_2v5i7_640x480_BIN_1-0.pgm");
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIDFd34<bool>* filtro = new TCFEMMIDFd34<bool>(pm,1,0);

	//Executa o filtro na imagem original
	filtro->Go(pm);
	//filtro->SetFormato(P5_X_Y_GRAY_BINARY);
	//filtro->NumCores(filtro->MaiorValor());
	//filtro->Write("result_IDFd34_2v5i7_640x480_BIN_1-0_new.pgm");

	//compara a imagem filtrada com o a imagem carregada anteriormente
	bool iguais = true;
	int i,j;
#pragma omp parallel for collapse(2) default(shared) private(i,j) //schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			if ( pm2->data2D[i][j] != filtro->data2D[i][j] ) {
				iguais = false;
				//ver como faz para sair do loop aninhado sem dar problema com a openMP
			}
		}
	}

	//exibe o resultado do teste
	EXPECT_TRUE(iguais);

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(FilterTest_IDF_2D, TestIDFd34_0_1) {
	//carrega a imagem original
	TCMatriz2D<bool>* pm = new TCMatriz2D<bool>("../../data/images/2v5i7_640x480_BIN_1-0.pbm");
	//carrega a imagem previamente processada com o filtro IDF sendo indice 0 e fundo 1.
	TCMatriz2D<int>* pm2 = new TCMatriz2D<int>("../../data/images/result_IDFd34_2v5i7_640x480_BIN_0-1.pgm");
	//Cria o filtro com poro igual e 0 e fundo igual a 1
	TCFEMMIDFd34<bool>* filtro = new TCFEMMIDFd34<bool>(pm,0,1);

	//aplica novamente o filtro
	filtro->Go(pm);
	//filtro->SetFormato(P5_X_Y_GRAY_BINARY);
	//filtro->NumCores(filtro->MaiorValor());
	//filtro->Path(pm->Path());
	//filtro->Write("result_IDFd34_2v5i7_640x480_BIN_0-1_new.pgm");

	bool iguais = true;
	int i,j;
	//compara a imagem filtrada com novos valores de INDICE e FUNDO
	#pragma omp parallel for collapse(2) default(shared) private(i,j) //schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			if ( pm2->data2D[i][j] != filtro->data2D[i][j] ) {
				iguais = false;
			}
		}
	}

	//exibe o resultado do teste
	EXPECT_TRUE(iguais);

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(FilterTest_IDF_2D, TestIDFd5711_1_0) {
	//carrega a imagem original
	TCMatriz2D<bool>* pm = new TCMatriz2D<bool>("../../data/images/2v5i7_640x480_BIN_1-0.pbm");
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz2D<int>* pm2 = new TCMatriz2D<int>("../../data/images/result_IDFd5711_2v5i7_640x480_BIN_1-0.pgm");
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIDFd5711<bool>* filtro = new TCFEMMIDFd5711<bool>(pm,1,0);

	//Executa o filtro na imagem original
	filtro->Go(pm);
	//filtro->SetFormato(P5_X_Y_GRAY_BINARY);
	//filtro->NumCores(filtro->MaiorValor());
	//filtro->Write("result_IDFd5711_2v5i7_640x480_BIN_1-0_new.pgm");

	//compara a imagem filtrada com o a imagem carregada anteriormente
	bool iguais = true;
	int i,j;
#pragma omp parallel for collapse(2) default(shared) private(i,j) //schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			if ( pm2->data2D[i][j] != filtro->data2D[i][j] ) {
				iguais = false;
				//ver como faz para sair do loop aninhado sem dar problema com a openMP
			}
		}
	}

	//exibe o resultado do teste
	EXPECT_TRUE(iguais);

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(FilterTest_IDF_2D, TestIDFd5711_0_1) {
	//carrega a imagem original
	TCMatriz2D<bool>* pm = new TCMatriz2D<bool>("../../data/images/2v5i7_640x480_BIN_1-0.pbm");
	//carrega a imagem previamente processada com o filtro IDF sendo indice 0 e fundo 1.
	TCMatriz2D<int>* pm2 = new TCMatriz2D<int>("../../data/images/result_IDFd5711_2v5i7_640x480_BIN_0-1.pgm");
	//Cria o filtro com poro igual e 0 e fundo igual a 1
	TCFEMMIDFd5711<bool>* filtro = new TCFEMMIDFd5711<bool>(pm,0,1);

	//aplica novamente o filtro
	filtro->Go(pm);
	//filtro->SetFormato(P5_X_Y_GRAY_BINARY);
	//filtro->NumCores(filtro->MaiorValor());
	//filtro->Path(pm->Path());
	//filtro->Write("result_IDFd5711_2v5i7_640x480_BIN_0-1_new.pgm");

	bool iguais = true;
	int i,j;
	//compara a imagem filtrada com novos valores de INDICE e FUNDO
	#pragma omp parallel for collapse(2) default(shared) private(i,j) //schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			if ( pm2->data2D[i][j] != filtro->data2D[i][j] ) {
				iguais = false;
			}
		}
	}

	//exibe o resultado do teste
	EXPECT_TRUE(iguais);

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}
