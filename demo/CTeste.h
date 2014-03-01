
#ifndef CTeste_h
#define CTeste_h

/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: CTeste...
  ===============================================================================
  Desenvolvido por:
  Laboratorio de Desenvolvimento de Software Cientifico
  [LDSC].
  @author     Andre Duarte Bueno
  @file       CTeste.h
  @begin      Sat Sep 16 2000
  @copyright  (C) 2000 by Andre Duarte Bueno
  @email      andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//Bibliotecas LIB_LDSC
//-----------------------------------------------------------------------
//#include <Base/_LIB_LDSC_CLASS.h>

/*
  ===============================================================================
  Documentacao Classe: CTeste
  ===============================================================================
*/
/**
   @short
   Classe de teste das diversas classes da LIB_LDSC.
   O objetivo e dentro da main criar CTeste e chamar Run
   que cria e chama as demais classes.

   Assunto:     Teste da LIB_LDSC
   Superclasse:  CTeste
   @author 	Andre Duarte Bueno
   @version	versao...
   @see		veja assunto...
*/
class CTeste
{
    //--------------------------------------------------------------Atributos
public:

    //-------------------------------------------------------------Construtor
    //Construtor
    CTeste ()
    {
    }

    //--------------------------------------------------------------Destrutor
    //Destrutor
    virtual ~ CTeste ()
    {
    }

    //----------------------------------------------------------------Metodos
public:
    /** Funcao principal, executada por main().
        Vai solicitar ao usuario o tipo de objeto a ser criado,
        criar o objeto e passar o controle
        do programa para o objeto criado */
    void Run (unsigned int argc = 1, char *argv[] = 0);

    //--------------------------------------------------------------------Get
    //--------------------------------------------------------------------Set
    //-----------------------------------------------------------------Friend
    //friend ostream& operator<< (ostream& os, CTeste& obj);
    //friend istream& operator>> (istream& is, CTeste& obj);
};

//-----------------------------------------------------------------Friend
#endif
