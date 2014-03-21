#ifndef TIChamber_h
#define TIChamber_h
/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: |CLASSNAME|...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     André Duarte Bueno
@file       TIChamber.h
@begin      Fri Nov 17 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------
#include <iostream>
#include <string>

//COI-LIB
//Defini��es dos tipos uint16 int32
#include <coi20/sys/types/primitive.h>

//Imago
//aquiaqui
//#include <imago/img3d/rawimg3d.h>
#include <imago/img3d/image/raw.h>

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
#ifndef _LIB_LMPT_CLASS_h
#include <Base/_LIB_LMPT_CLASS.h>
#endif

#ifndef IListPixelRotuled_h
#include <TLista/IListPixelRotuled.h>
#endif


/*
=========================================================================
Documentacao Classe: TIChamber
=========================================================================
Assunto:        TIChamber.
Superclasse:    a propria
Descri��o:
Declara a classe TIChamber.
		  Representa uma camara onde "experimentos"  podem ser realizados.
                       Esta classe representa unicamente a etapa de desenho da camara,
                       ou seja a constru��o da camara em s�. Ou o redesenho de partes da camara.
                       A simulac�o dos "processos" � realizada atrav�s do objeto
                       de determinação das configuracoes de equil�brio.

                       A constru��o da camara passa pela defini��o das regi�es
                       onde est�o o meio poroso, as paredes, e as camaras dos flu�dos molhante e n�o molhante.

                       A camara pode ser simples, ou complexa.
                       Uma camara simples � formada a partir de um cubo 3*3*3,
                       onde cada p�xel deste cubo tem um �ndice de cor que representa:
                       um flu�do molhante,um flu�do n�o molhante, o meio poroso,....
                       Ou seja: A imagem da camara � representada por uma imagem reduzida.
                       Uma imagem prototipo.

                       Esta classe (TIChamber) representa o conceito de camara genérica.
                       A classe TSChamber representa uma camara simples
                       A classe TCChamber representa uma camara complexa
                       As classes TSCMercuryInjection2F ,TSCMercuryInjection4F,
                       TSCMercuryInjection6F, TSCImbibition ,TSCDraynage.
                       representam camaras simples previamente desenhadas, s�o filhas
                       da TSChamber.

                       As dimensoes da camara ser�o as mesmas da imagem do meio poroso, e
                       serao armazenadas no atributo size.

                       Esbo�o da imagem prototipo da camara simples (3*3*3):
                       		front					center				back
                       --------------     ---------------   -------------
                       |  |      |  |     |  |       |  |   | |        | |
                       --------------     ---------------   -------------
                       |  |      |  |     |  |       |  |   | |        | |
                       --------------     ---------------   -------------
                       |  |      |  |     |  |       |  |   | |        | |
                       --------------     ---------------   -------------

                       A dimensaoBase (baseDimension), � usada para separar na imagem final, as regi�es
                       ilustradas no esbo�o acima.
                       A dimensaoBase � sempre menor que a menor dimensao da imagem do meio poroso/3.
                       |  |->dimensaoBase  (equivale ao raioMaximo)

Obs:
                        1-
								A vantagem de se criar a camara desta forma � poder desenhar
                        diferentes casos de uma maneira extremamente simples.
                        A forma usada por Magnani, deixa o sistema travado, a altera��o
                        do tipo de camara implica num processo de reconstru��o e
                        recompila��o do programa para cada tipo de simula��o, deve-se
                        ressaltar que este modelo se justificava naquela �poca.
                        A TSChamber permite a constru��o da camara de uma maneira simplificada.

                        2-A TCChamber permite a constru��o de uma camara que pode
                        ser efetivamente desenhada pelo usuário.
                        Com a camara complexa o usuário poder� construir qualquer tipo
                        de camara, podendo simular praticamente qualquer tipo de processo
                        que envolva a determinação das configura��es de equil�brio.
                        Inclusive com mais de um tipo de meio poroso, interagindo.
                        -Obs: Para que se possa usar mais de um tipo de meio poroso,
                        deve-se dispor de um conjunto de imagens 3D com as mesmas
                        dimensoes. A camara dever� receber uma imagem final que � a combina��o
                        das diversas imagens de meios porosos.

                        3-TConfigura��es de Equilibrio:
                        Para se resolver o problema de desenho da membrana, adotou-se
                        um procedimemto extremamente simples, que reduz a complexidade
                        do sistema, e aumenta sua clareza.
                        Elimina-se o uso das membranas desenhando-se as camaras com
                        flu�do molhante como se fossem s�lidos (inativa), desta forma as mesmas
                        n�o s�o consideradas no processo de cálculo da IDF (abertura).
                        Mas para efeito da verifica��o das conectividades desenha-se
                        as camaras com flu�do molhante como ativas.

                        4-
                        Depois de desenhada a camara, o meio poroso � �nico, ou seja,
                        Atrav�s do quadro de di�logo, o usuário pode desenhar a camara,
                        com diversos tipos de meios porosos, mas depois, � gerado um
                        meio poroso �nico, que aglutina todos os meios porosos.

                        Deve ficar claro que a imgPrototipo � uma img 8bpp em que cada cor representa
                        uma regi�o.
                        As imagens a serem desenhadas s�o imagens bin�rias, ou seja, recebe img binaria
                        desenha sobre esta imagem a regi�o solicitada.

                        RESUMINDO:
                        -A TIChamber TEM UMA IMAGEM PROTOTIPO, 8BPP EM QUE CADA PIXEL, INDICA
                        PELA SUA COR O TIPO DE REGI�O.

                        -PARA A TISChamber //tipo camara SIMPLES
                        A IMAGEM PROTOTIPO TEM A DIMENSAO 3*3*3
                        -PARA A TICChamber //tipo camara COMPLEXA
                        A IMAGEM PROTOTIPO TEM AS MESMAS DIMENSOES DA IMAGEM DO MEIO POROSO


 //Implementa��o:
 //Deve-se armazenar a informa��o da imagem do meio poroso na camara, desta forma
 //a mesma pode ser obtida com camara->GetimgMeioPoroso, se for uma TSChamber, vai
 //ter um ponteiro e o controle da imagem do meio poroso passado, se for uma TCChamber
 //pode deletar a imagem do meio poroso, a mesma passa a ser fornecida pela imgPrototype.

@short		Breve descri��o
@author 	André Duarte Bueno
@version
@see		Veja assunto
*/


