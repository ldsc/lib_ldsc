// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFABVarianciaOtsu_h
#define CFABVarianciaOtsu_h

#ifndef CFABinario_h
#include <AnaliseImagem/Filtro/FAmplitude/CFABinario.h>
#endif
#include <MetNum/Matriz/CHistograma.h>

/*
----------------------------------------------------------------------------
PROJETO:		Bibliotecas LDSC
			Biblioteca para manipulacao de mascaras
----------------------------------------------------------------------------

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Paulo C. Philippi, Cunha Neto J.A.B.,Nathan Mendes,...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CFABVarianciaOtsu.h   (thrglh)
Nome da classe:      CFABVarianciaOtsu
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Filtros de amplitude, usa método da VarianciaOtsu.
 *
 * é um filtro que atua sobre o a amplitude da imagem, ou seja
 * sobre o espaço de cor da imagem.
 * Otsu's method of 'grey level histograms'
 * Método de Otsu ou Grey Level Histogram (GLH)
 * define duas regiões e calcula a variancia das classes
 * o nível de corte ideal é aquele que minimiza a razão
 * entre as variâncias da classe 1 e 2
 * nivelCorte=varianca1/varianca2
*/
template<typename T>
class CFABVarianciaOtsu : public CFABinario<T> {
	public:
		/// Construtor
		CFABVarianciaOtsu (TCMatriz2D<T> * &_pm) : CFABinario<T> (_pm) {
		}

		/// Destrutor
		~CFABVarianciaOtsu () {
		}

		/// Realiza o processamento da filtragem
		virtual TCMatriz2D<T> *Go (TCMatriz2D<T> * &matriz, unsigned int _tamanhoMascara = 0);

	protected:
		/// Funções auxiliares
		float nu (CHistograma * histAux, int k, float iXdataiTotal,  float varianciaTotal);

		/// Funções auxiliares
		float iXdatai (int *datai, int k);

};
#endif //  CFABVarianciaOtsu_h
