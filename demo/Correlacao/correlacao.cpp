/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: Programa de teste da LIB_LDSC
  ===============================================================================

  Desenvolvido por:
  Laboratorio de Desenvolvimento de Software Cientifico
  [LDSC].
  @author     Liang Zhirong, Andre Duarte Bueno
  @file       main.cpp
  @begin      Sat Sep 16 2000
  @copyright  (C) 2000 by Andre Duarte Bueno
  @email      andre@lmpt.ufsc.br

  Tarefa:
  Eliminar referencias a formato antigo, carregar CMatriz2D e aplicar correlacao,
  visualizar correlacao usando CGnuplot.
*/

// ----------------------------------------------------------------------
// Bibliotecas C/C++
// ----------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cassert>
using namespace std;

// ----------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------
// Para teste da correlacao fft
#include <Correlacao/CCorrelacaoFFT.h>

// Defincoes globais
#define REAL float
#define Real_E1(x,y)	Re_data [(y)*NX + (x)]
#define Imag_E1(x,y)	Im_data [(y)*NX + (x)]
#define z(x,y)	        z_data  [(y)*NX + (x)]

char InputFile[200] = "320220.dat";

FILE *fpin, *fpout, *fpout2;

// int NX = 609,NY = 458;
int NX = 620,NY = 460;

static REAL *Re_data = NULL;

static REAL *Im_data = NULL;

void   Read2D();

// Funcao main da LIB_LDSC
int main( int argc, char *argv[] )
{

    cout  << "\n================================================================================"
          << "\n====================== Programa de teste da LIB_LDSC ==========================="
          << "\n================================================================================" << endl;

    cout  << "\n================================================================================"
          << "\nUSO:"
          << "\n./correlacao"
          << "\n================================================================================" << endl;


    CCorrelacaoFFT *correlacao = new CCorrelacaoFFT ();

    assert ( correlacao ); // assert é comando de C que verifica alocacao e envia mensagens.
    if (argc == 1)
    {
        //Re_data = (REAL*) calloc ( NX*NY, sizeof(REAL) );
        Re_data = new REAL [ NX*NY ] ;

        if ( Re_data == NULL )
        {
            cerr<< "Unable to allocate memory Real_E1 for data storage.\n"<< endl;
            return 1;
        }
        Read2D(); // Carrega os dados do disco

        cerr << "Usando correlacao->Go ( Re_data, NX, NY);"<<endl;
        correlacao->Go ( Re_data, NX, NY); // Executa funcao autocorrelacao
//      delete [] Re_data;//2009
        correlacao->Writerzf ("resultado_rzf");
        correlacao->Write("resultado_correlacao");
    }
    else
    {
        cerr << "Usando correlacao->GoMain ( argc,argv);"<<endl;
        correlacao->GoMain (argc, argv);  			// Executa funcao autocorrelacao
        correlacao->Writerzf ("resultado_rzf");
        correlacao->Write("resultado_correlacao");

    }
    delete correlacao;

    return EXIT_SUCCESS;
}

/*
  int CCorrelacaoFFT::GoMain (int argc, char **argv);
  virtual int Go (float *_Re_data, int _nx, int _ny);
*/
void Read2D()
{
    int i,j;
    long int   cont=0,cont2;
    int  aux;
    float por;

    printf("Reading data... ...");
    if ((fpin=fopen(InputFile,"r"))==NULL)
    {
        puts("Unable to open file.\n");
        exit(1);
    }

    for (j=0;j<(NY);j++)
    {
        for (i=0;i<(NX);i++)
        {
            fscanf(fpin,"%d",&aux);
            Real_E1(i,j)=aux;

            if (aux == 1) // cálculo porosidade
                cont++;
        }
        fscanf(fpin,"\n");
    }
    fclose(fpin);

    por=(float)cont;
    por/=(float)(NX*NY);
    printf("Porosity=%f",por);
    printf ("... ... done.\n");
}
