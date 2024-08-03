/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================
Desenvolvido por:
			Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:    André Duarte Bueno
File:       CGrafoTest.cpp
begin:      Sat Sep 16 2000
copyright:  (C) 2000 by André Duarte Bueno
email:      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cstdlib> // int system( const char* command );
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include "CGrafoConexaoSerial_Teste.h"

//  -------------------------------------------------- Objetos da LIB_LDSC
#include <MetNum/Matriz/CVetor.h>
#include <MetNum/Matriz/TCMatriz2D.h>
#include <MetNum/Matriz/TCMatriz3D.h>
#include <AnaliseImagem/Filtro/FEspacial/FERotulagem/TCRotulador2D.h>
#include <AnaliseImagem/Filtro/FEspacial/FERotulagem/TCRotulador3D.h>
#include <Tempo/CTime.h>

// ------------------------------------------------------ Calculo do grafo
#include <EstruturaDados/CRedeContorno.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M1.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M2.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M3.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M4.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M5.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M6_Tortuosidade.h>
#include <Base/COperacao.h>

// -----------------------------------------------Calculo da permeabilidade
#include <Amostra/Material/CMFluido.h>
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonalDominante.h>
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_GaussSeidel.h>
#include <MetNum/Solver/SistemaEquacoes/SolverMatrizDiagonal/CSolverMatrizDiagonal_SOR.h>
#include <AnaliseImagem/Simulacao/Permeabilidade/GrafoConexaoSerial/CSimPermeabilidadeGrafo.h>
#include <AnaliseImagem/Simulacao/Permeabilidade/CSimPermeabilidade.h>

// -----------------------------------------------Encrustracao
#include <Encrustracao/CEncrustracao.h>

//  novo
//#include <SMatriz/SMDiagonal/CSolverMatrizDiagonalDominante_Threads.h>
using namespace std;

/*
-------------------------------------------------------------------------
Função:   Run()
-------------------------------------------------------------------------
@short  :	Função de teste das funções da classe CGrafo
@author :	André Duarte Bueno

@param  :	void
@return :	void
*/
void
CGrafoTest::Run ()
{
    int selecao {0};

    do {
        cout << "\nObjeto CGrafoTest"
             << "\nRealiza o teste das classes CGrafoConexaoSerial, CSimPermeabilidade e CEncrustracao"
             << "\nSelecione a opção que deseja testar"
             << "\nGrafo................................1"
             << "\nGrafo->Tortuosidade..................2"
             << "\nGrafo->Permeabilidade................3:"
             << "\nEncrustração/Scale...................4:"
             << "\nEncrustração->Grafo->Permeabilidade..5:"
             << "\nQuit.................................9:";
        cin >> selecao;
        cin.get ();		// pega retorno de carro

        switch ( selecao ) {
        case 1:
            cout << "\n-> Função Grafo()\n";
            Grafo ();
            break;

        case 2:
            cout << "\n-> Função Tortuosidade()\n";
            Tortuosidade ();
            break;

        case 3:
            cout << "\n-> Função Permeabilidade()\n";
            Permeabilidade ();
            break;

        case 4:
            cout << "\n-> Função Encrustracao()\n";
            Encrustracao();
            break;

        case 5:
            cout << "\n-> Função EncrustracaoPermeabilidade()\n";
            EncrustracaoPermeabilidade ();
            break;

        default:
            cout << "\n Saindo da função TCGrafo->Run\n";
            break;
        }
    }
    while ( selecao != 9 );

    cin.clear ();
}

/**
-------------------------------------------------------------------------
Função: Grafo
-------------------------------------------------------------------------
@short  : Função de teste do Grafo, recebe a imagem a partir da qual o
	grafo vai ser gerado, e um inteiro que identifica o método grafo
	a ser utilizado.
@author :	André Duarte Bueno
@return :	bool
*/
bool
CGrafoTest::Grafo ()
{
    string nomeArquivo = SolicitarNomeImagem ();
    TCMatriz3D<int>* pm3D = LerImagemDisco ( nomeArquivo );
    int modelo = SolicitarModeloGrafo ();
    CGrafoConexaoSerial* grafo = CriarGrafo ( modelo, nomeArquivo );
    DeterminarGrafo ( grafo, pm3D, nomeArquivo );
    SalvarGrafo ( grafo );
    delete grafo;			// deleta objeto
    delete pm3D;			// deleta objeto
    return 1;
}

/**
-------------------------------------------------------------------------
Função: Tortuosidade
-------------------------------------------------------------------------
@short  : Função de teste do cálculo da tortuosidade.
@author :	André Duarte Bueno
@return :	bool
*/
bool
CGrafoTest::Tortuosidade ()
{
    string nomeArquivo = SolicitarNomeImagem ();
    TCMatriz3D<int>* pm3D = LerImagemDisco ( nomeArquivo );
    CGrafoConexaoSerial_M6_Tortuosidade* grafo = new CGrafoConexaoSerial_M6_Tortuosidade ( nomeArquivo );
    DeterminarGrafo ( grafo, pm3D, nomeArquivo );
    SalvarGrafo ( grafo );
    grafo->CalcularTortuosidade();
    delete grafo;			// deleta objeto
    delete pm3D;			// deleta objeto
    return 1;
}

/**
-------------------------------------------------------------------------
Função: Permeabilidade
-------------------------------------------------------------------------
@short  : Calcula a permeabilidade a partir do grafo
	  Solicita o nome do arquivo de disco com a imagem e o modelo a ser utilizado.
	  Le do disco dados da imagem, e do solver.
@author : André Duarte Bueno
@param  : int tipo de solver
@return : double (a permeabilidade)
*/
double CGrafoTest::Permeabilidade ( int tipoSolver )
{
    // Determinação do grafo
    MostrarInstrucoesArquivosExternos ();

    string nomeArquivo = SolicitarNomeImagem ();
    int fatorAmplificacao = 2;
    double dimensaoPixel = 5.0e-6;
    LerPropriedadesImagemDoDisco ( fatorAmplificacao, dimensaoPixel, nomeArquivo );
    TCMatriz3D<int>* pm3D { nullptr };
    pm3D = LerImagemDisco ( nomeArquivo );

    int modelo = SolicitarModeloGrafo ();
    CGrafoConexaoSerial* grafo { nullptr } ;
    grafo = CriarGrafo ( modelo, nomeArquivo );
    DeterminarGrafo ( grafo, pm3D, nomeArquivo );
    SalvarGrafo ( grafo );

    // Determinação da Permeabilidade
    CMFluido* fluido { nullptr };
    fluido = CriarFluido ();

    CSolverMatrizDiagonalDominante* solver { nullptr };
    solver = CriarSolver ( tipoSolver );

    CSimPermeabilidadeGrafo* permeabilidade { nullptr };
    permeabilidade = CriarPermeabilidade ( fluido, solver, grafo, pm3D, fatorAmplificacao, dimensaoPixel );

    double permeabilidade_calculada = DeterminarPermeabilidade ( permeabilidade );

    // Resolvido o sistema salva tudo em disco
    string nome = permeabilidade->Grafo()->NomeGrafo()+ ".permeabilidade";
    ofstream fout ( nome );
    fout << *permeabilidade;
		fout << "\n\nPermeabilidade = " << permeabilidade_calculada << endl;
    fout.close();

    delete pm3D;
    delete permeabilidade;
    return permeabilidade_calculada;
}

