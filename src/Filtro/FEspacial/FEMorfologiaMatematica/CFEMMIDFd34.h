//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMMIDFd34_h
#define CFEMMIDFd34_h

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
Nome deste arquivo:	CFEMMIDFd34.h
Nome da classe:      CFEMMIDFd34
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFdij.h>

//  ----------------------------------------------------------------------------
//  Classe:       CFEMMIDFd34
//  ----------------------------------------------------------------------------

/**
 * @brief Matriz IDF de uma imagem usando mascara de chanfro d34.
 *
 */
template<typename T>
class CFEMMIDFd34  : public CFEMMIDFdij<T>
{
	public:
		/// Construtor
		CFEMMIDFd34 (TCMatriz2D<T> * &matriz, int _indice=1, int _fundo=0)
			: CFEMMIDFdij<T> (matriz, 3, 4, 1, _indice, _fundo) {
		}

		///  Destrutor
		virtual ~ CFEMMIDFd34 () {
		}

};

#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFd34.cpp"

#endif //  CFEMMIDFd34_h
