#ifndef CSegPorosGargantas3D_H
#define CSegPorosGargantas3D_H

#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIRA3D.h>
#include <Rotulador/CRotuladorIRA3D.h>
#include <Segmentacao/CObjetoImagem.h>
#include <Matriz/TCImagem3D.h>
#include <Matriz/CVetor.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>

using namespace std;

/**
 * Classe para determinacao da distribuicao de poros e gargantas (sítios e ligacoes) usando método da abertura através da IRA.
 * @author Leandro Puerari <puerari@gmail.com>
 * @author André Duarte Bueno <bueno@lenep.uenf.br>
*/
class CSegPorosGargantas3D
{
	protected:
		// // Ponteiro para objeto filtro IRA
		//TCFEMMIRA3D<bool>* pfira;

		/// Ponteiro para matriz IRA
		TCMatriz3D<int>* pmira;

		/// Matriz original (ponteiro para matriz original)
		TCMatriz3D<bool>* pm;

		/// Ponteiro para objeto rotulador o qual herda TCRotulador3D<T> que por sua vez herda TCMatriz3D<int> para representar a matriz rotulada.
		CRotuladorIRA3D* matrizRotulo;

		/// Porosidade
		double porosidade;

		/// Nome da imagem que esta processando (usado para salvar resultado final)
		std::string nomeImagem;

		/// fator usado como critério de parada
		int raioMaximoElementoEstruturante;

		/// fator usado para reducao do Raio do Elemento Estruturante no processamento da dilatacao.
		int fatorReducaoRaioElemEst;

		/// Valor do incremento do raio do elemento estruturante
		int incrementoRaioElementoEstruturante;

		/// Numero do modelo de calculo
		int modelo;

		/// Valor que indentifica os objetos de interesse na imagem
		int INDICE;

		/// Valor que indentifica o fundo da imagem
		int FUNDO;

		/// Se ativo salva os resultados parciais
		static bool salvarResultadosParciais;

	public:
		/// Construtor
		CSegPorosGargantas3D(TCMatriz3D<bool>* &matriz, std::string _nomeImagem = "", int _indice=1, int _fundo=0);

		/// Construtor
		CSegPorosGargantas3D(TCImagem3D<bool>* &matriz, std::string _nomeImagem = "", int _indice=1, int _fundo=0);

		/// Destrutor
		~CSegPorosGargantas3D();

		/// Calculo da porosidade
		double Porosidade(TCMatriz3D<int> *&pm, int _ra);

		/// Retorna o raio Maximo do Elemento Estruturante
		int RaioMaximoElementoEstruturante () {
			return  raioMaximoElementoEstruturante ;
		}

		/// Seta o raioMaximoElementoEstruturante  - o raio Maximo do Elemento Estruturante
		void RaioMaximoElementoEstruturante ( int _rmee) {
			raioMaximoElementoEstruturante = _rmee;
		}

		/// Retorna o fatorReducaoRaioElemEst
		int FatorReducaoRaioElemEst() {
			return  fatorReducaoRaioElemEst;
		}

		/// Seta o fatorReducaoRaioElemEst ( ex: =2, significa que a dilatacao vai ser realizada com raioElemen = raioElemen / 2 )
		void FatorReducaoRaioElemEst( int _frree) {
			fatorReducaoRaioElemEst = _frree;
		}

		/// Retorna o incrementoRaioElementoEstruturante
		int IncrementoRaioElementoEstruturante () {
			return  incrementoRaioElementoEstruturante;
		}

		/// Seta o incrementoRaioElementoEstruturante
		void IncrementoRaioElementoEstruturante ( int _iree) {
			incrementoRaioElementoEstruturante = _iree;
		}

		/// Retorna flag salvarResultadosParciais
		static bool SalvarResultadosParciais( ) {
			return salvarResultadosParciais;
		}

		/// Seta flag salvarResultadosParciais
		static void SalvarResultadosParciais( bool b ) {
			salvarResultadosParciais = b;
		}

		/** @brief Recebe nome do arquivo e ponteiro para duas matrizes do tipo bool.
		*		Salva a mesclagem das matrizes em disco de forma que 0 será o fundo, 1 serão os índices da primeira matriz e 2 serão os índices da segunda matriz.
		*		Se a possição dos índices coincidirem, o indice da última matriz informada como parâmetro será considerado.
		*/
		bool Write(string fileName, TCMatriz3D<bool>* &mat1, TCMatriz3D<bool>* &mat2 );

		/// Método que executa a segmentação de poros e gargantas de acordo com o modelo informado.
		pair< TCMatriz3D<bool> *, TCMatriz3D<bool> * > Go(int _modelo);

	private:
		/// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz bool informada como parametro.
		void SalvarResultadosParciaisEmDisco(TCMatriz3D<bool>* &mat, string fileName);

		/// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz int informada como parametro.
		void SalvarResultadosParciaisEmDisco(TCMatriz3D<int>* &mat, string fileName);

		/// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz rotulada informada como parametro.
		void SalvarResultadosParciaisEmDisco(CRotuladorIRA3D *&mat, string fileName);

		/// Determina distribuicao de sitios e ligacoes (Modelo 0)
		pair< TCMatriz3D<bool> *, TCMatriz3D<bool> * > Modelo_0();

};

#endif
