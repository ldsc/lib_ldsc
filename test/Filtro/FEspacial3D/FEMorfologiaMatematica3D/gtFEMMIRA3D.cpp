#include <gtest/gtest.h>
#include <omp.h>
#include <string>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIRA3D.h>
#include "../../../gtest_configure.h"
#include "../../../fixtures.h"

TEST_F(Test3DFilter, IRA_1_0) {
	//variável com nome da imagem a ser comparada
	string result_image = "../../data/images/result_IRA_1_0.dgm";

	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg.original3D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIRA3D<bool>* filtro = new TCFEMMIRA3D<bool>(pm,1,0);

	//Executa o filtro na imagem original
	TCMatriz3D<int>* pmfiltrada = filtro->Go( );
	if ( cfg.criarImagensResultantes && cfg.FileNotExists(result_image)) {
		pmfiltrada->SetFormato(D2_X_Y_Z_GRAY_ASCII);
		pmfiltrada->NumCores(pmfiltrada->MaiorValor());
		pmfiltrada->Path("");
		pmfiltrada->Write(result_image);
	}

	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(result_image);

	//exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pmfiltrada ) << "Compared image: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
	delete pmfiltrada;
}

TEST_F(Test3DFilter, IRA_0_1) {
	//variável com nome da imagem a ser comparada
	string result_image = "../../data/images/result_IRA_0_1.dgm";

	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg.original3D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIRA3D<bool>* filtro = new TCFEMMIRA3D<bool>(pm,0,1);

	//Executa o filtro na imagem original
	TCMatriz3D<int>* pmfiltrada = filtro->Go( );
	if ( cfg.criarImagensResultantes && cfg.FileNotExists(result_image) ) {
		pmfiltrada->SetFormato(D2_X_Y_Z_GRAY_ASCII);
		pmfiltrada->NumCores(pmfiltrada->MaiorValor());
		pmfiltrada->Path("");
		pmfiltrada->Write(result_image);
	}

	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(result_image);

	//exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pmfiltrada ) << "Compared image: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
	delete pmfiltrada;
}
