//=======================================================================
//PROJETO:          Biblioteca LIB_LMPT
//                  Ramo: TPadrao_ramo   BUG
//=======================================================================
//
//Desenvolvido por: Laboratorio de Meios Porosos
//                  e Propriedades Termofisicas     [LMPT].
//@author:          André Duarte Bueno
//File:             TIConfiguracoesEquilibrio3D.cpp
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
@see    :
@param  :
@return :
*/


//#ifndef TIConfiguracoesEquilibrio3D_cpp
// #define TIConfiguracoesEquilibrio3D_cpp

/*
----------------------------------------------------------------------------
PROJETO:			Imago
----------------------------------------------------------------------------

Desenvolvido por:  Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:     Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi,
		Andre D.Bueno,Luiz Orlando Emerich, Marcos Damiani,...
Copyright @1997-1999: Todos os direitos reservados.
Nome deste arquivo:	  TIConfiguracoesEquilibrio3D.h
Nome da classe:       TIConfiguracoesEquilibrio3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/
//C
//Fun��es da biblioteca de C
#include <cstdio>
//strean's do C++
#include <fstream>

//#include <time.h>
//COI
//#include <win/window.h>
//Imago
//#include <imago/img3d/pixit3d1.h>
//#include <imago/img3d/pixit3d8.h>
//#include <imago/img3d/pixit3d16.h>
//#include <imago/img3d/pixit3d32.h>
#include <imago/img3d/pixit/pixit1.h>
#include <imago/img3d/pixit/pixit8.h>
#include <imago/img3d/pixit/pixit16.h>
#include <imago/img3d/pixit/pixit32.h>
//Filtro idf
//#include <imago/flt3d/flt3dd345.h>
#include <imago/flt3d/core/d345.h>

//Abertura
//#include <imago/flt3d/flt3dop.h>
#include <imago/flt3d/core/openning.h>

//Filtro rotulagem
//#include <flt3d/Flt3dlb.h>
//Filtro rotulagem
//#include <imago/flt3d/flt3dlb.h>
#include <imago/flt3d/core/label.h>

//Funcao para salvar imagem em disco IWriteI3D
//#include <imago/img3d/i3d.h>
#include <imago/img3d/io/i3d.h>

//TConfiguracoesEquilibrio
//classe
#include <Base/_LIB_LMPT_CLASS.h>
#include <TConEquCOILIB/TIConfiguracoesEquilibrio3D.h>
//Camara simples
#include <TConEquCOILIB/TISChamber.h>
//verifica��o da conectividade
#include <TConectividade/TFEConectividade3d.h>
//Objetos de convers�o de imagens e formatos
#include <TConversaoImago/TOperacoesImg.h>

//A pedido aqui
COI20_CLASSID_DEF1 (TIConfiguracoesEquilibrio3D, ITask)
//Construtor
TIConfiguracoesEquilibrio3D::TIConfiguracoesEquilibrio3D (TIChamber * _chamber, bool _molhabilidadeB, bool _compressibilidadeA, uint32 _increment, uint32 _maxRadius, uint32 _minRadius, bool _salvarDisco):ITask ("Equilibrium_Configuration")
{
    //Ativo se a imgIDF ja foi determinada
    imgIDFDeterminada = 0;

    //O default � zero, se for para ter descascamento deve receber o
    //tamanho do descascamento nbo objeto  herdeiro que calcula a permeabilidade relativa
    //Usado para descascar a imagem a ser usada na determinação da permeabilidade
    borderDiscount = 0;

    //Recebe uma camara, criada e definida em quadro de dialogo
    chamber = _chamber;

    // SaveImg( _chamber->GetimgPrototypeptr() , "imagemPrototype.i3d" );

    //Seta flag de salvamento das imagens e propriedades a cada passo
    saveParcialSimulation = false;

    //seta flag interno
    endSimulation = false;

    //Faz parte de this
    pass = 1;

    //seta flag de salvamento dos resultados em disco
    saveToDisc = _salvarDisco;

    //maxRadius = _maxRadius ;
    //BUG: ERRO O IMAGO PASSA O RAIO MAXIMO SELECIONADO NO DIALOGO
    //AQUI DEVERIA USAR O PASSADO E N�O O DA CAMARA
    //AQUIAQUI AQUIAQUI
    //novo, agora o raio m�ximo e obtido da camara
    TISChamber *
    objTISChamber = dynamic_cast < TISChamber * >(chamber);
    assert (objTISChamber != NULL);
    maxRadius = objTISChamber->GetmaxRadius ();
    //A linha abaixo � como esta na vers�o oficial do Imago
    // maxRadius = chamber->GetmaxRadius();
    //maxRadius = _maxRadius ;                             //NOVO EM 20-2-2001

    //compara com o raio m�ximo passado pelo usuário
    //if(_maxRadius != maxRadius)
    //A id�ia era, ou calcular o raio m�ximo ou receber o mesmo do di�logo
    //agora, s� calcula, desconsidera o passado pelo usuário[USA O DO USUARIO]

    //Define o raio m�nimo
    minRadius = _minRadius;

    //Define a dire��o do escoamento em fun��o da molhabilidade de B
    //Se B for molhante a dire��o � 1 (raio crescente)
    direction = (_molhabilidadeB == 1) ? 1 : 0;

    //Define o incremento (depende da molhabilidade de B)
    increment = (direction == 1) ? _increment : -_increment;	//era assim
    //increment =  (_molhabilidadeB == 1)             ?  _increment          :  - _increment;

    //Se falso B invade, se verdadeiro A invade
    //no inicio da simula��o B sempre invade
    inverted = false;

    //Determina o n�mero de invers�es que foram realizadas
    //no inicio da simula��o � 0
    numberInversion = 0;

    //Cria fluidos A e B
    //viscos, dens, comp, molh
    fluidA = NULL;
    fluidOut = fluidA =
                   new TMFluido (1, 1, _compressibilidadeA, !_molhabilidadeB);
    assert (fluidA != NULL);
    fluidB = NULL;
    //fluidIn = fluidB = new TMFluido(1 , 1, _compressibilidadeA,_molhabilidadeB);
    //novo, fluidoB sempre incompress�vel
    fluidIn = fluidB = new TMFluido (1, 1, 0, _molhabilidadeB);
    assert (fluidB != NULL);

    /////////////////////////////////
    //Cria imagens imgA, imgB, imgY, zeradas
    imgA = imgB = imgY = imgTemp = imgFluidIn = imgFluidOut = imgIDF = NULL;
    imgFluidOut = imgA = chamber->GetNewImg ();
    imgFluidIn = imgB = chamber->GetNewImg ();
    imgY = chamber->GetNewImg ();

    //Debug
    //SaveImg(imgA,"imgA-NoConstrutor.i3d");
    //SaveImg(imgB,"imgB-NoConstrutor.i3d");
    //SaveImg(imgY,"imgY-NoConstrutor.i3d");

    /////////////////////////////////
    //Cria as listas de p�xeis
    //Cria lista de p�xeis para imagem da camara molhante
    //Cria imagem nova, desenha camara molhante, verifica a lista de p�xeis
    LPWChamber = NULL;
    LPWChamber = chamber->CreateListPixelWChamber ();
    assert (LPWChamber != NULL);

    //Cria lista de p�xeis para imagem da camara N�o molhante
    //Cria imagem nova, desenha camara n�o molhante, verifica a lista de p�xeis
    LPNWChamber = NULL;
    LPNWChamber = chamber->CreateListPixelNWChamber ();
    assert (LPNWChamber != NULL);

//Debug: sa�da de dados das listas de p�xeis
    /*  {
      ofstream fout("lista de pixeis.txt");
      fout<<"\n------->Objeto LPWChamber";
      //converte ponteiro para classe base, e depois desreferencia
      fout <<*(IListPixel*)LPWChamber;
      fout<<"\n------->Objeto LPNWChamber";
      fout <<*(IListPixel*)LPNWChamber;
      }
    */

    //Fun��o que determina a imagem de dist�ncia ao fundo imgIDF, para a imagem da camara
    //novo, agora ao passar por DeterminaAbertura chama o cálculo da IDf
    //DeterminaImgIDF();

    //Determina o raio inicial como sendo o raio m�nimo ou m�ximo
    //em fun��o da dire��o do escoamento
    radius = direction ? minRadius : maxRadius;
}