//O QUADRO DE DI�LOGO:
//Deve receber as entradas do usuário
//Deve preencher a imagem prototipo 8bpp, definindo as regioes da camara
//onde os indices de cor indicam o tipo de objeto.
//Compoe as diversas imagens de meios porosos em uma �nica.
//Assim a saida do quadro de di�logo � a camara com a imagem prototipo definida
//e um meio poroso que reune os demais meios porosos

//Representa uma imagem Virtual
/*
class TIVImage
{
  //Dimensoes da imagem
  T3Dint32 size;
  //Imagem representativa
  I3DRawImage* imgPrototype;
  //pixel iterator para a imagem prototipo
  I3DPixelIterator8* piimgPrototype ;
};
*/

class TIChamber:public IObject
{
    COI20_CLASSID COI20_SAFECAST (TIChamber, IObject)
//--------------------------------------------------------------Atributos
private:
protected:
    //---------------------------------
    //Imagem do prototipo da camara
    //Dimensoes da imagem reconstruida
    T3Dint32 size;

    //Imagem 3*3*3 ou nx*ny*nz com atributos da imagem da camara
    I3DRawImage *imgPrototype;

    //Atributos dos flu�dos das camaras
    //Molhabilidade do fluido b
    int wb;
    //Compressibilidade do fluido a
    int ca;

    //Volume e porosidade da regi�o v�lida (regi�o com meio poroso)
    //Volume da regi�o meio poroso
    uint32 volumeRegionPorousMedia;
    //Porosidade medida somente na regi�o do meio poroso
    double porosityRegionPorousMedia;

    //flag de desenho, por default=false
    //s� desenha pixeis ativos
    //Para desenhar toda a imagem p�xeis ativos e inativos fazer redrawAll = true
    bool redrawAll;



    //---------------------------------
    //Indices de cor, cada tipo de objeto a ser desenhado deve ter uma cor associada
    //ou seja, cada pixel da imagem prototipo deve ter uma cor espec�fica
    //Existem duas fazes, a s�lida e a flu�dica.
    //A cor cPorousMedia indica que a cor efetiva � aquela da imagem do meio poroso, podendo ser 0 ou 1
    //PS: antes as cores eram uint32
    //////////////////////
    uint8 cSolid;			//Regi�o Solida  (0)
    //////////////////////
    //Regi�es Flu�dicas
    //////////////////////
    uint8 cFluid;			//Fluido qualquer (1)
    uint8 cWettingFluid;		//Fluido molhante
    uint8 cNowWettingFluid;	//Fluido nao molhante
    uint8 cChamberWettingFluid;	//Camara com fluido molhante
    uint8 cChamberNotWettingFluid;	//Camara com fluido nao molhante
    uint8 cIsolatedFluid;		//Fluido isolado
    uint8 cWall;
    //////////////////////
    uint8 cPorousMedia;		//Meio Poroso, Com esta cor, vale a cor da imagem meio poroso
    //ou seja pode ser 0 ou 1.

