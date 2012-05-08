
#ifndef TISCDraynage_h
#define TISCDraynage_h
/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: |CLASSNAME|...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     Andr� Duarte Bueno
@file       TISCDraynage.h
@begin      Fri Nov 17 2000
@copyright  (C) 2000 by Andr� Duarte Bueno
@email      andre@lmpt.ufsc.br
*/


//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------

/*
=========================================================================
Documentacao CLASSE: TISCDraynage
=========================================================================
Assunto:        TISCDraynage
Superclasse:    TIChamber
Descri��o: 			Declara a classe TISCDraynage.
                Uma camara para experimentos de drenagem
                Somente pinta os pixeis na imagem prototipo, de forma
                a criar uma simula��o de drenagem.
                PS: De fato ocorre o que define a drenagem
@short					Breve descri��o
@author 				Andr� Duarte Bueno
@version
@see						Veja assunto
*/
/*
============================================================================
Documentacao CLASSE:	  TISCDraynage
============================================================================
Assunto:               TISCDraynage.
Superclasse:           a propria
Descri��o:             Declara a classe TISCDraynage.
                       Uma camara para experimentos de drenagem
                       Somente pinta os pixeis na imagem prototipo, de forma
                       a criar uma simula��o de drenagem.
                       PS: De fato ocorre o que define a drenagem
Acesso:
Cardinalidade:
Abstrata/Concreta:	  Abstrata
Arquivo de documentacao auxiliar:
----------------------------------------------------------------------------
*/
//Arquivos da COILIB
//#include <imago/img3d/pixit3d8.h>
#include <imago/img3d/pixit/pixit8.h>

//Arquivos da LIB_LMPT
#ifndef _LIB_LMPT_CLASS_h
#include <Base/_LIB_LMPT_CLASS.h>
#endif

#ifndef TISChamber_h
#include <TConEquCOILIB/TISChamber.h>
#endif

class TISCDraynage:public TISChamber
{
//--------------------------------------------------------------Atributos
private:
protected:
public:
//-------------------------------------------------------------Construtor
    TISCDraynage (I3DRawImage * _imgPorousMedia):TISChamber (_imgPorousMedia, 0, 0)
//rTISCDraynage__wb0ca0 //wb0ca1 Intrus�o merc�rio
    {
        //Posicoes da camara 3*3*3
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

        //Pinta as posi��es da iamgem prototipo
        I3DPixelIterator8 *piimgPrototype =
            I3DPixelIterator8::SafeCast (imgPrototype->InitPixelIterator ());
        //uint8 v = cChamberWettingFluid ;
        uint8 v;
        v = cChamberNotWettingFluid;	// novo em 20-2-2001
        //front
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
        //v = cChamberNotWettingFluid;
        v = cChamberWettingFluid;	// novo em 20-2-2001
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
    virtual ~ TISCDraynage ()
    {
    };

//----------------------------------------------------------------M�todos
private:
protected:
public:
//-------------------------------------------------------------Sobrecarga

//--------------------------------------------------------------------Get
    //Rretorna o nome da classe
    virtual std::string GetClassName ()const
    {
        return std::string ("TISCDraynage");
    };

    virtual int Type () const
    {
        return 3;
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
