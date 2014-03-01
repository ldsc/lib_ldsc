#ifndef TICChamber_h
#define TICChamber_h

/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: |CLASSNAME|...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     Andr� Duarte Bueno
@file       TICChamber.h
@begin      Fri Nov 17 2000
@copyright  (C) 2000 by Andr� Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------
#include <string>


//-----------------------------------------------------------------------
//Bibliotecas imago
//-----------------------------------------------------------------------
//#include <imago/img3d/rawimg3d.h>
//#include <imago/img3d/pixit3d8.h>
#include <imago/img3d/image/raw.h>
#include <imago/img3d/pixit/pixit8.h>

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
#ifndef _LIB_LMPT_CLASS_h
#include <Base/_LIB_LMPT_CLASS.h>
#endif

//Classe base
#ifndef TIChamber_h
#include <TConEquCOILIB/TIChamber.h>
#endif

/*
=========================================================================
Documentacao Classe: TICChamber
=========================================================================
Assunto:        Assunto
Superclasse:    |CLASSNAME|
Descri��o: 			Declara a classe TIChamber.
@short					Breve descri��o
@author 				Andr� Duarte Bueno
@version
@see						Veja assunto
*/
class TICChamber:public TIChamber
{
//--------------------------------------------------------------Atributos
private:
protected:
public:
//-------------------------------------------------------------Construtor
    //O construtor deve alocar a imagem prototipo
    TICChamber (I3DRawImage * &img):TIChamber (img->Size ())
    {
        //Cria imagem prototipo do tamanho de size
        imgPrototype = new I3DRawImage (img->Size (), 8);
        assert (imgPrototype != NULL);
        //seta atributos
        imgPrototype->SetVoxelSize (img->VoxelSize ());
        //seta atributos
        imgPrototype->SetAmplifyFactor (img->AmplifyFactor ());
    };

//--------------------------------------------------------------Destrutor
    virtual ~ TICChamber ()
    {
    };

//----------------------------------------------------------------M�todos
private:
protected:
public:
    virtual void Draw (I3DRawImage * &imgBinaria, uint32 & indiceCor);
    virtual void DrawPorousMedia (I3DRawImage * &imgBinaria);
    virtual void VolumeRegionPorousMedia ();

    //Cria imagem nova e preenche a mesma com o que foi marcado como sendo meio poroso
    /*
       //virtual na classe base
       virtual I3DRawImage* GetimgPorousMedia()
       {
       //Cria imagem nova binaria
       I3DRawImage* img = GetNewImg();
       //Desenha o meio Poroso
       DrawPorousMedia(img);
       //retorna a imagem
       return img;
       };
     */

//-------------------------------------------------------------Sobrecarga

//--------------------------------------------------------------------Get
    //retorna o nome da classe
    virtual std::string GetClassName () const
    {
        return std::string ("TICChamber");
    };

//--------------------------------------------------------------------Set
//-----------------------------------------------------------------Friend
//      friend ostream& operator<< (ostream& os, const |CLASSNAME|& obj);/
//      friend istream& operator>> (istream& is, |CLASSNAME|& obj);
};

//-----------------------------------------------------------------Friend
//Declara��o de Fun��es Friend
//ostream& operator<< (ostream& os, const |CLASSNAME|& obj);
//istream& operator>> (istream& is, |CLASSNAME|& obj);

#endif
