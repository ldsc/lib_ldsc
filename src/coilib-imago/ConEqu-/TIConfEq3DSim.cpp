//=======================================================================
//PROJETO:          Biblioteca LIB_LMPT
//                  Ramo: TPadrao_ramo
//=======================================================================
//
//Desenvolvido por: Laboratorio de Meios Porosos
//e Propriedades Termofisicas     [LMPT].
//@author:          André Duarte Bueno
//File:             TIConfeq3DSim.cpp
//begin:            Fri Nov 17 2000
//copyright:        (C) 2000 by André Duarte Bueno
//email:            andre@lmpt.ufsc.br.

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
@see    :
@param  :
@return :
*/


//#ifndef TIConfEq3DSim_cpp
// #define TIConfEq3DSim_cpp

/*
----------------------------------------------------------------------------
PROJETO:		Imago
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   	Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi,
			Andre D.Bueno,Luiz Orlando Emerich, Marcos Damiani,...
Copyright @1997-1999:   Todos os direitos reservados.
Nome deste arquivo:	TIConfEq3DSim.h
Nome da classe:         TIConfEq3DSim
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/
//--------------------------------------------------------------------------
//C++
//--------------------------------------------------------------------------
#include <stdio.h>
#include <iomanip.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <math.h>		//cos(teta)

//--------------------------------------------------------------------------
//COI20
//--------------------------------------------------------------------------
#include <coi20/sys/core/system.h>

//--------------------------------------------------------------------------
// Imago
//--------------------------------------------------------------------------
//aquiaqui
//#include <imago/img3d/pixit3d1.h>
//#include <imago/img3d/pixit3d16.h>
//#include <imago/img3d/pixit3d32.h>
#include <imago/img3d/pixit/pixit1.h>
#include <imago/img3d/pixit/pixit16.h>
#include <imago/img3d/pixit/pixit32.h>

// /usr/include/imago/img3d/image/raw.h
#include <imago/img3d/image/raw.h>

//--------------------------------------------------------------------------
// LIB_LMPT
//--------------------------------------------------------------------------
#include <Base/_LIB_LMPT_CLASS.h>
#include <TConEquCOILIB/TIConfEq3DSim.h>

//Objetos de convers�o de imagens e formatos
#include <TConversaoImago/TOperacoesImg.h>

//A pedido
COI20_CLASSID_DEF1 (TIConfEq3DSim, TIConfiguracoesEquilibrio3D);

//Cria objeto de configura��es de equilibrio e armazena em disco
//cabe�alho de arquivo com resultado da simula��o
//Dimensoes imagem:
//nx:                                                           50
//ny:                                                           50
//nz:                                                           50
//Volume regi�o v�lida:                 125452
//Porosidade regi�o v�lida:   .2515
//Passo Raio Fluido_B  Fluido_A Fluido_A_isolado Tempo(ms)
void
TIConfEq3DSim::PrepareConfEqSim ()
{
    cout << "\nfuncao TIConfEq3DSim::PrepareConfEqSim()" << endl;
    //timeInit = time(NULL);
    //Anota hora inicio simula��o (cria��o do objeto)
    //Vou precisar da porosidade e do volume v�lido
    timeInit = ISystem::Object ()->Time ();

    //Calcula porosidade da regi�o v�lida da imagem inicial
    //j� calcula o volume e a porosidade
    Getchamber ()->PorosityRegionPorousMedia ();

    //Cria  arquivo de disco, e preenche o cabe�alho
    //fout.open( GetNomeArquivo().c_str() );         em 22-7-2002
    fout.open (GetNomeArquivo ().c_str (), ios::app);
    if (fout.fail ())
        return;

    fout << "#\n---------------------------------------------------------" <<
    endl;
    fout << "#Resultado da simula��o" << endl;

    //Sai dados objeto confEq, fluidos, chamber
    fout << *this;

    /* fout << "\n---------------------------------------------------------"<<endl;
       fout << "Resultado da simula��o"<<endl;
       fout << "Dados do objeto camara:"<<endl;
       chamber->Output(fout);              		//Sai com os dados da camara

       fout << "\n---------------------------------------------------------"<<endl;
       fout << "Dados do objeto configura��es de equil�brio:"<<endl;

       this->Output (fout);
    */

    fout << "#\n---------------------------------------------------------" <<
    endl;
    fout << "#Dados dinamicos da simulac�o:" << endl;
    fout <<
    "#Passo        Raio         Fluido_B     Fluido_A     F_A_isolado  Fluido_B_Tot Fluido_A_Tot Tempo(ms)    "
    << endl;
    fout.close ();
}

