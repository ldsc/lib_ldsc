#include <gtest/gtest.h>
#include <omp.h>
#include <string>
#include <Segmentacao/3D/PorosGargantas/CAberturaDilatacao3D.h>
#include "../../../gtest_configure.h"
GtestConfigure cfg3 = GtestConfigure();

TEST(TestSegmentation, OpenningDilatationModel_7) {
	string result_image = "../../data/images/result_SegmentationOpenningDilatation_IBS0105_Model_7.dgm";

	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg3.original3D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	CAberturaDilatacao3D filtro = CAberturaDilatacao3D(pm, 1, 0 );
	filtro.RaioMaximoElementoEstruturante(23);
	filtro.FatorReducaoRaioElemEst(1);
	filtro.IncrementoRaioElementoEstruturante(1);
	filtro.SalvarResultadosParciais(false);

	//Executa o filtro na imagem original
	filtro.Go( SETE );
	if ( cfg3.criarImagensResultantes && cfg3.FileNotExists(result_image) ) {
		filtro.Write(result_image);
	}

	//carrega a imagem previamente result_image com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(result_image);

	//compara a imagem filtrada com o a imagem original
	bool iguais = true;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			for (k = 0; k < pm2->NZ(); k++) {
				if ( pm2->data3D[i][j][k]==0 and (filtro.GetMatrizSitios()->data3D[i][j][k]!=0 or filtro.GetMatrizLigacoes()->data3D[i][j][k]!=0) ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==1 and filtro.GetMatrizSitios()->data3D[i][j][k]!=1 ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==2 and filtro.GetMatrizLigacoes()->data3D[i][j][k]!=1 ) {
					iguais = false;
				}
			}
		}
	}

	//exibe o resultado do teste
	EXPECT_TRUE(iguais) << "Imagem resultante: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
}

TEST(TestSegmentation, OpenningDilatationModel_8) {
	string result_image = "../../data/images/result_SegmentationOpenningDilatation_IBS0105_Model_8.dgm";

	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg3.original3D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	CAberturaDilatacao3D filtro = CAberturaDilatacao3D(pm, 1, 0 );
	filtro.RaioMaximoElementoEstruturante(23);
	filtro.FatorReducaoRaioElemEst(1);
	filtro.IncrementoRaioElementoEstruturante(1);
	filtro.SalvarResultadosParciais(false);

	//Executa o filtro na imagem original
	filtro.Go( OITO );
	if ( cfg3.criarImagensResultantes && cfg3.FileNotExists(result_image) ) {
		filtro.Write(result_image);
	}

	//carrega a imagem previamente result_image com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(result_image);

	//compara a imagem filtrada com o a imagem original
	bool iguais = true;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			for (k = 0; k < pm2->NZ(); k++) {
				if ( pm2->data3D[i][j][k]==0 and (filtro.GetMatrizSitios()->data3D[i][j][k]!=0 or filtro.GetMatrizLigacoes()->data3D[i][j][k]!=0) ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==1 and filtro.GetMatrizSitios()->data3D[i][j][k]!=1 ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==2 and filtro.GetMatrizLigacoes()->data3D[i][j][k]!=1 ) {
					iguais = false;
				}
			}
		}
	}

	//exibe o resultado do teste
	EXPECT_TRUE(iguais) << "Imagem resultante: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
}