double CGrafoTest::PermeabilidadeImagem ( TCImagem3D<int>* pimg3D, int modeloGrafo, string nomeArquivoGrafo, bool salvarGrafo , int tipoSolver)
{
    // Determinação do grafo
    //MostrarInstrucoesArquivosExternos ();
    CGrafoConexaoSerial* grafo { nullptr } ;
    grafo = CriarGrafo ( modeloGrafo, nomeArquivoGrafo );
    DeterminarGrafo ( grafo, pimg3D, nomeArquivoGrafo ); //
    if(salvarGrafo)
      SalvarGrafo ( grafo );

    // Determinação da Permeabilidade
    CMFluido* fluido { nullptr };
    fluido = CriarFluido ();

    CSolverMatrizDiagonalDominante* solver { nullptr };
    solver = CriarSolver ( tipoSolver );

    CSimPermeabilidadeGrafo* permeabilidade { nullptr };
    permeabilidade = CriarPermeabilidade ( fluido, solver, grafo, pimg3D, pimg3D->FatorAmplificacao(), pimg3D->DimensaoPixel() );

    double permeabilidade_calculada = DeterminarPermeabilidade ( permeabilidade );

    // Resolvido o sistema salva tudo em disco
    string nome = permeabilidade->Grafo()->NomeGrafo()+ ".permeabilidade";
    ofstream fout ( nome );
    fout << *permeabilidade;
    fout << "\n\nPermeabilidade = " << permeabilidade_calculada << endl;
    fout.close();

    delete permeabilidade;
    return permeabilidade_calculada;
}
/*
-------------------------------------------------------------------------
Função: MostrarInstrucoesArquivosExternos()
-------------------------------------------------------------------------
*/
void
CGrafoTest::MostrarInstrucoesArquivosExternos ()
{
cout
	<< "\n\n----------------------------------------------------------"
	<< "\nOs métodos para geração do grafo/rede/esqueleto e cálculo da permeabilidade ou tortuosidade, "
	<< "\nusam o método CGrafoTest::Grafo ou CGrafoTest::Permeabilidade()."
	<< "\n\nEste último requer que os seguintes dados estejam armezenados no diretório input_data:\n"
	<< "\nAtributos do objeto fluido     em:  \n\tinput_data/fluido.info\n\t\tviscosidade"
	<< "\nAtributos do objeto solverSOR  em:  \n\tinput_data/solver.info "
	<< "\n\t\tfatorRelaxação, LimiteIterações, LimiteErro,"
	<< "\n\t\ttipoSolver(1=CSolverMatrizDiagonal_GaussSeidel,2=CSolverMatrizDiagonal_SOR,3=CSolverMatrizDiagonalDominante_Threads),"
	<< "\n\t\t\tnproc - NÃO IMPLEMENTADO)"
	<< "\nAtributos do objeto solverPerm em:  \n\tinput_data/solver_permeabilidade.info (LimiteIterações,LimiteErro%)"
	<< "\nO arquivo com as informações da imagem (fatorAmplificação e dimensaoPixel)"
	<< "deve estar armazenado em path/nomeImagem.ext.dat"
	<< "\nObserve que é o mesmo nome da imagem (devendo ser adicionada a extensão .dat) "
	<< "\nAlguns resultados são salvos em: "
	<< "\npath/nomeImagem.modelo.grafo.log e path/nomeImagem.modelo.perm.log"
	<<  endl;
}

/*
-------------------------------------------------------------------------
Função: SolicitarNomeImagem()
-------------------------------------------------------------------------
*/
string
CGrafoTest::SolicitarNomeImagem ()
{
    cout << "\nEntre com o nome da imagem:"
         << "\nExemplo:\n";
		int r = system ("dir *.txt *.pm3");
		r = r; //evita warning
    string nomeArquivo;
    cout << "\n: ";
    getline ( cin, nomeArquivo );
    return nomeArquivo;
}

/*
-------------------------------------------------------------------------
Função: LerPropriedadesImagemDoDisco
-------------------------------------------------------------------------
*/
//Antes void CGrafoTest::SolicitarPropriedadesImagem ( int& fatorAmplificacao,double& dimensaoPixel, string nomeArquivo )
void
CGrafoTest::LerPropriedadesImagemDoDisco ( int& fatorAmplificacao,
        double& dimensaoPixel, string nomeArquivo )
{
    string nomeArquivoDat ( nomeArquivo + ".dat" );
    cout << "\n\nVai ler as propriedades fatorAmplificacao e dimensaoPixel do arquivo "
         << nomeArquivoDat << endl;

    ifstream finimg3D ( nomeArquivoDat.c_str () );
    string msg;

    if ( finimg3D /*.good() */ ) {
        finimg3D >> fatorAmplificacao;
        finimg3D >> dimensaoPixel;
        finimg3D.close ();
        msg = " ...done";
    }
    else {
        msg = " ...não abriu o arquivo de disco, \nusando valores:";
    }

    cout << msg << endl
         << "fa=" << setw ( 3 ) << fatorAmplificacao << " sp=" << setw ( 8 ) << dimensaoPixel << endl;
}

/*
-------------------------------------------------------------------------
Função: LerImagemDisco
-------------------------------------------------------------------------
*/
TCMatriz3D<int>*
CGrafoTest::LerImagemDisco ( string nomeArquivo )
{
    TCMatriz3D<int>* pm3D;
    {
        CTime ( "Tempo leitura imagem = ", &cout );
        cout << "Lendo imagem (" << nomeArquivo << ") do disco...\n";
        pm3D = new TCMatriz3D<int> ( nomeArquivo );
    }
    return pm3D;
}

/*
-------------------------------------------------------------------------
Função: SolicitarModeloGrafo()
-------------------------------------------------------------------------
*/
int
CGrafoTest::SolicitarModeloGrafo ()
{
    cout << "\n\n---------------------------------------------------------------------------"
         << "\nselecione um dos modelos para cálculo da permeabilidade:"
         << "\nModelo1: CGrafoConexaoSerial_M1........................................1"
         << "\n(condutâncias dada por um feixe de tubos, cada pixel uma cconexão)"
         << "\nModelo2: CGrafoConexaoSerial_M2......................................2"
         << "\n(condutâncias média entre dois sítios, super-estima a permeabilidade)"
         << "\nModelo3: CGrafoConexaoSerial_M3..........................................3"
         << "\n(condutâncias definida pela área de intersecção dos dois sítios)"
         << "\nModelo4: CGrafoConexaoSerial_M4..........................................4"
         << "\n(como o modelo 3, com correção da distância entre os sitios)"
         << "\nModelo5: CGrafoConexaoSerial_M5..........................................5"
         << "\n(como o modelo 4, com eliminação píxeis sem intersecção plano anterior ou posterior)"
         << "\nModelo6: CGrafoConexaoSerial_M6..........................................6"
         << "\n(como o modelo 4, mas com correção da distância entre os sitios limitada a 2;"
         << "\nAs coordenadas {cx,cy,cz} são armazenadas no sítio - esqueleto!)"
         << "\nInclui cálculo da Tortuosidade!)"
         << endl;

    int modelo {3}; // usar ETipoGrafo!
    cin >> modelo;
    cin.get ();
    cout << "Selecionou a opção: " << modelo << endl
         <<  "\n\n----------------------------------------------------------"
         << endl;
    return modelo;
}