//Destrutor
//Destr�e objetos alocados
TIConfiguracoesEquilibrio3D::~TIConfiguracoesEquilibrio3D ()
{
    if (chamber != NULL)
        delete chamber;
    if (imgIDF != NULL)
        delete imgIDF;
    if (imgA != NULL)
        delete imgA;
    if (imgB != NULL)
        delete imgB;
    if (imgY != NULL)
        delete imgY;
    if (fluidA != NULL)
        delete fluidA;
    if (fluidB != NULL)
        delete fluidB;
    if (LPWChamber != NULL)
        delete LPWChamber;
    if (LPNWChamber != NULL)
        delete LPNWChamber;
}

//Fun��o DeterminaImgIDF();
//Vai obter imagem da camara e calcular a imgIDF
void
TIConfiguracoesEquilibrio3D::DeterminaImgIDF ()
{
    /////////////////////////////////
    //Cria imagem da camara
    //Solicita ao objeto camara, uma imagem da camara, passa a informa��o da molhabilidade do flu�do que entra,
    //vai desenhar na imagem da camara o meio poroso e
    //se o fluido que entra for n�o molhante, adiciona a camara n�o molhante
    //ou seja, se  o fluido que entra � o merc�rio, adiciona a camara do merc�rio (n�o molhante).

    //Na visualizacao do imago esta assim (e esta correto):
    //I3DRawImage* img = confEq->Getchamber()->CreateChamber(confEq->GetfluidIn()->Getmolhabilidade());
    //cout <<"\nBUG: Fun��o TIConfiguracoesEquilibrio3D::DeterminaImgIDF()"<<endl;//
    I3DRawImage *imgChamber =
        chamber->CreateChamber (fluidIn->Getmolhabilidade ());
    assert (imgChamber != NULL);
    imgChamber->SetVoxelSize (chamber->GetimgPrototypeptr ()->VoxelSize ());
    imgChamber->SetAmplifyFactor (chamber->GetimgPrototypeptr ()->
                                  AmplifyFactor ());
    //Debug:
    //SaveImg(imgChamber,"imgChamber-SemVerificarMolhabilidade.i3d");

    //////////////////////////////////////////////////
    //Determina imagem IDF
    //Cria filtro idf
    I3DFltD345 fidf;
    fidf.Go (imgChamber);

    //Obtem imagem idf 16bpp
    imgIDF = fidf.Image ();
    assert (imgIDF != NULL);

    //N�o precisa mais da imagem da camara
    delete imgChamber;

    //Debug
    // SaveImg(imgIDF, "imgIDF.i3d");
}


//A simula��o vai de minRadius a maxRadius
//E para cada raio processa as fun��es abaixo
void
TIConfiguracoesEquilibrio3D::SolucaoOmega ()
{
    //Realiza a abertura G (ok), Cria imgTemp
    DeterminaAbertura ();

    //Se fluidIn for molhante
    //Determina a imagem abertura complementar (ok)
    if (fluidIn->Getmolhabilidade () == 1)
        DeterminaAberturaComplementar ();

    //Determina o fluidIn
    DefineFluidIn ();

    //se for tipo 1 � camara de intrus�o de merc�rio
    //n�o necessita realizar os c�lculos abaixo
    if (chamber->Type () == 1)
        return;

    //Determina o fluidOut
    DefineFluidOut ();

    //Determina  o flu�do isolado, depende da compressibilidade do flu�do que sai
    DefineFluidIsolated ();
}

