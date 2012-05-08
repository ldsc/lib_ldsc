#ifndef TFEConectividade3D_h
#define TFEConectividade3D_h

/*
  =========================================================================
  PROJETO:    Biblioteca LIB_LMPT
  Assunto/Ramo: |TFEConectividade3D|...
  =========================================================================
  Desenvolvido por:
  Laboratorio de Meios Porosos e Propriedades Termofisicas
  [LMPT].
  @author     Andr� Duarte Bueno
  @file       TFEConectividade3d.h
  @begin      Fri Nov 17 2000
  @copyright  (C) 2000 by Andr� Duarte Bueno
  @email      andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------
//#include <imago/img3d/rawimg3d.h>
#include <imago/img3d/image/raw.h>

//filtro rotulagem
//aquiaqui #include <imago/flt3d/flt3dlb.h>
#include <imago/flt3d/core/label.h>

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
#ifndef _LIB_LMPT_CLASS_h
#include <Base/_LIB_LMPT_CLASS.h>
#endif

#ifndef IListPixel_h
#include <TLista/IListPixel.h>
#endif

/*
  =========================================================================
  Documentacao CLASSE: TFEConectividade3D
  =========================================================================
  Assunto:               TFEConectividade3D.
  Superclasse:           a propria
  Descri��o:             Declara a classe TFEConectividade3D.
  Tem a fun��o  VerifyConectivit, que verifica a conex�o de v�rios
  objetos em uma imagem.
  Recebe uma imagem rotulada (imagem que se deseja verificar a conectividade)
  uma lista de pixeis a serem verificados e uma imagem a ser preenchida.
  Apanha o rotulo de todos os pixeis da lista de pixeis,
  depois percorre a imagem rotulo e seta como ativos
  todos os pixeis que tem rotulos iguais ao da lista.
  Ou seja, a imagem retornada � uma imagem com a uni�o de todos
  os objetos que tem os r�tulos dos p�xeis da lista de p�xeis.
  @short		Breve descri��o
  @author 	Andr� Duarte Bueno
  @version
  @see		Veja assunto
*/
class TFEConectividade3D
{
    //--------------------------------------------------------------Atributos
private:
protected:
public:
    //-------------------------------------------------------------Construtor
    TFEConectividade3D ()
    {
    };

    //--------------------------------------------------------------Destrutor
    virtual ~ TFEConectividade3D ()
    {
    };

    //----------------------------------------------------------------M�todos
private:
protected:
public:
    //Fun��o que retorna na imgBinaria as regi�es conexas
    bool VerifyConectivit		//Verifica a conectividade da lista de p�xeis
    (I3DRawImage * imgRotulo,	//Imagem rotulada 32bpp
     IListPixel * lPixel,	//Lista dos pixeis que participam da regiao conexa
     I3DRawImage * imgBinaria	//Imagem a ser marcada com a regiao conexa
    );

    //Fun��o nova, recebe uma imagem binaria, e retorna a mesma conectada em 3D
    //na dire��o Z
    //  bool GetConectedImgZ(I3DRawImage* imgBinaria); INCOMPLETA


    /*
      bool VerifyConectivit             //Verifica a conectividade da lista de p�xeis
      (
      IListPixel * lPixel,              //Lista dos pixeis que participam da regiao conexa
      I3DRawImage* imgBinaria           //Imagem a ser marcada com a regiao conexa
      )
      {
      if(imgBinaria->BPP!=1)
      return 0;
      I3DFltLabel frotulagem;
      frotulagem.Go(imgBinaria);
      I3DRawImage* imgRotulo = frotulagem.Image();
      assert(imgRotulo != NULL);
      bool result = TFEConectividade3D::VerifyConectivit(imgRotulo, lPixel , imgBinaria);
      return result;
      }
    */
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
