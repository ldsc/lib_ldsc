#include <gtest/gtest.h>
#include <omp.h>
#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFd3453D.h>

TEST(FilterTest_IDF_3D, TestIDF3D_1_0) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>("../../data/images/IBS0105_100kv_3.9um_cam4K_0.25d_3602_2f_0.5Al_3200ms_8x100_BIN_1-0_conected.dbm");
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>("../../data/images/result_IDFd345_IBS0105_100kv_3.9um_cam4K_0.25d_3602_2f_0.5Al_3200ms_8x100_BIN_1-0_conected.dgm");
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIDFd3453D<bool>* filtro = new TCFEMMIDFd3453D<bool>(pm,1,0);

	//Executa o filtro na imagem original
	filtro->Go(pm);
	//filtro->SetFormato(D2_X_Y_Z_GRAY_ASCII);
	//filtro->NumCores(filtro->MaiorValor());
	//filtro->Write("result_IDFd345_IBS0105_100kv_3.9um_cam4K_0.25d_3602_2f_0.5Al_3200ms_8x100_BIN_1-0_conected_new.dgm");

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

TEST(FilterTest_IDF_3D, TestIDF3D_0_1) {
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>("../../data/images/IBS0105_100kv_3.9um_cam4K_0.25d_3602_2f_0.5Al_3200ms_8x100_BIN_1-0_conected.dbm");
	//carrega a imagem previamente processada com o filtro IDF sendo indice 0 e fundo 1.
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>("../../data/images/result_IDFd345_IBS0105_100kv_3.9um_cam4K_0.25d_3602_2f_0.5Al_3200ms_8x100_BIN_0-1_conected.dgm");
	//Cria o filtro com poro igual e 0 e fundo igual a 1
	TCFEMMIDFd3453D<bool>* filtro = new TCFEMMIDFd3453D<bool>(pm,0,1);

	//aplica novamente o filtro
	filtro->Go(pm);
	//filtro->SetFormato(D2_X_Y_Z_GRAY_ASCII);
	//filtro->NumCores(filtro->MaiorValor());
	//filtro->Write("result_IDFd345_IBS0105_100kv_3.9um_cam4K_0.25d_3602_2f_0.5Al_3200ms_8x100_BIN_0-1_conected_new.dgm");

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
