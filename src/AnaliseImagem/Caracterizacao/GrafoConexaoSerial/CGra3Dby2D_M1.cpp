
/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
                  Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:          André Duarte Bueno
@file:             CGra3Dby2_M1.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cassert>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef CMath_h
#include <Base/CMath.h>
#endif

#ifndef CGra3Dby2D_M1_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGra3Dby2D_M1.h>
#endif

using namespace std;

/*
-------------------------------------------------------------------------
Função:   DeterminarConeccoesObjetos
-------------------------------------------------------------------------
@short  : DeterminarConeccoesObjetos entre planos consecutivos.
          Nesta versão compara-se diretamente os planos bidimensionais
          rotulados (ra e rp).
          A comparação é feita píxel a pixel,
          de forma que duas regiões conexas vão gerar tantas conecções
          quanto forem o número de píxeis da intersecção das duas regioes.
          Ou seja se a região_A do plano ra tiver 5 píxeis conectados
          a região_B do plano rp
          vai criar 5 links entre os objetos.
@author :	André Duarte Bueno
@see    : grafos
@param  : Rótulo do maior rótulo já utilizado no grafo
@return : void
@version:
*/
void
CGra3Dby2D_M1::
DeterminarConeccoesObjetos (unsigned long int maiorRotuloUtilizado)
{
  // Variáveis auxiliares (rotulos dos planos ra e rp)
  unsigned long int pa, pp;

  // Percorre os planos de rotulagem
  for (unsigned long int i = 0; i < ra->NX (); i++)
    for (unsigned long int j = 0; j < ra->NY (); j++)
      {
	// Armazena o rotulo dos objetos
	pa = ra->data2D[i][j];
	pp = rp->data2D[i][j];

	// Verifica se estão conectados (ambos > 0)
	if (pa && pp)
	  {
	    // No plano ra, soma o maiorRotuloUtilizado
	    pa += maiorRotuloUtilizado;

	    // No plano rp, soma o maiorRotuloUtilizado + ra->RotuloFinal()
	    pp += maiorRotuloUtilizado + ra->RotuloFinal ();

	    // Cada sítio tem uma lista das conecções
	    // Adiciona nos dois sítios a informação do link
	    // ou seja, conecta os sítios.
	    // PS: pa-1 porque o sítio de rótulo 1 esta na posição [0]

	    // Informa pa-1 que esta conectado a pp-1
	    objeto[pa - 1]->Conectar (objeto[pp - 1]);

	    // Informa pp-1 que esta conectado a pa-1
	    objeto[pp - 1]->Conectar (objeto[pa - 1]);
	  }
      }
}

/*
-------------------------------------------------------------------------
Função:   CalcularCondutancias
-------------------------------------------------------------------------
@short  : Transforma a propriedades raioHidraulico dos sítios em condutâncias.
	Função que percorre todo o grafo e transforma a propriedade
	raioHidraulico em condutancia (dos sítios)
	pois para o objeto CPermeabilidadeGrafo o que interessa é a condutividade
	que é usada pelo próprio objeto sítio para calcular a sua pressão (x).
@author :	André Duarte Bueno
@see    : Condutância
@param  : viscosidade, dimensão do pixel e fator de amplificação do pixel
@return : void
*/
void
CGra3Dby2D_M1::CalcularCondutancias (long double _viscosidade,
				       long double _dimensaoPixel,
				       unsigned long int _fatorAmplificacao) 
{
  // Variáveis auxiliares
  long double raio_hidraulico;
  long double dH;
  long double dimensaoPixelXfatorAmplificacao = _dimensaoPixel * _fatorAmplificacao;

  // long double PI=3.141592653589;
  long double variavelAuxiliar =
    (CMath::PI) / (128.0 * _viscosidade * dimensaoPixelXfatorAmplificacao);

  // Percorre  todos os objetos do  grafo
  raio_hidraulico = 0.25 * dimensaoPixelXfatorAmplificacao;
  dH = 4.0 * raio_hidraulico;
  long double condutanciaDoPixel = variavelAuxiliar * dH * dH * dH * dH;

  for (unsigned long int k = 0; k < objeto.size (); k++)
    {
      objeto[k]->propriedade = condutanciaDoPixel;
    }
  return;
}
