/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
			[LDSC].
@author:          André Duarte Bueno
@file:             CHistograma.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>
#include <algorithm>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Matriz/CHistograma.h>

/*
-------------------------------------------------------------------------
Função:  Go
-------------------------------------------------------------------------
@short  : Calcula o histograma de níveis de cinza
@author : André Duarte Bueno
@see    : Matrizes
@param  : Recebe uma matriz 2D
@return : Retorna this
*/
CHistograma *
CHistograma::Go (TCMatriz2D< int > *matriz)
{
// Desaloca();
// nx=pow(2,matriz->BitsPixel());
  if (data1D)			// Só calcula se os dados foram corretamente alocados
    {
      Constante (0);		// zera o vetor histograma
      for (int j = 0; j < matriz->NY (); j++)	// Percorre a imagem2D
	for (int i = 0; i < matriz->NX (); i++)
	  {
	    if (matriz->data2D[i][j] < this->NX ())	// evita acesso a ponto alem dos limites do vetor histograma
	      data1D[matriz->data2D[i][j]]++;	// incrementa
	  }
      float area = matriz->NX () * matriz->NY ();	// área total da imagem
      for (int k = 0; k < NX (); k++)	// Percorre o histograma
	data1D[k] = static_cast < int >(data1D[k] * 100.0 / area);	// coloca histograma no intervalo 0-100
    }
  return this;
}

/*
-------------------------------------------------------------------------
Função:  NivelCortePara
-------------------------------------------------------------------------
@short  :
O histograma pode ter seus valores acumulados, criando uma função crescente
ou seja a função histogramaAcumulado.

Pode-se relacionar uma percentagem do histograma acumulado
com um nível de corte associado.
A função NivelCortePara(percentagem)
Procura o valor de corte que corresponde a percentagem (ex: porosidade) solicitada.

Exemplo de uso:
	 CHistograma histograma(256);  			// cria histograma
   histograma.Go();              			// calcula o histograma
   histograma.NivelCortePara(percentagem);// calcula o nível de corte baseado na percentagem

A percentagem é um valor de 0->100
O nível de corte é uma valor entre 0->255

@author :André Duarte Bueno
@see    :Matrizes
@param  :double percentagem
@return :  short int
*/
int
CHistograma::NivelCortePara (double percentagem)
{
  int nivelCorte = 0;		// Define o nivel de corte solicitado
  int i;

  if (data1D)			// Verifica se o vetor foi alocado
    for (i = 0; i < nx; i++)	// Normalmente nx = 256
      {				// O vetor data1D armazena o histograma, cuja soma é 100
	nivelCorte += /*static_cast<int> */ data1D[i];
	if (nivelCorte >= percentagem)
	  break;		// Encerra o for
      }
  // Retorna o
  // nível de corte associado a percentagem passada
  return i - 1;			// Retorna o nivel de corte associado
}

/*
-------------------------------------------------------------------------
Função:  PercentagemPara
-------------------------------------------------------------------------
@short  :
É o inverso da função acima.
Relaciona um nível de corte com o valor acumulado no histograma,
ou seja com uma percentagem.
A função PercentagemPara retorna a percentagem para o nível de corte passado.
O nível de corte é uma valor entre 0->255 (ou entre 0 e nx).

Se NivelCorte = 10, retorna a soma do histograma entre
0 e 10, ou seja, a soma de 0,1,2,3,4,5,6,7,8,9,10

@author : André Duarte Bueno
@see    :
@param  :   short int NivelCorte
@return : Percentagem acumulada do histograma
*/
// double PercentagemPara (    int NivelCorte);
double
CHistograma::PercentagemPara (int NivelCorte)
{
  if (NivelCorte > nx)
    NivelCorte = nx;		// Evita acesso a ponto inexistente

  double percentagem = 0.0;	// Zera a percentagem

  for (int i = 0; i <= NivelCorte; i++)	// Percorre até nivel de corte passado
    percentagem += data1D[i];	// Acumula a percentagem

  return percentagem;		// Retorna valor entre 0->100
}
