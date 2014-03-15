//=======================================================================
//PROJETO:          Biblioteca LIB_LMPT
//                  Ramo: TPadrao_ramo
//=======================================================================
//
//Desenvolvido por: Laboratorio de Meios Porosos
//                  e Propriedades Termofisicas     [LMPT].
//@author:          Andr� Duarte Bueno
//File:             TISChamber.cpp
//begin:            Fri Nov 17 2000
//copyright:        (C) 2000 by Andr� Duarte Bueno
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
@author :Andr� Duarte Bueno
@see    :
@param  :
@return :
*/

//#ifndef TISChamber_cpp
// #define TISChamber_cpp

/*
----------------------------------------------------------------------------
PROJETO:			IMAGO
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Andre D.Bueno,
			Marcos Damiani,Paulo C. Philippi,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TISChamber.h
Nome da classe:         TISChamber
			Arquivos de documentacao do projeto em:
			path\documentacao\*.doc, path\Help

----------------------------------------------------------------------------
*/
//--------------------------------------------------------------------------
//Imago
//--------------------------------------------------------------------------
//#include <imago/img3d/pixit3d1.h>
//#include <imago/img3d/pixit3d8.h>
#include <imago/img3d/pixit/pixit1.h>	//iterator's
#include <imago/img3d/pixit/pixit8.h>	//iterator's

//--------------------------------------------------------------------------
//LIB_LMPT
//--------------------------------------------------------------------------
#include <Base/_LIB_LMPT_CLASS.h>
#include <TConEquCOILIB/TISChamber.h>
#include <TConversaoImago/TOperacoesImg.h>

//aquiaqui
COI20_CLASSID_DEF1 (TISChamber, TIChamber)
//TISChamber(I3DRawImage * _imgPorousMedia,int _wb=0, int _ca=1);
TISChamber::TISChamber (I3DRawImage * _imgPorousMedia, int _wb, int _ca):
        TIChamber (_imgPorousMedia->Size (), _wb, _ca)
{
    //Cria uma copia da imagem reconstruida passada
    imgPorousMedia = TOI::GetCopyBinaria (_imgPorousMedia);
    assert (imgPorousMedia != NULL);

    //Novo cálculo o raio m�ximo aqui
    maxRadius = TOI::GetMaxIDFRadius (imgPorousMedia);
    //fim novo

    //Evita que a dimens�o base seja maior que 1/3 da imagem
    //desta forma o n�cleo ter� pelo menos 1/3 da menor dimens�o da imagem
    baseDimension = 2 * maxRadius + 1;
    if (baseDimension > size.x / 3)
        baseDimension = size.x / 3;
    if (baseDimension > size.y / 3)
        baseDimension = size.y / 3;
    if (baseDimension > size.z / 3)
        baseDimension = size.z / 3;


    //Cria a imagem prototipo
    imgPrototype = new I3DRawImage (T3Dint32 (3, 3, 3), 8);
    assert (imgPrototype != NULL);
    //Define par�metros
    imgPrototype->SetAmplifyFactor (_imgPorousMedia->AmplifyFactor ());
    imgPrototype->SetVoxelSize (_imgPorousMedia->VoxelSize ());
};

//A funcao Draw � gen�rica, recebe uma imagem binaria e um indice de cor
//percorre toda a imagem prototipo e verifica se o indice bate, se ok
//chama DrawBox, que desenha o box solicitado
I3DRawImage *
TISChamber::Draw (I3DRawImage * imgBinaria, uint8 cIndex)
{
    //Se � para redesenhar toda a camara
    if (redrawAll)
        //zera todos os p�xeis,
        imgBinaria->ClearBits ();

    //E abaixo s� desenha pixeis a serem ativados
    I3DPixelIterator8 *piimgPrototype =
        I3DPixelIterator8::SafeCast (imgPrototype->InitPixelIterator ());
    //Percore a imagem prototipo
    for (int ii = 0; ii < imgPrototype->Width (); ii++)
        for (int jj = 0; jj < imgPrototype->Height (); jj++)
            for (int kk = 0; kk < imgPrototype->Depth (); kk++)
                //se ativo na imagem prototipo
                if (piimgPrototype->Get (ii, jj, kk) == cIndex)
                    //Desenha na imagem binaria o bloco ii,jj,kk
                    DrawBox (imgBinaria, ii, jj, kk);
    delete piimgPrototype;
    return imgBinaria;
}

//Funcoes Get
uint32
TISChamber::Getxi (uint32 _i)
{
    uint32 pos;
    switch (_i)
    {
    case 0:
        pos = 0;
        break;
    case 1:
        pos = baseDimension;
        break;
    case 2:
        pos = size.x - baseDimension;
        break;
    }
    return pos;
}

uint32
TISChamber::Getxf (uint32 _i)
{
    uint32 pos;
    switch (_i)
    {
    case 0:
        pos = baseDimension;
        break;
    case 1:
        pos = size.x - baseDimension;
        break;
    case 2:
        pos = size.x;
        break;
    }
    return pos;
}

uint32
TISChamber::Getyi (uint32 _i)
{
    uint32 pos;
    switch (_i)
    {
    case 0:
        pos = 0;
        break;
    case 1:
        pos = baseDimension;
        break;
    case 2:
        pos = size.y - baseDimension;
        break;
    }
    return pos;
}

uint32
TISChamber::Getyf (uint32 _i)
{
    uint32 pos;
    switch (_i)
    {
    case 0:
        pos = baseDimension;
        break;
    case 1:
        pos = size.y - baseDimension;
        break;
    case 2:
        pos = size.y;
        break;
    }
    return pos;
}

