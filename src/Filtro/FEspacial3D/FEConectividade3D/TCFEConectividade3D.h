#ifndef TCFEConectividade3D_h
#define TCFEConectividade3D_h
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
Nome deste arquivo: TCFEConectividade3D.h
Nome da classe:     TCFEConectividade3D
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
#include <Rotulador/TCRotulador3D.h>
#include <Matriz/TCImagem3D.h>
#include <Matriz/TCMatriz3D.h>
#include <set>

/**
 * @brief Declara a classe TCFEConectividade3D.
 *
 * A classe TCFEConectividade3D representa conceitos geométricos.
 *
 * A classe  TCFEConectividade3D é filha da classe CRotulador3D.
 * De forma que recebe uma imagem  no construtor e processa a
 * rotulagem através da função Go (da mesma forma que na classe pai).
 *
 * Acrecenta entretanto a função GetImagemConectada, que retorna os objetos da
 * imagem que tiverem conectividade entre a primeira e última camada.
 *
*/

using namespace std;

template<typename T>
class TCFEConectividade3D : public TCRotulador3D<T>
{
public:
  /// Construtor (recebe TCImagem3D)
	TCFEConectividade3D ( TCImagem3D<T> * imagem, int indice = 1, int fundo = 0 )
		 : TCRotulador3D<T> ( dynamic_cast<TCMatriz3D<T> *>(imagem), indice, fundo ) {
  }

  /// Construtor (recebe CMatriz3D)
	TCFEConectividade3D ( TCMatriz3D<T> * imagem, int indice = 1, int fundo = 0 )
		 : TCRotulador3D<T> ( imagem, indice, fundo ) {
  }

  /// Destrutor
	virtual ~ TCFEConectividade3D () { }

// Métodos
  /// Retorna a lista de objetos conectados.
	set<int> GetObjetosConectados ( TCImagem3D<T> * pmOrig);

  /// Retorna a lista de objetos conectados.
	set<int> GetObjetosConectados ( TCMatriz3D<T> * pmOrig);

  /// Verifica se a imagem possui conectividade no eixo Y.
	bool isConnected ( TCImagem3D<T> * pmOrig);

  /// Verifica se a matriz possui conectividade no eixo Y.
	bool isConnected ( TCMatriz3D<T> * pmOrig);

  /// Retorna uma nova imagem conectada de acordo com o indice e com o valor de fundo informados.
	TCImagem3D<T> * GetImagemConectada ( TCImagem3D<T> * pmOrig);

  /// Retorna uma nova matriz conectada de acordo com o indice e com o valor de fundo informados.
	TCMatriz3D<T> * GetMatrizConectada ( TCMatriz3D<T> * pmOrig);

  /// Altera Imagem 3D retornando Imagem 3D conectada.
	bool Go (TCImagem3D<T> * pmCon);

  /// Altera Matriz 3D retornando matriz 3D conectada.
	bool Go (TCMatriz3D<T> * pmCon);

};

#include <Filtro/FEspacial3D/FEConectividade3D/TCFEConectividade3D.cpp>

#endif
