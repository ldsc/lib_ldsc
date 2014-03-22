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
#include <cstdlib>
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
#include <AnaliseImagem/Matriz/CVetor.h>
#include <AnaliseImagem/Matriz/TCMatriz2D.h>
#include <AnaliseImagem/Matriz/TCMatriz3D.h>
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
#include <MetNum/Solver/SistemaEquacoes//SolverMatrizDiagonal/CSolverMatrizDiagonal_SOR.h>
#include <AnaliseImagem/Simulacao/Permeabilidade/GrafoConexaoSerial/CSimPermeabilidadeGrafo.h>
#include <AnaliseImagem/Simulacao/Permeabilidade/CSimPermeabilidade.h>
//  novo
//#include <SMatriz/SMDiagonal/CSolverMatrizDiagonalDominante_Threads.h>
using namespace std;

/*
-------------------------------------------------------------------------
Função:   Run()
-------------------------------------------------------------------------
@short  :	Função de teste das funções da classe CGrafo
@author :	André Duarte Bueno
@see    :
@param  :	void
@return :	void
*/
void
CGrafoTest::Run ()
{
   int selecao {0};

   do {
         cout << "\nObjeto CGrafoTest"
              << "\nTeste das funções das classes CGrafoConexaoSerial e CSimPermeabilidade"
              << "\nSelecione a opção que deseja testar"
              << "\nGrafo.........................1"
              << "\nTortuosidade..................2"
              << "\nPermeabilidade................3:"
              << "\nQuit..........................9:";
         cin >> selecao;
         cin.get ();		// pega retorno de carro

         switch ( selecao ) {
            case 1:
               cout << "\n Chamando função Grafo()" << endl;
               Grafo ();
               break;

            case 2:
               cout << "\n Chamando função Tortuosidade()" << endl;
               Tortuosidade ();
               break;
			   
            case 3:
               cout << "\n Chamando função Permeabilidade()" << endl;
               Permeabilidade ();
               break;

            default:
               cout << "\n Saindo da função TCGrafo->Run" << endl;
               break;
            }
      }
   while ( selecao != 9 );

   cin.clear ();
}

/*
-------------------------------------------------------------------------
Função: Grafo
-------------------------------------------------------------------------
@short  : Função de teste do Grafo, recebe a imagem a partir da qual o
	grafo vai ser gerado, e um inteiro que identifica o método grafo
	a ser utilizado.
@author :	André Duarte Bueno
@see    :
@param  :	unsigned long int escolha
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

/*
-------------------------------------------------------------------------
Função: Permeabilidade
-------------------------------------------------------------------------
@short  : Calcula a permeabilidade a partir do grafo
	  Solicita o nome do arquivo de disco com a imagem e o modelo a ser utilizado.
	  Le do disco dados da imagem, e do solver.
@author : André Duarte Bueno
@see    : Permeabilidade
@param  :
@return : double (a permeabilidade)
*/
double CGrafoTest::Permeabilidade ( int tipoSolver )
{
// cerr << "\n=============================================================="; //aqui aqui
// cerr << "\nPassei por: CGrafoTest::Permeabilidade().";
// cerr << "\n==============================================================";

   // Determinação do grafo
   MostrarInstrucoesArquivosExternos ();

   string nomeArquivo = SolicitarNomeImagem ();
   int fatorAmplificacao = 2;
   double dimensaoPixel = 5.0e-6;
   SolicitarPropriedadesImagem ( fatorAmplificacao, dimensaoPixel, nomeArquivo );
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

   delete pm3D;
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
    cout << "\nEntre com o nome da imagem"
         << "\nExemplo:\n";
		int r = system ("dir *.txt *.pm3");
		r = r; //evita warming
   string nomeArquivo;
   getline ( cin, nomeArquivo );
   return nomeArquivo;
}

