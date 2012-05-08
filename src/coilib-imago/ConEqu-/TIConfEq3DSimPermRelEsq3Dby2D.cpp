//=======================================================================
//PROJETO:          Biblioteca LIB_LMPT.
//                  Ramo: TPadrao_ramo
//=======================================================================
//
//Desenvolvido por: Laboratorio de Meios Porosos
//                  e Propriedades Termofisicas     [LMPT].
//@author:          Andr� Duarte Bueno
//File:             TOConfEq3DSimPermRel.cpp
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

//C++
#include <cstdio>
#include <iomanip>
//#include <cstring>
#include <string>
#include <cstdlib>
//COI
//#include <win/window.h>
//Imago
//#include <imago/img3d/pixit3d1.h>
//#include <imago/img3d/pixit3d16.h>
//#include <imago/img3d/pixit3d32.h>
#include <imago/img3d/pixit/pixit1.h>
#include <imago/img3d/pixit/pixit16.h>
#include <imago/img3d/pixit/pixit32.h>

//classifica��o
//#include <imago/img3d/classify.h>
//#include <imago/flt3d/flt3dcon.h>
//#include <imago/img3d/skesimul.h>
#include <imago/img3d/skeleton/classify.h>
#include <imago/flt3d/core/connect.h>
#include <imago/img3d/skeleton/simulation.h>



//ConfEq
//#include <Base/_LIB_LMPT_CLASS.h>
#include <TConEquCOILIB/TIConfEq3DSimPermRelEsq3Dby2D.h>

//inclue o objeto, declaracao da classe
#include <TConversaoImago/TOperacoesImg.h>

// -------------------------------------------------------------- Objetos grafo
//#define __NAO_INCLUIR_MIN_MAX__
//#Para tentar resolver o conflito com min e max inclui o flag
//-D__BORLANDC__
//CXXFLAGS+= -D__ANDRE_STL_ALGOBASE_H__
//
#define  __ANDRE_STL_ALGOBASE_H__
#include <TGrafo/TGra3Dby2DSingle.h>
#include <TGrafo/TGra3Dby2DAdvanced.h>
#include <TGrafo/TGra3Dby2DAdv3.h>
#include <TGrafo/TGra3Dby2DAdv4.h>

// -------------------------------------------------------------- Objetos fluido e solver
#include <TMaterial/TMFluido.h>
#include <TSMatriz/TSMDiagonal/TSMDSOR.h>

// -------------------------------------------------------------- Objetos calculo permeabilidade
#include <TPermeabilidade/TPermeabilidadeGrafo.h>

//NA classe TIConfEq3DSim usa:
//COI20_CLASSID_DEF1( TIConfEq3DSim, TIConfiguracoesEquilibrio3D );
//NA classe TIConfEq3DSimPermRel usa:
//COI20_CLASSID_DEF1( TIConfEq3DSimPermRel, ITask ) ???
//NA classe TIConfEq3DSimPermRelEsq3Dby2D usa:
//  COI20_CLASSID_DEF1( TIConfEq3DSimPermRelEsq3Dby2D, ITask )compilador reclama
COI20_CLASSID_DEF1 (TIConfEq3DSimPermRelEsq3Dby2D, TIConfEq3DSimPermRel)
////////////////////////////////////////////////////////////////////////////
//BUG
//A chamada a fun��o Calcula permeabilidade estava acessando
//a funcao da classe TIConfEq3DSimPermRel::CalculaPermeabilidade
//e nao a CalculaPermeabilidade desta classe.
//Este problema so ocorria no calculo da permebilidade intrinseca.
//Fiz uma copia da fun��o PrepareConfEqSim() para verificar se o problema
//cessa.
//Depois verificar onde esta o problema, e eliminar esta fun��o
//pois a mesma � uma copia da TIConfEq3DSimPermRel::CalculaPermeabilidade
////////////////////////////////////////////////////////////////////////////
void TIConfEq3DSimPermRelEsq3Dby2D::PrepareConfEqSim ()
{
    cout << " TIConfEq3DSimPermRelEsq3Dby2D::PrepareConfEqSim()" << endl;
    cout <<
         " TIConfEq3DSimPermRelEsq3Dby2D::PrepareConfEqSim()->chama TIConfEq3DSim::PrepareConfEqSim()"
         << endl;

    //Executa funcao da classe base, que preenche cabe�alho
    TIConfEq3DSim::PrepareConfEqSim ();

    //Calcula a permeabilidade intrinseca obtem imagem
    I3DRawImage *imagem = chamber->GetimgPorousMedia ();
    assert (imagem != NULL);

    //salva imagem usada para calculo da permeabilidade intrinseca
    //SaveImg1bpp ( imagem, "imagem_chamber_GetimgPorousMedia.i3d");

    //Obtem imagem do n�cleo
//aquiaqui
    cout << "Borda utilizada= " << borderDiscount << endl;
    I3DRawImage *imgNucleo = chamber->GetImgNucleo (imagem, borderDiscount);
    //deleta imagem parcial
    delete imagem;
    assert (imgNucleo != NULL);

    //salva imagem usada para calculo da permeabilidade intrinseca
    //SaveImg1bpp ( imgNucleo, "imagem_chamber_GetimgPorousMedia_nucleo.i3d");
    //calcula permeabilidade intrinseca
    //intrinsicPermeability = TIConfEq3DSimPermRel::CalculaPermeabilidade(imgNucleo);
    cout <<
         " TIConfEq3DSimPermRelEsq3Dby2D::PrepareConfEqSim()->chama CalculaPermeabilidade(imgNucleo)"
         << endl;
    intrinsicPermeability = CalculaPermeabilidade (imgNucleo);	//calcula permeabilidade intrinseca
    delete imgNucleo;

    //Abre arquivo de disco
    //ofstream fout(GetFileName().c_str(),ios::app);
    fout.open (GetFileName ().c_str (), ios::app);

    //Adiciona permeabilidade
    if (fout.fail ())
        return;
    fout << "#PR_B     PR_A";
    fout << "\nintrinsicPermeability = " << intrinsicPermeability << endl;
    fout.close ();
}