//Fun��o redefinida, inclue a chamada da fun��o PrepareConfEqSim
bool
TIConfEq3DSim::Next ()
{
    cout << " funcao TIConfEq3DSim::Next()" << endl;

    //se ainda n�o ocorreu nenhum passo,
    if (pass == 1)
    {
        cout << " chamando funcao PrepareConfEqSim" << endl;

        //Escreva cabe�alho do arquivo de disco
        PrepareConfEqSim ();
    }
    cout << " chamando funcao TIConfiguracoesEquilibrio3D::Next()" << endl;
    //fun��o da classe base;
    return TIConfiguracoesEquilibrio3D::Next ();
};

//Cria as imagens nucleo, que ser�o utilizadas
//no cálculos da IntermediatNextCalculate
//Calculo das satura��es das imagens e evcentualmente
//da permeabilidade relativa
void
TIConfEq3DSim::IntermediatNextCreate ()
{
    nucleoImgB = GetNucleoImgB ();
    assert (nucleoImgB != NULL);
    SaveImg (nucleoImgB, "nucleoImgB-%d.i3d");

    nucleoImgA = GetNucleoImgA ();
    assert (nucleoImgA != NULL);
    SaveImg (nucleoImgA, "nucleoImgA-%d.i3d");

    nucleoImgY = GetNucleoImgY ();
    assert (nucleoImgY != NULL);
    SaveImg (nucleoImgY, "nucleoImgY-%d.i3d");
}

//CALCULO DAS CURVAS DE PRESSAO CAPILAR
void
TIConfEq3DSim::IntermediatNextCalculate ()
{
    double porosidade = Getchamber ()->GetporosityRegionPorousMedia ();
    double volume = (double) chamber->GetvolumeRegionPorousMedia ();

    //{porosidade da imagem=numeroPixeis/volume)
    curvaPressaoCapilarImgB[radius - minRadius] =
        100 * TOI::GetNumberOfPixelsActived (nucleoImgB) / volume / porosidade;
    curvaPressaoCapilarImgA[radius - minRadius] =
        100 * TOI::GetNumberOfPixelsActived (nucleoImgA) / volume / porosidade;
    curvaPressaoCapilarImgY[radius - minRadius] =
        100 * TOI::GetNumberOfPixelsActived (nucleoImgY) / volume / porosidade;
}

void
TIConfEq3DSim::IntermediatNextOutput ()
{
    //Calculo do tempo de processamento
    //Marca o tempo de execu��o
    TSysTime timeEnd;
    timeEnd = ISystem::Object ()->Time ();

    uint32 demora = +(timeEnd.day - timeInit.day) * 24 * 60 * 60 * 100
                    + (timeEnd.hour - timeInit.hour) * 60 * 60 * 100
                    + (timeEnd.minute - timeInit.minute) * 60 * 100
                    + (timeEnd.second - timeInit.second) * 100
                    + (timeEnd.milliseconds - timeInit.milliseconds);

    //SAIDA DE DADOS PARA O DISCO
    fout.open (GetNomeArquivo ().c_str (), ios::app);

    if (fout.fail ())
    {
        cout << "\nN�o conseguiu abrir arquivo de disco:" << GetNomeArquivo () <<
             endl;
        exit (0);
    }
    fout.setf (ios::left);
    fout << "\n";
    fout << setw (13) << pass;
    fout << setw (13) << radius;
    fout << setw (13) << curvaPressaoCapilarImgB[radius - minRadius];
    fout << setw (13) << curvaPressaoCapilarImgA[radius - minRadius];
    fout << setw (13) << curvaPressaoCapilarImgY[radius - minRadius];

    //n�o invertido Y � Aisolado
    if (!inverted)
    {
        fout << setw (13) << curvaPressaoCapilarImgB[radius - minRadius];
        fout << setw (13) << (curvaPressaoCapilarImgA[radius - minRadius] +
                              curvaPressaoCapilarImgY[radius - minRadius]);
    }
    //invertido  Y � B isolado
    else
    {
        fout << setw (13) << (curvaPressaoCapilarImgB[radius - minRadius] +
                              curvaPressaoCapilarImgY[radius - minRadius]);
        fout << setw (13) << curvaPressaoCapilarImgA[radius - minRadius];
    }
    fout << setw (13) << demora;
}

void
TIConfEq3DSim::IntermediatNextClose ()
{
    if (nucleoImgB != NULL)
        delete nucleoImgB;

    if (nucleoImgA != NULL)
        delete nucleoImgA;

    if (nucleoImgY != NULL)
        delete nucleoImgY;

    fout.close ();
}

