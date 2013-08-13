
#include <Mascara/CMSobelVertical.h>

//mascara horizontal
//  1   2   1
//  0   0   0
// -1  -2  -1

void CMSobelVertical::PreencheMascara () {
  data2D[0][0] = 1;
  data2D[0][1] = 2;
  data2D[0][2] = 1;
  data2D[1][0] = 0;
  data2D[1][1] = 0;
  data2D[1][2] = 0;
  data2D[2][0] = -1;
  data2D[2][1] = -2;
  data2D[2][2] = -1;
     
   peso = 1;  //   armazena no atributo peso
}