/*
-------------------------------------------------------------------------
Função: CriarGrafo
-------------------------------------------------------------------------
*/
CGrafoConexaoSerial*
CGrafoTest::CriarGrafo ( int modelo, string nomeArquivo )
{
    cout << "Criando objeto grafo...";
    CGrafoConexaoSerial* grafo;
    {
        CTime ( "Tempo Criação do grafo = ", &cout );
        string nomeArquivoExt ( nomeArquivo );

        switch ( modelo ) {
        case 1:
            cout << "...Criando objeto CGrafoConexaoSerial_M1...";
            grafo = new CGrafoConexaoSerial_M1 ( nomeArquivoExt );
            break;

        case 2:
            cout << "...Criando objeto CGrafoConexaoSerial_M2...";
            grafo = new CGrafoConexaoSerial_M2 ( nomeArquivoExt );
            break;

        case 3:
            cout << "...Criando objeto CGrafoConexaoSerial_M1...";
            grafo = new CGrafoConexaoSerial_M3 ( nomeArquivoExt );
            break;

        case 4:
            cout << "...Criando objeto CGrafo_3Dby2_M4...";
            grafo = new CGrafoConexaoSerial_M4 ( nomeArquivoExt );
            break;

        case 5:
            cout << "...Criando objeto CGrafoConexaoSerial_M5...";
            grafo   = new CGrafoConexaoSerial_M5 ( nomeArquivoExt );
            break;

        case 6:
            cout << "...Criando objeto CGrafoConexaoSerial_M6...";
            grafo = new CGrafoConexaoSerial_M6_Tortuosidade ( nomeArquivoExt );
            break;

        default:
            cout << "...Criando objeto CGrafoConexaoSerial_M3...";
            grafo = new CGrafoConexaoSerial_M3 ( nomeArquivoExt );
            break;
        };

        assert ( grafo );

        cout << " ...done\n";
    }
    return grafo;
}

/*
-------------------------------------------------------------------------
Função: DeterminarGrafo
-------------------------------------------------------------------------
*/
void
CGrafoTest::DeterminarGrafo ( CGrafoConexaoSerial* grafo, TCMatriz3D<int>* pm3D, string nomeArquivo )
{
    CTime ( "Tempo Determinação do grafo (imagem previamente carregada)= ", &cout );
    cout << "Determinando o grafo da imagem(" << nomeArquivo << ")..." << "\nChamando o grafo->Go()\n";
    grafo->Go ( pm3D );
    cout << "\nDimensão do grafo = " << grafo->objeto.size () <<  "\n ...done\n";
}

/*
-------------------------------------------------------------------------
Função: DeterminarGrafo
-------------------------------------------------------------------------
*/
void
CGrafoTest::DeterminarGrafo ( CGrafoConexaoSerial* grafo, string nomeArquivo )
{
    CTime* t = new CTime ( "Tempo abertura imagem e determinação do grafo = ", &cout );
    cout << "Determinando o grafo da imagem(" << nomeArquivo << ")..." << "\nChamando o grafo->Go()\n";
    grafo->Go ( nomeArquivo );
    delete t;				// deleta objeto
    cout << "\nDimensão do grafo = " << grafo->objeto.size () << endl  << " ...done\n";
}

// Gera vetor e matriz para solver externo
//       {
//       CMatriz2D* A = new CMatriz2D(pm3D->NX(),pm3D->NX());
//       A->Constante(0);
//       CVetor* B = new CVetor(pm3D->NX());
//       B->Constante(0);
//       grafo->SetarMatrizAVetorB(A,B);
//       }

/*
-------------------------------------------------------------------------
Função: SalvarGrafo
-------------------------------------------------------------------------
*/
void
CGrafoTest::SalvarGrafo ( CGrafoConexaoSerial* grafo )
{
    grafo->Write ();
}


/*
-------------------------------------------------------------------------
Função: CriarFluido
-------------------------------------------------------------------------
*/
CMFluido* CGrafoTest::CriarFluido ()
{
    // Solicita propriedades do fluido
    cout << "Vai ler a propriedade viscosidade do arquivo input_data/fluido.info...";
    long double viscosidade = 0.001002;
    ifstream finfluido ( "input_data/fluido.info" );
    string msg = " ...não abriu o arquivo de disco, \nusando valores:";

    if ( finfluido ) {
        finfluido >> viscosidade;
        finfluido.close ();
        msg = " ...done";
    }
    cout << msg << endl;

    // Cria objeto fluido
    cout << "Criando objeto fluido...";
    CMFluido* fluido = new CMFluido ( viscosidade );
    assert ( fluido );
    cout << " ...done\n";
    cout << "fluido.Viscosidade()=" << fluido->Viscosidade () << endl;
    return fluido;
}


/*
-------------------------------------------------------------------------
Função: CriarSolver
-------------------------------------------------------------------------
*/
CSolverMatrizDiagonalDominante*
CGrafoTest::CriarSolver ( int tipoSolver )
{
    // Solicita propriedades do solver
    cout << "\n\nVai ler as propriedades fatorRelaxacao,limiteIteracoes,limiteErro nproc"
         << "\ndo arquivo input_data/solver.info...";
    long double fatorRelaxacao = 0.7;
    unsigned long int limiteIteracoes = 100000;
    long double limiteErro = 1.0e-6;
    int nproc = 1;		// novo ->testar!
    ifstream finsolver ( "input_data/solver.info" );
    string msg = " ...não abriu o arquivo de disco, \nusando valores default";

    if ( finsolver /*.good() */ ) {
        finsolver >> fatorRelaxacao;
        finsolver >> limiteIteracoes;
        finsolver >> limiteErro;
        finsolver >> tipoSolver;
        finsolver >> nproc;
        finsolver.close ();
        msg = " ...done";
    }

    cout << msg << endl;

    cout << "Criando objeto CSolverMatrizDiagonalDominante* solver= \n1=CSolverMatrizDiagonal_GaussSeidel,"
         << "\n2=CSolverMatrizDiagonal_SOR,\n3=CSolverMatrizDiagonalDominante_Threads"
         << "\nUsando opção:" << tipoSolver << endl;
    CSolverMatrizDiagonalDominante* solver;

    // Cria objeto solver
    switch ( tipoSolver ) {
    case 1:
        solver =  new CSolverMatrizDiagonal_GaussSeidel ( limiteIteracoes, limiteErro ) ;
        break;

    case 2:
        solver =  new CSolverMatrizDiagonal_SOR ( limiteIteracoes, limiteErro, fatorRelaxacao ) ;
        break;

    case 3:
    default:
        /*2009      solver =
        	static_cast < CSolverMatrizDiagonalDominante * >(new CSolverMatrizDiagonalDominante_Threads
        					       (nproc, fatorRelaxacao, limiteIteracoes, limiteErro));
              */
        solver =  new CSolverMatrizDiagonal_SOR ( limiteIteracoes, limiteErro, fatorRelaxacao ) ;
        break;
    }

    // CSolverMatrizDiagonalDominante_ThreadsExec::nproc = nproc;
    // CSolverMatrizDiagonalDominante_ThreadsExec::fatorRelaxacao = fatorRelaxacao;
    // CSolverMatrizDiagonalDominante_ThreadsExec::fatorRelaxacaoC = 1 - fatorRelaxacao;
    assert ( solver );
    cout << "\n ...done"
         // cout << "fatorRelaxacao=" <<  fatorRelaxacao
         << "\nfatorRelaxacao="  << fatorRelaxacao
         << "\nlimiteIteracoes=" << solver->LimiteIteracoes ()
         << "\nlimiteErro="      << solver->LimiteErro ()
         // cout << "nproc="      <<    CSolverMatrizDiagonalDominante_ThreadsExec::nproc
         << "\ntipo solver = "   << tipoSolver
         << "\nnproc="           << nproc << endl;
    return solver;
}