///////////////////////////////////////////
//Determina a regi�o abertura na imagem tempor�ria
void
TIConfiguracoesEquilibrio3D::DeterminaAbertura ()
{
    //Novo, em 13-11-2000
    //A imgIDF estava sendo determinada no construtor, agora, vai ser determinada
    //na primeira chamada a Next, ou Go
    if (imgIDFDeterminada == false)
    {
        DeterminaImgIDF ();
        imgIDFDeterminada = true;
    }
//fim do novo de 13-11-2000

    if (radius > 0)
    {
        //Cria filtro abertura
        I3DFltOpenning fAbertura (radius);

        //Processa abertura
        fAbertura.Go (imgIDF);

        //Obt�m a imagem abertura, e armazena na imgTemp
        imgTemp = fAbertura.Image ();
    }
    else
    {
        //se o raio for 0, a abertura pintaria toda a imagem  da camara
        //retorna imagem meio poroso + (1-fluidIn->Getmolhabilidade())*camara n�o molhante
        imgTemp = chamber->CreateChamber (fluidIn->Getmolhabilidade ());
    }
    assert (imgTemp != 0);

    //SaveImg(imgTemp,"imagemAbertura-%d.i3d");

    //Atualiza processo imago sub_etapa =1
    Update ();
}


///////////////////////////
//Se B molhante,  inverte a imagem Abertura, marcando G_
//Faz parte da abertura complementar, a imagem meio poroso + camara n�o molhante
void
TIConfiguracoesEquilibrio3D::DeterminaAberturaComplementar ()
{
    I3DPixelIterator1 *piimgTemp =
        I3DPixelIterator1::SafeCast (imgTemp->InitPixelIterator ());
    I3DPixelIterator16 *piimgIDF =
        I3DPixelIterator16::SafeCast (imgIDF->InitPixelIterator ());

    for (; !piimgTemp->ImageWrapped (); piimgTemp->Next (), piimgIDF->Next ())
        //se esta na imagem do meio poroso ou na imagem da camara n�o molhante
        if (piimgIDF->Get () != 0
                //e se nao esta na imagem abertura, ent�o n�o faz parte da abertura
                && piimgTemp->Get () == 0)
            //� G_ (abertura complementar)
            piimgTemp->Set (1);
        else
            //sen�o n�o � G_
            piimgTemp->Set (0);
    delete piimgTemp;
    delete piimgIDF;

    // SaveImg(imgTemp,"imagemAberturaComplementar-%d.i3d");
    //atualiza processo imago sub_etapa =2
    Update ();
}

///////////////////////////////////////////
//No inicio da funcao a imgTemp � a imagem com a solucao de
//K(G,B0) ou KwbG__U_KGB0B0
//A imagem B � aditiva, os pixeis so podem ser adicionados e nao eliminados,
//pois o processo � primario.
//imgB = imgB + imgTemp;
//A imgB inclue a camara n�o molhante, se B for n�o molhante
void
TIConfiguracoesEquilibrio3D::DefineFluidIn ()
{
    if (
        //se existe a camara molhante e o fluido � molhante
        chamber->GetwettingChamber () && (fluidIn->Getmolhabilidade () == 1)
        //se existe a camara n�o molhante e o flu�do � n�o molhante
        || chamber->GetnowWettingChamber ()
        && (fluidIn->Getmolhabilidade () == 0))
    {
        //SaveImg(imgTemp, "imgTemp-%d-1-AntesDeMarcarACamara.i3d");
        ///////////////////////////
        //Marca a camara do fluido que entra
        fluidIn->Getmolhabilidade () ==
        1 ? chamber->DrawChamberWettingFluid (imgTemp) : chamber->
        DrawChamberNotWettingFluid (imgTemp);

        //atualiza processo imago sub_etapa =3
        Update ();

        //SaveImg(imgTemp, "imgTemp-%d-1-AposMarcarACamara.i3d");

        //debug:
        //fluidOut->Getmolhabilidade() != 1 ?
        //  SaveImg(imgTemp,"2G-imagemAberturaComplementar_camarafluidIn-%d.i3d"):
        //  SaveImg(imgTemp,"2G-imagemAbertura_camarafluidIn-%d.i3d");

        ///////////////////////////
        //Verifica a conectividade
        //demora=1
        fluidIn->Getmolhabilidade () == 1 ? VerifyConectivit (LPWChamber,
                imgTemp) :
        VerifyConectivit (LPNWChamber, imgTemp);

        //Atualiza processo imago sub_etapa =4
        Update ();

        //SaveImg(imgTemp, "imgTemp-%d-1-AposVerificarConectividade.i3d");

        //fluidIn->Getmolhabilidade() == 1 ?  SaveImg(imgTemp, "2G-imagem-KwbG__U_KGB0-%d.i3d"):
        //                                    SaveImg(imgTemp, "2G-imagem-KGB0-%d.i3d");//debug

        /////////////////////////
        //Marca a imagem  fluidIn
        I3DPixelIterator1 *piimgFluidIn =
            I3DPixelIterator1::SafeCast (imgFluidIn->InitPixelIterator ());
        I3DPixelIterator1 *piimgTemp =
            I3DPixelIterator1::SafeCast (imgTemp->InitPixelIterator ());
        I3DPixelIterator16 *piimgIDF =
            I3DPixelIterator16::SafeCast (imgIDF->InitPixelIterator ());
        for (; !piimgIDF->ImageWrapped ();
                piimgIDF->Next (), piimgTemp->Next (), piimgFluidIn->Next ())
            if (piimgIDF->Get () != 0 && piimgTemp->Get () == 1)
                //Adiciona o pixel atual
                piimgFluidIn->Set (1);

        //Deleta os ponteiros alocados internamente
        delete piimgFluidIn;
        delete piimgTemp;
        delete piimgIDF;

        //Atualiza processo imago sub_etapa =5
        Update ();
    }

    //sa�da dos resultados passo a passo (se solicitado)
    SaveImg (imgFluidIn, "imgFluidIn-%d.i3d");

    //Criada na fun��o determina abertura
    //e deletada aqui.
    delete imgTemp;
    //N�o vai mais ser usada
    imgTemp = NULL;
}


