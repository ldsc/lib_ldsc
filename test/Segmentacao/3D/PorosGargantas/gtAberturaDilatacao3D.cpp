#include <gtest/gtest.h>
#include <omp.h>
#include <string>
#include <Segmentacao/3D/PorosGargantas/CAberturaDilatacao3D.h>
#include "../../../gtest_configure.h"
GtestConfigure cfg3 = GtestConfigure();

TEST(TestSegmentation, OpenningDilatationModel_7) {
	string processada = "../../data/images/result_SegmentationOpenningDilatation_IBS0105_Model_7.dgm";
	pair<TCMatriz3D<bool>*,TCMatriz3D<bool>*> pms;
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg3.original3D);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(processada);
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	CAberturaDilatacao3D filtro = CAberturaDilatacao3D(pm, processada, 1, 0 );
	filtro.RaioMaximoElementoEstruturante(23);
	filtro.FatorReducaoRaioElemEst(1);
	filtro.IncrementoRaioElementoEstruturante(1);
	filtro.SalvarResultadosParciais(false);

	//Executa o filtro na imagem original
	pms = filtro.DistSitiosLigacoes_Modelo_7();
	if (cfg3.criarImagensResultantes) {
		filtro.Write(processada, pms.first, pms.second);
	}

	//compara a imagem filtrada com o a imagem carregada anteriormente
	bool iguais = true;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			for (k = 0; k < pm2->NZ(); k++) {
				if ( pm2->data3D[i][j][k]==0 and (pms.first->data3D[i][j][k]!=0 or pms.second->data3D[i][j][k]!=0) ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==1 and pms.first->data3D[i][j][k]!=1 ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==2 and pms.second->data3D[i][j][k]!=1 ) {
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
	delete pms.first;
	delete pms.second;
}

TEST(TestSegmentation, OpenningDilatationModel_6) {
	string processada = "../../data/images/result_SegmentationOpenningDilatation_IBS0105_Model_6.dgm";
	pair<TCMatriz3D<bool>*,TCMatriz3D<bool>*> pms;
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg3.original3D);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(processada);
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	CAberturaDilatacao3D filtro = CAberturaDilatacao3D(pm, processada, 1, 0 );
	filtro.RaioMaximoElementoEstruturante(23);
	filtro.FatorReducaoRaioElemEst(1);
	filtro.IncrementoRaioElementoEstruturante(1);
	filtro.SalvarResultadosParciais(false);

	//Executa o filtro na imagem original
	pms = filtro.DistSitiosLigacoes_Modelo_6();
	if (cfg3.criarImagensResultantes) {
		filtro.Write(processada, pms.first, pms.second);
	}

	//compara a imagem filtrada com o a imagem carregada anteriormente
	bool iguais = true;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			for (k = 0; k < pm2->NZ(); k++) {
				if ( pm2->data3D[i][j][k]==0 and (pms.first->data3D[i][j][k]!=0 or pms.second->data3D[i][j][k]!=0) ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==1 and pms.first->data3D[i][j][k]!=1 ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==2 and pms.second->data3D[i][j][k]!=1 ) {
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
	delete pms.first;
	delete pms.second;
}

TEST(TestSegmentation, OpenningDilatationModel_5) {
	string processada = "../../data/images/result_SegmentationOpenningDilatation_IBS0105_Model_5.dgm";
	pair<TCMatriz3D<bool>*,TCMatriz3D<bool>*> pms;
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg3.original3D);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(processada);
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	CAberturaDilatacao3D filtro = CAberturaDilatacao3D(pm, processada, 1, 0 );
	filtro.RaioMaximoElementoEstruturante(23);
	filtro.FatorReducaoRaioElemEst(1);
	filtro.IncrementoRaioElementoEstruturante(1);
	filtro.SalvarResultadosParciais(false);

	//Executa o filtro na imagem original
	pms = filtro.DistSitiosLigacoes_Modelo_5();
	if (cfg3.criarImagensResultantes) {
		filtro.Write(processada, pms.first, pms.second);
	}

	//compara a imagem filtrada com o a imagem carregada anteriormente
	bool iguais = true;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			for (k = 0; k < pm2->NZ(); k++) {
				if ( pm2->data3D[i][j][k]==0 and (pms.first->data3D[i][j][k]!=0 or pms.second->data3D[i][j][k]!=0) ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==1 and pms.first->data3D[i][j][k]!=1 ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==2 and pms.second->data3D[i][j][k]!=1 ) {
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
	delete pms.first;
	delete pms.second;
}

TEST(TestSegmentation, OpenningDilatationModel_4) {
	string processada = "../../data/images/result_SegmentationOpenningDilatation_IBS0105_Model_4.dgm";
	pair<TCMatriz3D<bool>*,TCMatriz3D<bool>*> pms;
	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg3.original3D);
	//carrega a imagem previamente processada com o filtro IDF sendo indice 1 e fundo 0
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(processada);
	//Cria o filtro com poro igual e 1 e fundo igual a 0
	CAberturaDilatacao3D filtro = CAberturaDilatacao3D(pm, processada, 1, 0 );
	filtro.RaioMaximoElementoEstruturante(23);
	filtro.FatorReducaoRaioElemEst(1);
	filtro.IncrementoRaioElementoEstruturante(1);
	filtro.SalvarResultadosParciais(false);

	//Executa o filtro na imagem original
	pms = filtro.DistSitiosLigacoes_Modelo_4();
	if (cfg3.criarImagensResultantes) {
		filtro.Write(processada, pms.first, pms.second);
	}

	//compara a imagem filtrada com o a imagem carregada anteriormente
	bool iguais = true;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			for (k = 0; k < pm2->NZ(); k++) {
				if ( pm2->data3D[i][j][k]==0 and (pms.first->data3D[i][j][k]!=0 or pms.second->data3D[i][j][k]!=0) ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==1 and pms.first->data3D[i][j][k]!=1 ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==2 and pms.second->data3D[i][j][k]!=1 ) {
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
	delete pms.first;
	delete pms.second;
}
