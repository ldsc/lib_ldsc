#include <iostream>
#include <string>
#include <Matriz/CMatriz2D.h>
#include <Filtro/FEspacial/CFEPassaAlta.h>
#include <Filtro/FEspacial/CFEPassaBaixa.h>
#include <Filtro/FEspacial/CFELaplaciano.h>
#include <Filtro/FEspacial/CFEBorda.h>
#include <Filtro/FEspacial/CFEEmboss.h>

#include <Filtro/FEspacial/FEDeteccaoContorno/CFEDeteccaoContorno.h>
#include <Filtro/FEspacial/FEDeteccaoContorno/CFEDCCanny.h>
#include <Filtro/FEspacial/FEDeteccaoContorno/CFEDCMarrHildreth.h>
#include <Filtro/FEspacial/FEDeteccaoContorno/CFEDCShenCastan.h>

#include <Filtro/FEspacial/FEEsqueleto/CFEEsqueletoV4.h>

#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFd34.h>

/*
  Formato do arquivo de disco img.pgm
  P2
  717 562
  65535
  18548 18912 19190 18734 18610 18530 18604 18258 18305 18427 18912 18411
  18544 18104 18152 18303 17895 18104 18526 18804 18308 18234 18414 18261
  ...
*/
using namespace std;

/**
 * Exemplo simples de uso da biblioteca de filtros.
 *
 * Exercício: Gerar saida em modo texto e gerar gráficos no display.
 */

/// Testa a determinação do esqueleto usando modelo 4 de Bueno
void EmTeste()
{
    cout <<  " Vai determinar o esqueleto usando modelo 4."<< endl;
    CMatriz2D* pm = new CMatriz2D("logoLDSC.pbm");
    CFEEsqueletoV4* esqueleto = new CFEEsqueletoV4(pm);
    esqueleto->Go (pm);
    pm->Write("logoLDSC-CFEEsqueletoV4.pgm");
    delete esqueleto;
    delete pm;
}

/// Testa alguns filtros
int
main (int argc, char *argv[])
{

    cout  << "\n================================================================================"
          << "\n====================== Programa de teste da LIB_LDSC ==========================="
          << "\n================================================================================" << endl;

    cout  << "\n================================================================================"
          << "\nUSO:"
          << "\n./filtro"
          << "\n================================================================================" << endl;

    // EmTeste();
    cout << "Qual filtro deseja testar?\n"
         << "----> Filtros espaciais\n"
         << "\tCFEPassaBaixa     (1)\n"
         << "\tCFEPassaAlta      (2)\n"
         << "\tCFELaplaciano     (3)\n"
         << "\tCFEEmboss         (4)\n"
         << "\tCFEBorda          (5)\n:"//;

         << "----> Filtros detecção contorno\n"
         << "\tCFEDCCanny        (6) trava?\n"//;
         << "\tCFEDCMarrHildreth (7) - roda mas fica tudo preto?\n"//;
         << "\tCFEDCShenCastan   (8) trava?\n"

         << "----> Filtros determinação esqueleto\n"
         << "\tCFEEsqueletoV4    (9)  (imagem pbm)\n"

         << "----> Filtros morfologia matematica\n"
         << "\tCFEMMIDFd34       (10) (imagem pbm)\n";

    int opcao;
    cin >> opcao;
    cin.get();

    cout << "Entre com o nome da imagem a ser filtrada (*.pgm ou *.pbm):";
    string nomeArquivo;
    system("ls *.pgm *.pbm");
    getline(cin,nomeArquivo);

    // visualizando a imagem com display
    string comando = "display " + nomeArquivo + "& ";
    system( comando.c_str() );

    cout << "Criando imagem a ser filtrada.\n";
    CMatriz2D* pm = new CMatriz2D(nomeArquivo);

    CFiltro* filtro = NULL;
    string nomeFiltro;
    cout << "Criando filtro selecionado.\n";
    switch (opcao)
    {

    case 1 :
        filtro = new  	CFEPassaBaixa(pm,5);
        nomeFiltro = "CFEPassaBaixa";
        break;
    case 2 :
        filtro = new  	CFEPassaAlta(pm,1);
        nomeFiltro = "CFEPassaAlta";
        break;
    case 3 :
        filtro = new  	CFELaplaciano(pm,1);
        nomeFiltro = "CFELaplaciano";
        break;
    case 4 :
        filtro = new  	CFEEmboss(pm,1);
        nomeFiltro = "CFEEmboss";
        break;
    case 5 :
        filtro = new  	CFEBorda(20,pm,1);
        nomeFiltro = "CFEBorda";
        break;

    case 6 :
        filtro = new  	CFEDCCanny(pm,1);  // Deteccao Contorno
        nomeFiltro = "CFEDCCanny";
        break;
    case 7 :
        filtro = new  	CFEDCMarrHildreth (pm,1);
        nomeFiltro = "CFEDCMarrHildreth";
        break;
    case 8 :
        filtro = new  	CFEDCShenCastan(pm,1);
        nomeFiltro = "CFEDCShenCastan";
        break;

    case 9 :
        filtro = new  	CFEEsqueletoV4(pm,1);
        nomeFiltro = "CFEEsqueletoV4";
        break;

    case 10 :
        //delete pm;
        //pm = new CMatriz2D("figuras-geometricas.pbm");
        filtro = new  	CFEMMIDFd34(pm);
        nomeFiltro = "CFEMMIDFd34";
        break;
    }

    cout << "Processando a filtragem.\n";
    filtro->Go(pm,5);

    pm->NumCores(256);

    cout << "Salvando a imagem em disco com o nome "<< nomeFiltro << ".pgm" << endl;
    pm->Write(nomeFiltro+".pgm");


    if (opcao == 10)
    {
        CFEMMIDF* idf	= static_cast<CFEMMIDF*>(filtro);

        idf->NumCores(256);
        cout << "Salvando a imagem em disco com o nome "<< nomeFiltro << ".pgm" << endl;
        idf->Write(nomeFiltro+".pgm");
    }

    string comandoSaida = "display " + nomeFiltro + ".pgm  & ";
    system( comandoSaida.c_str() );

    delete pm;
    delete filtro;

    /*
      // Abaixo exemplo mais direto
      {
      cout << "Criando imagem a ser filtrada.\n";
      CMatriz2D* imgPassaAlta = 	new CMatriz2D(*pm);

      cout << "Criando filtro passa alta.\n";
      CFiltro* filtroPassaAlta = new CFEPassaAlta(imgPassaAlta,1);

      cout << "Processando a filtragem.\n";
      filtroPassaAlta->Go(pm,1);

      cout << "Salvando a imagem em disco.\n";
      pm->Write("imgPassaAlta.pgm");

      delete imgPassaAlta;
      delete filtroPassaAlta;
      }
    */


    return 0;
}
