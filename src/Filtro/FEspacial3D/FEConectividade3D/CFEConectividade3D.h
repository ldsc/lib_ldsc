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
#include <Matriz/TCImagem3D.h>
#include <Matriz/TCMatriz3D.h>
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
public:
  /// Construtor (recebe TCImagem3D)
	CFEConectividade3D ( TCImagem3D<int> * imagem, int indice = 1, int fundo = 0 )
     : CRotulador3D ( dynamic_cast<TCMatriz3D<int> *>(imagem), indice, fundo ) {
  }

  /// Construtor (recebe CMatriz3D)
	CFEConectividade3D ( TCMatriz3D<int> * imagem, int indice = 1, int fundo = 0 )
     : CRotulador3D ( imagem, indice, fundo ) {
  }

  /// Destrutor
  virtual ~ CFEConectividade3D () { }

// Métodos
  /// Retorna a lista de objetos conectados.
	set<int> GetObjetosConectados ( TCImagem3D<int> * pmOrig);

  /// Retorna a lista de objetos conectados.
  set<int> GetObjetosConectados ( TCMatriz3D<int> * pmOrig);

  /// Verifica se a imagem possui conectividade no eixo Y.
	bool isConnected ( TCImagem3D<int> * pmOrig);

  /// Verifica se a matriz possui conectividade no eixo Y.
  bool isConnected ( TCMatriz3D<int> * pmOrig);

  /// Retorna uma nova imagem conectada de acordo com o indice e com o valor de fundo informados.
	TCImagem3D<int> * GetImagemConectada ( TCImagem3D<int> * pmOrig);

  /// Retorna uma nova matriz conectada de acordo com o indice e com o valor de fundo informados.
  TCMatriz3D<int> * GetMatrizConectada ( TCMatriz3D<int> * pmOrig);

  /// Altera Imagem 3D retornando Imagem 3D conectada.
	bool Go (TCImagem3D<int> * pmCon);

  /// Altera Matriz 3D retornando matriz 3D conectada.
  bool Go (TCMatriz3D<int> * pmCon);



};

#endif
