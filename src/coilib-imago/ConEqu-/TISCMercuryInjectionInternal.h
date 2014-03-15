
#ifndef TISCMercuryInjectionInternal_h
#define TISCMercuryInjectionInternal_h

/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: |CLASSNAME|...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     Andr� Duarte Bueno
@file       TISCMercuryInjectionInternal.h
@begin      Fri Nov 17 2000
@copyright  (C) 2000 by Andr� Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------
#include <string>

//#include <img3d/pixit3d8.h>
#include <imago/img3d/pixit/pixit8.h>

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
#ifndef _LIB_LMPT_CLASS_h
#include <Base/_LIB_LMPT_CLASS.h>
#endif

#include <TConEquCOILIB/TISChamber.h>

/*
=========================================================================
Documentacao Classe: TISCMercuryInjectionInternal
=========================================================================
Assunto:        Camaras
Superclasse:    TISCMercuryInjectionInternal
Descri��o:			Declara a classe TISCMercuryInjectionInternal.
                Uma camara para experimentos de embebi��o
@short
@author 				Andr� Duarte Bueno
@version
@see						Veja assunto
*/


class TISCMercuryInjectionInternal:public TISChamber
{
//--------------------------------------------------------------Atributos
private:
protected:
public:
//-------------------------------------------------------------Construtor
    TISCMercuryInjectionInternal (I3DRawImage *
                                  _imgPorousMedia):TISChamber (_imgPorousMedia)
    {
        //front
        //000 100  200
        //010 110  210
        //020 120  220
        //center
        //001 101  201
        //011 111  211
        //021 121  221
        //back
        //002 102  202
        //012 112  212
        //022 122  222

        //Calcula a dimensao base
        baseDimension = (size.x - baseDimension) / 2;	//?????????

        I3DPixelIterator8 *piimgPrototype =
            I3DPixelIterator8::SafeCast (imgPrototype->InitPixelIterator ());

        //front
        uint8 v;
        v = cPorousMedia;
        piimgPrototype->Set (0, 0, 0, v);
        piimgPrototype->Set (1, 0, 0, v);
        piimgPrototype->Set (2, 0, 0, v);
        piimgPrototype->Set (0, 1, 0, v);
        piimgPrototype->Set (1, 1, 0, v);
        piimgPrototype->Set (2, 1, 0, v);
        piimgPrototype->Set (0, 2, 0, v);
        piimgPrototype->Set (1, 2, 0, v);
        piimgPrototype->Set (2, 2, 0, v);

        //center
        //uint8  v  = cPorousMedia;
        piimgPrototype->Set (0, 0, 1, v);
        piimgPrototype->Set (1, 0, 1, v);
        piimgPrototype->Set (2, 0, 1, v);
        piimgPrototype->Set (0, 1, 1, v);
        piimgPrototype->Set (1, 1, 1, v);
        piimgPrototype->Set (2, 1, 1, v);
        piimgPrototype->Set (0, 2, 1, v);
        piimgPrototype->Set (1, 2, 1, v);
        piimgPrototype->Set (2, 2, 1, v);

        //back
        //uint8  v  = cPorousMedia;
        piimgPrototype->Set (0, 0, 2, v);
        piimgPrototype->Set (1, 0, 2, v);
        piimgPrototype->Set (2, 0, 2, v);
        piimgPrototype->Set (0, 1, 2, v);
        piimgPrototype->Set (1, 1, 2, v);
        piimgPrototype->Set (2, 1, 2, v);
        piimgPrototype->Set (0, 2, 2, v);
        piimgPrototype->Set (1, 2, 2, v);
        piimgPrototype->Set (2, 2, 2, v);

        //A camara com flu�do n�o molhante fica no centro
        v = cChamberNotWettingFluid;
        piimgPrototype->Set (1, 1, 1, v);

        //Seta que inexiste a camara molhante
        wettingChamber = false;
    };

//--------------------------------------------------------------Destrutor
    virtual ~ TISCMercuryInjectionInternal ()
    {
    };

//----------------------------------------------------------------M�todos
private:
protected:
public:
//-------------------------------------------------------------Sobrecarga

//--------------------------------------------------------------------Get
    //retorna o nome da classe
    virtual std::string GetClassName ()const
    {
        return std::string ("TISCMercuryInjectionInternal");
    };

//--------------------------------------------------------------------Set
//-----------------------------------------------------------------Friend
//      friend ostream& operator<< (ostream& os, const |CLASSNAME|& obj);
//      friend istream& operator>> (istream& is, |CLASSNAME|& obj);


//Desativa cálculo flu�do que sai
// virtual bool GetcalculateFluidOut()          const   {return false;};
//e fluido isolado
// virtual bool GetcalculateFluidIsolated()     const   {return false;};
//Tipo 1 � o default, tipo 2 � para intrus�o de merc�rio
// virtual int TypeOfChamber()                                  const   {return 2;};

};

//-----------------------------------------------------------------Friend
//Declara��o de Fun��es Friend
//ostream& operator<< (ostream& os, const |CLASSNAME|& obj);
//istream& operator>> (istream& is, |CLASSNAME|& obj);

#endif
