// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef TCFEMMIDFdijk3D_h
#define TCFEMMIDFdijk3D_h

/*
----------------------------------------------------------------------------
PROJETO:		Anaimp
	 Analise de Imagens de Meios Porosos
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong, Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEMMIDFdijk3D.h
Nome da classe:      TCFEMMIDFdijk3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/
/*
==================================================================================
Documentacao CLASSE:		TCFEMMIDFdijk3D
==================================================================================
Nome Classe:         TCFEMMIDFdijk3D
Assunto:             Matriz IDF de uma imagem usando mascara de chanfro d34
Superclasse:         TCFEMMIDFdijk3D
Descrição:

Acesso:              import
Cardinalidade:
Concorrência:
Transformacoes:
Especificacoes da linguagem:
Persistencia:
Tamanho:
Abstrata/Concreta:   Concreta 
Arquivo de documentacao auxiliar:
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------

#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDF3D.h>
#include <Mascara/MCDiscreta/CMCdijk.h>

// ----------------------------------------------------------------------------
// Classe:       TCFEMMIDFdijk3D
// ----------------------------------------------------------------------------
/**
 * @brief  Matriz IDF de uma imagem usando mascara de chanfro dijk.
 */
template<typename T>
class TCFEMMIDFdijk3D : public TCFEMMIDF3D<T>, public CMCdijk
{
	protected:
		virtual void CriaMascara (unsigned int _tamanhoMascara);	// redefinida

		virtual unsigned int Mi () {
			return mi; // redefinida, herdada de CFEMMIDF3D
		}

	public:
		/// Construtor:
		/// Recebe ponteiro para matriz imagem, métrica da mascara a ser criada, raio máximo a ser usado, identificação de índice e fundo
		TCFEMMIDFdijk3D (TCMatriz3D<T> * &matriz, unsigned int _mi, unsigned int _mj, unsigned int _mk, unsigned int _rb, int _indice=1, int _fundo=0)
			: TCFEMMIDF3D<T> (matriz, 3, 32000, _indice, _fundo), CMCdijk (_mi, _mj, _mk, _rb) {
		}

		/// Destrutor
		virtual ~ TCFEMMIDFdijk3D () {
		}

		virtual TCMatriz3D<T> *Go (TCMatriz3D<T> * &matriz, unsigned int _tamanhoMascara = 0);
		// redefinida

		/// Corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.
		virtual void CorrigeAbertura (TCMatriz3D<T> * &matriz, int &regiao) = 0;


};

#include "Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFdijk3D.cpp"

#endif // CFEMMIDFd345_h
