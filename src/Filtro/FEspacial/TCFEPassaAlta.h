#ifndef TCFEPassaAlta_h
#define TCFEPassaAlta_h

// inclue a biblioteca de filtros
#ifndef CFEspacial_h
#include <Filtro/FEspacial/TCFEspacial.h>
#endif

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFEPassaAlta.h
Nome da classe:      TCFEPassaAlta
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Implementa o filtro TCFEPassaAlta.
 * Herdeiro de CFiltroEspacial.
 * Cria e utiliza uma máscara passa alta para atuar sobre a imagem.
 * O resultado geral é o aumento dos contornos. A identificação dos contornos.
 */
template<typename T>
class TCFEPassaAlta : public TCFEspacial<T>
{
	public:
		/// Construtor
		TCFEPassaAlta (TCMatriz2D<T> * &matriz, unsigned int _tamanhoMascara)
			: TCFEspacial<T> (matriz, _tamanhoMascara) {
		}

		/// Cria a mascara adequada
		virtual void CriaMascara (unsigned int _tamanhoMascara);

};

#include "Filtro/FEspacial/TCFEPassaAlta.cpp"

#endif //  TCFEPassaAlta_h
