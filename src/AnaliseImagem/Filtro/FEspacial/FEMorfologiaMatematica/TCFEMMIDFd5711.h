//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef TCFEMMIDFd5711_h
#define TCFEMMIDFd5711_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e
Propriedades Termofisicas  dos Materiais.
Programadores: Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,  Liang Zirong,
Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEMMIDFd5711.h
Nome da classe:      TCFEMMIDFd5711
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------

#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFdijk.h>

//  ----------------------------------------------------------------------------
//  Classe:       TCFEMMIDFd5711
//  ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem usando mascara de chanfro d5711.
 */
template<typename T>
class TCFEMMIDFd5711 : public TCFEMMIDFdijk<T>
{
	public:
		/// Construtor
		TCFEMMIDFd5711 (TCMatriz2D<T> * &matriz, int _indice=1, int _fundo=0)
			: TCFEMMIDFdijk<T> (matriz, 5, 7, 11, 2, _indice, _fundo) {
		}

		///  Destrutor
		virtual ~ TCFEMMIDFd5711 () {
		}

		///  Corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.
		virtual void CorrigeAbertura ( TCMatriz2D<T> * &matriz, int &regiao );
};

#include "AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd5711.cpp"

#endif //  TCFEMMIDFd5711_h
