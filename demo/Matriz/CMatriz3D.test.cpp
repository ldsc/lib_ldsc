// g++ teste_lib_lmpt-TMatriz.cpp -I/usr/local/include/lib_lmpt/ -L/usr/local/lib/lib_lmpt -lTMatriz

#include <iostream>

#include <string>

#include <AnaliseImagem/Matriz/TCMatriz3D.h>

using namespace std;

/**
 * Função de teste da classe CMatriz3D.
 * @param argc
 * @param argv[]
 * @return
 */
int main (int argc, char *argv[]) {
    cout  << "\n================================================================================"
          << "\n====================== Programa de teste da LIB_LDSC ==========================="
          << "\n================================================================================" << endl;

    cout  << "\n================================================================================"
          << "\nUSO:"
					<< "\n./matriz3D"
          << "\n================================================================================" << endl;

    // Cria string com nome da imagem
    string nomeArquivo = "imagem-[P262_K70-2-45-50-lp-d6].pm3";

    if (argc > 1)
        nomeArquivo = argv[1];

    cout 	<< "\nSaída gerada pelo programa de demo: \n"
          << "Carrega imagem " << nomeArquivo << " do disco" << endl;

    // Cria imagem
		TCMatriz3D<int> *pm3 = new TCMatriz3D<int> (nomeArquivo);

    // Cria nova string com nome do arquivo de saída
    string fileOut = "out." + nomeArquivo;

    cout << "Salva imagem com nome " << fileOut << endl;

    // Salva imagem no disco
    pm3->Write (fileOut, 0);

    // Destroe a imagem
    delete pm3;
    return 0;
}