/*
-------------------------------------------------------------------------
Função: SolicitarPropriedadesImagem
-------------------------------------------------------------------------
*/
void
CGrafoTest::SolicitarPropriedadesImagem ( int& fatorAmplificacao,
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
      cout << "Lendo imagem (" << nomeArquivo << ") do disco..." << endl;
      pm3D = new TCMatriz3D<int> ( nomeArquivo );
ofstream debug("imagem_lida.pm3");
debug << pm3D;
      assert ( pm3D );
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
//             nomeArquivoExt += ".mod1";
            grafo = new CGrafoConexaoSerial_M1 ( nomeArquivoExt );
            break;

         case 2:
            cout << "...Criando objeto CGrafoConexaoSerial_M2...";
//             nomeArquivoExt += ".mod2";
            grafo = new CGrafoConexaoSerial_M2 ( nomeArquivoExt );
            break;

         case 3:
            cout << "...Criando objeto CGrafoConexaoSerial_M1...";
//             nomeArquivoExt += ".mod3";
            grafo = new CGrafoConexaoSerial_M3 ( nomeArquivoExt );
            break;

         case 4:
            cout << "...Criando objeto CGrafo_3Dby2_M4...";
//             nomeArquivoExt += ".mod4";
            grafo = new CGrafoConexaoSerial_M4 ( nomeArquivoExt );
            break;

         case 5:
            cout << "...Criando objeto CGrafoConexaoSerial_M5...";
//             nomeArquivoExt +=  ".mod5";
            grafo   = new CGrafoConexaoSerial_M5 ( nomeArquivoExt );
            break;

         case 6:
            cout << "...Criando objeto CGrafoConexaoSerial_M6...";
//             nomeArquivoExt += ".mod6";
            grafo = new CGrafoConexaoSerial_M6_Tortuosidade ( nomeArquivoExt );
            break;

         default:
            cout << "...Criando objeto CGrafoConexaoSerial_M3...";
//             nomeArquivoExt += ".mod3";
            grafo = new CGrafoConexaoSerial_M3 ( nomeArquivoExt );
            break;
         };

      assert ( grafo );

      cout << " ...done" << endl;
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
   cout << "Determinando o grafo da imagem(" << nomeArquivo << ")..." << "\nChamando o grafo->Go()" << endl;
   grafo->Go ( pm3D );
   cout << "\nDimensão do grafo = " << grafo->objeto.size () << endl << " ...done" << endl;
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
   cout << "Determinando o grafo da imagem(" << nomeArquivo << ")..." << "\nChamando o grafo->Go()" << endl;
   grafo->Go ( nomeArquivo );
   delete t;				// deleta objeto
   cout << "\nDimensão do grafo = " << grafo->objeto.size () << endl  << " ...done" << endl;
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

// for (int i=0; i< grafo->objeto.size(); i++) // aqui aqui
//  cerr << "\ngrafo->objeto[" << i << "]->Go() = " << grafo->objeto[i]->Go() << endl;
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
   cout << " ...done" << endl;
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
      cout << " ...done" << endl;
   }
   
// for (int i=0; i< permeabilidade_grafo->Grafo()->objeto.size(); i++) // aqui aqui
//  cerr 	<< "\nCGrafoTest::CriarPermeabilidade  -> permeabilidade_grafo->Grafo()->objeto[" << i << "]->Go() = " 
// 		<< permeabilidade_grafo->Grafo()->objeto[i]->Go() << endl;
   
   return permeabilidade_grafo;
}

