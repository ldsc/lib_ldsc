#ifndef CFEConectividade3D_h
#define CFEConectividade3D_h
/*
----------------------------------------------------------------------------
PROJETO:	LIB_LMPT
		Bibliotecas de Objetos
----------------------------------------------------------------------------
Desenvolvido por:Laboratorio de Meios Porosos e Propriedades Termofisicas  dos Materiais.
Programadores:   Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi,
							Marcos Damiani,...
Copyright @1997:    Todos os direitos reservados.
Nome deste arquivo: CFEConectividade3D.h
Nome da classe:     CFEConectividade3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/

/*
============================================================================
Documentacao Classe:		CFEConectividade3D
============================================================================
Assunto:               CFEConectividade3D
Superclasse:           CRotulador3D
Descri��o:             Declara a classe CFEConectividade3D.

   A classe CFEConectividade3D representa conceitos geom�tricos.

   A classe  CFEConectividade3D � filha da classe TRotulagem3D. De forma que recebe uma imagem
   no construtor e processa a rotulagem atrav�s da fun��o Go (da mesma forma que na classe pai).
   Acrecenta entretanto a fun��o VerificaConectividade, utilizada para verificar a conectividade
   de duas regi�es na imagem.

   A funcao VerificaConectividade(ira,irb,irc) recebe como parametros os �ndices das regioes A, B
   e irc=indiceRegiaoConexa, utilizado para "pintar" a regi�o conexa.
   Ou seja, o que diferencia as duas regi�es A e B s�o seus �ndices na matriz imagem.
   Outro aspecto importante com relacao a funcao VerificaConectividade, � que para determinar
   o rotulo da regi�o B, a funcao faz uma pesquisa, a procura de pontos na imagem que tenham o indice
   indiceRegiaoB, depois verifica na matriz rotulo, o rotulo que foi dado a regi�o B. O que
   implica que a regi�o B deve ser �nica. Se B for multipla, somente uma delas ser� considerada.

Acesso:
Cardinalidade:
Abstrata/Concreta:		Concreta
Arquivo de documentacao auxiliar:
----------------------------------------------------------------------------
*/

#ifndef __IOSTREAM_H
#include <iostream>
#endif

#ifndef _LIB_LMPT_CLASS_h
#include <Base/_LIB_LMPT_CLASS.h>
#endif

#include <Rotulador/CRotulador3D.h>


class CFEConectividade3D : public CRotulador3D
{

// Atributos
    CMatriz3D *pm;		// Ponteiro para a imagem recebida, usado na fun��o VerificaConectividade.

    // A fun��o verifica conectividade n�o recebe a imagem, pois � necess�ria
    // a rotulagem da imagem(funcao Go()).
    // A rotulagem pode ser feita uma �nica vez, e a
    // verifica��o da conectividade diversas vezes.
    int rotuloRegiaoB;		// rotulo dado a regi�o B

    // Obs: Futuramente pode ter varias regi�es B, (v�rios r�tulos)?

// M�todos

protected:
    // Pesquisa na imagem ponto com o indice passado e determina o seu rotulo
    void DeterminaRotuloRegiaoB (int &indice);

public:
    // Construtor Default
    // CFEConectividade3D():CRotulador3D(),CMatriz3D(){};// Exige a defini��o de pm e data3D posteriormente
    CFEConectividade3D (CMatriz3D * imagem):CRotulador3D (imagem), pm (imagem)
    {
    }

    // Destrutor
    virtual ~ CFEConectividade3D ()
    {
    }

    // A regi�o conexa � marcada com valores negativos
    void VerificaConectividade (int &indiceRegiaoA, int &indiceRegiaoB);

    // A regi�o conexa � marcada com o valor indiceRegiaoConexa
    void VerificaConectividade (int &indiceRegiaoA, int &indiceRegiaoB,
                                int &indiceRegiaoConexa);

    // A regi�o conexa � marcada com os novos �ndices passados
    void VerificaConectividade (int &indiceRegiaoA, int &indiceRegiaoB,
                                int &novoIndiceA, int &novoIndiceB);
};

#endif

