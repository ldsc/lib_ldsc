//=======================================================================
//PROJETO:          Biblioteca LIB_LMPT
//                  Ramo: TPadrao_ramo
//=======================================================================
//
//Desenvolvido por: Laboratorio de Meios Porosos
//e Propriedades Termofisicas     [LMPT].
//@author:          André Duarte Bueno
//File:             TOConfEq3DSimPermRel.cpp
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
#include <Base/_LIB_LMPT_CLASS.h>
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

// -------------------------------------------------------------- Objetos cálculo permeabilidade
#include <TPermeabilidade/TPermeabilidadeGrafo.h>

//NA classe TIConfEq3DSim usa:
//COI20_CLASSID_DEF1( TIConfEq3DSim, TIConfiguracoesEquilibrio3D );
//NA classe TIConfEq3DSimPermRel usa:
//COI20_CLASSID_DEF1(TIConfEq3DSimPermRel , ITask )

//aqui aqui aqui COI20_CLASSID_DEF1( TIConfEq3DSimPermRelEsq3Dby2D, ITask )
COI20_CLASSID_DEF1 (TIConfEq3DSimPermRelEsq3Dby2D, TIConfEq3DSim)
//Chama PrepareConfEq da classe base, que monta um arquivo de disco com os dados do objeto confeq.
//Aqui, adiciona no arquivo de disco a PR_B e PR_A, e
//adicionalmente calcula a permeabilidade intrinseca
void TIConfEq3DSimPermRelEsq3Dby2D::PrepareConfEqSim ()
{
    TIConfEq3DSim::PrepareConfEqSim ();	//Executa funcao da classe base, que preenche cabe�alho

    //Calcula a permeabilidade intrinseca
    I3DRawImage *imagem = chamber->GetimgPorousMedia ();	//obtem imagem
    assert (imagem != NULL);
    //      SaveImg1bpp ( imagem, "imagem_chamber_GetimgPorousMedia.i3d");                  //salva imagem usada para cálculo da permeabilidade intrinseca

    I3DRawImage *imgNucleo = chamber->GetImgNucleo (imagem, borderDiscount);	//obtem imagem do n�cleo
    delete imagem;		//deleta imagem parcial
    assert (imgNucleo != NULL);
    //      SaveImg1bpp ( imgNucleo, "imagem_chamber_GetimgPorousMedia_nucleo.i3d"); //salva imagem usada para cálculo da permeabilidade intrinseca
    //intrinsicPermeability = TIConfEq3DSimPermRel::CalculaPermeabilidade(imgNucleo); //calcula permeabilidade intrinseca
    intrinsicPermeability = CalculaPermeabilidade (imgNucleo);	//calcula permeabilidade intrinseca
    delete imgNucleo;

    ofstream fout (GetNomeArquivo (), ios::app);	//Abre arquivo de disco
    if (fout.fail ())
        return;			//Adiciona permeabilidade
    fout << "     PR_B     PR_A";
    fout << "    intrinsicPermeability = " << intrinsicPermeability;
    fout.close ();
}

//Chama a fun��o da classe base, e adicionalmente determina as permeabilidades relativas
//dos n�cleos das imagens A e B
void
TIConfEq3DSimPermRelEsq3Dby2D::IntermediatNextCalculate ()
{
    TIConfEq3DSim::IntermediatNextCalculate ();	//cálculo das curvas de press�o capilar na classe base

    //Calculo da permeabilidade relativa para FLUIDO B (imgB)
    prfb[radius - minRadius] = CalculaPermeabilidade (nucleoImgB);	//intrinsicPermeability ;

    //Calculo das permeabilidade relativa para FLUIDO A (imgA)
    prfa[radius - minRadius] = CalculaPermeabilidade (nucleoImgA);	//  / intrinsicPermeability ;
}

void
TIConfEq3DSimPermRelEsq3Dby2D::IntermediatNextOutput ()
{
    TIConfEq3DSim::IntermediatNextOutput ();	//Classe base, sa�da dados curvas de press�o capilar
    fout << setw (16) << prfb[radius - minRadius] << setw (16) << prfa[radius -
            minRadius];
}

//Fun��o de cálculo da permeabilidade em s�.
//calcula direto, sem o uso do di�logo
/*
  double  TIConfEq3DSimPermRel::CalculaPermeabilidade(I3DRawImage*& img)
  {
  double perm=0;
  if(TOI::Conected(img) == true)		//Verifica se a imagem esta conectada
  {
Classify cls;					//Cria filtro de classificacao
  if (cls.Go(img))				//Executa classifica��o
  {
  ISkeSimulation simul;				//Cria objeto simula��o esqueleto
  perm = simul.Simulate( cls.Image(), cls.Nodes(), cls.Viscosity() );//executa simula��o
  }
  }
  return perm;				        //Retorna a permeabilidade
  }
*/

//Fun��o de cálculo da permeabilidade em s�.
//calcula direto, sem o uso do di�logo.
double
TIConfEq3DSimPermRelEsq3Dby2D::CalculaPermeabilidade (I3DRawImage *
        &imgConnect)
{
    double perm = 0;

    //Verifica se a imagem esta conectada
    if (TOI::Conected (imgConnect) == true)
    {
        TRotulador3D *pm3D = TOperacoesImg::GetTMatriz3D (imgConnect);
        assert (pm3D);

        //PROPRIEDADES DA SIMULA��O
        int fatorAmplificacao = imgConnect->AmplifyFactor ();
        double dimensaoPixel = imgConnect->VoxelSize ();
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

        //Cria objeto de cálculo da permeabilidade-------------------------------------
        TPermeabilidadeGrafo *permeabilidade =
            new TPermeabilidadeGrafo (fluido, solver, grafo, pm3D->GetNX (),
                                      pm3D->GetNY (), pm3D->GetNZ (),
                                      fatorAmplificacao, dimensaoPixel);
        assert (permeabilidade);

        //Chama fun��o de inicializa��o do sistema de cálculo da permeabilidade------------------
        //Seta condi��es de contorno, valores iniciais, chama solver pela 1 vez
        permeabilidade->SolucaoSistema ();

        //Chama fun��o de cálculo da permeabilidade-----------------------------------
        perm = permeabilidade->Go ();
        delete pm3D;
        delete permeabilidade;
    }
    //Retorna a permeabilidade
    return perm;
}
