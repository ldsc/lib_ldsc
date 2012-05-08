//=======================================================================
//PROJETO:          Biblioteca LIB_LMPT
//                  Ramo: TPadrao_ramo
//=======================================================================
//
//Desenvolvido por: Laboratorio de Meios Porosos
//                  e Propriedades Termofisicas     [LMPT].
//@author:          Andr� Duarte Bueno
//File:             TIChamber.cpp
//begin:            Fri Nov 17 2000
//copyright:        (C) 2000 by Andr� Duarte Bueno
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
@author :Andr� Duarte Bueno
@see    :
@param  :
@return :
*/

//#ifndef TIChamber_cpp
// #define TIChamber_cpp

/*
----------------------------------------------------------------------------
PROJETO:	Imago
----------------------------------------------------------------------------

Desenvolvido por: Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:    Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi,Andre D.Bueno,
		  Marcos Damiani,...
Copyright @1997:  Todos os direitos reservados.
Nome deste arquivo: TIChamber.h
Nome da classe:     TIChamber
		Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/
#include <Base/_LIB_LMPT_CLASS.h>
#include <TConEquCOILIB/TIChamber.h>	//Classe

//Objetos de convers�o de imagens e formatos
#include <TConversaoImago/TOperacoesImg.h>

//aquiaqui
COI20_CLASSID_DEF1 (TIChamber, IObject)
//TIChamber::TIChamber (T3Dint32 _size)
TIChamber::TIChamber (T3Dint32 _size, int _wb, int _ca):
        size (_size),
        wb (_wb),
        ca (_ca)
{
    //recebe a dimens�o da imagem do meio poroso
    //anula o ponteiro para a imagem prot�tipo
    imgPrototype = NULL;

    //S� desenha a regi�o solicitada, sem apagar toda a imagem
    redrawAll = false;

    //Color atributs
    //A principio setas os pixeis com 1
    cPaintBrush = 1;

    //� meio poroso
    cSolid = 0;
    //� meio poroso
    cFluid = 1;
    //� meio poroso
    cWettingFluid = 2;
    //� meio poroso
    cNowWettingFluid = 3;
    //� camara
    cChamberWettingFluid = 4;
    //� camara
    cChamberNotWettingFluid = 5;
    //� parede
    cIsolatedFluid = 6;
    //Coloquei de volta, pois tem coisa que n�o � meio poroso e n�o � camara
    cWall = 7;
    //� meio poroso
    cPorousMedia = 8;

    //Acusa a presen�a das duas camaras por default
    wettingChamber = true;
    //novo, antes tinha  calculateFluidIn, calculateFluidOut,calculateFluidIsolated
    nowWettingChamber = true;

    maxRadius = 10;
};

//Funcao chamada antes do calculo da idf no objeto TConfiguracoesEquilibrio.
//Recebe a molhabilidade do fluido que vai invadir o meio poroso.
//Retorna uma imagem binaria com 0 e 1, sendo 1 o meio Poroso [e a camara n�o molhante]
//A camara n�o molhante pode ou n�o ser desenhada, depende da molhabilidade do fluidoin.
//Usa a imagem da camara para calcular a idf. Depois a imagem da camara � deletada.
//PS: Antes, sempre desenhava a camara n�o molhante, e ao longo do processo
//de determina��o das confeq apagava a camara n�o molhante
//para impedir que o flu�do n�o molhante invadisse a camara do molhante.
I3DRawImage *
TIChamber::CreateChamber (bool molhabilidadeFluidIn)
{
    //Novo, recebe a molhabilidade e decide aqui se � para desenhar a camara n�o molhante
    //Aloca (cria) a imagem da camara
    I3DRawImage *imgChamber = GetNewImg ();
    assert (imgChamber != NULL);

    //Se molhante desenha S� o meio poroso
    if (molhabilidadeFluidIn == 1)
        DrawPorousMedia (imgChamber);
    else
        //Se n�o molhante
    {
        //Desenha o meio poroso
        DrawPorousMedia (imgChamber);
        //e adiciona a camara n�o molhante
        DrawChamberNotWettingFluid (imgChamber);
    }

    //Retorna a imagem da camara
    return imgChamber;
}

//Calcula a porosidade da imagem do  meio poroso, e o volume da regi�o v�lida
void
TIChamber::PorosityRegionPorousMedia ()
{
    //calcula o volume da regi�o porosa na imagem prototipo
    VolumeRegionPorousMedia ();

    //Obtem imagem
    I3DRawImage *imgRegionPorousMedia = GetimgPorousMedia ();

    //Verifica e depois Calcula porosidade
    assert (imgRegionPorousMedia != NULL);
    porosityRegionPorousMedia =
        TOI::GetNumberOfPixelsActived (imgRegionPorousMedia) /
        (double) GetvolumeRegionPorousMedia ();
    delete imgRegionPorousMedia;
}

/*Nome antigo ostream& TIChamber::Output (ostream& os)
ostream& operator<< (ostream& os, TIChamber& aTSCamara)
{
  os << "Presen�a camara molhante    :"<<wettingChamber<<"\n";
  os << "Presen�a camara n�o molhante:"<<nowWettingChamber<<"\n";
  os << "Dimens�o x da camara        :"<<size.x<<"\n";
  os << "Dimens�o y da camara        :"<<size.y<<"\n";
  os << "Dimens�o z da camara        :"<<size.z<<"\n";
  os << "Volume regi�o v�lida        :"<<GetvolumeRegionPorousMedia()<<"\n";
  os << "Porosidade regi�o v�lida    :"<<GetporosityRegionPorousMedia()<<"\n";
  os << "AmplifyFactor               :"<<imgPrototype->AmplifyFactor()<<"\n";
  os << "VoxelSize                   :"<<imgPrototype->VoxelSize()<<"\n";
  return os;
}
*/

