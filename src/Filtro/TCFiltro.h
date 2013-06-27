// ítens conferidos: 1[] 2[] 3[] 4[] 5[] 6[] 7[] 8[] 9[] 10[]
#ifndef TCFiltro_h
#define TCFiltro_h

// ponteiro para matriz
#include <Matriz/TCMatriz2D.h>

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico e Propriedades
Termofisicas  dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez,
Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	TCFiltro.h
Nome da classe:      TCFiltro
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief            Classe básica para os filtros.
 * Representa a superclasse para os filtros que podem atuar sobre
 * o espaço discreto da imagem (filtros espaciais) ou filtros que atuam
 * sobre o espaço de cor das imagens (filtros de amplitude).
 * Tem um atributo básico que é um ponteiro para a imagem (matriz imagem)
 * a ser tratada.
 * Ou seja um filtro tem um ponteiro para a imagem podendo atuar sobre éla.
 * Cardinalidade:    1:1    ,de uma maneira geral usa-se um filtro para atuar sobre uma imagem
 *
 * Tarefas de um filtro:
 * -Funcao CanGo () informa se a operacao pode ser realizada
 * -Funcao Go realiza a filtragem da imagem
*/
template<typename T>
class TCFiltro {
	protected:
		/// Ponteiro para a imagem a ser filtrada
		TCMatriz2D< T > * pm;

	public:
		/// Construtor, recebe o endereço da matriz a ser processada
		TCFiltro (TCMatriz2D<T> * &matriz) {
			pm = matriz;
		}

		/// Destrutor
		virtual ~TCFiltro ()	{
		}

		/**
		 * @brief Realiza o processamento da filtragem
		 * @param matriz ponteiro para imagem a ser processada
		 * @param _tamanhoMascara  dimensão da mascara a ser utilizada
		 * @return ponteiro para imagem processada
		 */
		virtual TCMatriz2D< T > *Go (TCMatriz2D< T > * &matriz, unsigned int _tamanhoMascara = 0) = 0;
};

#include "Filtro/TCFiltro.cpp"

#endif //   TCFiltro_h
