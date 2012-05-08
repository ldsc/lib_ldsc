#include <iostream>
#include <string>

#include <Matriz/CMatriz2D.h>
#include <Filtro/FEspacial/CFEPassaAlta.h>
#include <Filtro/FEspacial/CFEPassaBaixa.h>
#include <Filtro/FEspacial/CFELaplaciano.h>
#include <Filtro/FEspacial/CFEBorda.h>
#include <Filtro/FEspacial/CFEEmboss.h>

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
 * Exerc�cio: Gerar saida em modo texto e gerar gr�ficos no display.
 */

int
main (int argc, char *argv[])
{
    cout << "Criando imagem a ser filtrada.\n";
    CMatriz2D* pm = new CMatriz2D("img.pgm");

    cout << "Qual filtro deseja testar? \n"
         << "CFEPassaBaixa (1)\n"
         << "CFEPassaAlta  (2)\n"
         << "CFELaplaciano (3)\n"
         << "CFEEmboss     (4)\n"
         << "CFEBorda      (5)\n:";
    int opcao;
    cin >> opcao;

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
    }

    cout << "Processando a filtragem.\n";
    filtro->Go(pm,1);

    cout << "Salvando a imagem em disco com o nome "<< nomeFiltro << ".pgm" << endl;
    pm->Write(nomeFiltro+".pgm");

    delete pm;
    delete filtro;

    /*
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
