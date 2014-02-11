#ifndef CFEConectividade3D_h
#define CFEConectividade3D_h
/*
----------------------------------------------------------------------------
PROJETO:	LIB_LDSC
		Bibliotecas de Objetos
----------------------------------------------------------------------------
Desenvolvido por:Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
Termofisicas  dos Materiais.
Programadores:   Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi,
							Marcos Damiani,...
Copyright @1997:    Todos os direitos reservados.
Nome deste arquivo: CFEConectividade3D.h
Nome da classe:     CFEConectividade3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/

#ifndef __IOSTREAM_H
#include <iostream>
#endif
/*
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif
*/
#include <Rotulador/CRotulador3D.h>
#include <Matriz/CImagem3D.h>
#include <set>

/**
 * @brief Declara a classe CFEConectividade3D.
 *
 * A classe CFEConectividade3D representa conceitos geométricos.
 *
 * A classe  CFEConectividade3D é filha da classe CRotulador3D.
 * De forma que recebe uma imagem  no construtor e processa a
 * rotulagem através da função Go (da mesma forma que na classe pai).
 *
 * Acrecenta entretanto a função GetImagemConectada, que retorna os objetos da
 * imagem que tiverem conectividade entre a primeira e última camada.
 *
*/

using namespace std;

class CFEConectividade3D : public CRotulador3D
{

// Atributos
    CMatriz3D *pmOrig;		/// Ponteiro para a imagem recebida. Usado na função GetImagemConectada.
    // A função verifica conectividade não recebe a imagem, pois é necessária a rotulagem da imagem(funcao Go()).
    // A rotulagem pode ser feita uma única vez, e a verificação da conectividade diversas vezes.

    //int rotuloRegiaoB;	/// Rotulo dado a região B
    // Obs: Futuramente pode ter varias regiões B, (vários rótulos)?

// Métodos
protected:
    //// Pesquisa na imagem ponto com o indice passado e determina o seu rotulo
    //void DeterminaRotuloRegiaoB (int &indice);

public:
    /// Construtor (recebe CImagem3D)
    CFEConectividade3D (CImagem3D * imagem) : CRotulador3D ( dynamic_cast<CMatriz3D *>(imagem) ), pmOrig (imagem) { }

    /// Construtor (recebe CMatriz3D)
    CFEConectividade3D (CMatriz3D * imagem) : CRotulador3D ( imagem ), pmOrig ( dynamic_cast<CImagem3D *>(imagem) ) { }

    /// Destrutor
    virtual ~ CFEConectividade3D () { }

    /// Retorna a imagem conectada de acordo com o indice e com o valor de fundo informados.
    CImagem3D * GetImagemConectada (int indice, int fundo);


    /// Recebe imagem 3D e retorna imagem 3D conectada.
    // Por padrão fundo == 0.
    // Depois adicionar a Direção Gox Goy Goz
    // retiramos nova alocação, percorre matriz list.size()
    bool Go (CMatriz3D * imagem, int indice = 1, int fundo = 0 /*,char direcao = 'y' */);
};

#endif
