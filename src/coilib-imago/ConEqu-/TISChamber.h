#ifndef TISChamber_h
#define TISChamber_h


/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: |CLASSNAME|...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     Andr� Duarte Bueno
@file       TISChamber.h
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
Documentacao Classe: TISChamber
=========================================================================
Assunto:        TISChamber
Superclasse:    TISChamber
Descri��o:			Declara a classe TISChamber.
                Para a TISChamber, a imagem prot�tipo � uma pequena imagem
                que representa a imagem toda da camara
@short					Breve descri��o
@author 				Andr� Duarte Bueno
@version
@see						Veja assunto
*/

#ifndef _LIB_LMPT_CLASS_h
#include <Base/_LIB_LMPT_CLASS.h>
#endif


//objeto camara (classe pai)
#ifndef 	 TIChamber_h
#include <TConEquCOILIB/TIChamber.h>
#endif

//Funcoes gen�ricas manipula��o de imagens
//#include <TConversaoImago/TOperacoesImg.h>

//A imgPrototipo � uma imagem (a principio 3*3*3) com indices de cor a serem aplicados
//a imagem da camara. � uma imagem 8bpp = 255 niveis de cores
//Desta forma pode-se armazenar a informa��o da camara de uma maneira compacta.
//O atributo dimensaoBase � usado para defini��o da dimensao (raioMaximo) que divide a imagem em 3 partes.
//       |0            |--->dimensaoBase              |dimensaoBase-Width()          width()-1|
//Os indices de cor s�o usados para defini��o dos tipos de objetos a serem pintados.
//----------------------------------------------------------------------------

class TISChamber:public TIChamber
{
//--------------------------------------------------------------Atributos
COI20_CLASSID COI20_SAFECAST (TISChamber, IObject) private:
protected:
    /*
      Criar objeto gen�rico de cordenadas
      uint32 baseDimension_n;      	//norte
      uint32 baseDimension_s;      	//sul
      uint32 baseDimension_o;		 		//oeste
      uint32 baseDimension_l;      	//leste
      uint32 baseDimension_f;      	//front
      uint32 baseDimension_b;      	//back
    */
    //Ponteiro para um copia da Imagem do meio poroso
    I3DRawImage * imgPorousMedia;

    //Dimensao base, di�metro da maior bola
    uint32 baseDimension;
public:

//-------------------------------------------------------------Construtor
    //Construtor, tem de criar a imagem prototipo
    TISChamber (I3DRawImage * _imgPorousMedia, int _wb = 0, int _ca = 1);

//--------------------------------------------------------------Destrutor
    virtual ~ TISChamber ()
    {
        if (imgPorousMedia)
            delete imgPorousMedia;
    };

//----------------------------------------------------------------M�todos
private:
    inline void DrawBox (I3DRawImage * imgBinaria, uint32 _i, uint32 _j,
                         uint32 _k);

protected:

    //Desenha sobre a imagem binaria, o objeto com o indiceDeCor passado
    //O objeto � definido pela cor na imagem prototipo
    virtual I3DRawImage * Draw (I3DRawImage * imgBinaria, uint8 indiceCor);

public:

    //Desenha sobre a imagem binaria a imgMeioPoroso, verificando se � meio poroso na imagem prototipo
    virtual I3DRawImage * DrawPorousMedia (I3DRawImage * imgBinaria);

    //Retorna uma copia da imagem do meio poroso, verificando o que � meio poroso na imagem prototipo
    /*  virtual I3DRawImage* TISChamber::GetimgPorousMedia()
       {
       //Cria imagem nova
       I3DRawImage*img =  GetNewImg();
       //Desenha o meio Poroso
       DrawPorousMedia(img);
       //retorna ponteiro para imagem nova criada
       return img;
       };   */

    //Calculo do volume da regi�o ocupada pelo meio poroso
    virtual void VolumeRegionPorousMedia ();

    //Retorna uma imagem nova, que cont�m somente o n�cleo da imagem
    //A defini��o do tamanho do n�cleo da imagem � feita pelas fun��es Getxi,...
    // e pelo parametro borderDiscount
    //Recebe uma imagem completa e preenchida (a imgDados)
    //Retorna uma imagem do centro da imagem imgDados,
    //levando em conta as posi��es definidas pelos par�metros xi,yi,zi e  xf,yf,zf, e pelo borderDiscount.
    //Fun��o nova, retorna o n�cleo da imagem passada, ou seja, recebe imgDados e retorna o seu n�cleo.
    //O borderDiscount � usado para cortar as bordas do n�cleo

//-------------------------------------------------------------Sobrecarga
//--------------------------------------------------------------------Get
    //o i indica o bloco.
    inline uint32 Getxi (uint32 _i);
    inline uint32 Getxf (uint32 _i);
    inline uint32 Getyi (uint32 _i);
    inline uint32 Getyf (uint32 _i);
    inline uint32 Getzi (uint32 _i);
    inline uint32 Getzf (uint32 _i);

    virtual I3DRawImage *GetImgNucleo (I3DRawImage * imgDados,
                                       unsigned int borderDiscount);

    //retorna o nome da classe
    virtual std::string GetClassName () const
    {
        return std::string ("TISChamber");
    };

//--------------------------------------------------------------------Set
//-----------------------------------------------------------------Friend
    //friend ostream& operator<< (ostream& os, const |CLASSNAME|& obj);
    //friend istream& operator>> (istream& is, |CLASSNAME|& obj);
};

//-----------------------------------------------------------------Friend
//Declara��o de Fun��es Friend
//ostream& operator<< (ostream& os, const |CLASSNAME|& obj);
//istream& operator>> (istream& is, |CLASSNAME|& obj);


#endif
