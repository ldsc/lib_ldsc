#include <Segmentacao/3D/PorosGargantas/CPorosGargantas3D.h>
#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDFd3453D.h>
#include <iomanip>
#include <cmath>
#include <omp.h>

using namespace std;

bool CPorosGargantas3D::salvarResultadosParciais = 0;

CPorosGargantas3D::CPorosGargantas3D( TCMatriz3D<bool>* &matriz , std::string _nomeImagem, int _indice, int _fundo)
	: pm(matriz), // pm é ponteiro para imagem externa (se mudar externamente teremos problemas).
		nomeImagem(_nomeImagem),
		INDICE(_indice), FUNDO(_fundo)
{
	pmRotulo = new TCRotulador3D<bool>( matriz, INDICE, FUNDO );
	pfidf = new TCFEMMIDFd3453D<bool>( matriz, INDICE, FUNDO );
}

CPorosGargantas3D::~CPorosGargantas3D() {
	delete pmRotulo;
	delete pfidf;
}

// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz bool informada como parametro.
void CPorosGargantas3D::SalvarResultadosParciaisEmDisco(TCMatriz3D<bool>* &mat, string fileName){
	if (salvarResultadosParciais) {
		cout << "-->Salvando imagem " << fileName.c_str() << endl ;
		mat->Write(fileName);
	}
}

// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz int informada como parametro.
void CPorosGargantas3D::SalvarResultadosParciaisEmDisco(TCMatriz3D<int>* &mat, string fileName){
	if (salvarResultadosParciais) {
		cout << "-->Salvando imagem " << fileName.c_str() << endl ;
		mat->Write(fileName);
	}
}

// Analisa a flag salvarResultadosParciais e caso esta seja verdadeira, salva em disco a matriz rotulo informada como parametro.
void CPorosGargantas3D::SalvarResultadosParciaisEmDisco(TCRotulador3D<bool>* &mat, string fileName){
	if (salvarResultadosParciais) {
		cout << "-->Salvando imagem " << fileName.c_str() << endl ;
		mat->Write(fileName);
	}
}