///////////////////////////////////////////
//Primeiro, tudo o que n�o for B � A, depois fica sendo A somente o que estiver
//conexo a camara de A
//A imgA inclue a camara do flu�do A
void
TIConfiguracoesEquilibrio3D::DefineFluidOut ()
{
    //Se a camara for do tipo intrus�o de merc�rio
    //Realiza a conex�o do complemento da imgIn, com a camara do fluidOut.
    //PS: Agora faz o teste abaixo

    //se o fluido � molhante e existe a camara molhante
    if (chamber->GetwettingChamber () && fluidOut->Getmolhabilidade () == 1
            //ou se, o flu�do � n�o molhante e existe a camara n�o molhante
            || chamber->GetnowWettingChamber ()
            && fluidOut->Getmolhabilidade () == 0)
    {
        //Tudo que n�o � B � considerado A
        I3DPixelIterator1 *piimgFluidIn =
            I3DPixelIterator1::SafeCast (imgFluidIn->InitPixelIterator ());
        I3DPixelIterator1 *piimgFluidOut =
            I3DPixelIterator1::SafeCast (imgFluidOut->InitPixelIterator ());
        I3DPixelIterator16 *piimgIDF =
            I3DPixelIterator16::SafeCast (imgIDF->InitPixelIterator ());

        //� necess�rio, porque a imgFluidOut esta diminuindo
        imgFluidOut->ClearBits ();
        for (; !piimgIDF->ImageWrapped ();
                piimgIDF->Next (), piimgFluidIn->Next (), piimgFluidOut->Next ())
            //faz parte do meio poroso
            if (piimgIDF->Get () != 0
                    //e n�o faz parte de B
                    && piimgFluidIn->Get () == 0)
                //ent�o � por enquanto A
                piimgFluidOut->Set (1);

        //Atualiza processo imago sub_etapa =6
        Update ();

        //Marca a camara
        fluidOut->Getmolhabilidade () ==
        1 ? chamber->DrawChamberWettingFluid (imgFluidOut) : chamber->
        DrawChamberNotWettingFluid (imgFluidOut);

        //atualiza processo imago sub_etapa =7
        Update ();

        //Verifica a conectividade da camara com o resto da imagem
        //� por isso que o if acima faz sentido, se n�o tiver a camara a regi�o conexa � nulla
        //demora=1
        fluidOut->Getmolhabilidade () == 1 ? VerifyConectivit (LPWChamber,
                imgFluidOut) :
        VerifyConectivit (LPNWChamber, imgFluidOut);

        //Atualiza processo imago sub_etapa =8
        Update ();

        delete piimgFluidIn;
        delete piimgFluidOut;
        delete piimgIDF;
    }

    SaveImg (imgFluidOut, "imgFluidOut-%d.i3d");
}

///////////////////////////////////////////
//Se Y for compressivel define Y a partir de A e B,
//Se Y for incompressivel, define Y a partir de A, B e Y anterior
//podendo corrigir A e B.
void
TIConfiguracoesEquilibrio3D::DefineFluidIsolated ()
{
    //Abaixo leva em conta se � compress�vel ou incompress�vel
    I3DPixelIterator1 *piimgFluidIn =
        I3DPixelIterator1::SafeCast (imgFluidIn->InitPixelIterator ());
    I3DPixelIterator1 *piimgFluidOut =
        I3DPixelIterator1::SafeCast (imgFluidOut->InitPixelIterator ());
    I3DPixelIterator1 *piimgY =
        I3DPixelIterator1::SafeCast (imgY->InitPixelIterator ());
    I3DPixelIterator16 *piimgIDF =
        I3DPixelIterator16::SafeCast (imgIDF->InitPixelIterator ());

    //Se fluidOut,Y,(A) for incompressivel, Y vai depender de A, B e Yanterior
    //No codigo antigo, a compressibilidade dos dois flu�dos era igual, agora podem ser diferentes
    if (fluidOut->Getcompressibilidade () == 0 && pass != 1)
    {
        for (; !piimgIDF->ImageWrapped ();
                piimgIDF->Next (), piimgFluidOut->Next (), piimgFluidIn->Next (),
                piimgY->Next ())
        {
            //se j� era  Y,
            if (piimgY->Get () == 1)
            {
                //deve corrigir A e B
                piimgFluidIn->Set (0);
                //seta em B e em A como inativo
                piimgFluidOut->Set (0);
            }

            //� meio poroso, n�o � A, n�o � B
            else if (piimgIDF->Get () != 0 && piimgFluidOut->Get () == 0
                     && piimgFluidIn->Get () == 0)
                //ent�o � Y
                piimgY->Set (1);
        }
    }
    //Y compressivel, depende apenas de A e B
    else
    {
        for (; !piimgIDF->ImageWrapped ();
                piimgIDF->Next (), piimgFluidOut->Next (), piimgFluidIn->Next (),
                piimgY->Next ())
        {
            //� meio poroso, n�o � A nem B, ent�o � Y
            if (piimgIDF->Get () != 0 && piimgFluidOut->Get () == 0
                    && piimgFluidIn->Get () == 0)
                piimgY->Set (1);
            else
                //ou n�o � meio poroso, ou  � A, ou �   B
                piimgY->Set (0);
        }
    }
    delete piimgFluidIn;
    delete piimgFluidOut;
    delete piimgY;
    delete piimgIDF;

    SaveImg (imgY, "imgFLuidIsolated-imgY-%d.i3d");

    //Atualiza processo imago sub_etapa =9
    Update ();
}

