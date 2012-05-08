// g++ exemplo  -I/usr/local/include/lib_ldsc/ -L/usr/local/lib/lib_ldsc -lMatriz
#include <iostream>
//#include <string>

#include <Matriz/CVetor.h>

using namespace std;

/**
 * Exemplo simples de uso da biblioteca
 * Cria vetor, armazena valores e entÃ£o salva em disco
 * @param argc
 * @param argv[]
 * @return
 */
int
main (int argc, char *argv[])
{
    cout <<
         "\n================================================================================\
			\n====================== Programa de teste da LIB_LDSC ===========================\
			\n================================================================================" << endl;

    CVetor *v = new CVetor (10);

    for ( int i=0; i< v->NX(); i++)
        v->data1D[i] = i*3;

    v->Write ("vetorGerado.dat");	// salva vetor no disco
    cout << "Gerado o arquivo vetorGerado.dat.\n";

    for ( int i=0; i< v->NX(); i++)
        v->data1D[i] = (i%2 == 0) ? 0 : 1 ;

    v->Write ("vetorGerado-01.dat",0);	// salva vetor no disco
    cout << "Gerado o arquivo vetorGerado-01.dat.\n";

    delete v;
    return 0;

}