/*
-------------------------------------------------------------------------
Função: DeterminarPermeabilidade
-------------------------------------------------------------------------
*/
double CGrafoTest::DeterminarPermeabilidade ( CSimPermeabilidadeGrafo* permeabilidade )
{
// for (int i=0; i< permeabilidade->Grafo()->objeto.size(); i++) // aqui aqui
//  cerr 	<< "\nCGrafoTest::DeterminarPermeabilidade ANTES -> permeabilidade->Grafo()->objeto[" << i << "]->Go() = " 
// 		<< permeabilidade->Grafo()->objeto[i]->Go() << endl;

{
      CTime ( "Tempo permeabilidade->SolucaoSistema(); = ", &cout );
      cout << "Chamando permeabilidade->SolucaoSistema() "
           << "que realiza pré-processamento e primeira iteração do Solver..."
           << endl;
      permeabilidade->SolucaoSistema ();
      cout << " permeabilidade->SolucaoSistema()...done" << endl;
   }
   // Determinação da permeabilidade---------------------------------------
   double permeabilidade_calculada;
   {
      CTime ( "Tempo permeabilidade->Go() = ", &cout );
      cout << "Chamando permeabilidade->Go()..." << endl;
      permeabilidade_calculada = permeabilidade->Go ();
      cout << " permeabilidade->Go()... ...done" << endl;
      cout << "\a\a\nPermeabilidade (mD)= " << permeabilidade_calculada << endl;
   }
// for (int i=0; i< permeabilidade->Grafo()->objeto.size(); i++) // aqui aqui
//  cerr 	<< "\nCGrafoTest::DeterminarPermeabilidade DEPOIS -> permeabilidade->Grafo()->objeto[" << i << "]->Go() = " 
// 		<< permeabilidade->Grafo()->objeto[i]->Go() << endl;

   // Resolvido o sistema salva em disco o grafo com as pressões finais
   // SalvarGrafo(grafo);
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

/*
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
   string arquivoSaida ( "ResultadosPermeabilidade.txt" );
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
                    << "Lendo Imagem (" << nomeArquivo << ") do disco" << endl;
               fout << "=============================================================\n"
                    << "Lendo Imagem (" << nomeArquivo << ") do disco" << endl;
               {
                  ifstream teste ( nomeArquivo.c_str () );

                  if ( teste.fail () ) {
                        cout << "\n A Imagem " << nomeArquivo << " é inválida (Nome errado?).\a" << endl;
                        fout << "\n A Imagem " << nomeArquivo << " é inválida (Nome errado?)." << endl;
                        teste.close ();	// ?
                        continue;	// Passa para proximo passo do laço while ? confirmar->ok
                     }
                  else {
                        cout << "\n A Imagem " << nomeArquivo << " é válida.\a" << endl;
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
               Permeabilidade_By_ModelX_Decisao ( nomeArquivo, 3, pm3D, fout );	// modelo 3 //AQUI AQUI
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

   /*	string nomeComModelo(nomeArquivo);
   	if 	( modelo == 1 )			nomeComModelo +=  ".mod1";
   	else if	( modelo == 2 )			nomeComModelo +=  ".mod2";
   	else if	( modelo == 3 )			nomeComModelo +=  ".mod3";
   	else if	( modelo == 4 )			nomeComModelo +=  ".mod4";
   	else if	( modelo == 5 )			nomeComModelo +=  ".mod5";
   	else if	( modelo == 6 )			nomeComModelo +=  ".mod6";
   */
   // Simulação para a imagem já foi ENCERRADA, existe o arquivo NomeImagem.modi.fim
   // Pula para proxima imagem
   string nomeTeste;
   nomeTeste = nomeComModelo + ".fim";
   in.open ( nomeTeste.c_str () );

   if ( in ) {
         cout << "\n--->Imagem ("
              << nomeComModelo
              << ") já foi simulada, pulando para práxima" << endl;
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
Recebe o nome da imagem a ser processada, o modelo
a imagem , um fout, e um flag de reinicialização.
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
   SolicitarPropriedadesImagem ( fatorAmplificacao, dimensaoPixel, nomeArquivo );
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
         cout << "(reiniciando simulação anterior)" << endl;
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
      cout << " permeabilidade->SolucaoSistema()...done" << endl;
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
      // Definição do nome da imagem, considerando o modelo
//       ostringstream os;
//       os << nomeArquivo << ".mod" << modelo << ".grafo.Pressoes";
//       string nomeArquivoGrafoFinal ( os.str () );

      /*
      	string nomeArquivoGrafoFinal(nomeArquivo );
      	switch(modelo)
      		{
      		case 1:               nomeArquivoGrafoFinal += ".mod1"; break;
      		case 2:               nomeArquivoGrafoFinal += ".mod2"; break;
      		case 3:               nomeArquivoGrafoFinal += ".mod3"; break;
      		case 4:               nomeArquivoGrafoFinal += ".mod4"; break;
      		case 5:               nomeArquivoGrafoFinal += ".mod5"; break;
      		case 6:               nomeArquivoGrafoFinal += ".mod6"; break;
      		}
      		nomeArquivoGrafoFinal += ".grafo.Pressoes";
      */
//       cout << "Grafo com condutâncias e pressões finais  salvo em : " << nomeArquivoGrafoFinal << endl;
// 	  cout << "informação acima incorreta, corrigir arquivo " << __FILE __ << " linha " << __LINE__ << endl;
//         grafo->Write (nomeArquivoGrafoFinal);
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
   	// cout << "\nVai executar a função 	grafo->SetarMatrizAVetorB(A,B);" << endl ;
   	// cin.get();
   	grafo->SetarMatrizAVetorB(A,B);
   	// cout << "\nExecutou a função 	grafo->SetarMatrizAVetorB(A,B);" << endl ;
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

// NOVO NOVO NOVO
   /*ofstream s( (nomeArquivo + "perm.novo.txt").c_str() );
   if(s)
   	{
   	s<<(*permeabilidade);
   	s.close();
   	}*/
// FIM NOVO
   delete permeabilidade;	// Destróe objeto permeabilidade (que destróe o grafo)
   return 1;
}
