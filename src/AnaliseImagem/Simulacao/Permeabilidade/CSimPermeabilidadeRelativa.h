// Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]
#ifndef CSimPermeabilidadeRelativa_h
#define CSimPermeabilidadeRelativa_h

/*
=========================================================================
PROJETO:    Biblioteca libldsc CSimPermeabilidadeRelativa
=========================================================================
Desenvolvido por:	
            LDSC - Laboratorio de Desenvolvimento de Software Científico 
@author     Leandro Puerari
@begin      2009
@copyright  (C) 2009 by Leandro Puerari
@email      puerari@gmail.com
@file 	   CSimPermeabilidadeRelativa.h
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
#include <AnaliseImagem/Simulacao/ConfiguracaoEquilibrio/CConfiguracoesEquilibrio3D.h>
#endif

#ifndef CSimPermeabilidadeIntrinseca_h
#include <AnaliseImagem/Simulacao/Permeabilidade/CSimPermeabilidadeIntrinseca.h>
#endif


#ifndef TCImagem3D_h
#include <AnaliseImagem/Matriz/TCImagem3D.h>
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

class CSimPermeabilidadeRelativa
{
// --------------------------------------------------------------Atributos
protected:
	/// Ponteiro para TCMatriz3D<int> (imagemA);
	TCImagem3D<int> * imagemA;

	/// Ponteiro para TCMatriz3D<int> (imagemB);
	TCImagem3D<int> * imagemB;

	/// Ponteiro para CSimPermeabilidadeIntrinseca responsável pelo cálculo da permeabilidade da fase A;
	CSimPermeabilidadeIntrinseca * permA;

	/// Ponteiro para CSimPermeabilidadeIntrinseca responsável pelo cálculo da permeabilidade da fase B;
	CSimPermeabilidadeIntrinseca * permB;

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
	CSimPermeabilidadeRelativa( ofstream & out );

// --------------------------------------------------------------Destrutor
	/// Destrutor
	virtual ~ CSimPermeabilidadeRelativa();

// ----------------------------------------------------------------Métodos
protected:
	/// Destrói os objetos que possam ter sido criados.
	void DestruirObjetos( );
	void SalvarImagens( );

public:
	/// Determina as curvas de permeabilidade relativa e salva o resultado em arquivo (.rpc). Recebe um ponteiro para TCImagem3D.
	// tirar fator relaxacao
	bool Go( TCImagem3D<int> * imagem3D );

	
	/// Determina as curvas de permeabilidade relativa e salva o resultado em arquivo (.rpc). Recebe um o path de uma imagem 3D (.dbm).
	// tirar fator relaxacao
	bool Go( string pathNomeArquivo );

	/// Determina as curvas de permeabilidade relativa e salva o resultado em arquivo (.rpc). Recebe um ponteiro para CMatriz3D.
	bool Go( TCMatriz3D<int> * matriz3D, unsigned int fatorAmplificacao, double dimensaoPixel, unsigned int numeroPixeisBorda = 0 );
	
	/// Determina as curvas de permeabilidade relativa e salva o resultado em arquivo (.rpc). Recebe o path de uma imagem 3D (.dbm).
	bool Go( string pathNomeArquivo, unsigned int fatorAmplificacao, double dimensaoPixel, unsigned int numeroPixeisBorda = 0 );

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
