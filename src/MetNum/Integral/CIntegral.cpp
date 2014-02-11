
#include <MetNum/Integral/CIntegral.h>

#include <Base/CSolver.h>

// Definir todos os elementos a serem utilizados na integração

CIntegral::CIntegral (CFuncao * _funcao)
 : CSolver ()
{
  funcao = _funcao;

  // limiteInferior=0; // 0  Passados para a funcao Go
  // limiteSuperior=1;// 1

  numeroPontos = 100;

  // Dx();// é chamada pela Go
}
