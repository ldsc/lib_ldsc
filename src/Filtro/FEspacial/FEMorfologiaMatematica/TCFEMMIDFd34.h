//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef TCFEMMIDFd34_h
#define TCFEMMIDFd34_h

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
Nome deste arquivo:	TCFEMMIDFd34.h
Nome da classe:      TCFEMMIDFd34
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFdij.h>

//  ----------------------------------------------------------------------------
//  Classe:       TCFEMMIDFd34
//  ----------------------------------------------------------------------------

/**
 * @brief Matriz IDF de uma imagem usando mascara de chanfro d34.
 *
 */
template<typename T>
class TCFEMMIDFd34  : public TCFEMMIDFdij<T>
{
	public:
		/// Construtor
		TCFEMMIDFd34 (TCMatriz2D<T> * &matriz, int _indice=1, int _fundo=0)
			: TCFEMMIDFdij<T> (matriz, 3, 4, 1, _indice, _fundo) {
		}

		///  Destrutor
		virtual ~ TCFEMMIDFd34 () {
		}

		///  Corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.
		virtual void CorrigeAbertura ( TCMatriz2D<T> * &matriz, int &regiao );

};

#include "Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd34.cpp"

#endif //  TCFEMMIDFd34_h