//Fun��o de calculo da permeabilidade em s�.
//calcula direto, sem o uso do di�logo.
double
TIConfEq3DSimPermRelEsq3Dby2D::CalculaPermeabilidade (I3DRawImage *
        &imgConnect)
{
    cout <<
         " funcao TIConfEq3DSimPermRelEsq3Dby2D::CalculaPermeabilidade(I3DRawImage*& imgConnect)"
         << endl;
    double perm = 0;

    //Verifica se a imagem esta conectada
    if (TOI::Conected (imgConnect) == true)
    {
        TRotulador3D *pm3D = TOperacoesImg::GetTMatriz3D (imgConnect);
        assert (pm3D);

        //PROPRIEDADES DA SIMULA��O
        int fatorAmplificacao = imgConnect->AmplifyFactor ();
        double sizePixel = imgConnect->VoxelSize ();
        long double viscosidade = 0.001002;
        long double fatorRelaxacao = 0.7;
        unsigned long int limiteIteracoes = 1000;
        long double limiteErro = 1.0e-8;
//aquiaqui
        cout << "\nfatorAmplificacao = " << imgConnect->AmplifyFactor ()
             << "\nsizePixel =  " << imgConnect->VoxelSize ()
             << "\nfatorRelaxacao = " << fatorRelaxacao
             << "\nlimiteIteracoes =" << limiteIteracoes
             << "\nlimiteErro =" << limiteErro << endl;

        //Cria objeto fluido do tipo TMFluido------------------------------------------
        TMFluido *fluido = new TMFluido (viscosidade);

        //Cria objeto solver do tipo TSMDSOR------------------------------------------
        TSMDiagonalDominante *solver =
            new TSMDSOR (limiteIteracoes, limiteErro, fatorRelaxacao);
        assert (solver);

        //Cria objeto de classificacao do tipo TGrafo--------------------------------------
        time_t inicio = time (NULL);
        TGra3Dby2D *grafo;

        string fileNameGrafoFormatado;
        //O tipo de grafo � passado no construtor
        switch (tipoGrafo)
        {
        case 1:
            fileNameGrafoFormatado = fileNameSaidaGrafo + ".mod1";
            grafo = new TGra3Dby2DSingle (fileNameGrafoFormatado.c_str ());
            break;
        case 2:
            fileNameGrafoFormatado = fileNameSaidaGrafo + ".mod2";
            grafo = new TGra3Dby2DAdvanced (fileNameGrafoFormatado.c_str ());
            break;
        case 3:
            fileNameGrafoFormatado = fileNameSaidaGrafo + ".mod3";
            grafo = new TGra3Dby2DAdv3 (fileNameGrafoFormatado.c_str ());
            break;
        default:
        case 4:
            fileNameGrafoFormatado = fileNameSaidaGrafo + ".mod4";
            grafo = new TGra3Dby2DAdv4 (fileNameGrafoFormatado.c_str ());
            break;
        };
        assert (grafo);

        //Determina o grafo---------------------------------------------------
        grafo->Go (pm3D);

        //Cria objeto de calculo da permeabilidade-------------------------------------
        TPermeabilidadeGrafo *permeabilidade =
            new TPermeabilidadeGrafo (fluido, solver, grafo, pm3D->GetNX (),
                                      pm3D->GetNY (), pm3D->GetNZ (),
                                      fatorAmplificacao, sizePixel);
        assert (permeabilidade);

        //Chama fun��o de inicializa��o do sistema de calculo da permeabilidade------------------
        //Seta condi��es de contorno, valores iniciais, chama solver pela 1 vez
        permeabilidade->SolucaoSistema ();

        //Chama fun��o de calculo da permeabilidade-----------------------------------
        perm = permeabilidade->Go ();
        delete pm3D;
        delete permeabilidade;
    }
//Retorna a permeabilidade
    return perm;
}
