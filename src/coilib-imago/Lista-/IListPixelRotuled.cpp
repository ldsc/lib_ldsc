//=======================================================================
//PROJETO:          Biblioteca LIB_LMPT
//                  Ramo: TPadrao_ramo
//=======================================================================
//
//Desenvolvido por:     Laboratorio de Meios Porosos
//                     e Propriedades Termofisicas     [LMPT].
//@author:          André Duarte Bueno
//File:             IListPixelRotuled.cpp
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

#ifndef IListPixelRotuled_cpp
#define IListPixelRotuled_cpp
/*
  ----------------------------------------------------------------------------
  PROJETO:					Imago
  ----------------------------------------------------------------------------

  Desenvolvido por:		  Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
  Programadores:   		  Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi,
  Andre D.Bueno,Luiz Orlando Emerich, Marcos Damiani,...
  Copyright @1997-1999:  Todos os direitos reservados.
  Nome deste arquivo:	  IListPixelRotuled.cpp
  Nome da classe:        IListPixelRotuled
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  ----------------------------------------------------------------------------
*/

//C++
//#include <fstream.h>
#include <cassert>

//Imago
//#include <imago/img3d/pixit3d1.h>
//#include <imago/img3d/pixit3d32.h>
#include <imago/img3d/pixit/pixit1.h>
#include <imago/img3d/pixit/pixit32.h>

//#include <imago/flt3d/flt3dlb.h>
#include <imago/flt3d/core/label.h>

//confEq
#include <TLista/IListPixelRotuled.h>
#include <TConversaoImago/TOperacoesImg.h>

//O objetivo da fun��o Go � receber uma imagem bin�ria, a partir dela criar uma imagem rotulada,
//Determinar o n�mero de objetos da imagem rotulada, alocar os vetores internos, e ent�o
//varrer a imagem rotulada e determinar um p�xel para cada objeto.
bool
IListPixelRotuled::Go (I3DRawImage * &imgBinaria)
{
    if (imgBinaria->BPP () != 1)	//Testa se a imagem � bin�ria
    {
        assert (0);
        return 0;
    }

    //2D==3D
    //Rotula o objeto de interesse
    I3DFltLabel frotulagem;	//Cria objeto rotulagem
    frotulagem.Go (imgBinaria);	//Rotula a imagem binaria
    I3DRawImage *imgRotulo = frotulagem.Image ();	//Obtem a imagem rotulada de 32 bpp
    assert (imgRotulo != NULL);	//Testa

    //Determina o maior rotulo da imagem
    //Ou seja o numero de objetos vai ser igual ao valor do maior r�tulo
    //ou seja desconsidera-se o fundo (que tem r�tulo 0)
    numberOfObjects = TOperacoesImg::GetMaxValueFrom32bppImage (imgRotulo);
    //se s� tem fundo ou Alocate retornou false
    //2D!=3D
    if (numberOfObjects == 0 || Alocate (numberOfObjects) == false)
    {
        delete imgRotulo;		//Destr�e imagem r�tulo

        return 0;			//Retorna false

    }
    else				//Se alocou corretamente e o numero objetos != 0
    {
        //Obtem os pontos que identificam o objeto, na imagem rotulada
        uint32 rotulo;		//R�tulo do p�xel atual na imagem rotulo
        //Percorre toda a imagem rotulo
        I3DPixelIterator *piimgRotulo =
            I3DPixelIterator32::SafeCast (imgRotulo->InitPixelIterator ());
        for (int i = 0; i < imgRotulo->Width (); i++)
            for (int j = 0; j < imgRotulo->Height (); j++)
                for (int k = 0; k < imgRotulo->Depth (); k++)
                {
                    rotulo = piimgRotulo->Get (i, j, k);	//Obt�m rotulo da imagem r�tulo
                    if (rotulo != 0)	//se n�o for o fundo
                    {		//Anota a posi��o
                        x[rotulo - 1] = i;	//
                        y[rotulo - 1] = j;	//Ex: 7 objetos, rotulos 1,2,3,4,5,6,7, posi��es 0,1,2,3,4,5,6
                        z[rotulo - 1] = k;	//objeto com r�tulo 1 na posi��o 0 (o fundo n�o entra)
                    }
                }
        delete piimgRotulo;	//novo
        delete imgRotulo;		//Destr�e a imagem rotulo
        return 1;			//retorna a lista de p�xeis
    }
};
#endif
