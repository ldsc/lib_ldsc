#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

#include <Geometria/Bola/BCDiscreta/CBCd4.h>
#include <Geometria/Bola/BCDiscreta/CBCd8.h>
#include <Geometria/Bola/BCDiscreta/CBCd34.h>
#include <Geometria/Bola/BCDiscreta/CBCd5711.h>
using namespace std;

/**
 * Exemplo simples de uso da biblioteca de objetos geometricos.
 *
 * Exercício: Gerar saida em modo texto e gerar gráficos no display.
 */
int
main (int argc, char *argv[])
{

    cout  << "\n================================================================================"
          << "\n====================== Programa de teste da LIB_LDSC ==========================="
          << "\n================================================================================" << endl;

    cout  << "\n================================================================================"
          << "\nUSO:"
          << "\n./geometria"
          << "\n================================================================================" << endl;

    cout << "Qual a dimensão da matriz que deseja criar (impar)? \n";
    int dimensao;
    cin >>  dimensao;
    cout << "Qual objeto geometrico deseja criar? \n"
         << "CDCd4    (1)\n"
         << "CDCd8    (2)\n"
         << "CDCd34   (3)\n"
         << "CDCd5711 (4)\n";
    int opcao;
    cin >> opcao;

    CBCDiscreta* bola = NULL;
    string nomebola;
    cout << "Criando bola selecionado.\n";
    switch (opcao)
    {
    case 1 :
        bola = new  	CBCd4(dimensao);
        nomebola = "CBCd4";
        break;
    case 2 :
        bola = new  	CBCd8(dimensao);
        nomebola = "CBCd8";
        break;
    case 3 :
        bola = new  	CBCd34(dimensao);
        nomebola = "CBCd34";
        break;
    case 4 :
        bola = new  	CBCd8(dimensao);
        nomebola = "CBCd5711";
        break;
    }

    ostringstream os;
    os << nomebola << "-" << dimensao << ".pgm";
    cout << "Salvando a bola em disco com o nome " << os.str() << endl;
    bola->Write( os.str() );

    string comando = string( "display ") + os.str()  + " &";
    system( comando.c_str() ) ;

    cout
        << "\nAtualmente esta salvando a imagem com ~65000 cores, "
        << "\nAbra os arquivos num editor e substitua o valor ~65000 pelo maior valor"
        << "encontrado, a seguir visualize a imagem gerada no display."<< endl;

    delete bola;

    return 0;
}

