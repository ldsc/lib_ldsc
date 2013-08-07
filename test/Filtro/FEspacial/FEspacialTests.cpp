#include <gtest/gtest.h>
#include <iostream>
#include "Filtro/FEspacial/TCFEPassaBaixa.h"
#include "Filtro/FEspacial/TCFEPassaAlta.h"

#include "Matriz/TCMatriz2D.h"
#include "../../gtest_configure.h"
#include "../../fixtures.h"

TEST_F(Test2DFilter, LowPass) {
  std::string result_image("../../data/images/result_low_pass.pbm");
  TCMatriz2D<int>* pm = new TCMatriz2D<int>(cfg.original2D);
  TCFiltro<int>* filter = new TCFEPassaBaixa<int>(pm, 5);

  filter->Go(pm, 5);

	if ( cfg.criarImagensResultantes && cfg.FileNotExists(result_image) ) {
		pm->SetFormato(P1_X_Y_ASCII);
    pm->Path("");
    pm->Write(result_image);
  }

  TCMatriz2D<int>* pm2 = new TCMatriz2D<int>(result_image);

  EXPECT_TRUE(*pm == *pm2);
}

TEST_F(Test2DFilter, TCFEspacial) {
  std::string result_image("../../data/images/result_espacial.pbm");
  TCMatriz2D<int>* pm = new TCMatriz2D<int>(cfg.original2D);
  TCFEspacial<int>* filter = new TCFEspacial<int>(pm, 5);

  filter->Go(pm, 5);

	if ( cfg.criarImagensResultantes && cfg.FileNotExists(result_image) ) {
		pm->SetFormato(P1_X_Y_ASCII);
    pm->Path("");
    pm->Write(result_image);
  }

  TCMatriz2D<int>* pm2 = new TCMatriz2D<int>(result_image);

  EXPECT_TRUE(*pm == *pm2);
}
