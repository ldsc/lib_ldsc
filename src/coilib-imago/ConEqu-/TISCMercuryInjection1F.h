
#ifndef TISCMercuryInjection1F_h
#define TISCMercuryInjection1F_h

/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: |CLASSNAME|...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     Andr� Duarte Bueno
@file       TISCMercuryInjection1F.h
@begin      Fri Nov 17 2000
@copyright  (C) 2000 by Andr� Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------
//iterator's
//aqui
//#include <imago/img3d/pixit3d8.h>
#include <imago/img3d/pixit/pixit8.h>
//#include <imago/img3d/image/raw.h>
///home/andre2/Andre/Desenvolvimento/Imago/Imago/Imago/heart/include/imago/img3d/image/raw.h

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
#ifndef _LIB_LMPT_CLASS_h
#include <Base/_LIB_LMPT_CLASS.h>
#endif

//classe base
#ifndef TISChamber_h
#include <TConEquCOILIB/TISChamber.h>
#endif

/*
=========================================================================
Documentacao CLASSE: TISCMercuryInjection1F
=========================================================================
Assunto:        camaras
Superclasse:    TChamber
Descri��o:			Declara a classe TISCMercuryInjection1F.
								Representa uma camara de um porosimetro a mercurio
                com 1 faces de intrus�o
@short					Breve descri��o
@author 				Andr� Duarte Bueno
@version
@see						Veja assunto
*/


//Para intrusao de mercurio o fluido B � formado pela Camara N�o Molhante
class TISCMercuryInjection1F:public TISChamber
{
//--------------------------------------------------------------Atributos
private:
protected:
public:
//-------------------------------------------------------------Construtor

    TISCMercuryInjection1F (I3DRawImage *
                            _imgPorousMedia):TISChamber (_imgPorousMedia)
    {
        //imgPrototype->SetBits(cSolid); aqui
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
        //022 122  222      x y z

        I3DPixelIterator8 *piimgPrototype =
            I3DPixelIterator8::SafeCast (imgPrototype->InitPixelIterator ());

        uint8 v;

        //Front (face z=0, primeiro plano)
        v = cChamberNotWettingFluid;
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

        //Back
        v = cWall;
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

        //Informa que n�o existe a camara com flu�do molhante //novo
        wettingChamber = false;
    }

//--------------------------------------------------------------Destrutor
    virtual ~ TISCMercuryInjection1F ()
    {
    };

//----------------------------------------------------------------M�todos
private:
protected:
public:
//-------------------------------------------------------------Sobrecarga

//--------------------------------------------------------------------Get

    //Retorna o nome da classe
    virtual std::string GetClassName ()const
    {
        return std::string ("TISCMercuryInjection1F");
    };

//--------------------------------------------------------------------Set
//-----------------------------------------------------------------Friend
//      friend ostream& operator<< (ostream& os, const |CLASSNAME|& obj);
//      friend istream& operator>> (istream& is, |CLASSNAME|& obj);
};

//-----------------------------------------------------------------Friend
//Declara��o de Fun��es Friend
//ostream& operator<< (ostream& os, const |CLASSNAME|& obj);
//istream& operator>> (istream& is, |CLASSNAME|& obj);

#endif
