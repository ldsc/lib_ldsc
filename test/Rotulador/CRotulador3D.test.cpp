// g++ TRotulador3D.test.cpp -I/usr/local/include/lenep/
// -L/usr/local/lib/lenep -llenep -o Rotulador3D

#include <iostream>
#include <string>
using namespace std;

#include <Rotulador/CRotulador3D.h>
#include <Tempo/CTime.h>

/**
* Função de teste da classe rotulador 3D.
* @param argc
* @param argv[]
* @return
 */
int
main (int argc, char *argv[])
{
    cout <<

         cout  << "\n================================================================================"
               << "\n====================== Programa de teste da LIB_LDSC ==========================="
               << "\n================================================================================" << endl;

    cout  << "\n================================================================================"
          << "\nUSO:"
          << "\n./CRotulador3D.test"
          << "\n================================================================================" << endl;

// Cria string
    string fileName = "imagem-[P262_K70-2-45-50-lp-d6].pm3";

    if (argc > 1)
        fileName = argv[1];

    cout << "Carrega imagem " << fileName << endl;
// Cria rotulador
    CRotulador3D rot (fileName);

    cout << "Realizando rotulagem... ";
    {
// Mostra o tempo de processamentp da função no dispositivo cout
        CTime(string("Processando função rot.Go()"), &cout);

        // Executa rotulagem
        rot.Go (&rot);
    }

    cout << " ...ok;" << endl;

    string fileOut = "out.CRotulador3D." + fileName;

    rot.larguraCampo = 3;

    // Salva imagem rotulada em disco
    rot.Write (fileOut);

    cout << "Salvou imagem rotulada em disco com nome " << fileOut << endl;

    return 0;

}
