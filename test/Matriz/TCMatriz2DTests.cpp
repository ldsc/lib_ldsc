#include <gtest/gtest.h>
#include <Matriz/TCMatriz2D.h>

#include "../gtest_configure.h"
#include "../fixtures.h"

TEST_F(TestMatriz2D, ConstrutorCopia) {
  TCMatriz2D<int>* pm = new TCMatriz2D<int>(cfg.original2D);
  TCMatriz2D<int>* pm2 = new TCMatriz2D<int>(*pm);

  EXPECT_TRUE(*pm == *pm2);
}