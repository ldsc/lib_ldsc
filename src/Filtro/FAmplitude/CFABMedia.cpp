// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef  CFABMedia_cpp
#define  CFABMedia_cpp

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC.
			Analise de Imagens de Meios Porosos (mascaras e filtros)
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:        Todos os direitos reservados.
Nome deste arquivo:	CFABMedia.cpp      (thrmean)
Nome da classe:         CFABMedia
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:      	Implementa o filtro binario usando nível de corte médio
*/
/*
----------------------------------------------------------------------------
Bibliotecas
----------------------------------------------------------------------------
*/

#include "Filtro/FAmplitude/CFABMedia.h"
#include "Matriz/TMatriz2D.h"

/*
==================================================================================
Documentacao       Go
==================================================================================
Descrição:        Calcula o valor médio do nível de corte
						e depois processa binarização
Programador:      Andre Duarte Bueno
*/
TMatriz2D< int > *
CFABMedia::Go (TMatriz2D< int > * &matriz, unsigned int _tamanhoMascara)
{
  pm = matriz;

  nivel = pm->Media ();		// a matriz imagem calcula sua média
  // que é utilizada como nível de corte
  return CFABinario::Go (pm);	// Executa função Go da classe base
  // que processa a binarização
}
#endif
