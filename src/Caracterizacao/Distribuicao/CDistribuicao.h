/* ----------------------------------------------------------------------------
 * PROJETO:	LIB_LDSC
 * ----------------------------------------------------------------------------
 * Desenvolvido por:Laboratorio de Desenvolvimento de Software Cientifico - LENEP-CCT-UENF
 * Programadores: Leandro Puerari, Andre Duarte Bueno.
 * Copyright @2011:    Todos os direitos reservados.
 * Nome deste arquivo: CDistribuicao.h
 * Nome da classe:     CDistribuicao
 * --------------------------------------------------------------------------*/

#ifndef CDistribuicao_h
#define CDistribuicao_h

#include <Matriz/TCMatriz2D.h>
#include <Caracterizacao/Distribuicao/CBaseDistribuicao.h>

/**
 * @brief Declara a classe CDistribuicao, herdeira de CBaseDistribuicao.
 * Esta classe permite determinar a distribuição de tamanho dos objetos
 * (poros) em uma imagem 2D.
 *
 * @author Leandro Puerari <puerari@gmail.com>
 * @author André Duarte Bueno <andreduartebueno@gmail.com>
*/

using namespace std;

class CDistribuicao : public CBaseDistribuicao
{
		//Atributos
	public:
		/// Enumera os tipos de métricas que podem ser utilizadas para executar a operação abertura no cálculo da distribuição em imagens 2D.
		enum Metrica {
			espacial,
			d34,
			d5711,
			euclidiana
		};
	private:
		/// Ponteiro para matriz bidimensional (imagem)
		TCMatriz2D<bool> * pm;

		// Construtores / Destrutor
	public:
		/// Construtor default
		CDistribuicao () : CBaseDistribuicao( ), pm( NULL ) {
		}

		/// Construtor (recebe TCMatriz2D< int >)
		CDistribuicao ( TCMatriz2D<bool> * _pm ) : CBaseDistribuicao( ), pm(_pm) {
		}

		/// Destrutor
		~ CDistribuicao () {}

		// Métodos
	public:
		/// Calcula a distribuição de tamanho dos objetos em uma imagem 2D binária. Recebe como parâmetros: Tipo, Metrica, indice e fundo.
		bool Go( Tipos _tipo=dts, Metrica _metrica=d34, int indice=1, int fundo=0 );

		/// Calcula a distribuição de tamanho dos objetos em uma imagem 2D binária. Recebe como parâmetros: Ponteiro para Matriz 2D, Tipo, Metrica, indice e fundo.
		bool Go( TCMatriz2D<bool> * _pm, Tipos _tipo=dts, Metrica _metrica=d34, int indice=1, int fundo=0 );

};

#endif
