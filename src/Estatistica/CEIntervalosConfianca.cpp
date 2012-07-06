
#include <Estatistica/CEIntervalosConfianca.h>

#include <Estatistica/CETStudent.h>

#include <Estatistica/CEChiQuadrado.h>

// Calcula o intervalo de confiança para a média
bool
CEIntervalosConfianca::DeslocamentoMedia (double probabilidade)
{
  double tableT, df, p;

  if (sum < 2)
    return false;

  df = sum - 1.0;

  if (probabilidade > 1.0)
    p = 0.50 - probabilidade / 200.0;
  else
    p = 0.50 - probabilidade / 2.0;

  // chamada antiga
  // tableT = TInv(p, df);
  // novo

  CETStudent student;

  student.df1 = df;

  tableT = student.GoInv (p);

  // fim novo
  deslocamento = tableT * desvioPadrao / sqrt (sum);

  return true;
}

// Calcula o intervalo de confianca para o desvio padrao
bool
CEIntervalosConfianca::DeslocamentoDesvioPadrao (double probabilidade)
{

  double df, p;

  if (sum < 2)
    return false;

  df = sum - 1.0;

  if (probabilidade > 1.0)
    p = 0.50 - probabilidade / 200.0;
  else
    p = 0.50 - probabilidade / 2.0;

  CEChiQuadrado chiQuadrado;

  chiQuadrado.df1 = df;

  minimo = desvioPadrao * sqrt (df / chiQuadrado.GoInv (p));		// usa chi-quadrado

  maximo = desvioPadrao * sqrt (df / chiQuadrado.GoInv (1 - p));	// precisa corrigir

  return true;
}

