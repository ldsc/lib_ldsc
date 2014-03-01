// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef TCFEMMIDFEuclidiana3D_h
#define TCFEMMIDFEuclidiana3D_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEMMIDFEuclidiana3D.h
Nome da classe:      TCFEMMIDFEuclidiana3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
==================================================================================
Documentacao Classe:		TCFEMMIDFEuclidiana3D
==================================================================================
Nome Classe:         TCFEMMIDFEuclidiana3D
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d34
Superclasse:         TCFEMMIDFEuclidiana3D
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

#include <Base/CMath.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFdijk3D.h>

// ----------------------------------------------------------------------------
// Classe:       TCFEMMIDFEuclidiana3D
// ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem 3D usando mascara de chanfro pseudo euclidiana.
 */
template<typename T>
class TCFEMMIDFEuclidiana3D : public TCFEMMIDFdijk3D<T>
{
public:
	TCFEMMIDFEuclidiana3D (TCMatriz3D<T> * &matriz, int _indice=1, int _fundo=0)
		: TCFEMMIDFdijk3D<T> (matriz, 100, (int)CMath::MSQRT2 * 100, (int)CMath::MSQRT3 * 100, 100, _indice, _fundo) {
  }

	virtual ~ TCFEMMIDFEuclidiana3D ()
  {
  }

	/// Corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.
	virtual void CorrigeAbertura (TCMatriz3D<T> * &matriz, int &regiao);

protected:
  /// Cria mascara CBCEuclidiana3D.
  virtual void CriaMascara (unsigned int _tamanhoMascara);
};

#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFEuclidiana3D.cpp>

#endif // TCFEMMIDFEuclidiana3D_h
