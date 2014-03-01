
#ifndef TIConfiguracoesEquilibrio3D_h
#define TIConfiguracoesEquilibrio3D_h

/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: |CLASSNAME|...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     Andr� Duarte Bueno
@file       TIConfiguracoesEquilibrio3d.h
@begin      Fri Nov 17 2000
@copyright  (C) 2000 by Andr� Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------
#include <cstdio>

//-----------------------------------------------------------------------
//Bibliotecas COILIB
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//Bibliotecas IMAGO
//-----------------------------------------------------------------------
//#include <imago/gimg/core/task.h>
#include <imago/img/core/task.h>
//#include <imago/img3d/rawimg3d.h>
#include <imago/img3d/image/raw.h>

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
#ifndef 	 TMFluido_h
#include <TMaterial/TMFluido.h>
#endif

#ifndef 	 TIChamber_h
#include <TConEquCOILIB/TIChamber.h>
#endif

#ifndef IListPixelRotuled_h
#include <TLista/IListPixelRotuled.h>
#endif

/*
=========================================================================
Documentacao Classe: TIConfiguracoesEquilibrio3D
=========================================================================
Assunto:        Configuracoes Equilibrio 3D
Superclasse:    TIConfiguracoesEquilibrio3D
Descri��o:
@short	     	Declara a classe TIConfiguracoesEquilibrio3D.
                Cont�m atributos � m�todos necess�rios para a determina��o das
                configura��es de equilibrio.

                Use TConfiguracoesEquilibrio para obter as configuracoes de
                equilibrio (visualizacao)
                e  TIConfEq3DSim quando deseja salvar os dados em disco
                e TIConfEq3DSimPermRel se quizer adicionalmente determinar
                a permeabilidade relativa

PS:
A determina��o da permeabilidade relativa deve ser realizada por um objeto
PermeabilidadeRelativa
	No contrutor:
   Cria um objeto confEq (ou recebe um)
   Next:
   Chama next de confeq
   Obt�m as imagens
   Determina as permeabilidades intrinsecas
   Calcula a permeabilidade relativa para o ponto
@author 				Andr� Duarte Bueno
@version
@see						Veja assunto
*/

class TIConfiguracoesEquilibrio3D:public ITask
{
    COI20_CLASSID COI20_SAFECAST (TIConfiguracoesEquilibrio3D, IObject)
//--------------------------------------------------------------Atributos
private:
    //Listas de p�xeis
    //Lista dos pixeis que compoem a imagem da camara molhante e N�o molhante
    IListPixelRotuled * LPWChamber;
    //usada para determina��o das conectividades
    IListPixelRotuled *LPNWChamber;

    //Objetos fluidos reais
    TMFluido *fluidB;
    TMFluido *fluidA;

    //ATRIBUTOS CONTROLE
    //=1 raio cresce, =0 raio decresce
    bool direction;
    //ativo se a simula��o acabou
    bool endSimulation;
    //Se true salva resultados em disco
    bool saveToDisc;
    //Ativo se � para salvar simula��es parciais em disco
    bool saveParcialSimulation;

    //IMAGENS
    //Imagem distancia ao fundo (representa todo o meio poroso + camara N�o molhante)
    I3DRawImage *imgIDF;
    //Imagem fluidoA
    I3DRawImage *imgA;
    //Imagem fluidoB
    I3DRawImage *imgB;
    //Imagem fluidoA isolado
    I3DRawImage *imgY;
    //Imagem temporaria, usada processos internos
    I3DRawImage *imgTemp;
    //imagem do fluido que entra no meio poroso (usualmente=imgB)
    I3DRawImage *imgFluidIn;
    //imagem do fluido que sai do meio poroso (usualmente=imgA)
    I3DRawImage *imgFluidOut;

protected:
    //objeto camara
    TIChamber * chamber;

    //objetos flu�dos usados internamente para c�lculo
    TMFluido *fluidIn;
    TMFluido *fluidOut;

    //nome do imagem, recebido atraves da funcao Go? ou SetimageName();
    std::string imageName;

    //Como a imagem das configuracoes de equilibrio � desconexa
    //o borderDiscount corta a imagem, normalmente corte = 2 (pode ser definido atrav�s do di�logo de simula��o)
    //este corte � necess�rio em fun��o da simula��o da permeabilidade.
    uint borderDiscount;

