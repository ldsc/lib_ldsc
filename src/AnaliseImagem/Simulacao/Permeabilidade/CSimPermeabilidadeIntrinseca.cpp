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
@file 	  CSimPermeabilidadeIntrinseca.cpp
@license    GNU General Public License - version 2
						see  $LICENSEFILE$ for the full license text.
*/
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_GaussSeidel.h> //novo
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_SOR.h> //novo

#include <AnaliseImagem/Simulacao/Permeabilidade/CSimPermeabilidadeIntrinseca.h>

using namespace std;

// Construtor
CSimPermeabilidadeIntrinseca::CSimPermeabilidadeIntrinseca ()
{
	grafo	= nullptr;	// Ponteiro para CGrafo;
	solver	= nullptr;	// Ponteiro para CSolverMatrizDiagonalDominante;
	fluido	= nullptr;	// Ponteiro para CMFluido;
	perm		= nullptr;	// Ponteiro para CSimPermeabilidadeGrafo;

	// valores usados em CSolverMatrizDiagonalDominante
	limiteIteracoes = 5000; // depois retirar
	limiteErro = 0.000010;  // depois retirar
}

//Destrutor
CSimPermeabilidadeIntrinseca::~CSimPermeabilidadeIntrinseca ()
{
	DestruirObjetos();
}

//Métodos
void CSimPermeabilidadeIntrinseca::DestruirObjetos () {
	if ( grafo  ) { delete grafo;  grafo  = nullptr; }
	if ( solver ) { delete solver; solver = nullptr; }
	if ( fluido ) { delete fluido; fluido = nullptr; }
	if ( perm   ) { delete perm;	 perm   = nullptr; }
}

bool CSimPermeabilidadeIntrinseca::CriarObjetos (TCMatriz3D<int> * matriz3D, unsigned int fatorAmplificacao, double dimensaoPixel, unsigned int numeroPixeisBorda, long double fatorRelaxacao, unsigned short int modelo) {
	DestruirObjetos();					// Destrói os objtos que possam estar criados.
	/*
	unsigned int size = matriz3D->NX(); 			// em 64bits int já é bem grande
	cerr << "size: " << size << endl;
	cerr << "fatorAmplificacao: " << fatorAmplificacao << endl;
	cerr << "dimensaoPixel: " << dimensaoPixel << endl;
	cerr << "numeroPixeisBorda: " << numeroPixeisBorda << endl;
	cerr << "fatorRelaxacao: " << fatorRelaxacao << endl;
	*/
	switch ( modelo ) {
		case 1: grafo = new CGrafoConexaoSerial_M1 ( "imagem.grafo" );
			break;
		case 2:	grafo = new CGrafoConexaoSerial_M2 ( "imagem.grafo" );
			break;
		case 3: grafo = new CGrafoConexaoSerial_M3 ( "imagem.grafo" );
			break;
		case 4:	grafo = new CGrafoConexaoSerial_M4 ( "imagem.grafo" );
			break;
		case 5:	grafo   = new CGrafoConexaoSerial_M5 ( "imagem.grafo" );
			break;
		default: grafo = new CGrafoConexaoSerial_M3 ( "imagem.grafo" );
	};

	if ( ! grafo  ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}
	
	fluido = new CMFluido(0.001002); // Cria fluido setando viscosidade (0.001002)
	if ( ! fluido ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}
	cout << "limiteErro em CSimPermeabilidadeIntrinseca::CriarObjetos: " << limiteErro << endl;
	//solver = static_cast < CSolverMatrizDiagonalDominante * > (new  CSolverMatrizDiagonal_SOR( limiteIteracoes, limiteErro, fatorRelaxacao/*, size */));
	solver = new  CSolverMatrizDiagonal_SOR( limiteIteracoes, limiteErro, fatorRelaxacao/*, size */);
	if ( ! solver ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}
	
	perm = new CSimPermeabilidadeGrafo ( fluido, solver, grafo, matriz3D->NX(), matriz3D->NY(), matriz3D->NZ(), fatorAmplificacao, dimensaoPixel/*,numeroPixeisBorda*/ );
	if ( ! perm   ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}
	return true;
}