uint32
TISChamber::Getzi (uint32 _i)
{
    uint32 pos;
    switch (_i)
    {
    case 0:
        pos = 0;
        break;
    case 1:
        pos = baseDimension;
        break;
    case 2:
        pos = size.z - baseDimension;
        break;
    }
    return pos;
}

uint32
TISChamber::Getzf (uint32 _i)
{
    uint32 pos;
    switch (_i)
    {
    case 0:
        pos = baseDimension;
        break;
    case 1:
        pos = size.z - baseDimension;
        break;
    case 2:
        pos = size.z;
        break;
    }
    return pos;
}

//Dada as posicoes i,j,k na imagem prototipo, determina as posicoes xi,xf...na imagem binaria
//e desenha na imagem binaria o bloco definido por xi,xf,yi,yf,zi,zf.
void
TISChamber::DrawBox (I3DRawImage * imgBinaria, uint32 _i, uint32 _j,
                     uint32 _k)
{
    //aqui colocar um if BPP()==1
    //Determina a posi��o na imagem
    uint32 xi = Getxi (_i);
    uint32 xf = Getxf (_i);
    uint32 yi = Getyi (_j);
    uint32 yf = Getyf (_j);
    uint32 zi = Getzi (_k);
    uint32 zf = Getzf (_k);

    //Desenha na imagem o bloco
    I3DPixelIterator1 *piimgBinaria =
        I3DPixelIterator1::SafeCast (imgBinaria->InitPixelIterator ());
    for (unsigned int i = xi; i < xf; i++)
        for (unsigned int j = yi; j < yf; j++)
            for (unsigned int k = zi; k < zf; k++)
                piimgBinaria->Set (i, j, k, cPaintBrush);
}

//Fun��o que desenha na imagem binaria passada a imagem do meio poroso
I3DRawImage *
TISChamber::DrawPorousMedia (I3DRawImage * imgBinaria)
{
    //Se � para redesenhar toda a camara
    if (redrawAll == true)
        //zera todos os p�xeis,
        imgBinaria->ClearBits ();

    //e abaixo s� desenha pixeis a serem ativados
    I3DPixelIterator1 *piimgBinaria =
        I3DPixelIterator1::SafeCast (imgBinaria->InitPixelIterator ());
    I3DPixelIterator1 *piimgPorousMedia =
        I3DPixelIterator1::SafeCast (imgPorousMedia->InitPixelIterator ());
    I3DPixelIterator8 *piimgPrototype =
        I3DPixelIterator8::SafeCast (imgPrototype->InitPixelIterator ());

    //Percore a imagem prototipo
    for (uint32 ii = 0; ii < imgPrototype->Width (); ii++)
        for (uint32 jj = 0; jj < imgPrototype->Height (); jj++)
            for (uint32 kk = 0; kk < imgPrototype->Depth (); kk++)
                //se o indice de cor esta ok
                //desenha na imgBinaria a img do meio poroso
                if (piimgPrototype->Get (ii, jj, kk) == cPorousMedia)
                {
                    uint32 xi = Getxi (ii);
                    uint32 xf = Getxf (ii);
                    uint32 yi = Getyi (jj);
                    uint32 yf = Getyf (jj);
                    uint32 zi = Getzi (kk);
                    uint32 zf = Getzf (kk);

                    //percorre a imagem binaria
                    for (unsigned int i = xi; i < xf; i++)
                        for (unsigned int j = yi; j < yf; j++)
                            for (unsigned int k = zi; k < zf; k++)
                                //seta com o valor da imagem do meio poroso
                                piimgBinaria->Set (i, j, k,
                                                   piimgPorousMedia->Get (i, j, k));
                }
    delete piimgBinaria;
    delete piimgPorousMedia;
    delete piimgPrototype;
    return imgBinaria;
}

///////////////////////////////////////////////////////////
//Calculo do volume da regi�o ocupada pelo meio poroso
void
TISChamber::VolumeRegionPorousMedia ()
{
    I3DPixelIterator8 *piimgPrototype =
        I3DPixelIterator8::SafeCast (imgPrototype->InitPixelIterator ());
    volumeRegionPorousMedia = 0;
    //Percorre a imagem prototipo
    for (uint32 ii = 0; ii < imgPrototype->Width (); ii++)
        for (uint32 jj = 0; jj < imgPrototype->Height (); jj++)
            for (uint32 kk = 0; kk < imgPrototype->Depth (); kk++)
                //se o indice de cor esta ok
                //desenha na imgBinaria a img do meio poroso
                if (piimgPrototype->Get (ii, jj, kk) == cPorousMedia)
                {
                    uint32 xi = Getxi (ii);
                    uint32 xf = Getxf (ii);
                    uint32 yi = Getyi (jj);
                    uint32 yf = Getyf (jj);
                    uint32 zi = Getzi (kk);
                    uint32 zf = Getzf (kk);

                    //ACumula o volume do cubinho
                    volumeRegionPorousMedia += (xf - xi) * (yf - yi) * (zf - zi);
                }
    delete piimgPrototype;
}

I3DRawImage *
TISChamber::GetImgNucleo (I3DRawImage * imgDados, unsigned int borderDiscount)
{
    I3DRawImage *img =
        TOI::GetNewImgShelled (imgDados, Getxi (1), Getyi (1), Getzi (1),
                               Getxf (1), Getyf (1), Getzf (1), borderDiscount);
    return img;
}

//#endif