    //ATRIBUTOS SIMULA��O
    //contadorPassosExecutados;
    uint32 pass;
    //raioMaximo
    uint32 maxRadius;
    //raioMinimo;
    uint32 minRadius;
    //raio em uso pois raio minimo pode ser 0
    int32 radius;
    //incremento (default =1)
    int32 increment;
    //numero de invers�es realizadas
    int32 numberInversion;
    //Ativo se a imgIDF ja foi determinada //NOVO em 13-11-2000
    bool imgIDFDeterminada;
    //default=inativo, se ativo o processo foi invertido
    bool inverted;

public:
//-------------------------------------------------------------Construtor
    TIConfiguracoesEquilibrio3D (TIChamber * _chamber, bool _molhabilidadeB =
                                     1, bool _compressibilidadeA =
                                     1, uint32 _incremento =
                                     1, uint32 _raioMaximo =
                                     0, uint32 _raioMinimo =
                                     0, bool _salvarDisco = false);

//--------------------------------------------------------------Destrutor
    virtual ~ TIConfiguracoesEquilibrio3D ();

//----------------------------------------------------------------M�todos
    //FUN��ES AUXILIARES
private:

    //Verifica a conectividade do flu�do com a camara
    void VerifyConectivit (IListPixel * _listPixel, I3DRawImage * _imgTemp);
    void DeterminaImgIDF ();

protected:

    //Salvar
    void SaveImg (I3DRawImage * &_imagem, std::string _nomeArquivo);
    virtual std::string GetClassName () const
    {
        return std::string ("TIConfiguracoesEquilibrio3D");
    };

    //sa�da de dados b�sicos do objeto confEq
    virtual ostream & Output (ostream & os);

    //METODOLOGIA DE CALCULO
    void SolucaoOmega ();		//Solu��o de Omega

    //Fun��es virtuais protegidas
    void DeterminaAbertura ();
    void DeterminaAberturaComplementar ();
    //Solu��o para fluido B
    virtual void DefineFluidIn ();
    //Solu��o para fluido A
    virtual void DefineFluidOut ();
    //Solu��o para fluido A isolado
    virtual void DefineFluidIsolated ();

    //Fun��es intermediarias para Next
    //Cria objetos
    virtual void IntermediatNextCreate ()
    {
    };
    //Calcula processos
    virtual void IntermediatNextCalculate ()
    {
    };
    //Sa�da de dados
    virtual void IntermediatNextOutput ()
    {
    };

    virtual void IntermediatNextClose ()
    {
    };
    //novo, salva dados simula��o em disco para reinicializa��o em outro dia (testar)
    void SaveParcialSimulation ();
public:
    //Processa para raioAtual, retorna 1 se ok, e 0 se endSimulation=1
    virtual bool Next ();
    //Processa para todos os raios e salva em disco
    bool Go ();
    //Processa para _raio, faz compressibilidade A = 1
    bool Direct (uint32 _raio);
    //Inverte a dire��o do processo
    void InvertDirection ();

    //nova, processa toda a histerese
    //virtual bool Histerese (); //
    virtual bool Histerese (int numeroCiclos);	//

//-------------------------------------------------------------Sobrecarga
//--------------------------------------------------------------------Get
public:
    const std::string GetimageName () const
    {
        return imageName;
    };

//Retornam os flu�dos
    TMFluido *GetfluidA () const
    {
        return fluidA;
    }
    TMFluido *GetfluidB () const
    {
        return fluidB;
    }
    TMFluido *GetfluidIn () const
    {
        return fluidIn;
    }
    TMFluido *GetfluidOut () const
    {
        return fluidOut;
    }
    int GetborderDiscount () const
    {
        return borderDiscount;
    };
    int32 GetnumberInversion () const
    {
        return numberInversion;
    };
    //retorna o raio do ultimo passo executado
    int32 Getradius () const
    {
        return radius - increment;
    };
    uint32 GetmaxRadius () const
    {
        return maxRadius;
    };
    uint32 GetminRadius () const
    {
        return minRadius;
    };
    uint32 Getpass () const
    {
        return pass;
    };
    //retorna o flag de fim da simula��o
    bool GetendSimulation () const
    {
        return endSimulation;
    };
    bool Getinverted () const
    {
        return inverted;
    };
    bool GetsaveParcialSimulation () const
    {
        return saveParcialSimulation;
    };

    //Retorna ponteiro para a camara
    TIChamber *Getchamber () const
    {
        return chamber;
    };

    //Retornam imagens
    //retorna uma imagem composta (com as imagens A,B, Y e fundo)
    I3DRawImage *GetImgComposta ();
    //retorna somente o ponteiro
    I3DRawImage *GetptrimgA () const
    {
        return imgA;
    };
    I3DRawImage *GetptrimgB () const
    {
        return imgB;
    };
    I3DRawImage *GetptrimgY () const
    {
        return imgY;
    };

