#ifndef CDistribuicaoTamanhoPorosGargantas_H
#define CDistribuicaoTamanhoPorosGargantas_H

#include <AnaliseImagem/Caracterizacao/Distribuicao/CDistribuicao3D.h>
#include <AnaliseImagem/Segmentacao/PorosGargantas/CAberturaDilatacao3D.h>
#include <MetNum/Matriz/TCImagem3D.h>

/**
 * @brief Calcula a distribuição de tamanho de poros e gargantas de imagens binárias ou de imagens em
 * tons de cinza segmentadas pela classe CAberturaDilatacao3D.
 *
 * Se a imagem recebída for uma imagem binária, esta será segmantada através da classe CAberturaDilatacao3D.
 *
 * Utiliza a classe CDistribuicao3D para calcular as distribuições.
 *
 * @author Leandro Puerari <puerari@gmail.com>
 * @author André Duarte Bueno <andreduartebueno@gmail.com>
*/
class CDistribuicaoTamanhoPorosGargantas
{		// Atributos
	private:
		CAberturaDilatacao3D *filtro; ///<Ponteiro para classe que realiza a segmentação de poros e gargantas
		TCMatriz3D<bool> * pms; ///<Ponteiro para a matriz de sítios
		TCMatriz3D<bool> * pml; ///<Ponteiro para a matriz de ligações
		EModelo modelo; ///<Modelo a ser utilizado na segmentação da matriz binária
		int indice;
		int fundo;

		// Construtores / Destrutor
	public:
		/// Construtor (recebe imagem binária que será segmentada)
		CDistribuicaoTamanhoPorosGargantas( TCImagem3D<bool> *&_pm, int & _raioMaximo, int & _raioDilatacao, int & _fatorReducao, int & _incrementoRaio, EModelo _modelo, int _indice=1, int _fundo=0 );

		/// Construtor (recebe imagem em tons de cinza fundo=0; sitio=1, ligação=2)
		CDistribuicaoTamanhoPorosGargantas( TCImagem3D<int> *&_pm );

		/// Destrutor
		~CDistribuicaoTamanhoPorosGargantas();

		// Métodos
	public:
		/// Executa o cálculo das distribuições
		std::pair< CDistribuicao3D*, CDistribuicao3D* > Go(CDistribuicao3D::Metrica3D _metrica = CDistribuicao3D::d345);
};

#endif // CDistribuicaoTamanhoPorosGargantas_H
