// Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CPermeabilidadeIntrinseca_h
#define CPermeabilidadeIntrinseca_h

/*
=========================================================================
PROJETO:    Biblioteca libldsc CPermeabilidadeIntrinseca
=========================================================================
Desenvolvido por:	
            LDSC - Laboratorio de Desenvolvimento de Software Científico 
@author     Leandro Puerari
@begin      2009
@copyright  (C) 2009 by Leandro Puerari
@email      puerari@gmail.com
@file 	  CPermeabilidadeIntrinseca.h
@license    GNU General Public License - version 2
            see  $LICENSEFILE$ for the full license text.
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#ifndef __IOSTREAM_H
#include <iostream>
#endif

#include<iomanip>

// -----------------------------------------------------------------------
// Bibliotecas lib_ldsc
// -----------------------------------------------------------------------
#ifndef CImagem3D_h
#include <Matriz/CImagem3D.h>
#endif

#ifndef CMatriz3D_h
#include <Matriz/CMatriz3D.h>
#endif

#ifndef CGrafo_h
#include <Grafo/CGrafo.h>
#endif

#ifndef CGra3Dby2D_h
#include <Grafo/CGra3Dby2D.h>
#endif

#ifndef CGra3Dby2D_M3_h
#include <Grafo/CGra3Dby2D_M3.h>
#endif

#ifndef CPermeabilidadeGrafo_h
#include <Permeabilidade/CPermeabilidadeGrafo.h>
#endif

#ifndef CSMDiagonalDominante_h
#include <SMatriz/SMDiagonal/CSMDiagonalDominante.h>
#endif

#ifndef CMFluido_h
#include <Material/CMFluido.h>
#endif


#ifndef CFEConectividade3D_h
#include <Filtro/FEspacial3D/FEConectividade3D/CFEConectividade3D.h>
#endif

/**
 * @brief Determina a permeabilidade intrinseca a partir de uma imagem 3D.
 * 
*/

class CPermeabilidadeIntrinseca
{
// --------------------------------------------------------------Atributos
protected:
	/// Ponteiro para CGrafo;
	CGra3Dby2D * grafo;

	/// Ponteiro para solver (CSMDiagonalDominante);
	CSMDiagonalDominante * solver;

	/// Ponteiro para CMFluido;
	CMFluido * fluido;

	/// Ponteiro para CPermeabilidadeGrafo responsável pelo cálculo da permeabilidade;
	CPermeabilidadeGrafo * perm;

public:
	// Depois os atributos abaixo devem ser retirados pois fazem parte do solver
	// Número limite de iterações para o solver.
	unsigned long int limiteIteracoes;

	// Valor do erro aceitável para o solver.
	long double limiteErro;

// -----------------------------------------------------------Construtores
public:	
	/// Construtor Default
	CPermeabilidadeIntrinseca();

// --------------------------------------------------------------Destrutor
	/// Destrutor
	virtual ~ CPermeabilidadeIntrinseca();

// ----------------------------------------------------------------Métodos
protected:
	/// Destrói os objetos que possam ter sido criados.
	void DestruirObjetos();

public:
	/// Cria os objetos necessários para cálculo da permeabilidade intrínseca. 
	/// (Deve ser chamado antes de CalcularPermeabilidade())
	bool CriarObjetos( CMatriz3D * matriz3D, unsigned int fatorAmplificacao, double sizePixel,  unsigned int numeroPixelsBorda = 0, long double fatorRelaxacao = 0.7 );
	
	/// Cálcula a permeabilidade intrínseca. (Deve ser chamado depois de CriarObjetos())
	long double CalcularPermeabilidade(CMatriz3D * matriz3D);
	
	/// Determina a permeabilidade e retorna o resultado. Recebe um ponteiro para CImagem3D.
	long double Go( CImagem3D * imagem3D, long double fatorRelaxacao = 0.7);
	
	/// Determina a permeabilidade e retorna o resultado. Recebe um ponteiro para CMatriz3D.
	long double Go( CMatriz3D * matriz3D, unsigned int fatorAmplificacao, double sizePixel, unsigned int numeroPixelsBorda = 0, long double fatorRelaxacao = 0.7);
	
	/// Determina a permeabilidade e retorna o resultado. Recebe um ponteiro para path do arquivo CImagem3D.
	long double Go( string pathFileName, long double fatorRelaxacao = 0.7);
	
	/// Determina a permeabilidade e retorna o resultado. Recebe um ponteiro para path do arquivo CMatriz3D.
	long double Go( string pathFileName, unsigned int fatorAmplificacao, double sizePixel, unsigned int numeroPixelsBorda = 0, long double fatorRelaxacao = 0.7);
	
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