/*
-------------------------------------------------------------------------
Função: CriarPermeabilidade
-------------------------------------------------------------------------
*/
CSimPermeabilidadeGrafo* CGrafoTest::CriarPermeabilidade ( CMFluido* fluido,
        CSolverMatrizDiagonalDominante* solver,     // CSolverMatrizDiagonalDominante_Threads* solver,
        CGrafoConexaoSerial* grafo, TCMatriz3D<int>* pm3D, int fatorAmplificacao,
        double dimensaoPixel )
{
    CSimPermeabilidadeGrafo* permeabilidade_grafo { nullptr };
    {
        CTime ( "Tempo criação do CSimPermeabilidadeGrafo = ", &cout );
        cout << "Criando objeto CSimPermeabilidadeGrafo (fluido,solver,grafo,nx,ny,nz,fatorAmplificacao,sizePizel)...";
        // CSolverMatrizDiagonalDominante* solverold = static_cast<CSolverMatrizDiagonalDominante*>(solver);// novo
        permeabilidade_grafo = new CSimPermeabilidadeGrafo ( fluido, solver /*old */ , grafo,
                pm3D->NX (), pm3D->NY (), pm3D->NZ (),
                fatorAmplificacao, dimensaoPixel );
        assert ( permeabilidade_grafo );
        cout << " ...done\n";
    }

    return permeabilidade_grafo;
}

/*
-------------------------------------------------------------------------
Função: DeterminarPermeabilidade
-------------------------------------------------------------------------
*/
double CGrafoTest::DeterminarPermeabilidade ( CSimPermeabilidadeGrafo* permeabilidade )
{
    {
        CTime ( "Tempo permeabilidade->SolucaoSistema(); = ", &cout );
        cout << "Chamando permeabilidade->SolucaoSistema() "
             << "que realiza pré-processamento e primeira iteração do Solver..."
             << endl;
        permeabilidade->SolucaoSistema ();
        cout << " permeabilidade->SolucaoSistema()...done\n";
    }
    // Determinação da permeabilidade---------------------------------------
    double permeabilidade_calculada;
    {
        CTime ( "Tempo permeabilidade->Go() = ", &cout );
        cout << "Chamando permeabilidade->Go()...\n";
        permeabilidade_calculada = permeabilidade->Go ();
        cout << " permeabilidade->Go()... ...done\n";
        cout << "\a\a\nPermeabilidade (mD)= " << permeabilidade_calculada << endl;
    }

    return permeabilidade_calculada;
}

/*
Função usada para testar o solver
*/
/*
void CGrafoTest::teste_solver()
{
	//
	cout<<"\nVersão específica para teste do solver SOR"
	<<"\nEntrada: imagem3D chamada 'input.dat' (Berea500 fa=6,sp=2.5e-6 visc=0.001002)"
	<<"\nSaida  : 'solver.log.txt'"
	<<"\nfator de relaxação:  .1 -> .5  [só se aplica para o SOR]"
	<<"\nLembre-se: Xi=fr*Xiant + (1-fr)*Xi"
	<<"\nerro do solver    :  0.01 (1e-2) -> 1.0e-12"
	<<"\nEntre com o tipo de solver (0=SOR,1=gauss seidel):"<<endl;
	int tipoSolver=0;
	cin >>tipoSolver; cin.get();

	// Criação do arquivo de disco com o log das simulações realizadas
	ofstream fout("solver.log.txt");
	ofstream fout2("solver_FR_LES_PERM.log.txt");
	char nomeArquivo[256];
	strcpy(nomeArquivo,"input.dat");

  TCMatriz3D<int>* pm3D;
  // Abre o arquivo de disco e lê a imagem Resultados
	{
	CTime* t = new CTime("Tempo leitura imagem = ",&fout);
	pm3D = new TCMatriz3D<int>(nomeArquivo);
  assert(pm3D);
	}

	// Definição das propriedades
  int 	fatorAmplificacao = 6;			// da imagem
  double dimensaoPixel = 2.5e-6;
  long double viscosidade = 0.001002;		// do fluido
  unsigned long int limiteIteracoes = 100;

// INICIO loop for
 for ( int fr = 1 ; fr <= 10 ; fr = fr + 2 )
 {
  long double limiteErro = 1.0;// vai decrescer até e-12
  if(tipoSolver==1)// gaussSeidel
  	fr = 12;// garante uma única passagem

 	while ( limiteErro > 1.0e-12 )
 	{// Inicio loop while
	time_t  ti,tf;
	ti = time(NULL);

 	limiteErro = limiteErro / 100.0;
	long double fatorRelaxacao= 0.1*(float)fr; // 0.3,  ,0.5,  ,0.7, ,0.9
	// long double limiteErro = 1.0 / (float)le;

  // Criação dos objetos agregados
  CGrafoConexaoSerial * 	grafo	;
  CMFluido* fluido;
  CSolverMatrizDiagonalDominante* solver;
  CSimPermeabilidadeGrafo* permeabilidade ;
	{
	CTime* t = new CTime("Tempo criação objetos agregados = ",&fout);
	fluido = new CMFluido(viscosidade);
	if(tipoSolver==1)
	  solver = new CSolverMatrizDiagonal_GaussSeidel(limiteIteracoes,limiteErro);
	else
	  solver = new CSolverMatrizDiagonal_SOR(limiteIteracoes,limiteErro,fatorRelaxacao);

	char nomeArquivoExt[256];
	sprintf(nomeArquivoExt,"%s.mod4_FR%i_LES%e.txt",nomeArquivo,fr,(float)limiteErro);
	grafo	= new CGrafo_3Dby2_M4(nomeArquivoExt);
	permeabilidade =
  		new CSimPermeabilidadeGrafo(fluido,solver,grafo, pm3D->NX(),pm3D->NY(),pm3D->NZ(),
		fatorAmplificacao,dimensaoPixel);
	}

	{
	CTime* t = new CTime("Tempo determinação do grafo = ",&fout);
	grafo->Go(pm3D);
	fout<<"\nDimensão do TGRAFO="<<grafo->objeto.size()<<endl;
	}

	double permeabilidade_calculada;
	{
	// CTime("Tempo solução do sistema = ",&fout2);
	CTime* t = new CTime("Tempo solução do sistema = ",&fout);
	CTime* t = new CTime("Tempo solução do sistema = ",&cout);
	permeabilidade->SolucaoSistema();
	permeabilidade_calculada = permeabilidade->Go();
	}
	// Informações dos atributos do solver
	tf = time(NULL);
	cout<<"------>fr="	<<	fatorRelaxacao<< " LES="<<limiteErro<<" perm(M4_mD)="
			<<permeabilidade_calculada
	<<" erroPer="<<permeabilidade->ErroPermeabilidade()	<< " time="<<(difftime(tf,ti)) <<endl;
	fout<<"------>fr="	<<	fatorRelaxacao<< " LES="<<limiteErro<<" perm(M4_mD)="
		<<permeabilidade_calculada
	<<" erroPer="<<permeabilidade->ErroPermeabilidade()	<< " time="<<(difftime(tf,ti)) <<endl;
	fout2<<"------>fr="	<<	fatorRelaxacao<< " LES="<<limiteErro<<" perm(M4_mD)="
		<<permeabilidade_calculada
	<<" erroPer="<<permeabilidade->ErroPermeabilidade()	<< " time="<<(difftime(tf,ti)) <<endl;

	{
	CTime* t = new CTime("Tempo destruição dos objetos = ",&fout);
  delete permeabilidade;
	}
  };// fim loop while
 }// fim loop for
  delete pm3D;
}
*/

