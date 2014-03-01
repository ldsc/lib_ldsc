
#ifndef TISCMercuryInjection2F_h
#define TISCMercuryInjection2F_h

/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: TISCMercuryInjection2F...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     Andr� Duarte Bueno
@file       TISCMercuryInjection2F.h
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
#ifndef _LIB_LMPT_CLASS_h
#include <Base/_LIB_LMPT_CLASS.h>
#endif

#ifndef TISCMercuryInjection1F_h
#include <TConEquCOILIB/TISCMercuryInjection1F.h>
#endif

/*
=========================================================================
Documentacao Classe: TISCMercuryInjection2F
=========================================================================
Assunto:        Camaras
Superclasse:	TISCMercuryInjection2F
Descri��o:	Declara a classe TISCMercuryInjection2F.
                Representa uma camara de um porosimetro a mercurio
                com 2 faces de intrus�o
@short		Breve descri��o
@author 	Andr� Duarte Bueno
@version
@see		Veja assunto
*/


//Para intrusao de mercurio o fluido B � formado pela camara N�o Molhante
// cSolid = 0;
//cPorousMedia
// cChamberWettingFluid = 4;
// cChamberNowWettingFluid = 5;

class TISCMercuryInjection2F:public TISCMercuryInjection1F
{
//--------------------------------------------------------------Atributos
private:
protected:
public:

//-------------------------------------------------------------Construtor
    TISCMercuryInjection2F (I3DRawImage *
                            _imgPorousMedia):TISCMercuryInjection1F
            (_imgPorousMedia)
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

        //Face z=2 (terceiro plano)
        uint8 v = cChamberNotWettingFluid;
        piimgPrototype->Set (0, 0, 2, v);
        piimgPrototype->Set (1, 0, 2, v);
        piimgPrototype->Set (2, 0, 2, v);
        piimgPrototype->Set (0, 1, 2, v);
        piimgPrototype->Set (1, 1, 2, v);
        piimgPrototype->Set (2, 1, 2, v);
        piimgPrototype->Set (0, 2, 2, v);
        piimgPrototype->Set (1, 2, 2, v);
        piimgPrototype->Set (2, 2, 2, v);
    }

//--------------------------------------------------------------Destrutor
    virtual ~ TISCMercuryInjection2F ()
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
        return std::string ("TISCMercuryInjection2F");
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
