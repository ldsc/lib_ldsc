#ifndef TOperacoesImg_h
#define TOperacoesImg_h

/*
=========================================================================
PROJETO:    Biblioteca LIB_LMPT
            Assunto/Ramo: |CLASSNAME|...
=========================================================================
Desenvolvido por:
            Laboratorio de Meios Porosos e Propriedades Termofisicas
            [LMPT].
@author     Andr� Duarte Bueno
@file       TOperacoesImg.h
@begin      Fri Nov 17 2000
@copyright  (C) 2000 by Andr� Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//Bibliotecas Coi-lib
//-----------------------------------------------------------------------
#include <coi20/sys/core/system.h>
#include <imago/img3d/image/raw.h>
//#include <img3d/rawimg3d.h>                                   //I3DRawImage

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
#ifndef _LIB_LMPT_CLASS_h
#include <Base/_LIB_LMPT_CLASS.h>
#endif

#ifndef TRotulador3D_h
#include <TRotulador/TRotulador3D.h>
#endif

/*
=========================================================================
Documentacao Classe: TOperacoesImg
=========================================================================
Assunto:        Interface com  COILIB e IMAGO
Superclasse:    TOperacoesImg
@short		Classe criada para fornecer funcoes simples de alteracao
              	de imagens da COI-LIB
@author 	Andr� Duarte Bueno
@version
@see
*/
class TOperacoesImg
{
//--------------------------------------------------------------Atributos
private:
protected:
public:

//-------------------------------------------------------------Construtor
//--------------------------------------------------------------Destrutor
//----------------------------------------------------------------M�todos
private:
protected:
public:

    //---------------------------------------------------------------------
    //INTERFACE DISCO
    //---------------------------------------------------------------------
    //Salva imagem em disco
    static bool SaveImg (I3DRawImage * img, std::string nomeArquivo);
    static bool SaveImgBinaria (I3DRawImage * imgBinaria, std::string nomeArquivo)
    {
        return SaveImg1bpp (imgBinaria, nomeArquivo);
    };
    static bool SaveImg1bpp (I3DRawImage * imgBinaria, std::string nomeArquivo);
    static bool SaveImg8bpp (I3DRawImage * img8bpp, std::string nomeArquivo);
    static bool SaveImg16bpp (I3DRawImage * img16bpp, std::string nomeArquivo);
    static bool SaveImg32bpp (I3DRawImage * img32bpp, std::string nomeArquivo);

    //Cria imagem binaria e l� a mesma do disco
    static I3DRawImage *OpenImgBinary (std::string nomeArquivo,
                                       unsigned int nx, unsigned int ny,
                                       unsigned int nz)
    {
        return OpenImg1bpp (nomeArquivo, nx, ny, nz);
    };
    static I3DRawImage *OpenImg1bpp (std::string nomeArquivo,
                                     unsigned int nx, unsigned int ny,
                                     unsigned int nz);
    static I3DRawImage *OpenImg16bpp (std::string nomeArquivo, unsigned int nx,
                                      unsigned int ny, unsigned int nz);

    //Funcao que l� uma imagem 16bpp do disco, mas s� marca como v�lido
    //os pontos que coincidem com o indiceCor recebido.
    static I3DRawImage *OpenImg16bppFrom1bpp (std::string nomeArquivo,
            unsigned int nx, unsigned int ny,
            unsigned int nz,
            unsigned int indiceCor = 0);

    //----------------------------------------------------------------------------
    //    CONVERSOES      CONVERSOES      CONVERSOES      CONVERSOES
    //----------------------------------------------------------------------------
    //Cria e retorna uma copia bin�ria de uma imagem bin�ria
    static I3DRawImage *GetCopyBinaria (I3DRawImage * img)
    {
        return GetCopy1bpp (img);
    };

    static I3DRawImage *GetCopy1bpp (I3DRawImage * img);

    //Recebe uma imagem16bpp e um indice de corte, retorna uma imagem binaria (nova)
    static I3DRawImage *GetImg1bppFrom16bpp (I3DRawImage * img16bpp,
            unsigned int indiceCorte = 0);

    //Recebe uma imagemBinaria e um valor, retorna uma imagem 16bpp nova, preenchida com valor
    static I3DRawImage *GetImg16bppFrom1bpp (I3DRawImage * imgBinaria,
            unsigned int cor = 1);

    //Dada uma imagem16bpp e um indice de cor, vai preencher na imagem binaria
    //os pontos que coincidem com o indice de cor passado
    static bool SetImg1bppFrom16bppColor (I3DRawImage * img16bpp,
                                          I3DRawImage * imgBinaria,
                                          uint32 indiceCorRegiao);