////////////////////////////////////////////////////////////////
//Realiza a rotulagem e a seguir a verificacao da conectividade
//Verifica a conectividade da _imgTemp considerando a lista de pixeis
void
TIConfiguracoesEquilibrio3D::VerifyConectivit (IListPixel * _listPixel,
        I3DRawImage * _imgTemp)
{
    //usa filtro de rotulagemdo imago
    //Rotula a imagem
    I3DFltLabel frotulagem;
    frotulagem.Go (_imgTemp);
    I3DRawImage *imgRotulo = frotulagem.Image ();
    assert (imgRotulo != NULL);

    //Cria filtro especial de verifica��o da conectividade
    TFEConectividade3D fconectividade;

    //Verifica a conectividade
    fconectividade.VerifyConectivit (
        //imagem rotulada 32bpp
        imgRotulo,
        //lista dos pixeis a serem considerados na conec��o
        _listPixel,
        //imagem binaria a ser preenchida
        _imgTemp);
    delete imgRotulo;
}

//////////////////////////////////////////////////////////////////////
//Processa para o radius atual e depois incrementa o radius
bool
TIConfiguracoesEquilibrio3D::Next ()
{
    //se o raio � menor ou maior que o limite sai
    if (radius < minRadius || radius > maxRadius
            //ou se endsimulation=true tamb�m sai
            || endSimulation == true)
    {
        endSimulation = true;
        return false;
    }

//Dados da simula��o para este passo
    cout << "\nDados da simulação passo=" << pass << endl;
    cout << "maxRadius:(" << maxRadius << ")";
    cout << " minRadius:(" << minRadius << ")";
    cout << " radius:(" << radius << ")";
    cout << " increment:(" << increment << ")";
    cout << " fIn->M : (" << (int) fluidIn->Getmolhabilidade () << ")";
    cout << " fIn->C : (" << (int) fluidIn->Getcompressibilidade () << ")";
    cout << " fOut->M: (" << (int) fluidOut->Getmolhabilidade () << ")";
    cout << " direction:(" << (int) direction << ")";
    cout << " inverted:(" << (int) inverted << ")";
    cout << " numberInversion:(" << numberInversion << ")" << endl;

    ofstream hout ("histerese.dat", ios::app);
    hout << "\nDados da simulação passo=" << pass << endl;
    hout << "maxRadius:(" << maxRadius << ")";
    hout << " minRadius:(" << minRadius << ")";
    hout << " radius:(" << radius << ")";
    hout << " increment:(" << increment << ")";
    hout << " fIn->M : (" << (int) fluidIn->Getmolhabilidade () << ")";
    hout << " fIn->C : (" << (int) fluidIn->Getcompressibilidade () << ")";
    hout << " fOut->M: (" << (int) fluidOut->Getmolhabilidade () << ")";
    hout << " direction:(" << (int) direction << ")";
    hout << " inverted:(" << (int) inverted << ")";
    hout << " numberInversion:(" << numberInversion << ")" << endl;
    hout.close ();

    //Solu��o geral do sistema
    SolucaoOmega ();

    //Conjunto de fun��es virtuais novas
    //Cria objetos                                                (cria imagens)
    IntermediatNextCreate ();
    //Calcula processos                   (calcula confEq e permrel)
    IntermediatNextCalculate ();
    //Sa�da de dados                                      (salva resultados em disco)
    IntermediatNextOutput ();
    //Fecha e deleta objetos  (deleta imagens)
    IntermediatNextClose ();

    //incrementa o passo
    this->pass++;

    //s� incrementa o raio no final da simula��o
    radius = radius + increment;

    //Salva simula��o passo a passo (permite reiniciar simula��o parada)
    if (saveParcialSimulation)
        SaveParcialSimulation ();
    return true;
}

//Nova, processa toda a histerese
bool
TIConfiguracoesEquilibrio3D::Histerese (int numeroCiclos)
{

    //Enquanto maxRadius for maior ou igual a minRadius
    cout << "\nFun��o de cálculo da histerese." << endl;
    for (int i = 0; i < numeroCiclos; i++)
    {
        cout << "\a\nExecutando o ciclo: " << i << endl;

        //Executa todos os passos
        Go ();

        //Inverte a dire��o
        InvertDirection ();
    }
    return true;
}

/* Modificada em 19-2-2001, antes n�o recebia o numero de ciclos
bool TIConfiguracoesEquilibrio3D::Histerese ()
{

	//Enquanto maxRadius for maior ou igual a minRadius
   while(maxRadius >= minRadius)
   {
   //Executa todos os passos
   Go();

   //Inverte a dire��o
	 InvertDirection();
   }
   return true;
}
*/
////////////////////////////////////////////////
//Processa tudo e salva em disco
/*
Fun��o antiga sem controle evolu��o do processo
bool  TIConfiguracoesEquilibrio3D::Go()
{
  while(Next());
return 1;
}
*/
bool
TIConfiguracoesEquilibrio3D::Go ()
{
    //Determina o n�mero de etapas da simula��o
    //Cada evolu��o da simula��o corresponde a uma chamada a Next,
    //mas Next executa uma sequ�ncia de etapas que foram subdivididas
    //em 9 sub_etapas.
    //numero de Next
    uint32 numeroNext = (maxRadius - minRadius + 1);	//*numeroCiclos

    //numero total de etapas
    uint32 numeroEtapas = numeroNext * 9;

    //Inicializa o processo
    Begin (numeroEtapas);

    //Log do processo
    IString logData;
    logData << "Equilibrium configuration Parameters:\r"
    << "maxRadius: ( " << maxRadius << ")\r"
    << "minRadius: ( " << minRadius << ")\r"
    << "increment: ( " << increment << ")\r"
    << "fluidIn->Molhabilidade : ( " << fluidIn->Getmolhabilidade () << ")\r"
    << "fluidIn->Compressibilidade : ( " << fluidIn->
    Getcompressibilidade () << ")\r" << "fluidOut->Molhabilidade : ( " <<
    fluidOut->
    Getmolhabilidade () << ")\r" << "fluidOut->Compressibilidade : ( " <<
    fluidOut->Getcompressibilidade () << ")\r";
    onLogData.Call (this, logData);

    //Executa os diversos Next.
    //Next chama Solu��oOmega que chama
    //Update() que atualiza o processo.
    while (Next ());

    //Encerra o processo
    End ();

    return 1;
}

