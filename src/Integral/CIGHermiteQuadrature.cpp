// #include <iostream.h>#include <cmath>#include <Integral/CIGHermiteQuadrature.h>// ------------------------------------------------------------------------------// calcula area dos subintervalos, se numeroSubIntervalos é o resultado finaldouble   CIGHermiteQuadrature::Go(double x,double y){//    limiteInferior=y;//    limiteSuperior=x;//    Dx();  resultado=                     0.00453001 * funcao->Go(-2.35060497)                                                  + 0.15706732 * funcao->Go(-1.33584907)                                                  + 0.72462960 * funcao->Go(-0.43607741)                                                  + 0.72462960 * funcao->Go(+0.43607741)                                                  + 0.15706732 * funcao->Go(+1.33584907)                                                  + 0.00453001 * funcao->Go(+2.35060497);  return resultado;}
