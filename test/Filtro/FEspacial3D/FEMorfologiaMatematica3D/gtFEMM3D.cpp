#include <gtest/gtest.h>
#include <omp.h>
#include <string>
#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMorfologiaMatematica3D.h>
#include "../../../gtest_configure.h"
#include "../../../fixtures.h"

TEST_F(Test3DFilter, ErosionMM_R5_1_0) {
	//variável com nome da imagem a ser comparada
	string result_image = "../../data/images/result_ErosionMM_R5_1_0.dbm";

	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg.original3D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMorfologiaMatematica3D<bool>* filtro = new TCFEMorfologiaMatematica3D<bool>(pm,5,1,0);

	//Executa o filtro na imagem original
	//std::cerr << "--1--" << std::endl;
	filtro->Erosao(pm,5);
	//std::cerr << "--2--" << std::endl;
	if ( cfg.criarImagensResultantes && cfg.FileNotExists(result_image)) {
		pm->SetFormato(D1_X_Y_Z_ASCII);
		pm->Path("");
		pm->Write(result_image);
	}

	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>(result_image);

	//exibe o resultado do teste
	EXPECT_TRUE( *pm == *pm2 ) << "Compared image: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST_F(Test3DFilter, DilatationMM_R5_1_0) {
	//variável com nome da imagem a ser comparada
	string result_image = "../../data/images/result_DilatationMM_R5_1_0.dbm";

	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg.original3D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMorfologiaMatematica3D<bool>* filtro = new TCFEMorfologiaMatematica3D<bool>(pm,5,1,0);

	//Executa o filtro na imagem original
	//std::cerr << "--1--" << std::endl;
	filtro->Dilatacao(pm,5);
	//std::cerr << "--2--" << std::endl;
	if ( cfg.criarImagensResultantes && cfg.FileNotExists(result_image)) {
		pm->SetFormato(D1_X_Y_Z_ASCII);
		pm->Path("");
		pm->Write(result_image);
	}

	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>(result_image);

	//exibe o resultado do teste
	EXPECT_TRUE( *pm == *pm2 ) << "Compared image: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}
