//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMMIDFd8_h
#define CFEMMIDFd8_h

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
Nome deste arquivo:	CFEMMIDFd8.h
Nome da classe:      CFEMMIDFd8
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFdj.h>

//  ----------------------------------------------------------------------------
//  Classe:       CFEMMIDFd8
//  ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem usando mascara de chanfro d8.
 */
template<typename T>
class CFEMMIDFd8 : public CFEMMIDFdj<T>
{
	public:
		/// Construtor
		CFEMMIDFd8 (TCMatriz2D<T> * &matriz, int _indice=1, int _fundo=0)
			: CFEMMIDFdj<T> (matriz, 1, 1, _indice, _fundo) {
		}

		///  Destrutor
		virtual ~ CFEMMIDFd8 () {
		}

};

#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFd8.cpp"

#endif //  CFEMMIDFd8_h
