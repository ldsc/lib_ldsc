// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFACinza_cpp
#define CFACinza_cpp

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC.
			Analise de Imagens de Meios Porosos (mascaras e filtros)
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:        Todos os direitos reservados.
Nome deste arquivo:	CFACinza.cpp
Nome da classe:         CFACinza
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:      	Implementa o filtro binario
*/
/*
----------------------------------------------------------------------------
Bibliotecas
----------------------------------------------------------------------------
*/

#include "Filtro/FAmplitude/CFACinza.h"

/*
==================================================================================
Documentacao       Go
==================================================================================
Descricao:      Tres vetores 2D representam um objeto com 3 cores
		Recebe um vetor de ponteiros pm.
		Onde pm aponta para cor red, pm+1 para green e pm+2 para blue
Programador:      Andre Duarte Bueno
*/
CMatriz2D *
CFACinza::Go (CMatriz2D * &matriz, unsigned int _tamanhoMascara)
{
  pm = matriz;
  CMatriz2D *ptr[3];
  ptr[0] = pm;
  ptr[1] = pm + 1;
  ptr[2] = pm + 2;

  for (unsigned int i = 0; i < pm->NX (); i++)
    for (unsigned int j = 0; j < pm->NY (); j++)
      {
	pm->data2D[i][j] = (unsigned int)
	  (ptr[0]->data2D[i][j] * pesoRed
	   + ptr[1]->data2D[i][j] * pesoGreen
	   + ptr[2]->data2D[i][j] * pesoBlue);
      }
  return pm;
}
#endif
