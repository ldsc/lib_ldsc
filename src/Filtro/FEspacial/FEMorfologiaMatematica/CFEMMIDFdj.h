//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMMIDFdj_h
#define CFEMMIDFdj_h

/*
----------------------------------------------------------------------------
PROJETO:	Anaimp
	Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFdj.h
Nome da classe:      CFEMMIDFdj
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDF.h>
#include <Mascara/MCDiscreta/CMCdi.h>
//Geometria/Bola/BCDiscreta/CBCdi.h

//  ----------------------------------------------------------------------------
//  Classe:       CFEMMIDFdj
//  ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem usando mascara de chanfro dj.
 */
template<typename T>
class CFEMMIDFdj : public CFEMMIDF<T>, public CMCdi
{
	protected:
		///  Redefinida, herdada de CFEMMIDF
		virtual unsigned int Mi () {
			return mi;
		}

	public:
		/// Construtor
		CFEMMIDFdj (TCMatriz2D<T> * &matriz, unsigned int _mj,	unsigned int _rb, int _indice=1, int _fundo=0)
			: CFEMMIDF<T> (matriz, _rb, 32000, _indice, _fundo), CMCdi (_mj, _rb) {
		}

		///  Destrutor
		virtual ~ CFEMMIDFdj () {
		}

		///  Redefinida, cria a mascara.
		virtual void CriaMascara (unsigned int _tamanhoMascara);

		/// Processa idf.
		virtual TCMatriz2D<T> *Go (TCMatriz2D<T> * &matriz, unsigned int _tamanhoMascara = 0);
};

#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFdj.cpp"

#endif //  CFEMMIDFdj_h
