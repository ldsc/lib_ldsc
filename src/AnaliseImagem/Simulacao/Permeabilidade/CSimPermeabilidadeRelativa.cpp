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
@file 	  CSimPermeabilidadeRelativa.cpp
@license    GNU General Public License - version 2
            see  $LICENSEFILE$ for the full license text.
*/
#include <AnaliseImagem/Filtro/FEspacial/FEConectividade/TCFEConectividade3D.h>
#include <AnaliseImagem/Simulacao/Permeabilidade/CSimPermeabilidadeRelativa.h>

// Construtor
CSimPermeabilidadeRelativa::CSimPermeabilidadeRelativa ( ofstream & out ) {
	foutPermRel = & out; // Cria arquivo que receberá os dados de permeabilidade relativa

	permA	= nullptr;	// Ponteiro para CSimPermeabilidadeGrafo;
	permB	= nullptr;	// Ponteiro para CSimPermeabilidadeGrafo;
	confeq	= nullptr;	// Ponteiro para CConfiguracoesEquilibrio3D;

	imagemA = new TCImagem3D<int>(); //Estou criando imagens A e B no construtor para poder setar paramentor de recontrução antes da execução de Go()
	imagemB = new TCImagem3D<int>(); //Como as imagens podem ser destruídas em Go(), sempre verificar se são != nullptr;

	// valores usados em CSMDiagonalDominante
	limiteIteracoes = 5000;
	limiteErro = 0.000010;

	salvarImagensAB = false;
	corrigirAbertura = true;
}

//Destrutor
CSimPermeabilidadeRelativa::~CSimPermeabilidadeRelativa () {
	DestruirObjetos();		// destroi os objetos criados.
}

void CSimPermeabilidadeRelativa::DestruirObjetos () {
	//if ( permA   ) { delete permA;	permA	= nullptr; }
	//if ( permB   ) { delete permB;	permB	= nullptr; }
	//COMENTADO PARA NÃO DAR FALHA DE SEGMENTAÇÃO
   if ( confeq  ) { delete confeq;	confeq	= nullptr; }
	if ( imagemA ) { delete imagemA;	imagemA	= nullptr; }
	if ( imagemB ) { delete imagemB;	imagemB	= nullptr; }
}

// Métodos
bool CSimPermeabilidadeRelativa::Go( string pathNomeArquivo ) {
	TCImagem3D<int> * imagem3D = new TCImagem3D<int>( pathNomeArquivo );
	bool retorno = Go( imagem3D );
	delete imagem3D;
	return retorno;
}

bool CSimPermeabilidadeRelativa::Go ( TCImagem3D<int> *imagem3D ) {
	if ( ! imagem3D )
		return false;

	if ( ! imagemA )
		imagemA = new TCImagem3D<int>();

	if ( ! imagemB )
		imagemB = new TCImagem3D<int>();

	imagemA->Path(imagem3D->Path());
	imagemB->Path(imagem3D->Path());
	imagemA->FatorAmplificacao( imagem3D->FatorAmplificacao() );
	imagemB->FatorAmplificacao( imagem3D->FatorAmplificacao() );
	imagemA->NumeroPixelsBorda( imagem3D->NumeroPixelsBorda() );
	imagemB->NumeroPixelsBorda( imagem3D->NumeroPixelsBorda() );
	imagemA->DimensaoPixel( imagem3D->DimensaoPixel() );
	imagemB->DimensaoPixel( imagem3D->DimensaoPixel() );

	return Go( dynamic_cast< TCMatriz3D<int> *>( imagem3D ), imagem3D->FatorAmplificacao(), imagem3D->DimensaoPixel(), imagem3D->NumeroPixelsBorda());
}

bool CSimPermeabilidadeRelativa::Go( string pathNomeArquivo, unsigned int fatorAmplificacao, double dimensaoPixel, unsigned int numeroPixelsBorda ) {
	TCMatriz3D<int> * mat3D = new TCMatriz3D<int>( pathNomeArquivo );
	bool retorno = Go( mat3D, fatorAmplificacao, dimensaoPixel, numeroPixelsBorda );
	delete mat3D;
	return retorno;
}

bool CSimPermeabilidadeRelativa::Go ( TCMatriz3D<int> * matriz3D, unsigned int fatorAmplificacao, double dimensaoPixel, unsigned int numeroPixelsBorda ) {
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
	if ( ! imagemA ) imagemA = new TCImagem3D<int>();
	if ( ! imagemA ) {
		DestruirObjetos();
		cerr << "Erro: imagemA nula!" << endl;
		return false;
	}

	if ( ! imagemB ) imagemB = new TCImagem3D<int>();
	if ( ! imagemB ) {
		DestruirObjetos();
		cerr << "Erro: imagemB nula!" << endl;
		return false;
	}

	cout << "Calculando permeabilidade intrinseca..." << endl;
	CSimPermeabilidadeIntrinseca permIn;
	permIn.SetarPropriedadesSolver(limiteErro, limiteIteracoes);
	long double permeabilidadeIntrinseca = permIn.Go( matriz3D, fatorAmplificacao, dimensaoPixel, numeroPixelsBorda );
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
		TCMatriz3D<int> * pm3dA = dynamic_cast< TCMatriz3D<int> *>( imagemA );
		if ( ! confeq->GetImagem( pm3dA, confeq->GetIndiceRegiaoA()) ) {
			DestruirObjetos();
			cerr << "Erro: GetImagem A!" << endl;
			return false;
		}
		cout << "\nconfeq->GetImagem(imagemA, confeq->IndiceRegiaoA())" << endl;

		// se não recuperar a imagem corretamente, destroi os objetos já criados e retorna false.
		TCMatriz3D<int> * pm3dB = dynamic_cast< TCMatriz3D<int> *>( imagemB );
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
		permA = new CSimPermeabilidadeIntrinseca ();
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
		permB = new CSimPermeabilidadeIntrinseca ();
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

void CSimPermeabilidadeRelativa::SalvarImagens( ) {
	static int cont = 1;
	ostringstream outA;
	outA	<< "imagemA-"<< setw(0) << cont << ".dbm";
	imagemA->SetFormato(D1_X_Y_Z_ASCII);
   	imagemA->Write (outA.str());

	ostringstream outB;
	outB	<< "imagemB-"<< setw(0) << cont << ".dbm";
	imagemB->SetFormato(D1_X_Y_Z_ASCII);
   	imagemB->Write (outB.str());
   	cont++;
}
