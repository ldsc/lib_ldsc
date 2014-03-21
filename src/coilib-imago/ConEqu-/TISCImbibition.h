#ifndef TISCImbibition_h
#define TISCImbibition_h
/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: |CLASSNAME|...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     André Duarte Bueno
@file       TISCImbibition.h
@begin      Fri Nov 17 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------
//#include <imago/img3d/pixit3d8.h>
#include <imago/img3d/pixit/pixit8.h>

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
#ifndef _LIB_LMPT_CLASS_h
#include <Base/_LIB_LMPT_CLASS.h>
#endif

#ifndef TISChamber_h
#include <TConEquCOILIB/TISChamber.h>
#endif

/*
=========================================================================
Documentacao Classe: TISCImbibition
=========================================================================
Assunto:        Camaras
Superclasse:    TISCImbibition
Descri��o:
@short					Breve descri��o
@author 				André Duarte Bueno
@version
@see						Veja assunto
*/

//Na imbibi��o wb sempre 1
//rTISCImbibition_wb1ca0
//rTISCImbibition_wb1ca1
class TISCImbibition:public TISChamber
{
//--------------------------------------------------------------Atributos
private:
protected:
public:
//-------------------------------------------------------------Construtor
    TISCImbibition (I3DRawImage * _imgPorousMedia):TISChamber (_imgPorousMedia,
                    1, 0)
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
        I3DPixelIterator8 *piimgPrototype =
            I3DPixelIterator8::SafeCast (imgPrototype->InitPixelIterator ());

        //front
        uint8 v = cChamberNotWettingFluid;
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
        v = cWall;
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
        v = cChamberWettingFluid;
        piimgPrototype->Set (0, 0, 2, v);
        piimgPrototype->Set (1, 0, 2, v);
        piimgPrototype->Set (2, 0, 2, v);
        piimgPrototype->Set (0, 1, 2, v);
        piimgPrototype->Set (1, 1, 2, v);
        piimgPrototype->Set (2, 1, 2, v);
        piimgPrototype->Set (0, 2, 2, v);
        piimgPrototype->Set (1, 2, 2, v);
        piimgPrototype->Set (2, 2, 2, v);

        piimgPrototype->Set (1, 1, 1, cPorousMedia);
    };

//--------------------------------------------------------------Destrutor
    virtual ~ TISCImbibition ()
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
        return std::string ("TISCImbibition");
    };

    //novo, define o tipo de camara
    virtual int Type () const
    {
        return 2;
    };

//--------------------------------------------------------------------Set
//-----------------------------------------------------------------Friend
    //friend ostream& operator<< (ostream& os, const |CLASSNAME|& obj);
    //friend istream& operator>> (istream& is, |CLASSNAME|& obj);
};

//-----------------------------------------------------------------Friend
//Declara��o de Fun��es Friend
//ostream& operator<< (ostream& os, const |CLASSNAME|& obj);
//istream& operator>> (istream& is, |CLASSNAME|& obj);

#endif
