
#include <Mascara/CMSobelHorizontal.h>

//mascara horizontal
// -1  0  1
// -2  0  2
// -1  0  1

void CMSobelHorizontal::PreencheMascara () {
  data2D[0][0] = -1;
  data2D[0][1] = 0;
  data2D[0][2] = 1;
  data2D[1][0] = -2;
  data2D[1][1] = 0;
  data2D[1][2] = 2;
  data2D[2][0] = -1;
  data2D[2][1] = 0;
  data2D[2][2] = 1;

     
   peso = 1;  //   armazena no atributo peso
}