/**
-------------------------------------------------------------------------
Função: ProcessarListaImagens
-------------------------------------------------------------------------
Função executada por main
Se foi passado um parâmetro o mesmo é o nome do arquivo com a lista de imagens

PROBLEMA a enterprise nunca termina uma simulação grande, sempre cai.
Solução:Implementar método que permita a releitura do vetor de dados do sistema de equações.
Como implementar:
1-Ao iniciar   a simulação verifica se existe um arquivo nomeImagem.inicio
nomeImagem.meio e nomeImagem.fim.
2-Ao finalizar a simulação cria arquivo nomeImagem.fim
*/

bool CGrafoTest::ProcessarListaImagens ( unsigned int argc, char* argv[] )
{
    // Lista as imagens------------------------------------------------------
    // Se passou parâmetro, é o nome do arquivo com a Lista de imagens
    ifstream fin;
    fin.open ( argv[1] );

    if ( !fin ) {
        cout << "\nNão conseguiu abrir o arquivo de disco = "
             << argv[1] << endl;
        return 0;
    }

    // Vai percorrer a lista de imagens e mostrar o nome na tela (para verificação)
    string nomeArquivo;
    cout << "=============================================================\n"
         << "Vai determinar a permeabilidade das imagens:\n"
         << "============================================================="
         << endl;
    bool flagEnd = false;

    do {
        // Lê o nome das imagens do arquivo passado,
        getline ( fin, nomeArquivo );

        // se encontrar um string =end ou string==fim, encerra a leitura do arquivo.
        if ( nomeArquivo == "end" || nomeArquivo == "fim" ) {
            flagEnd = true;
            continue;
        }

        cout << nomeArquivo << endl;
    }
    while ( fin.eof () == false && flagEnd == false );

    fin.close ();

    // Abre arquivo saída------------------------------------------------
    string arquivoSaida ( "ResultadosPermeabilidade.txt");
    cout << "Arquivo saida=" << arquivoSaida << endl;
    ofstream fout ( arquivoSaida.c_str (), ios::app );

    if ( !fout ) {
        cerr << "\nFalha abertura do arquivo de disco->"
             << arquivoSaida
             << endl;
        return 0;
    }

    // Abre imagem do disco-----------------------------------------------
    // Abre a imagem do disco, e chama as funções de cálculo
    // da permeabilidade para os diferentes modelos
    fin.open ( argv[1] );
    flagEnd = false;

    do {
        getline ( fin, nomeArquivo );

        // Se for end ou fim, pula para práximo laço while (próxima imagem)
        if ( nomeArquivo == "end" || nomeArquivo == "fim" ) {
            flagEnd = true;
            continue;
        }
        // Se não for end realiza o processamento da imagem
        else {
            string msg ( "Tempo total processamento da imagem (" + nomeArquivo + " = " );
            CTime ( msg, &cout );
            TCMatriz3D<int>* pm3D = NULL;
            cout << "=============================================================\n"
                 << "Lendo Imagem (" << nomeArquivo << ") do disco\n";
            fout << "=============================================================\n"
                 << "Lendo Imagem (" << nomeArquivo << ") do disco\n";
            {
                ifstream teste ( nomeArquivo.c_str () );

                if ( teste.fail () ) {
                    cout << "\n A Imagem " << nomeArquivo << " é inválida (Nome errado?).\a\n";
                    fout << "\n A Imagem " << nomeArquivo << " é inválida (Nome errado?).\n";
                    teste.close ();	// ?
                    continue;	// Passa para proximo passo do laço while ? confirmar->ok
                }
                else {
                    cout << "\n A Imagem " << nomeArquivo << " é válida.\a\n";
                    teste.close ();

                    // -->Originalmente carregava a imagem do disco aqui
                    // CTime* t = new CTime("Demora na abertura da imagem do disco = ",&cout);
                    // cout<<"Criando img3D ..."<<endl;
                    // pm3D = new TCMatriz3D<int>(nomeArquivo);
                    // assert(pm3D);
                    // pm3D=NULL;
                    // cout <<"...done";
                    // -->Agora a imagem e carregada na função DeterminarGrafo(grafo,nomeArquivo)
                    // que abre a imagem e le plano a plano
                }
            }

            // Chama função de decisão do cálculo da permeabilidade
            Permeabilidade_By_ModelX_Decisao ( nomeArquivo , 1 , pm3D , fout ); // modelo 1
            cout << "\a";
            Permeabilidade_By_ModelX_Decisao ( nomeArquivo , 2 , pm3D , fout ); // modelo 2
            cout << "\a";
            Permeabilidade_By_ModelX_Decisao ( nomeArquivo, 3, pm3D, fout );	// modelo 3
            cout << "\a";
            Permeabilidade_By_ModelX_Decisao ( nomeArquivo , 4 , pm3D , fout ); // modelo 4
            cout << "\a";
            Permeabilidade_By_ModelX_Decisao ( nomeArquivo , 5 , pm3D , fout ); // modelo 5
            cout << "\a";
            Permeabilidade_By_ModelX_Decisao ( nomeArquivo , 6 , pm3D , fout ); // modelo 6
            cout << "\a";
        }
    }
    while ( fin.eof () == false && flagEnd == false );

    fout.close ();
    fin.close ();
    return 1;
}

