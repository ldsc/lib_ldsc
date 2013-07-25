//Programa de teste da classe CGnuplot.
#include <iostream>

#include "Grafico/Gnuplot/CGnuplot.h"

using namespace std;       // Usando espaco de nomes da std

void wait_for_key ()
{
    cout << endl << "Pressione ENTER para continuar..." << endl;
    std::cin.clear();                             // Zera estado de cin
    std::cin.ignore(std::cin.rdbuf()->in_avail());// Ignora
    std::cin.get();                               // Espera o pressionamento do enter
    return;
}

int main(int argc, char* argv[])
{

    cout  << "\n================================================================================"
          << "\n====================== Programa de teste da LIB_LDSC ==========================="
          << "\n================================================================================" << endl;

    cout  << "\n================================================================================"
          << "\nUSO:"
          << "\n./cgnuplot.teste.min"
          << "\n================================================================================" << endl;

    Gnuplot::Terminal("wxt");
    // ------------- Graficos 2D ----------------
    Gnuplot g2d = Gnuplot("lines"); // Construtor
    g2d.Legend("inside").Legend("left").Legend("bottom").Legend("box");
    g2d.Title("Titulo do grafico"); // Titulo do grafico
    g2d.XLabel("rotulo eixo x");    // Rotulo eixo x
    g2d.YLabel("rotulo eixo y");    // Rotulo eixo y
    g2d.XRange(-10,10);             // Seta intervalo do eixo x.
    g2d.PlotEquation( "x*x*sin(x)");// Plota uma determinada equacao
    // Usando os diferentes estilos de graficos

    // Muda o estilo da funcao para linhas e replota
    cout << "Style(\"lines\")" << endl;
    g2d.Style("Style( lines )");    // Muda estilo linha
    g2d.Replot();                   // Replota o gráfico
    wait_for_key();

    // Muda o estilo da funcao para impulsos, muda titulo e plota nova equacao
    cout << "Style(\"impulses\")" << endl;
    g2d.Style("impulses").Title("Style( impulses )").PlotEquation( "x*x+ 5");
    wait_for_key();

    g2d.Reset();                    // Reseta estado do grafico
    cout << endl << "*** Fim do exemplo " << endl;
    return 0;
}

