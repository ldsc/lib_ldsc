
#ifndef TISCMercuryInjection6F_h
#define TISCMercuryInjection6F_h

/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: |CLASSNAME|...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     Andr� Duarte Bueno
@file       TISCMercuryInjection6F.h
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

#ifndef TISCMercuryInjection4F_h
#include <TConEquCOILIB/TISCMercuryInjection4F.h>
#endif

/*
=========================================================================
Documentacao Classe: TISCMercuryInjection6F
=========================================================================
Assunto:        Camaras
Superclasse:    TISCMercuryInjection6F
Descri��o:
@short					Declara a classe TISCMercuryInjection6F.
                Representa uma camara de um porosimetro a mercurio
                com 6 faces de intrus�o
@author 				Andr� Duarte Bueno
@version
@see						Veja assunto
*/


class TISCMercuryInjection6F:public TISCMercuryInjection4F
{
//--------------------------------------------------------------Atributos
private:
protected:
public:
//-------------------------------------------------------------Construtor
    TISCMercuryInjection6F (I3DRawImage *
                            _imgPorousMedia):TISCMercuryInjection4F
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
        uint8 v = cChamberNotWettingFluid;

        //center //novo, agora, s� seta   1,0,1 e 1,2,1
        /*piimgPrototype->Set(0,0,1,v); */
        piimgPrototype->Set (1, 0, 1, v);
        /*piimgPrototype->Set(2,0,1,v); */
        //piimgPrototype->Set(0,1,1,v);         piimgPrototype->Set(1,1,1,);     piimgPrototype->Set(2,1,1,v);
        /*piimgPrototype->Set(0,2,1,v); */
        piimgPrototype->Set (1, 2, 1, v);
        /*piimgPrototype->Set(2,2,1,v); */
    };

//--------------------------------------------------------------Destrutor
    virtual ~ TISCMercuryInjection6F ()
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
        return std::string ("TISCMercuryInjection6F");
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
