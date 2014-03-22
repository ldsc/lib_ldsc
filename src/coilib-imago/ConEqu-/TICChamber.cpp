//=======================================================================
//PROJETO:          Biblioteca LIB_LMPT
//                  Ramo: TPadrao_ramo
//=======================================================================
//
//Desenvolvido por:     Laboratorio de Meios Porosos
//e Propriedades Termofisicas     [LMPT].
//@author:          André Duarte Bueno
//File:             TICChamber.cpp
//begin:            Fri Nov 17 2000
//copyright:        (C) 2000 by André Duarte Bueno
//email:            andre@lmpt.ufsc.br

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------
//#include <fstream>

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
//#include <path/nome>

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/

//#ifndef TICChamber_cpp
// #define TICChamber_cpp

/*
----------------------------------------------------------------------------
PROJETO:					Imago
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Andre D.Bueno,
		        Marcos Damiani,Paulo C. Philippi,,....
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TICChamber.h
Nome da classe:         TICChamber
			Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/
//confEq
//#include <imago/img3d/pixit3d1.h>
#include <imago/img3d/pixit/pixit1.h>

#include <Base/_LIB_LMPT_CLASS.h>
#include <TConEquCOILIB/TICChamber.h>

//Desenha na imagem binaria a regi�o definida na imagem prototipo com a mesma cor de cIndex
void
TICChamber::Draw (I3DRawImage * &imgBinaria, uint32 & cIndex)
{

    //se for para redesenhar tudo
    if (redrawAll)
        //limpa a imagem
        imgBinaria->ClearBits ();

    I3DPixelIterator1 *piimgBinaria =
        I3DPixelIterator1::SafeCast (imgBinaria->InitPixelIterator ());
    I3DPixelIterator8 *piimgPrototype =
        I3DPixelIterator8::SafeCast (imgPrototype->InitPixelIterator ());
    for (; !piimgBinaria->ImageWrapped ();
            piimgBinaria->Next (), piimgPrototype->Next ())
        if (piimgPrototype->Get () == cIndex)
            //cPaintBrush � 1 ou 0, se 1 desenha se 0 apaga
            piimgBinaria->Set (cPaintBrush);
}

//Desenha o meio poroso em si, verificando a informação na imagem prototipo
//Observe que n�o precisa receber a imagem do meio poroso, visto que
//a imagem prototipo j� cont�m a informa��o do meio poroso
//Recebe a mesma por compatibilidade com a classe base.
void
TICChamber::DrawPorousMedia (I3DRawImage * &imgBinaria)
{

    //se for para redesenhar tudo
    if (redrawAll)
        //limpa a imagem
        imgBinaria->ClearBits ();

    //Desenha na imagemBinaria a imagem do meio poroso passada
    I3DPixelIterator1 *piimgBinaria =
        I3DPixelIterator1::SafeCast (imgBinaria->InitPixelIterator ());
    I3DPixelIterator8 *piimgPrototype =
        I3DPixelIterator8::SafeCast (imgPrototype->InitPixelIterator ());
    for (; !piimgBinaria->ImageWrapped ();
            piimgBinaria->Next (), piimgPrototype->Next ())
        //se for um pixel de meio poroso, desenha o mesmo
        if (piimgPrototype->Get () == cPorousMedia)
            //tudo o que for meio poroso vai ser marcado.
            piimgBinaria->Set (cPaintBrush);
}

/*
Calculo do volume da regi�o meio poroso
 cSolid = 0; 			     //� meio poroso
 cFluid = 1;                        //� meio poroso
 cWettingFluid = 2;                 //� meio poroso
 cNowWettingFluid = 3;              //� meio poroso
 cChamberWettingFluid = 4;          //� camara
 cChamberNowWettingFluid = 5;       //� camara
 cIsolatedFluid=6;                  //� meio poroso
 cWall = 7;                         //� parede
 cPorousMedia = 8;                  //� meio poroso
*/
void
TICChamber::VolumeRegionPorousMedia ()
{
    I3DPixelIterator8 *piimgPrototype =
        I3DPixelIterator8::SafeCast (imgPrototype->InitPixelIterator ());
    /* volumeRegionPorousMedia = size.x * size.y * size.z;		 // volumeTotal da imagem
     for (; !piimgPrototype->ImageWrapped(); piimgPrototype->Next()) // percorre toda a imagem
     	   if(	piimgPrototype->Get() == cChamberWettingFluid    // se for camara
          	|| piimgPrototype->Get() == cChamberNotWettingFluid      // ou parede diminue
          	|| piimgPrototype->Get() == cWall
             )
    	       volumeRegionPorousMedia--;
    */
    volumeRegionPorousMedia = 0;
    //se for um pixel de meio poroso, acumula
    if (piimgPrototype->Get () == cPorousMedia)
        volumeRegionPorousMedia++;
}

//#endif
