#include <AnaliseImagem/Caracterizacao/RedePercolacao/CContornoRedePercolacao.h>

#include <cstdio>
#include <cassert>
#include <iomanip>

//#include <AnaliseImagem/Caracterizacao/RedePercolacao/COGSitio.h>
//#include <AnaliseImagem/Caracterizacao/RedePercolacao/COGSitioEsquerda.h>
//#include <AnaliseImagem/Caracterizacao/RedePercolacao/COGSitioDireita.h>

// Cria os objetos de contorno e incluí no vetor contorno (recebe imagem binária que será segmentada).
CContornoRedePercolacao::CContornoRedePercolacao (unsigned int nx, unsigned int ny, unsigned int nz, unsigned long int _numero_contornos)
	: CRedeDePercolacao(nx, ny, nz)//, CRedeContorno("nomeArquivoIndefinido", _numero_contornos)
{
	for (unsigned long int i = 0; i < _numero_contornos; ++i) {
		CContornoCentro *obj_cont = new CContornoCentro ();
		assert (obj_cont);
		contorno.push_back (obj_cont);
	}
}

// Destrutor - Deleta 0s objetos contorno criados por esta classe.
CContornoRedePercolacao::~CContornoRedePercolacao () {
	for ( auto contorno_i : contorno )
		if ( contorno_i )
			delete contorno_i;
}

// operator<<
// Salva dados do objeto em "os" (disco);
// Salva o número de objetos que fazem parte de grafo e a seguir os dados de cada objeto.
ostream & operator<< (ostream & os, const CContornoRedePercolacao & rede) {
	rede.Write(os);
	return os;
}