    I3DRawImage *GetptrimgFluidIn () const
    {
        return imgFluidIn;
    };				//novo
    I3DRawImage *GetptrimgFluidOut () const
    {
        return imgFluidOut;
    };				//novo

    //Retornam somente a parte meio poroso da imagem, desmarcando as camaras
    I3DRawImage *GetimgA () const;
    I3DRawImage *GetimgB () const;
    I3DRawImage *GetimgY () const;
    //Retornam as imagens incluindo as camaras
    I3DRawImage *GetimgFluidIn () const;
    I3DRawImage *GetimgFluidOut () const;
    //Retornam somente o n�cleo das imagens, desconsiderando as camaras
    I3DRawImage *GetNucleoImgB () const;
    I3DRawImage *GetNucleoImgA () const;
    I3DRawImage *GetNucleoImgY () const;

//--------------------------------------------------------------------Set
    void SetimageName (std::string _imageName)
    {
        imageName = _imageName;
    };

    void SetfluidA (TMFluido * &_fa)
    {
        if (fluidA)
            delete fluidA;
        fluidA = _fa;
    };
    void SetfluidB (TMFluido * &_fb)
    {
        if (fluidB)
            delete fluidB;
        fluidB = _fb;
    };
    void SetfluidIn (TMFluido * &_fi)
    {
        fluidIn = _fi;
    };
    void SetfluidOut (TMFluido * &_fo)
    {
        fluidOut = _fo;
    };
    void SetAux ()
    {				//Define fluidIn fluidOut imgFluidIn imngFluidOut
        if (!inverted)
        {
            fluidIn = fluidB;
            fluidOut = fluidA;
            imgFluidIn = imgB;
            imgFluidOut = imgA;
        }
        else
        {
            fluidIn = fluidA;
            fluidOut = fluidB;
            imgFluidIn = imgA;
            imgFluidOut = imgB;
        }
    };

    void SetImgB (I3DRawImage * _img)
    {
        if (imgB)
            delete imgB;
        imgB = _img;
    };
    void SetImgA (I3DRawImage * _img)
    {
        if (imgA)
            delete imgA;
        imgA = _img;
    };
    void SetImgY (I3DRawImage * _img)
    {
        if (imgY)
            delete imgY;
        imgY = _img;
    };

    void Setincrement (int32 & _inc)
    {
        direction ? increment = _inc : increment = -_inc;
    };
    void SetminRadius (uint32 & _mr)
    {
        minRadius = _mr;
    };
    void SetmaxRadius (uint32 & _mr)
    {
        maxRadius = _mr;
    };

    void SetborderDiscount (unsigned int &_borderDiscount)
    {
        borderDiscount = _borderDiscount;
    };
    void Setpass (uint32 _pass)
    {
        pass = _pass;
    };
    void Setdirection (bool _d)
    {
        direction = _d;
    };
    void Setinverted (bool _d)
    {
        inverted = _d;
    };
    void SetsaveToDisc (bool _d)
    {
        saveToDisc = _d;
    };
    void SetendSimulation (bool _d)
    {
        endSimulation = _d;
    };
    void SetsaveParcialSimulation (bool _d)
    {
        saveParcialSimulation = _d;
    };

//-----------------------------------------------------------------Friend
    friend ostream & operator<< (ostream & os,
                                 const TIConfiguracoesEquilibrio3D & obj);

    //friend istream& operator>> (istream& is, |CLASSNAME|& obj);

    //Atendendo a pedido do Damiani/Philippi
public:
    uint32 ProcessSteps (const T3Dint32 &);	//Passa para o Imagog a informa��o do passo atual
    T3Dint32 processStep;		//indicador de progresso


};

//-----------------------------------------------------------------Friend
//Declara��o de Fun��es Friend
ostream & operator<< (ostream & os, const TIConfiguracoesEquilibrio3D & obj);
//istream& operator>> (istream& is, |CLASSNAME|& obj);
#endif

/*
#include <gimg/process.h>
class IReconstruct : public IProcess
{
	COI20_CLASSID
   COI20_SAFECAST( IReconstruct, IObject )
public:
	uint32 ProcessSteps( const T3Dint32 & );
	T3Dint32 processStep;
*/
/*
  I3DRawImage* GetimgPrototypeptr()	const	{return imgPrototype;};
*/
