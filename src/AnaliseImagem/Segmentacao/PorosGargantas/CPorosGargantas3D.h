#ifndef CPorosGargantas3D_H
#define CPorosGargantas3D_H

#include <AnaliseImagem/Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFd3453D.h>
//#include <Segmentacao/CObjetoImagem.h> // novo, repreenta objeto da imagem
#include <AnaliseImagem/Filtro/FEspacial/FERotulagem/TCRotulador3D.h>
#include <AnaliseImagem/Matriz/TCImagem3D.h>
//#include <AnaliseImagem/Matriz/CVetor.h>
#include <iostream>
#include <fstream>
//#include <sstream>
//#include <cstdlib>
//#include <map>

using namespace std;

/**
 * Classe que realiza a segmentação de poros e gargantas em imagens de meios porosos
	@author Leandro Puerari <puerari@gmail.com>
	@author André Duarte Bueno <andreduartebueno@gmail.com>
*/
class CPorosGargantas3D
{
	protected:
		/// Ponteiro para objeto filtro morfologia matematica
		TCFEMMIDFd3453D<bool>* pfidf;

		/// Matriz original (ponteiro para matriz original)
		TCMatriz3D<bool>* pm;

		/// Ponteiro para objeto rotulador o qual herda TCMatriz3D<int> para representar a matriz rotulada.
		TCRotulador3D<bool>* pmRotulo;

		/// Nome da imagem que esta processando (usado para salvar resultado final)
		std::string nomeImagem;

		/// Valor que indentifica os objetos de interesse na imagem
		int INDICE;

		/// Valor que indentifica o fundo da imagem
		int FUNDO;

		/// Se ativo salva os resultados parciais
		static bool salvarResultadosParciais ;

	public:
		/// Construtor
		CPorosGargantas3D(TCMatriz3D<bool>* &matriz, std::string _nomeImagem = "", int _indice=1, int _fundo=0);

		/// Destrutor
		~CPorosGargantas3D();

		/// Retorna flag salvarResultadosParciais
		static bool SalvarResultadosParciais( ) {
			return salvarResultadosParciais;
		}

		/// Seta flag salvarResultadosParciais
		static void SalvarResultadosParciais( bool b ) {
			salvarResultadosParciais = b;
		}

	private:
		/// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz bool informada como parametro.
		void SalvarResultadosParciaisEmDisco(TCMatriz3D<bool>* &mat, string nomeArquivo);

		/// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz int informada como parametro.
		void SalvarResultadosParciaisEmDisco(TCMatriz3D<int>* &mat, string nomeArquivo);

		/// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz rotulada informada como parametro.
		void SalvarResultadosParciaisEmDisco(TCRotulador3D<bool>* &mat, string nomeArquivo);

};

#endif
