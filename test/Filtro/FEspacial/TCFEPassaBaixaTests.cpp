#include <gtest/gtest.h>
#include <iostream>
#include "Filtro/FEspacial/TCFEPassaBaixa.h"
#include "Matriz/TCMatriz2D.h"
#include "../../gtest_configure.h"
#include "../../fixtures.h"

TEST_F(Test2DFilter, LowPass) {
  std::string result_image("../../data/images/result_low_pass.pbm");
  TCMatriz2D<int>* pm = new TCMatriz2D<int>(cfg.original2D);
  TCFiltro<int>* filtro = new TCFEPassaBaixa<int>(pm,5);

  filtro->Go(pm, 5);

  if (cfg.criarImagensResultantes) {
    pm->SetFormato(P4_X_Y_BINARY);
    pm->Path("");
    bool x = pm->Write(result_image);
  }

  TCMatriz2D<int>* pm2 = new TCMatriz2D<int>(result_image);

  EXPECT_TRUE(*pm == *pm2);
}