////////////////////////////////////////////////
//Processa um �nico passo (radius)
bool
TIConfiguracoesEquilibrio3D::Direct (uint32 _radius)
{
    bool oldCompressibility = fluidOut->Getcompressibilidade ();
    fluidOut->Setcompressibilidade (1);
    radius = _radius;
    bool r = Next ();
    fluidOut->Setcompressibilidade (oldCompressibility);
    return r;
}

//////////////////////////////////////////////////////////////////
//Inverte a dire��o do processo, invertendo os fluidos
//Para inverter o processo, precisa inverter os fluidos que entram e que saem.
//1-Inverte a dire��o do escoamento
//2-recalcula o radius e o increment
//3-Inverte os fluidos fluidoin fluidoout
//4-Seta a imagem de Y como sendo o fluidoin
//5-Zera a imagem de Y (isolada)
//6-
//7-
void
TIConfiguracoesEquilibrio3D::InvertDirection ()
{
    cout << "\nFun��o TIConfiguracoesEquilibrio3D::InvertDirection()";

    //Permite continuar dando next
    endSimulation = false;

    //N�mero de invers�es realizadas
    numberInversion++;

    //A cada invers�o esta reduzindo o maxRadius e aumentando o minRadius
    //
    //maxRadius--;Antes o intervalo de cálculo ia sendo reduzido
    //minRadius++;agora vai de ponta a ponta

    cout << "\nInvertendo o fluxo";
    //Inverte o sentido do escoamento
    inverted = !inverted;

    //direction indica a dire��o do escoamento
    //se true entra fluidoB se false entra flu�do A
    //Inverte a dire��o do escoamento
    direction = !direction;

    //radius = 7, inc=+2--> radius = 3
    //Recalcula o raio a ser usado na determinação da confeq
    //radius     = radius - 2 * increment;
    //Calcula o Raio Inicial em fun��o da dire��o
    cout << "\nCalculanndo raio e incrementos";

    //A linha abaixo � desnecess�ria, pois se o ciclo atual j�
    //encerrou, o radius j� esta correto
    radius = direction ? minRadius : maxRadius;

    //Define o incremento como positivo ou negativo
    increment = -increment;
    //increment = (direction == 1 )                  ?  increment   :  - increment;era assim estava errado

    //Define os fluidos in,out e as imagens in e out
    //Default (se n�o invertido)
    cout << "\nInvertendo os flu�dos e as imagens";
    if (!inverted)
    {
        //Flu�do que entra � o fluido B
        fluidIn = fluidB;
        //Flu�do que sai   � o fluido A
        fluidOut = fluidA;
        //imgFluidIn  aponta para imagem do flu�do  B
        imgFluidIn = imgB;
        //imgFluidOut aponta para imagem do flu�do  A
        imgFluidOut = imgA;
    }

    //Invertida
    else
    {
        //Flu�do que entra � o fluido A
        fluidIn = fluidA;
        //Flu�do que sai   � o fluido B
        fluidOut = fluidB;
        //imgFluidIn  aponta para imagem do flu�do  A
        imgFluidIn = imgA;
        //imgFluidOut aponta para imagem do flu�do  B
        imgFluidOut = imgB;
    }

    cout << "\nRedefinindo o flu�do isolado";
    //Marca a regi�o Y (A isolado) como sendo fluidIn
    //novo: troquei fluidOut por fluidIn, agora esta correto
    //pois acima inverteu
    I3DPixelIterator1 *piimgfluidIn =
        I3DPixelIterator1::SafeCast (imgFluidIn->InitPixelIterator ());
    I3DPixelIterator1 *piimgY =
        I3DPixelIterator1::SafeCast (imgY->InitPixelIterator ());
    for (; !piimgfluidIn->ImageWrapped ();
            piimgfluidIn->Next (), piimgY->Next ())
        //se for Y (A isolado)
        if (piimgY->Get () == 1)
            //Marcar no fluidIn
            piimgfluidIn->Set (1);

    delete piimgfluidIn;
    delete piimgY;

    //Zera a imagem de Y (ok)
    cout << "\nRedefinindo a imagem de Y";
    imgY->ClearBits ();

    //Como os flu�dos foram invertidos, precisa redesenhar
    //as camaras e recalcular a IDF
    //Se B for molhante a camara n�o � desenhada, e a IDF
    //� calculada somente sobre a regi�o do meio poroso
    //Se B for N�o molhante, a camara com o flu�do n�o molhante
    //(mercurio) � desenhada e entra no cálculo da IDF
    cout << "\nRedefinindo a camara";
    cout << "\nRecalculando a IDF" << endl;
    DeterminaImgIDF ();

    //No construtor � criada uma lista de p�xeis
    //como a camara � a mesma n�o precisa recriar
    //a lista de p�xeis
}

