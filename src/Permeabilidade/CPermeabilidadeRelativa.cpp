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
@file 	  CPermeabilidadeRelativa.cpp
@license    GNU General Public License - version 2
            see  $LICENSEFILE$ for the full license text.
*/
#include <Filtro/FEspacial3D/FEConectividade3D/CFEConectividade3D.h>
#include <Permeabilidade/CPermeabilidadeRelativa.h>

// Construtor
CPermeabilidadeRelativa::CPermeabilidadeRelativa ( ofstream & out )
{
	foutPermRel = & out; // Cria arquivo que receberá os dados de permeabilidade relativa
	
	permA	= NULL;	// Ponteiro para CPermeabilidadeGrafo;
	permB	= NULL;	// Ponteiro para CPermeabilidadeGrafo;
	confeq	= NULL;	// Ponteiro para CConfiguracoesEquilibrio3D;	

	imagemA = new CImagem3D(); //Estou criando imagens A e B no construtor para poder setar paramentor de recontrução antes da execução de Go()
	imagemB = new CImagem3D(); //Como as imagens podem ser destruídas em Go(), sempre verificar se são != NULL;
	
	// valores usados em CSMDiagonalDominante
	limiteIteracoes = 5000;
	limiteErro = 0.000010;
	
	salvarImagensAB = false;
	corrigirAbertura = true;
}

//Destrutor
CPermeabilidadeRelativa::~CPermeabilidadeRelativa () {
	DestruirObjetos();		// destroi os objetos criados.
}

void CPermeabilidadeRelativa::DestruirObjetos () {
	//if ( permA   ) { delete permA;	permA	= NULL; }
	//if ( permB   ) { delete permB;	permB	= NULL; }
	//COMENTADO PARA NÃO DAR FALHA DE SEGMENTAÇÃO
	if ( confeq  ) { delete confeq;	confeq	= NULL; }
	if ( imagemA ) { delete imagemA;	imagemA	= NULL; }
	if ( imagemB ) { delete imagemB;	imagemB	= NULL; }
}

// Métodos
bool CPermeabilidadeRelativa::Go( string pathFileName )
{
	CImagem3D * imagem3D = new CImagem3D( pathFileName );
	bool retorno = Go( imagem3D );
	delete imagem3D;
	return retorno;
}

bool CPermeabilidadeRelativa::Go ( CImagem3D * imagem3D )
{
	if ( ! imagem3D ) 
		return false;
		
	if ( ! imagemA )
		imagemA = new CImagem3D();
	
	if ( ! imagemB )
		imagemB = new CImagem3D();
	
	imagemA->Path(imagem3D->Path());
	imagemB->Path(imagem3D->Path());
	imagemA->FatorAmplificacao( imagem3D->FatorAmplificacao() );
	imagemB->FatorAmplificacao( imagem3D->FatorAmplificacao() );
	imagemA->NumeroPixelsBorda( imagem3D->NumeroPixelsBorda() );
	imagemB->NumeroPixelsBorda( imagem3D->NumeroPixelsBorda() );
	imagemA->SizePixel( imagem3D->SizePixel() );
	imagemB->SizePixel( imagem3D->SizePixel() );
	
	return Go( dynamic_cast< CMatriz3D *>( imagem3D ), imagem3D->FatorAmplificacao(), imagem3D->SizePixel(), imagem3D->NumeroPixelsBorda());
}

bool CPermeabilidadeRelativa::Go( string pathFileName, unsigned int fatorAmplificacao, double sizePixel, unsigned int numeroPixelsBorda )
{
	CMatriz3D * mat3D = new CMatriz3D( pathFileName );
	bool retorno = Go( mat3D, fatorAmplificacao, sizePixel, numeroPixelsBorda );
	delete mat3D;
	return retorno;
}