/*
-------------------------------------------------------------------------
Função: Permeabilidade_By_ModelX_Decisao
-------------------------------------------------------------------------
Resolve se é para iniciar a simulação
ou para continuar uma simulação que não terminou
*/
bool CGrafoTest::Permeabilidade_By_ModelX_Decisao ( string nomeArquivo, int modelo,
        TCMatriz3D<int>* pm3D, ofstream& fout )
{
    // Variáveis auxiliares
    ifstream in;
    bool reiniciar = false;

    // Definição do nome da imagem, considerando o modelo
    ostringstream os;
    os << nomeArquivo << ".mod" << modelo;
    string nomeComModelo ( os.str () );

    // Simulação para a imagem já foi ENCERRADA, existe o arquivo NomeImagem.modi.fim
    // Pula para proxima imagem
    string nomeTeste;
    nomeTeste = nomeComModelo + ".fim";
    in.open ( nomeTeste.c_str () );

    if ( in ) {
        cout << "\n--->Imagem ("
             << nomeComModelo
             << ") já foi simulada, pulando para práxima\n";
        return 1;
    }

    // Simulação para a imagem já foi INICIADA, existe o arquivo NomeImagem.meio
    // Precisa setar para reiniciar de onde parou
    nomeTeste = nomeComModelo + ".meio";
    in.open ( nomeTeste.c_str () );
    int tipoSolver = 2;		// 0;// 0=SOR, 1=GaussSeidel //AQUI AQUI

    if ( in ) {
        reiniciar = true;
        // Determina a permeabilidade reiniciando de onde havia parado
        Permeabilidade_By_ModelX ( nomeArquivo, modelo, pm3D,
                                   fout, tipoSolver, reiniciar );
        // CriaArquivoFim
        nomeTeste = nomeComModelo + ".fim";
        ofstream fim ( nomeTeste.c_str () );
        fim << "Simulação para a imagem ( " << nomeArquivo << " ) Finalizada";
        fim.close ();
        return 1;
    }

    // Simulação para a imagem NÃO FOI INICIADA, precisa criar arquivo meio
    ofstream meio ( nomeTeste.c_str () );
    meio << "Simulação para a imagem ( "
         << nomeComModelo
         << " ) iniciada , e em andamento...";
    meio.close ();

    Permeabilidade_By_ModelX ( nomeArquivo, modelo, pm3D, fout, tipoSolver );

    // CriaArquivoFim
    nomeTeste = nomeComModelo + ".fim";
    ofstream fim ( nomeTeste.c_str () );
    fim << "Simulação para a imagem ( " << nomeArquivo << " ) Finalizada";
    fim.close ();
    return 1;
}

/*
-------------------------------------------------------------------------
Função: Permeabilidade_By_ModelX
-------------------------------------------------------------------------
Recebe o nome da imagem a ser processada, o modelo a imagem , um fout, e um flag de reinicialização.
*/
bool
CGrafoTest::Permeabilidade_By_ModelX ( string nomeArquivo,
                                       int modelo,
                                       TCMatriz3D<int>* pm3D,
                                       ofstream& fout,
                                       int tipoSolver,
                                       bool reiniciar /* =false */ )
{
    CTime ( "Tempo total execução da função  Permeabilidade_By_ModelX = ",     &cout );

    // Solicita propriedades da Imagem3D------------------------------------
    int fatorAmplificacao = 1;
    double dimensaoPixel = 5e-6;
    LerPropriedadesImagemDoDisco ( fatorAmplificacao, dimensaoPixel, nomeArquivo );
    CMFluido* fluido = CriarFluido ();

    CSolverMatrizDiagonalDominante* solver = CriarSolver ();	// vai usar default=3

    CGrafoConexaoSerial* grafo = CriarGrafo ( modelo, nomeArquivo );
    // Determina o grafo passando o nome da imagem

    DeterminarGrafo ( grafo, nomeArquivo );

    SalvarGrafo ( grafo );

    // SE FOI SETADA PARA REINICIAR A SIMULAÇÃO, Lê O VETOR DE DADOS DO DISCO
    if ( reiniciar ) {
        CTime ( "Tempo leitura do vetor de dados do grafo = ", &cout );
        cout << "\n\aLendo vetor com dados da simulação inacabada do disco " <<
             endl;
        cout << "(reiniciando simulação anterior)\n";
        // Lê   do arquivo grafo.vectorX (o vetor solução X)
        grafo->LerVetorPropriedades_x ();
    }

    // Cria permeabilidade------------------------------------------------------
    // aqui
    CSimPermeabilidadeGrafo* permeabilidade = nullptr;
    {
        CTime ( "Tempo criação objeto  CSimPermeabilidadeGrafo = ", &cout );
        permeabilidade =  new CSimPermeabilidadeGrafo ( fluido, solver, grafo, grafo->Nx (),
                grafo->Ny (), grafo->Nz (), fatorAmplificacao, dimensaoPixel );
        assert ( permeabilidade );
    }

    // Solução do sistema de equações(primeira passagem)------------------------
    time_t tiPermeabilidade, tfPermeabilidade;
    tiPermeabilidade = time ( NULL );
    {
        CTime ( "Tempo permeabilidade->SolucaoSistema() = ", &cout );
        cout << "Chamando permeabilidade->SolucaoSistema() que realiza \n";
        cout << "pré-processamento e primeira iteração do Solver...";
        permeabilidade->SolucaoSistema ();
        cout << " permeabilidade->SolucaoSistema()...done\n";
    }
    // Determinação da permeabilidade-------------------------------------------
    double permeabilidade_calculada;
    {
        CTime ( "Tempo permeabilidade->Go() = ", &cout );
        permeabilidade_calculada = permeabilidade->Go ();
        tfPermeabilidade = time ( NULL );
    }
// #ifdef DEBUG
    {
        CTime ( "Tempo salvamento grafo em disco = ", &cout );
        grafo->Write ();
    }
// #endif
// // // // // // // // // // // // // // // // // // // // /
    /*
    if ( modelo == 2 || modelo==3 )		// novo
    	{
    	CMatriz2D* A = new CMatriz2D(1,1);
    	// A->Write("R_A_1.txt");
    	CVetor* B    = new CVetor(1);
    	// B->Write("R_B_1.txt");
    	// cout << "\nVai executar a função 	grafo->SetarMatrizAVetorB(A,B);\n" ;
    	// cin.get();
    	grafo->SetarMatrizAVetorB(A,B);
    	// cout << "\nExecutou a função 	grafo->SetarMatrizAVetorB(A,B);\n" ;
    	}
    */
// // // // // // // // // // // // // // // // // // // //
    // cout<<" ...done"<<endl;
    cout <<
         "\n__________________________________________________________________________________";
    cout << "\nM" << modelo << " P(mD)=" << setw ( 8 ) << permeabilidade_calculada
         << " E(%)=" << setw ( 4 ) << permeabilidade->ErroRelativo ()
         << " it=" << setw ( 5 ) << permeabilidade->Iteracoes ()
         << " time(s)=" << ( difftime ( tfPermeabilidade, tiPermeabilidade ) )
         << "\n_________________________________________________________________________________"
         << endl;
    fout << "\nM" << modelo << " P(mD)=" << setw ( 8 ) << permeabilidade_calculada
         << " E(%)=" << setw ( 4 ) << permeabilidade->ErroRelativo ()
         << " it=" << setw ( 5 ) << permeabilidade->Iteracoes ()
         << " time(s)=" << ( difftime ( tfPermeabilidade, tiPermeabilidade ) ) << endl;

    delete permeabilidade;	// Destróe objeto permeabilidade (que destróe o grafo)
    return 1;
}


