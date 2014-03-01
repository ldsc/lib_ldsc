

#ifndef IListPixel_h
#define IListPixel_h
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
//COI-LIB
//-----------------------------------------------------------------------
#include <coi20/sys/types/primitive.h>

//-----------------------------------------------------------------------
//Imago

//-----------------------------------------------------------------------
//aqui
//#include <imago/img3d/rawimg3d.h>
#include <imago/img3d/image/raw.h>

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
#ifndef _LIB_LMPT_CLASS_h
#include <Base/_LIB_LMPT_CLASS.h>
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


class IListPixel
{
    //--------------------------------------------------------------Atributos
private:
protected:
    uint32 numberOfObjects;
    uint32 pos;
public:
    uint32 * x;
    uint32 *y;
    uint32 *z;
    //-------------------------------------------------------------Construtor
    IListPixel ()
    {
        numberOfObjects = 0;
        pos = 0;
        x = y = z = NULL;
    };
    IListPixel (uint32 n)
    {
        x = y = z = NULL;
        Alocate (n);
        pos = 0;
    };

    //--------------------------------------------------------------Destrutor
    virtual ~ IListPixel ()
    {
        if (x != NULL)
            delete x;
        if (y != NULL)
            delete y;
        if (z != NULL)
            delete z;
    }

    //----------------------------------------------------------------M�todos
private:
protected:
public:

    //Posiciona no in�cio
    void Init ()
    {
        pos = 0;
    };
    //Posiciona na proxima posi��o v�lida
    bool Next ();
    //Aloca vetores
    bool Alocate (int32 n);
    //Realoca vetores
    bool Realocate (int32 n)
    {
        return Alocate (n);
    };

    //Fun��o de prop�sito qualquer, geralmente usada para calcular
    virtual bool Go (I3DRawImage * &imagemBinaria) = 0;

    //Fun��o de prop�sito qualquer, geralmente usada para calcular
    //os pontos da lista de p�xeis.
    //virtual bool Go(I3DRawImage*& imagemBinaria){};

    //-------------------------------------------------------------Sobrecarga
    //--------------------------------------------------------------------Get
    //Retorna posi��o x
    uint32 Getx () const
    {
        return x[pos];
    };
    //Retorna posi��o y
    uint32 Gety () const
    {
        return y[pos];
    };
    //Retorna posi��o z
    uint32 Getz () const
    {
        return z[pos];
    };
    //Retorna n�mero objetos
    uint32 GetnumberOfObjects () const
    {
        return numberOfObjects;
    };
    //Retorna posi��o atual
    uint32 Getpos () const
    {
        return pos;
    };

    //void   SetnumberOfObjects(int _n) {numberOfObjects = _n;};
    //uint32 Setpos(uint32 _pos){_pos < numberOfObjects ? pos = _pos:pos = numberOfObjects-1; return pos;};

    //--------------------------------------------------------------------Set
    //Objetiva definir a posi��o atual para fun��es Get
    bool Setpos (uint32 & _pos)
    {
        if (_pos < numberOfObjects)
        {
            //Define a posi��o
            pos = _pos;
            //retorna true
            return 1;
        }
        //Define como sendo a ultima posi��o
        pos = numberOfObjects - 1;
        //retorna false
        return 0;
    };

    //-----------------------------------------------------------------Friend
    friend ostream & operator<< (ostream & os, const IListPixel & f);
    //friend istream& operator>> (istream& is, |CLASSNAME|& obj);
};

//-----------------------------------------------------------------Friend
//Declara��o de Fun��es Friend
ostream & operator<< (ostream & os, const IListPixel & obj);
//istream& operator>> (istream& is, |CLASSNAME|& obj);

#endif
/*
  class IListPixel
  {
  //Atributos
  uint32 numberOfObjects;      	//N�mero de objetos
  uint32 pos;                  	//Posi��o

  public:
  //Metodos
  IListPixel()	{		//Construtor
  numberOfObjects = 0;       //Zera n�mero de objetos
  pos = 0 ;                  //Marca pos
  };
  virtual ~IListPixel() 	//Destrutor
  {
  }
  //Retorna true se ainda nao chegou ao final da lista
  bool 	Next();		//Posiciona na proxima posi��o v�lida
  void   Init()
  {pos = 0 ;};		//Posiciona no in�cio
  virtual bool 	Alocate(uint32 n)=0;		//Aloca vetores
  virtual bool 	Realocate(uint32 n)	{return Alocate(n);};		//Realoca vetores
  uint32 GetnumberOfObjects() const {return numberOfObjects;}; //Retorna n�mero objetos
  uint32 Getpos()	const {return pos;};          //Retorna posi��o atual

  bool Setpos(uint32 _pos)			{	//Objetiva definir a posi��o atual para fun��es Get
  if(_pos < numberOfObjects )
  {
  pos =	_pos;                //Define a posi��o
  return 1;                  //retorna true
  }
  pos =	numberOfObjects - 1;    //Define como sendo a ultima posi��o
  return 0;                     //retorna false
  };

  };

  class IListPixel2D: public IListPixel
  {
  //Atributos
  protected:
  uint32 *x;                  //Vetor com valores x
  uint32 *y;                  //Vetor com valores y

  public:
  //Metodos
  IListPixel2D():x(NULL),y(NULL)
  {				//Construtor
  };
  IListPixel2D(uint32 n):x(NULL),y(NULL)
  {
  IListPixel2D::Alocate(n);	//Aloca os vetores
  pos = 0 ;
  };
  virtual ~IListPixel2D()//Destrutor
  {
  if(x!=NULL) delete x;
  if(y!=NULL) delete y;
  }
  virtual bool 	Alocate(uint32 n);//Aloca vetores
  uint32 Getx()const {return x[pos];};		//Retorna posi��o x
  uint32 Gety()	const {return y[pos];};          //Retorna posi��o y
  };

  class IListPixel3D: public IListPixel2D
  {
  //Atributos
  protected:
  uint32 *z;//Vetor com valores z

  public:
  //Metodos
  IListPixel2D():z(NULL)
  {//Construtor
  };
  IListPixel2D(uint32 n):x(NULL),y(NULL)
  {
  IListPixel3D::Alocate(n);	//Aloca os vetores
  pos = 0 ;
  };
  virtual ~IListPixel2D()//Destrutor
  {
  if(x!=NULL) delete x;
  if(y!=NULL) delete y;
  }
  virtual bool 	Alocate(uint32 n);//Aloca vetores
  uint32 Getz() 	       const {return z[pos];};		//Retorna posi��o z
  };
*/


/*
  Objeto IListPixel:
  -------------------
  IListPixel->	Deve ser uma classe base, para uma lista de p�xeis.
  IListPixel2D->	lista de p�xeis de  imagem 2D
  IListPixel3D->	lista de p�xeis de  imagem 3D
*/
