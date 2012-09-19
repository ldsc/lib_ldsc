// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef  CFABinario_cpp
#define  CFABinario_cpp

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC.
			Analise de Imagens de Meios Porosos (mascaras e filtros)
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  Todos os direitos reservados.
Nome deste arquivo:	CFABinario.cpp
Nome da classe:      CFABinario
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:      	Implementa o filtro binario
*/
/*
----------------------------------------------------------------------------
Bibliotecas
----------------------------------------------------------------------------
*/

#include "Filtro/FAmplitude/CFABinario.h"

/*
==================================================================================
Documentacao       Go
==================================================================================
Descrição:        Se o valor de cor do pixel for maior que o nivel de corte, assume o valor 1
						caso contrário assume valor  0.
Programador:      Andre Duarte Bueno
*/
TCMatriz2D< int > *
CFABinario::Go (TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara)
{
  pm = matriz;
  for (unsigned int i = 0; i < pm->NX (); i++)
    for (unsigned int j = 0; j < pm->NY (); j++)
      {
	(pm->data2D[i][j] > nivel) ?
	  // pm->data2D[i][j]=1:  pm->data2D[i][j]=0;
	  pm->data2D[i][j] = 0 : pm->data2D[i][j] = 1;	// a imagem cinza tem branco=255 preto=0
      }				// o resultado binario deve ter branco=0 preto=1
  return pm;
}
#endif
