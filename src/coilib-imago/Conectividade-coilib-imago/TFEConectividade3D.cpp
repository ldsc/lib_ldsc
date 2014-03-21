// =======================================================================
// PROJETO:          Biblioteca LIB_LMPT
//                   Ramo: TPadrao_ramo
// =======================================================================
//
// Desenvolvido por:    Laboratorio de Meios Porosos
//                      e Propriedades Termofisicas     [LMPT].
// @author:          André Duarte Bueno
// File:             CFEConectividade3D.cpp
// begin:            Fri Nov 17 2000
// copyright:        (C) 2000 by André Duarte Bueno
// email:            andre@lmpt.ufsc.br

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LMPT
// -----------------------------------------------------------------------
// #include <path/nome>

/*
  -------------------------------------------------------------------------
  Fun��o:
  -------------------------------------------------------------------------
  @short  :
  @author :André Duarte Bueno
  @see    :
  @param  :
  @return :
*/

#ifndef CFEConectividade3D_cpp
#define CFEConectividade3D_cpp
/*
  ----------------------------------------------------------------------------
  PROJETO:					Imago
  ----------------------------------------------------------------------------

  Desenvolvido por:	  Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
  Programadores:   	  Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi,
  Andre D.Bueno,Luiz Orlando Emerich, Marcos Damiani,...
  Copyright @1997-1999:  Todos os direitos reservados.
  Nome deste arquivo:	 CFEConectividade3D.h
  Nome da classe:        CFEConectividade3D
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
  ----------------------------------------------------------------------------
*/
// Imago
// #include <imago/img3d/pixit3d1.h>
// #include <imago/img3d/pixit3d32.h>
#include <imago/img3d/pixit/pixit1.h>
#include <imago/img3d/pixit/pixit32.h>

// LIB_LMPT
#include <TConectividade/CFEConectividade3d.h>
// novo
#include <TConversaoImago/TOperacoesImg.h>

// #include <confeq/CFEConectividade3D.h>
// nclude <TConversaoImago/TOperacoesImg.h>                             // inclue o objeto, declaracao da classe

// ----------------------------------------------------------------------------
// Recebe a imagem rotulada, a imagem a ser preenchida e uma lista com os pontos
// que s�o usados para obtencao dos r�tulos a serem considerados no processo de conex�o
// A imagem que originou a lista de p�xeis deve ter o mesmo tamanho da imagem rotulo,
// sobre pena de ocorrer estouro de pilha.

