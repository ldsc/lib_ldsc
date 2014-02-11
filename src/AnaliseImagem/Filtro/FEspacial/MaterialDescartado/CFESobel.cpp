#ifndef CFiltroSobel_cpp
#define CFiltroSobel_cpp
/*
----------------------------------------------------------------------------
PROJETO:		Anaimp.
			Analise de Imagens de Meios Porosos.
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TFGaussiano.cpp
Nome da classe:      TFGaussiano
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
----------------------------------------------------------------------------
			BIBLIOTECAS
----------------------------------------------------------------------------
*/

#define CFiltroDLL		// define como sendo uma dll
#include "Filtro/FEspacial/CFiltroSobel.h"

CMatriz2D *
CFiltroSobel::Go ()
{
  IMask8 hMask (rawImg, GetMaskSize ());
  int8 ch = -1;
  // Seta a primeira mascara
  for (int i = 0; i < GetMaskSize (); i++)
    for (int j = 0; j < GetMaskSize (); j++)
      {
	if (i == (GetMaskSize () / 2))
	  {
	    hMask.SetMaskWeigh (i, j, 0);
	    ch = 1;
	  }
	else if (j == (GetMaskSize () / 2))
	  hMask.SetMaskWeigh (i, j, 2 * ch);
	else
	  hMask.SetMaskWeigh (i, j, 1 * ch);
      }
  // Aplica a mascara
  IRawImage *hImg = ApplyMask (hMask, rawImg->GetSize ());


  IMask8 mask (hImg, GetMaskSize ());
  ch = -1;
  // Seta a segunda mascara
  for (int i = 0; i < GetMaskSize (); i++)
    for (int j = 0; j < GetMaskSize (); j++)
      {
	if (i == (GetMaskSize () / 2))
	  {
	    mask.SetMaskWeigh (j, i, 0);
	    ch = 1;
	  }
	else if (j == (GetMaskSize () / 2))
	  mask.SetMaskWeigh (j, i, 2 * ch);
	else
	  mask.SetMaskWeigh (j, i, 1 * ch);
      }
  // Aplica a mascara
  IRawImage *rImg = ApplyMask (mask, hImg->GetSize ());
  delete hImg;
  return rImg;
}


#endif
