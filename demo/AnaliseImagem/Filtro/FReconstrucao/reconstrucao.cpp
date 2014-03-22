#include <iostream>
#include <string>

#include <AnaliseImagem/Filtro/FReconstrucao/CFReconstrucao.h>
#include <MetNum/Matriz/CMatriz3D.h>

using namespace std;

int main (int argc, char *argv[])
{
    cout <<
         "\n================================================================================\
     \nUSO:\
     \n./reconstrucao nomeImagem\
     \nou\
     \n./reconstrucao \
     \n================================================================================"
         << endl;
    string nomeArquivo;
    if (argc==2) {
        nomeArquivo = argv[1];
    } else {
        cout << "Entre com o nome da imagem a ser reconstruida (*.pbm):";
        //system("ls *.pbm");
        getline(cin,nomeArquivo);
    }
    cout << "Criando imagem a ser filtrada.\n";
    CFReconstrucao * pr = new CFReconstrucao( );
    CMatriz3D * pm = NULL;
    pm = pr->Go(nomeArquivo, E_EIXO_X);
    if (pm)
        pm->Write("resReconstrucao.dbm");

    delete pr;
    return 0;
}