    uint8 cPaintBrush;		//cor do pincel, default=1, mas pode ser invertido
    //sendo usado para apagar o objeto solicitado

    //flags de exist�ncia das camaras molhante e n�o molhante
    bool wettingChamber;		//se ativo existe a camara molhante             //novo
    bool nowWettingChamber;	//se ativo existe a camara n�o molhante //novo

    uint32 maxRadius;		//Novo, agora calcula raio m�ximo no construtor da chamber

public:
//-------------------------------------------------------------Construtor
    //TIChamber (T3Dint32 _size);                 //Recebe as dimensoes da imagem
    //Recebe as dimensoes da imagem, wb=0 e ca=1 (intrus�o merc�rio � default)
    TIChamber (T3Dint32 _size, int _wb = 0, int _ca = 1);


//--------------------------------------------------------------Destrutor
    virtual ~ TIChamber ()
    {
        if (imgPrototype)
            delete imgPrototype;
    };

//----------------------------------------------------------------M�todos
private:
protected:
    //Desativada
    //virtual ostream& Output (ostream& os);

    //Percorre a imagem prototipo e compara cada pixel com o indice de cor passado,
    //se for igual marca na imagem binaria passada
    virtual I3DRawImage * Draw (I3DRawImage * imgBinaria, uint8 indiceCor) = 0;

public:

    //Funcoes virtuais
    //Desenha na imagem passada o meio poroso, a cor efetiva vai ser aquela da imagem do meio poroso
    //para a camara simples, ou da imagem prototipo para camara complexa
    //virtual void DrawPM( I3DRawImage* imgBinaria ) = 0;
    virtual I3DRawImage * DrawPorousMedia (I3DRawImage * imgBinaria) = 0;

    //---------------------------------
    //Fun��es de desenho
    //---------------------------------
    //Recebem uma imagem binaria e desenham sobre a mesma
    //O flag drawAll � por default = false
    //para desenhar toda a imagem fazer o flag = true com SetdrawAll(true);
    //PS: Agora retornam uma imagem, e n�o mais void
    //Desenha regi�o s�lido
    inline I3DRawImage *DrawSolid (I3DRawImage * _imgBinaria)
    {
        return Draw (_imgBinaria, cSolid);
    };

    //Desenha regi�o fluido
    inline I3DRawImage *DrawFluid (I3DRawImage * _imgBinaria)
    {
        return Draw (_imgBinaria, cFluid);
    };

    //Desenha camaraFluidoMolhante
    inline I3DRawImage *DrawChamberWettingFluid (I3DRawImage * _imgBinaria)
    {
        return Draw (_imgBinaria, cChamberWettingFluid);
    };

    //Desenha ChamberFluidoNaoMolhante
    inline I3DRawImage *DrawChamberNotWettingFluid (I3DRawImage * _imgBinaria)
    {
        return Draw (_imgBinaria, cChamberNotWettingFluid);
    };

    //Desenha fluido molhante
    inline I3DRawImage *DrawWettingFluid (I3DRawImage * _imgBinaria)
    {
        return Draw (_imgBinaria, cWettingFluid);
    };

    //Desenha fluido nao molhante
    inline I3DRawImage *DrawNowWettingFluid (I3DRawImage * _imgBinaria)
    {
        return Draw (_imgBinaria, cNowWettingFluid);
    };

    //Desenha fluido isolado
    inline I3DRawImage *DrawIsolatedFluid (I3DRawImage * _imgBinaria)
    {
        return Draw (_imgBinaria, cIsolatedFluid);
    };

    //Desenha as paredes
    inline I3DRawImage *DrawWall (I3DRawImage * _imgBinaria)
    {
        return Draw (_imgBinaria, cWall);
    };



    void SetredrawAll (bool _d)
    {
        redrawAll = _d;
    };
    void SetcPaintBrush (uint c)
    {
        cPaintBrush = c;
    };
    void Setca (int _ca)
    {
        ca = _ca;
    };
    void Setwb (int _wb)
    {
        ca = _wb;
    };

    //Fun��es de c�lculo de propriedades internas
    //Determina o volume v�lido do meio poroso na camara
    virtual void VolumeRegionPorousMedia () = 0;

    //Determina a porosidade da regi�o v�lida
    void PorosityRegionPorousMedia ();

    //Cria e retorna uma imagem nova binaria,
    //com o desenho da regi�o meio poroso + desenho da camara com fluidoIn se for n�o molhante (ex:merc�rio)
    I3DRawImage *CreateChamber (bool molhabilidadeFluidIn);

