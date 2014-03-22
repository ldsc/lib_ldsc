//=======================================================================
//PROJETO:          Biblioteca LIB_LMPT
//                  Ramo: conversao<->interface
//=======================================================================
//Desenvolvido por: Laboratorio de Meios Porosos
//                  Propriedades Termofisicas     [LMPT].
//@author:          André Duarte Bueno
//File:             TOperacoesImg.cpp
//begin:            Fri Nov 17 2000
//copyright:        (C) 2000 by André Duarte Bueno
//email:            andre@lmpt.ufsc.br

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------
#include <iostream>
#include <fstream>

//-----------------------------------------------------------------------
//Bibliotecas Coi-lib
//-----------------------------------------------------------------------
//#include <img3d/RawImg3D.h>
#include <imago/img3d/image/raw.h>

#include <imago/img3d/pixit/pixit1.h>	//pixel iterator 1bpp
#include <imago/img3d/pixit/pixit8.h>	//pixel iterator 16bpp
#include <imago/img3d/pixit/pixit16.h>	//pixel iterator 16bpp
#include <imago/img3d/pixit/pixit32.h>	//pixel iterator 16bpp

//Imago
#include <imago/flt3d/core/d345.h>	//filtro idf
#include <imago/flt3d/core/connect.h>	//retorna imagem conectada


//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
#include <TConversaoImago/TOperacoesImg.h>
#include <TRotulador/TRotulador3D.h>

/*
-------------------------------------------------------------------------
Fun��o:   SaveImg
-------------------------------------------------------------------------
@short  :	Salva imagem binaria em disco
          Abre uma stream para um arquivo de disco (fout), verifica se o mesmo
          abriu corretamente e salva os valores da imagem em disco.
@author : André Duarte Bueno
 
@param  : Recebe como parametros, uma imagem 3D binaria, e um nome de arquivo.
@return : Retorna um true/false que indica se a operacao ocorreu corretamente.
*/
bool
TOperacoesImg::SaveImg (I3DRawImage * img, string nomeArquivo)
{
    switch (img->BPP ())
    {
    case 1:
        SaveImg1bpp (img, nomeArquivo);
    case 8:
        SaveImg8bpp (img, nomeArquivo);
    case 16:
        SaveImg16bpp (img, nomeArquivo);
    case 32:
        SaveImg32bpp (img, nomeArquivo);
    default:
        return 0;
    }
}

/*
-------------------------------------------------------------------------
Fun��o: SaveImg1bpp
-------------------------------------------------------------------------
@short  :	Salva imagem binaria em disco
*/
bool
TOperacoesImg::SaveImg1bpp (I3DRawImage * img1bpp, string nomeArquivo)
{
    if (img1bpp->BPP () != 1)
        return 0;
    ofstream fout (nomeArquivo.c_str ());
    if (fout.good ())
    {
        I3DPixelIterator1 *piimg1bpp =
            I3DPixelIterator1::SafeCast (img1bpp->InitPixelIterator ());
        assert (piimg1bpp != NULL);
        for (int k = 0; k < img1bpp->Depth (); k++)
        {
            for (int j = 0; j < img1bpp->Height (); j++)
            {
                for (int i = 0; i < img1bpp->Width (); i++)
                {
                    fout << piimg1bpp->Get (i, j, k) << " ";
                }
                fout << "\n";
            }
            fout << "\n";
        }
        fout.close ();
        delete piimg1bpp;
        return true;
    }
    return false;
}

/*
-------------------------------------------------------------------------
Fun��o: SaveImg8bpp
-------------------------------------------------------------------------
@short  :	Salva imagem 8bpp em disco
*/
bool
TOperacoesImg::SaveImg8bpp (I3DRawImage * img8bpp, string nomeArquivo)
{
    if (img8bpp->BPP () != 8)
        return 0;
    ofstream fout (nomeArquivo.c_str ());
    if (fout.good ())
    {
        I3DPixelIterator8 *piimg8bpp =
            I3DPixelIterator8::SafeCast (img8bpp->InitPixelIterator ());
        assert (piimg8bpp != NULL);
        for (int k = 0; k < img8bpp->Depth (); k++)
        {
            for (int j = 0; j < img8bpp->Height (); j++)
            {
                for (int i = 0; i < img8bpp->Width (); i++)
                {
                    fout << piimg8bpp->Get (i, j, k) << " ";
                }
                fout << "\n";
            }
            fout << "\n";
        }
        fout.close ();
        delete piimg8bpp;
        return true;
    }
    return false;
}

