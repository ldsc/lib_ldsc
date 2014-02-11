// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef  CFABPixelContorno_cpp
#define  CFABPixelContorno_cpp

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC.
			Analise de Imagens de Meios Porosos (mascaras e filtros)
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:        Todos os direitos reservados.
Nome deste arquivo:	CFABPixelContorno.cpp       (thrlap)
Nome da classe:         CFABPixelContorno
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:      	Implementa o filtro binario usando nível de corte médio
*/

/*
----------------------------------------------------------------------------
Bibliotecas
----------------------------------------------------------------------------
*/

#include "AnaliseImagem/Filtro/FAmplitude/CFABPixelContorno.h"
#include "AnaliseImagem/Matriz/CHistograma.h"	// objetos usados aqui
#include "AnaliseImagem/Matriz/TCMatriz2D.h"
#include "AnaliseImagem/Filtro/FEspacial/CFELaplaciano.h"
#include "AnaliseImagem/Filtro/FAmplitude/CFABDoisPicos.h"

/*
==================================================================================
Documentacao       Go
==================================================================================
Descrição:        Calcula o valor médio do nível de corte e depois processa binarização
Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz2D<T> * CFABPixelContorno<T>::Go (TCMatriz2D<T> * &matriz, unsigned int _tamanhoMascara) {
	// unsigned int i,j;
	int i, j;
	this->pm = matriz;
	TCMatriz2D<T> *lap = new TCMatriz2D<T> ( * this->pm);	// 0-Cria matriz laplaciano, copia de pm

	// 1-Calcula o laplaciano da imagem
	CFELaplaciano filtroLaplaciano (lap, 3);	// tamanho da mascara=3

	filtroLaplaciano.Go (lap);	// calcula o laplaciano em sí

	CHistograma *histograma = new CHistograma (256);	// cria objeto histograma

	histograma->Go (lap);		// 2-Calcula o histograma para a matriz laplaciano

	// 3-calcula nível de corte para ter PCHistLap%
	int nivelCorteLaplaciano = histograma->NivelCortePara (PCHistLap);	// de píxel's ativos na matriz laplaciano

	// 4-Agora vai calcular o histograma da imagem pm, mas considerando
	// somente os pontos de pm que satisfaçam a condição de nivel de corte
	// definida pela matriz laplaciano
	// ----------inicio calculo histograma-------
	histograma->Constante (0);	// zera o histograma
	for (i = 0; i < this->pm->NX (); i++)	// percorre a imagem
		for (j = 0; j < this->pm->NY (); j++)	{
			if (// evita acesso a ponto alem dos limites do vetor histograma
					(this->pm->data2D[i][j] < histograma->NX ()) &&	// só considerar pontos que satisfaçam a condição do laplaciano
					(lap->data2D[i][j] >= nivelCorteLaplaciano))
				histograma->data1D[this->pm->data2D[i][j]]++;	// incrementa
		}
	float area = this->pm->NX () * this->pm->NY ();	// coloca histograma em percentuais

	for (unsigned int k = 0; k < histograma->NX (); k++)
		histograma->data1D[k] = histograma->data1D[k] * 100.0 / area;
	// ----------fim calculo histograma-------


	delete lap;			// elimina objeto matriz laplaciano

	// 5-Agora precisa determinar o nivel de corte
	// usando função determinaNivelCorte da classe irmã
	CFABDoisPicos<T> * fBinarizacaoDoisPicos = new CFABDoisPicos<T> (this->pm);	// Cria filtro de classe irmã
	CFABinario<T>::nivel = fBinarizacaoDoisPicos->determinaNivelCorte (histograma);	// chama função publica da classe irmã

	delete histograma;		// elimina objeto histograma

	delete fBinarizacaoDoisPicos;	// elimina objeto filtro auxiliar

	return CFABinario<T>::Go (this->pm);	// Executa função Go da classe base
}
#endif
