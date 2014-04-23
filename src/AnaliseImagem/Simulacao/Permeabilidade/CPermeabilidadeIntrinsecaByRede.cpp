/*
=========================================================================
PROJETO:    Biblioteca libldsc CPermeabilidadeIntrinsecaByRede
=========================================================================
Desenvolvido por:	LDSC - Laboratorio de Desenvolvimento de Software Científico
@author     Leandro Puerari
@begin      2009
@copyright  (C) 2009 by Leandro Puerari
@email      puerari@gmail.com
@file				CPermeabilidadeIntrinsecaByRede.cpp
@license    GNU General Public License - version 2
						see  $LICENSEFILE$ for the full license text.
*/
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_GaussSeidel.h> //novo
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_SOR.h> //novo
#include <Tempo/CTime.h>

#include <AnaliseImagem/Simulacao/Permeabilidade/CPermeabilidadeIntrinsecaByRede.h>

using namespace std;

// Construtor
CPermeabilidadeIntrinsecaByRede::CPermeabilidadeIntrinsecaByRede () {

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

bool CPermeabilidadeIntrinsecaByRede::CriarObjetos (unsigned int &nx, unsigned int &ny, unsigned int &nz ) {
	if (rede && fluido && solver)
		return true;

	DestruirObjetos();

	//rede = new CRedeContorno(imagem3D, _raioMaximo, _raioDilatacao, _fatorReducao, _incrementoRaio, _modelo, _indice, _fundo, _numero_contornos);		// Cria objeto rede
	rede = new CContornoRedePercolacao(nx, ny, nz);		// Cria objeto rede
	if ( ! rede  ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		cerr << "Não pode criar rede em CPermeabilidadeIntrinsecaByRede::CriarObjetos()!" << endl;
		return false;
	}

	fluido = new CMFluido(0.001002);//0.001002); 			// Cria fluido setando viscosidade
	if ( ! fluido ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		cerr << "Não pode criar fluido em CPermeabilidadeIntrinsecaByRede::CriarObjetos()!" << endl;
		return false;
	}
	//SetarPropriedadesFluido(0.001002,1,1,1);

	//solver = static_cast < CSolverMatrizDiagonalDominante * > (new  CSolverMatrizDiagonal_SOR( limiteIteracoes, limiteErro, fatorRelaxacao/*, size */));
	solver = new  CSolverMatrizDiagonal_SOR(/* limiteIteracoes, limiteErro, fatorRelaxacao */);
	if ( ! solver ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
		DestruirObjetos();
		cerr << "Não pode criar solver em CPermeabilidadeIntrinsecaByRede::CriarObjetos()!" << endl;
		return false;
	}
	return true;
}

bool CPermeabilidadeIntrinsecaByRede::CriarObjetos ( TCImagem3D<int> * &imagem3D, unsigned int &nx, unsigned int &ny, unsigned int &nz) {
	if ( CriarObjetos( nx, ny, nz ) ) {
		perm = new CSimPermeabilidadeRede ( fluido, solver, rede, imagem3D->NX(), imagem3D->NY(), imagem3D->NZ(), imagem3D->FatorAmplificacao(), imagem3D->DimensaoPixel(), imagem3D->NumeroPixelsBorda() );
		if ( ! perm   ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
			DestruirObjetos();
			return false;
		}
		return true;
	}
	return false;
}

bool CPermeabilidadeIntrinsecaByRede::CriarObjetos ( TCImagem3D<bool> * &imagem3D, unsigned int &nx, unsigned int &ny, unsigned int &nz ) {
	if ( CriarObjetos( nx, ny, nz ) ) {
		perm = new CSimPermeabilidadeRede ( fluido, solver, rede, imagem3D->NX(), imagem3D->NY(), imagem3D->NZ(), imagem3D->FatorAmplificacao(), imagem3D->DimensaoPixel(), imagem3D->NumeroPixelsBorda() );
		if ( ! perm   ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
			DestruirObjetos();
			return false;
		}
		return true;
	}
	return false;
}

long double CPermeabilidadeIntrinsecaByRede::CalcularPermeabilidade( ) {
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

	cout << "fluido->Viscosidade() = "					<< fluido->Viscosidade();
	cout << "\nfluido->Compressibilidade() = "	<< fluido->Compressibilidade();
	cout << "\nfluido->Densidade() = "					<< fluido->Densidade();
	cout << "\nfluido->Molhabilidade() = "			<< fluido->Molhabilidade();
	cout << "\n\nsolver->LimiteIteracoes() = "	<< solver->LimiteIteracoes();
	cout << "\nsolver->LimiteErro() = "					<< solver->LimiteErro();
	cout << "\n\nperm->FatorAmplificacao() = "	<< perm->FatorAmplificacao();
	cout << "\nperm->DimensaoPixel() = "				<< perm->DimensaoPixel();
	cout << "\nperm->NumeroPixeisBorda() = "		<< perm->NumeroPixeisBorda() << endl;

	cout << "Calculando perm->SolucaoSistema()" << endl;
	perm->SolucaoSistema();
	cout << "perm->SolucaoSistema()...ok" << endl;
	long double p = perm->Go();
	cerr << "perm->Go()...ok" << endl;
	cerr << "Permeabilidade = " << p << endl;
	return p;
}

long double CPermeabilidadeIntrinsecaByRede::Go( TCImagem3D<int> * &imagem3D, unsigned int nx, unsigned int ny, unsigned int nz, CDistribuicao3D::Metrica3D metrica ) {
	if ( CriarObjetos( imagem3D, nx, ny, nz ) ) {
		cout << "Calculando rede->Go( )..." << endl;
		rede->Go( imagem3D, metrica );
		cout << "rede->Go( )...ok!" << endl << endl;
		if (salvarRede != "") {
			cout << "Salvando a rede..." << endl;
			if (rede->SalvarListaObjetos(salvarRede)) {
				cout << "Rede salva no arquivo: " << salvarRede << endl << endl;
			} else {
				cerr << "Não foi possível salvar a rede no arquivo: " << salvarRede << endl << endl;
			}
		}
		return CalcularPermeabilidade( );
	}
	cerr << "Não criou objetos em CPermeabilidadeIntrinsecaByRede::Go(TCImagem3D<int> *)! Retornando 0.0 ..." << endl;
	return 0.0;
}

long double CPermeabilidadeIntrinsecaByRede::Go( TCImagem3D<bool> * &imagem3D, unsigned int nx, unsigned int ny, unsigned int nz, int _raioMaximo, int _raioDilatacao, int _fatorReducao, int _incrementoRaio, EModelo _modelo, int _indice, int _fundo, unsigned long int _numero_contornos, CDistribuicao3D::Metrica3D metrica ) {
	CTime ( "Tempo CPermeabilidadeIntrinsecaByRede::Go() = ", &cout );
	if ( CriarObjetos( imagem3D, nx, ny, nz ) ) {
		cerr << "Calculando rede->Go( )...." << endl;
		rede->Go( imagem3D, _raioMaximo, _raioDilatacao, _fatorReducao, _incrementoRaio, _modelo, _indice, _fundo, metrica );
		cerr << "rede->Go( )...ok!" << endl << endl;
		if (salvarRede != "") {
			cout << "Salvando a rede..." << endl;
			if (rede->SalvarListaObjetos(salvarRede)) {
				cout << "Rede salva no arquivo" << salvarRede << endl << endl;
			} else {
				cerr << "Não foi possível salvar a rede no arquivo" << salvarRede << endl << endl;
			}
		}
		return CalcularPermeabilidade( );
	}
	cerr << "Não criou objetos em CPermeabilidadeIntrinsecaByRede::Go(TCImagem3D<bool> *)! Retornando 0.0 ..." << endl;
	return 0.0;
}

bool CPermeabilidadeIntrinsecaByRede::SetarPropriedadesFluido( double viscosidade, bool densidade, bool compressibilidade, bool molhabilidade ) {
	if(fluido) {
		fluido->Viscosidade(viscosidade);
		fluido->Compressibilidade(compressibilidade);
		fluido->Densidade(densidade);
		fluido->Molhabilidade(molhabilidade);
		return true;
	}
	return false;
}

bool CPermeabilidadeIntrinsecaByRede::SetarPropriedadesSolver( long double limiteErro, unsigned long int limiteIteracoes/*, long double fatorRelaxacao*/ ) {
	if(solver) {
		solver->LimiteErro(limiteErro);
		solver->LimiteIteracoes(limiteIteracoes);
		//solver->FatorRelaxacao(fatorRelaxacao);
		return true;
	}
	return false;
}

//	operator<<
ostream & operator<< (ostream & os, const CPermeabilidadeIntrinsecaByRede & obj) {
	os << "=====Permeabilidade Intrinseca by Rede=====";
	os << *(obj.perm);
	return os;
}