/*
-------------------------------------------------------------------------
Fun��o: SaveImg16bpp
-------------------------------------------------------------------------
@short  : Salva imagem 16bpp em disco
*/
bool
TOperacoesImg::SaveImg16bpp (I3DRawImage * img16bpp, string nomeArquivo)
{
    if (img16bpp->BPP () != 16)
        return 0;
    ofstream fout (nomeArquivo.c_str ());
    if (fout.good ())
    {
        I3DPixelIterator16 *piimg16bpp =
            I3DPixelIterator16::SafeCast (img16bpp->InitPixelIterator ());
        assert (piimg16bpp != NULL);
        for (int k = 0; k < img16bpp->Depth (); k++)
        {
            for (int j = 0; j < img16bpp->Height (); j++)
            {
                for (int i = 0; i < img16bpp->Width (); i++)
                {
                    fout << piimg16bpp->Get (i, j, k) << " ";
                }
                fout << "\n";
            }
            fout << "\n";
        }
        fout.close ();
        delete piimg16bpp;
        return true;
    }
    return false;
}

/*
-------------------------------------------------------------------------
Fun��o:   SaveImg32bpp
-------------------------------------------------------------------------
@short  : Salva imagem 32bpp em disco
*/
bool
TOperacoesImg::SaveImg32bpp (I3DRawImage * img32bpp, string nomeArquivo)
{
    if (img32bpp->BPP () != 32)
        return 0;
    ofstream fout (nomeArquivo.c_str ());
    if (fout.good ())
    {
        I3DPixelIterator32 *piimg32bpp =
            I3DPixelIterator32::SafeCast (img32bpp->InitPixelIterator ());
        assert (piimg32bpp != NULL);
        for (int k = 0; k < img32bpp->Depth (); k++)
        {
            for (int j = 0; j < img32bpp->Height (); j++)
            {
                for (int i = 0; i < img32bpp->Width (); i++)
                {
                    fout << piimg32bpp->Get (i, j, k) << " ";
                }
                fout << "\n";
            }
            fout << "\n";
        }
        delete piimg32bpp;
        fout.close ();
        return true;
    }
    return false;
}

/*
-------------------------------------------------------------------------
Fun��o: OpenImg1bpp
-------------------------------------------------------------------------
@short  : Cria imagem binaria e l� a mesma do disco
*/
I3DRawImage *
TOperacoesImg::OpenImg1bpp (string nomeArquivo,
                            unsigned int nx, unsigned int ny, unsigned int nz)
{
    ifstream fin (nomeArquivo.c_str ());
    if (fin.good ())
    {
        I3DRawImage *imagem = new I3DRawImage (T3Dint32 (nx, ny, nz), 1);
        assert (imagem != NULL);
        unsigned int temp;
        if (imagem != NULL)
        {
            I3DPixelIterator1 *pi =
                I3DPixelIterator1::SafeCast (imagem->InitPixelIterator ());
            assert (pi != NULL);
            for (int k = 0; k < imagem->Depth (); k++)
                for (int j = 0; j < imagem->Height (); j++)
                    for (int i = 0; i < imagem->Width (); i++)
                    {
                        fin >> temp;
                        if (temp != 0)
                            pi->Set (i, j, k, 1);
                        else
                            pi->Set (i, j, k, 0);
                    }
            fin.close ();
            delete pi;
            return imagem;
        }
    }
    return NULL;
}

/*
-------------------------------------------------------------------------
Fun��o:   OpenImg16bpp
-------------------------------------------------------------------------
@short  : Cria imagem 16bpp e l� a mesma do disco
*/
I3DRawImage *
TOperacoesImg::OpenImg16bpp (string nomeArquivo,
                             unsigned int nx, unsigned int ny,
                             unsigned int nz)
{
    ifstream fin (nomeArquivo.c_str ());
    if (fin.good ())
    {
        I3DRawImage *imagem = new I3DRawImage (T3Dint32 (nx, ny, nz), 16);
        assert (imagem != NULL);
        imagem->ClearBits ();
        unsigned int temp;
        if (imagem != NULL)
        {
            I3DPixelIterator16 *pi =
                I3DPixelIterator16::SafeCast (imagem->InitPixelIterator ());
            assert (pi != NULL);
            for (int k = 0; k < imagem->Depth (); k++)
                for (int j = 0; j < imagem->Height (); j++)
                    for (int i = 0; i < imagem->Width (); i++)
                    {
                        fin >> temp;
                        pi->Set (i, j, k, temp);
                    }
            delete pi;
        }
        fin.close ();
        return imagem;
    }

    return NULL;
}

