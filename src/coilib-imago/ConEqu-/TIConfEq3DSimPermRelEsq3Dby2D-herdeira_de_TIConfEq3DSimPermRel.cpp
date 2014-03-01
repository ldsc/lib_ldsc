//=======================================================================
//PROJETO:          Biblioteca LIB_LMPT
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
#include <cstring>
#include <cstdlib>
//COI
//#include <win/window.h>
//Imago
#include <imago/img3d/pixit3d1.h>
#include <imago/img3d/pixit3d16.h>	//iterators para pixeis
#include <imago/img3d/pixit3d32.h>	//iterators para pixeis

//#include <imago/img3d/classify.h>     //classifica��o
#include <imago/flt3d/flt3dcon.h>	//Retorna imagem conectada
//#include <imago/img3d/skesimul.h>     //simula��o permeabilidade pelo esqueleto

//ConfEq
//#include <Base/_LIB_LMPT_CLASS.h>
#include <TConEquCOILIB/TIConfEq3DSimPermRelEsq3Dby2D.h>
#include <TConversaoImago/TOperacoesImg.h>	//inclue o objeto, declaracao da classe

// -------------------------------------------------------------- Objetos grafo
//#define __NAO_INCLUIR_MIN_MAX__
//#Para tentar resolver o conflito com min e max inclui o flag        -D__BORLANDC__
//CXXFLAGS+= -D__ANDRE_STL_ALGOBASE_H__
#define  __ANDRE_STL_ALGOBASE_H__	//aqui aqui aqui
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
//Fun��o de calculo da permeabilidade em s�.
//calcula direto, sem o uso do di�logo.
double TIConfEq3DSimPermRelEsq3Dby2D::CalculaPermeabilidade (I3DRawImage * &imgConnect)
{
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
        unsigned long int limiteIteracoes = 100;
        long double limiteErro = 1.0e-6;

        //Cria objeto fluido do tipo TMFluido------------------------------------------
        TMFluido *fluido = new TMFluido (viscosidade);

        //Cria objeto solver do tipo TSMDSOR------------------------------------------
        TSMDiagonalDominante *solver =
            new TSMDSOR (limiteIteracoes, limiteErro, fatorRelaxacao);
        assert (solver);

        //Cria objeto de classificacao do tipo TGrafo--------------------------------------
        time_t inicio = time (NULL);
        TGra3Dby2D *grafo;
        char nomeArquivo2[256];
        //O tipo de grafo � passado no construtor
        switch (tipoGrafo)
        {
        case 1:
            sprintf (nomeArquivo2, "%s.mod1", nomeArquivo);
            grafo = new TGra3Dby2DSingle (nomeArquivo2);
            break;
        case 2:
            sprintf (nomeArquivo2, "%s.mod2", nomeArquivo);
            grafo = new TGra3Dby2DAdvanced (nomeArquivo2);
            break;
        case 3:
            sprintf (nomeArquivo2, "%s.mod3", nomeArquivo);
            grafo = new TGra3Dby2DAdv3 (nomeArquivo2);
            break;
        default:
        case 4:
            sprintf (nomeArquivo2, "%s.mod4", nomeArquivo);
            grafo = new TGra3Dby2DAdv4 (nomeArquivo2);
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