//ANTIGA substituida em 19-2-2001
/*
//////////////////////////////////////////////////////////////////
//Inverte a dire��o do processo, invertendo os fluidos
//Para inverter o processo, precisa inverter os fluidos que entra e que sai.
void TIConfiguracoesEquilibrio3D::InvertDirection()
{
  //Permite continuar dando next
  endSimulation = false;
  //numero de invers�es realizadas
  numberInversion++;
	//redefinia os raios maximo e minimo (estava errado aqui)
  maxRadius--;
  minRadius++;

  //Inverte atributos
  inverted   = !inverted;
  direction  = !direction;
  radius     = radius - 2 * increment;
  increment  = - increment ;

  if(!inverted)
	  {fluidIn = fluidB; fluidOut = fluidA;
      imgFluidIn = imgB; imgFluidOut = imgA;
     }
  else
	  {fluidIn = fluidA; fluidOut = fluidB;
      imgFluidIn = imgA; imgFluidOut = imgB;
     }

  //Marca a regi�o Y (A isolado) como sendo fluidIn
  //novo: troquei fluidOut por fluidIn, agora esta correto
  I3DPixelIterator1* piimgfluidIn	= 	I3DPixelIterator1::SafeCast(imgFluidIn->InitPixelIterator());
  I3DPixelIterator1* piimgY 		   = 	I3DPixelIterator1::SafeCast(imgY->InitPixelIterator());
  for (; !piimgfluidIn->ImageWrapped(); piimgfluidIn->Next(),piimgY->Next())
     if(piimgY->Get()==1)
	     piimgfluidIn->Set(1);

  delete piimgfluidIn;
  delete piimgY;
  imgY->ClearBits();

 //Redeterminação da IDF (pois os flu�dos foram invertidos)
 DeterminaImgIDF();
}

*/
//Voc� n�o sabe se a imgA esta entrando ou saindo
//Se a imgA for molhante, esta conectada a camara molhante e a mesma deve ser apagada.
I3DRawImage *
TIConfiguracoesEquilibrio3D::GetimgA () const const
{
    //retorna copia de A
    I3DRawImage *img = TOperacoesImg::GetCopyBinaria (imgA);
    assert (img != NULL);
    //obtem cor do pincel
    uint32 c = chamber->GetcPaintBrush ();
    //seta para desmarcar
    chamber->SetcPaintBrush (0);
    fluidA->Getmolhabilidade () == 1 ?
    //Desmarca a camara
    chamber->DrawChamberWettingFluid (img) :
    chamber->DrawChamberNotWettingFluid (img);
    chamber->SetcPaintBrush (c);
    return img;
};


//Se B � n�o molhante, deve apagar a camara n�o molhante,
//se molhante deve apagar a camara molhante
I3DRawImage *
TIConfiguracoesEquilibrio3D::GetimgB () const const
{
    I3DRawImage *img = TOperacoesImg::GetCopyBinaria (imgB);
    assert (img != NULL);
    uint32 c = chamber->GetcPaintBrush ();
    chamber->SetcPaintBrush (0);
    fluidB->Getmolhabilidade () == 1 ? chamber->DrawChamberWettingFluid (img) :
    //ok  apaga a camara nao molhante
    chamber->DrawChamberNotWettingFluid (img);
    chamber->SetcPaintBrush (c);
    return img;
};

//
I3DRawImage *
TIConfiguracoesEquilibrio3D::GetimgY () const const
{
    I3DRawImage *img = TOperacoesImg::GetCopyBinaria (imgY);
    return img;
};

//Retornam as imagens incluindo as camaras
I3DRawImage *
TIConfiguracoesEquilibrio3D::GetimgFluidIn () const const
{
    I3DRawImage *img = TOperacoesImg::GetCopyBinaria (imgFluidIn);
    return img;
};

I3DRawImage *
TIConfiguracoesEquilibrio3D::GetimgFluidOut () const const
{
    I3DRawImage *img = TOperacoesImg::GetCopyBinaria (imgFluidOut);
    return img;
};

//Novo, retornam somente o n�cleo das imagens, desconsiderando as camaras
I3DRawImage *
TIConfiguracoesEquilibrio3D::GetNucleoImgB () const const
{
    //SaveImg(imgB,"imgB-dentro de GetNucleoImgB.i3d");
    //Cria imagem do n�cleo a partir da imagemB
    I3DRawImage *imgBNucleo = chamber->GetImgNucleo (imgB, borderDiscount);

    //SaveImg(imgB,"imgBNucleo-dentro de GetNucleoImgB.i3d");
    return imgBNucleo;
}

I3DRawImage *
TIConfiguracoesEquilibrio3D::GetNucleoImgA () const const
{
    I3DRawImage *imgANucleo = chamber->GetImgNucleo (imgA, borderDiscount);
    return imgANucleo;
}

I3DRawImage *
TIConfiguracoesEquilibrio3D::GetNucleoImgY () const const
{
    I3DRawImage *imgYNucleo = chamber->GetImgNucleo (imgY, borderDiscount);
    return imgYNucleo;
}

//Salvar imagem em disco
void
TIConfiguracoesEquilibrio3D::SaveImg (I3DRawImage * &_imagem,
                                      string _nomeArquivo)
{
    if (saveToDisc)
    {
        char nomeArquivoContador[256];
        sprintf (nomeArquivoContador, _nomeArquivo.c_str (), pass);

        //Para salvar em modo ASCII
        //TOperacoesImg::SaveImg( _imagem, nomeArquivoContador);
        //novo, agora usa objeto para salvar imagem3D
        IWriteI3D wi3d (nomeArquivoContador);
        if (wi3d.Good ())
            wi3d << *_imagem;
    }
};

