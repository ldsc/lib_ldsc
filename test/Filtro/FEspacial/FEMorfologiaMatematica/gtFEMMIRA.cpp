#include <gtest/gtest.h>
#include <omp.h>
#include <string>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIRA.h>
#include "../../../gtest_configure.h"
#include "../../../fixtures.h"

TEST_F(Test2DFilter, IRA_1_0) {
	//variável com nome da imagem a ser comparada
	string result_image = "../../data/images/result_IRA_1_0.pgm";

	//carrega a imagem original
	TCMatriz2D<bool>* pm = new TCMatriz2D<bool>(cfg.original2D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIRA<bool>* filtro = new TCFEMMIRA<bool>(pm,1,0);

	//Executa o filtro na imagem original
	TCMatriz2D<int>* pmfiltrada = filtro->Go( );
	if ( cfg.criarImagensResultantes && cfg.FileNotExists(result_image)) {
		pmfiltrada->SetFormato(P2_X_Y_GRAY_ASCII);
		pmfiltrada->NumCores(pmfiltrada->MaiorValor());
		pmfiltrada->Path("");
		pmfiltrada->Write(result_image);
	}

	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz2D<int>* pm2 = new TCMatriz2D<int>(result_image);

	//exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pmfiltrada ) << "Compared image: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
	delete pmfiltrada;
}

TEST_F(Test2DFilter, IRA_0_1) {
	//variável com nome da imagem a ser comparada
	string result_image = "../../data/images/result_IRA_0_1.pgm";

	//carrega a imagem original
	TCMatriz2D<bool>* pm = new TCMatriz2D<bool>(cfg.original2D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCFEMMIRA<bool>* filtro = new TCFEMMIRA<bool>(pm,0,1);

	//Executa o filtro na imagem original
	TCMatriz2D<int>* pmfiltrada = filtro->Go( );
	if ( cfg.criarImagensResultantes && cfg.FileNotExists(result_image) ) {
		pmfiltrada->SetFormato(P2_X_Y_GRAY_ASCII);
		pmfiltrada->NumCores(pmfiltrada->MaiorValor());
		pmfiltrada->Path("");
		pmfiltrada->Write(result_image);
	}

	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz2D<int>* pm2 = new TCMatriz2D<int>(result_image);

	//exibe o resultado do teste
	EXPECT_TRUE( *pm2 == *pmfiltrada ) << "Compared image: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
	delete pmfiltrada;
}
