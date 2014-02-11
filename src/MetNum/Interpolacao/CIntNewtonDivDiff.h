
#ifndef CIntNewtonDivDiff_h
#define CIntNewtonDivDiff_h
#include <MetNum/Interpolacao/CInterpolacao.h>

/*
--------------------------------------------------------------------
                          Classe CIntNewtonDivDiff
--------------------------------------------------------------------
*/
/**
 * @brief Calcula interpolação usando diferenças divididas de Newton.
 * 
 * Usa expansão de Taylor.
 * Faz aproximação das várias derivadas e e abilita o método a trabalhar com
 * com dados não equidistantes.
 * O método das diferenças divididas de primeira ordem aproxima as derivadas de primeira ordem,
 * o método das diferenças divididas de segunda  ordem aproxima as derivadas de segunda  ordem,
 * e assim por diante.
 * x0	y0	D0	D02	D03	D04
 * x1	y1	D1	D12	D13
 * x2	y2	D2	D22
 * x3	y3	D3
 * x4	y4
 * sendo:
 * D0 = (y1-y0)   / (x1-x0)
 * D02= (D1-D0)   / (x1-x0)
 * D03= (D12-D02) / (x1-x0)
 * e assim por diante.
 * O que gera uma matriz triangular, que pode ser salva em vetores (para economizar espaço).
 * Requer:
 * A matriz x,y  uma tabela (para armazenar x,y e a diferença), o número de elementos nx,ny,
 *
 * flags de construção da tabela, e os valores interpolados de x ->xint.
*/
class CIntNewtonDivDiff:public CInterpolacao
{
	
//Atributos
protected:
	
  int buildMatFlag;		///< Flag de calculo da  tabela auxiliar diffTable
  double *diffTable;		///< Tabela auxiliar  (vetor)

//Métodos

public:
  /// Construtor
    CIntNewtonDivDiff ():buildMatFlag (1), diffTable (0)
  {
  }

  /// Destrutor
  virtual ~ CIntNewtonDivDiff ()
  {
    if (diffTable)
      delete [] diffTable;
  }

  /** @brief Função que calcula a tabela auxiliar
   *  diferente da da classe base, recebe um xarr e não um double h
   * por isso não é virtual.
   * */
  void GeraTabelaAuxiliar (double *xarr, double *yarr, int n);

  
  /// Calcula a interpolação.
  virtual double Go (double *xarr, double *yarr, int n, double x);

  /**
   * @brief Função estática pode ser chamada diretamente com todos os parâmetros,
   é a função original do C.Shammas.
   * */
  static double NewtonDivDiff(double *xarr, double *yarr, double *diffTable, int buildMatFlag, int n, double x);
};

#endif
