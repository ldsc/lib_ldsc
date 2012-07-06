

#include <SFuncao/CSFBissecao.h>

// Determina o valor de x a ser usado na função Go.
// A função Go chama a função Determina_x()

void
CSFBissecao::Determina_x ()
{

  // Determina a posicao de x

  x = (xMenor + xMaior) / 2.0;

}