/*
-------------------------------------------------------------------------
Fun��o:	OpenImg16bppFrom1bpp
-------------------------------------------------------------------------
@short  :
@author : André Duarte Bueno
 
@param  :
@return :
*/
I3DRawImage *
TOperacoesImg::OpenImg16bppFrom1bpp (string nomeArquivo,
                                     unsigned int nx, unsigned int ny,
                                     unsigned int nz, unsigned int indiceCor)
{
    ifstream fin (nomeArquivo.c_str ());
    if (fin.good ())
    {
        I3DRawImage *imagemBinaria;
        imagemBinaria = new I3DRawImage (T3Dint32 (nx, ny, nz), 1);
        assert (imagemBinaria != NULL);

        imagemBinaria->ClearBits ();
        unsigned int temp;
        if (imagemBinaria != NULL)
        {
            I3DPixelIterator1 *pi =
                I3DPixelIterator1::SafeCast (imagemBinaria->InitPixelIterator ());
            for (int k = 0; k < imagemBinaria->Depth (); k++)
                for (int j = 0; j < imagemBinaria->Height (); j++)
                    for (int i = 0; i < imagemBinaria->Width (); i++)
                    {
                        fin >> temp;
                        if (temp == indiceCor)
                            pi->Set (i, j, k, 1);
                    }
            delete pi;
        }
        fin.close ();
        return imagemBinaria;
    }
    return NULL;
}

/*
-------------------------------------------------------------------------
Fun��o:	GetCopy1bpp
-------------------------------------------------------------------------
@short  :   Obt�m uma c�pia da imagem
	    Oberve que al�m de copiar os dados, copia os atributos VoxelSize e AmplifyFactor
@author :   André Duarte Bueno
*/
I3DRawImage *
TOperacoesImg::GetCopy1bpp (I3DRawImage * img)
{
    if (img->BPP () != 1)
    {
        assert (0);		//Sai do programa
        return 0;
    }

    I3DRawImage *imgCopy = new I3DRawImage (img->Size (), 1);
    assert (imgCopy != NULL);
    imgCopy->ClearBits ();
    I3DPixelIterator1 *piimg =
        I3DPixelIterator1::SafeCast (img->InitPixelIterator ());
    I3DPixelIterator1 *piimgCopy =
        I3DPixelIterator1::SafeCast (imgCopy->InitPixelIterator ());
    for (; !piimg->ImageWrapped (); piimg->Next (), piimgCopy->Next ())
        piimgCopy->Set (piimg->Get ());
    delete piimg;
    delete piimgCopy;

    imgCopy->SetVoxelSize (img->VoxelSize ());
    imgCopy->SetAmplifyFactor (img->AmplifyFactor ());

    return imgCopy;
}

