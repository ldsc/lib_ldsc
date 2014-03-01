#ifndef TISCMercuryInjection4F_h
#define TISCMercuryInjection4F_h

/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: |CLASSNAME|...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     Andr� Duarte Bueno
@file       TISCMercuryInjection4F.h
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

#ifndef TISCMercuryInjection2F_h
#include <TConEquCOILIB/TISCMercuryInjection2F.h>
#endif
/*
=========================================================================
Documentacao Classe: TISCMercuryInjection4F
=========================================================================
Assunto:        Camaras
Superclasse:    TIChamber
Descri��o:	Declara a classe TISCMercuryInjection4F.
		Representa uma camara de um porosimetro a mercurio
		com 4 faces de intrus�o
@short		Breve descri��o
@author 	Andr� Duarte Bueno
@version
@see		Veja assunto
*/
class TISCMercuryInjection4F:public TISCMercuryInjection2F
{
//--------------------------------------------------------------Atributos
private:
protected:
public:
//-------------------------------------------------------------Construtor
    TISCMercuryInjection4F (I3DRawImage *
                            _imgPorousMedia):TISCMercuryInjection2F
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
        //uint8  v =cChamberNotWettingFluid;

        //center (novo, agora, seta toda a coluna 0 e 2
        piimgPrototype->Set (0, 0, 1, cChamberNotWettingFluid);	/*piimgPrototype->Set(1,0,1,cWall); */
        piimgPrototype->Set (2, 0, 1, cChamberNotWettingFluid);
        piimgPrototype->Set (0, 1, 1, cChamberNotWettingFluid);	/*piimgPrototype->Set(1,1,1,cWall); */
        piimgPrototype->Set (2, 1, 1, cChamberNotWettingFluid);
        piimgPrototype->Set (0, 2, 1, cChamberNotWettingFluid);	/*piimgPrototype->Set(1,2,1,cWall); */
        piimgPrototype->Set (2, 2, 1, cChamberNotWettingFluid);

    }

//--------------------------------------------------------------Destrutor
    virtual ~ TISCMercuryInjection4F ()
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
        return std::string ("TISCMercuryInjection4F");
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
