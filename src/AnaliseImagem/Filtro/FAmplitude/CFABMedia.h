// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFABMedia_h
#define CFABMedia_h

#ifndef CFABinario_h
#include <AnaliseImagem/Filtro/FAmplitude/CFABinario.h>
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
Nome deste arquivo:	CFABMedia.h       (thrmean)
Nome da classe:      CFABMedia
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Filtro de amplitude, realiza a binarizacao usando um valor de corte médio.
 *
 * É um filtro que atua sobre o a amplitude da imagem, ou seja
 * sobre o espaço de cor da imagem.
 *
 * O CFABMedia realiza a binarizacao usando um valor de corte médio.
 * A função Go calcula o nível de corte médio, e depois solicita a classe
 * base que execute a binarização em só.
 * Se o valor de cor do pixel for maior que o nivel de corte, assume o valor 1
 * caso contrário assume valor  0.
*/
template<typename T>
class CFABMedia : public CFABinario<T>
{
	public:
		/// Construtor
		CFABMedia (TCMatriz2D<T> * &_pm) : CFABinario<T> (_pm) {
		}

		/// Destrutor
		~CFABMedia () {
		}

		/// Realiza o processamento da filtragem
		virtual TCMatriz2D<T> *Go (TCMatriz2D<T> * &matriz, unsigned int _tamanhoMascara = 0);

};
#endif //  CFABMedia_h
