#ifndef CSegPorosGargantas3D_H
#define CSegPorosGargantas3D_H

#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIRA3D.h>
#include <AnaliseImagem/Filtro/FEspacial/FERotulagem/CRotuladorIRA3D.h>
#include <MetNum/Matriz/CMatrizObjetoImagem.h>
#include <MetNum/Matriz/TCImagem3D.h>
#include <MetNum/Matriz/CVetor.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>

using namespace std;

/**
 * Classe para determinacao da distribuicao de poros e gargantas (sítios e ligacoes) usando método da abertura através da IRA.
 * @author Leandro Puerari <puerari@gmail.com>
 * @author André Duarte Bueno <andreduartebueno@gmail.com>
*/
class CSegPorosGargantas3D : public CMatrizObjetoImagem
{
	protected:
		// // Ponteiro para objeto filtro IRA
		//TCFEMMIRA3D<bool>* pfira;

		/// Ponteiro para matriz IRA
		TCMatriz3D<int>* pmira;

		/// Matriz original (ponteiro para matriz original)
		TCMatriz3D<bool>* pm;

		/// Imagem original (ponteiro para imagem original)
		TCImagem3D<bool>* pi3d;

		/// Matriz que armazena os sítios identificados
		TCMatriz3D<bool>* matrizSitios;

		/// Matriz que armazena as ligações identificadas
		TCMatriz3D<bool>* matrizLigacoes;

		/// Ponteiro para objeto rotulador o qual herda TCRotulador3D<T> que por sua vez herda TCMatriz3D<int> para representar a matriz rotulada.
		CRotuladorIRA3D* matrizRotulo;

		/// Porosidade
		double porosidade;

		/// fator usado como critério de parada
		int raioMaximoElementoEstruturante;

		/// fator usado para reducao do Raio do Elemento Estruturante no processamento da dilatacao.
		int fatorReducaoRaioElemEst;

		/// Valor do incremento do raio do elemento estruturante
		int incrementoRaioElementoEstruturante;

		/// Raio do Elemento Estruturante usado na operação de dilatação (Modelo 7)
		int raioEEDilatacao;

		/// Numero do modelo de cálculo
		int modelo;

		/// Valor que indentifica os objetos de interesse na imagem
		int INDICE;

		/// Valor que indentifica o fundo da imagem
		int FUNDO;

		/// Se ativo salva os resultados parciais
		bool salvarResultadosParciais;

		/// Se ativo gera detalhes dos objetos em matrizObjetos
		bool gerarDetalhesObjetos;

	public:
		/// Construtor
		CSegPorosGargantas3D(TCMatriz3D<bool>* &matriz, int _indice=1, int _fundo=0);

		/// Construtor
		CSegPorosGargantas3D(TCImagem3D<bool>* &matriz, int _indice=1, int _fundo=0);

		/// Destrutor
		~CSegPorosGargantas3D();

		/// Declara iterator para a matrizObjetos
		map<int,CObjetoImagem>::iterator it;

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
		bool SalvarResultadosParciais( ) {
			return salvarResultadosParciais;
		}

		/// Seta flag salvarResultadosParciais
		void SalvarResultadosParciais( bool b ) {
			salvarResultadosParciais = b;
		}

		/// Retorna flag gerarDetalhesObjetos
		bool GerarDetalhesObjetos( ) {
			return gerarDetalhesObjetos;
		}

		/// Seta flag gerarDetalhesObjetos
		void GerarDetalhesObjetos( bool b ) {
			gerarDetalhesObjetos = b;
		}

		///Retorna ponteiro para a matriz que representa sítios
		inline TCMatriz3D<bool> * MatrizSitios(){
			return matrizSitios;
		}

		///Retorna ponteiro para a matriz que representa ligações
		inline TCMatriz3D<bool> * MatrizLigacoes(){
			return matrizLigacoes;
		}

		/// Grava em disco, com o nome informado, a mesclagem da matrizSitios com a matrizLigacoes
		bool Write(string nomeArquivo );

		/// Grava em disco, com o nome informado, os objetos identificados.
		bool SalvarListaObjetos(string nomeArquivo);

		/// Método que executa a segmentação de poros e gargantas de acordo com o modelo informado.
		void Go(int _modelo);

	private:
		/// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz bool informada como parametro.
		void SalvarResultadosParciaisEmDisco(TCMatriz3D<bool>* &mat, string nomeArquivo);

		/// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz int informada como parametro.
		void SalvarResultadosParciaisEmDisco(TCMatriz3D<int>* &mat, string nomeArquivo);

		/// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz rotulada informada como parametro.
		void SalvarResultadosParciaisEmDisco(CRotuladorIRA3D *&mat, string nomeArquivo);

		/// Se a flag gerarDetalhesObjetos estiver setada, gera detalhes dos objetos identificados (Num. voxeis, ligações, etc).
		void GerarDetalhesMatrizObjetos();

		/// Determina distribuicao de sitios e ligacoes (Modelo 0)
		void Modelo_0();

};

#endif
