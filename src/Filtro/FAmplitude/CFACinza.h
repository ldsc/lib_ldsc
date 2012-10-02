// ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CFACinza_h
#define CFACinza_h

#ifndef CFiltroAmplitude_h
#include <Filtro/FAmplitude/CFAmplitude.h>
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
Nome deste arquivo:	CFACinza.h
Nome da classe:      CFACinza
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

/**
 * @brief Filtro de amplitude, para conversão de imagens coloridas em tons de cinza.
 *
 * é um filtro que atua sobre o a amplitude da imagem, ou seja
 * sobre o espaço de cor da imagem.
 *
 * Os atributos wr,wg,wb representam os pesos das ponderações a serem
 * utilizadas para as cores vermelho, verde e azul respectivamente.
 * Transforma uma imagem colorida em tons de cinza
 * Usa três pesos para os valores RGB.
 * wr peso para vermelho,  wg peso para verde ,wb=peso para azul
*/
template<typename T>
class CFACinza : public CFAmplitude<T>
{
	private:

		float pesoRed;  ///< Ponderações rgb, red
		float pesoGreen;///< Ponderações rgb, green
		float pesoBlue; ///< Ponderações rgb, blue

	public:
		/// Construtor, define atributos internos
		CFACinza (TCMatriz2D<T> * matriz, float _pesoRed = 0.29, float _pesoGreen = 0.60, float _pesoBlue = 0.11)
			: CFAmplitude<T> (matriz), pesoRed (_pesoRed), pesoGreen (_pesoGreen), pesoBlue (_pesoBlue) {
		}

		/// Destrutor
		~CFACinza () {
		}

		/// Realiza o processamento da filtragem
		virtual TCMatriz2D<T> *Go (TCMatriz2D<T> * &matriz, unsigned int _tamanhoMascara =0);

		/// Retorna pesoRed
		float PesoRed () const {
			return pesoRed;
		}				// funções de obtenção e definição

		/// Seta pesoRed
		void PesoRed (float _pesoRed) {
			pesoRed = _pesoRed;
		}				// dos atributos internos

		/// Retorna pesoGreen
		float PesoGreen () const {
			return pesoGreen;
		}

		/// Seta pesoGreen
		void PesoGreen (float _pesoGreen) {
			pesoGreen = _pesoGreen;
		}

		/// Retorna pesoBlue
		float PesoBlue () const {
			return pesoBlue;
		}

		/// Seta pesoBlue
		void PesoBlue (float _pesoBlue) {
			pesoBlue = _pesoBlue;
		}
};

#endif
