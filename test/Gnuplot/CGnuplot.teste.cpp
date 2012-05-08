////////////////////////////////////////////////////////////////////////////////////////
//                          Programa de teste da classe CGnuplot.                     //
////////////////////////////////////////////////////////////////////////////////////////
// Esta interface usa pipes e nao ira funcionar em sistemas que nao suportam
// o padrao POSIX pipe.
//
// O mesmo foi testado em sistemas Windows (MinGW e Visual C++) e GNU/Linux(GCC/G++)
//
// Este programa foi originalmente escrito por:
// Historico de versoes:
// 0. Interface para linguagem C
//    por N. Devillard (27/01/03)
// 1. Interface para C++: tradução direta da versao em C
//    por Rajarshi Guha (07/03/03)
// 2. Correcoes para compatibilidadde com Win32
//    por V. Chyzhdzenka (20/05/03)
// 3. Novos métodos membros, correcoes para compatibilidade com Win32 e Linux
//    por M. Burgis (10/03/08)
// 4. Traducao para Portugues, documentacao e modificacoes na interface
//    por Bueno.A.D. (30/07/08)
//
////////////////////////////////////////////////////////////////////////////////////////
//
// Requisitos:
// - O programa gnuplot deve estar instalado (veja http://www.gnuplot.info/download.html)
// - No Windows: setar a Path do Gnuplot (i.e. C:/program files/gnuplot/bin)
//      ou setar a path usando: Gnuplot::set_GNUPlotPath(const std::string &path);
//      antes ded criar qualquer objeto da classe.
//
////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>

/// A classe CGnuplot  usa pipes no estilo POSIX para se comunicar com o gnuplot.
// POSIX-Pipe-communikation.
#include "CGnuplot.h"

// Se estamos no windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
#include <conio.h>        // Para acesso a getch(), necessario em wait_for_key()
#include <windows.h>      // Para acesso a Sleep()
void sleep(int i) {
    Sleep(i*1000);
}
#endif

// Variaveis globais
const int SLEEP_LGTH = 2;  // Tempo de espera em segundos
const int NPOINTS  = 50;   // Dimensao do array (vetor)
//#define  SLEEP_LGTH  2
//#define  NPOINTS   50

using namespace std;       // Usando espaco de nomes da std

/// @brief O programa para ate o pressionamento de uma tecla.
void wait_for_key();

