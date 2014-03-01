#ifndef TIConfEq3DSimPermRel_h
#define TIConfEq3DSimPermRel_h

/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: |TIConfEq3DSimPermRel|...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     Andr� Duarte Bueno
@file       TIConfEq3DSimPermRel.h
@begin      Fri Nov 17 2000
@copyright  (C) 2000 by Andr� Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------
#include <fstream.h>

//-----------------------------------------------------------------------
//COI20
//-----------------------------------------------------------------------
#include <coi20/sys/adt/svector.h>

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
#ifndef _LIB_LMPT_CLASS_h
#include <Base/_LIB_LMPT_CLASS.h>
#endif

#ifndef TIConfEq3DSim_h
#include <TConEquCOILIB/TIConfEq3DSim.h>
#endif

/*
=========================================================================
Documentacao Classe: |TIConfEq3DSimPermRel|
=========================================================================
Assunto:        TIConfEq3DSimPermRel
Superclasse:    a propria
Descri��o: 	Declara a classe TIConfEq3DSimPermRel.
		Herdeira da classe TIConfEq3DSim,
                acrescenta nos calculos informa��es relacionadas a determina��o da permeabilidade relativa.
                Ou seja, use TConfiguracoesEquilibrio para obter as configuracoes de equilibrio (visualizacao)
                TIConfEq3DSim para determina��o das curvas de pressao capilar
                e  TIConfEq3DSimPermRel quando deseja ainda as curvas de permeabilidade relativa.
@short		Breve descri��o
@author 	Andr� Duarte Bueno
@version
@see		Veja assunto
*/

//Classe nova, inexistente no imagoh3-funciona
class TIConfEq3DSimPermRel:public TIConfEq3DSim
{
    COI20_CLASSID COI20_SAFECAST (TIConfEq3DSimPermRel, IObject)
//--------------------------------------------------------------Atributos
private:

    //Vetor com as permeabilidades relativas de A
    ISimpleVectorOf < float >prfa;

    //Vetor com as permeabilidades relativas de B
    ISimpleVectorOf < float >prfb;

protected:
    //Valor permeabilidade intrinseca
    double intrinsicPermeability;

public:

//-------------------------------------------------------------Construtor
    TIConfEq3DSimPermRel (TIChamber * _chamber, bool _molhabilidadeB = 1,
                          bool _compressibilidadeA = 1,
                          uint32 _incremento = 1, uint32 _raioMaximo = 0,
                          uint32 _raioMinimo = 0, bool _salvarDisco = false,
                          int _borderDiscount = 2
                                                //novo em 22-7-2002
                                                , string fileArq = '\0'):
            TIConfEq3DSim (_chamber, _molhabilidadeB, _compressibilidadeA,
                           _incremento, _raioMaximo, _raioMinimo, _salvarDisco
                           //novo em 22-7-2002
                           , fileArq)
            //,prfa(_raioMaximo - _raioMinimo + 1)
            //,prfb(_raioMaximo - _raioMinimo + 1)
            //inicia vetores
            , prfa (chamber->GetmaxRadius () + 1), prfb (chamber->GetmaxRadius () + 1)
    {
        borderDiscount = _borderDiscount;

        //Zera vetores
        memset (prfa.Data (), 0, prfa.Size () * sizeof (float));
        memset (prfb.Data (), 0, prfb.Size () * sizeof (float));

        intrinsicPermeability = 1;
        determinarPermeabilidade = 1;
    };

//--------------------------------------------------------------Destrutor
    virtual ~ TIConfEq3DSimPermRel ()
    {
    };

    //Retorna ponteiro para os vetores
    const ISimpleVectorOf < float >&GetPermRelFluid_A () const
    {
        return prfa;
    };
    const ISimpleVectorOf < float >&GetPermRelFluid_B () const
    {
        return prfb;
    };

//----------------------------------------------------------------M�todos
private:
protected:
    virtual double CalculaPermeabilidade (I3DRawImage * &img);
    virtual void PrepareConfEqSim ();

    //virtual void IntermediatNextCreate();
    virtual void IntermediatNextCalculate ();
    virtual void IntermediatNextOutput ();
    //virtual void IntermediatNextClose();

public:

//-------------------------------------------------------------Sobrecarga

//--------------------------------------------------------------------Get
    virtual std::string GetClassName ()const
    {
        return std::string ("TIConfEq3DSimPermRel");
    };

//novo, s� determina permeabilidade se ativo,
//por detfault � ativo
    bool determinarPermeabilidade;
    bool GetdeterminarPermeabilidade ()
    {
        return determinarPermeabilidade;
    };
    void SetdeterminarPermeabilidade (bool b)
    {
        determinarPermeabilidade = b;
    };

//--------------------------------------------------------------------Set
//-----------------------------------------------------------------Friend
//      friend ostream& operator<< (ostream& os, const |TIConfEq3DSimPermRel|& obj);
//      friend istream& operator>> (istream& is, |TIConfEq3DSimPermRel|& obj);

//-----------------------------------------------------------------Friend
//Declara��o de Fun��es Friend
//ostream& operator<< (ostream& os, const |TIConfEq3DSimPermRel|& obj);
//istream& operator>> (istream& is, |TIConfEq3DSimPermRel|& obj);
};
#endif
