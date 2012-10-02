//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMMIDFEuclidiana_h
#define CFEMMIDFEuclidiana_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades 
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFEuclidiana.h
Nome da classe:      CFEMMIDFEuclidiana
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao CLASSE:		CFEMMIDFEuclidiana
==================================================================================
Nome Classe:         CFEMMIDFEuclidiana
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d34
Superclasse:         CFEMMIDFEuclidiana
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

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <Base/CMath.h>
#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFdij.h>

//  ----------------------------------------------------------------------------
//  Classe:       CFEMMIDFEuclidiana
//  ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem usando mascara de chanfro pseudo euclidiana.
 */
template<typename T>
class CFEMMIDFEuclidiana : public CFEMMIDFdij<T>
{
	public:
		/// Construtor
		CFEMMIDFEuclidiana (TCMatriz2D<T> * &matriz, int _indice=1, int _fundo=0)
			: CFEMMIDFdij<T> (matriz, 100, (int)CMath::MSQRT2 * 100, 100, _indice, _fundo) {
		}

		///  Destrutor
		virtual ~ CFEMMIDFEuclidiana () {
		}

	public:
		//  virtual TCMatriz2D< int > * Go( TCMatriz2D< int > *& matriz,unsigned int _tamanhoMascara=0);
		//  virtual TCMatriz2D< int >*  Abertura(TCMatriz2D< int >*& matriz,unsigned int _RaioBola);    //
		//  virtual TCMatriz2D< int >*  Erosao(TCMatriz2D< int >*& matriz,unsigned int _RaioBola);    //
		//  virtual TCMatriz2D< int >*  Dilatacao(TCMatriz2D< int >*& matriz,unsigned int _RaioBola); //
		//  virtual TCMatriz2D< int >*  Fechamento(TCMatriz2D< int >*& matriz,unsigned int _RaioBola);//

	protected:
		/// Cria mascara. //  Cria bola  euclidiana?.
		virtual void CriaMascara (unsigned int _tamanhoMascara);

};

#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFEuclidiana.cpp"

#endif //  CFEMMIDFEuclidiana_h
