// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef  CFABDoisPicos_cpp
#define  CFABDoisPicos_cpp

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC.
			Analise de Imagens de Meios Porosos (mascaras e filtros)
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  Todos os direitos reservados.
Nome deste arquivo:	CFABDoisPicos.cpp     (twopeaks)
Nome da classe:      CFABDoisPicos
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:      	Implementa o filtro binario usando nível de corte médio
*/
/*
----------------------------------------------------------------------------
Bibliotecas
----------------------------------------------------------------------------
*/

#include "MetNum/Matriz/CHistograma.h"
#include "MetNum/Matriz/TCMatriz2D.h"
#include "AnaliseImagem/Filtro/FAmplitude/CFABDoisPicos.h"

/*
==================================================================================
Documentacao       Go
==================================================================================
Descrição:        Calcula o valor médio do nível de corte
						e depois processa binarização

Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz2D<T> * CFABDoisPicos<T>::Go (TCMatriz2D<T> * &matriz, unsigned int _tamanhoMascara) {
	this->pm = matriz;
	//
	CFABinario<T>::nivel = -1;			// variável herdada
	CHistograma *hist = new CHistograma (256);	// cria histograma de níveis de cinza
	hist->Go (this->pm);		// calcula o histograma

	CFABinario<T>::nivel = determinaNivelCorte (hist);	// função desta classe que determina o nivel
	// de corte a partir do histograma
	return CFABinario<T>::Go (this->pm);	// Executa função Go da classe base
	// que processa a binarização
}

/*
==================================================================================
Documentacao      determinaNivelCorte
==================================================================================
Descrição:        recebe o histograma de níveis de cinza da imagem e determina
						o nível de corte.

Programador:      Andre Duarte Bueno
*/
template<typename T>
unsigned int CFABDoisPicos<T>::determinaNivelCorte (CHistograma * &hist) {
	// Procura o primeiro pico
	unsigned int i, j, k;
	unsigned int nivelCorte;
	j = 0;
	for (i = 0; i < hist->NX (); i++)
		if (hist->data1D[i] > hist->data1D[j])
			j = i;

	// procura o segundo pico
	k = 0;
	for (i = 0; i < hist->NX (); i++)
		if (i > 0 && hist->data1D[i - 1] <= hist->data1D[i]
				&& i < hist->NX () - 1 && hist->data1D[i + 1] <= hist->data1D[i])
			if ((k - j) * (k - j) * hist->data1D[k] <
					(i - j) * (i - j) * hist->data1D[i])
				k = i;

	nivelCorte = j;
	if (j < k)
	{
		for (i = j; i < k; i++)
			if (hist->data1D[i] < hist->data1D[nivelCorte])
				nivelCorte = i;
	}
	else
	{
		for (i = k; i < j; i++)
			if (hist->data1D[i] < hist->data1D[nivelCorte])
				nivelCorte = i;
	}
	return nivelCorte;
}
#endif
