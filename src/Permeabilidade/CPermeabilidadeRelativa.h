// Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CPermeabilidadeRelativa_h
#define CPermeabilidadeRelativa_h

/*
=========================================================================
PROJETO:    Biblioteca libldsc CPermeabilidadeRelativa
=========================================================================
Desenvolvido por:	
            LDSC - Laboratorio de Desenvolvimento de Software Científico 
@author     Leandro Puerari
@begin      2009
@copyright  (C) 2009 by Leandro Puerari
@email      puerari@gmail.com
@file 	   CPermeabilidadeRelativa.h
@license    GNU General Public License - version 2
            see  $LICENSEFILE$ for the full license text.
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#ifndef __IOSTREAM_H
#include <iostream>
#endif

#include <iomanip>

// -----------------------------------------------------------------------
// Bibliotecas lib_ldsc
// -----------------------------------------------------------------------

#ifndef CConfiguracoesEquilibrio3D_h
#include <ConEqu/CConfiguracoesEquilibrio3D.h>
#endif

#ifndef CPermeabilidadeIntrinseca_h
#include <Permeabilidade/CPermeabilidadeIntrinseca.h>
#endif


#ifndef CImagem3D_h
#include <Matriz/CImagem3D.h>
#endif

#include <sstream>

using namespace std;
/**
 * @brief Determina as curvas de permeabilidade relativa a partir de uma imagem 3D.
 * 
 * Para cada passo de pressão aplicado na determinação das configurações de equilíbrio
 * de uma imagem tridimensional, obtem-se as fases molhante (ex: agua) e não molhante
 * (ex: óleo), determina-se a permeabilidade intrínseca de cada fase e a seguir grava
 * em arquivo (.rpc) as curvas de permeabilidade relativa.
 * 
*/

class CPermeabilidadeRelativa
{
// --------------------------------------------------------------Atributos
protected:
	/// Ponteiro para TCMatriz3D<int> (imagemA);
	CImagem3D<int> * imagemA;

	/// Ponteiro para TCMatriz3D<int> (imagemB);
	CImagem3D<int> * imagemB;

	/// Ponteiro para CPermeabilidadeIntrinseca responsável pelo cálculo da permeabilidade da fase A;
	CPermeabilidadeIntrinseca * permA;

	/// Ponteiro para CPermeabilidadeIntrinseca responsável pelo cálculo da permeabilidade da fase B;
	CPermeabilidadeIntrinseca * permB;

	/// Ponteiro para CConfiguracoesEquilibrio3D responsável por determinar as configurações de equilíbrio da imagem 3D;
  	CConfiguracoesEquilibrio3D *confeq;
  	
  	/// Arquivo de saída com os dados das curvas de permeabilidade relativa.
  	ofstream *foutPermRel;
  	
public: 
	/// Número limite de iterações para o solver.
	unsigned long int limiteIteracoes;
	
	/// Valor do erro aceitável para o solver.
	long double limiteErro;
	
	///se verdadeiro, salva imagens A e B de confEq.
  	bool salvarImagensAB;
  	
  	///se verdadeiro, corrige a imagem abertura em confEq.
  	bool corrigirAbertura;
// -----------------------------------------------------------Construtores
public:	
	/// Construtor Default
	CPermeabilidadeRelativa( ofstream & out );

// --------------------------------------------------------------Destrutor
	/// Destrutor
	virtual ~ CPermeabilidadeRelativa();

// ----------------------------------------------------------------Métodos
protected:
	/// Destrói os objetos que possam ter sido criados.
	void DestruirObjetos( );
	void SalvarImagens( );

public:
	/// Determina as curvas de permeabilidade relativa e salva o resultado em arquivo (.rpc). Recebe um ponteiro para CImagem3D.
	// tirar fator relaxacao
	bool Go( CImagem3D<int> * imagem3D );

	
	/// Determina as curvas de permeabilidade relativa e salva o resultado em arquivo (.rpc). Recebe um o path de uma imagem 3D (.dbm).
	// tirar fator relaxacao
	bool Go( string pathFileName );

	/// Determina as curvas de permeabilidade relativa e salva o resultado em arquivo (.rpc). Recebe um ponteiro para CMatriz3D.
	bool Go( TCMatriz3D<int> * matriz3D, unsigned int fatorAmplificacao, double sizePixel, unsigned int numeroPixelsBorda = 0 );
	
	/// Determina as curvas de permeabilidade relativa e salva o resultado em arquivo (.rpc). Recebe um o path de uma imagem 3D (.dbm).
	bool Go( string pathFileName, unsigned int fatorAmplificacao, double sizePixel, unsigned int numeroPixelsBorda = 0 );

// --------------------------------------------------------------------Set
	void LimiteIteracoes ( unsigned long int li ) { 
		limiteIteracoes = li;	
	}
	
	void LimiteErro ( long double le ) { 
		limiteErro = le;  
	}
// --------------------------------------------------------------------Get
// -------------------------------------------------------------Sobrecarga
// -----------------------------------------------------------------Friend
};

#endif
