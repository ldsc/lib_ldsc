// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef  CFABVarianciaOtsu_cpp
#define  CFABVarianciaOtsu_cpp

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC.
			Analise de Imagens de Meios Porosos (mascaras e filtros)
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:        Todos os direitos reservados.
Nome deste arquivo:	CFABVarianciaOtsu.cpp    (thrglh)
Nome da classe:         CFABVarianciaOtsu
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:      	Implementa o filtro binario usando nível de corte médio
*/

/*
----------------------------------------------------------------------------
Bibliotecas
----------------------------------------------------------------------------
*/

#include "Filtro/FAmplitude/CFABVarianciaOtsu.h"
#include "Matriz/CHistograma.h"	// ponteiro para histograma
#include "Matriz/TCMatriz2D.h"

/*
==================================================================================
Documentacao       Go
==================================================================================
Descrição:
Threshold selection using grey level histograms.
SMC-9 No 1 Jan 1979	N. Otsu

Calcula o valor do nível de corte e depois processa binarização
Método variancia, separa a imagem em duas regioes a e b
Para cada regiao calcula a variancia va e vb
Faz processo iterativo de forma a determinar a menor razão entre
As variancias razao=va/vb

Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz2D<T> * CFABVarianciaOtsu<T>::Go (TCMatriz2D<T> * &matriz, unsigned int _tamanhoMascara) {
	CFiltro<T>::pm = matriz;
	//
	int i, j, k;			// contadores
	float y, z;			// variáveis auxiliares
	float iXdataiTotal;		// i multiplicado por data[i] de 0 a 256
	// datai é o vetor de dados do histograma auxiliar
	float varianciaTotal;		// variancia total calculada

	CHistograma histAux (256);	// Cria histograma, zera, calcula
	histAux.Go (CFiltro<T>::pm);		// calcula

	iXdataiTotal = iXdatai (histAux.data1D, 256);	// calcula i*data[i] de 0 a 256
	varianciaTotal = 0.0;		// variancia global
	for (i = 1; i <= 256; i++)	// calcula a variancia total
		varianciaTotal +=
				(i - iXdataiTotal) * (i - iXdataiTotal) * histAux.data1D[i - 1];

	j = -1;
	k = -1;
	for (i = 1; i <= 256; i++)
	{
		if ((j < 0) && (histAux.data1D[i - 1] > 0.0))
			j = i;			// primeiro indice
		if (histAux.data1D[i - 1] > 0.0)
			k = i;			// ultimo indice
	}
	z = -1.0;
	CFABinario<T>::nivel = 0;
	for (i = j; i <= k; i++) 	{
		y = nu (&histAux, i, iXdataiTotal, varianciaTotal);	// calcula NU
		if (y >= z) {		// se for o maior
			z = y;		// salvar valor e i
			CFABinario<T>::nivel = i;
		}
	}
	return CFABinario<T>::Go (CFiltro<T>::pm);	// Executa função Go da classe base
	// que processa a binarização
}

// Calcula a multiplicação de um valor data[i] do histograma pelo indice i
// de 0 até o nível k passado
// alterei o código, antes recebia um vetor de float's agora recebe vetor de int
// antes fazia i*data[i], não usava o data[0], agora usa data[0]
// pois data[i] foi trocado por data[i-1]
template<typename T>
float CFABVarianciaOtsu<T>::iXdatai (int *datai, int k) {
	int i;
	float acumulado = 0.0;
	for (i = 1; i <= k; i++)
		acumulado += (float) i *datai[i - 1];
	return acumulado;
}

// histAuxDatai é o vetor de dados do histograma
// antes recebia vetor da dados em float agora recebe em int
// ok
template<typename T>
float CFABVarianciaOtsu<T>::nu (CHistograma * histAux, int k, float iXdataiTotal, float varianciaTotal) {
	float xx, yy;
	yy = histAux->PercentagemPara (k - 1);	// retorna a percentagem acumulada relacionada ao nível
	// de corte k (agora é k-1 pois vetor histograma
	// começa de zero e não de 1)
	// diferença entre iXdataiTotal*yy e iXdatai
	// de 0 até k
	xx = iXdataiTotal * yy - iXdatai (histAux->data1D, k);	//
	xx = xx * xx;
	yy = yy * (1.0 - yy);
	if (yy > 0)
		xx = xx / yy;
	else
		xx = 0.0;
	return xx / varianciaTotal;
}
#endif