string
TIConfEq3DSim::GetNomeArquivo ()
{
    //Abaixo o novo inclue o nome da imagem
    //Condi��o normal
    //IF SEM SENTIDO
    //if(inverted == false)
    nomeArquivo = imageName + Getchamber ()->GetClassName () + ".dat";
//aquiaqui
    cout << " no metodo TIConfEq3DSim::GetNomeArquivo()...nomeArquivo=: " << nomeArquivo
         << endl;
    //o fluxo foi invertido, observe a nega��o ! na molhabilidade
    //else
    //  nomeArquivo = imageName + Getchamber()->GetClassName() + ".dat";

    return nomeArquivo;
}

//Fun��o nova retorna a press�o
long double
TIConfEq3DSim::GetPressure (int _raioBola, long double _teta,
                            long double _sigma)
{
    long double pixelSize = Getchamber ()->GetimgPrototypeptr ()->VoxelSize ();
    long double fa = Getchamber ()->GetimgPrototypeptr ()->AmplifyFactor ();
    long double pressao =
        2.0 * _sigma * cos (_teta) / (_raioBola * pixelSize * fa);
    return pressao;
}

//Funo nova retorna o volume da fase molhante
long double
TIConfEq3DSim::GetWeetingVol (int _raioBola)
{
    //Verifica se a bola passada esta com raio vlido
    //ou seja dentro do intervalo minRadius <= raio <= maxRadius
    if (_raioBola < minRadius || _raioBola > maxRadius)
        return 0;

    //Verifica o fluido molhante e retorna o volume associado
    long double sp = Getchamber ()->GetimgPrototypeptr ()->VoxelSize ();
    long double fa = Getchamber ()->GetimgPrototypeptr ()->AmplifyFactor ();
    long double volumePixel = fa * sp * fa * sp * fa * sp;
    long double porosidadeTotal =
        Getchamber ()->GetporosityRegionPorousMedia ();
    long double volume = (double) chamber->GetvolumeRegionPorousMedia ();
    long double numeroPixeis;
    long double saturacaoMolhante;
    long double volumeMolhante;

    //Se o invasor for molhante
    if (fluidIn->Getmolhabilidade () == 1)
    {
        //Retorna a Satura��o
        //porosidadeImagem = TOI::GetNumberOfPixelsActived(nucleoImgA) /volume ;
        //curvaPressaoCapilarImgA[radius - minRadius] =  100 * porosidadeImagem / porosidadeTotal;
        numeroPixeis =
            curvaPressaoCapilarImgB[_raioBola -
                                    minRadius] * porosidadeTotal * volume / 100;
        volumeMolhante = volumePixel * numeroPixeis;
    }
    else				//fluidOut->Getmolhabilidade() == 0
    {
        numeroPixeis =
            curvaPressaoCapilarImgA[_raioBola -
                                    minRadius] * porosidadeTotal * volume / 100 +
            curvaPressaoCapilarImgY[_raioBola -
                                    minRadius] * porosidadeTotal * volume / 100;
        volumeMolhante = volumePixel * numeroPixeis;
    }
    return volumeMolhante;
}

//Funo nova retorna o volume da fase no molhante
long double
TIConfEq3DSim::GetNotWeetingVol (int _raioBola)
{
    //Verifica se a bola passada esta com raio vlido
    //ou seja dentro do intervalo minRadius <= raio <= maxRadius
    if (_raioBola < minRadius || _raioBola > maxRadius)
        return 0;

    //Verifica o fluido molhante e retorna o volume associado
    long double sp = Getchamber ()->GetimgPrototypeptr ()->VoxelSize ();
    long double fa = Getchamber ()->GetimgPrototypeptr ()->AmplifyFactor ();
    long double volumePixel = fa * sp * fa * sp * fa * sp;
    long double porosidadeTotal =
        Getchamber ()->GetporosityRegionPorousMedia ();
    long double volume = (double) chamber->GetvolumeRegionPorousMedia ();
    long double numeroPixeis;
    long double saturacaoMolhante;
    long double volumeNaoMolhante;

    //Se o invasor for no molhante
    if (fluidIn->Getmolhabilidade () == 0)
    {
        //Determina o numero de pixeis a partir da saturao armazenada em curvaPressaoCapilarImgB[ _raioBola - minRadius ]
        numeroPixeis =
            curvaPressaoCapilarImgB[_raioBola -
                                    minRadius] * porosidadeTotal * volume / 100;
        volumeNaoMolhante = volumePixel * numeroPixeis;
    }
    else
    {
        //Determina o numero de pixeis a partir da saturao armazenada em curvaPressaoCapilarImgB[ _raioBola - minRadius ]
        numeroPixeis =
            curvaPressaoCapilarImgA[_raioBola -
                                    minRadius] * porosidadeTotal * volume / 100 +
            curvaPressaoCapilarImgY[_raioBola -
                                    minRadius] * porosidadeTotal * volume / 100;
        volumeNaoMolhante = volumePixel * numeroPixeis;
    }
    return volumeNaoMolhante;
}