bool
CFEConectividade3D::VerifyConectivit (I3DRawImage * imgRotulo,	// imagem rotulada 32bpp,
                                      IListPixel * lPixel,	// lista de pixeis
                                      I3DRawImage * imgBinaria	// imagem a ser marcada
                                     )
{
    // Testa as imagens recebidas
    if (imgRotulo->BPP () != 32 || imgBinaria->BPP () != 1)
    {
        assert (0);		// Sai do programa
        return 0;
    }

    // Se o n�mero de objetos, for zero, zera a imagem bin�ria e sai
    if (lPixel->GetnumberOfObjects () == 0)	// Se n�o exitem pixeis (n�o foi alocado)
    {				// zera a imagem e sai
        imgBinaria->ClearBits ();	// pois n�o existem conex�es
        return 1;			// novo, agora retorna um bool
    }

    // ---------------------------------------------------------------------------
    // Determina a lista de rotulos a serem considerados na conex�o
    // O n�mero de r�tulos v�lidos pode ser no m�ximo igual ao n�mero de objetos da lista de p�xeis
    uint32 *
    listaRotulosValidos = new uint32[lPixel->GetnumberOfObjects ()];
    uint32
    cont = 0;			// Contador do n�mero efetivo de r�tulos v�lidos
    uint32
    rotuloAtual;		// �ndice do rotulo atual
    lPixel->Init ();		// Posiciona no inicio do vetor de pixeis , faz pos = 0
    // Cria pixel iterator para imagem r�tulo
    I3DPixelIterator32 *
    piimgRotulo =
        I3DPixelIterator32::SafeCast (imgRotulo->InitPixelIterator ());

    // A imagem rotulada recebida � construida com a soma da imagem do flu�do de interesse
    // com as camaras a serem conectadas a este flu�do.
    // A lista de p�xeis aponta para pontos v�lidos das camaras.
    do
    {				// A lista de p�xeis aponta para pontos das camaras
        rotuloAtual =
            piimgRotulo->Get (lPixel->Getx (), lPixel->Gety (), lPixel->Getz ());
        // novo coment�rio abaixo, pois o r�tulo nunca vai ser 0, quando determina a lista de p�xeis o fundo � desconsiderado.
        if (rotuloAtual != 0)	// se n�o for fundo
        {			// anota
            listaRotulosValidos[cont] = rotuloAtual;	//  7 objetos, fundo + 6 objetos
            cont++;		// incrementa o contador de r�tulos v�lidos
        }
    }
    while (lPixel->Next ());	// Next avan�a a posi��o dos p�xeis e retorna true se nao chegou ao final da lista
    // enquanto nao chegou ao final
    // 0,1,2,3,4,5,5,6 (7 objetos)

    // ---------------------------------------------------------------------------
    // Agora j� tem os r�tulos dos objetos que fazem parte da regi�o conexa,
    // Abaixo percorre a imagem r�tulo e seta na imagem bin�ria todos os pontos que pertencem
    // aos objetos identificados
    imgBinaria->ClearBits ();	// zera a imagem binaria
    I3DPixelIterator1 *
    piimgBinaria =
        I3DPixelIterator1::SafeCast (imgBinaria->InitPixelIterator ());
    piimgRotulo->Reset ();	// Vai para posi��o 0,0,0 na imagem rotulo
    // Percorre as imagens r�tulo e bin�ria
    for (; !piimgBinaria->ImageWrapped ();
            piimgBinaria->Next (), piimgRotulo->Next ())
    {
        rotuloAtual = piimgRotulo->Get ();	// Obt�m rotulo da imagem
        for (int i = 0;		// Percorre os rotulos v�lidos da lista
                i < cont;		// se numberOfObjects=3 tem 3 rotulos para testar
                i++)			// novo, corrigi um bug, antes no lugar de i usava novamente cont
            if (rotuloAtual == listaRotulosValidos[i])	// Se o rotulo atual da imagem, for igual a algum da lista
            {
                piimgBinaria->Set (1);	// Marca o pixel da imagem resultado como sendo conectado
                i = cont;		// se j� setou sai
                // break ou continue
            }
    }
    delete
    listaRotulosValidos;	// Deleta a lista de r�tulos v�lidos

    delete
    piimgRotulo;
    delete
    piimgBinaria;
    return 1;
}

// ----------------------------------------------------------------------------
// Fun��o que recebe uma imagem bin�ria
// rotula a mesma
// retorna a mesma imagem, mas conectada na dire��o z
/*
  bool CFEConectividade3D::GetConectedImgZ(I3DRawImage* imgBinaria)
  {
  // Verifica se a imagem � bin�ria
  if(imgBinaria->BPP()!=1)
  {
  assert(0);
  return 0;
  }

  // Retorna uma imagem 3D (rotulador3D) da lib_lmpt (copia da imgBinaria)
  TRotulador3D* img3D = TOI::GetTMatriz3D(imgBinaria);
  img3D->Go(img3D);// rotula �la mesma, n�o alocando uma nova img (testar)

  // Verifica a conec��o na dire��o Z
  IListPixel *  lPixelZ0;
  IListPixel *  lPixelZn;

  // Cria pixel iterator
  I3DPixelIterator1*  piimgBinaria = I3DPixelIterator1::SafeCast(imgBinaria->InitPixelIterator());
  piimgRotulo->Reset();

  // A fun��o abaixo recebe uma imagem da coilib, rotula a mesma usando
  // o filtro de rotulagem do imago (damiani) e retorna,
  // uma lista de p�xeis da imagem que abrange todos os r�tulos da imagem.
  // bool  IListPixelRotuled::Go(I3DRawImage*& imgBinaria)



  delete  piimgBinaria;
  }
*/
#endif
