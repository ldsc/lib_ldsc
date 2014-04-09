#ifndef CSimPermeabilidadeIntrinseca_h
#define CSimPermeabilidadeIntrinseca_h

/*
=========================================================================
PROJETO:    Biblioteca libldsc CSimPermeabilidadeIntrinseca
=========================================================================
Desenvolvido por:	
            LDSC - Laboratorio de Desenvolvimento de Software Científico 
@author     Leandro Puerari
@begin      2009
@copyright  (C) 2009 by Leandro Puerari
@email      puerari@gmail.com
@file 	   CSimPermeabilidadeIntrinseca.h
@license    GNU General Public License - version 2
            see  $LICENSEFILE$ for the full license text.
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M3.h>
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
	CGrafoConexaoSerial * grafo;

	/// Ponteiro para solver (CSolverMatrizDiagonalDominante);
	CSolverMatrizDiagonalDominante * solver;

	/// Ponteiro para CMFluido;
	CMFluido * fluido;

	/// Ponteiro para CSimPermeabilidadeGrafo responsável pelo cálculo da permeabilidade;
	CSimPermeabilidadeGrafo * perm;

public:
	// Depois os atributos abaixo devem ser retirados pois fazem parte do solver
	// Número limite de iterações para o solver.
	unsigned long int limiteIteracoes;

	// Valor do erro aceitável para o solver.
	long double limiteErro;

// -----------------------------------------------------------Construtores
public:	
	/// Construtor Default
	CSimPermeabilidadeIntrinseca();

// --------------------------------------------------------------Destrutor
	/// Destrutor
	virtual ~ CSimPermeabilidadeIntrinseca();

// ----------------------------------------------------------------Métodos
protected:
	/// Destrói os objetos que possam ter sido criados.
	void DestruirObjetos();

public:
	/// Cria os objetos necessários para cálculo da permeabilidade intrínseca. 
	/// (Deve ser chamado antes de CalcularPermeabilidade())
	bool CriarObjetos( TCMatriz3D<int> * matriz3D, unsigned int fatorAmplificacao, double dimensaoPixel,  unsigned int numeroPixeisBorda = 0, long double fatorRelaxacao = 0.7 );
	
	/// Cálcula a permeabilidade intrínseca. (Deve ser chamado depois de CriarObjetos())
	long double CalcularPermeabilidade(TCMatriz3D<int> * matriz3D);
	
	/// Determina a permeabilidade e retorna o resultado. Recebe um ponteiro para TCImagem3D.
	long double Go( TCImagem3D<int> * imagem3D, long double fatorRelaxacao = 0.7);
	
	/// Determina a permeabilidade e retorna o resultado. Recebe um ponteiro para CMatriz3D.
	long double Go( TCMatriz3D<int> * matriz3D, unsigned int fatorAmplificacao, double dimensaoPixel, unsigned int numeroPixeisBorda = 0, long double fatorRelaxacao = 0.7);
	
	/// Determina a permeabilidade e retorna o resultado. Recebe path do arquivo TCImagem3D.
	long double Go( string pathNomeArquivo, long double fatorRelaxacao = 0.7);
	
	/// Determina a permeabilidade e retorna o resultado. Recebe path do arquivo CMatriz3D.
	long double Go( string pathNomeArquivo, unsigned int fatorAmplificacao, double dimensaoPixel, unsigned int numeroPixeisBorda = 0, long double fatorRelaxacao = 0.7);
	
	/// Permite alterar as propriedades do fluido.
	void SetarPropriedadesFluido( double viscosidade = 1.0, bool densidade = 1, bool compressibilidade = 1, bool molhabilidade = 1 );

	/// Permite alterar os parâmetros do solver.
	void SetarPropriedadesSolver( long double limiteErro = 1e-05, unsigned long int limiteIteracoes = 5000 );//, long double fatorRelaxacao = 0.7);

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -------------------------------------------------------------Sobrecarga
// -----------------------------------------------------------------Friend
};
#endif