//Operator<< insertion for cout
//Sa�da dos atributos do objeto chamber
ostream & operator<< (ostream & os, const TIChamber & aTSCamara)
{
    os << "\n#Presen�a camara molhante         :" << aTSCamara.
    wettingChamber << "\n";
    os << "#Presen�a camara n�o molhante     :" << aTSCamara.
    nowWettingChamber << "\n";
    os << "#Dimens�o x da camara             :" << aTSCamara.size.x << "\n";
    os << "#Dimens�o y da camara             :" << aTSCamara.size.y << "\n";
    os << "#Dimens�o z da camara             :" << aTSCamara.size.z << "\n";
    os << "#Volume regi�o v�lida             :" << aTSCamara.
    GetvolumeRegionPorousMedia () << "\n";
    os << "#Volume regi�o v�lida direto      :" << aTSCamara.
    volumeRegionPorousMedia << "\n";
    os << "#Porosidade regi�o v�lida         :" << aTSCamara.
    GetporosityRegionPorousMedia () << "\n";
    os << "#Porosidade regi�o v�lida  direto :" << aTSCamara.
    porosityRegionPorousMedia << "\n";
    os << "#AmplifyFactor                    :" << aTSCamara.imgPrototype->
    AmplifyFactor () << "\n";
    os << "#VoxelSize                        :" << aTSCamara.imgPrototype->
    VoxelSize () << "\n";
    return os;
}

//Operator<< insertion for cout
//Sa�da dos atributos do objeto chamber
/*ostream& operator<< (ostream& os, TIChamber& aTSCamara)
{
  //os << "Atributos do objeto  - Class TSCamara" << "\n";
  os << "\nDados objeto chamber\n";
  os << "redrawAll                  :"	 << aTSCamara.redrawAll<<"\n";
  os << "cPaintBrush                :"	 <<aTSCamara.cPaintBrush<<"\n";

  os << "cSolid                     :"	 <<aTSCamara.cSolid<<"\n";
  os << "cFluid                     :"	 <<aTSCamara.cFluid<<"\n";
  os << "cWettingFluid              :"	 <<aTSCamara.cWettingFluid<<"\n";
  os << "cNowWettingFluid           :"	 <<aTSCamara.cNowWettingFluid<<"\n";
  os << "cChamberWettingFluid       :"	 <<aTSCamara.cChamberWettingFluid<<"\n";
  os << "cChamberNotWettingFluid    :"	 <<aTSCamara.cChamberNotWettingFluid<<"\n";
  os << "cIsolatedFluid             :"	 <<aTSCamara.cIsolatedFluid<<"\n";
  os << "cPorousMedia               :"	 <<aTSCamara.cPorousMedia<<"\n";

  os << "wettingChamber             :"	 <<aTSCamara.wettingChamber<<"\n";
  os << "nowWettingChamber          :"	 <<aTSCamara.nowWettingChamber<<"\n";
  os << "size.x                     :"	 <<aTSCamara.size.x<<"\n";
  os << "size.y                     :"	 <<aTSCamara.size.y<<"\n";
  os << "size.z                     :"	 <<aTSCamara.size.z<<"\n";

  os << "\nDados objeto imagemPrototype no arquivo imagemPrototype.txt\n";
  TOI::SaveImg8bpp(aTSCamara.imgPrototype  , "imagemPrototype.txt");//ok
  return os;
}    */

//Operator>> extraction for cin
//Entrada dos atributos do objeto chamber
istream & operator>> (istream & is, TIChamber & aTSCamara)
{
    is >> aTSCamara.redrawAll;
    is >> aTSCamara.cPaintBrush;

    is >> aTSCamara.cSolid;
    is >> aTSCamara.cFluid;
    is >> aTSCamara.cWettingFluid;
    is >> aTSCamara.cNowWettingFluid;
    is >> aTSCamara.cChamberWettingFluid;
    is >> aTSCamara.cChamberNotWettingFluid;
    is >> aTSCamara.cIsolatedFluid;
    is >> aTSCamara.cPorousMedia;

    is >> aTSCamara.wettingChamber;
    is >> aTSCamara.nowWettingChamber;
    is >> aTSCamara.size.x;
    is >> aTSCamara.size.y;
    is >> aTSCamara.size.z;
    return is;
}

//Cria e retorna uma lista de p�xeis para a camara n�o molhante
IListPixelRotuled *
TIChamber::CreateListPixelNWChamber ()
{
    //Cria uma imagem nova, com a camara de flu�do n�o molhante
    I3DRawImage *img = GetimgChamberNotWettingFluid ();

    //Cria objeto lista de p�xeis
    IListPixelRotuled *lpixel = new IListPixelRotuled ();
    assert (lpixel != NULL);
    //Determina a lista de p�xeis
    lpixel->Go (img);

    delete img;

    //Retorna a lista de p�xeis
    return lpixel;
}

//Cria e retorna uma lista de p�xeis para a camara molhante
IListPixelRotuled *
TIChamber::CreateListPixelWChamber ()
{
    I3DRawImage *img = GetimgChamberWettingFluid ();
    IListPixelRotuled *lpixel = new IListPixelRotuled ();
    assert (lpixel != NULL);
    lpixel->Go (img);
    delete img;
    return lpixel;
}

//#endif