    //Funcoes novas, de uso geral.
    //Retorna uma imagem nova, com o tamanho de size, e os atributos da img prototipo
    inline I3DRawImage *GetNewImg ()
    {
        I3DRawImage *img = new I3DRawImage (size, 1);
        assert (img != NULL);
        img->ClearBits ();
        //Seta atributos
        img->SetVoxelSize (imgPrototype->VoxelSize ());
        //Seta atributos
        img->SetAmplifyFactor (imgPrototype->AmplifyFactor ());
        return img;
    }

    //Retorna uma imagem do que foi marcado como sendo meio poroso
    //novo, esta fun��o �ra repetida nas classes herdeiras, puxei para c�, chama a fun��o virtual DrawPorousMedia
    virtual I3DRawImage *GetimgPorousMedia ()
    {
        //Cria imagem nova
        I3DRawImage *img = GetNewImg ();
        //Desenha o meio Poroso
        DrawPorousMedia (img);
        //retorna ponteiro para imagem nova criada
        return img;
    };


    //Cria e retorna uma lista de pixeis para pontos da camara nao molhante
    IListPixelRotuled *CreateListPixelNWChamber ();

    //Cria e retorna uma lista de pixeis para pontos da camara molhante
    IListPixelRotuled *CreateListPixelWChamber ();

    //Dada uma imagem, retorna uma imagem descascada
    virtual I3DRawImage *GetImgNucleo (I3DRawImage * imgDados,
                                       unsigned int borderDiscount)
    {
        return GetNewImg ();
    };


//-------------------------------------------------------------Sobrecarga

//--------------------------------------------------------------------Get
    //Fun��es que retornam uma imagem bin�ria do objeto solicitado
    inline I3DRawImage *GetimgSolid ()
    {
        return DrawSolid (GetNewImg ());
    };
    inline I3DRawImage *GetimgFluid ()
    {
        return DrawFluid (GetNewImg ());
    };
    inline I3DRawImage *GetimgChamberWettingFluid ()
    {
        return DrawChamberWettingFluid (GetNewImg ());
    };
    inline I3DRawImage *GetimgChamberNotWettingFluid ()
    {
        return DrawChamberNotWettingFluid (GetNewImg ());
    };
    inline I3DRawImage *GetimgWettingFluid ()
    {
        return DrawWettingFluid (GetNewImg ());
    };
    inline I3DRawImage *GetimgNowWettingFluid ()
    {
        return DrawNowWettingFluid (GetNewImg ());
    };
    inline I3DRawImage *GetimgIsolatedFluid ()
    {
        return DrawIsolatedFluid (GetNewImg ());
    };
    inline I3DRawImage *GetimgWall ()
    {
        return DrawWall (GetNewImg ());
    };

    //Retornam a informa��o da presen�a da camara molhante ou n�o molhante
    bool GetwettingChamber () const
    {
        return wettingChamber;
    };
    bool GetnowWettingChamber () const
    {
        return nowWettingChamber;
    };

    //Funcoes Set Get
    //Retorna copia do ponteiro para imagem prototipo
    I3DRawImage *GetimgPrototypeptr () const
    {
        return imgPrototype;
    };
    bool GetredrawAll () const
    {
        return redrawAll;
    };
    uint8 GetcPaintBrush () const
    {
        return cPaintBrush;
    };

    //Retorna dimensoes
    T3Dint32 Getsize () const
    {
        return size;
    };

    //Retorna volume da regi�o com meio poroso
    uint32 GetvolumeRegionPorousMedia () const
    {
        return volumeRegionPorousMedia;
    };

    //Retorna porosidade da regi�o porosa
    double GetporosityRegionPorousMedia () const
    {
        return porosityRegionPorousMedia;
    };

    //Retorna o raio M�ximo
    uint32 GetmaxRadius () const
    {
        return maxRadius;
    };

    //Retorna compressibilidade de a e molhabilidade de b
    int Getca () const
    {
        return ca;
    };
    int Getwb () const
    {
        return wb;
    };

    virtual std::string GetClassName () const
    {
        return std::string ("TIChamber");
    };

    //O default � tipo 1, intrus�o de merc�rio = 1
    virtual int Type () const
    {
        return 1;
    };


//--------------------------------------------------------------------Set
//-----------------------------------------------------------------Friend
    //Sobrecarga iostreans (entrada e saida dos atributos  da Chamber)
    friend ostream & operator<< (ostream & os, const TIChamber & aTIChamber);
    friend istream & operator>> (istream & is, TIChamber & aTIChamber);

};

//-----------------------------------------------------------------Friend
//Declara��o de Fun��es Friend
ostream & operator<< (ostream & os, const TIChamber & aTIChamber);
istream & operator>> (istream & is, TIChamber & aTIChamber);
#endif
