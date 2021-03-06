#include <gtest/gtest.h>
#include <omp.h>
#include <string>
#include <AnaliseImagem/Caracterizacao/RedePercolacao/CRedeDePercolacao.h>
#include "../../gtest_configure.h"
GtestConfigure cfg5 = GtestConfigure();

/*
TEST(TestCaracterizacao, RedeDePercolacao) {
	string result_image = "../../data/images/result_CaracterizacaoRedeDePercolacao.dbm";

	//carrega a imagem original
	TCImagem3D<bool>* pm = new TCImagem3D<bool>(cfg5.original3D);

	//Cria objeto rede
	CRedeDePercolacao rede = CRedeDePercolacao(100, 100, 100);

	//Segmenta a imagem, calcula as distribuicoes e cria a rede
	srand(1); //faz com que a função rand gere sempre os mesmos números rândomicos
	rede.Go(pm,23,2,1,1,ONZE,1,0,CDistribuicao3D::d345 );
	rede.SalvarListaObjetos("../../data/result_CaracterizacaoRedeDePercolacao.rsl");
	rede.SalvarListaObjetosGrafo("../../data/result_CaracterizacaoRedeDePercolacao.grafo.txt");

	if ( cfg5.criarImagensResultantes && cfg5.FileNotExists(result_image) ) {
		rede.pm->Write(result_image);
	}

	//carrega a result_image.
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>(result_image);

	//compara a imagem filtrada com o a imagem original e exibe o resultado do teste
	EXPECT_TRUE(*(rede.pm) == *pm2) << "Imagem resultante: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
}

TEST(TestCaracterizacao, RedeDePercolacaoPhiCalc) {
	string result_image = "../../data/images/result_CaracterizacaoRedeDePercolacaoPhiCalc.dbm";

	//carrega a imagem original
	TCImagem3D<bool>* pm = new TCImagem3D<bool>(cfg5.original3D);

	//Cria objeto rede
	CRedeDePercolacao rede = CRedeDePercolacao(100, 100, 100);
	rede.calcPixeis = true;

	//Segmenta a imagem, calcula as distribuicoes e cria a rede
	srand(1); //faz com que a função rand gere sempre os mesmos números rândomicos
	rede.Go( pm,23,2,1,1,ONZE,1,0,CDistribuicao3D::d345 );
	rede.SalvarListaObjetos("../../data/result_CaracterizacaoRedeDePercolacaoPhiCalc.rsl");
	rede.SalvarListaObjetosGrafo("../../data/result_CaracterizacaoRedeDePercolacaoPhiCalc.grafo.txt");

	if ( cfg5.criarImagensResultantes && cfg5.FileNotExists(result_image) ) {
		rede.pm->Write(result_image);
	}

	//carrega a result_image.
	TCMatriz3D<bool>* pm2 = new TCMatriz3D<bool>(result_image);

	//compara a imagem filtrada com o a imagem original e exibe o resultado do teste
	EXPECT_TRUE(*(rede.pm) == *pm2) << "Imagem resultante: " << result_image;

	//deleta ponteiros
	delete pm;
	delete pm2;
}
*/
