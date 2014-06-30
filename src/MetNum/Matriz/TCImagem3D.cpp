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
#include <MetNum/Matriz/TCImagem3D.h>
#endif

template< typename T >
void TCImagem3D<T>::SalvaInformacoesRecontrucao (ofstream & fout) const {
	if (fout) {
		fout << setw (0) << "\n# fatorAmplificacao: " << fatorAmplificacao << "\n";
		fout << setw (0) << "# dimensaoPixel: " 	    << dimensaoPixel 	     << "\n";
		fout << setw (0) << "# numeroPixeisBorda: " << numeroPixeisBorda;
	}
}

template< typename T >
bool TCImagem3D<T>::LeInformacoesRecontrucao (ifstream & fin) {
	fatorAmplificacao = dimensaoPixel = numeroPixeisBorda = 0;
	if (fin.good()) {
		int posInicial = fin.tellg();		//guarda a posição de leitura no arquivo.
		char linha[256];
		string aux;
		fin.seekg(0, ios::beg);			//posiciona o ponteiro de leitura no começo do arquivo
		do {
			fin >> aux;
		} while ( ( aux != "#" ) and ( fin.tellg() > 0 ) and ( fin.tellg() < 64 ) ); //
		bool encontrou = false;
		if (aux == "#") {
			do {
				fin >> aux;
				if (aux == "fatorAmplificacao:") {
					fin >> fatorAmplificacao;
					//cerr << "\nfatorAmplificacao=" << fatorAmplificacao << endl;
					encontrou = true;
				} else if (aux == "dimensaoPixel:") {
					fin >> dimensaoPixel;
					encontrou = true;
					//cerr << "\ndimensaoPixel=" << dimensaoPixel << endl;
				} else if (aux == "numeroPixeisBorda:") {
					fin >> numeroPixeisBorda;
					//cerr << "\nnumeroPixeisBorda=" << numeroPixeisBorda << endl;
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
bool TCImagem3D<T>::LeInformacoesRecontrucao (string nomeArquivo) {
	ifstream fin;									// Ponteiro para arquivo de disco
	CBaseMatriz::AbreArquivo (fin, nomeArquivo);			// Abre o arquivo de disco no formato correto
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
 
@param  :
@return :
*/
template< typename T >
void TCImagem3D<T>::SalvaCabecalho (ofstream & fout) const {
	TCMatriz3D<T>::SalvaCabecalho (fout);
	SalvaInformacoesRecontrucao(fout);
}

// Cria e retorna uma nova matriz 3D que será um recorte da matriz (this).
template< typename T >
TCImagem3D<T>* TCImagem3D<T>::Crop (int startX, int endX, int startY, int endY, int startZ, int endZ) {
	if (startX >= endX || startY >= endY || startZ >= endZ) {
		cerr << "O valor inicial deve ser menor que o falor final" << endl;
		return NULL;
	}

	if (endX > this->nx || endY > this->ny || endZ > this->nz){
		cerr << "O valor final deve ser menor que o tamanho da matriz original (this)" << endl;
		return NULL;
	}

	int _nx = endX-startX+1;
	int _ny = endY-startY+1;
	int _nz = endZ-startZ+1;

	TCImagem3D<T> * pmtmp = NULL;
	pmtmp = new TCImagem3D<T>( _nx, _ny, _nz );
	if( ! pmtmp )
		return NULL;
	pmtmp->SetFormato(this->GetFormato());
	pmtmp->NumCores(this->NumCores());
	pmtmp->dimensaoPixel = this->DimensaoPixel();
	pmtmp->fatorAmplificacao = this->FatorAmplificacao();
	pmtmp->numeroPixeisBorda = 0;
	int i,j,k;
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
	for (i = 0; i < _nx; i++) {
		for (j = 0; j < _ny; j++) {
			for (k = 0; k < _nz; k++) {
				pmtmp->data3D[i][j][k] = this->data3D[i+startX][j+startY][k+startZ];
			}
		}
	}
	return pmtmp;
}

