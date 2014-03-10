
#ifndef IListPixelRotuled_h
#define IListPixelRotuled_h

/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: |CLASSNAME|...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     Andr� Duarte Bueno
@file       IListPixelRotuled.h
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
#ifndef IListPixel_h
#include "TLista/IListPixel.h"
#endif

/*
=========================================================================
Documentacao Classe: |CLASSNAME|
=========================================================================
Assunto:        Assunto
Superclasse:    |CLASSNAME|
Descri��o:
@short		Breve descri��o
@author 	Andr� Duarte Bueno
@version
@see		Veja assunto
*/

/*
----------------------------------------------------------------------------
Nome deste arquivo:	IListPixelRotuled.h
Nome da classe:      IListPixelRotuled

Descri��o:
----------------------------------------------------------------------------
*/

//IListPixelRotulos que realiza a determinação da lista de p�xeis,
//utilizando a rotulagem de uma imagem bin�ria recebida
//Determina a lista de p�xeis (lista com 1 p�xel para cada objeto)
class IListPixelRotuled:public IListPixel
{
//--------------------------------------------------------------Atributos
private:
protected:
public:
//-------------------------------------------------------------Construtor
    IListPixelRotuled ()
    {
    };
//--------------------------------------------------------------Destrutor
    virtual ~ IListPixelRotuled ()
    {
    };
//----------------------------------------------------------------M�todos
private:
protected:
public:
    //Determina a lista de pixeis para a imagem passada
    //Go recebe uma imagem, realiza a rotulagem da mesma, e define
    //a menor  lista de p�xeis que permite obter os objetos
    //j� aloca a lista de p�xeis e preenche os vetores.
    virtual bool Go (I3DRawImage * &imagemBinaria);

//-------------------------------------------------------------Sobrecarga
//--------------------------------------------------------------------Get
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
