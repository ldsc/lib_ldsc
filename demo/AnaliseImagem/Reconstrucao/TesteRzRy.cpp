
// -----------------------------------------------------------------------------
// Includes:
// ---------------------------------------------------------------------------
#include<cmath>
#include<fstream>
#include<iostream>
#include<iomanip>
#include<cstdio>
#include<cstdlib>
#include<string>	// novo strcpy
#include<cassert>

using namespace std;

// ------------------------------------------------------------------------------
// Includes do LDSC
// ------------------------------------------------------------------------------
#ifndef _ProgramaSemDLL
#define CFuncaoDLL
#endif

// Objeto CFuncaoNaoLinearRzRy
#include    "Funcao/CFNaoLinearRzRy.h"

// dll estatistica
// #ifndef _ProgramaSemDLL
// #define TEstatisticaDLL
// #endif
//#include    "Estatistica/CEstatistica.h"
#include    "Estatistica/CENormal.h"

// classe base
#include    "TesteRzRy.h"

// ------------------------------------------------------------------------------
// Construtor
// ------------------------------------------------------------------------------
// Construtor, entrada de dados da linha de comando ou manual,
// leitura do arquivo de disco, alocacao de matrizes,ponteiros arquivos disco
TesteRzRy::TesteRzRy()
{
    unsigned int resp;
    do
    {
        std::cout << "\nEntre com a porosidade minima (0.0 a 1.0, default 0): ";
        std::cin  >>  porosidadeMinima;
        std::cin.get();
        std::cout << "\nEntre com a porosidade maxima (0.0 a 1.0 default 0.5): ";
        std::cin  >>  porosidadeMaxima;
        std::cin.get();
        std::cout << "\nEntre com o incremento da porosidade  (default 0.05): ";
        std::cin  >>  incrementoPorosidade;
        std::cin.get();

        /*	std::cout << "\nPS: o intervalo do comprimento de correlacao e usado para definir o numero de pontos:";
          std::cout << "\nEntre com o comprimento de correlacao minimo : ";
          std::cin  >>  compCorMin;	std::cin.get();
          std::cout << "\nEntre com o comprimento de correlacao máximo: ";
          std::cin  >>  compCorMax;	std::cin.get();
          std::cout << "\nEntre com o incremento da correlação  (default 30): ";
          std::cin  >>  incrementoCorrelacao;	std::cin.get();
        */
        std::cout << "\nEntre com o número de pontos da curva de autocorrelação Cz (default=4000): ";
        std::cin  >>  numeroPontosCz;
        std::cin.get();
        // numeroPontosCz = 150;
        // numeroPontosCz = static_cast<unsigned int>( 1+ (compCorMax - compCorMin) / incrementoCorrelacao );

        std::cout << "\nUsar gt1 (1) ou gt2(2)  (default 1): ";
        std::cin  >>  gt;
        std::cin.get();
        if (gt > 2) gt = 1;


        std::cout <<  "Numero de pontos da curva = " <<  numeroPontosCz
                  <<  "\nIntervalo da Porosidade: de ("  << porosidadeMinima
                  <<  " a "  <<  porosidadeMaxima << " )" << endl;
        // std::cout << "Intervalo do comprimento de correlação: de ("  << compCorMin << " a "  <<  compCorMax << " )" << endl;
        std::cout << "\nSe os dados estiverem corretos, tecle 1 e enter :";
        std::cin >> resp;
        std::cin.get();
    } while ( resp != 1 );

    // Aloca vetor Cz
    Cz = new double [ numeroPontosCz  ];
    assert(Cz);

    // Aloca o vetor de Ry
    Ry = new double [ numeroPontosCz  ];
    assert(Ry);
}

// ----------------------------------------
// Destrutor
// ----------------------------------------
TesteRzRy::~TesteRzRy()
{
    delete [] Cz;
    delete [] Ry;
}

// ----------------------------------------
// Calculo_Cz
// ----------------------------------------
// calcula os valores de Cz usando função exponencial entre o deslocamento u,
//  e o comprimento de correlação lambda. (ou alcance de correlação)
void TesteRzRy::Calculo_Cz()
{
    // Porosidade
    // função de cálculo de Cz normalizado em função de:
    // u=deslocamento
    // lambda = comprimentoCorrelacao em píxeis
    // 	std::cout << "\n aqui2000
    for ( int u = 0 ; u <  numeroPontosCz; u++ )
        Cz[ u ] = exp ( - u /(float) comprimentoCorrelacao);
}

