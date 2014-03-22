//=======================================================================
//PROJETO:          Biblioteca LIB_LMPT
//                  Ramo: TPadrao_ramo
//=======================================================================
//
//Desenvolvido por:     Laboratorio de Meios Porosos
//                          e Propriedades Termofisicas     [LMPT].
//@author:          André Duarte Bueno
//File:             IListPixel.cpp
//begin:            Fri Nov 17 2000
//copyright:        (C) 2000 by André Duarte Bueno
//email:            andre@lmpt.ufsc.br

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------
//#include <fstream>

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
//#include <path/nome>

/*
-------------------------------------------------------------------------
Fun��o:
-------------------------------------------------------------------------
@short  :
@author :André Duarte Bueno
 
@param  :
@return :
*/


#ifndef IListPixel_cpp
#define IListPixel_cpp
/*
----------------------------------------------------------------------------
PROJETO:					Imago
----------------------------------------------------------------------------

Desenvolvido por:  Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:     Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi,
		   Andre D.Bueno,Luiz Orlando Emerich, Marcos Damiani,...
Copyright @1997-1999:  Todos os direitos reservados.
Nome deste arquivo:	  IListPixel.h
Nome da classe:        IListPixel
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/

//C++
//#include <fstream.h>
#include <assert.h>

//Imago
//#include <img3d/pixit3d1.h>
//#include <img3d/pixit3d32.h>                  //iterators para pixeis

//confEq
#include <TLista/IListPixel.h>	//this
//#include <confeq/TOperacoesImg.h>             //Opera��es com imagens

//----------------------------------------------------------------------------
//Implementa��o dos m�todos
//Avan�a para pr�ximo ponto (ciclicamente), retorna true se ok, e false se chegou ao fim do vetor
//Ex: vetor com   numberOfObjects=5, Init faz pos=0
//Next pos=1    if(pos=1 >= numberOfObjects=5)                  retorna 1
//Next pos=2    if(pos=2 >= numberOfObjects=5)                  retorna 1
//Next pos=3    if(pos=3 >= numberOfObjects=5)          retorna 1
//Next pos=4    if(pos=4 >= numberOfObjects=5)          retorna 1
//Next pos=5    if(pos=5 >= numberOfObjects=5)          retorna 0       ok,
//pois n�o pode acessar x[5], somente x[0,1,2,3,4]
bool
IListPixel::Next ()
{
    pos++;			//Init faz pos =0, aqu� fa�o pos = 1
    if (pos >= numberOfObjects)	//numberOfObjects = 1
    {
        pos = 0;			//coloca na primeira posi��o
        return 0;			//retorna que falhou, indicando a chegada do fim do vetor
    }
    return 1;
};

//Aloca vetores
bool
IListPixel::Alocate (int32 n)
{
    numberOfObjects = (n < 1) ? 1 : n;	//Seta numero de objetos

    if (x != NULL)
        delete x;
    x = new uint32[numberOfObjects];	//Aloca os vetores
    assert (x != NULL);

    if (y != NULL)
        delete y;
    y = new uint32[numberOfObjects];
    assert (y != NULL);

    if (z != NULL)
        delete z;
    z = new uint32[numberOfObjects];
    assert (z != NULL);
    return 1;
};

/*
//2D
bool IListPixel2D::Alocate(uint32 n)
{
	if(n < 1)
   	n=1;
	numberOfObjects = n;
	if(x != NULL)
   	delete x;
	x = new uint32 [numberOfObjects];
	assert(x != NULL);//Com assert o funcao termina aqui
	if(y != NULL)     //Posteriormente implementar
   	delete y;         //sistema mais coerente
	y = new uint32 [numberOfObjects];
	assert(y != NULL);
   return 1;
};
//3D
bool IListPixel3D::Alocate(uint32 n)
{

   IListPixel2D::Alocate(n);//Chama fun��o da classe base
   if(z != NULL)
   		delete z;
	z = new uint32 [numberOfObjects];
	assert(z != NULL);
   return 1;
};
*/

ostream & operator<< (ostream & os, const IListPixel & f)
{
    for (int i = 0; i < f.GetnumberOfObjects (); i++)
    {
        os << "\nx[" << i << "]=" << f.x[i];
        os << "\ty[" << i << "]=" << f.y[i];
        os << "\tz[" << i << "]=" << f.z[i];
    }
    return os;
}


#endif
