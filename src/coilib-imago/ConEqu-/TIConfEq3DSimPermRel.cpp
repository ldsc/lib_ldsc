//=======================================================================
//PROJETO:          Biblioteca LIB_LMPT
//                  Ramo: TPadrao_ramo
//=======================================================================
//
//Desenvolvido por: Laboratorio de Meios Porosos
//e Propriedades Termofisicas     [LMPT].
//@author:          Andr� Duarte Bueno
//File:             TOConfEq3DSimPermRel.cpp
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
Fun��o:.
-------------------------------------------------------------------------
@short  :
@author :Andr� Duarte Bueno
@see    :
@param  :
@return :
*/

//#ifndef TIConfEq3DSimPermRel_cpp   //TIConfEq3DSimPermRel
// #define TIConfEq3DSimPermRel_cpp

/*
----------------------------------------------------------------------------
PROJETO:					  Imago
----------------------------------------------------------------------------

Desenvolvido por:	  Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:            Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi,
			  Andre D.Bueno,Luiz Orlando Emerich, Marcos Damiani,...
Copyright @1997-1999: Todos os direitos reservados.
Nome deste arquivo:	  TIConfEq3DSimPermRel.h
Nome da classe:       TIConfEq3DSimPermRel
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/

//C++
#include <cstdio>
#include <iomanip>
#include <cstring>
#include <cstdlib>

//COI
//#include <win/window.h>
//Imago

//#include <imago/img3d/pixit3d1.h>
//#include <imago/img3d/pixit3d16.h>
//#include <imago/img3d/pixit3d32.h>
#include <imago/img3d/pixit/pixit1.h>
#include <imago/img3d/pixit/pixit16.h>
#include <imago/img3d/pixit/pixit32.h>

//abaixo novo
#include <imago/img3d/image/raw.h>

//classifica��o
//#include <imago/img3d/classify.h>
#include <imago/img3d/skeleton/simulation.h>
#include <imago/img3d/skeleton/classify.h>

//Retorna imagem conectada
//#include <imago/flt3d/flt3dcon.h>
#include <imago/flt3d/core/connect.h>

//simula��o permeabilidade pelo esqueleto
//#include <imago/img3d/skesimul.h>

//ConfEq
#include <Base/_LIB_LMPT_CLASS.h>
#include <TConEquCOILIB/TIConfEq3DSimPermRel.h>
//inclue o objeto, declaracao da classe
#include <TConversaoImago/TOperacoesImg.h>


//NA classe TIConfEq3DSim usa:
//COI20_CLASSID_DEF1( TIConfEq3DSim, TIConfiguracoesEquilibrio3D );
//NA classe TIConfEq3DSimPermRel usa:
//estava com a linha abaixo
//estava COI20_CLASSID_DEF1( TIConfEq3DSimPermRel, ITask )
COI20_CLASSID_DEF1 (TIConfEq3DSimPermRel, TIConfEq3DSim)
//Chama PrepareConfEq da classe base, que monta um arquivo de disco com os dados do objeto confeq.
//Adiciona no arquivo de disco a PR_B e PR_A, e
//adicionalmente calcula a permeabilidade intrinseca
void TIConfEq3DSimPermRel::PrepareConfEqSim ()
{
    cout << " funcao TIConfEq3DSimPermRel::PrepareConfEqSim()" << endl;
    cout << " chamando funcao TIConfEq3DSim::PrepareConfEqSim()" << endl;

    //Executa funcao da classe base, que preenche cabe�alho
    TIConfEq3DSim::PrepareConfEqSim ();

    //Calcula a permeabilidade intrinseca
    //obtem imagem
    I3DRawImage *imagem = chamber->GetimgPorousMedia ();
    assert (imagem != NULL);

    //salva imagem usada para calculo da permeabilidade intrinseca
    //SaveImg1bpp ( imagem, "imagem_chamber_GetimgPorousMedia.i3d");

    //obtem imagem do n�cleo
    I3DRawImage *imgNucleo = chamber->GetImgNucleo (imagem, borderDiscount);

    //deleta imagem parcial
    delete imagem;
    assert (imgNucleo != NULL);

    //salva imagem usada para calculo da permeabilidade intrinseca
    //     SaveImg1bpp ( imgNucleo, "imagem_chamber_GetimgPorousMedia_nucleo.i3d");
    //calcula permeabilidade intrinseca
    //   intrinsicPermeability = TIConfEq3DSimPermRel::CalculaPermeabilidade(imgNucleo);
    cout <<
         " TIConfEq3DSimPermRel::PrepareConfEqSim()->chamando funcao CalculaPermeabilidade(imgNucleo)"
         << endl;
    //calcula permeabilidade intrinseca
    intrinsicPermeability = CalculaPermeabilidade (imgNucleo);
    delete imgNucleo;

    //Abre arquivo de disco
    //errado ofstream fout(GetFileName().c_str(),ios::app);
    fout.open (GetFileName ().c_str (), ios::app);
    if (fout.fail ())
        return;

    //Adiciona permeabilidade
    fout << "#PemRel_B     PermRel_A    ";
    fout << "\nintrinsicPermeability = " << intrinsicPermeability << endl;
    fout.close ();
}

//Chama a fun��o da classe base, e adicionalmente determina as permeabilidades relativas
//dos n�cleos das imagens A e B
void
TIConfEq3DSimPermRel::IntermediatNextCalculate ()
{
    //Calculo das curvas de press�o capilar na classe base
    TIConfEq3DSim::IntermediatNextCalculate ();

//novo em 22-7-2002
    if (determinarPermeabilidade)
    {
        //Calculo da permeabilidade relativa para FLUIDO B (imgB)
        //prfb[radius - minRadius] = CalculaPermeabilidade(nucleoImgB);
//novo em 22-7-2002
        prfb[radius - minRadius] =
            CalculaPermeabilidade (nucleoImgB) / intrinsicPermeability;

        //Calculo das permeabilidade relativa para FLUIDO A (imgA)
        //prfa[radius - minRadius] = CalculaPermeabilidade(nucleoImgA);
//novo em 22-7-2002
        prfa[radius - minRadius] =
            CalculaPermeabilidade (nucleoImgA) / intrinsicPermeability;
    }
}

void
TIConfEq3DSimPermRel::IntermediatNextOutput ()
{
    //Classe base, sa�da dados curvas de press�o capilar
    TIConfEq3DSim::IntermediatNextOutput ();

    //Saida dos dados de permeabilidade relativa dos flu�dos B e A
    fout << setw (16) << prfb[radius - minRadius] << setw (16) << prfa[radius -
            minRadius];
    fout << " detPerm = " << determinarPermeabilidade;
}

//Fun��o de calculo da permeabilidade em s�.
//calcula direto, sem o uso do di�logo
double
TIConfEq3DSimPermRel::CalculaPermeabilidade (I3DRawImage * &img)
{
    cout <<
         " funcao TIConfEq3DSimPermRel::CalculaPermeabilidade(I3DRawImage*& img)"
         << endl;
    double perm = 0;

    //Verifica se a imagem esta conectada
    if (TOI::Conected (img) == true)
    {
        //Cria filtro de classificacao
        IClassify cls;
        //Executa classifica��o
        if (cls.Go (img))
        {
            //Cria objeto simula��o esqueleto
            ISkeSimulation simul;

            //Executa simula��o
            perm =
                simul.Simulate (cls.Image (), cls.Nodes (), cls.Viscosity ());
        }
    }
//Retorna a permeabilidade
    return perm;
}

/*
class RelativePermeability
{
	 //Objeto configura��es de equil�brio
   TIConfEq3DSim*  confEq;

   //Construtor
   RelativePermeability (TIConfEq3DSim*  _confEq)
   		{
      confEq = _confEq;
      };
   virtual double CalculaPermeabilidade(I3DRawImage* img);
   virtual void Next();
   virtual void Go();
};

RelativePermeability::Prepare()
{
	//obtem imagem parcial
	I3DRawImage* imagem = confEq->chamber->GetimgPorousMedia();
  assert(imagem!=NULL);

  //obtem imagem do n�cleo
	I3DRawImage* imgNucleo = confEq->chamber->GetImgNucleo(imagem,borderDiscount);
	//deleta imagem parcial
  delete imagem;
  assert(imgNucleo!=NULL);

  //calcula permeabilidade intrinseca
	intrinsicPermeability = CalculaPermeabilidade(imgNucleo);
	delete imgNucleo;


	//Abre arquivo de disco
  ofstream fout(confEq->GetFileName().c_str(),ios::app);
  if(fout.fail())
   		return ;
  //Adiciona permeabilidade
  fout << "     PR_B     PR_A";
  fout << "    intrinsicPermeability = "<<  intrinsicPermeability;
  fout.close();
}

void RelativePermeability::Go()
{
   while(confEq->Next())
		Next();
};

void RelativePermeability::Next()
{
}
*/
//#endif