// ----------------------------------------
// cálculo de Ry
// ----------------------------------------
void TesteRzRy::Calculo_Ry()
{
    // Cria função não linear Ry Rz
    // 35 é o número de pontos da serie
    int numpoints = 35;
    CFNaoLinearRzRy* funcaoNaoLinearRzRy = new CFNaoLinearRzRy ( yc , porosidade , numpoints , gt );

    // gt é novo
    if ( funcaoNaoLinearRzRy == NULL )
        exit(1);

    // Abre arquivo de disco para armazenar resultados

    // static_cast<int>( ceil(porosidade*100)   )
    // int por = porosidade*100;
    // std::cout << " " <<  por  << endl;
    sprintf(nomeArquivoRy,
            // "porosidade_%d_compCor_%d_gt%d.txt",
            "porosidade_%2.0f_gt%i.txt",
            // static_cast<int>(porosidade*100),
            porosidade*100,
            // comprimentoCorrelacao,
            gt);// novo

    ofstream foutDisco ( nomeArquivoRy ) ;
    if (foutDisco == NULL)
    {
        std::cout << "\nNao abriu arquivo disco";
        exit(1);
    }
    foutDisco.precision(5);
    foutDisco  <<  "Arquivo;" << nomeArquivoRy << endl;
    foutDisco  <<  "Porosidade;" << porosidade <<  ";" << endl;
    foutDisco  <<  "Numero de pontos da curva = " <<  numeroPontosCz << endl;

    //  << "Comprimento Correlacao;" << comprimentoCorrelacao << endl;
    foutDisco  <<  "deslocamento;   Rz;   Ry;" << endl;

    // Calcula Ry para diferentes valores de Rz
    for ( int i = 0 ; i < numeroPontosCz ; i++ )
    {
        // Calculo de Ry
        Ry[i] = funcaoNaoLinearRzRy->Go( Cz[i] );

        // Saída em disco
        // foutDisco  <<  setw(4)  <<  i  <<  " "  <<  setw(10)  <<  Cz[i]  <<  "  "  <<  setw(10)  <<  Ry[i ] <<  endl;
        foutDisco  <<  setw(4)  <<  i  <<  ";"  <<  setw(10)  <<  Cz[i]  <<  ";"  <<  setw(10)  <<  Ry[i ] <<  ";" <<  endl;
    }
    // elimina objeto criado
    delete funcaoNaoLinearRzRy;

    // fecha  arquivo disco
    foutDisco.close();
}

void TesteRzRy::SaidaDados()
{
}

// -----------------------------------------------------------------------------
// Funcao Go
// -----------------------------------------------------------------------------
int TesteRzRy::Go()
{
    std::cout << "\nCriando arquivo de disco log_testeRzRy.txt" << endl;
    ofstream fout("log_testeRzRy.txt");

    // Cria objeto CNormalque representa a curva normal
    fout << "\nCriando objeto CENormal para cálculo de yc ...";
    CENormal* normal = new CENormal();
    fout << " ...objeto CNormal criado.";
    normal->limiteErro=1e-10;

    // Definição da porosidade e do comprimento de correlação
    // porosidade = 						// 10,15,20,23,30
    // for(  int kp = (porosidadeMinima*100); kp<=(porosidadeMaxima*100);kp=kp+5 )
    for ( porosidade =  porosidadeMinima;
            porosidade <= porosidadeMaxima ;
            porosidade += incrementoPorosidade)
        // numeroPontosCz
    {
        // porosidade = (double) kp/100.0;			// 10/100=0.1
        std::cout << "porosidade = " << porosidade*100 << endl;
        fout << "porosidade = " << porosidade << endl;

        // Calculo de yc
        fout << "yc=normal->GoInv(" << porosidade << ")=";
        yc = normal->GoInv(porosidade);                // retorna yc da eq 3.15
        fout  <<  yc  << "...ok." << endl;

        // comprimentoCorrelacao =	// 30,60,90,120
        // for ( int kc = 30 ;  kc <= 120 ;  kc = kc + 30 )
        /*for ( comprimentoCorrelacao  = compCorMin ;
        comprimentoCorrelacao <=  compCorMax;
        comprimentoCorrelacao += incrementoCorrelacao )*/
        {
            // Calculo_Cz (usando função exponencial)
            // comprimentoCorrelacao = kc;
            // fout << "comprimentoCorrelacao = " << 		comprimentoCorrelacao ;

            fout << "\nCz..";
            std::cout << "\nCz..";
            Calculo_Cz(  );
            fout << "..ok";

            // Calculo de Ry a partir de Rz
            fout << "..Ry..";
            std::cout << "..Ry..";
            Calculo_Ry();
            fout << "..ok" << endl;
        } // fim for kc
    } // fim for kp

    delete normal;
    fout << "\nobjeto normal deletado.";
    fout.close();
    return 1;
}