bool CPermeabilidadeRelativa::Go ( CMatriz3D * matriz3D, unsigned int fatorAmplificacao, double sizePixel, unsigned int numeroPixelsBorda )
{
	if ( ! matriz3D ) { 
		DestruirObjetos();	
		cerr << "Erro: Imagem matriz3D nula!" << endl;
		return false; 
	}
	if ( ! foutPermRel->is_open() ) {
		DestruirObjetos();	
		cerr << "Erro: Arquivo de saída inválido!" << endl;
		return false;
	}
	long double saturacaoA = 0.0;
	long double saturacaoB = 0.0; 

  	long double permeabilidadeA = 0.0;
	long double permeabilidadeB = 0.0;

	//certificar que as imagens A e B estão devidamente criadas.
	if ( ! imagemA ) imagemA = new CImagem3D();
	if ( ! imagemA ) { 
		DestruirObjetos();			
		cerr << "Erro: imagemA nula!" << endl;
		return false; 
	}
	
	if ( ! imagemB ) imagemB = new CImagem3D();
	if ( ! imagemB ) { 
		DestruirObjetos();
		cerr << "Erro: imagemB nula!" << endl;
		return false; 
	}
	
	cout << "Calculando permeabilidade intrinseca..." << endl;
	CPermeabilidadeIntrinseca permIn;
	permIn.SetarPropriedadesSolver(limiteErro, limiteIteracoes);
	long double permeabilidadeIntrinseca = permIn.Go( matriz3D, fatorAmplificacao, sizePixel, numeroPixelsBorda );
	cout << "Permeabilidade intrinseca = " << permeabilidadeIntrinseca << endl;
	
	// Criando objeto Configurações de Equilíbrio 3D e executando o loop para o cálculo da Permeabilidade Relativa
	confeq = new CConfiguracoesEquilibrio3D ( );			// Cria objeto Configurações de Equilíbrio 3D
	if ( ! confeq  ) { 
		DestruirObjetos();	
		cerr << "Erro: confeq nulo!" << endl;
		return false; 
	} 	// se não criou o objeto, destroi os objetos já criados e retorna false.
	
	confeq->SalvarResultadosFinaisDisco(true);
	// Habilita/desabilita correção da abertura
	
	// Configurar fluidos para drenagem.
	// FluidoA é agua (incompressivel e molhante)
	confeq->fluidoA->Compressibilidade(1);
	confeq->fluidoA->Molhabilidade(0);
	// FluidoB é oleo (incompressivel e não molhante)
	confeq->fluidoB->Compressibilidade(0);
	confeq->fluidoB->Molhabilidade(1);

	cout << "\n ----------> Determinando Configurações de Equilíbrio... (DRENAGEM) <---------- " << endl;
	confeq->Go( matriz3D );
	
	cout << "\n ----------> Invertendo fluxo em Configurações de Equilíbrio... (DRENAGEM -> EMBEBIÇÃO) <---------- " << endl;
	confeq->InverterFluxo( matriz3D );
	confeq->corrigirAbertura = corrigirAbertura;	
	cout << "\n ----------> Determinando Configurações de Equilíbrio e Permeabilidades... (EMBEBIÇÃO) <---------- " << endl;
	
	int passo = 0;
	while ( confeq->Next( matriz3D ) ) { // Para cada passo de pressão...
		passo++;
		cout << "\n----------------------------------------- Executando passo " << passo << " -----------------------------------------" << endl;
		// se não recuperar a imagem corretamente, destroi os objetos já criados e retorna false.
		CMatriz3D * pm3dA = dynamic_cast< CMatriz3D *>( imagemA );
		if ( ! confeq->GetImagem( pm3dA, confeq->GetIndiceRegiaoA()) ) { 
			DestruirObjetos();		
			cerr << "Erro: GetImagem A!" << endl;
			return false; 
		} 
		cout << "\nconfeq->GetImagem(imagemA, confeq->IndiceRegiaoA())" << endl;
		
		// se não recuperar a imagem corretamente, destroi os objetos já criados e retorna false.
		CMatriz3D * pm3dB = dynamic_cast< CMatriz3D *>( imagemB );
		if ( ! confeq->GetImagem( pm3dB, confeq->GetIndiceRegiaoB()) ) { 
			DestruirObjetos();	
			cerr << "Erro: GetImagem A!" << endl;
			return false; 
		}
		cout << "\nconfeq->GetImagem(imagemB, confeq->IndiceRegiaoB())" << endl;
		
		if(salvarImagensAB){
			cout << "\nSalvando imagens A e B" << endl;
			SalvarImagens( );
		}
				
		saturacaoA = confeq->SaturacaoA();
		saturacaoB = confeq->SaturacaoB();
		cout << "\nCalculando Permeabilidade da imagem A no passo " << passo << endl;
// 		permeabilidadeA = a->CalcularPermeabilidade( imagemA );
//isto estava fora do loop. mudei para testes...	
		permA = new CPermeabilidadeIntrinseca ();
		if ( ! permA ) { 
			DestruirObjetos();
			cerr << "Erro: permA nula!" << endl;
			return false; 
		}

		permA->SetarPropriedadesSolver(limiteErro, limiteIteracoes);
		permA->SetarPropriedadesFluido(confeq->fluidoA->Viscosidade(), confeq->fluidoA->Compressibilidade(), confeq->fluidoA->Densidade(), confeq->fluidoA->Molhabilidade()); 
		permeabilidadeA = permA->Go( imagemA );
//---------
		cout << "\nCalculando Permeabilidade da imagem B no passo " << passo << endl;
// 		permeabilidadeB = permB->CalcularPermeabilidade( imagemB );

//isto estava fora do loop. mudei para testes...
		permB = new CPermeabilidadeIntrinseca ();
		if ( ! permB ) { // se não criou o objeto, destroi os objetos já criados e retorna false.
			DestruirObjetos();	
			cerr << "Erro: permB nula!" << endl;
			return false; 
		} 	

		permB->SetarPropriedadesSolver(limiteErro, limiteIteracoes);
		permB->SetarPropriedadesFluido(confeq->fluidoB->Viscosidade(), confeq->fluidoB->Compressibilidade(), confeq->fluidoB->Densidade(), confeq->fluidoB->Molhabilidade());
		permeabilidadeB = permB->Go( imagemB );
//---------
		cout << "Salvando informações em arquivo!" << endl;
		//cout << "permeabilidadeB = " << permeabilidadeB << endl;
		* foutPermRel << setw(14) << left << fixed << setprecision(2) << saturacaoB * 100
				    << setw(10) << right << scientific << setprecision(4) << ( permeabilidadeB / permeabilidadeIntrinseca )
				    //<< setw(19) << right << fixed << setprecision(2) << saturacaoB * 100		// Estou armazenando somente a saturação da agua para ficar no mesmo formato dos dados fornecidos pelo CENPES
				    << setw(19) << right << scientific << setprecision(4) << ( permeabilidadeA / permeabilidadeIntrinseca ) << endl;
		cout << "Fim do passo " << passo << endl;

		//NÃO ESTÁ PERMITINDO DESTRUIR permA e permB!
	
	}
	DestruirObjetos();
	cout << "\nFim das configurações de equilíbrio e permeabilidade relativa!" << endl;
	return true;
}

void CPermeabilidadeRelativa::SalvarImagens( ) {
	static int cont = 1;
	ostringstream outA;
	outA	<< "imagemA-"<< setw(0) << cont << ".dbm";
	imagemA->WriteFormat(WRITEFORM_DI_X_Y_Z_ASCII);
   	imagemA->Write (outA.str());
	
	ostringstream outB;
	outB	<< "imagemB-"<< setw(0) << cont << ".dbm";
	imagemB->WriteFormat(WRITEFORM_DI_X_Y_Z_ASCII);
   	imagemB->Write (outB.str());
   	cont++;
}