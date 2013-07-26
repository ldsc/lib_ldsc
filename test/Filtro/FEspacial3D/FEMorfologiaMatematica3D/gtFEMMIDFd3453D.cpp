#include <gtest/gtest.h>
#include <omp.h>
#include <string>
#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFd3453D.h>
#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFEuclidiana3D.h>

string original = "../../data/images/IBS0105-50x50x50.dbm";
bool criarImagensResultantes = false;

//IDF d345
TEST(Test3DFilter, IDFd3453D_1_0) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>("../../data/images/result_IDFd345_IBS0105_1_0.dgm");
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIDFd3453D<bool>* filtro = new TCFEMMIDFd3453D<bool>(pm,1,0);

	//Executa o filtro na imagem original
	filtro->Go(pm);
	if (criarImagensResultantes) {
		filtro->SetFormato(D5_X_Y_Z_GRAY_BINARY);
		filtro->NumCores(filtro->MaiorValor());
		filtro->Write("result_IDFd345_IBS0105_1_0.dgm");
	}

	//compara a imagem filtrada com o a imagem carregada anteriormente
	bool iguais = true;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			for (k = 0; k < pm2->NZ(); k++) {
				if ( pm2->data3D[i][j][k] != filtro->data3D[i][j][k] ) {
					iguais = false;
					//ver como faz para sair do loop aninhado sem dar problema com a openMP
				}
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

TEST(Test3DFilter, IDFd3453D_0_1) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 0 e fundo 1.
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>("../../data/images/result_IDFd345_IBS0105_0_1.dgm");
	//Cria o filtro com poro igual e 0 e fundo igual a 1
	TCFEMMIDFd3453D<bool>* filtro = new TCFEMMIDFd3453D<bool>(pm,0,1);

	//aplica novamente o filtro
	filtro->Go(pm);
	if (criarImagensResultantes) {
		filtro->SetFormato(D2_X_Y_Z_GRAY_ASCII);
		filtro->NumCores(filtro->MaiorValor());
		filtro->Write("result_IDFd345_IBS0105_0_1.dgm");
	}

	bool iguais = true;
	int i,j,k;
	//compara a imagem filtrada com novos valores de INDICE e FUNDO
	#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			for (k = 0; k < pm2->NZ(); k++) {
				if ( pm2->data3D[i][j][k] != filtro->data3D[i][j][k] ) {
					iguais = false;
				}
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

TEST(Test3DFilter, ErosionByIDFd3453D_1_0) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_ErosionIDFd345_R3_IBS0105_1_0.dbm");
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIDFd3453D<bool>* filtro = new TCFEMMIDFd3453D<bool>(pm,1,0);

	//Executa o filtro na imagem original
	filtro->Go(pm);
	filtro->Erosao(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_ErosionIDFd345_R3_IBS0105_1_0.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(Test3DFilter, ErosionByIDFd3453D_0_1) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 0 e fundo 1.
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_ErosionIDFd345_R3_IBS0105_0_1.dbm");
	//Cria o filtro com poro igual e 0 e fundo igual a 1
	TCFEMMIDFd3453D<bool>* filtro = new TCFEMMIDFd3453D<bool>(pm,0,1);

	//aplica novamente o filtro
	filtro->Go(pm);
	filtro->Erosao(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_ErosionIDFd345_R3_IBS0105_0_1.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(Test3DFilter, DilatationByIDFd3453D_1_0) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_DilatationIDFd345_R3_IBS0105_1_0.dbm");
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIDFd3453D<bool>* filtro = new TCFEMMIDFd3453D<bool>(pm,1,0);

	//Executa o filtro na imagem original
	filtro->Go(pm);
	filtro->Dilatacao(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_DilatationIDFd345_R3_IBS0105_1_0.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(Test3DFilter, DilatationByIDFd3453D_0_1) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 0 e fundo 1.
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_DilatationIDFd345_R3_IBS0105_0_1.dbm");
	//Cria o filtro com poro igual e 0 e fundo igual a 1
	TCFEMMIDFd3453D<bool>* filtro = new TCFEMMIDFd3453D<bool>(pm,0,1);

	//aplica novamente o filtro
	filtro->Go(pm);
	filtro->Dilatacao(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_DilatationIDFd345_R3_IBS0105_0_1.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(Test3DFilter, OpeningByIDFd3453D_1_0) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_OpeningIDFd345_R3_IBS0105_1_0.dbm");
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIDFd3453D<bool>* filtro = new TCFEMMIDFd3453D<bool>(pm,1,0);

	//Executa o filtro na imagem original
	filtro->Go(pm);
	filtro->Abertura(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_OpeningIDFd345_R3_IBS0105_1_0.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(Test3DFilter, OpeningByIDFd3453D_0_1) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 0 e fundo 1.
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_OpeningIDFd345_R3_IBS0105_0_1.dbm");
	//Cria o filtro com poro igual e 0 e fundo igual a 1
	TCFEMMIDFd3453D<bool>* filtro = new TCFEMMIDFd3453D<bool>(pm,0,1);

	//aplica novamente o filtro
	filtro->Go(pm);
	filtro->Abertura(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_OpeningIDFd345_R3_IBS0105_0_1.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(Test3DFilter, ClosingByIDFd3453D_1_0) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_ClosingIDFd345_R3_IBS0105_1_0.dbm");
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIDFd3453D<bool>* filtro = new TCFEMMIDFd3453D<bool>(pm,1,0);

	//Executa o filtro na imagem original
	filtro->Go(pm);
	filtro->Fechamento(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_ClosingIDFd345_R3_IBS0105_1_0.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(Test3DFilter, ClosingByIDFd3453D_0_1) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 0 e fundo 1.
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_ClosingIDFd345_R3_IBS0105_0_1.dbm");
	//Cria o filtro com poro igual e 0 e fundo igual a 1
	TCFEMMIDFd3453D<bool>* filtro = new TCFEMMIDFd3453D<bool>(pm,0,1);

	//aplica novamente o filtro
	filtro->Go(pm);
	filtro->Fechamento(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_ClosingIDFd345_R3_IBS0105_0_1.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

//IDF Euclidiana
TEST(Test3DFilter, IDFEuclidiana_1_0) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>("../../data/images/result_IDFEuclidiana_IBS0105_1_0.dgm");
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIDFEuclidiana3D<bool>* filtro = new TCFEMMIDFEuclidiana3D<bool>(pm,1,0);

	//Executa o filtro na imagem original
	filtro->Go(pm);
	if (criarImagensResultantes) {
		//filtro->SetFormato(D5_X_Y_Z_GRAY_BINARY); // não está funcionando pois a euclidiana está utilizando valores maiores que 255
		filtro->SetFormato(D2_X_Y_Z_GRAY_ASCII);
		filtro->NumCores(filtro->MaiorValor());
		filtro->Write("result_IDFEuclidiana_IBS0105_1_0.dgm");
	}

	//compara a imagem filtrada com o a imagem carregada anteriormente
	bool iguais = true;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			for (k = 0; k < pm2->NZ(); k++) {
				if ( pm2->data3D[i][j][k] != filtro->data3D[i][j][k] ) {
					iguais = false;
					//ver como faz para sair do loop aninhado sem dar problema com a openMP
				}
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

TEST(Test3DFilter, IDFEuclidiana_0_1) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 0 e fundo 1.
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>("../../data/images/result_IDFEuclidiana_IBS0105_0_1.dgm");
	//Cria o filtro com poro igual e 0 e fundo igual a 1
	TCFEMMIDFEuclidiana3D<bool>* filtro = new TCFEMMIDFEuclidiana3D<bool>(pm,0,1);

	//aplica novamente o filtro
	filtro->Go(pm);
	if (criarImagensResultantes) {
		filtro->SetFormato(D2_X_Y_Z_GRAY_ASCII);
		filtro->NumCores(filtro->MaiorValor());
		filtro->Write("result_IDFEuclidiana_IBS0105_0_1.dgm");
	}

	bool iguais = true;
	int i,j,k;
	//compara a imagem filtrada com novos valores de INDICE e FUNDO
	#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			for (k = 0; k < pm2->NZ(); k++) {
				if ( pm2->data3D[i][j][k] != filtro->data3D[i][j][k] ) {
					iguais = false;
				}
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

TEST(Test3DFilter, ErosionByIDFEuclidiana_1_0) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_ErosionIDFEuclidiana_R3_IBS0105_1_0.dbm");
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIDFEuclidiana3D<bool>* filtro = new TCFEMMIDFEuclidiana3D<bool>(pm,1,0);

	//Executa o filtro na imagem original
	filtro->Go(pm);
	filtro->Erosao(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_ErosionIDFEuclidiana_R3_IBS0105_1_0.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(Test3DFilter, ErosionByIDFEuclidiana_0_1) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 0 e fundo 1.
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_ErosionIDFEuclidiana_R3_IBS0105_0_1.dbm");
	//Cria o filtro com poro igual e 0 e fundo igual a 1
	TCFEMMIDFEuclidiana3D<bool>* filtro = new TCFEMMIDFEuclidiana3D<bool>(pm,0,1);

	//aplica novamente o filtro
	filtro->Go(pm);
	filtro->Erosao(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_ErosionIDFEuclidiana_R3_IBS0105_0_1.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(Test3DFilter, DilatationByIDFEuclidiana_1_0) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_DilatationIDFEuclidiana_R3_IBS0105_1_0.dbm");
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIDFEuclidiana3D<bool>* filtro = new TCFEMMIDFEuclidiana3D<bool>(pm,1,0);

	//Executa o filtro na imagem original
	filtro->Go(pm);
	filtro->Dilatacao(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_DilatationIDFEuclidiana_R3_IBS0105_1_0.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(Test3DFilter, DilatationByIDFEuclidiana_0_1) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 0 e fundo 1.
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_DilatationIDFEuclidiana_R3_IBS0105_0_1.dbm");
	//Cria o filtro com poro igual e 0 e fundo igual a 1
	TCFEMMIDFEuclidiana3D<bool>* filtro = new TCFEMMIDFEuclidiana3D<bool>(pm,0,1);

	//aplica novamente o filtro
	filtro->Go(pm);
	filtro->Dilatacao(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_DilatationIDFEuclidiana_R3_IBS0105_0_1.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(Test3DFilter, OpeningByIDFEuclidiana_1_0) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_OpeningIDFEuclidiana_R3_IBS0105_1_0.dbm");
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIDFEuclidiana3D<bool>* filtro = new TCFEMMIDFEuclidiana3D<bool>(pm,1,0);

	//Executa o filtro na imagem original
	filtro->Go(pm);
	filtro->Abertura(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_OpeningIDFEuclidiana_R3_IBS0105_1_0.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(Test3DFilter, OpeningByIDFEuclidiana_0_1) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 0 e fundo 1.
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_OpeningIDFEuclidiana_R3_IBS0105_0_1.dbm");
	//Cria o filtro com poro igual e 0 e fundo igual a 1
	TCFEMMIDFEuclidiana3D<bool>* filtro = new TCFEMMIDFEuclidiana3D<bool>(pm,0,1);

	//aplica novamente o filtro
	filtro->Go(pm);
	filtro->Abertura(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_OpeningIDFEuclidiana_R3_IBS0105_0_1.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(Test3DFilter, ClosingByIDFEuclidiana_1_0) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_ClosingIDFEuclidiana_R3_IBS0105_1_0.dbm");
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIDFEuclidiana3D<bool>* filtro = new TCFEMMIDFEuclidiana3D<bool>(pm,1,0);

	//Executa o filtro na imagem original
	filtro->Go(pm);
	filtro->Fechamento(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_ClosingIDFEuclidiana_R3_IBS0105_1_0.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST(Test3DFilter, ClosingByIDFEuclidiana_0_1) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(original);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 0 e fundo 1.
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>("../../data/images/result_ClosingIDFEuclidiana_R3_IBS0105_0_1.dbm");
	//Cria o filtro com poro igual e 0 e fundo igual a 1
	TCFEMMIDFEuclidiana3D<bool>* filtro = new TCFEMMIDFEuclidiana3D<bool>(pm,0,1);

	//aplica novamente o filtro
	filtro->Go(pm);
	filtro->Fechamento(pm,3);
	if (criarImagensResultantes) {
		pm->SetFormato(D4_X_Y_Z_BINARY);
		pm->Write("result_ClosingIDFEuclidiana_R3_IBS0105_0_1.dbm");
	}

	//compara e exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pm );

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}
