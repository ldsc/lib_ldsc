// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef  CFABEntropiaJohansen_cpp
#define  CFABEntropiaJohansen_cpp

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC.
			Analise de Imagens de Meios Porosos (mascaras e filtros)
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  Todos os direitos reservados.
Nome deste arquivo:	CFABEntropiaJohansen.cpp   (thr_joh)
Nome da classe:      CFABEntropiaJohansen
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:      	Implementa o filtro de Johansen
						Baseado no metodo da entropia.

// Antes o histograma era em float, agora é int, verificar efeitos
*/
/*
----------------------------------------------------------------------------
Bibliotecas
----------------------------------------------------------------------------
*/
#include <cmath>
#include "AnaliseImagem/Filtro/FAmplitude/CFABEntropiaJohansen.h"
#include "AnaliseImagem/Matriz/TCMatriz2D.h"
#include "AnaliseImagem/Matriz/CHistograma.h"	// ponteiro para histograma

/*
==================================================================================
Documentacao       Go
==================================================================================
Descricao:        Calcula o valor médio do nível de corte
						e depois processa binarização
Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz2D<T> * CFABEntropiaJohansen<T>::Go (TCMatriz2D<T> * &matriz, unsigned int _tamanhoMascara) {
	this->pm = matriz;			// armazena a matriz
	int i;			// , j;                     // contadores
	int inicio, fim;		// valor inicial e final do intervalo útil do histograma
	float Sb, Sw;			// variáveis auxiliares

	float *Pt = new float[256];	// aloca vetores float
	float *F = new float[256];
	float *Pq = new float[256];

	CHistograma histograma (256);	// cria histograma
	histograma.Go (this->pm);		// e calcula

	// calcula fatores
	Pt[0] = histograma.data1D[0];
	Pq[0] = 1.0 - Pt[0];
	for (i = 1; i < 256; i++)
	{
		Pt[i] = Pt[i - 1] + histograma.data1D[i];	// acumula valores
		Pq[i] = 1.0 - Pt[i - 1];
	}

	// Determina o intervalo válido do histograma
	inicio = 0;			// maior nível de corte com valor 0 no histograma
	while (histograma.data1D[inicio] <= 0)	// calcula variavel inicio
		inicio++;			// que representa o menor indice do histograma com valores válidos

	fim = 255;			// menor nível de corte com valor 255 no histograma
	while (histograma.data1D[fim] <= 0)	// calcula variavel fim
		fim--;

	// Calcula a funcao que minimiza todos os níveis
	CFABinario<T>::nivel = inicio;		// -1;
	float temporaria1;		// variavel temporaria para acelerar processo
	for (i = inicio; i <= fim; i++)
	{
		if (histograma.data1D[i] <= 0)	// se histograma menor ou igual a zero encerra                 0 *   0  **
			continue;		// sai do for  // isto pode ocorrer se o histograma for bimodal__**** __****
		temporaria1 = entropy (histograma.data1D[i]);

		Sb = flog (Pt[i]) + (1.0 / Pt[i]) * (temporaria1 + entropy (Pt[i - 1]));
		Sw = flog (Pq[i]) + (1.0 / Pq[i]) * (temporaria1 + entropy (Pq[i + 1]));
		F[i] = Sb + Sw;
		if (CFABinario<T>::nivel < 0)		// acusou ser sempre falso, porque nivel é unsigned int(sempre positivo)
			CFABinario<T>::nivel = i;		// ???????codigo inutil? verificar no codigo original se nivel é unsigned int
		else if (F[i] < F[CFABinario<T>::nivel])
			CFABinario<T>::nivel = i;
	}

	delete [] Pt;
	delete [] F;
	delete [] Pq;
	// aqui o nível ótimo de binarização ja foi calculado
	return CFABinario<T>::Go (this->pm);	// Executa funcao Go da classe base
	// que processa a binarização
}

// Calcula a entropia para o valor h
// entropia(h)=-h*log(h)
template<typename T>
float CFABEntropiaJohansen<T>::entropy (float h) {
	if (h > 0.0)
		return (-h * (float) std::log ((double) (h)));
	else
		return 0.0;
}

// Calcula logaritmo de numero float
// primeiro verifica se é maior que zero
template<typename T>
float CFABEntropiaJohansen<T>::flog (float x) {
	if (x > 0.0)
		return (float) std::log ((double) x);
	else
		return 0.0;
}
#endif
