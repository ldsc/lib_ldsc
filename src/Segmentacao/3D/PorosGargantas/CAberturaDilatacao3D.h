#ifndef CAberturaDilatacao3D_H
#define CAberturaDilatacao3D_H

#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFd3453D.h>
#include <Segmentacao/CObjetoImagem.h> // novo, repreenta objeto da imagem
#include <Rotulador/TCRotulador3D.h>
#include <Matriz/TCImagem3D.h>
#include <Matriz/CVetor.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>

using namespace std;

struct Ponto {
		int df; ///<Distância ao fundo.
		int i; ///<Posição do ponto em x.
		int j; ///<Posição do ponto em y.
		int k; ///<Posição do ponto em z.
};

/// Enumera os modelos de segmentação disponíveis
enum EModelo { ZERO=0, UM=1, DOIS=2, TRES=3, QUATRO=4, CINCO=5, SEIS=6, SETE=7, OITO=8 };

/**
 * Classe para determinacao da distribuicao de sítios e ligacoes usando método da abertura-dilatacao.
 * Substituído rotulador interno pela classe CRotulador3D (otimizada).
	@author André Duarte Bueno <bueno@lenep.uenf.br>
	@author Leandro Puerari <puerari@gmail.com>
*/
class CAberturaDilatacao3D
{
	protected:
		/// Ponteiro para objeto filtro morfologia matematica
		TCFEMMIDFd3453D<bool>* pfmf;

		/// Matriz original (ponteiro para matriz original)
		TCMatriz3D<bool>* pm;

		/// Matriz que armazena os sítios identificados
		TCMatriz3D<bool>* matrizSitios;

		/// Matriz que armazena as ligações identificadas
		TCMatriz3D<bool>* matrizLigacoes;

		/// Ponteiro para objeto rotulador o qual herda TCMatriz3D<int> para representar a matriz rotulada.
		TCRotulador3D<bool>* matrizRotulo;

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

		/// Raio do Elemento Estruturante usado na operação de dilatação (Modelo 7)
		int raioEEDilatacao;

		/// Numero do modelo de calculo
		EModelo modelo;

		/// Valor que indentifica os objetos de interesse na imagem
		int INDICE;

		/// Valor que indentifica o fundo da imagem
		int FUNDO;

		/// Se ativo salva os resultados parciais
		static bool salvarResultadosParciais ;

	public:
		/// Construtor
		CAberturaDilatacao3D(TCMatriz3D<bool>* &matriz, std::string _nomeImagem = "", int _indice=1, int _fundo=0);

		/// Construtor
		CAberturaDilatacao3D(TCImagem3D<bool>* &matriz, std::string _nomeImagem = "", int _indice=1, int _fundo=0);

		/// Destrutor
		~CAberturaDilatacao3D();

		/// Salva vetor em disco
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

		/// Retorna o raio do Elemento Estruturante utilizado na Dilatacao
		int RaioEEDilatacao () {
			return  raioEEDilatacao;
		}

		/// Seta o raio do Elemento Estruturante utilizado na Dilatacao
		void RaioEEDilatacao ( int _ree) {
			raioEEDilatacao = _ree;
		}

		/// Retorna flag salvarResultadosParciais
		static bool SalvarResultadosParciais( ) {
			return salvarResultadosParciais;
		}

		/// Seta flag salvarResultadosParciais
		static void SalvarResultadosParciais( bool b ) {
			salvarResultadosParciais = b;
		}

		/// Calculo da porosidade
		double Porosidade(TCMatriz3D<bool> *&pm);

		/// Determina distribuicao Total de Poros (método normal)
		void DistTotalPoros();

		/** Recebe nome do arquivo e ponteiro para duas matrizes do tipo bool.
		*		Salva a mesclagem das matrizes em disco de forma que 0 será o fundo, 1 serão os índices da primeira matriz e 2 serão os índices da segunda matriz.
		*		Se a possição dos índices coincidirem, o indice da última matriz informada como parâmetro será considerado.
		*/
		bool Write(string fileName);

		/// Realiza a segmentação de poros e gargantas através do modelo informado.
		void Go( EModelo _modelo );

		/// Retorna ponteiro para a matriz que armazena as ligações
		inline TCMatriz3D<bool>* GetMatrizLigacoes (){
			return matrizLigacoes;
		}

		/// Retorna ponteiro para a matriz que armazena os sítios
		inline TCMatriz3D<bool>* GetMatrizSitios (){
			return matrizSitios;
		}

	private:
		/// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz bool informada como parametro.
		void SalvarResultadosParciaisEmDisco(TCMatriz3D<bool>* &mat, string fileName);

		/// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz int informada como parametro.
		void SalvarResultadosParciaisEmDisco(TCMatriz3D<int>* &mat, string fileName);

		/// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz rotulada informada como parametro.
		void SalvarResultadosParciaisEmDisco(TCRotulador3D<bool>* &mat, string fileName);

		/// Determina distribuicao de sitios e ligacoes (método novo) modelo 0 (Anaimp)
		void DistSitiosLigacoes_Modelo_0();

		/// Determina distribuicao de sitios e ligacoes (método novo, modelo 1)
		void DistSitiosLigacoes_Modelo_1();

		/// Determina distribuicao de sitios e ligacoes (método novo, modelo 2)
		void DistSitiosLigacoes_Modelo_2();

		/// Determina distribuicao de sitios e ligacoes (método novo, modelo 3)
		void DistSitiosLigacoes_Modelo_3();

		/// Determina distribuicao de sitios e ligacoes (método novo, modelo 4)
		void DistSitiosLigacoes_Modelo_4();

		/// Determina distribuicao de sitios e ligacoes (método novo, modelo 5)
		void DistSitiosLigacoes_Modelo_5();

		/// Determina distribuicao de sitios e ligacoes (método novo, modelo 6)
		void DistSitiosLigacoes_Modelo_6();

		/// Determina distribuicao de sitios e ligacoes (método novo, modelo 7, utiliza IDF)
		void DistSitiosLigacoes_Modelo_7();

		/// Determina distribuicao de sitios e ligacoes (baseado no modelo 7, tenta corrigir bug na identificação dos poros)
		void DistSitiosLigacoes_Modelo_8();
};

#endif