    //Rotaciona a imagem na direcao YZ, ou seja troca Y->Z e Z->Y
    //Basta passar a imagem, e os flags da direcoes a serem rotacionadas
    //static void Rotate(I3DRawImage* imgBinaria, bool x=0, bool y=0, bool z=0);

    //Dada uma imagem inteira, um intervalo definido pelos pontos (xi,yi,zi) e (xf,yf,zf)
    //retorna uma imagem nova, que cont�m somente o n�cleo da imagem definidos pelos 2 pontos
    //Adicionalmente pode-se passar um indice de corte, que reduz ainda mais a imagem final
    //Um �ndice de corte=2 provoca um descascamento de toda a imagem de 2 p�xeis
    static I3DRawImage *GetNewImgShelled (I3DRawImage * imgDados,
                                          unsigned int xi, unsigned int yi,
                                          unsigned int zi, unsigned int xf,
                                          unsigned int yf, unsigned int zf,
                                          unsigned int corte = 0);

    //Dado um fator de resample, retorna uma nova imagem resamplada
    static I3DRawImage *GetCopyResampled (I3DRawImage * img,
                                          unsigned int resample);

    //----------------------------------------------------------------------------
    //Metodos para obtencao de dados
    //----------------------------------------------------------------------------
    //Funcao que recebe uma imagem16bpp e retorna o maior valor na imagem
    static unsigned int GetMaxValueFrom16bppImage (I3DRawImage * img16bpp);

    //Funcao que recebe uma imagem32bpp e retorna o maior valor na imagem
    static unsigned int GetMaxValueFrom32bppImage (I3DRawImage * img32bpp);

    //Retorna o numero de pixeis da imagem binaria (p�xeis ativos ==1)
    static unsigned int GetNumberOfPixelsActived (I3DRawImage * imgBinaria);

    //Dada uma imagem binaria, determina o raio da maior bola
    //recebe ou uma imagem bin�ria ou uma imagem 16bpp
    static unsigned int GetMaxIDFRadius (I3DRawImage * img3D);

    //----------------------------------------------------------------------------
    //Conexao imagens
    //----------------------------------------------------------------------------
    //Verifica se tem pixel ativo no primeiro plano
    static bool ActivePixelInFirstPlane (I3DRawImage * img);

    //verifica se tem pixel ativo no ultimo plano
    static bool ActivePixelInLastPlane (I3DRawImage * img);

    //Retorna true se conectada (pixel ativo nos planos 0 e n)
    static bool Conected (I3DRawImage * img);

    //Retorna uma copia conectada
    static I3DRawImage *GetCopyConected (I3DRawImage * img);


    //static TMatriz3D* GetTMatriz3D(I3DRawImage* img);
    static TRotulador3D *GetTMatriz3D (I3DRawImage * img1bpp);

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

typedef TOperacoesImg TOI;

#endif


/*
Objeto I3DRawImage:
-------------------
I3DRawImage-> Imagem 3D base
I3DRotuledImage-> Imagem r�tulo
Ex:
   -------
   1)Assumindo que a I3DRotuledImage tem uma imagem3D e atributos internos

	 I3DRotuledImage* imgRot=new I3DRotuledImage(imagem);
   imgRot->Go();								//Processa rotulagem
   imgRot->image->Size();				//acesso a imagem
   ptrImg = imgRot->imageptr();	//retorna ponteiro para imagem
   imgRot->Area(rotuloObjeto);  //Retorna �rea do objeto (fun��o interna, realiza o calculo 1 ves)
   imgRot->area(rotuloObjeto);	//vetor �rea

   imgRot->ListPixeis(rotuloObjeto); 			//Retorna um objeto lista de p�xeis
   imgRot->ListPixeisptr(rotuloObjeto); 	//Retorna ponteiro para a lista de p�xeis

   -------
   2)Assumindo que a I3DRotuledImage � herdeira da  I3DRawImage

	 I3DRotuledImage* imgRot=new I3DRotuledImage(imagem);
   imgRot->Go();								//Processa rotulagem
   imgRot->Size();							//Fun��o herdada
   ptrImg = imgRot;							//retorna ponteiro para imagem
   imgRot->Area(rotuloObjeto);  //Retorna �rea do objeto

   imgRot->ListPixeis(rotuloObjeto); 			//Retorna um objeto lista de p�xeis
   imgRot->ListPixeisptr(rotuloObjeto); 	//Retorna ponteiro para a lista de p�xeis

PS:
	A quest�o fundamental � que os atributos �rea, perimetro e raioHidraulico n�o existem
	sem a exist�ncia da imagem rotulada.
  Isto implica no fato de que �rea,perimetro e raioHidraulico s�o atributos
  de um objeto imagemRotulada. Com isso estes atributos ficam encapsulados, e sua
  determinação e uso ficam mais claros.
*/
