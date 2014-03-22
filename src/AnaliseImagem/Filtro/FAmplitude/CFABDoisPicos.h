// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFABDoisPicos_h
#define CFABDoisPicos_h

#ifndef CFABinario_h
#include <AnaliseImagem/Filtro/FAmplitude/CFABinario.h>
#endif

#include <MetNum/Matriz/CHistograma.h>

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
Nome deste arquivo:	CFABDoisPicos.h      (twopeaks)
Nome da classe:      CFABDoisPicos
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/** @brief Filtro de amplitude, método de cálculo baseado na determinação
 * dos dois picos de níveis de cinza na imagem.
 *
 * é um filtro que atua sobre o a amplitude da imagem, ou seja
 * sobre o espaço de cor da imagem.
 *
 * Método de cálculo da posição de corte (threshold)
 * baseado na determinação dos dois picos de níveis de cinza na imagem
 * ideal para imagem com duas regiões de níveis de cinza
 *
*/
template<typename T>
class CFABDoisPicos : public CFABinario<T>
{
	public:

		/// Construtor
		CFABDoisPicos (TCMatriz2D<T> * &_pm) : CFABinario<T> (_pm) {
		}

		/// Destrutor
		~CFABDoisPicos () {
		}

		/// Realiza o processamento da filtragem
		virtual TCMatriz2D< T > *Go (TCMatriz2D< T > * &matriz, unsigned int _tamanhoMascara = 0);

		/// Função que determina o nivel de corte a partir do histograma da imagem
		unsigned int determinaNivelCorte (CHistograma * &hist);

};
#endif //  CFABDoisPicos_h
