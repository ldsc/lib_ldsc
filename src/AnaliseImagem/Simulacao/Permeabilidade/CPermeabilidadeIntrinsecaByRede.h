#ifndef CPermeabilidadeIntrinsecaByRede_h
#define CPermeabilidadeIntrinsecaByRede_h

/*
=========================================================================
PROJETO:    Biblioteca libldsc CPermeabilidadeIntrinsecaByRede
=========================================================================
Desenvolvido por:	LDSC - Laboratorio de Desenvolvimento de Software Científico
@author     Leandro Puerari
@begin      2014
@copyright  (C) 2014 by Leandro Puerari
@email      puerari@gmail.com
@file 	    CPermeabilidadeIntrinsecaByRede.h
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
#include <EstruturaDados/CRede.h>
#include <AnaliseImagem/Caracterizacao/RedePercolacao/CRedeDePercolacao.h>
#include <AnaliseImagem/Simulacao/Permeabilidade/RedePercolacao/CSimPermeabilidadeRede.h>
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonalDominante.h>
#include <Amostra/Material/CMFluido.h>
#include <AnaliseImagem/Filtro/FEspacial/FEConectividade/TCFEConectividade3D.h>
//#include <AnaliseImagem/Caracterizacao/RedePercolacao/CContornoRedePercolacao.h>
//#include <EstruturaDados/CRedeContorno.h>

/**
 * @brief Determina a permeabilidade intrinseca a partir de uma imagem 3D
 * que será representada através de uma rede de precolação.
*/
class CPermeabilidadeIntrinsecaByRede {
		// --------------------------------------------------------------Atributos
	protected:
		/// Ponteiro para CContornoRedePercolacao;
		CContornoRedePercolacao * rede {nullptr};

		/// Ponteiro para solver (CSolverMatrizDiagonalDominante);
		CSolverMatrizDiagonalDominante * solver {nullptr};

		/// Ponteiro para CMFluido;
		CMFluido * fluido {nullptr};

		/// Ponteiro para CSimPermeabilidadeRede responsável pelo cálculo da permeabilidade;
		CSimPermeabilidadeRede * perm {nullptr};

		// -----------------------------------------------------------Construtores
	public:
		/// Construtor Default
		CPermeabilidadeIntrinsecaByRede();

		// --------------------------------------------------------------Destrutor
		/// Destrutor
		virtual ~ CPermeabilidadeIntrinsecaByRede();

		// ----------------------------------------------------------------Métodos
	public:
		/// Cria os objetos necessários para cálculo da permeabilidade intrínseca. Para alterar as propriedades do fluido ou do solver, primeiro chamar este método e depois chamar os métodos correspondentes para setar as propriedades.
		bool CriarObjetos ( unsigned int &nx, unsigned int &ny, unsigned int &nz );

		/// Permite alterar as propriedades do fluido.
		bool SetarPropriedadesFluido( double viscosidade = 0.001002, bool densidade = true, bool compressibilidade = true, bool molhabilidade = true );

		/// Permite alterar os parâmetros do solver.
		bool SetarPropriedadesSolver( long double limiteErro = 1.0e-06, unsigned long int limiteIteracoes = 10000/*, long double fatorRelaxacao = 0.7*/);

		/// Determina a permeabilidade e retorna o resultado. Recebe um ponteiro para TCImagem3D.
		long double Go( TCImagem3D<int> * &imagem3D, unsigned int nx, unsigned int ny, unsigned int nz, CDistribuicao3D::Metrica3D metrica = CDistribuicao3D::Metrica3D::d345);

		/// Determina a permeabilidade e retorna o resultado. Recebe um ponteiro para TCImagem3D.
		long double Go( TCImagem3D<bool> * &imagem3D, unsigned int nx, unsigned int ny, unsigned int nz, int _raioMaximo, int _raioDilatacao, int _fatorReducao, int _incrementoRaio, EModelo _modelo, int _indice=1, int _fundo=0, unsigned long int _numero_contornos = 0, CDistribuicao3D::Metrica3D metrica = CDistribuicao3D::Metrica3D::d345 );

	protected:
		/// Destrói os objetos que possam ter sido criados.
		void DestruirObjetos();

	private:
		/// Cria os objetos necessários para cálculo da permeabilidade intrínseca.
		bool CriarObjetos( TCImagem3D<int> * &imagem3D, unsigned int &nx, unsigned int &ny, unsigned int &nz );

		/// Cria os objetos necessários para cálculo da permeabilidade intrínseca.
		bool CriarObjetos( TCImagem3D<bool> * &imagem3D, unsigned int &nx, unsigned int &ny, unsigned int &nz );

		/// Cálcula a permeabilidade intrínseca. (Deve ser chamado depois de CriarObjetos())
		long double CalcularPermeabilidade( );
};

#endif
