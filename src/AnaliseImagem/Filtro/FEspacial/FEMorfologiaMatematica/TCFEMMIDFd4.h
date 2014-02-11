//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef TCFEMMIDFd4_h
#define TCFEMMIDFd4_h

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
Nome deste arquivo:	TCFEMMIDFd4.h
Nome da classe:      TCFEMMIDFd4
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFdi.h>

//  ----------------------------------------------------------------------------
//  Classe:       TCFEMMIDFd4
//  ----------------------------------------------------------------------------
/**
 * @brief            Matriz IDF de uma imagem usando mascara de chanfro d4.
 */
template<typename T>
class TCFEMMIDFd4 : public TCFEMMIDFdi<T>
{
	public:
		/// Construtor
		TCFEMMIDFd4 (TCMatriz2D<T> * &matriz, int _indice=1, int _fundo=0)
			: TCFEMMIDFdi<T> (matriz, 1, 1, _indice, _fundo) {
		}

		///  Destrutor
		virtual ~ TCFEMMIDFd4 () {
		}

		///  Corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.
		virtual void CorrigeAbertura ( TCMatriz2D<T> * &matriz, int &regiao );

};

#include "AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFd4.cpp"

#endif //  TCFEMMIDFd4_h
