//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef TCFEMMIDFd8_h
#define TCFEMMIDFd8_h

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
Nome deste arquivo:	TCFEMMIDFd8.h
Nome da classe:      TCFEMMIDFd8
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFdj.h>

//  ----------------------------------------------------------------------------
//  Classe:       TCFEMMIDFd8
//  ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem usando mascara de chanfro d8.
 */
template<typename T>
class TCFEMMIDFd8 : public TCFEMMIDFdj<T>
{
	public:
		/// Construtor
		TCFEMMIDFd8 (TCMatriz2D<T> * &matriz, int _indice=1, int _fundo=0)
			: TCFEMMIDFdj<T> (matriz, 1, 1, _indice, _fundo) {
		}

		///  Destrutor
		virtual ~ TCFEMMIDFd8 () {
		}

		///  Corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.
		virtual void CorrigeAbertura ( TCMatriz2D<T> * &matriz, int &regiao );
};

#include "Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd8.cpp"

#endif //  TCFEMMIDFd8_h
