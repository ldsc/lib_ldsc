/*
=========================================================================
PROJETO:    Biblioteca libldsc CPermeabilidadeIntrinsecaByRede
=========================================================================
Desenvolvido por:	
						LDSC - Laboratorio de Desenvolvimento de Software Científico
@author     Leandro Puerari
@begin      2009
@copyright  (C) 2009 by Leandro Puerari
@email      puerari@gmail.com
@file 	  CPermeabilidadeIntrinsecaByRede.cpp
@license    GNU General Public License - version 2
						see  $LICENSEFILE$ for the full license text.
*/
#include <MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDGaussSeidel.h> //novo
#include <MetNum/Solver/SistemaEquacoes/SMDiagonal/CSMDSOR.h> //novo

#include <AnaliseImagem/Simulacao/Permeabilidade/CPermeabilidadeIntrinsecaByRede.h>

using namespace std;

// Construtor
CPermeabilidadeIntrinsecaByRede::CPermeabilidadeIntrinsecaByRede () {
	rede	= nullptr;	// Ponteiro para CRedeDePercolacao;
	solver	= nullptr;	// Ponteiro para CSMDiagonalDominante;
	fluido	= nullptr;	// Ponteiro para CMFluido;
	perm		= nullptr;	// Ponteiro para CPermeabilidadeRede;

	// valores usados em CSMDiagonalDominante
	limiteIteracoes = 5000; // depois retirar
	limiteErro = 0.000010;  // depois retirar
}

//Destrutor
CPermeabilidadeIntrinsecaByRede::~CPermeabilidadeIntrinsecaByRede () {
	DestruirObjetos();
}

//Métodos
void CPermeabilidadeIntrinsecaByRede::DestruirObjetos () {
	if ( rede  )	{ delete rede;		rede	 = nullptr;	}
	if ( solver ) { delete solver;	solver = nullptr; }
	if ( fluido ) { delete fluido;	fluido = nullptr; }
	if ( perm   ) { delete perm;		perm   = nullptr; }
}

bool CPermeabilidadeIntrinsecaByRede::CriarObjetos (TCImagem3D<int> * imagem3D, unsigned int nx, unsigned int ny, unsigned int nz, long double &fatorRelaxacao) {
	DestruirObjetos();					// Destrói os objtos que possam estar criados.
	/*
	unsigned int size = matriz3D->NX(); 			// em 64bits int já é bem grande
	cerr << "size: " << size << endl;
	cerr << "fatorAmplificacao: " << fatorAmplificacao << endl;
	cerr << "sizePixel: " << sizePixel << endl;
	cerr << "numeroPixelsBorda: " << numeroPixelsBorda << endl;
	cerr << "fatorRelaxacao: " << fatorRelaxacao << endl;
	*/

	rede = new CRedeContorno(nx, ny, nz);		// Cria objeto rede
	if ( ! rede  ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}
	
	fluido = new CMFluido();//0.001002); 			// Cria fluido setando viscosidade
	if ( ! fluido ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}
	cout << "limiteErro em CPermeabilidadeIntrinsecaByRede::CriarObjetos: " << limiteErro << endl;
	solver = static_cast < CSMDiagonalDominante * > (new  CSMDSOR( limiteIteracoes, limiteErro, fatorRelaxacao/*, size */));
	if ( ! solver ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}
	
	perm = new CPermeabilidadeRede ( fluido, solver, rede, imagem3D->NX(), imagem3D->NY(), imagem3D->NZ(), imagem3D->FatorAmplificacao(), imagem3D->SizePixel() /*,numeroPixelsBorda*/ );
	if ( ! perm   ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}
	return true;
}

bool CPermeabilidadeIntrinsecaByRede::CriarObjetos ( TCImagem3D<bool> * imagem3D, unsigned int nx, unsigned int ny, unsigned int nz, int &_raioMaximo, int &_raioDilatacao, int &_fatorReducao, int &_incrementoRaio, EModelo &_modelo, int &_indice, int &_fundo, unsigned long int &_numero_contornos, long double &fatorRelaxacao ) {
	DestruirObjetos();					// Destrói os objtos que possam estar criados.

	//rede = new CRedeContorno(imagem3D, _raioMaximo, _raioDilatacao, _fatorReducao, _incrementoRaio, _modelo, _indice, _fundo, _numero_contornos);		// Cria objeto rede
	rede = new CRedeContorno(nx, ny, nz);		// Cria objeto rede
	if ( ! rede  ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}

	fluido = new CMFluido();//0.001002); 			// Cria fluido setando viscosidade
	if ( ! fluido ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}
	cout << "limiteErro em CPermeabilidadeIntrinsecaByRede::CriarObjetos: " << limiteErro << endl;
	solver = static_cast < CSMDiagonalDominante * > (new  CSMDSOR( limiteIteracoes, limiteErro, fatorRelaxacao/*, size */));
	if ( ! solver ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}

	perm = new CPermeabilidadeRede ( fluido, solver, rede, imagem3D->NX(), imagem3D->NY(), imagem3D->NZ(), imagem3D->FatorAmplificacao(), imagem3D->SizePixel() /*,numeroPixelsBorda*/ );
	if ( ! perm   ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}
	return true;
}

