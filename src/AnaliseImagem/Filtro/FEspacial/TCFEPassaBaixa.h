// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef TCFEPassaBaixa_h
#define TCFEPassaBaixa_h

// inclue a biblioteca de filtros
#ifndef CFEspacial_h
#include <AnaliseImagem/Filtro/FEspacial/TCFEspacial.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
 Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, 
Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEPassaBaixa.h
Nome da classe:      TCFEPassaBaixa
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Implementa o filtro passa baixa, TCFEPassaBaixa.
 * Herdeiro de CFiltroEspacial.
 *
 * Cria e utiliza uma máscara passa baixa para atuar sobre a imagem.
 * O resultado geral é a atenuação dos contornos, pois a mascara é formada
 * por números 1, e o resultado é uma média dos píxel's vizinhos.
 */
template<typename T>
class TCFEPassaBaixa : public TCFEspacial<T>
{
	public:
		/// Construtor
		TCFEPassaBaixa (TCMatriz2D<T> * &matriz, unsigned int _tamanhoMascara)
			: TCFEspacial<T> (matriz,  _tamanhoMascara) {
		}

		/// Cria a mascara adequada
		virtual void CriaMascara (unsigned int _tamanhoMascara);
};

#include "AnaliseImagem/Filtro/FEspacial/TCFEPassaBaixa.cpp"

#endif // TCFEPassaBaixa_h
