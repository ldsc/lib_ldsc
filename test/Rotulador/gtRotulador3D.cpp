#include <gtest/gtest.h>
#include <omp.h>
#include <string>
#include <ostream>
#include <AnaliseImagem/Filtro/FEspacial/FERotulagem/TCRotulador3D.h>
#include <AnaliseImagem/Filtro/FEspacial/FERotulagem/CRotuladorIRA3D.h>
#include <AnaliseImagem/Matriz/TCMatriz3D.h>
#include "../gtest_configure.h"
#include "../fixtures.h"

TEST_F(Test3DLabeler, Rotulada_1_0) {
	//variável com nome da imagem a ser comparada
	string result_image = "../../data/images/result_Rotulada_1_0.dgm";

	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg.original3D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCRotulador3D<bool>* filtro = new TCRotulador3D<bool>(pm,1,0);

	//Executa o filtro na imagem original
	filtro->Go( pm );
	if ( cfg.criarImagensResultantes && cfg.FileNotExists(result_image)) {
		filtro->SetFormato(D2_X_Y_Z_GRAY_ASCII);
		filtro->NumCores(filtro->MaiorValor());
		filtro->Path("");
		filtro->Write(result_image);
	}

	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(result_image);

	//compara a imagem filtrada com o a imagem carregada anteriormente
	bool iguais = true;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); ++i) {
		for (j = 0; j < pm2->NY(); ++j) {
			for (k = 0; k < pm2->NZ(); ++k) {
				if ( pm2->data3D[i][j][k] != filtro->data3D[i][j][k] ) {
					iguais = false;
					//ver como faz para sair do loop aninhado sem dar problema com a openMP
				}
			}
		}
	}

	//exibe o resultado do teste
	EXPECT_TRUE(iguais) << "Compared image: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST_F(Test3DLabeler, Rotulada_0_1) {
	//variável com nome da imagem a ser comparada
	string result_image = "../../data/images/result_Rotulada_0_1.dgm";

	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg.original3D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	TCRotulador3D<bool>* filtro = new TCRotulador3D<bool>(pm,0,1);

	//Executa o filtro na imagem original
	filtro->Go( pm );
	if ( cfg.criarImagensResultantes && cfg.FileNotExists(result_image) ) {
		filtro->SetFormato(D2_X_Y_Z_GRAY_ASCII);
		filtro->NumCores(filtro->MaiorValor());
		filtro->Path("");
		filtro->Write(result_image);
	}

	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(result_image);

	//compara a imagem filtrada com o a imagem carregada anteriormente
	bool iguais = true;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); ++i) {
		for (j = 0; j < pm2->NY(); ++j) {
			for (k = 0; k < pm2->NZ(); ++k) {
				if ( pm2->data3D[i][j][k] != filtro->data3D[i][j][k] ) {
					iguais = false;
					//ver como faz para sair do loop aninhado sem dar problema com a openMP
				}
			}
		}
	}

	//exibe o resultado do teste
	EXPECT_TRUE(iguais) << "Compared image: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
	delete filtro;
}

TEST_F(Test3DLabeler, RotuladaByIRA) {
	string result_image;

	//Utiliza como original a IRA criada pelo teste Test3DFilter.IRA_1_0
	string original3D = "../../data/images/result_IRA_1_0.dgm";

	//carrega a imagem original
	TCMatriz3D<int>* pm = new TCMatriz3D<int>(original3D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	CRotuladorIRA3D* filtro = new CRotuladorIRA3D(pm);

	for ( int r = -15; r <= 20; r+=5 ) {
		//variável com nome da imagem a ser comparada
		ostringstream os;
		os << "../../data/images/result_RotuladaByIRA_r" << r << ".dgm";
		result_image = os.str();

		//Executa o filtro na imagem original
		EXPECT_TRUE( filtro->Go( pm, r ) ) << "Método comparado: filtro->Go( pm, r )";
		if ( cfg.criarImagensResultantes && cfg.FileNotExists(result_image) ) {
			filtro->SetFormato(D2_X_Y_Z_GRAY_ASCII);
			filtro->NumCores(filtro->MaiorValor());
			filtro->Path("");
			filtro->Write(result_image);
		}

		//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
		TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(result_image);

		//compara a imagem filtrada com o a imagem carregada anteriormente
		int i,j,k;
		bool iguais = true;
	#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
		for (i = 0; i < pm2->NX(); ++i) {
			for (j = 0; j < pm2->NY(); ++j) {
				for (k = 0; k < pm2->NZ(); ++k) {
					if ( pm2->data3D[i][j][k] != filtro->data3D[i][j][k] ) {
						iguais = false;
						//ver como faz para sair do loop aninhado sem dar problema com a openMP
					}
				}
			}
		}

		//exibe o resultado do teste
		EXPECT_TRUE(iguais) << "Compared image: " << result_image;
		delete pm2;
	}
	//deleta ponteiros
	delete pm;
	delete filtro;
}