TEST(TestSegmentation, OpenningDilatationModel_9) {
	string result_image = "../../data/images/result_SegmentationOpenningDilatation_IBS0105_Model_9.dgm";

	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg3.original3D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	CAberturaDilatacao3D filtro = CAberturaDilatacao3D(pm, 1, 0 );
	filtro.RaioMaximoElementoEstruturante(23);
	filtro.FatorReducaoRaioElemEst(1);
	filtro.IncrementoRaioElementoEstruturante(1);
	filtro.SalvarResultadosParciais(false);

	//Executa o filtro na imagem original
	filtro.Go( NOVE );
	if ( cfg3.criarImagensResultantes && cfg3.FileNotExists(result_image) ) {
		filtro.Write(result_image);
	}

	//carrega a imagem previamente result_image com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(result_image);

	//compara a imagem filtrada com o a imagem original
	bool iguais = true;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			for (k = 0; k < pm2->NZ(); k++) {
				if ( pm2->data3D[i][j][k]==0 and (filtro.GetMatrizSitios()->data3D[i][j][k]!=0 or filtro.GetMatrizLigacoes()->data3D[i][j][k]!=0) ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==1 and filtro.GetMatrizSitios()->data3D[i][j][k]!=1 ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==2 and filtro.GetMatrizLigacoes()->data3D[i][j][k]!=1 ) {
					iguais = false;
				}
			}
		}
	}

	//exibe o resultado do teste
	EXPECT_TRUE(iguais) << "Imagem resultante: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
}

TEST(TestSegmentation, OpenningDilatationModel_10) {
	string result_image = "../../data/images/result_SegmentationOpenningDilatation_IBS0105_Model_10.dgm";

	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg3.original3D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	CAberturaDilatacao3D filtro = CAberturaDilatacao3D(pm, 1, 0 );
	filtro.RaioMaximoElementoEstruturante(23);
	filtro.FatorReducaoRaioElemEst(1);
	filtro.IncrementoRaioElementoEstruturante(1);
	filtro.RaioEEDilatacao(3);
	filtro.SalvarResultadosParciais(false);

	//Executa o filtro na imagem original
	filtro.Go( DEZ );
	if ( cfg3.criarImagensResultantes && cfg3.FileNotExists(result_image) ) {
		filtro.Write(result_image);
	}

	//carrega a imagem previamente result_image com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(result_image);

	//compara a imagem filtrada com o a imagem original
	bool iguais = true;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			for (k = 0; k < pm2->NZ(); k++) {
				if ( pm2->data3D[i][j][k]==0 and (filtro.GetMatrizSitios()->data3D[i][j][k]!=0 or filtro.GetMatrizLigacoes()->data3D[i][j][k]!=0) ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==1 and filtro.GetMatrizSitios()->data3D[i][j][k]!=1 ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==2 and filtro.GetMatrizLigacoes()->data3D[i][j][k]!=1 ) {
					iguais = false;
				}
			}
		}
	}

	//exibe o resultado do teste
	EXPECT_TRUE(iguais) << "Imagem resultante: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
}

TEST(TestSegmentation, OpenningDilatationModel_11) {
	string result_image = "../../data/images/result_SegmentationOpenningDilatation_IBS0105_Model_11.dgm";

	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg3.original3D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	CAberturaDilatacao3D filtro = CAberturaDilatacao3D(pm, 1, 0 );
	filtro.RaioMaximoElementoEstruturante(23);
	filtro.FatorReducaoRaioElemEst(1);
	filtro.IncrementoRaioElementoEstruturante(1);
	filtro.RaioEEDilatacao(3);
	filtro.SalvarResultadosParciais(false);

	//Executa o filtro na imagem original
	filtro.Go( ONZE );
	if ( cfg3.criarImagensResultantes && cfg3.FileNotExists(result_image) ) {
		filtro.Write(result_image);
	}

	//carrega a imagem previamente result_image com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(result_image);

	//compara a imagem filtrada com o a imagem original
	bool iguais = true;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			for (k = 0; k < pm2->NZ(); k++) {
				if ( pm2->data3D[i][j][k]==0 and (filtro.GetMatrizSitios()->data3D[i][j][k]!=0 or filtro.GetMatrizLigacoes()->data3D[i][j][k]!=0) ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==1 and filtro.GetMatrizSitios()->data3D[i][j][k]!=1 ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==2 and filtro.GetMatrizLigacoes()->data3D[i][j][k]!=1 ) {
					iguais = false;
				}
			}
		}
	}

	//exibe o resultado do teste
	EXPECT_TRUE(iguais) << "Imagem resultante: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
}
