#include <iostream>
#include <string>

#include <Interpolacao/CInterpolacao.h>
#include <Interpolacao/CIntBarycentric.h>
//#include <Interpolacao/CIntCubSpline.h>
#include <Interpolacao/CIntEDBarycentric.h>
#include <Interpolacao/CIntLagrange.h>
#include <Interpolacao/CIntNewtonDiff.h>
#include <Interpolacao/CIntNewtonDivDiff.h>

using namespace std;

/**
 * Exemplo simples de uso da biblioteca interpolacao.
 *
 * Exercício: Gerar saida em modo texto e gerar gráficos no gnuplot.
 */
int
main (int argc, char *argv[])
{
    cout  << "\n================================================================================"
          << "\n====================== Programa de teste da LIB_LDSC ==========================="
          << "\n================================================================================" << endl;

    cout  << "\n================================================================================"
          << "\nUSO:"
          << "\n./interpolacao"
          << "\n================================================================================" << endl;

    int nDados = 9;
    double vx[9] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    double vy[9] = {0.1, 1.3, 1.8, 3.5, 3.4, 5.8, 6.3, 7.0, 7.6};

    int opcao;
    do
    {
        cout << "Qual método de interpolacao deseja testar? \n"
             << " CIntBarycentric      (1) trava\n"
             //<< " CIntCubSpline        (2)\n"
             << " CIntEDBarycentric    (3)*\n"
             << " CIntLagrange         (4)*\n"
             << " CIntNewtonDiff       (5) trava\n:"
             << " CIntNewtonDivDiff    (6) trava\n:";
        cin >> opcao;

        CInterpolacao* interpolacao =NULL;
        cout << "Criando método integração selecionado.\n";
        switch (opcao)
        {
        case 1 :
            interpolacao = new CIntBarycentric;
            break;
            //case 2 : interpolacao = new CIntCubSpline;	break;
        case 3 :
            interpolacao = new CIntEDBarycentric;
            break;
        case 4 :
            interpolacao = new CIntLagrange;
            break;
        case 5 :
            interpolacao = new CIntNewtonDiff;
            break;
        case 6 :
            interpolacao = new CIntNewtonDivDiff;
            break;
        default:
            cerr << "Selecao errada, saindo." ;
            return 1;
        }

        for (double x=0; x<9; x++)
            cout << "x = " << x << " ~y = "
                 <<  interpolacao->Go (vx, vy, nDados, x) << endl;
        double x;
        do {
            cout << "Entre com o valor de x (ctrl+d encerra): ";
            cin >> x;
            cin.get();
            cout << "x = " << x << " ~y = " << interpolacao->Go (vx, vy, nDados, x) << endl;
        } while (cin.good());

        delete interpolacao;
        interpolacao = NULL ;
    } while ( opcao < 7 );

    return 0;
}
