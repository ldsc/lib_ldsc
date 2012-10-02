//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMMIDFd4_h
#define CFEMMIDFd4_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
		Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico 
	dos Materiais.
Programadores:    Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, 
Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFd4.h
Nome da classe:      CFEMMIDFd4
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFdi.h>

//  ----------------------------------------------------------------------------
//  Classe:       CFEMMIDFd4
//  ----------------------------------------------------------------------------
/**
 * @brief            Matriz IDF de uma imagem usando mascara de chanfro d4.
 */
template<typename T>
class CFEMMIDFd4 : public CFEMMIDFdi<T>
{
	public:
		/// Construtor
		CFEMMIDFd4 (TCMatriz2D<T> * &matriz, int _indice=1, int _fundo=0)
			: CFEMMIDFdi<T> (matriz, 1, 1, _indice, _fundo) {
		}

		///  Destrutor
		virtual ~ CFEMMIDFd4 () {
		}

};

#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFd4.cpp"

#endif //  CFEMMIDFd4_h