//Retorna uma imagem 8bpp composta.
//Sem paleta de cores, para ver como colocar uma paleta de cores
//veja a funcao void GFrameWindowConfEq::ViewImgComposta( IObject * obj);
I3DRawImage *
TIConfiguracoesEquilibrio3D::GetImgComposta ()
{
    //Cria imagem 8bpp, zerada e um pixel iterator
    I3DRawImage *imgComposta = new I3DRawImage (chamber->Getsize (), 8);
    assert (imgComposta);
    imgComposta->SetVoxelSize (chamber->GetimgPrototypeptr ()->VoxelSize ());
    imgComposta->SetAmplifyFactor (chamber->GetimgPrototypeptr ()->
                                   AmplifyFactor ());

    //Tudo preto
    imgComposta->ClearBits ();
    I3DPixelIterator8 *piimgComposta =
        I3DPixelIterator8::SafeCast (imgComposta->InitPixelIterator ());

    //Cria pixeis iterator para imagens A,B,Y
    I3DPixelIterator1 *piimgB =
        I3DPixelIterator1::SafeCast (imgB->InitPixelIterator ());
    I3DPixelIterator1 *piimgA =
        I3DPixelIterator1::SafeCast (imgA->InitPixelIterator ());
    I3DPixelIterator1 *piimgY =
        I3DPixelIterator1::SafeCast (imgY->InitPixelIterator ());

    //Copia valores dos pixeis para imgComposta
    for (; !piimgComposta->ImageWrapped ();
            piimgComposta->Next (), piimgB->Next (), piimgA->Next (),
            piimgY->Next ())
    {
        //valor 0 � o fundo, � preto
        //valor 255 � branco
        if (piimgB->Get () == 1)
            //paleta->SetColor( 0, IColor (0,0,255) );   //imbB blue ok
            piimgComposta->Set (0);
        else if (piimgA->Get () == 1)
            //paleta->SetColor( 1, IColor (255,0,0) );   //imgA red ok
            piimgComposta->Set (1);
        else if (piimgY->Get () == 1)
            //paleta->SetColor( 2, IColor (255,255,0));  //imgY yellow
            piimgComposta->Set (2);
        else
            //paleta->SetColor( 3, IColor (0,0,0) );             //black
            piimgComposta->Set (3);
    }
    delete piimgY;
    delete piimgA;
    delete piimgB;
    return imgComposta;
}

//Novo: fun��o nova, ainda n�o testada
//Salva dados da simula��o em disco
//O objetivo � salvar os dados da simula��o e permitir o rein�cio da simula��o
//a qualquer tempo.
//N�o foi testada, precisa ser aprimorada (salvando os objetos)
void
TIConfiguracoesEquilibrio3D::SaveParcialSimulation ()
{
    //Cria arquivo confEqProperties.dat
    ofstream ffout ("confEqProperties.dat");
    if (ffout.good ())
    {
        ffout << chamber->GetClassName () << endl;
        ffout << GetClassName () << endl;
        ffout << chamber->GetimgPrototypeptr ()->AmplifyFactor () << endl;
        ffout << chamber->GetimgPrototypeptr ()->VoxelSize () << endl;

        ffout << borderDiscount << endl;
        ffout << fluidB->Getmolhabilidade () << endl;
        ffout << fluidA->Getcompressibilidade () << endl;
        ffout << pass << endl;
        ffout << maxRadius << endl;
        ffout << minRadius << endl;
        ffout << radius << endl;
        ffout << increment << endl;
        ffout << direction << endl;
        ffout << inverted << endl;
        ffout << saveToDisc << endl;
        ffout << endSimulation << endl;
        ffout << saveParcialSimulation << endl;
        ffout.close ();
    }

    //Salva imagens em disco
    //Imagem inicial, foi salva com o nome "imgMeioPoroso.i3d"
    bool oldsaveToDisc = saveToDisc;
    saveToDisc = true;
    SaveImg (imgA, "imgA.i3d");
    SaveImg (imgB, "imgB.i3d");
    SaveImg (imgY, "imgY.i3d");
    saveToDisc = oldsaveToDisc;
}

ostream & TIConfiguracoesEquilibrio3D::Output (ostream & os)
{
    os << "raio inicial                :" << radius << "\n";
    os << "minRadius                   :" << minRadius << "\n";
    os << "maxRadius                   :" << maxRadius << "\n";
    os << "numberInversion             :" << numberInversion << "\n";
    os << "increment                   :" << increment << "\n";
    os << "direction                   :" << direction << "\n";
    return os;
}

ostream & operator<< (ostream & os, const TIConfiguracoesEquilibrio3D & obj)
{
    os << "\nDados objeto configura��es de equil�brio\n";
    os << "raio inicial                :" << obj.radius << "\n";
    os << "minRadius                   :" << obj.minRadius << "\n";
    os << "maxRadius                   :" << obj.maxRadius << "\n";
    os << "numberInversion             :" << obj.numberInversion << "\n";
    os << "increment                   :" << obj.increment << "\n";
    os << "direction                   :" << obj.direction << "\n";


    os << "\nObjetos Auxiliares ";
    os << "\nDados objeto fluidA         :" << *obj.fluidA << "\n";
    os << "\nDados objeto fluidB         :" << *obj.fluidB << "\n";

    os << "\nDados objeto chamber         :" << *obj.chamber << "\n";
    /*debug
       {
       ofstream teste("chamber.txt");
       teste<<*chamber;
       teste.close();
       }
     */
    return os;
}


//#endif

/*Novidades do IProcess
No contrutor
IProcess( "Reconstruct" ),
Na fun��o Go
	Begin( ProcessSteps( size ) );
	IString logData;
	logData << "Reconstruct Parameters:\r"
		<< "Size: ( " << size.x << ", " << size.y << ", " << size.z << " )\r"
		<< "N. Points: " << nPoints << "\r"
		<< "Amplify Factor: " << ampFactor << "\r";
	onLogData.Call( this, logData );
		End();

Ao longo dos processos
   Update()
*/
