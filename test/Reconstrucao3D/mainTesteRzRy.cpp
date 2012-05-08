#include    <iostream>
using namespace std;

//-----------------------------------------------------------------------------
//Biblioteca do LDSC (Lib_LDSC)
//-----------------------------------------------------------------------------
#include    "TesteRzRy.h"

//-----------------------------------------------------------------------------
//Funcao main
//-----------------------------------------------------------------------------
int main( int argc, char **argv )
{
    cout  << "\n================================================================================"
          << "\n====================== Programa de teste da LIB_LDSC ==========================="
          << "\n================================================================================" << endl;

    cout  << "\n================================================================================"
          << "\nUSO:"
          << "\n./ctesteRzRy"
          << "\n================================================================================" << endl;

    TesteRzRy teste;
    teste.Go();
    return 0;
}