/// @brief Funcao principal
int main(int argc, char* argv[])
{
    // Se a variavel do gnuplot nao esta setada, faca isto antes de
    // criar objetos da classe CGnuplot, usando o método publico e estatico:
    // Gnuplot::set_GNUPlotPath("C:/program files/gnuplot/bin/");

    // Seta o terminal padrao para visualizacao dos graficos  (normalmente nao necessario),
    // Usuarios de Mac dedvem usar a opcao "aqua", e nao x11.
    // Gnuplot::set_terminal_std("x11");
    cout << "-------------------------------------------------------------\n"
         << "--> Plotando graficos do gnuplot usando a classe CGnuplot <--\n"
         << "--> Exemplo de controle do gnuplot usando C++             <--\n"
         << "--> Os titulos do grafico ilustram a operacao realizada   <--\n"
         << "-------------------------------------------------------------\n" << endl;

    // Controla a ocorrencia de excessoes, usa a classe GnuplotException
    try
    {

        // Teste geral
        // Terminal padrao do gnuplot no fedora 9
        // Se nao funcionar em seu sistema, comente a linha
        Gnuplot::Terminal("wxt");


        // ------------- Graficos 2D ----------------
        Gnuplot g2d = Gnuplot("lines"); // Construtor
        g2d.PointSize(0.8);             // Escala o tamanho do ponto usado na plotagem
        // Legenda
        g2d.Legend("inside").Legend("left").Legend("bottom").Legend("box");
        g2d.Title("Titulo do grafico"); // Titulo do grafico
        g2d.XLabel("rotulo eixo x");    // Rotulo eixo x
        g2d.YLabel("rotulo eixo y");    // Rotulo eixo y
        g2d.XRange(-10,10);             // Seta intervalo do eixo x.
        g2d.PlotEquation( "x*x*sin(x)");// Plota uma determinada equacao

        // Usando os diferentes estilos de graficos
        cout << "Style(\"lines\")" << endl;
        g2d.Style("Style( lines )").Replot();
        wait_for_key();
        g2d.Reset();                    // Reseta estado do grafico
        cout << "Style(\"points\")" << endl;
        g2d.Style("points").Title("Style( points )").PlotEquation( "x");
        wait_for_key();
        g2d.Reset();
        cout << "Style(\"linespoints\")" << endl;
        g2d.Style("linespoints").Title("Style( linespoints )").PlotEquation( "x*x");
        wait_for_key();
        g2d.Reset();
        cout << "Style(\"impulses\")" << endl;
        g2d.Style("impulses").Title("Style( impulses )").PlotEquation( "x*x+ 5");
        wait_for_key();
        g2d.Reset();
        cout << "Style(\"dots\")" << endl;
        g2d.Style("dots").Title("Style( dots )").PlotEquation( "x*x*x");
        wait_for_key();
        g2d.Reset();
        cout << "Style(steps)" << endl;
        g2d.Style("steps").Title("Style( steps )").PlotEquation( "x*x*x*x");
        wait_for_key();
        g2d.Reset();
        cout << "Style(\"fsteps\")" << endl;
        g2d.Style("fsteps").Title("Style( fsteps )").PlotEquation( "x*x*sin(x)");
        wait_for_key();
        g2d.Reset();
        cout << "Style(\"histeps\")" << endl;
        g2d.Style("histeps").Title("Style( histeps )").PlotEquation( "x*x*sin(x)");
        wait_for_key();

        // Legendas, posicoes possiveis
        cout << "Legend(\"inside left top nobox\")" << endl;
        g2d.Legend("inside left top nobox").Title("Legend( inside left top nobox )").Replot();
        wait_for_key();
        cout << "Legend(\"inside center center nobox\")" << endl;
        g2d.Legend("inside center center nobox").Title("Legend( inside center center nobox )").Replot();
        wait_for_key();
        cout << "Legend(\"inside right bottom box\")" << endl;
        g2d.Legend("inside right bottom box").Title("Legend( inside right bottom box )").Replot();
        wait_for_key();
        cout << "Legend(\"outside right top box\")" << endl;
        g2d.Legend("outside right top box").Title("Legend( outside right top box )").Replot();
        wait_for_key();

        // ------------- Graficos 3D ----------------
        Gnuplot g3d = Gnuplot("lines"); // Construtor
        g3d.Grid(1);                    // Ativa/Desativa o grid
        g3d.Samples(50);                // Seta taxa de amostragem
        g3d.IsoSamples(50);             // Seta densidade de isolinhas
        g3d.Hidden3d();                 // Ativa/Desativa remocao de linhas ocultas
        g3d.Surface();                  // Ativa/Desativa a visualizacao da superficie
        g3d.Title("Titulo do grafico"); // Titulo do grafico
        g3d.XLabel("rotulo eixo x");    // Rotulo eixo x
        g3d.YLabel("rotulo eixo y");    // Rotulo eixo y
        g3d.ZLabel("rotulo eixo z");    // Rotulo eixo z
        g3d.XRange(-10,10);             // Seta intervalo do eixo x.
        g3d.ZAutoscale();               // Seta autoescala de z
        g3d.PlotEquation3d( "x*sin(x)*sin(y)+4" );
        wait_for_key();

        // Suavizacao
        cout << "Smooth(0)" << endl;
        g3d.Smooth(0).Title("Smooth(0)").Replot();         // Desativa suavizacao
        wait_for_key();
        cout << "Smooth(1)" << endl;
        g3d.Smooth(1).Title("Smooth(1)").Replot();         // Ativa suavizacao
        wait_for_key();

        // Ativar/desativar grid
        cout << "Grid()" << endl;
        g3d.Grid().Title("Grid()").Replot();
        wait_for_key();
        cout << "Grid(0)" << endl;
        g3d.Grid(0).Title("Grid(0)").Replot();
        wait_for_key();

        // Ocultar linhas escondidas
        cout << "Hidden3d(0)" << endl;
        g3d.Hidden3d(0).Title("Hidden3d(0)").Replot();
        wait_for_key();
        cout << "Hidden3d()" << endl;
        g3d.Hidden3d().Title("Hidden3d()").Replot();
        wait_for_key();

        // Taxa amostragem
        cout << "Samples(10)" << endl;
        g3d.Samples(10).Title("Samples(10)").Replot();
        wait_for_key();
        cout << "Samples(50)" << endl;
        g3d.Samples(50).Title("Samples(50)").Replot();
        wait_for_key();

        // Densidade de isolinhas
        cout << "IsoSamples(10)" << endl;
        g3d.IsoSamples(10).Title("IsoSamples(10)").Replot();
        wait_for_key();
        cout << "IsoSamples(50)" << endl;
        g3d.IsoSamples(50).Title("IsoSamples(50)").Replot();
        wait_for_key();

        // Contorno em superficies, base, surface, both.
        cout << "Contour(\"base\")" << endl;
        g3d.Contour("base").Title("Contour(base)").Replot();
        wait_for_key();
        cout << "Contour(\"surface\")" << endl;
        g3d.Contour("surface").Title("Contour(surface)").Replot();
        wait_for_key();
        cout << "Contour(\"both\")" << endl;
        g3d.Contour("both").Title("Contour(both)").Replot();
        wait_for_key();


        // ------------- A seguir exemplos do codigo original ----------------
        Gnuplot g1 = Gnuplot("lines");
        cout << "*** Plota uma equacao da forma y = ax + b; com a=1, b=0" << endl;
        g1.Title("PlotSlope y = x");                     // Seta o titulo.
        g1.PlotSlope(1.0,0.0,"PlotSlope y = x");         // Plota Reta
        wait_for_key();

        cout << "*** Plota uma equacao da forma y = ax + b; com a=2, b=0" << endl;
        cout << "PlotSlope y = 2*x" << endl;
        g1.PlotSlope(2.0,0.0,"y = 2x");
        wait_for_key();

        cout << "*** Plota uma equacao da forma y = ax + b; com a=-1, b=0" << endl;
        cout << "PlotSlope y = -x" << endl;
        g1.PlotSlope(-1.0,0.0,"y = -x");
        wait_for_key();
        g1.Title();

        // Equacoes
        g1.ResetPlot();                        // Reseta o grafico
        cout << endl << endl << "*** Plotando Equacoes" << endl;

        cout << "*** PlotEquation y = sin(x)" << endl;
        g1.PlotEquation("sin(x)","PlotEquation sine, sin(x)");      // Plota uma equacao
        wait_for_key();

        cout << "*** y = log(x)" << endl;
        g1.Legend("box").Legend("left").PlotEquation("log(x)","PlotEquation logarithm, log(x)"); // Plota uma equacao
        wait_for_key();

        cout << "*** y = sin(x) * cos(2*x)" << endl;
        g1.PlotEquation("sin(x)*cos(2*x)","PlotEquation, sin(x)*cos(2*x)"); // Plota uma equacao
        wait_for_key();

        // Controlando estilos de graficos - styles
        g1.ResetPlot();
        cout << endl << endl << "*** Mostrando estilos -  styles" << endl;

        cout << "*** sin(x) usando PointSize(0.8).Style(\"points\")" << endl;
        g1.PointSize(0.8).Style("points");
        g1.PlotEquation("sin(x)","PlotEquation sin(x), usando points");
        wait_for_key();

        cout << "*** sine usando estilo de impulses" << endl;
        g1.Style("impulses");
        g1.PlotEquation("sin(x)","PlotEquation sin(x), usando impulses");
        wait_for_key();

        cout << "*** sine usando estilo de steps" << endl;
        g1.Style("steps");
        g1.PlotEquation("sin(x)","PlotEquation sin(x), usando steps");
        wait_for_key();

        // Salvando para arquivo postscript - ps
        g1.ResetAll();                            // Reseta todos os dados
        cout << endl << endl << "*** Salvando para arquivo postscript - ps" << endl;

        cout << "y = sin(x) salvo no arquivo test_output.ps no diretorio corrente" << endl;
        g1.SaveTops("test_output");
        g1.Style("lines").Samples(300).XRange(0,5);
        g1.PlotEquation("sin(12*x)*exp(-x)").PlotEquation("exp(-x)");

        cout << "*** Plotando novamente em uma janela" << endl;
        g1.ShowOnScreen();                        // Ativa janela de saida grafica

        // Usando vetores do usuario (conjunto de dados)
        cout << "*** Criando vetores x, y, y2, dy, z a serem plotados" << endl;
        std::vector<double> x, y, y2, dy, z;

        for (int i = 0; i < NPOINTS; i++)         // Preenche os vetores x, y, z
        {
            x.push_back((double)i);               // x[i] = i
            y.push_back((double)i * (double)i);   // y[i] = i^2
            z.push_back( x[i]*y[i] );             // z[i] = x[i]*y[i] = i^3
            dy.push_back((double)i * (double)i / (double) 10); // dy[i] = i^2 / 10
        }
        y2.push_back(0.00);
        y2.push_back(0.78);
        y2.push_back(0.97);
        y2.push_back(0.43);
        y2.push_back(-0.44);
        y2.push_back(-0.98);
        y2.push_back(-0.77);
        y2.push_back(0.02);

        g1.ResetAll();
        cout << endl << endl << "*** Plota vetor y de doubles" << endl;
        g1.Style("impulses").PlotVector(y,"PlotVector y, usando impulses");
        wait_for_key();

        g1.ResetPlot();
        cout << endl << endl << "*** Plota vetores x e y, pares ordenados (x,y)" << endl;
        g1.Grid();
        g1.Style("points").PlotVector(x,y,"PlotVector x e y, pares ordenados (x,y), usando points");
        wait_for_key();

        g1.ResetPlot();
        cout << endl << endl << "*** Plota vetores x e y e z, valores ordenados (x,y,z)" << endl;
        g1.Grid(0);
        g1.PlotVector(x,y,z,"PlotVector (x,y,z), usanddo points");
        wait_for_key();

        g1.ResetPlot();
        cout << endl << endl << "*** Plota vetores x, y e dy, valores ordenados e barra de erro (x,y,dy)" << endl;
        g1.PlotVectorXYErrorBar(x,y,dy,"PlotVectorXYErrorBar  valores ordenados x,y e barra de erro (x,y,dy)");
        wait_for_key();

        // Usando multiplas janelas de saida
        cout << endl << endl;
        cout << "*** multiple output windows" << endl;

        g1.ResetPlot();
        g1.Style("lines");
        cout << "window 1: sin(x)" << endl;
        g1.Grid(1).Samples(600).XRange(0,300);
        g1.PlotEquation("sin(x)+sin(x*1.1)", "PlotEquation Grid(1).Samples(600).XRange(0,300)");
        wait_for_key();

        g1.XAutoscale().Title("XAutoscale()").replot();
        wait_for_key();

        Gnuplot g2;
        cout << "Janela 2: plotando vetores" << endl;
        g2.PlotVector(y2,"Pontos de y2");
        g2.Smooth().PlotVector(y2,"Smooth(cspline)");
        g2.Smooth("bezier").PlotVector(y2,"Smooth(bezier)");
        g2.Smooth();
        wait_for_key();

        cout << "Janela 3: plotando equacoes, log(x)/x" << endl;
        Gnuplot g3("lines");
        g3.Grid(1);
        g3.PlotEquation("log(x)/x","log(x)/x");
        wait_for_key();

        cout << "Janela 4: splot x*x+y*y" << endl;
        Gnuplot g4("lines");
        g4.ZRange(0,100);
        g4.XLabel("x-axis").YLabel("y-axis").ZLabel("z-axis");
        g4.PlotEquation3d("x*x+y*y");
        wait_for_key();

        cout << "Janela 5: splot usando Hidden3d" << endl;
        Gnuplot g5("lines");
        g5.IsoSamples(25).Hidden3d();
        g5.PlotEquation3d("x*y*y");
        wait_for_key();

        Gnuplot g6("lines");
        cout << "Janela 6: splot usando Contour" << endl;
        g6.IsoSamples(60).Contour();
        g6.Surface().PlotEquation3d("sin(x)*sin(y)+4");
        wait_for_key();

        g6.Surface().Replot();
        wait_for_key();

        Gnuplot g7("lines");
        cout << "Janela 7: usando Samples" << endl;
        g7.XRange(-30,20).Samples(40);
        g7.PlotEquation("besj0(x)*0.12e1").PlotEquation("(x**besj0(x))-2.5");
        wait_for_key();

        g7.Samples(400).Replot();
        wait_for_key();

        Gnuplot g8("filledcurves");
        cout << "Janela 8: filledcurves" << endl;
        g8.Legend("outside right top").XRange(-5,5);
        g8.PlotEquation("x*x").PlotEquation("-x*x+4");

        // Plota uma imagem
        Gnuplot g9;
        cout << "Janela 9: plot_image" << endl;
        const int iWidth  = 255;
        const int iHeight = 255;
        g9.XRange(0,iWidth).set_yrange(0,iHeight).CBRange(0,255);
        g9.Command("set palette gray");
        unsigned char ucPicBuf[iWidth*iHeight];
        // Gera imagem em tons dde cinza
        for (int iIndex = 0; iIndex < iHeight*iWidth; iIndex++)
        {
            ucPicBuf[iIndex] = iIndex % 255;
        }
        g9.plot_image(ucPicBuf,iWidth,iHeight,"greyscale");
        wait_for_key();

        g9.PointSize(0.6).Legend(0).PlotSlope(0.8,20);
        wait_for_key();

        // Controle manual
        Gnuplot g10;
        cout << "Janela 10: controle manual" << endl;
        g10.Cmd("set samples 400").Cmd("plot abs(x)/2"); // Usando Cmd()
        g10 << "replot sqrt(x)" << "replot sqrt(-x)";    // Usando operador  <<
        wait_for_key();

    }
    catch (GnuplotException ge)
    {
        cout << ge.what() << endl;
    }

    cout << endl << "*** Fim do exemplo " << endl;

    return 0;
}

void wait_for_key ()
{
    // Todos as teclas serao considedradas, inclusive as setas
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    cout << endl << "Pressione qualquer tecla para continuar..." << endl;
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    cout << endl << "Pressione ENTER para continuar..." << endl;
    std::cin.clear();                             // Zera estado de cin
    std::cin.ignore(std::cin.rdbuf()->in_avail());// Ignora
    std::cin.get();                               // Espera o pressionamento do enter
#endif
    return;
}