/**
-------------------------------------------------------------------------
Função: Encrustracao()
-------------------------------------------------------------------------
@short  : Função de teste dos modelos de encrustração do Thiago,
pode receber o modelo a partir da qual o processo de incrustração ocorre.
@author :	André Duarte Bueno baseado no código do Thiago
@return :	bool
// tese: dd=5um ed=25um fvd=0.8 fva=0.99 FIBaSo4=0.3
//Scale the 3D matrix reducing the porosity by percentPorosityScale.
//percentPorosityScale must be a value between 1 (0.01%) and 10000 (100%)
//5% = 500 10% =1000
// bool scalePercent(int percentPorosityScale);

//Scale the 3D matrix filling a number of voxels.
//normalScaleVoxels number of voxels which may be generated next to any non-porous voxel.
//agglomeratedScaleVoxels number of voxels which may be generated next to an already scaled voxel
// bool scaleAgglomerate(long long normalScaleVoxels, long long agglomerateScaleVoxels);
// bool scalePorousAgglomerate(long long normalScaleVoxels, long long agglomerateScaleVoxels, float microPorosity);
// bool scaleAgglomerate(long long normalScaleVoxels, long long agglomerateScaleVoxels);
// bool scaleAgglomeratePercentage(int percentPorosityScale, int agglomerateShare, int microPorosity);
// bool scaleDentriticPercentage(int percentPorosityScale, int dentriticShare);
// bool scaleDentritic(long long normalScaleVoxels, long long dentriticScaleVoxels);
**/
bool CGrafoTest::Encrustracao(int modelo) {
  // Solicita o nome do arquivo com a imagem
  string nomeArquivoImagem = SolicitarNomeImagem ();
  // Cria a imagem 3D.
  // Os dados de FA-fator amplicacao DP=dimensao pixel estão no início arquivo dentro #
  TCImagem3D<int>* pimagem3D = new TCImagem3D<int>(nomeArquivoImagem);

  modelo = SolicitarModeloEncrustracao ();
  CEncrustracao encrustracao ( pimagem3D );
  int percentual = 500; //=5%
  cout << "\nQual o percentual de incrustração:";
  cin >> percentual; cin.get();
  percentual = percentual * 100;
  // modelo 2
  int agglomerateShare = 5; //?agglomerateShare must be a value between 1 and 10000 (100%)
  int microPorosity = 5; //?
  // modelo 3
  int dentriticShare=5;//?
  string smodelo = "_encrustrada_M" + to_string(modelo);
  switch(modelo) {
  case 1:
    encrustracao.scalePercent(percentual); break;
  case 2:
    cout << "\nQual o valor de agglomerateShare( " << agglomerateShare << " ):";
    cin >> agglomerateShare; cin.get();
    agglomerateShare *= 100;
    cout << "\nQual o valor de microPorosity( " << microPorosity << " ):";
    cin >> microPorosity; cin.get();
    microPorosity *= 100;
    encrustracao.scaleAgglomeratePercentage(percentual, agglomerateShare, microPorosity); break;
  case 3:
    cout << "\nQual o valor de dentriticShare( " << dentriticShare << " ):";
    cin >> dentriticShare; cin.get();
    dentriticShare *= 100;
    encrustracao.scaleDentriticPercentage(percentual, dentriticShare*100); break;
  }
  string nomeArquivoImagemEncrustrada = nomeArquivoImagem;
  size_t pos = nomeArquivoImagemEncrustrada.find(".pm3");
  if (pos != std::string::npos) {         // Substituindo a palavra
    nomeArquivoImagemEncrustrada  = nomeArquivoImagemEncrustrada.substr(0,pos);
    nomeArquivoImagemEncrustrada += smodelo+".pm3";
  } else {
    nomeArquivoImagemEncrustrada = nomeArquivoImagem + smodelo + ".pm3";
    std::cout << "Extensão .pm3 não encontrada." << std::endl;
  }
  std::cout << "\nVai salvar no disco a imagem após encrustracao: " << nomeArquivoImagemEncrustrada << std::endl;
  pimagem3D->Write(nomeArquivoImagemEncrustrada);
  delete pimagem3D;			// deleta objeto
  return 1;
}

