#ifndef CObjetoRedePercolacao_h
#define CObjetoRedePercolacao_h

/**
=========================================================================
PROJETO:    Biblioteca libldsc CObjetoRedePercolacao
=========================================================================
Desenvolvido por: LDSC - Laboratorio de Desenvolvimento de Software Científico
@author     Leandro Puerari <puerari@gmail.com>
@file 	    CObjetoRedePercolacao.h
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <set>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <AnaliseImagem/Segmentacao/PorosGargantas/CObjetoImagem.h>
#include <MetNum/Contorno/CContorno.h>
#include <MetNum/Solver/SistemaEquacoes/CSolverMatriz_ParametroSolver.h>

// -----------------------------------------------------------------------
// Bibliotecas libldsc
// -----------------------------------------------------------------------

using namespace std;
class CMatrizObjetoRede;

class CObjetoRedePercolacao : public CObjetoImagem, public CSolverMatriz_ParametroSolver
{
	protected:
		//----------------------------------------------------Atributos
		/// Identifica em qual posição o objeto se encontra (Leste, Oeste, Centro...).
		CContorno::ETipoContorno contorno {CContorno::ETipoContorno::CENTER};

		/// Armazena propriedade do objeto (condutância)
		double propriedade {0.0};

		/// Ponteiro para matriz de objetos que poderão estar conectados ao objeto this
		CMatrizObjetoRede * matrizObjetos {nullptr};

	public:
		// ---------------------------------------------------Construtor-Destrutor
		/// Construtor
		CObjetoRedePercolacao() : CObjetoImagem()	{
		}

		/// Construtor sobrecarregado. Recebe ponteiro para metriz de objetos, tipo do objeto, rótulo e opcionalmente o número de objetos representados
		//CObjetoRedePercolacao( CMatrizObjetoRede * ptrMatObjs, ETipoObjetoImagem _t, int _n=0) : CObjetoImagem(_t, _n), matrizObjetos(ptrMatObjs) {
		CObjetoRedePercolacao( ETipoObjetoImagem _t, int _n=0) : CObjetoImagem(_t, _n) {
		}

		/// Destrutor
		~CObjetoRedePercolacao() {}

		/// Grava as informações do objeto no arquivo recebido como parâmetro (formato Grafo de Conexão Serial).
		void GravarObjetoGrafo(ofstream &_fout, const int & seq);

		/** @brief Calcula o fluxo associado ao objeto, ou seja, considera-se que este objeto
		 * esta conectado a outros objetos e que em função das propriedades dos objetos,
		 * existe alguma informação que transita entre os objetos.
		 * Esta propriedade é calculada por esta função.
		 * Pode ser fluxo de massa, de calor, de qualquer coisa, ...
		*/
		long double Fluxo (std::map<int, CObjetoRedePercolacao> *moi );

		/** @brief Função herdada da classe CParametroSolver, usada para calcular o valor de x (pressão).
		 * Redefinição de funções herdadas.
		*/
		virtual long double Go (long double d = 0);

		// --------------------------------------------------------------------Get
		/// Retorna a camada na qual o objeto se encontra
		inline CContorno::ETipoContorno Contorno() {
			return contorno;
		}

		/// Retorna a propriedade do objeto (condutância)
		inline double Propriedade() {
			return propriedade;
		}

		// --------------------------------------------------------------------Set
		/// Seta a camada na qual o objeto se encontra
		inline void Contorno(CContorno::ETipoContorno _c) { contorno = _c; }

		/// Seta a propriedade do objeto (condutância)
		inline void Propriedade( double p ) { propriedade = p; }

		/// Seta ponteiro para matriz de objetos
		inline void MatrizObjetos (CMatrizObjetoRede * ptr) { matrizObjetos = ptr; }

};

#endif
