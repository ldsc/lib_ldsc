// -----------------------------------------------------------------------------
// Biblioteca de C++.
// -----------------------------------------------------------------------------
#include <iostream>
#include <cstdlib>
#include  <cstring>	// novo strcpy
using namespace std;

// -----------------------------------------------------------------------------
// Biblioteca do LDSC (Lib_LDSC)
// -----------------------------------------------------------------------------
#include "Reconstrucao/CReconstrucaoZhirong.h"
#include "Reconstrucao/CReconstrucaoBueno.h" // novo

/**
	  * Função para teste do calculo da permeabilidade.
	  * @param argc
	  * @param argv
	  * @return
 */
int main( int argc, char **argv )
{

    cout  << "\n================================================================================"
          << "\n====================== Programa de teste da LIB_LDSC ==========================="
          << "\n================================================================================" << endl;

    cout  << "\n================================================================================"
          << "\nUSO:"
          << "\n./reconstrucao3d"
          << "\n================================================================================" << endl;

    string  nomeArquivoCz = "320220.cor";
    int  fatorAmplificacaoNumeroPontos=3;
    int  numeroPontosCz=10;
    int  NX=20;
    int  NY=20;
    int  NZ=20;
    int gt = 1; // novo

    // Obtem valores a partir da linha de comando
    if (argc > 1) nomeArquivoCz = argv[1];
    if (argc > 2) fatorAmplificacaoNumeroPontos = atoi (argv[2]);
    if (argc > 3) numeroPontosCz = atoi (argv[3]);
    if (argc > 4) NX = NY = NZ = atoi (argv[4]);
    if (argc > 5) NY = NZ = atoi (argv[5]);
    if (argc > 6) NZ = atoi (argv[6]);
    if (argc > 7) gt = atoi (argv[7]);

    // Se não entrou com valores na linha de comando solicita entrada de dados
    if (argc == 1)
    {
        bool primeiraPassagem = true;
        char resp, newline;
        string entreCom="\n\tEntre com ";
        string linha="\n__________________________________________________________________";
        do
        {
            cout<< linha
                << "\n  ***  Programa de Reconstrucao Tridimensional (LDSC)  ***"
                << linha
                << "\n\t\t\tEntrada de dados:"
                << linha;
            if (   primeiraPassagem != true )
            {
                cout<< entreCom<< "o nome do arquivo ["<< nomeArquivoCz<< "]:";
                getline(cin, nomeArquivoCz);

                cout<< entreCom<< "o fator de reducao ["<< fatorAmplificacaoNumeroPontos<< "]:";
                cin>> fatorAmplificacaoNumeroPontos;

                cout<< entreCom<< "o numeroPontos a serem lidos no arquivo de correlacao["
                    << numeroPontosCz<< "]:";
                cin>> numeroPontosCz;

                cout<< entreCom<< "a dimensao x da matriz NX ["<< NX<< "]:";
                cin>> NX;

                cout<< entreCom<< "a dimensao y da matriz NY ["<< NY<< "]:";
                cin>> NY;

                cout<< entreCom<< "a dimensao z da matriz NZ ["<< NZ<< "]:";
                cin>> NZ;

                cout<< entreCom<< "tipo de gaussiana gt1=1, gt2=2 ["<< gt<< "]:";
                cin>> gt;

                cin.get();// Pega o ultimo return
                cout<< "\n\n\n"<< linha;
            }
            primeiraPassagem = false;
            cout<< "\nDados fornecidos:"
                << linha<< linha
                << "\n\tNome do arquivo: ["<< nomeArquivoCz<< "]"
                << "\n\tFatorAmplificacao: ["<< fatorAmplificacaoNumeroPontos<< "]"
                << "\n\tNumeroPontosCorrelacaoCz: ["<< numeroPontosCz<< "]"
                << "\n\tDimensao x da matriz NX: ["<< NX<< "]"
                << "\n\tDimensao y da matriz NY: ["<< NY<< "]"
                << "\n\tDimensao z da matriz NZ: ["<< NZ<< "]"
                << "\n\tTipo de gaussiana (gt1 ou gt2) ["<< gt<< "]"
                << linha
                << "\n\tValores corretos, iniciar reconstrução...........(r)"
                << "\n\tValores incorretos, modificar....................(m)"
                << "\n\tAbandonar programa...............................(a)"
                << linha;
            cin.get(resp);			// resp=getche();
            cin.get(newline);		// pega retorno de carro.

            if (resp=='a'||resp=='A')
                exit(1);
        }
        while (resp=='m'||resp=='M');
    }
    cout<< "\nFim da funcao de entrada de dados"
        << "\nVai criar objeto de reconstrucao...";
    if (gt==1)
    {
        // cria objeto de reconstrucao
        CReconstrucaoZhirong reconstrucao(nomeArquivoCz, fatorAmplificacaoNumeroPontos, numeroPontosCz, NX,NY,NZ);
        cout<< "...pronto."
            << "\nVai executar Go...";
        reconstrucao.Go();					// executa a reconstrucao
        cout<< "...pronto."
            << "\nCalculando o covariograma...";
        reconstrucao.Covariograma();				// calcula e salva o covariograma,
    }
    else //  gt = 2 ou maior
    {
        // cria objeto de reconstrucao
        CReconstrucaoBueno reconstrucao(nomeArquivoCz, fatorAmplificacaoNumeroPontos, numeroPontosCz, NX,NY,NZ,gt);
        cout<< "...pronto."
            << "\nVai executar Go...";
        reconstrucao.Go();					// executa a reconstrucao
        cout<< "...pronto."
            << "\nCalculando o covariograma...";
        reconstrucao.Covariograma();				// calcula e salva o covariograma,
    }
    cout<< "...pronto."
        << "\nReconstrução Tridimensional encerrada."
        << "\nFoi criado o arquivo  log_reconstrucao.txt com as informações da reconstrução "
        << "\nPressione qualquer tecla para encerrar";
    return 0;		// encerra o programa
}


