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
@file 	  CPermeabilidadeIntrinseca.cpp
@license    GNU General Public License - version 2
            see  $LICENSEFILE$ for the full license text.
*/
#include <SMatriz/SMDiagonal/CSMDGaussSeidel.h> //novo
#include <SMatriz/SMDiagonal/CSMDSOR.h> //novo

#include <Permeabilidade/CPermeabilidadeIntrinseca.h>

using namespace std;

// Construtor
CPermeabilidadeIntrinseca::CPermeabilidadeIntrinseca ()
{
	grafo	= NULL;	// Ponteiro para CGrafo;
	solver	= NULL;	// Ponteiro para CSMDiagonalDominante;
	fluido	= NULL;	// Ponteiro para CMFluido;
	perm		= NULL;	// Ponteiro para CPermeabilidadeGrafo;

	// valores usados em CSMDiagonalDominante
	limiteIteracoes = 5000; // depois retirar
	limiteErro = 0.000010;  // depois retirar
}

//Destrutor
CPermeabilidadeIntrinseca::~CPermeabilidadeIntrinseca ()
{
	DestruirObjetos();
}

//Métodos
void CPermeabilidadeIntrinseca::DestruirObjetos () {
   if ( grafo  ) { delete grafo;  grafo  = NULL; }
   if ( solver ) { delete solver; solver = NULL; }
   if ( fluido ) { delete fluido; fluido = NULL; }
   if ( perm   ) { delete perm;	 perm   = NULL; }
}

bool CPermeabilidadeIntrinseca::CriarObjetos (TCMatriz3D<int> * matriz3D, unsigned int fatorAmplificacao, double sizePixel, unsigned int numeroPixelsBorda, long double fatorRelaxacao) {
	DestruirObjetos();					// Destrói os objtos que possam estar criados.

	unsigned int size = matriz3D->NX(); 			// em 64bits int já é bem grande

	/*cerr << "size: " << size << endl;
  	cerr << "fatorAmplificacao: " << fatorAmplificacao << endl;
	cerr << "sizePixel: " << sizePixel << endl;
	cerr << "numeroPixelsBorda: " << numeroPixelsBorda << endl;
	cerr << "fatorRelaxacao: " << fatorRelaxacao << endl;*/

	grafo = new CGra3Dby2D_M3("imagem.grafo");		// Cria objeto grafo
   if ( ! grafo  ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
      DestruirObjetos();
      return false;
   }
	
	fluido = new CMFluido();//0.001002); 			// Cria fluido setando viscosidade
   if ( ! fluido ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
      DestruirObjetos();
      return false;
   }
	cout << "limiteErro em CPermeabilidadeIntrinseca::CriarObjetos: " << limiteErro << endl;
	solver = static_cast < CSMDiagonalDominante * > (new  CSMDSOR( limiteIteracoes, limiteErro, fatorRelaxacao/*, size */));
   if ( ! solver ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
      DestruirObjetos();
      return false;
   }
	
	perm = new CPermeabilidadeGrafo ( fluido, solver, grafo, matriz3D->NX(), matriz3D->NY(), matriz3D->NZ(), fatorAmplificacao, sizePixel/*,numeroPixelsBorda*/ );
   if ( ! perm   ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
      DestruirObjetos();
      return false;
   }
	return true;
}

long double CPermeabilidadeIntrinseca::CalcularPermeabilidade( TCMatriz3D<int> * matriz3D ) {
	// cria objetos para verificar se existe conectividade em Y, na matriz3D recebida.
	CFEConectividade3D * objCon = new CFEConectividade3D( matriz3D, 1, 0 );
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
	
   cout << "\n\nperm->GetfatorAmplificacao() = " << perm->GetfatorAmplificacao();
   cout << "\nperm->GetsizePixel() = " << perm->GetsizePixel();
   cout << "\nperm->GetnumeroPixelsBorda() = " << perm->GetnumeroPixelsBorda() << endl;
	
	
	cout << "Calculando grafo->Go( matriz3D )...." << endl;
	grafo->Go( matriz3D );
   cout << "grafo->Go( matriz3D )...ok!" << endl;

	cout << "Calculando perm->SolucaoSistema()" << endl;
	perm->SolucaoSistema();
	long double p = perm->Go(); // verificar se devia chamar Go
   // if( p > 1 ) p = 1;
   // if( p < 0 ) p = 0;
	cout << "perm->SolucaoSistema()...ok Permeabilidade = " << p << endl;
	return p;
}

long double CPermeabilidadeIntrinseca::Go( string pathFileName, long double fatorRelaxacao ) {
	CImagem3D<int> * img3D = NULL;
	img3D = new CImagem3D<int>( pathFileName ); 				// não é deletado ?
	if ( img3D ) 
		return Go( img3D );	
	else 
		return 0.0;
}

long double CPermeabilidadeIntrinseca::Go ( CImagem3D<int> * imagem3D, long double fatorRelaxacao ) {
	TCMatriz3D<int> * matriz3D = NULL;
	matriz3D = dynamic_cast< TCMatriz3D<int> *>(imagem3D);
	if ( ! matriz3D)
		return 0.0;
	return Go( matriz3D, imagem3D->FatorAmplificacao(), imagem3D->SizePixel(), imagem3D->NumeroPixelsBorda(), fatorRelaxacao);
}

long double CPermeabilidadeIntrinseca::Go( string pathFileName, unsigned int fatorAmplificacao, double sizePixel, unsigned int numeroPixelsBorda, long double fatorRelaxacao ) {
	TCMatriz3D<int> * mat3D = NULL;
	mat3D = new TCMatriz3D<int>( pathFileName ); 				// não é deletado ?
	if ( ! mat3D ) 
		return 0.0;	
	return Go( mat3D, fatorAmplificacao, sizePixel, numeroPixelsBorda, fatorRelaxacao );
}

long double CPermeabilidadeIntrinseca::Go ( TCMatriz3D<int> * matriz3D, unsigned int fatorAmplificacao, double sizePixel, unsigned int numeroPixelsBorda, long double fatorRelaxacao ) {
   if ( CriarObjetos( matriz3D, fatorAmplificacao, sizePixel, numeroPixelsBorda, fatorRelaxacao ) ) {
		return CalcularPermeabilidade( matriz3D );
	}
	return 0.0;
}

void CPermeabilidadeIntrinseca::SetarPropriedadesFluido( double viscosidade, bool densidade, bool compressibilidade, bool molhabilidade ) {
	if(fluido) {
		fluido->Viscosidade(viscosidade);
		fluido->Compressibilidade(compressibilidade);
		fluido->Densidade(densidade);
		fluido->Molhabilidade(molhabilidade);
	}
}

void CPermeabilidadeIntrinseca::SetarPropriedadesSolver( long double _limiteErro, unsigned long int _limiteIteracoes ) {
	limiteIteracoes = _limiteIteracoes; 	//depois apagar...tirar estes atributos
	limiteErro = _limiteErro;			//depois apagar...tirar estes atributos
	
	if(solver) {
		solver->LimiteErro(_limiteErro);
		solver->LimiteIteracoes(_limiteIteracoes);
		//solver->FatorRelaxacao(_fatorRelaxacao);
	}
}
