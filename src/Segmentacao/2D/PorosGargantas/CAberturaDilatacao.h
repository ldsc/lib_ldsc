#ifndef CAberturaDilatacao_H
#define CAberturaDilatacao_H

#include <Filtro/FEspacial/FEMorfologiaMatematica/TCFEMorfologiaMatematica.h>
#include <Rotulador/TCRotulador2D.h>
#include <Segmentacao/2D/PorosGargantas/CObjetoImagem.h> // novo, repreenta objeto da imagem
#include <Matriz/TCMatriz2D.h>
#include <Matriz/CVetor.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

using namespace std;

/**
 * Classe para determinacao da distribuicao de sítios e ligacoes usando método da abertura-dilatacao.
 * Substituído rotulador interno pela classe CRotulador2D (otimizada).
	@author André Duarte Bueno <bueno@lenep.uenf.br>
	@author Rafael Borgate <borgate@gmail.com>
	@author Leandro Puerari <puerari@gmail.com>
*/
class CAberturaDilatacao
{
	protected:
		/// Ponteiro para objeto filtro morfologia matematica
		TCFEMorfologiaMatematica<int>* pfmf;

		/// Vetor distribuicao total = distTotal = distPoros + distLigacoes
		CVetor* distribuicaoTotalPoros; // usado pelo modelo 0

		/// Vetor dist. ligacoes
		CVetor* distribuicaoLigacoes; // usado pelo modelo 0

		/// vetor dist. poros
		CVetor* distribuicaoSitios; // usado pelo modelo 0

		/// Matriz original (ponteiro para matriz original)
		TCMatriz2D<int>* pm;

		/// Matriz rotulada
		TCRotulador2D<int>* matrizRotulo;

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
		static bool salvarResultadosParciais ;

	public:
		/// Construtor
		CAberturaDilatacao();

		/// Construtor sobrecarregado
		CAberturaDilatacao(TCMatriz2D< int >* &matriz, std::string _nomeImagem = "", int _indice=1, int _fundo=0);

		/// Destrutor
		~CAberturaDilatacao();

		/// Calculo da porosidade
		double Porosidade(TCMatriz2D< int >*& pm);

		/// Determina distribuicao Total de Poros (método normal)
		void DistTotalPoros();

		/// Determina distribuicao de sitios e ligacoes (método novo) modelo 0 (Anaimp)
		void DistSitiosLigacoes_Modelo_0();

		/// Determina distribuicao de sitios e ligacoes (método novo, modelo 1)
		// void DistSitiosLigacoes_Modelo_1_old_usaCVetor(); // Usa CVetor
		void DistSitiosLigacoes_Modelo_1();               // Usa vector

		/// Determina distribuicao de sitios e ligacoes (método novo, modelo 2)
		void DistSitiosLigacoes_Modelo_2();

		/// Determina distribuicao de sitios e ligacoes (método novo, modelo 3)
		void DistSitiosLigacoes_Modelo_3();

		/// Determina distribuicao de sitios e ligacoes (método novo, modelo 4)
		void DistSitiosLigacoes_Modelo_4();

		/// Salva vetor em disco (já tem Write?)
		void Salvar(CVetor* &v, std::string nomeArquivo);
		void Salvar(std::vector<double> v, std::string nomeArquivo);

		void SequenciaAberturaTonsCinza();

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
};

#endif
