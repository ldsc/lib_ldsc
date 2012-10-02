#ifndef TCFEspacial3D_h
#define TCFEspacial3D_h

// inclue a biblioteca de filtros
#ifndef TCFiltro3D_h
#include <Filtro/TCFiltro3D.h>
#endif

// inclue a biblioteca de mascaras
#ifndef CMascara3D_h
#include <Mascara/CMascara3D.h>
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
Nome deste arquivo:	TCFEspacial3D.h
Nome da classe:      TCFEspacial3D
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/*
==================================================================================
Documentacao CLASSE:    TCFEspacial3D
==================================================================================
Assunto:             Classe básica para os filtros da morfologia matemática
Superclasse:         TFiltro3D
Descrição:           Representa a superclasse para os filtros que atuam sobre
							o espaço discreto da imagem.
										 Ou seja atua sobre o espaço geométrico da imagem,
										 enquanto os filtros de amplitude atuam sobre o espaço de cor.
										 Tem uma variável inteira que define o tamanho da mascara a ser utilizada
										 e um ponteiro para um objeto da hierarquia das mascaras.
										 Como a classe base TFiltro3D tem um ponteiro para uma matriz imagem, eu tenho
										 acesso a imagem através de pm, e a mascara através de mask.
										 A função cria mascara é utilizada para criação da mascara
										 e a função Go executa o filtro em sí.
Acesso:           import
Cardinalidade:    1:1    ,um filtro uma mascara
Abstrata/Concreta:  concreta
Arquivo de documentacao auxiliar:
*/

/**
 * @brief Classe básica para os filtros espaciais 3D.
 *
 * Representa a superclasse para os filtros que atuam sobre
 * o espaço discreto da imagem 3D.
 *
 * Ou seja atua sobre o espaço geométrico da imagem,
 * enquanto os filtros de amplitude atuam sobre o espaço de cor.
 * em uma variável inteira que define o tamanho da mascara a ser utilizada
 * e um ponteiro para um objeto da hierarquia das mascaras.
 * Como a classe base CFiltro3D tem um ponteiro para uma matriz 3D imagem, eu tenho
 * acesso a imagem através de pm, e a mascara através de mask.
 * A função cria mascara é utilizada para criação da mascara
 * e a função Go executa o filtro em sí.
*/
template <typename T>
class TCFEspacial3D : public TCFiltro3D<T>
{
		// Atributos
	protected:
		unsigned int tamanhoMascara;	/// Tamanho da mascara

	public:
		CMascara3D * mask;			/// Ponteiro para mascara
		// Métodos
	public:
		/// Construtor, recebe ponteiro para imagem3d, e tamanho da mascara.
		TCFEspacial3D (TCMatriz3D<T> * &matriz, unsigned int _tamanhoMascara)	// ponteiro para imagem, e
			: TCFiltro3D<T> (matriz), tamanhoMascara (_tamanhoMascara)	// tamanho da mascara
		{
			mask = NULL;
		}

		/// Destrutor
		~TCFEspacial3D () {
			if (mask)
				delete mask;				// Apaga objeto mask
		}

		/// Obtem tamanhoMascara
		virtual unsigned int TamanhoMascara () const {
			return tamanhoMascara;
		}

		/// Seta o tamanhoMascara
		virtual void TamanhoMascara (unsigned int _tamanhoMascara) {
			tamanhoMascara = _tamanhoMascara;
		}

		/// Realiza o processamento da filtragem
		virtual TCMatriz3D<T> *Go (TCMatriz3D<T> * &matriz, unsigned int _tamanhoMascara = 0);

	protected:
		/// Cria a mascara adequada
		virtual void CriaMascara (unsigned int _tamanhoMascara);	// Cria a mascara adequada

};

#include "Filtro/FEspacial3D/TCFEspacial3D.cpp"

#endif //  TCFEspacial3D_h
