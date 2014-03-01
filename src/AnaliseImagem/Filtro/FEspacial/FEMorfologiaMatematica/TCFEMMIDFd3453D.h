#ifndef TCFEMMIDFd3453D_h
#define TCFEMMIDFd3453D_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEMMIDFd3453D.h
Nome da classe:      TCFEMMIDFd3453D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao Classe:		TCFEMMIDFd3453D
==================================================================================
Nome Classe:         TCFEMMIDFd3453D
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d34
Superclasse:         TCFEMMIDFd3453D
Descrição:

Acesso:              import
Cardinalidade:
Concorrência:
Transformacoes:
Especificacoes da linguagem:
Persistencia:
Tamanho:
Abstrata/Concreta:   Concreta 
Arquivo de documentacao auxiliar:
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------

#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFdijk3D.h>

// ----------------------------------------------------------------------------
// Classe:       TCFEMMIDFd3453D
// ----------------------------------------------------------------------------
/**
 * @brief Matriz IDF de uma imagem usando mascara de chanfro d345.
 *
 */
template<typename T>
class TCFEMMIDFd3453D : public TCFEMMIDFdijk3D<T>
{
public:
  /// Construtor:
  // ponteiro para matriz imagem, tamanho da mascara a ser criada,raio máximo a ser usado
	TCFEMMIDFd3453D (TCMatriz3D<T> * &matriz, int _indice=1, int _fundo=0)
		: TCFEMMIDFdijk3D<T> (matriz, 3, 4, 5, 1, _indice, _fundo) {
  }

	virtual ~ TCFEMMIDFd3453D () {
  }				// Destrutor

  // virtual TCMatriz3D<int> * Go( TCMatriz3D<int> *& matriz,unsigned int _tamanhoMascara=0);// redefinida
	/// Corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.
	virtual void CorrigeAbertura (TCMatriz3D<T> * &matriz, int &regiao);


};

#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd3453D.cpp>

#endif
