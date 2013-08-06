#include <iostream>
#include <Estatistica/CENormal.h>

using namespace std;

/**
 * Exemplo simples de uso da biblioteca de estatistica.
 *
 * Exercício: Gerar saida em modo texto e gerar gráficos no gnuplot.
 *
 * Tarefa:
 * Testar aqui as outras classes do diretorio de estatistica.
 * Gerar saida em modo texto e gerar gráficos no gnuplot.
 */
int
main (int argc, char *argv[])
{

    cout  << "\n================================================================================"
          << "\n====================== Programa de teste da LIB_LDSC ==========================="
          << "\n================================================================================" << endl;

    cout  << "\n================================================================================"
          << "\nUSO:"
          << "\n./estatistica"
          << "\n================================================================================" << endl;

    CENormal normal;
    double z, area;

    cout <<"\n\n-------->Valores de area em função de z.\n";
    for ( z = -3.0; z <= 3.0; z = z + 0.5)
    {
        area = 	normal.Go (z);
        cout << "normal.Go(" << z << ")    = " << area << endl;
    }

    cout <<"\n\n-------->Valores de z em função da area.\n";
    for ( area = -1.0; area <= 1.0; area = area + 0.1)
    {
        z  = normal.GoInv (area);
        cout << "normal.GoInv(" << area << ") = " << z << endl;
    }

    return 0;
}
