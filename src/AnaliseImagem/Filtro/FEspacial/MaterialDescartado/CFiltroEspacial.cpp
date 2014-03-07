/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEspacial.cpp
Nome da classe:         CFEspacial
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
----------------------------------------------------------------------------
			BIBLIOTECAS
----------------------------------------------------------------------------
*/
#define FiltroDLL		// define como sendo uma dll
#include "AnaliseImagem/Filtro/FEspacial/CFEspacial.h"
#include "AnaliseImagem/Filtro/Mascara/CMPassaBaixa.h"	// O filtro espacial atua sobre a mascara
// do passa baixa todos os valores=1// outros filtros usam outras mascaras.

/*
============================================================================
Documentacao      CriaMascara
============================================================================
Descri��o:        Se a mascara ja existe deve ser eliminada
Programador:      Andre Duarte Bueno
*/

void
CFEspacial::CriaMascara (unsigned int tamanhoMascara)
{
  if (mask)
    delete mask;

  mask = new CMPassaBaixa (tamanhoMascara);	// CMPAssaBaixa � a default, faz a m�dia
}

/*
============================================================================
Documentacao      Go
============================================================================
Descri��o:	Deve percorrer a imagem somente onde a mascara cabe // depois mudar
Programador:      Andre Duarte Bueno
*/

CMatriz2D *
CFEspacial::Go ()
{
  CriaMascara ();		// Cria a mascara adequada

  // A funcao de preenchimento da mascara
  // j� calculada o peso da mascara
  CMatriz2D *rImg = new CMatriz2D (*pm);	// rImg � uma matriz c�pia da matriz pm passada

  int i, j, k, l;		// vari�veis auxiliares

  int raioMascaraX = mask->RaioX ();	// temporaria aumenta performance
  int raioMascaraY = mask->RaioY ();
  float soma;

  // Percorre a matriz imagem, exceto a borda
  for (i = (raioMascaraX); i < (pm->NX () - raioMascaraX); i++)
    for (j = (raioMascaraY); j < (pm->NY () - raioMascaraY); j++)
      {
	soma = 0.0;
	for (k = 0; k < mask->NX (); k++)	// percorre a mascara
	  for (l = 0; l < mask->NY (); l++)
	    {			// realiza convolu��o da mascara com a imagem
	      soma +=
		mask->data2D[k][l] 
		* rImg->data2D[i + k - raioMascaraX][j + l -raioMascaraY];
	    }
	// devo garantir que nao seja valor negativo
	// (cor nao realizavel)
	// o fator de peso da mascara ou � 1 ou
	// um numero maior que 1
	if (soma > 0)
	  pm->data2D[i][j] = soma / mask->Peso ();
	else
	  pm->data2D[i][j] = 0;
      }
  delete rImg;
  return pm;
}
