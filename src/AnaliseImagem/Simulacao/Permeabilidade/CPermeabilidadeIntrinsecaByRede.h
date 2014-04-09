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
#include <AnaliseImagem/Caracterizacao/RedePercolacao/CRedeDePercolacao.h>
#include <AnaliseImagem/Caracterizacao/RedePercolacao/CContornoRedePercolacao.h>
#include <AnaliseImagem/Simulacao/Permeabilidade/RedePercolacao/CPermeabilidadeRede.h>
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonalDominante.h>
#include <Amostra/Material/CMFluido.h>
#include <AnaliseImagem/Filtro/FEspacial/FEConectividade/TCFEConectividade3D.h>

/**
 * @brief Determina a permeabilidade intrinseca a partir de uma imagem 3D
 * que será representada através de uma rede de precolação.
*/

class CPermeabilidadeIntrinsecaByRede {
		// --------------------------------------------------------------Atributos
	protected:
		/// Ponteiro para CContornoRedePercolacao;
		CContornoRedePercolacao * rede;

		/// Ponteiro para solver (CSolverMatrizDiagonalDominante);
		CSolverMatrizDiagonalDominante * solver;

		/// Ponteiro para CMFluido;
		CMFluido * fluido;

		/// Ponteiro para CPermeabilidadeRede responsável pelo cálculo da permeabilidade;
		CPermeabilidadeRede * perm;

	public:
		// Depois os atributos abaixo devem ser retirados pois fazem parte do solver
		// Número limite de iterações para o solver.
		unsigned long int limiteIteracoes;

		// Valor do erro aceitável para o solver.
		long double limiteErro;

		// -----------------------------------------------------------Construtores
	public:
		/// Construtor Default
		CPermeabilidadeIntrinsecaByRede();

		// --------------------------------------------------------------Destrutor
		/// Destrutor
		~ CPermeabilidadeIntrinsecaByRede();

		// ----------------------------------------------------------------Métodos
	protected:
		/// Destrói os objetos que possam ter sido criados.
		void DestruirObjetos();

	public:
		/// Permite alterar as propriedades do fluido.
		void SetarPropriedadesFluido( double viscosidade = 1.0, bool densidade = 1, bool compressibilidade = 1, bool molhabilidade = 1 );

		/// Permite alterar os parâmetros do solver.
		void SetarPropriedadesSolver( long double limiteErro = 1e-05, unsigned long int limiteIteracoes = 5000 );//, long double fatorRelaxacao = 0.7);

		/// Determina a permeabilidade e retorna o resultado. Recebe um ponteiro para TCImagem3D.
		long double Go( TCImagem3D<int> * &imagem3D, unsigned int nx, unsigned int ny, unsigned int nz, long double fatorRelaxacao = 0.7, CDistribuicao3D::Metrica3D metrica = CDistribuicao3D::Metrica3D::d345);

		/// Determina a permeabilidade e retorna o resultado. Recebe um ponteiro para TCImagem3D.
		long double Go( TCImagem3D<bool> * &imagem3D, unsigned int nx, unsigned int ny, unsigned int nz, int _raioMaximo, int _raioDilatacao, int _fatorReducao, int _incrementoRaio, EModelo _modelo, int _indice=1, int _fundo=0, unsigned long int _numero_contornos = 0, long double fatorRelaxacao = 0.7, CDistribuicao3D::Metrica3D metrica = CDistribuicao3D::Metrica3D::d345 );

	private:
		/// Cria os objetos necessários para cálculo da permeabilidade intrínseca.
		bool CriarObjetos( TCImagem3D<int> * &imagem3D, unsigned int &nx, unsigned int &ny, unsigned int &nz, long double &fatorRelaxacao );

		/// Cria os objetos necessários para cálculo da permeabilidade intrínseca.
		bool CriarObjetos( TCImagem3D<bool> * &imagem3D, unsigned int &nx, unsigned int &ny, unsigned int &nz, long double &fatorRelaxacao );

		/// Cria os objetos necessários para cálculo da permeabilidade intrínseca. Será chamada por um dos outros métodos CriarObjetos.
		bool CriarObjetos ( unsigned int &nx, unsigned int &ny, unsigned int &nz, long double &fatorRelaxacao );
};

#endif
