/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
			[LDSC].
@author:          André Duarte Bueno
File:             CImagem3D.cpp
begin:            Sat Sep 16 2000
copyright:        (C) 2000 by André Duarte Bueno
email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>
#include <algorithm>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef CImagem3D_h
#include <Matriz/TCImagem3D.h>
#endif

template< typename T >
void TCImagem3D<T>::SalvaInformacoesRecontrucao (ofstream & fout) const {
    if (fout) {
        fout << setw (0) << "\n# fatorAmplificacao: " << fatorAmplificacao << "\n";
        fout << setw (0) << "# sizePixel: " 	    << sizePixel 	     << "\n";
				fout << setw (0) << "# numeroPixelsBorda: " << numeroPixelsBorda;
    }
}

template< typename T >
bool TCImagem3D<T>::LeInformacoesRecontrucao (ifstream & fin) {
    if (fin.good()) {
        int posInicial = fin.tellg();		//guarda a posição de leitura no arquivo.
        char linha[256];
        string aux;
        fin.seekg(0, ios::beg);			//posiciona o ponteiro de leitura no começo do arquivo
        do {
            fin >> aux;
            //cerr << fin.tellg() < endl;
        } while ( ( aux != "#" ) and ( fin.tellg() < 128 ) ); //
        bool encontrou = false;
        if (aux == "#") {
            do {
                fin >> aux;
                if (aux == "fatorAmplificacao:") {
                    fin >> fatorAmplificacao;
                    //cerr << "\nfatorAmplificacao=" << fatorAmplificacao << endl;
                    encontrou = true;
                } else if (aux == "sizePixel:") {
                    fin >> sizePixel;
                    encontrou = true;
                    //cerr << "\nsizePixel=" << sizePixel << endl;
                } else if (aux == "numeroPixelsBorda:") {
                    fin >> numeroPixelsBorda;
                    //cerr << "\nnumeroPixelsBorda=" << numeroPixelsBorda << endl;
                    encontrou = true;
                }
                if ( encontrou ) {
                    fin >> aux;
                } else {
                    fin.getline(linha, 256);
                    fin >> aux;
                }
            } while ( aux == "#" );
        }
        fin.seekg(posInicial, ios::beg);	//reposiciona o ponteiro de leitura para a posição inicial;
        return encontrou;
    }
    return false;
}

template< typename T >
bool TCImagem3D<T>::LeInformacoesRecontrucao (string fileName) {
    ifstream fin;									// Ponteiro para arquivo de disco
    CBaseMatriz::AbreArquivo (fin, fileName);			// Abre o arquivo de disco no formato correto
    if (fin.good ()) {								// Se o arquivo foi corretamente aberto
        return LeInformacoesRecontrucao (fin);
    }
    return false;
}

/*
-------------------------------------------------------------------------
Funcao:
-------------------------------------------------------------------------
@short  : Salva dados do cabecalho
@author : Andre Duarte Bueno, modificada por: Leandro Puerari
@see    :
@param  :
@return :
*/
template< typename T >
void TCImagem3D<T>::SalvaCabecalho (ofstream & fout) const {
	TCMatriz3D<T>::SalvaCabecalho (fout);
	SalvaInformacoesRecontrucao(fout);
}

