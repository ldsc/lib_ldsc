//-----------------------------------------------------------------------------
//Biblioteca de C++.
//-----------------------------------------------------------------------------
#include <iostream>
#include <cstdlib>
#include  <cstring>	//novo strcpy
using namespace std;

//-----------------------------------------------------------------------------
//Biblioteca do LMPT (Lib_LMPT)
//-----------------------------------------------------------------------------
//#define     _ProgramaSemDLL
//#include    "TReconstrucaoZhirong.h"
#include "Reconstrucao/CReconstrucaoZhirong.h"
#include "Reconstrucao/CReconstrucaoBueno.h" //novo
//#include    "TesteRzRy.h"

//-----------------------------------------------------------------------------
//Funcao main
//-----------------------------------------------------------------------------
int main( int argc, char **argv )
{
    char nomeArquivoCz[254];		//atributos a serem passados para o construtor de TReconstrucaoZhirong
    strcpy(nomeArquivoCz,"320220.cor");
    int  fatorAmplificacaoNumeroPontos=3;
    int  numeroPontosCz=10;
    int  NX=20;
    int  NY=20;
    int  NZ=20;
    int gt = 1; //novo
    //Obtem valores a partir da linha de comando
    if (argc > 1) strcpy(nomeArquivoCz,argv[1]);
    //if (argc > 2) sementeGeradorAleatorio = atoi (argv[2]);
    if (argc > 2) fatorAmplificacaoNumeroPontos = atoi (argv[2]);
    if (argc > 3) numeroPontosCz = atoi (argv[3]);
    if (argc > 4) NX = NY = NZ = atoi (argv[4]);	/* cube */
    if (argc > 5) NY = NZ = atoi (argv[5]);
    if (argc > 6) NZ = atoi (argv[6]);
    if (argc > 7) gt = atoi (argv[7]); //novo

    //Se n�o entrou com valores na linha de comando solicita entrada de dados
    //	if (argc == 1)
    {
        bool primeiraPassagem=true;
        char resp,newline;
        char entreCom[]="\n\tEntre com ";
        char linha[]="\n__________________________________________________________________";
        do
        {
            cout<<linha;
            cout<<"\n  ***  Programa de Reconstrucao Tridimensional (LMPT)  ***";
            cout<<linha;
            cout<<"\n\t\t\tEntrada de dados:";
            cout<<linha;
            if (   primeiraPassagem != true )
            {
                cout<<entreCom<<"o nome do arquivo ["<<nomeArquivoCz<<"]:";
                cin>>nomeArquivoCz;
                //cout<<entreCom<<"a sementeGeradorAleatorio ["<<sementeGeradorAleatorio<<"]:"; cin>>sementeGeradorAleatorio;
                cout<<entreCom<<"o fator de reducao ["<<fatorAmplificacaoNumeroPontos<<"]:";
                cin>>fatorAmplificacaoNumeroPontos;
                cout<<entreCom<<"o numeroPontos a serem lidos no arquivo de correlacao["<<numeroPontosCz<<"]:";
                cin>>numeroPontosCz;
                cout<<entreCom<<"a dimensao x da matriz NX ["<<NX<<"]:";
                cin>>NX;
                cout<<entreCom<<"a dimensao y da matriz NY ["<<NY<<"]:";
                cin>>NY;
                cout<<entreCom<<"a dimensao z da matriz NZ ["<<NZ<<"]:";
                cin>>NZ;
                cout<<entreCom<<"tipo de gaussiana gt1=1, gt2=2 ["<<gt<<"]:";
                cin>>gt; //novo
                cin.get();//Pega o ultimo return
                cout<<"\n\n\n"<<linha;
            }
            primeiraPassagem = false;
            cout<<"\nDados fornecidos:";
            cout<<linha<<linha;
            cout<<"\n\tNome do arquivo: ["<<nomeArquivoCz<<"]";
            //cout<<"\n\tsementeGeradorAleatorio: ["<<sementeGeradorAleatorio<<"]";
            cout<<"\n\tFatorAmplificacao: ["<<fatorAmplificacaoNumeroPontos<<"]";
            cout<<"\n\tNumeroPontosCorrelacaoCz: ["<<numeroPontosCz<<"]";
            cout<<"\n\tDimensao x da matriz NX: ["<<NX<<"]";
            cout<<"\n\tDimensao y da matriz NY: ["<<NY<<"]";
            cout<<"\n\tDimensao z da matriz NZ: ["<<NZ<<"]";
            cout<<"\n\tTipo de gaussiana (gt1 ou gt2) ["<<gt<<"]";
            cout<<linha;
            cout<<"\n\tValores corretos, iniciar reconstru��o...........(r)";
            cout<<"\n\tValores incorretos, modificar....................(m)";
            cout<<"\n\tAbandonar programa...............................(a)";
            cout<<linha;
            cin.get(resp);			//resp=getche();
            cin.get(newline);		//pega retorno de carro.
            if (resp=='a'||resp=='A')
                exit(1);
        }
        //while(resp!='r'||resp!='R');
        while (resp=='m'||resp=='M');
    }
    cout<<"\nFim da funcao de entrada de dados";

    cout<<"\nVai criar objeto de reconstrucao...";
    if (gt==1)
    {
        //cria objeto de reconstrucao
        CReconstrucaoZhirong reconstrucao(nomeArquivoCz, fatorAmplificacaoNumeroPontos, numeroPontosCz, NX,NY,NZ);
        cout<<"...pronto.";
        cout<<"\nVai executar Go...";
        reconstrucao.Go();					//executa a reconstrucao
        cout<<"...pronto.";
        cout<<"\nCalculando o covariograma...";
        reconstrucao.Covariograma();				//calcula e salva o covariograma,
    }
    else // gt = 2 ou maior
    {
        //cria objeto de reconstrucao
        CReconstrucaoBueno reconstrucao(nomeArquivoCz, fatorAmplificacaoNumeroPontos, numeroPontosCz, NX,NY,NZ,gt);
        cout<<"...pronto.";
        cout<<"\nVai executar Go...";
        reconstrucao.Go();					//executa a reconstrucao
        cout<<"...pronto.";
        cout<<"\nCalculando o covariograma...";
        reconstrucao.Covariograma();				//calcula e salva o covariograma,
    }
    cout<<"...pronto.";
    cout<<"\nReconstru��o Tridimensional encerrada.";

    cout<<"\nFoi criado o arquivo  log_reconstrucao.txt com as informa��es da reconstru��o ";
    cout<<"\nPressione qualquer tecla para encerrar";
    return 0;		//encerra o programa
}



