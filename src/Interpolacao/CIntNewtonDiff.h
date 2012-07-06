
#ifndef CIntNewtonDiff_h
#define CIntNewtonDiff_h

#include <Interpolacao/CIntNewtonDivDiff.h>
/*
--------------------------------------------------------------------
                          Classe CIntNewtonDiff
--------------------------------------------------------------------
*/
/**
 * @brief Implementa interpolação das diferenças de Newton.
 * 
 * A classe CInterpolação e a classe CIntNewtonDiff tem funções
 * 
 * virtual double Go(double* xarr, double* yarr,int n, double x)
 * Aqui, é mais simplres, Não necessita do vetor xarr, apenas do primeiro
 * valor x0 e do deslocamento h.
 * 
 * Para deixar uniforme o uso, criei a função Go2 e GeraTabelaAuxiliar2
 * que calculam as coisas do jeito do CIntNewtonDiff.
 * Criei ainda uma
 * 
 * virtual double Go(double* xarr, double* yarr,int n, double x)
 * 
 * para satisfazer a classe base, observe que pode-se passar x0 e h
 * dentro do vetor xarr fazendo xarr[0]=x0 e xarr[1]=x0+h
*/
class CIntNewtonDiff : public CIntNewtonDivDiff
{
private:
  /** @brief Função que calcula a tabela auxiliar
   * A função GeraTabelaAuxiliar desta classe é diferente da da classe CIntNewtonDivDiff
   * aqui recebe um double h lê um double*xarr, o calculo também muda. 
  */
  void GeraTabelaAuxiliar (double h, double *yarr, int n);

  // Como os parâmetros recebidos pela função original são diferentes daquelas adotados
  // como default para a funcao Go da hierarquia,
  // criei a função Go2 que recebe os mesmos parametros da função original.
  // double Go2(double x0, double h, double* yarr,int n, double x);

public:
  /// Realiza a interpolação.
    virtual double Go (double *xarr, double *yarr, int n, double x);

  /// Função estática pode ser chamada diretamente com todos os parâmetros
  /// é a função original do C.Shammas
  static double NewtonDiff (double x0, double h, double *yarr,
			    double *diffTable, int buildMatFlag, int n,
			    double x);
  /*
     {
     CIntNewtonDiff* interpolacao = new CIntNewtonDiff();
     assert(CIntNewtonDiff);
     interpolacao->GeraTabelaAuxiliar2( h,  yarr, n);
     double result = interpolacao->Go(xarr,yarr,n,x);
     delete interpolacao;
     return       result;
     };
   */
};

#endif