long double CSimPermeabilidadeIntrinseca::CalcularPermeabilidade( TCMatriz3D<int> * matriz3D ) {
	// cria objetos para verificar se existe conectividade em Y, na matriz3D recebida.
	TCFEConectividade3D<int> * objCon = new TCFEConectividade3D<int>( matriz3D, 1, 0 );
	bool conectada  = objCon->isConnected( matriz3D );
	cout << "conectada: " << conectada << endl;
	delete objCon; // Destroe objeto auxiliar
	// se não estiver conectada, permeabilidade = 0.0;
	if ( ! conectada ) {
		cout << "Imagem não conectada, retornando 0.0" << endl;
		return 0.0;
	}

	cout << "fluido->Viscosidade() = " << fluido->Viscosidade();
	cout << "\nfluido->Compressibilidade() = " << fluido->Compressibilidade();
	cout << "\nfluido->Densidade() = " << fluido->Densidade();
	cout << "\nfluido->Molhabilidade() = " << fluido->Molhabilidade();
	
	cout << "\n\nsolver->LimiteIteracoes() = " << solver->LimiteIteracoes();
	cout << "\nsolver->LimiteErro() = " << solver->LimiteErro();
	
	cout << "\n\nperm->FatorAmplificacao() = " << perm->FatorAmplificacao();
	cout << "\nperm->DimensaoPixel() = " << perm->DimensaoPixel();
	cout << "\nperm->NumeroPixelsBorda() = " << perm->NumeroPixelsBorda() << endl;
	
	
	cout << "Calculando grafo->Go( matriz3D )...." << endl;
	grafo->Go( matriz3D );
	cout << "grafo->Go( matriz3D )...ok!" << endl;

	cout << "Calculando perm->SolucaoSistema()" << endl;
	perm->SolucaoSistema();
	long double p = perm->Go(); // verificar se devia chamar Go
	// if( p > 1 ) p = 1;
	// if( p < 0 ) p = 0;
	cout << "perm->SolucaoSistema()...ok" << endl;
	cout << "Permeabilidade = " << p << endl;
	return p;
}

long double CSimPermeabilidadeIntrinseca::Go( string pathNomeArquivo, long double fatorRelaxacao, unsigned short int modelo ) {
	TCImagem3D<int> * img3D = nullptr;
	img3D = new TCImagem3D<int>( pathNomeArquivo ); // não é deletado ?
	if ( img3D )
		return Go( img3D, fatorRelaxacao, modelo );
	else
		return 0.0;
}

long double CSimPermeabilidadeIntrinseca::Go ( TCImagem3D<int> * imagem3D, long double fatorRelaxacao, unsigned short int modelo ) {
	TCMatriz3D<int> * matriz3D = nullptr;
	matriz3D = dynamic_cast< TCMatriz3D<int> *>(imagem3D);
	if ( ! matriz3D)
		return 0.0;
	return Go( matriz3D, imagem3D->FatorAmplificacao(), imagem3D->DimensaoPixel(), imagem3D->NumeroPixelsBorda(), fatorRelaxacao, modelo);
}

long double CSimPermeabilidadeIntrinseca::Go( string pathNomeArquivo, unsigned int fatorAmplificacao, double dimensaoPixel, unsigned int numeroPixeisBorda, long double fatorRelaxacao, unsigned short int modelo ) {
	TCMatriz3D<int> * mat3D = nullptr;
	mat3D = new TCMatriz3D<int>( pathNomeArquivo ); 				// não é deletado ?
	if ( ! mat3D )
		return 0.0;
	return Go( mat3D, fatorAmplificacao, dimensaoPixel, numeroPixeisBorda, fatorRelaxacao, modelo );
}

long double CSimPermeabilidadeIntrinseca::Go ( TCMatriz3D<int> * matriz3D, unsigned int fatorAmplificacao, double dimensaoPixel, unsigned int numeroPixeisBorda, long double fatorRelaxacao, unsigned short int modelo ) {
	if ( CriarObjetos( matriz3D, fatorAmplificacao, dimensaoPixel, numeroPixeisBorda, fatorRelaxacao, modelo ) ) {
		return CalcularPermeabilidade( matriz3D );
	}
	return 0.0;
}

void CSimPermeabilidadeIntrinseca::SetarPropriedadesFluido( double viscosidade, bool densidade, bool compressibilidade, bool molhabilidade ) {
	if(fluido) {
		fluido->Viscosidade(viscosidade);
		fluido->Compressibilidade(compressibilidade);
		fluido->Densidade(densidade);
		fluido->Molhabilidade(molhabilidade);
	}
}

void CSimPermeabilidadeIntrinseca::SetarPropriedadesSolver( long double _limiteErro, unsigned long int _limiteIteracoes ) {
	limiteIteracoes = _limiteIteracoes; //depois apagar...tirar estes atributos
	limiteErro = _limiteErro;	//depois apagar...tirar estes atributos
	
	if(solver) {
		solver->LimiteErro(_limiteErro);
		solver->LimiteIteracoes(_limiteIteracoes);
		//solver->FatorRelaxacao(_fatorRelaxacao);
	}
}
