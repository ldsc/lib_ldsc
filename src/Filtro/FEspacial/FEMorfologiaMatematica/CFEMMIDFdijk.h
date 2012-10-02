//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFEMMIDFdijk_h
#define CFEMMIDFdijk_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
			Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFEMMIDFdijk.h
Nome da classe:      CFEMMIDFdijk
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------

#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDF.h>
#include <Mascara/MCDiscreta/CMCdijk.h>

//  ----------------------------------------------------------------------------
//  Classe:       CFEMMIDFdijk
//  ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem usando mascara de chanfro dijk.
 */
template<typename T>
class CFEMMIDFdijk : public CFEMMIDF<T>, public CMCdijk
{
	protected:
		///  Redefinida, herdada de CFEMMIDF
		virtual unsigned int Mi () {
			return mi;
		}

	protected:
		///  Redefinida, cria a mascara
		virtual void CriaMascara (unsigned int _tamanhoMascara);

	public:
		/// Construtor
		CFEMMIDFdijk (TCMatriz2D<T> * &matriz, unsigned int _mi, unsigned int _mj, unsigned int _mk, unsigned int _rb, int _indice=1, int _fundo=0)
			: CFEMMIDF<T> (matriz, _rb, 32000, _indice, _fundo), CMCdijk (_mi, _mj, _mk, _rb) {
		}

		///  Destrutor
		virtual ~ CFEMMIDFdijk () {
		}

		/// Processa a idf.
		virtual TCMatriz2D<T> *Go (TCMatriz2D<T> * &matriz, unsigned int _tamanhoMascara = 0);

};

#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFdijk.cpp"

#endif //  CFEMMIDFdijk_h
