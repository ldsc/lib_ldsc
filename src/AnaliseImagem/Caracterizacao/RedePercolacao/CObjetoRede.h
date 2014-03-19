#ifndef CObjetoRede_h
#define CObjetoRede_h

/*
=========================================================================
PROJETO:    Biblioteca libldsc CObjetoRede
=========================================================================
Desenvolvido por:
						LDSC - Laboratorio de Desenvolvimento de Software Científico
@author     Andre Duarte Bueno - http://www.lenep.uenf.br/~bueno
@begin      2010
@copyright  (C) 2010 by Andre Duarte Bueno - http://www.lenep.uenf.br/~bueno
@email      <bueno@lenep.uenf.br>
@file 	  CObjetoImagem.h
@license    GNU General Public License - version 2
						see  $LICENSEFILE$ for the full license text.
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
#include <MetNum/Solver/SistemaEquacoes/CSMParametroSolver.h>

// -----------------------------------------------------------------------
// Bibliotecas libldsc
// -----------------------------------------------------------------------

using namespace std;

class CObjetoRede : public CObjetoImagem, public CSMParametroSolver
{
	protected:
		//----------------------------------------------------Atributos
		/// Identifica em qual posição o objeto se encontra (Leste, Oeste, Centro...).
		CContorno::ETipoContorno contorno = CContorno::ETipoContorno::CENTER;

		/// Armazena propriedade do objeto (condutância)
		double propriedade = 0.0;

	public:
		// ---------------------------------------------------Construtor-Destrutor
		/// Construtor
		CObjetoRede() : CObjetoImagem()	{
		}

		/// Construtor sobrecarregado. Recebe tipo do objeto, rótulo e opcionalmente o número de objetos representados
		CObjetoRede( ETipoObjetoImagem _t, int _n=0) : CObjetoImagem(_t, _n) {
		}

		/// Destrutor
		~CObjetoRede() {}

		/// Grava as informações do objeto no arquivo recebido como parâmetro (formato Grafo de Conexão Serial).
		void GravarObjetoGrafo(ofstream &_fout, const int & seq);

		/** @brief Calcula o fluxo associado ao objeto, ou seja, considera-se que este objeto
		 * esta conectado a outros objetos e que em função das propriedades dos objetos,
		 * existe alguma informação que transita entre os objetos.
		 * Esta propriedade é calculada por esta função.
		 * Pode ser fluxo de massa, de calor, de qualquer coisa, ...
		*/
		long double Fluxo (std::map<int, CObjetoRede> *moi );

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

};

#endif
