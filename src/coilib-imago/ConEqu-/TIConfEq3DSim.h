
#ifndef TIConfEq3DSim_h
#define TIConfEq3DSim_h

/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: |CLASSNAME|...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     Andr� Duarte Bueno
@file       TIConfEq3DSim.h
@begin      Fri Nov 17 2000
@copyright  (C) 2000 by Andr� Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------
#include <fstream.h>

//-----------------------------------------------------------------------
//COI
//-----------------------------------------------------------------------
#include <coi20/sys/adt/svector.h>

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
#ifndef _LIB_LMPT_CLASS_h
#include <Base/_LIB_LMPT_CLASS.h>
#endif

#ifndef TIConfiguracoesEquilibrio3D_h
#include <TConEquCOILIB/TIConfiguracoesEquilibrio3D.h>
#endif

/*
=========================================================================
Documentacao Classe: TIConfEq3DSim
=========================================================================
Assunto:        Configuracoes de equilibrio
Superclasse:    TIConfEq3DSim
@short
		Declara a classe TIConfEq3DSim.
		Herdeira da classe TConfiguracoesEquilibrio,
                acrescenta o calculo das curvas de press�o capilar. Ou seja, depois
                de determinar as imagens imgB,nucleoImgA, imgY, determina as
                propor��es (porosidades) destas imagens.

                Ou seja, use TConfiguracoesEquilibrio para obter as configuracoes
                de equilibrio (visualizacao)
                e  TIConfEq3DSim quando deseja obter as curvas de press�o capilar.
@author 	Andr� Duarte Bueno
@version
@see		Veja assunto
----------------------------------------------------------------------------
*/

class TIConfEq3DSim:public TIConfiguracoesEquilibrio3D
{
    COI20_CLASSID COI20_SAFECAST (TIConfEq3DSim, IObject)
//--------------------------------------------------------------Atributos
private:
    //nome do arquivo de disco
    std::string nomeArquivo;

protected:

    //marcador de tempo
    TSysTime timeInit;

    //vetores com as informacoes
    //volumesAcumuladosimgA
    ISimpleVectorOf < float >curvaPressaoCapilarImgA;
    //curvaPressaoCapilarImgB
    ISimpleVectorOf < float >curvaPressaoCapilarImgB;
    //volumesAcumuladosimgY
    ISimpleVectorOf < float >curvaPressaoCapilarImgY;

    //Atributos auxiliares
    //Ponteiro para imagens utilizadas nos calculos internos
    I3DRawImage *nucleoImgB;
    I3DRawImage *nucleoImgA;
    I3DRawImage *nucleoImgY;

    //Ponteiro para arquivo de saida de disco
    ofstream fout;

public:

//-------------------------------------------------------------Construtor
    TIConfEq3DSim (TIChamber * _chamber, bool _molhabilidadeB =
                       1, bool _compressibilidadeA = 1, uint32 _incremento =
                       1, uint32 _raioMaximo = 0, uint32 _raioMinimo =
                       0, bool _salvarDisco = false
                                              //novo em 27-7-2002
                                              , string nomeArquivoArq = '\0'):
            TIConfiguracoesEquilibrio3D (_chamber, _molhabilidadeB, _compressibilidadeA,
                                         _incremento, _raioMaximo, _raioMinimo,
                                         _salvarDisco)
            //,curvaPressaoCapilarImgA(_raioMaximo - _raioMinimo + 1)      //inicia vetores
            //,curvaPressaoCapilarImgB(_raioMaximo - _raioMinimo + 1)
            //,curvaPressaoCapilarImgY(_raioMaximo - _raioMinimo + 1)
            , curvaPressaoCapilarImgA (chamber->GetmaxRadius () + 1)	//inicia vetores
            , curvaPressaoCapilarImgB (chamber->GetmaxRadius () + 1),
            curvaPressaoCapilarImgY (chamber->GetmaxRadius () + 1), nucleoImgB (NULL),
            nucleoImgA (NULL), nucleoImgY (NULL)
    {
        memset (curvaPressaoCapilarImgA.Data (), 0,
                curvaPressaoCapilarImgA.Size () * sizeof (float));
        memset (curvaPressaoCapilarImgB.Data (), 0,
                curvaPressaoCapilarImgB.Size () * sizeof (float));
        memset (curvaPressaoCapilarImgY.Data (), 0,
                curvaPressaoCapilarImgY.Size () * sizeof (float));
        //nomeArquivo="\0";//novo
        //novo em 27-7-2002
        //nomeArquivo = nomeArquivoArq;
        imageName = nomeArquivoArq;
        //aquiaqui
        cout << "TIConfEq3DSim: imageName = " << imageName << endl;
        cout << "TIConfEq3DSim: nomeArquivo = " << nomeArquivo << endl;
    };

//--------------------------------------------------------------Destrutor
    virtual ~ TIConfEq3DSim ()
    {
    };

//----------------------------------------------------------------M�todos
private:
protected:
    //Cria cabe�alho arquivo de disco
    virtual void PrepareConfEqSim ();

public:

    //Fun��es virtuais chamadas dentro de solu��o Omega, redefinidas aqui
    virtual bool Next ();

    //Cria objetos auxiliares
    virtual void IntermediatNextCreate ();

    //Calcula propriedades simula��o (valores satura��o)
    virtual void IntermediatNextCalculate ();

    //Sa�da de dados
    virtual void IntermediatNextOutput ();

    //Deleta objetos tempor�rios e fecha arquivo de disco
    virtual void IntermediatNextClose ();

    //Fun��es Get, retornam atributos internos (novos)
    const ISimpleVectorOf < float >&GetcurvaPressaoCapilarImgA () const
    {
        return curvaPressaoCapilarImgA;
    };
    const ISimpleVectorOf < float >&GetcurvaPressaoCapilarImgB () const
    {
        return curvaPressaoCapilarImgB;
    };
    const ISimpleVectorOf < float >&GetcurvaPressaoCapilarImgY () const
    {
        return curvaPressaoCapilarImgY;
    };

//-------------------------------------------------------------Sobrecarga

//--------------------------------------------------------------------Get
    virtual std::string GetClassName () const
    {
        return std::string ("TIConfEq3DSim");
    };

    //Retorna nome do arquivo de disco
    virtual std::string GetNomeArquivo ();


    //NOVO em 13-11-2000
    //Fun��o que retorna a presso  dado sigma, teta e o raioBola
    //virtual long double GetPressure(long double _raioBola, long double _teta, long double _sigma);
    //virtual long double GetVolumeMolhante(long double _raioBola);
    //(nx*fa*sp)*(ny*fa*sp)*(nz*fa*sp)*porosidadeFaseMolhante(rb)

    //NOVO em 13-11-2000
    long double GetNotWeetingVol (int _raioBola);
    long double GetWeetingVol (int _raioBola);
    long double GetPressure (int _raioBola, long double _teta,
                             long double _sigma);

//--------------------------------------------------------------------Set
//-----------------------------------------------------------------Friend
//      friend ostream& operator<< (ostream& os, const |CLASSNAME|& obj);//
//      friend istream& operator>> (istream& is, |CLASSNAME|& obj);

};

//-----------------------------------------------------------------Friend
//Declara��o de Fun��es Friend
//ostream& operator<< (ostream& os, const |CLASSNAME|& obj);
//istream& operator>> (istream& is, |CLASSNAME|& obj);
#endif