long double CPermeabilidadeIntrinsecaByRede::CalcularPermeabilidade( /*TCImagem3D<int> *imagem3D*/ ) {
	/*// cria objetos para verificar se existe conectividade em Y, na matriz3D recebida.
	TCFEConectividade3D<int> * objCon = new TCFEConectividade3D<int>( imagem3D, 1, 0 );
	bool conectada  = objCon->isConnected( imagem3D );
	cout << "conectada: " << conectada << endl;
	delete objCon; // Destroe objeto auxiliar
	// se não estiver conectada, permeabilidade = 0.0;
	if ( ! conectada ) {
		cout << "Imagem não conectada, retornando 0.0" << endl;
		return 0.0;
	}*/
	cout << "fluido->Viscosidade() = " << fluido->Viscosidade();
	cout << "\nfluido->Compressibilidade() = " << fluido->Compressibilidade();
	cout << "\nfluido->Densidade() = " << fluido->Densidade();
	cout << "\nfluido->Molhabilidade() = " << fluido->Molhabilidade();
	cout << "\n\nsolver->LimiteIteracoes() = " << solver->LimiteIteracoes();
	cout << "\nsolver->LimiteErro() = " << solver->LimiteErro();
	cout << "\n\nperm->GetfatorAmplificacao() = " << perm->GetfatorAmplificacao();
	cout << "\nperm->GetsizePixel() = " << perm->GetsizePixel();
	cout << "\nperm->GetnumeroPixelsBorda() = " << perm->GetnumeroPixelsBorda() << endl;
		
	cout << "Calculando rede->Go( )...." << endl;
	rede->Go( 300, 300, 300 );
	cout << "rede->Go( )...ok!" << endl;

	cout << "Calculando perm->SolucaoSistema()" << endl;
	perm->SolucaoSistema();
	long double p = perm->Go(); // verificar se devia chamar Go
	// if( p > 1 ) p = 1;
	// if( p < 0 ) p = 0;
	cout << "perm->SolucaoSistema()...ok Permeabilidade = " << p << endl;
	return p;
}

long double CPermeabilidadeIntrinsecaByRede::Go ( TCImagem3D<int> * imagem3D, long double fatorRelaxacao ) {
	if ( CriarObjetos( imagem3D, fatorRelaxacao ) ) {
		return CalcularPermeabilidade( );
	}
	return 0.0;
}

long double CPermeabilidadeIntrinsecaByRede::Go ( TCImagem3D<bool> * imagem3D, int _raioMaximo, int _raioDilatacao, int _fatorReducao, int _incrementoRaio, EModelo _modelo, int _indice, int _fundo, unsigned long int _numero_contornos, long double fatorRelaxacao ) {
	if ( CriarObjetos( imagem3D, _raioMaximo, _raioDilatacao, _fatorReducao, _incrementoRaio, _modelo, _indice, _fundo, _numero_contornos, fatorRelaxacao) ) {
		return CalcularPermeabilidade( );
	}
	return 0.0;
}

void CPermeabilidadeIntrinsecaByRede::SetarPropriedadesFluido( double viscosidade, bool densidade, bool compressibilidade, bool molhabilidade ) {
	if(fluido) {
		fluido->Viscosidade(viscosidade);
		fluido->Compressibilidade(compressibilidade);
		fluido->Densidade(densidade);
		fluido->Molhabilidade(molhabilidade);
	}
}

void CPermeabilidadeIntrinsecaByRede::SetarPropriedadesSolver( long double _limiteErro, unsigned long int _limiteIteracoes ) {
	limiteIteracoes = _limiteIteracoes; 	//depois apagar...tirar estes atributos
	limiteErro = _limiteErro;			//depois apagar...tirar estes atributos
	
	if(solver) {
		solver->LimiteErro(_limiteErro);
		solver->LimiteIteracoes(_limiteIteracoes);
		//solver->FatorRelaxacao(_fatorRelaxacao);
	}
}
