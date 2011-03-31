#ifndef CFNaoLinearRzRy_h
#define CFNaoLinearRzRy_h

// -----------------------------------------------------------------------------
//                              BIBLIOTECAS
// -----------------------------------------------------------------------------
// #include <stdio.h>
// #include <stdlib.h>
// #include <malloc.h>
// #include <math.h>

// Bibliotecas do lmpt
// #include "..\lib_lmpt\TSolverBissecao.h"
#include "Base/CFuncao.h"

/*
// ------------------------------------------------------------------------------
// Objeto CFNaoLinearRzRy
// ------------------------------------------------------------------------------
Descrição:
Dado uma funcao autocorrelacao Rz, determina a autocorrelacao Ry
Recebe _numeroPontosSerie no construtor.
A funcao SetParametros é utilizada para passagem de parametros.
A funcao Go  retorna Ry.

A equacao pg35, 3.2:
Rz=Cm^2*Ry^m
Aqui dalcula os coeficientes Cm, e depois determina Ry usando um solver

Programa baseado no codigo desemvolvido por  Liang Zhirong, Florianopolis, May, 1996
Desenvolvido por Andre Duarte Bueno
input/entrada:   	file.cvr    // Covariograma  Cz
porosity
output/saida:  	file.cry    // Covariograma  Cy
Para compilar o programa em Unix: cc -o Rzry Rzry.c NQD.c -lm
*/
/**
 * @brief Representa função filtro não linear RzRy, CFNaoLinearRzRy.
 * @author André Duarte Bueno
 * */
class /*_LIB_LDSC_CLASS*/ CFNaoLinearRzRy : public CFuncao
{
  // Atributos
  double yc; ///< Nível de corte
  double porosidade;	///< Valor da porosidade

  double *cm;	///< é o Bm da equacao 3.20 pagina 35, tese Liang

  unsigned int numeroPontosSerie;	///< Número de pontos usados no somatorio

  bool CoeficientesCmJaCalculados; ///< flag que indica se os coeficientes já foram calculados

  unsigned int gt;	///< indica tipo de gaussiana truncada =1gt1, =2 gt2

  // Metodos
public:

  /// Construtor (por default numeroPontosSerie=35)
CFNaoLinearRzRy (double _yc, double _porosidade, unsigned int _numeroPontosSerie = 35, unsigned int _gt = 1):numeroPontosSerie (_numeroPontosSerie),
    gt    (_gt)
  {
    // cout<<"\nnumeroPontosSerie="<<numeroPontosSerie;

    // cout<<"\nVai criar vetor cm...";

    cm = new double[numeroPontosSerie + 1];

    // cout<<"...cm criada.";

    yc = _yc;

    porosidade = _porosidade;	// rz=_rz;

    CoeficientesCmJaCalculados = false;
  }

  /// Destrutor
  virtual ~ CFNaoLinearRzRy ()
  {
    delete [] cm;
  }

  /// Calcula a função em só 
  virtual double Go (double x, double y = 0);
  
  /// Seta tipo de gaussiana truncada
  void setgt (unsigned int _gt)
  {
    gt = _gt;
  }
};
#endif
