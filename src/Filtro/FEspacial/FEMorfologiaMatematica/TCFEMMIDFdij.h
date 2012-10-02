//  ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef TCFEMMIDFdij_h
#define TCFEMMIDFdij_h


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
Nome deste arquivo:	TCFEMMIDFdij.h
Nome da classe:      TCFEMMIDFdij
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
//  ----------------------------------------------------------------------------
//  Bibliotecas
//  ----------------------------------------------------------------------------
#include <Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDF.h>
#include <Mascara/MCDiscreta/CMCdij.h>

//  ----------------------------------------------------------------------------
//  Classe:       TCFEMMIDFdij
//  ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem usando mascara de chanfro dij.
 */
template<typename T>
class TCFEMMIDFdij : public TCFEMMIDF<T>, public CMCdij
{
	protected:
		virtual void CriaMascara (int _tamanhoMascara);

	public:
		///  Retorna mi (Redefinida, herdada de TCFEMMIDF).
		virtual unsigned int Mi () {
			return mi;
		}

		/// Construtor
		TCFEMMIDFdij (TCMatriz2D<T> * &matriz, unsigned int _mi, unsigned int _mj, unsigned int _rb, int _indice=1, int _fundo=0)
			: TCFEMMIDF<T> (matriz, 3, 32000, _indice, _fundo), CMCdij (_mi, _mj, _rb) {
		}

		///  Destrutor
		virtual ~ TCFEMMIDFdij () {
		}

		/// Processa determinação da idf.
		virtual TCMatriz2D<T> *Go (TCMatriz2D<T> * &matriz, unsigned int _tamanhoMascara = 0);

		///  Corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.
		virtual void CorrigeAbertura ( TCMatriz2D<T> * &matriz, int &regiao ) = 0;

};

#include "Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDFdij.cpp"

#endif //  TCFEMMIDFdij.h
