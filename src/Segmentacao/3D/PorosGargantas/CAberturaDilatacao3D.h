#ifndef CAberturaDilatacao3D_H
#define CAberturaDilatacao3D_H

#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMorfologiaMatematica3D.h>
#include <Matriz/TCMatriz3D.h>
#include <Matriz/CVetor.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

using namespace std;

/**
 * Classe para determinacao da distribuicao de sítios e ligacoes usando método da abertura-dilatacao.
	@author André Duarte Bueno <bueno@lenep.uenf.br>
	@author Leandro Puerari <puerari@gmail.com>
*/
class CAberturaDilatacao3D {
	protected:
		/// Ponteiro para objeto filtro morfologia matematica
		TCFEMorfologiaMatematica3D<int>* pfmf;

		/// Vetor distribuicao total = distTotal = distPoros + distLigacoes
		CVetor* distribuicaoTotalPoros;

		/// Vetor dist. ligacoes
		CVetor* distribuicaoLigacoes;

		/// vetor dist. poros
		CVetor* distribuicaoSitios;

		/// Matriz original (ponteiro para matriz original)
		TCMatriz3D<int>* pm;

		/// Matriz rotulada
		TCMatriz3D<int>* matrizRotulo;

		/// Numero de objetos identificados na última rotulagem (Não é acumulado)
		int numeroObjetos;

		/// Vetor área dos objetos
		CVetor* areaObjetos;

		/// Vetor perimetro dos objetos
		CVetor* perimetroObjetos;

		/// Vetor raio hidraulico dos objetos
		CVetor* raioHidraulicoObjetos;

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

		/// Se ativo salva os resultados parciais
		static bool salvarResultadosParciais;

	public:
		/// Construtor
		CAberturaDilatacao3D();

		/// Construtor sobrecarregado
		CAberturaDilatacao3D(TCMatriz3D<int>* &matriz, std::string _nomeImagem = "");

		/// Destrutor
		~CAberturaDilatacao3D();

		/// Calculo da porosidade
		double Porosidade(TCMatriz3D<int>*& pm);

		/// Rotula a imagem
		void RotulaImagem();

		/// Rotula a imagem de forma sequencial  ( os rotulos ficarão sequênciais e iniciarão em rotuloInicial )
		// Normalmente rotuloInicial = 0, usase- um rotulo inicial diferente de zero quando
		// deseja-se trabalhar com multiplas rotulagens sobre uma mesma imagem.
		void RotulaImagemSequencial( int rotuloInicial , int FUNDO = 0 );

		/// Determina distribuicao Total de Poros (método normal)
		void DistTotalPoros();

		/// Determina distribuicao de sitios e ligacoes (método novo) modelo 0 (Anaimp)
		void DistSitiosLigacoes_Modelo_0();

		/// Determina distribuicao de sitios e ligacoes (método novo, modelo 1)
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
