
#include <iomanip>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include "CAberturaDilatacao.h"
#include "CUtil.h"

#include <Filtro/FEspacial/FEMorfologiaMatematica/CFEMorfologiaMatematica.h>
#include <Geometria/Bola/BCDiscreta/CBCd34.h>
#include <Matriz/TCMatriz2D.h>

#include "Grafico/Gnuplot/CGnuplot.h"


using namespace std;

// Parâmetros aberturaDilatacao:
// Modelo NomeImagem raioMaximoElementoEstruturante fatorReducaoRaioElemEst incrementoRaioElementoEstruturante
int main ( int argc, char *argv[] )
{
    // int opcao, eleStrut;
    int eleStrut;
    string nomeimg;
    string linha = "---------------------------------------------\n";

    enum EOpcao { 	Modelo_0 = 0, Modelo_1, Modelo_2, Modelo_3, Modelo_4,
                   Erosao, Dilatacao, Abertura, Fechamento,
                   Porosidade, DistribuicaoTotalPoros, ImagemSintetica, Visualizar, PlotarGraficos, FTeste,
                   SAIR = 20
                };

    int iOpcao;
    EOpcao 	Opcao  ;

    // fator usado como critério de parada
    int raioMaximoElementoEstruturante = 25;

    // fator usado para reducao do Raio do Elemento Estruturante no processamento da dilatacao.
    int fatorReducaoRaioElemEst  = 1;

    // Valor do incremento do raio do elemento estruturante
    int incrementoRaioElementoEstruturante = 1;

    // SalvarResultadosParciais
    bool salvarResultadosParciais;

    do {
        cout << linha+linha;
        cout.setf (ios::right);
        cout.width(20);
        cout << "OPERACOES DA MORFOLOGIA MATEMATICA \n";
        cout << linha+linha;
        cout << "\t0  - Distribuicao Total Sitos e Ligacoes (método Abertura-Dilatacao Modelo 0 - Antigo/Artigo)\n";
        cout << "\t1  - Distribuicao Total Sitos e Ligacoes (método Abertura-Dilatacao Modelo 1)\n";
        cout << "\t2  - Distribuicao Total Sitos e Ligacoes (método Abertura-Dilatacao Modelo 2)\n";
        cout << "\t3  - Distribuicao Total Sitos e Ligacoes (método Abertura-Dilatacao Modelo 3)\n";
        cout << "\t4  - Distribuicao Total Sitos e Ligacoes (método Abertura-Dilatacao Modelo 4)\n";
        cout << "\n";

        cout << "\t5  - Erosao\n";
        cout << "\t6  - Dilatacao\n";
        cout << "\t7  - Abertura\n";
        cout << "\t8  - Fechamento\n";
        cout << "\n";
        cout << "\t9  - Porosidade\n";
        cout << "\t10  - Distribuicao Total Poros (método normal)\n";
        cout << "\t11 - Gerar imagem sintética (D34)\n";
        cout << "\n";
        cout << "\t12 - Mostrar/Visualizar/Display na imagem\n";
        cout << "\t13 - Plotar grafico de distribuicao\n";
        cout << "\t14 - Funcao em teste\n";
        cout << "\t15 - Funcao SequenciaAberturaTonsCinza - Salva imagem abertura em tons de cinza\n";
        cout << "\t16 - Calcula a porosidade\n";
        cout << "\t17 - Seta flag para salvar resultados parciais em disco\n";
        cout << "\n";
        cout.clear();
        cout << "\nDigite 20 para encerrar o programa...\n";
        cout <<  "\nOpcao: ";
        cin >> iOpcao;
        cin.get();

TCMatriz2D<int> lixo;
        TCMatriz2D< int > * pm = NULL;
        CFEMorfologiaMatematica * filtro = NULL;
        CAberturaDilatacao * carac = NULL;
        int dimensao;

        CBCd34 * bola = NULL;
        CUtil * util = NULL;
        ostringstream os;

        // Em teste
        int borda ;
        // graficos
        string nomeDistribuicao;
        Gnuplot::Terminal("wxt");
        Gnuplot g2d = Gnuplot("linespoints"); // Construtor
        g2d.PointSize(0.8);             // Escala o tamanho do ponto usado na plotagem
        g2d.Legend("inside").Legend("right").Legend("bottom").Legend("box");// Legenda

        double porosidade;
        Opcao  = (EOpcao)  iOpcao ;
        switch ( Opcao )
        {
//                        case 0:
        case Modelo_0:
        {
            cout << "Selecionou método abertura-dilatacao, determina distribuicao sitios e ligacoes, Modelo 0" << endl;
            system("ls");
            cout << "Nome da imagem (nome.extensao): " << endl;
            cin >> nomeimg;
            cin.get(); // pega o enter
            pm = new TCMatriz2D< int >(nomeimg);
            cout << "Carregou imagem->" << nomeimg << endl;

            carac = new CAberturaDilatacao(pm,nomeimg);
            cout << "Criou objeto CAberturaDilatacao." << endl;
            if ( pm == NULL  || carac == NULL)
            {
                cerr << "Falha alocação Matriz ou CAberturaDilatacao.\n" ;
                exit(0);
            }

            cout << "Entre com o raioMaximoElementoEstruturante : " << endl;
            cin >> raioMaximoElementoEstruturante ;
            cin.get(); // pega o enter
            carac->RaioMaximoElementoEstruturante( raioMaximoElementoEstruturante );

            cout << "Entre com o fatorReducaoRaioElemEst  : " << endl;
            cin >> fatorReducaoRaioElemEst  ;
            cin.get(); // pega o enter
            carac->FatorReducaoRaioElemEst  ( fatorReducaoRaioElemEst  );

            cout << "Entre com o incrementoRaioElementoEstruturante  : " << endl;
            cin >> incrementoRaioElementoEstruturante  ;
            cin.get(); // pega o enter
            carac->IncrementoRaioElementoEstruturante ( incrementoRaioElementoEstruturante  );

            carac->DistSitiosLigacoes_Modelo_0();

            cout << "Calculou carac->DistSitiosLigacoes_Modelo_0();." << endl;
            delete pm;
            pm  = NULL;
            delete carac;
            carac = NULL;
            break;
        }//fim opcao5
//                        case 1:
        case Modelo_1:
        {
            cout << "Selecionou método abertura-dilatacao, determina distribuicao sitios e ligacoes, Modelo 1." << endl;
            system("ls");
            cout << "Nome da imagem (nome.extensao): " << endl;
            cin >> nomeimg;
            cin.get(); // pega o enter
            pm = new TCMatriz2D< int >(nomeimg);
            cout << "Carregou imagem->" << nomeimg << endl;

            carac = new CAberturaDilatacao(pm,nomeimg);
            cout << "Criou objeto CAberturaDilatacao." << endl;
            if ( pm == NULL  || carac == NULL)
            {
                cerr << "Falha alocação Matriz ou CAberturaDilatacao.\n" ;
                exit(0);
            }

            cout << "Entre com o raioMaximoElementoEstruturante : " << endl;
            cin >> raioMaximoElementoEstruturante ;
            cin.get(); // pega o enter
            carac->RaioMaximoElementoEstruturante( raioMaximoElementoEstruturante );

            cout << "Entre com o fatorReducaoRaioElemEst  : " << endl;
            cin >> fatorReducaoRaioElemEst  ;
            cin.get(); // pega o enter
            carac->FatorReducaoRaioElemEst  ( fatorReducaoRaioElemEst  );

            cout << "Entre com o incrementoRaioElementoEstruturante  : " << endl;
            cin >> incrementoRaioElementoEstruturante  ;
            cin.get(); // pega o enter
            carac->IncrementoRaioElementoEstruturante ( incrementoRaioElementoEstruturante  );

            carac->DistSitiosLigacoes_Modelo_1();
            cout << "Calculou carac->DistSitiosLigacoes_Modelo_1();." << endl;
            delete pm;
            pm  = NULL;
            delete carac;
            carac = NULL;
            break;
        }//fim opcao5
//                        case 2:
        case Modelo_2:
        {
            cout << "Selecionou Modelo 2." << endl;
            system("ls");
            cout << "Nome da imagem (nome.extensao): " << endl;
            cin >> nomeimg;
            cin.get(); // pega o enter
            pm = new TCMatriz2D< int >(nomeimg);
            cout << "Carregou imagem->" << nomeimg << endl;

            carac = new CAberturaDilatacao(pm,nomeimg);
            cout << "Criou objeto CAberturaDilatacao." << endl;
            if ( pm == NULL  || carac == NULL)
            {
                cerr << "Falha alocação Matriz ou CAberturaDilatacao.\n" ;
                exit(0);
            }

            cout << "Entre com o raioMaximoElementoEstruturante : " << endl;
            cin >> raioMaximoElementoEstruturante ;
            cin.get(); // pega o enter
            carac->RaioMaximoElementoEstruturante( raioMaximoElementoEstruturante );

            cout << "Entre com o fatorReducaoRaioElemEst  : " << endl;
            cin >> fatorReducaoRaioElemEst  ;
            cin.get(); // pega o enter
            carac->FatorReducaoRaioElemEst  ( fatorReducaoRaioElemEst  );

            cout << "Entre com o incrementoRaioElementoEstruturante  : " << endl;
            cin >> incrementoRaioElementoEstruturante  ;
            cin.get(); // pega o enter
            carac->IncrementoRaioElementoEstruturante ( incrementoRaioElementoEstruturante  );

            carac->DistSitiosLigacoes_Modelo_2();
            cout << "Calculou carac->DistSitiosLigacoes_Modelo_2();." << endl;
            delete pm;
            pm  = NULL;
            delete carac;
            carac = NULL;

            break;
        }//fim opcao5
//                        case 3:
        case Modelo_3:
        {
            cout << "Selecionou Modelo 3." << endl;
            system("ls");
            cout << "Nome da imagem (nome.extensao): " << endl;
            cin >> nomeimg;
            cin.get(); // pega o enter
            pm = new TCMatriz2D< int >(nomeimg);
            cout << "Carregou imagem->" << nomeimg << endl;

            carac = new CAberturaDilatacao(pm,nomeimg);
            cout << "Criou objeto CAberturaDilatacao." << endl;
            if ( pm == NULL  || carac == NULL)
            {
                cerr << "Falha alocação Matriz ou CAberturaDilatacao.\n" ;
                exit(0);
            }

            cout << "Entre com o raioMaximoElementoEstruturante : " << endl;
            cin >> raioMaximoElementoEstruturante ;
            cin.get(); // pega o enter
            carac->RaioMaximoElementoEstruturante( raioMaximoElementoEstruturante );

            cout << "Entre com o fatorReducaoRaioElemEst  : " << endl;
            cin >> fatorReducaoRaioElemEst  ;
            cin.get(); // pega o enter
            carac->FatorReducaoRaioElemEst  ( fatorReducaoRaioElemEst  );

            cout << "Entre com o incrementoRaioElementoEstruturante  : " << endl;
            cin >> incrementoRaioElementoEstruturante  ;
            cin.get(); // pega o enter
            carac->IncrementoRaioElementoEstruturante ( incrementoRaioElementoEstruturante  );

            carac->DistSitiosLigacoes_Modelo_3();
            cout << "Calculou carac->DistSitiosLigacoes_Modelo_3();." << endl;
            delete pm;
            pm  = NULL;
            delete carac;
            carac = NULL;
            break;
        }//fim opcao5

//                        case 4:
        case Modelo_4:
        {
            cout << "Selecionou Modelo 4." << endl;
            cout << "NAO FAZ NADA - IMPLEMENTAR." << endl;
            break;
        }//fim opcao5

        case 5:
        {
            cout << "Selecionou erosao." << endl;
            system("ls");
            cout << "Nome da imagem (nome.extensao): " << endl;
            cin >> nomeimg;
            cin.get();
            cout << "Tamanho do elemento estruturante: " << endl;
            cin >> eleStrut;
            cin.get(); // pega o enter
            pm = new TCMatriz2D< int >( nomeimg );
            cout << " Carregou a matriz do disco...." << endl;
            filtro = new CFEMorfologiaMatematica( pm , eleStrut );
            cout << " Criou o filtro...." << endl;
            if ( pm == NULL  || filtro == NULL)
            {
                cerr << "Falha alocação Matriz ou filtro.\n" ;
                exit(0);
            }
            filtro->Erosao(pm,eleStrut);
            cout << " Aplicou o filtro...." << endl;

            string nomeimgaux = "erodida_" + nomeimg ;
            //pm->Write( "erodida.pbm" );
            pm->Write( nomeimgaux );
            cout << " Salvou o resultado no arquivo ->  " << nomeimgaux << endl;

            // Mostra resultado
            cout << ( string("display ") + nomeimgaux + " &") << endl;
            system ( ( string("display ") + nomeimgaux + " &").c_str() );
            delete pm;
            pm  = NULL;
            delete filtro;
            filtro = NULL;
            break;
        } //fim opcao1
        case 6:
        {
            cout << "Selecionou dilatacao." << endl;
            system("ls");
            cout << "Nome da imagem (nome.extensao): " << endl;
            cin >> nomeimg;
            cin.get();
            cout << "Tamanho do elemento estruturante: " << endl;
            cin >> eleStrut;
            cin.get(); // pega o enter
            pm = new TCMatriz2D< int >( nomeimg );
            cout << " Carregou a matriz do disco...." << endl;
            filtro = new CFEMorfologiaMatematica( pm , eleStrut );
            cout << " Criou o filtro...." << endl;
            if ( pm == NULL  || filtro == NULL)
            {
                cerr << "Falha alocação Matriz ou filtro.\n" ;
                exit(0);
            }
            filtro->Dilatacao(pm,eleStrut);
            cout << " Aplicou o filtro...." << endl;

            string nomeimgaux = "dilatada_" + nomeimg;
            pm->Write( nomeimgaux );
            //pm->Write( "dilatada.pbm" );
            cout << " Salvou o resultado no arquivo ->  " << nomeimgaux << endl;

            // Mostra resultado
            cout << ( string("display ") + nomeimgaux + " &") << endl;
            system ( ( string("display ") + nomeimgaux + " &").c_str() );
            delete pm;
            pm  = NULL;
            delete filtro;
            filtro = NULL;
            break;
        } //fim opcao2
        case 7:
        {
            cout << "Selecionou abertura." << endl;
            system("ls");
            cout << "Nome da imagem (nome.extensao): " << endl;
            cin >> nomeimg;
            cin.get();
            cout << "Tamanho do elemento estruturante: " << endl;
            cin >> eleStrut;
            cin.get(); // pega o enter
            pm = new TCMatriz2D< int >( nomeimg );
            cout << " Carregou a matriz do disco...." << endl;
            filtro = new CFEMorfologiaMatematica( pm , eleStrut );
            cout << " Criou o filtro...." << endl;
            if ( pm == NULL  || filtro == NULL)
            {
                cerr << "Falha alocação Matriz ou filtro.\n" ;
                exit(0);
            }
            filtro->Abertura(pm,eleStrut);
            cout << " Aplicou o filtro...." << endl;

            string nomeimgaux = "abertura_" + nomeimg ;
            pm->Write( nomeimgaux );
            cout << " Salvou o resultado no arquivo ->  " << nomeimgaux << endl;

            // Mostra resultado
            cout << ( string("display ") + nomeimgaux + " &") << endl;
            system ( ( string("display ") + nomeimgaux + " &").c_str() );
            delete pm;
            pm  = NULL;
            delete filtro;
            filtro = NULL;
            break;
        } //fim opcao3
        case 8:
        {
            cout << "Selecionou fechamento." << endl;
            system("ls");
            cout << "Nome da imagem (nome.extensao): " << endl;
            cin >> nomeimg;
            cin.get();
            cout << "Tamanho do elemento estruturante: " << endl;
            cin >> eleStrut;
            cin.get(); // pega o enter
            pm = new TCMatriz2D< int >( nomeimg );
            cout << " Carregou a matriz do disco...." << endl;
            filtro = new CFEMorfologiaMatematica( pm , eleStrut );
            cout << " Criou o filtro...." << endl;
            if ( pm == NULL  || filtro == NULL)
            {
                cerr << "Falha alocação Matriz ou filtro.\n" ;
                exit(0);
            }
            filtro->Fechamento(pm,eleStrut);
            cout << " Aplicou o filtro...." << endl;

            string nomeimgaux = "fechamento_" + nomeimg ;
            pm->Write( nomeimgaux );
            //pm->Write( "fechamento.pbm" );
            cout << " Salvou o resultado no arquivo ->  " << nomeimgaux << endl;

            // Mostra resultado
            cout << ( string("display ") + nomeimgaux + " &") << endl;
            system ( ( string("display ") + nomeimgaux + " &").c_str() );
            delete pm;
            pm  = NULL;
            delete filtro;
            filtro = NULL;
            break;
        } //fim opcao3

        case 9:
        {
            cout << "Selecionou calculo porosidade." << endl;
            system("ls");
            cout << "Nome da imagem (nome.extensao): " << endl;
            cin >> nomeimg;
            cin.get(); // pega o enter
            pm = new TCMatriz2D< int >(nomeimg);
            cout << "Carregou imagem->" << nomeimg << endl;

            carac = new CAberturaDilatacao(pm,nomeimg);
            cout << "Criou objeto CAberturaDilatacao." << endl;
            if ( pm == NULL  || carac == NULL)
            {
                cerr << "Falha alocação Matriz ou CAberturaDilatacao.\n" ;
                exit(0);
            }
            porosidade = carac->Porosidade(pm);
            cout << "Porosidade da imagem:." << nomeimg << " = " << porosidade << endl;
            delete pm;
            pm  = NULL;
            delete carac;
            carac = NULL;
            break;
        }//fim opcao5

        case 10:
        {
            cout << "Selecionou distribuicao de poros (método normal)." << endl;
            system("ls");
            cout << "Nome da imagem (nome.extensao): " << endl;
            cin >> nomeimg;
            cin.get(); // pega o enter
            pm = new TCMatriz2D< int >(nomeimg);
            cout << "Carregou imagem->" << nomeimg << endl;
            if ( pm == NULL )
            {
                cerr << "Falha alocação Matriz.\n" ;
                exit(0);
            }
            carac = new CAberturaDilatacao(pm,nomeimg);
            cout << "Criou objeto CAberturaDilatacao." << endl;
            carac->DistTotalPoros();
            cout << "Calculou carac->DistTotalPoros();." << endl;
            delete pm;
            pm  = NULL;
            delete carac;
            carac = NULL;
            break;
        }

        case 11:
        {
            cout << "Dimensao da imagem (impar): ";
            cin >> dimensao;
            cin.get(); // pega o enter
            bola = new CBCd34(dimensao);
            util = new CUtil();
            os << "imgSinteticaD34[" << dimensao << "]" << ".pgm";
            bola->Write(os.str());
            util->ArrumarImagem(os.str());
            delete bola;
            bola = NULL;
            delete util;
            util = NULL;
            break;
        }
        case 12:
        {
            cout << " Selecionou visualizar a imagem  " << endl;
            system("ls");
            cout << "Nome da imagem (nome.extensao): " << endl;
            cin >> nomeimg;
            cin.get(); // pega o enter
            pm = new TCMatriz2D< int >(nomeimg);
            if ( pm == NULL)
            {
                cerr << "Antes precisa carregar a imagem, executando opcões 1-4.\n";
                break;
            }

            string nomeimgaux = "ImagemVisualizada.pbm";
            pm->Write( nomeimgaux.c_str() );

            // Visualiza o resultado
            system ( ( string("display ") + nomeimgaux + "&").c_str() );

            break;
        }

        case 13:
        {
            cout << " Selecionou plotar grafico de distribuicao  " << endl;
            // vai plotar grafico com resultado
            //g2d.Title("distribuicao acumulada de poros"); // Titulo do grafico
            //g2d.XLabel("raio elemento estruturante");    // Rotulo eixo x
            //g2d.YLabel("porosidade acumulada");    // Rotulo eixo y
            g2d.XRange(0,30);             // Seta intervalo do eixo x.
            g2d.YRange(0,1);             // Seta intervalo do eixo x.
            system("ls");
            cout << "Nome da distribuicao a ser plotada: " << endl;
            cin >> nomeDistribuicao;
            cin.get();
            g2d.PlotFile ( nomeDistribuicao, 1, nomeDistribuicao);
            break;
        }

        case 14:
        {
            cout << " Selecionou Funcao em teste  " << endl;
            system("ls");
            cout << "Nome da imagem (nome.extensao): " << endl;
            cin >> nomeimg;
            cin.get(); // pega o enter
            cout << "Dimensao da borda: " << endl;
            cin >> borda;
            cin.get(); // pega o enter

            pm = new TCMatriz2D< int >(nomeimg, borda);
            if ( pm == NULL  )
            {
                cerr << "Falha alocação Matriz .\n" ;
                exit(0);
            }
            cout << "Carregou imagem->" << nomeimg
                 << " e estendeu a mesma considerando a borda->" << borda << endl;
            string nomeimgaux = "Borda_" + nomeimg;
            pm->Write( nomeimgaux.c_str() );
            cout << "Salvou a imagem estendida:" << nomeimgaux << endl;

            // Visualiza o resultado
            system ( ( string("display ") + nomeimg + "&").c_str() );

            break;
        }

        case 15:
        {
            cout << "Selecionou SequenciaAberturaTonsCinza." << endl;
            system("ls");
            cout << "Nome da imagem (nome.extensao): " << endl;
            cin >> nomeimg;
            cin.get(); // pega o enter
            pm = new TCMatriz2D< int >(nomeimg);
            cout << "Carregou imagem->" << nomeimg << endl;
            if ( pm == NULL )
            {
                cerr << "Falha alocação Matriz.\n" ;
                exit(0);
            }
            carac = new CAberturaDilatacao(pm,nomeimg);
            cout << "Criou objeto CAberturaDilatacao." << endl;
            carac->SequenciaAberturaTonsCinza();
            cout << "Calculou carac->SequenciaAberturaTonsCinza();." << endl;
            delete pm;
            pm  = NULL;
            delete carac;
            carac = NULL;
            break;
        } //fim opcao4


        case 16:
        {
            cout << "Selecionou calculo porosidade." << endl;
            system("ls");
            cout << "Nome da imagem (nome.extensao): " << endl;
            cin >> nomeimg;
            cin.get(); // pega o enter
            pm = new TCMatriz2D< int >( nomeimg );
            cout << "Carregou imagem->" << nomeimg << endl;
            if ( pm == NULL )
            {
                cerr << "Falha alocação Matriz.\n" ;
                exit(0);
            }
            carac = new CAberturaDilatacao( pm ,nomeimg );
            cout << "Criou objeto CAberturaDilatacao." << endl;
            cout << "Porosidade = " << carac->Porosidade(pm) << endl;
            delete pm;
            pm  = NULL;
            delete carac;
            carac = NULL;

            break;
        }
        case 17:
        {
            cout << "Selecionou setar flag salvar resultados parciais." << endl;
            cout << "Deseja salvar resultados parciais em disco (entre com 1 para sim e 0 para não):" << endl;
            cin >> salvarResultadosParciais;
            cin.get(); // pega o enter
            CAberturaDilatacao::SalvarResultadosParciais ( salvarResultadosParciais );
            break;
        }


        } //fim switch
    } while ( Opcao != SAIR );
}

