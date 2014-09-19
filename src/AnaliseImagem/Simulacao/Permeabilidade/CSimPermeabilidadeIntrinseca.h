#ifndef CSimPermeabilidadeIntrinseca_h
#define CSimPermeabilidadeIntrinseca_h

/*
=========================================================================
PROJETO:    Biblioteca libldsc CSimPermeabilidadeIntrinseca
=========================================================================
Desenvolvido por:	LDSC - Laboratorio de Desenvolvimento de Software Científico
@author     Leandro Puerari
@begin      2009
@copyright  (C) 2009 by Leandro Puerari
@email      puerari@gmail.com
@file 	   CSimPermeabilidadeIntrinseca.h
@license    GNU General Public License - version 2 - see  $LICENSEFILE$ for the full license text.
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <iostream>
#include<iomanip>

// -----------------------------------------------------------------------
// Bibliotecas lib_ldsc
// -----------------------------------------------------------------------
#include <MetNum/Matriz/TCImagem3D.h>
#include <MetNum/Matriz/TCMatriz3D.h>
#include <EstruturaDados/CGrafo.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M1.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M2.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M3.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M4.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M5.h>
#include <AnaliseImagem/Simulacao/Permeabilidade/GrafoConexaoSerial/CSimPermeabilidadeGrafo.h>
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonalDominante.h>
#include <Amostra/Material/CMFluido.h>
#include <AnaliseImagem/Filtro/FEspacial/FEConectividade/TCFEConectividade3D.h>

/**
 * @brief Determina a permeabilidade intrinseca a partir de uma imagem 3D.
 * que será representada através de um grafo de conexão serial
*/

class CSimPermeabilidadeIntrinseca
{
		// --------------------------------------------------------------Atributos
	protected:
		/// Ponteiro para CGrafo;
		CGrafoConexaoSerial * grafo {nullptr};

		/// Ponteiro para solver (CSolverMatrizDiagonalDominante);
		CSolverMatrizDiagonalDominante * solver {nullptr};

		/// Ponteiro para CMFluido;
		CMFluido * fluido {nullptr};

		/// Ponteiro para CSimPermeabilidadeGrafo responsável pelo cálculo da permeabilidade;
		CSimPermeabilidadeGrafo * perm {nullptr};

		// -----------------------------------------------------------Construtores
	public:
		/// Construtor Default
		CSimPermeabilidadeIntrinseca();

		// --------------------------------------------------------------Destrutor
		/// Destrutor
		virtual ~ CSimPermeabilidadeIntrinseca();

		// ----------------------------------------------------------------Métodos
	public:
		/// Cria os objetos necessários para cálculo da permeabilidade intrínseca. Para alterar as propriedades do fluido ou do solver, primeiro chamar este método e depois chamar os métodos correspondentes para setar as propriedades.
		bool CriarObjetos ( );

		/// Permite alterar as propriedades do fluido.
		bool SetarPropriedadesFluido( double viscosidade = 0.001002, bool densidade = true, bool compressibilidade = true, bool molhabilidade = true );

		/// Permite alterar os parâmetros do solver.
		bool SetarPropriedadesSolver( long double limiteErro = 1.0e-06, unsigned long int limiteIteracoes = 100000/*, long double fatorRelaxacao = 0.7*/);

		/// Determina a permeabilidade e retorna o resultado. Recebe um ponteiro para TCImagem3D.
		long double Go( TCImagem3D<int> * imagem3D, unsigned short int modelo = 3);

		/// Determina a permeabilidade e retorna o resultado. Recebe um ponteiro para CMatriz3D.
		long double Go( TCMatriz3D<int> * matriz3D, unsigned int fatorAmplificacao, double dimensaoPixel, unsigned int numeroPixeisBorda = 0, unsigned short int modelo = 3);

		/// Determina a permeabilidade e retorna o resultado. Recebe path do arquivo TCImagem3D.
		long double Go( string pathNomeArquivo, unsigned short int modelo = 3);

		/// Determina a permeabilidade e retorna o resultado. Recebe path do arquivo CMatriz3D.
		long double Go( string pathNomeArquivo, unsigned int fatorAmplificacao, double dimensaoPixel, unsigned int numeroPixeisBorda = 0, unsigned short int modelo = 3);

	protected:
		/// Destrói os objetos que possam ter sido criados.
		void DestruirObjetos();

	private:
		/// Cria os objetos necessários para cálculo da permeabilidade intrínseca (Deve ser chamado antes de CalcularPermeabilidade()).
		bool CriarObjetos( TCMatriz3D<int> * matriz3D, unsigned int fatorAmplificacao, double dimensaoPixel,  unsigned int numeroPixeisBorda = 0, unsigned short int modelo = 3);

		/// Cria os objetos necessários para cálculo da permeabilidade intrínseca (Deve ser chamado antes de CalcularPermeabilidade()).
		bool CriarObjetos( TCImagem3D<int> * matriz3D, unsigned short int modelo = 3);

		/// Cálcula a permeabilidade intrínseca. (Deve ser chamado depois de CriarObjetos())
		long double CalcularPermeabilidade(TCMatriz3D<int> * matriz3D);

		// --------------------------------------------------------------------Get
		// --------------------------------------------------------------------Set
		// -------------------------------------------------------------Sobrecarga
		// -----------------------------------------------------------------Friend
		friend std::ostream & operator<< (std::ostream & os, const CSimPermeabilidadeIntrinseca & obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream & operator<< (std::ostream & os, const CSimPermeabilidadeIntrinseca & obj);

#endif