/*
-------------------------------------------------------------------------
Fun��o:  GetImg1bppFrom16bpp
-------------------------------------------------------------------------
@short  : Recebe uma imagem16bpp e um indice de corte, retorna uma imagem binaria (nova)
@author : André Duarte Bueno
 
@param  :
@return :
*/
I3DRawImage *
TOperacoesImg::GetImg1bppFrom16bpp (I3DRawImage * img16bpp,
                                    unsigned int indiceCorte)
{
    if (img16bpp->BPP () != 16)
        return 0;

    I3DRawImage *imgBinaria = new I3DRawImage (img16bpp->Size (), 1);
    assert (imgBinaria != NULL);
    imgBinaria->ClearBits ();

    I3DPixelIterator1 *piimgBinaria =
        I3DPixelIterator1::SafeCast (imgBinaria->InitPixelIterator ());
    I3DPixelIterator16 *piimg16bpp =
        I3DPixelIterator16::SafeCast (img16bpp->InitPixelIterator ());

    for (; !piimg16bpp->ImageWrapped ();
            piimg16bpp->Next (), piimgBinaria->Next ())
    {
        if (piimg16bpp->Get () > indiceCorte)	//indiceCorte default = 0
            piimgBinaria->Set (1);
    }
    delete piimg16bpp;
    delete piimgBinaria;

    imgBinaria->SetVoxelSize (img16bpp->VoxelSize ());
    imgBinaria->SetAmplifyFactor (img16bpp->AmplifyFactor ());
    return imgBinaria;
}

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/
//--------------------------------------------------------------------------------
//Dada uma imagem binaria, retorna uma imagem 16bpp nova, sem deletar a binaria
//os valores na imagem 16bpp s�o setados com valor
I3DRawImage *
TOperacoesImg::GetImg16bppFrom1bpp (I3DRawImage * imgBinaria,
                                    unsigned int cor)
{
    if (imgBinaria->BPP () != 1)
        return 0;

    //Cria imagem  16bpp e
    I3DRawImage *img16bpp = new I3DRawImage (imgBinaria->Size (), 16);

    assert (img16bpp != NULL);
    img16bpp->ClearBits ();

    //Procedimento de copia
    I3DPixelIterator1 *piimgBinaria =
        I3DPixelIterator1::SafeCast (imgBinaria->InitPixelIterator ());
    I3DPixelIterator16 *piimg16bpp =
        I3DPixelIterator16::SafeCast (img16bpp->InitPixelIterator ());
    for (; !piimgBinaria->ImageWrapped ();
            piimgBinaria->Next (), piimg16bpp->Next ())
    {
        if (piimgBinaria->Get () == 1)
            piimg16bpp->Set (cor);
    }
    delete piimgBinaria;
    delete piimg16bpp;
    img16bpp->SetVoxelSize (imgBinaria->VoxelSize ());
    img16bpp->SetAmplifyFactor (imgBinaria->AmplifyFactor ());

    return img16bpp;
}

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/
//------------------------------------------------------------------------------
//Dada uma imagem16bpp , uma imagem binaria e um indice de cor, vai preencher na imagem binaria
//os pontos que coincidem com o indice de cor passado
//Se voc� tiver uma imagem colorida em 16bpp, pode criar uma imagem bin�ria,
//usando uma das cores da imagem 16bpp.
bool
TOperacoesImg::SetImg1bppFrom16bppColor (I3DRawImage * img16bpp,
        I3DRawImage * imgBinaria,
        uint32 indiceCorRegiao)
{
    if (imgBinaria->BPP () != 1 || img16bpp->BPP () != 16)
        return 0;

    imgBinaria->ClearBits ();
    I3DPixelIterator16 *piimg16bpp =
        I3DPixelIterator16::SafeCast (img16bpp->InitPixelIterator ());
    I3DPixelIterator1 *piiimgBinaria =
        I3DPixelIterator1::SafeCast (imgBinaria->InitPixelIterator ());

    for (; !piimg16bpp->ImageWrapped ();
            piimg16bpp->Next (), piiimgBinaria->Next ())
    {
        if (piimg16bpp->Get () == indiceCorRegiao)
            piiimgBinaria->Set (1);
    }
    delete piimg16bpp;
    delete piiimgBinaria;
    return 1;
}

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/
////////////////////////////////////////////
//Fun��o nova, realiza o resample da imagem
I3DRawImage *
TOperacoesImg::GetCopyResampled (I3DRawImage * img, unsigned int resample)
{
    ///////////////////
    //Cria imagem nova, considerando o resample                      //bpp
    I3DRawImage *imgnova =
        new
    I3DRawImage (T3Dint32
                 (img->Width () * resample, img->Height () * resample,
                  img->Depth () * resample), 1);
    assert (imgnova != NULL);
    imgnova->SetVoxelSize (img->VoxelSize () / (float) resample);	//Passa demais atributos da imagem, observe a divis�o pelo resample
    imgnova->SetAmplifyFactor (img->AmplifyFactor ());	//

    ///////////////////
    //Preenche a imagem nova
    imgnova->ClearBits ();	//zera a imagem nova
    I3DPixelIterator1 *piimg =
        I3DPixelIterator1::SafeCast (img->InitPixelIterator ());
    I3DPixelIterator1 *piimgnova =
        I3DPixelIterator1::SafeCast (imgnova->InitPixelIterator ());
    //int ii,jj,kk;
    unsigned int ixresample;
    unsigned int jxresample;
    unsigned int kxresample;
    for (unsigned int i = 0; i < img->Width (); i++)	//Percorre a imagem original
        for (unsigned int j = 0; j < img->Height (); j++)
            for (unsigned int k = 0; k < img->Depth (); k++)
                if (piimg->Get (i, j, k) == 1)	//se o pixel da imagem atual esta ativo
                {			//percorre a imagem nova, resamplada
                    ixresample = i * resample;
                    jxresample = j * resample;
                    kxresample = k * resample;
                    for (unsigned int ir = 0; ir < resample; ir++)
                        for (unsigned int jr = 0; jr < resample; jr++)
                            for (unsigned int kr = 0; kr < resample; kr++)	//e marca os p�xeis
                                piimgnova->Set (ixresample + ir, jxresample + jr,
                                                kxresample + kr, 1);

                }
    return imgnova;
}

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/
//----------------------------------------------------------------------------
//INT INT INT INT INT
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Funcao que recebe uma imagem16bpp e retorna o maior valor na imagem
unsigned int
TOperacoesImg::GetMaxValueFrom16bppImage (I3DRawImage * img16bpp)
{
    if (img16bpp->BPP () != 16)
        return 0;
    I3DPixelIterator16 *pi =
        I3DPixelIterator16::SafeCast (img16bpp->InitPixelIterator ());
    unsigned int maiorValor = 0;
    //pi->Reset();
    for (; !pi->ImageWrapped (); pi->Next ())
    {
        if (pi->Get () > maiorValor)
            maiorValor = pi->Get ();
    }
    delete pi;
    return maiorValor;
}

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/
//----------------------------------------------------------------------------
//Fun��o que retorna o maior valor na imagem.
uint32
TOperacoesImg::GetMaxValueFrom32bppImage (I3DRawImage * img32bpp)
{
    if (img32bpp->BPP () != 32)
        return 0;
    I3DPixelIterator32 *pi =
        I3DPixelIterator32::SafeCast (img32bpp->InitPixelIterator ());
    uint32 maiorValor = 0;
    for (; !pi->ImageWrapped (); pi->Next ())
    {
        if (pi->Get () > maiorValor)
            maiorValor = pi->Get ();
    }
    delete pi;
    return maiorValor;
}

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/
//-------------------------------------------------------------------------------
//Fun��o que determina o numero de pixeis em uma imagem 3D
//usada para determinação da porosidade da imagem
uint32
TOperacoesImg::GetNumberOfPixelsActived (I3DRawImage * imgBinaria)
{
    if (imgBinaria->BPP () != 1)
        return 0;
    uint32 numberOfPixelActived = 0;
    I3DPixelIterator1 *piimgBinaria =
        I3DPixelIterator1::SafeCast (imgBinaria->InitPixelIterator ());
    for (; !piimgBinaria->ImageWrapped (); piimgBinaria->Next ())
        if (piimgBinaria->Get () == 1)
            numberOfPixelActived++;
    delete piimgBinaria;
    return numberOfPixelActived;
}

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/
//-------------------------------------------------------------------------------
//Dada uma imagem imgDados, e dois pontos3D (xi,yi,zi) (xf,yf,zf),
//retorna uma imagem do bloco definido pelos dois pontos.
//ou seja, retorna uma imagem nova definida pelos dois pontos.
//Adicionalmente acrescentou-se o corte, que objetiva aplicar um corte sobre a imagem,
//diminuindo a imagem final, isto se justifica, porque as configuracoes de equilibrio
//tendem a ter imagens desconectadas, e para o cálculo da permeabilidade as mesmas
//precisam estar conectadas. Normalmente o valor de corte � de 2 p�xeis.

