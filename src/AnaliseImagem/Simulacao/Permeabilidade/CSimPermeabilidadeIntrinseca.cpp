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
CSimPermeabilidadeIntrinseca::CSimPermeabilidadeIntrinseca () {

}

//Destrutor
CSimPermeabilidadeIntrinseca::~CSimPermeabilidadeIntrinseca () {
	DestruirObjetos();
}

//Métodos
void CSimPermeabilidadeIntrinseca::DestruirObjetos () {
	if ( grafo  ) { delete grafo;  grafo  = nullptr; }
	if ( solver ) { delete solver; solver = nullptr; }
	if ( fluido ) { delete fluido; fluido = nullptr; }
	if ( perm   ) { delete perm;	 perm   = nullptr; }
}
bool CSimPermeabilidadeIntrinseca::CriarObjetos ( ) {
	if (fluido && solver)
		return true;

	fluido = new CMFluido(0.001002); // Cria fluido setando viscosidade (0.001002)
	if ( ! fluido ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}

	//solver = static_cast < CSolverMatrizDiagonalDominante * > (new  CSolverMatrizDiagonal_SOR( limiteIteracoes, limiteErro, fatorRelaxacao/*, size */));
	solver = new  CSolverMatrizDiagonal_SOR(/* limiteIteracoes, limiteErro, fatorRelaxacao */);
	if ( ! solver ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}
	return true;
}

bool CSimPermeabilidadeIntrinseca::CriarObjetos (TCImagem3D<int> * matriz3D, unsigned short int modelo) {
	if (grafo && fluido && solver && perm) {
		return true;
	} else if ( CriarObjetos() == false ) {
		DestruirObjetos();
		return false;
	}

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

	perm = new CSimPermeabilidadeGrafo ( fluido, solver, grafo, matriz3D->NX(), matriz3D->NY(), matriz3D->NZ(),  matriz3D->FatorAmplificacao(),  matriz3D->DimensaoPixel(),  matriz3D->NumeroPixelsBorda() );
	if ( ! perm   ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		return false;
	}
	return true;
}

bool CSimPermeabilidadeIntrinseca::CriarObjetos (TCMatriz3D<int> * matriz3D, unsigned int fatorAmplificacao, double dimensaoPixel, unsigned int numeroPixeisBorda, unsigned short int modelo) {
	if (grafo && fluido && solver && perm) {
		return true;
	} else if ( CriarObjetos() == false ) {
		DestruirObjetos();
		return false;
	}
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
	
	perm = new CSimPermeabilidadeGrafo ( fluido, solver, grafo, matriz3D->NX(), matriz3D->NY(), matriz3D->NZ(), fatorAmplificacao, dimensaoPixel, numeroPixeisBorda );
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

	cout << "Calculando grafo->Go( matriz3D )...." << endl;
	grafo->Go( matriz3D );
	cout << "grafo->Go( matriz3D )...ok!" << endl;

	cout << "fluido->Viscosidade() = " << fluido->Viscosidade();
	cout << "\nfluido->Compressibilidade() = " << fluido->Compressibilidade();
	cout << "\nfluido->Densidade() = " << fluido->Densidade();
	cout << "\nfluido->Molhabilidade() = " << fluido->Molhabilidade();

	cout << "\n\nsolver->LimiteIteracoes() = " << solver->LimiteIteracoes();
	cout << "\nsolver->LimiteErro() = " << solver->LimiteErro();

	cout << "\n\nperm->FatorAmplificacao() = " << perm->FatorAmplificacao();
	cout << "\nperm->DimensaoPixel() = " << perm->DimensaoPixel();
	cout << "\nperm->NumeroPixeisBorda() = " << perm->NumeroPixeisBorda() << endl;

	cout << "Calculando perm->SolucaoSistema()" << endl;
	perm->SolucaoSistema();
	cout << "perm->SolucaoSistema()...ok" << endl;
	long double p = perm->Go();
	cerr << "perm->Go()...ok" << endl;
	cerr << "Permeabilidade = " << p << endl;
	return p;
}

long double CSimPermeabilidadeIntrinseca::Go( string pathNomeArquivo, unsigned short int modelo ) {
	TCImagem3D<int> * img3D = nullptr;
	img3D = new TCImagem3D<int>( pathNomeArquivo ); // não é deletado ?
	if ( img3D )
		return Go( img3D, modelo );
	else
		return 0.0;
}

long double CSimPermeabilidadeIntrinseca::Go ( TCImagem3D<int> * imagem3D, unsigned short int modelo ) {
	TCMatriz3D<int> * matriz3D = nullptr;
	matriz3D = dynamic_cast< TCMatriz3D<int> *>(imagem3D);
	if ( ! matriz3D)
		return 0.0;
	return Go( matriz3D, imagem3D->FatorAmplificacao(), imagem3D->DimensaoPixel(), imagem3D->NumeroPixelsBorda(), modelo);
}

long double CSimPermeabilidadeIntrinseca::Go( string pathNomeArquivo, unsigned int fatorAmplificacao, double dimensaoPixel, unsigned int numeroPixeisBorda, unsigned short int modelo ) {
	TCMatriz3D<int> * mat3D = nullptr;
	mat3D = new TCMatriz3D<int>( pathNomeArquivo ); 				// não é deletado ?
	if ( ! mat3D )
		return 0.0;
	return Go( mat3D, fatorAmplificacao, dimensaoPixel, numeroPixeisBorda, modelo );
}

long double CSimPermeabilidadeIntrinseca::Go ( TCMatriz3D<int> * matriz3D, unsigned int fatorAmplificacao, double dimensaoPixel, unsigned int numeroPixeisBorda, unsigned short int modelo ) {
	if ( CriarObjetos( matriz3D, fatorAmplificacao, dimensaoPixel, numeroPixeisBorda, modelo ) ) {
		return CalcularPermeabilidade( matriz3D );
	}
	return 0.0;
}

bool CSimPermeabilidadeIntrinseca::SetarPropriedadesFluido( double viscosidade, bool densidade, bool compressibilidade, bool molhabilidade ) {
	if(fluido) {
		fluido->Viscosidade(viscosidade);
		fluido->Compressibilidade(compressibilidade);
		fluido->Densidade(densidade);
		fluido->Molhabilidade(molhabilidade);
		return true;
	}
	return false;
}

bool CSimPermeabilidadeIntrinseca::SetarPropriedadesSolver( long double limiteErro, unsigned long int limiteIteracoes/*, long double fatorRelaxacao*/ ) {
	if(solver) {
		solver->LimiteErro(limiteErro);
		solver->LimiteIteracoes(limiteIteracoes);
		//solver->FatorRelaxacao(fatorRelaxacao);
		return true;
	}
	return false;
}

//	operator<<
ostream & operator<< (ostream & os, const CSimPermeabilidadeIntrinseca & obj) {
	os << "=====Permeabilidade Intrinseca by Rede=====";
	os << *(obj.perm);
	return os;
}