bool CGrafoTest::EncrustracaoPermeabilidade (int modelo) {
  // Entrada dados
  string nomeArquivoImagem = SolicitarNomeImagem ();
  TCImagem3D<int>* pimagem3D = new TCImagem3D<int>(nomeArquivoImagem);

  cout << "\n===> Dados modelo encrustração: ";
  modelo = SolicitarModeloEncrustracao ();
  CEncrustracao encrustracao ( pimagem3D );
  int percentual = 500; //=5%
  cout << "\nQual o percentual inicial de incrustração (ex: para 5% digite 5):";
  cin >> percentual; cin.get();
  percentual = percentual * 100;
  int percuntualAcrescimo = 500;
  cout << "\nQual o percuntualAcrescimo de incrustração (ex: inicial 5 e acrescimo 5 =  5%->10%->15%):";
  cin >> percuntualAcrescimo; cin.get();
  percuntualAcrescimo = percuntualAcrescimo * 100;
  // modelo 2
  int agglomerateShare = 5; //?agglomerateShare must be a value between 1 and 10000 (100%)
  int microPorosity = 5; //?
  // modelo 3
  int dentriticShare=5;//?
  double permeabilidade =0.0;
  string sModeloEncrustracao = "_Encrustrada_M";
  switch(modelo) {
  case 1:
    sModeloEncrustracao += "Scale_";
    break;
  case 2:
    sModeloEncrustracao += "AgglomerateShare_";
    cout << "\nQual o valor de agglomerateShare( " << agglomerateShare << " ):";
    cin >> agglomerateShare; cin.get();
    agglomerateShare *= 100;
    cout << "\nQual o valor de microPorosity( " << microPorosity << " ):";
    cin >> microPorosity; cin.get();
    microPorosity *= 100;
    break;
  case 3:
    sModeloEncrustracao += "DentriticShare_";
    cout << "\nQual o valor de dentriticShare( " << dentriticShare << " ):";
    cin >> dentriticShare; cin.get();
    dentriticShare *= 100;
    break;
  }
  // Dados do modelo calculo permeabilidade
  cout << "\n===> Dados modelo grafo usado no calculo da permeabilidade: ";
  bool salvarGrafo = false;
  cout << "\nVai gerar grafos de conexao serial, deseja salvar grafos parciais( " << salvarGrafo << " ) bool -> 0 ou 1 :";
  cin >> salvarGrafo; cin.get();

  int modeloGrafo = SolicitarModeloGrafo ();
  int tipoSolver = 2; //1 ou 2 ou 3 com thread??
  string nomeArquivoImagemEncrustrada = nomeArquivoImagem + "_Encrustrada_";

  size_t pos = nomeArquivoImagem.find(".pm3");
  string nomeArquivoResultadosPermeabilidade = nomeArquivoImagem.substr(0,pos) + sModeloEncrustracao + "Permeabilidades.dat";
  ofstream valores_permeabilidade (nomeArquivoResultadosPermeabilidade);
  valores_permeabilidade << "\nNomeImagem: "            << nomeArquivoImagem
                         << "\nModelo encrustração: "   << modelo
                         << "\nModelo grafo: "          << modeloGrafo
                         << "\nTipo solver: "           << tipoSolver
                         << "\nPorosidadeEncrustrada\tPermeabilidade" << endl;
  // Looping de cálculo
  do {
    // Encrustracao
    switch(modelo) {
    case 1:
      encrustracao.scalePercent(percentual); break;
    case 2:
      encrustracao.scaleAgglomeratePercentage(percentual, agglomerateShare, microPorosity); break;
    case 3:
      encrustracao.scaleDentriticPercentage(percentual, dentriticShare*100); break;
    }
    // Nome arquivo
    if (pos != std::string::npos) {         // Substituindo a palavra
      nomeArquivoImagemEncrustrada  = nomeArquivoImagem.substr(0,pos);
      nomeArquivoImagemEncrustrada += sModeloEncrustracao + "_porosidade_" + to_string(percentual/100) + ".pm3";
    } else {
      nomeArquivoImagemEncrustrada = nomeArquivoImagem + "_encrustrada_M" + "_porosidade_" + to_string(percentual/100) + ".pm3";
      std::cout << "Extensão .pm3 não encontrada." << std::endl;
    }
    std::cout << "\nVai salvar no disco a imagem após encrustracao: " << nomeArquivoImagemEncrustrada << std::endl;
    pimagem3D->Write(nomeArquivoImagemEncrustrada);
    // Calculo da permeabilidade
    string nomeArquivoGrafo = nomeArquivoImagemEncrustrada; // vai adicionar _permeabilidade e salvar
    double permeabilidade = PermeabilidadeImagem ( pimagem3D, modeloGrafo, nomeArquivoGrafo, salvarGrafo, tipoSolver );
    valores_permeabilidade << percentual/100 << '\t' << permeabilidade << endl;
    if(permeabilidade < 0.00001)
      break; // evita cálculo de permeabilidade de imagens que já estão com k~0
    // Avanço do percentual de incrustração
    percentual = percentual + percuntualAcrescimo;
  } while(percentual/100 <= 100 );
  valores_permeabilidade.close();
  delete pimagem3D;			// deleta objeto
  string comando = "gnuplot << \"plot \"" << nomeArquivoResultadosPermeabilidade << "\"" << " using 1:2 with lp " << endl;
  cout << comando << endl;
  system(comando.c_str());
  return 1;
}

int CGrafoTest::SolicitarModeloEncrustracao() {
  cout << "\nQual o modelo de incrustração:"
       << "\n1....scalePercent(percentual)"
       << "\n2....scaleAgglomeratePercentage(percentual, agglomerateShare, microPorosity)"
       << "\n3....scaleDentriticPercentage(percentual, dentriticShare): ";
  int modelo = 1;
  cin >> modelo; cin.get();
  return modelo;
}

//Versao abaixo apagar: funciona mas foi a primeira tentativa, muito poluida
// bool CGrafoTest::Encrustracao(int modelo) {
//   // Solicita o nome do arquivo com a imagem
//   string nomeArquivo = SolicitarNomeImagem ();
//   // Cria matriz3D e lê imagem do disco
//   TCMatriz3D<int>* pm3D { nullptr };
//   pm3D = LerImagemDisco ( nomeArquivo );
//   // Cria a imagem 3D como sendo cópia da matriz3D
//   TCImagem3D<int>* pimagem3D = new TCImagem3D<int>(*pm3D);
//   if(pm3D != nullptr)  delete pm3D;
//   // Seta as propriedades
//   // unsigned   int fatorAmplificacao = 2;
//   int fatorAmplificacao = 2;
//   double dimensaoPixel = 3.125e-06;//5.0e-6;
//   LerPropriedadesImagemDoDisco ( fatorAmplificacao, dimensaoPixel, nomeArquivo );
//   pimagem3D->FatorAmplificacao(fatorAmplificacao);
//   pimagem3D->DimensaoPixel(dimensaoPixel);
//   modelo = SolicitarModeloEncrustracao ();
//   //Construtor: CEncrustracao(TCImagem3D<int>* _pmatrix);
//   CEncrustracao encrustracao ( pimagem3D );
//   int percentual = 500; //=5%
//   cout << "\nQual o percentual de incrustração:";
//   cin >> percentual; cin.get();
//   percentual = percentual * 100;
//   // modelo 2
//   int agglomerateShare = 500; //?agglomerateShare must be a value between 1 and 10000 (100%)
//   int microPorosity = 5; //?
//   // modelo 3
//   int dentriticShare=500;//?
//   string smodelo = "_encrustrada_M" + to_string(modelo);
//   cerr << "\nsmodelo="<<smodelo<< endl;
//   switch(modelo) {
//   case 1:     encrustracao.scalePercent(percentual); break;
//   case 2:     encrustracao.scaleAgglomeratePercentage(percentual, agglomerateShare, microPorosity); break;
//   case 3:     encrustracao.scaleDentriticPercentage(percentual, dentriticShare); break;
//   }
//   //RealizarEncrustracao ( encrustacao, pimagem3D, nomeArquivo );
//   //SalvarImagem ( pimagem3D );
//   //TCMatriz3D template< typename T >void TCMatriz3D<T>::SalvaDados (ofstream & fout)
//   string nomeArquivoImagemEncrustrada = nomeArquivo;
//   size_t pos = nomeArquivoImagemEncrustrada.find(".pm3");
//   if (pos != std::string::npos) {         // Substituindo a palavra
//     nomeArquivoImagemEncrustrada  = nomeArquivoImagemEncrustrada.substr(0,pos);
//     nomeArquivoImagemEncrustrada += smodelo+".pm3";
//   } else {
//     nomeArquivoImagemEncrustrada = nomeArquivo + smodelo;
//     std::cout << "Extensão .pm3 não encontrada." << std::endl;
//   }
//   std::cout << "\nVai salvar no disco a imagem após encrustracao: " << nomeArquivoImagemEncrustrada << std::endl;
//   //ofstream fout(nomeArquivoImagemEncrustrada);
//   //pimagem3D->SalvaDados(fout); // não salva o cabeçalho; Leandro adicionou dados de reconstrução...[retirar]
//   //bool CBaseMatriz::Write(string nomeArquivo, int separado)
//   pimagem3D->Write(nomeArquivoImagemEncrustrada);
//   //delete encrustacao;			// deleta objeto
//   delete pimagem3D;			// deleta objeto
//   return 1;
// }


// string nomeArquivo = SolicitarNomeImagem ();
// TCImagem3D<int>* pimagem3D = new TCImagem3D<int>(nomeArquivo);
