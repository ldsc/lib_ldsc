#include <Distribuicao/CDistribuicaoIDF.h>

#ifndef CFEMMIDFd34_h
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFd34.h>
#endif

bool CDistribuicaoIDF::Go ( string fname, int indice, int fundo ) {
    if ( ! pm ) { // se o ponteiro para a matriz não estiver definido rotorna false
        cerr << "Erro em CDistribuicaoIDF::Go! Ponteiro para CMatriz2D não definido!" << endl;
        return false;
    }

    ofstream fout ( fname.c_str() );
    if ( fout.is_open() && fout.good() ) {
        CFEMMIDFd34 idf ( pm, indice, fundo ); // Cria filtro IDFd34
        idf.Go ( pm );
        int val;
        val = idf.MaiorValor();
        if ( ( val % 3 ) == 0 ) {
            val = ( val / 3 ) - 1;
        } else {
            val = int ( val / 3 );
        }
        raios.resize(val+1, 0); // redimensiona o vetor raios e preenche com zeros.
        for (int i = 0; i < raios.size(); i++) { //só pra garantir...
            raios[i] = 0;
        }
        for ( int i=0; i < idf.NX(); i++ ) {
            for ( int j=0; j < idf.NY(); j++ ) {
                val = idf.data2D[i][j];
                if ( val > 3 ) { //o raio será >= 1
                    if ( ( val % 3 ) == 0 ) {
                        val = ( val / 3 ) - 1;
                    } else {
                        val = int ( val / 3 );
                    }
                    raios[val]++;
                }
            }
        }
        for ( int i = 1; i < raios.size(); i++ ) {
            //cout << i << ": "<< raios[i] << endl;
            fout << i << '\t' << ( double(raios[i]) / double(idf.NX() * idf.NY()) ) * 100 << endl;
        }
    } else {
        cerr << "Erro em CDistribuicaoIDF::Go! O arquivo de saída não está aberto!" << endl;
        return false;
    }
    fout.close();
    return true;
}
