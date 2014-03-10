#ifndef TIConfEq3DSimPermRelEsq3Dby2D_h
#define TIConfEq3DSimPermRelEsq3Dby2D_h

/*
  =========================================================================
  PROJETO:    	Biblioteca LIB_LMPT
  Assunto/Ramo: TIConfEq3DSimPermRelEsq3Dby2D
  =========================================================================
  Desenvolvido por:
  Laboratorio de Meios Porosos e Propriedades Termofisicas
  [LMPT].
  @author     	Andr� Duarte Bueno
  @file       	TIConfEq3DSimPermRelEsq3Dby2D.h
  @begin      	Fri Nov 17 2000
  @copyright  	(C) 2000 by Andr� Duarte Bueno
  @email      	andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecasb C/C++
//-----------------------------------------------------------------------
#include <string>

//-----------------------------------------------------------------------
//Bibliotecas LIB_LMPT
//-----------------------------------------------------------------------
#ifndef 	 TIConfEq3DSimPermRel_h
#include <TConEquCOILIB/TIConfEq3DSimPermRel.h>
#endif

/*
  =========================================================================
  Documentacao Classe: |TIConfEq3DSimPermRelEsq3Dby2D|
  =========================================================================
  Assunto:      TIConfEq3DSimPermRelEsq3Dby2D
  Superclasse:  a propria
  Descri��o:    Declara a classe TIConfEq3DSimPermRelEsq3Dby2D.
  Herdeira da classe TIConfEq3DSim,
  acrescenta nos calculos informa��es relacionadas a determinação
  da permeabilidade relativa.Ou seja, use TConfiguracoesEquilibrio
  para obter as configuracoes de equilibrio (visualizacao)
  TIConfEq3DSim para determinação das curvas de pressao capilar
  e  TIConfEq3DSimPermRelEsq3Dby2D quando deseja ainda as curvas
  de permeabilidade relativa.
  @short	      Breve descri��o
  @author       Andr� Duarte Bueno
  @version
  @see	      Veja assunto
*/

class TIConfEq3DSimPermRelEsq3Dby2D:public TIConfEq3DSimPermRel
{
    COI20_CLASSID COI20_SAFECAST (TIConfEq3DSimPermRelEsq3Dby2D, IObject)
    //Define o tipo de modelo de calculo das condutâncias
private:

    //Usado para definir o modelo de calculo da permeabilidade
    int tipoGrafo;

    //Usado para gerar o nome do arquivo a ser passado para o grafo
    //inclue informa��es de modelo .mod1, .mod2, .mod3, .mod4
    //O grafo esta atualmente salvando os dados do grafo em disco.
    std::string nomeArquivoSaidaGrafo;


public:
    //-------------------------------------------------------------Construtor
    TIConfEq3DSimPermRelEsq3Dby2D (int _tipoGrafo, std::string _nomeArquivoSaidaGrafo,	//?????
                                   TIChamber * _chamber,
                                   bool _molhabilidadeB = 1,
                                   bool _compressibilidadeA = 1,
                                   uint32 _incremento = 1,
                                   uint32 _raioMaximo = 0,
                                   uint32 _raioMinimo = 0,
                                   bool _salvarDisco = false,
                                   int _borderDiscount = 2
                                                         //novo em 22-7-2002
                                                         , string fileArq = '\0'):
            TIConfEq3DSimPermRel (_chamber,
                                  _molhabilidadeB,
                                  _compressibilidadeA,
                                  _incremento,
                                  _raioMaximo,
                                  _raioMinimo,
                                  _salvarDisco,
                                  _borderDiscount
                                  //novo em 22-7-2002
                                  , fileArq)
    {
        tipoGrafo = _tipoGrafo;
        //strcpy(nomeArquivoSaidaGrafo,_nomeArquivoSaidaGrafo);
        nomeArquivoSaidaGrafo = _nomeArquivoSaidaGrafo;
    };

    //--------------------------------------------------------------Destrutor
    virtual ~ TIConfEq3DSimPermRelEsq3Dby2D ()
    {
    };

    //----------------------------------------------------------------M�todos
private:
protected:

    //Nao esta sendo chamada para calculo da permeabilidade intrinseca
    virtual double CalculaPermeabilidade (I3DRawImage * &img);
    //Foi incluida para verificar se chama acima tambem para
    //o calculo da permeabilidade intrinseca
    virtual void PrepareConfEqSim ();

    //-------------------------------------------------------------Sobrecarga

    //--------------------------------------------------------------------Get
    virtual std::string GetClassName () const
    {
        return std::string ("TIConfEq3DSimPermRelEsq3Dby2D");
    };

    //--------------------------------------------------------------------Set
    //-----------------------------------------------------------------Friend
    //      friend ostream& operator<< (ostream& os, const |TIConfEq3DSimPermRelEsq3Dby2D|& obj);
    //      friend istream& operator>> (istream& is, |TIConfEq3DSimPermRelEsq3Dby2D|& obj);

    //-----------------------------------------------------------------Friend
    //Declara��o de Fun��es Friend
    //ostream& operator<< (ostream& os, const |TIConfEq3DSimPermRelEsq3Dby2D|& obj);
    //istream& operator>> (istream& is, |TIConfEq3DSimPermRelEsq3Dby2D|& obj);
};
#endif
