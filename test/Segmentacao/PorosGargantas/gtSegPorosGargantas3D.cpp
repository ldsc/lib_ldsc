#include <gtest/gtest.h>
#include <omp.h>
#include <string>
#include <AnaliseImagem/Segmentacao/PorosGargantas/CSegPorosGargantas3D.h>
#include "../../gtest_configure.h"
GtestConfigure cfg4 = GtestConfigure();

TEST(TestSegmentation, SegPoresThroatsModel_0) {
	string result_image = "../../data/images/result_SegmentationPoresThroats_IBS0105_Model_0.dgm";

	//carrega a imagem original
	TCMatriz3D<bool>* pm = new TCMatriz3D<bool>(cfg4.original3D);

	//Cria o filtro com poro igual e 1 e fundo igual a 0
	CSegPorosGargantas3D filtro = CSegPorosGargantas3D(pm, 1, 0 );
	filtro.RaioMaximoElementoEstruturante(23);
	filtro.FatorReducaoRaioElemEst(1);
	filtro.IncrementoRaioElementoEstruturante(1);
	filtro.SalvarResultadosParciais(false);

	//Executa o filtro na imagem original
	filtro.Go(0);
	if ( cfg4.criarImagensResultantes && cfg4.FileNotExists(result_image) ) {
		filtro.Write(result_image);
	}

	//carrega a imagem resultante previamente criada
	TCMatriz3D<int>* pm2 = new TCMatriz3D<int>(result_image);

	//compara a imagem filtrada com o a imagem carregada anteriormente
	bool iguais = true;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) schedule(dynamic,10)
	for (i = 0; i < pm2->NX(); i++) {
		for (j = 0; j < pm2->NY(); j++) {
			for (k = 0; k < pm2->NZ(); k++) {
				if ( pm2->data3D[i][j][k]==0 and (filtro.MatrizSitios()->data3D[i][j][k]!=0 or filtro.MatrizLigacoes()->data3D[i][j][k]!=0) ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==1 and filtro.MatrizSitios()->data3D[i][j][k]!=1 ) {
					iguais = false;
				} else if ( pm2->data3D[i][j][k]==2 and filtro.MatrizLigacoes()->data3D[i][j][k]!=1 ) {
					iguais = false;
				}
			}
		}
	}

	//exibe o resultado do teste
	EXPECT_TRUE(iguais) << "Compared image: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;

}
