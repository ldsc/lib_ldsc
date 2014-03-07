//  itens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:    Laboratorio de Desenvolvimento de Software Cientifico
 dos Materiais.
Programadores:   Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  Todos os direitos reservados.
Nome deste arquivo:TCFEMMIDFdij.cpp
Nome da classe:      TCFEMMIDFdij
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 Implementa a funcao CriaMascara da classe TCFEMMIDFdij.
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
// using namespace std;
#ifndef TCFEMMIDFdij_h
#include "AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFdij.h"
#endif
//  Classe base
#include "AnaliseImagem/Geometria/Bola/BCDiscreta/CBCdij.h"

/*
==================================================================================
Documentacao 		CriaMascara
==================================================================================
Descricao:        Funcao que cria a mascara de chanfro adequada.
	O filtro CFEMMIDF recebe a imagem pm, e o tamanho da mascara.
	E a funcao Go recebe o raio maximo, define a variavel raioMaximo
	e chama cria mascara.
Pre-condicoes:
Excessoes:        tipos de excessoes
Concorrencia:
Tempo processamento(s):
Tamanho(bits):
Comentarios:
Programador:      Andre Duarte Bueno
*/
template<typename T>
void TCFEMMIDFdij<T>::CriaMascara ( int _tamanhoMascara ) {
	if ( this->mask ) {		//  se existe uma mascara
		if ( this->mask->NX () == _tamanhoMascara ) {	//  e o do mesmo  tamanho
			return;			//  sai
		}
		delete this->mask;		//  se  nao é do mesmo tamanho apaga objeto mask
	}				//  e abaixo cria uma nova
	//  se nao existe a mascara, cria uma nova
	this->mask = new CBCdij ( _tamanhoMascara, mi, mj, raioBase );
}

/*
==================================================================================
Documentacao      Go
==================================================================================
Descricao:
//  Substitui o uso da mascara, que usava loop's e indices confusos
//  pelo acesso direto aos valores da mascara.
//  Ao lado do codigo o esboco do ponto da mascara que esta sendo considerado.
//  Da forma como esta o codigo fica mais rapido e compreensivo.
Programador:      Andre Duarte Bueno
*/
template<typename T>
TCMatriz2D<T> * TCFEMMIDFdij<T>::Go ( TCMatriz2D<T> * &matriz, unsigned int /*_tamanhoMascara*/ ) {
	this->ExecutadaPorGo ( matriz );	//  armazena valores da matriz e _tamanhoMascara
	//Após ExecutadaPorGo a imagem this já vem prenchido com 0 (fundo) e mi (sólido)
	int x, y; // Indices para percorrer a matriz

	//  ida   MinimoIda
	for ( y = 1; y < this->ny - 1; y++ ) {	//  NY() e igual a ny, ny da matriz idf
		for ( x = 1; x < this->nx - 1; x++ ) {
			if ( this->data2D[x][y] != 0 ) {	//  Testa a imagem, se nao for solido entra
				this->minimo = this->raioMaximo;
				//  -----------------------------------------------------------
				this->min ( this->data2D[x-1][ y ] + mi );	/*ponto[x][y] */
				this->min ( this->data2D[x-1][y-1] + mj );
				this->min ( this->data2D[ x ][y-1] + mi );
				this->min ( this->data2D[x+1][y-1] + mj );
				//  -----------------------------------------------------------
				this->data2D[x][y] = this->minimo;
			}
		}
	}
	//  volta    MinimoVolta
	for ( y = this->ny - 2; y > 0; y-- ) {	//  -2 pois comeca do zero e a mascara tem tamanho 1
		for ( x = this->nx - 2; x > 0; x-- ) {
			if ( this->data2D[x][y] != 0 ) {	//  Se nao for solido
				this->minimo = this->data2D[x][y];	//  Armazena valor minimo da ida
				//  -----------------------------------------------------------
				this->min ( this->data2D[x-1][y+1] + mj );
				this->min ( this->data2D[ x ][y+1] + mi );
				this->min ( this->data2D[x+1][y+1] + mj );
				this->min ( this->data2D[x+1][ y ] + mi );
				//  -----------------------------------------------------------
				this->data2D[x][y] = this->minimo;
			}
		}
	}
	//Write ("TCFEMMIDFdij-Go.pgm");
	//pm->Write ("pmEmGo.pgm");
	return this->pm;
}