//static I3DRawImage*  ExtractImage( I3DRawImage* imgDados, int xi,int yi, int zi, int xf, int yf, int zf, int corte = 0 );
//I3DRawImage*  TOperacoesImg::ObtainShelledRectangle( I3DRawImage* imgDados, int xi,int yi, int zi, int xf, int yf, int zf, int corte   )
I3DRawImage *
TOperacoesImg::GetNewImgShelled (I3DRawImage * imgDados,
                                 unsigned int xi, unsigned int yi,
                                 unsigned int zi, unsigned int xf,
                                 unsigned int yf, unsigned int zf,
                                 unsigned int corte)
{
//SaveImg( imgDados, "imagemInput-TOI-GetNewImgShelled.txt");//debug
    if (imgDados->BPP () != 1)	//se a imagem passada n�o for bin�ria, retorna zero
    {				//novo debug
        assert (0);		//Sai do programa
        return 0;
    }
    //Define o tamanho da imagem
    int nx = xf - xi - 2 * corte;	//+1;
    if (nx < 1)
        nx = 1;
    int ny = yf - yi - 2 * corte;	//+1;
    if (ny < 1)
        ny = 1;
    int nz = zf - zi - 2 * corte;	//+1;
    if (nz < 1)
        nz = 1;

    //Cria imagem nova, bin�ria, do tamanho da regi�o interna passada
    //+1 pois se xi=0 e xf=10 xf-xi=10, mas tem 11 pontos ????
    //I3DRawImage* img = new I3DRawImage(T3Dint32(nx,ny,nz),1);
    I3DRawImage *img = new I3DRawImage (T3Dint32 (nx, ny, nz), 1);
    assert (img != NULL);
    //Percorre a imagem definindo os valores
    I3DPixelIterator1 *piimgDados =
        I3DPixelIterator1::SafeCast (imgDados->InitPixelIterator ());
    I3DPixelIterator1 *piimg =
        I3DPixelIterator1::SafeCast (img->InitPixelIterator ());
    int ixi;
    int jyi;
    //int kzi;
    for (int i = 0; i < img->Width (); i++)	//Percore a imagem de dados
    {
        ixi = +xi + corte + i;
        for (int j = 0; j < img->Height (); j++)
        {
            jyi = +yi + corte + j;
            for (int k = 0; k < img->Depth (); k++)
                piimg->Set (i, j, k, piimgDados->Get (ixi, jyi, +zi + corte + k));

        }
    }
    delete piimgDados;		//novo
    delete piimg;
//SaveImg( imgDados, "imagemOutput-TOI-GetNewImgShelled.txt");//debug
    img->SetVoxelSize (imgDados->VoxelSize ());
    img->SetAmplifyFactor (imgDados->AmplifyFactor ());

    return img;
}

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/
//-------------------------------------------------------------------------------
//Recebe uma imagem, e retorna o maior raio de bola para esta imagem
//Determina o raioMaximo como sugest�o
//Pode receber uma imagem bin�ria ou uma imagem 16bpp,
//Se receber uma imagem 16bpp calcula maxRadius direto,
//Se receber uma imagem 1bpp, cria objeto idf, cria imagem idf e ent�o calcula maxRadius
//PS: Antes s� valia para imagem 1bpp, agora pode ser 1bpp ou 16bpp
unsigned int
TOperacoesImg::GetMaxIDFRadius (I3DRawImage * img3D)
{
    I3DRawImage *imgidfTemp = img3D;	//imgidfTemp aponta para a imagem recebida
    if (img3D->BPP () == 1)	//Se a imagem recebida for bin�ria, determina a idf 16bpp
    {
        I3DFltD345 fidf;		//Objeto idf
        fidf.Go (img3D);		//Calcula a idf
        imgidfTemp = fidf.Image ();	//Obtem imagem idf 16bpp
        assert (imgidfTemp != NULL);	//Aqui a imgidfTemp � uma imagem 16bpp
    }
    if (imgidfTemp->BPP () != 16)	//Se a imgidfTemp n�o for 16bpp, sai
    {
        assert (0);		//Sai do programa //novo debug
        return 0;
    }
    unsigned int maxValue = GetMaxValueFrom16bppImage (imgidfTemp);	//obtem maior valor
    unsigned int maxRadiusIDF = (maxValue % 3 == 0) ? maxValue / 3 : 1 + maxValue / 3;	//calcula maior raio
    if (img3D->BPP () == 1)	//se a imagem recebida for bin�ria,
        delete imgidfTemp;		//alocou a imgidfTemp, aqui  deleta a mesma
    return maxRadiusIDF;		//retorna o maior raio
}

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/
//-------------------------------------------------------------------------------
//Verifica o plano zero
//Retorna true se tem algum pixel ativo no plano 0
//Fun��o nova
bool
TOperacoesImg::ActivePixelInFirstPlane (I3DRawImage * img)
{
    bool conectada = false;
    I3DPixelIterator1 *piimg =
        I3DPixelIterator1::SafeCast (img->InitPixelIterator ());
    for (; !piimg->PlaneWrapped (); piimg->Next ())	//Percorre o plano 0
        if (piimg->Get () != 0)	//se tem algum pixel !=0
        {
            conectada = true;	//seta como conectada
            delete piimg;
            return conectada;	//e encerra loop
        }
    delete piimg;
    return conectada;
}

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/
//-------------------------------------------------------------------------------
//Verifica o ultimo plano
//Retorna true se tem algum pixel ativo no ultimo plano 0
//bug novo alterar corrigir, esta funcao precisa ser corrigida e testada
bool
TOperacoesImg::ActivePixelInLastPlane (I3DRawImage * img)
{
    bool conectada = false;
    I3DPixelIterator1 *piimg =
        I3DPixelIterator1::SafeCast (img->InitPixelIterator ());

    int plane = img->Depth () - 1;
    for (int i = 0; i < img->Width (); i++)
        for (int j = 0; j < img->Height (); j++)
            if (piimg->Get (i, j, plane) != 0)	//se tem algum pixel !=0
            {
                conectada = true;	//seta como conectada
                delete piimg;
                return conectada;	//encerra a fun��o
            }
    delete piimg;
    return conectada;
}

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/
//-------------------------------------------------------------------------------
//Verifica se o primeiro e o ultimo plano cont�m p�xeis ativos, se afirmativo
//a imagem � conectada.
bool
TOperacoesImg::Conected (I3DRawImage * img)
{
    //Novo debug
    assert (img->BPP () == 1);	//Se for diferente de 1bpp sai do programa
    bool first = ActivePixelInFirstPlane (img);
    bool last = ActivePixelInLastPlane (img);
    return (first && last);
}

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/
//-------------------------------------------------------------------------------
//Altera��o 1: Cria e aplica um filtro que retorna uma imagem nova, toda conectada.
I3DRawImage *
TOperacoesImg::GetCopyConected (I3DRawImage * img)
{
    /*OLD
       I3DFltConnect flt;//Cria o filtro de conectividade
       flt.Go(img);//Executa
       I3DRawImage* imgConect =  flt.Image();//Obt�m imagem
       assert(imgConect !=NULL);//Testa

    	imgConect->SetVoxelSize(img->VoxelSize());//novo
    	imgConect->SetAmplifyFactor(img->AmplifyFactor());//novo
    */
    //aquiaqui novo
    I3DFltLabel fltLabel (lnD26);
    I3DFltConnect flt (&fltLabel);//Cria o filtro de conectividade
    flt.Go (img);			//Executa
    I3DRawImage *imgConect = flt.Image ();//Obtm imagem
    assert (imgConect != NULL);	//Testa

    imgConect->SetVoxelSize (img->VoxelSize ());	//novo
    imgConect->SetAmplifyFactor (img->AmplifyFactor ());	//novo
    return imgConect;
}

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/
//-------------------------------------------------------------------------------
//Altera��o 1: Cria e aplica um filtro que retorna uma imagem nova, toda conectada.
//Observe que retorna uma imagem do tipo rotulador 3D
//static
//TRotulador3D*GetTMatriz3D ( I3DRawImage * img1bpp);
TRotulador3D *
TOperacoesImg::GetTMatriz3D (I3DRawImage * img1bpp)
{
    //Cria imagem tridimensional

    //TMatriz3D* img3D = new TMatriz3D(img1bpp->Width(),img1bpp->Height(),img1bpp->Depth());
    TRotulador3D *pm3D =
        new TRotulador3D (img1bpp->Width (), img1bpp->Height (),
                          img1bpp->Depth ());

    //Copia imagem do imago para 3D da lib_lmpt
    I3DPixelIterator1 *piimg1bpp =
        I3DPixelIterator1::SafeCast (img1bpp->InitPixelIterator ());
    for (int k = 0; k < img1bpp->Depth (); k++)
        for (int j = 0; j < img1bpp->Height (); j++)
            for (int i = 0; i < img1bpp->Width (); i++)
                pm3D->data3D[i][j][k] = piimg1bpp->Get (i, j, k);
    delete piimg1bpp;

    return pm3D;
}






/*I3DRawImage* TOperacoesImg::OpenImg1bpp( string nomeArquivo,
						unsigned int nx, unsigned int ny, unsigned int nz )       */
