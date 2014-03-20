#include <AnaliseImagem/Caracterizacao/RedePercolacao/CRedeContorno.h>

#include <cstdio>
#include <cassert>
#include <iomanip>

//#include <AnaliseImagem/Caracterizacao/RedePercolacao/COGSitio.h>
//#include <AnaliseImagem/Caracterizacao/RedePercolacao/COGSitioEsquerda.h>
//#include <AnaliseImagem/Caracterizacao/RedePercolacao/COGSitioDireita.h>

// Cria os objetos de contorno e incluí no vetor contorno (recebe imagem binária que será segmentada).
CRedeContorno::CRedeContorno (unsigned int nx, unsigned int ny, unsigned int nz, unsigned long int _numero_contornos)
	: CRedeDePercolacao(nx, ny, nz) {
	for (unsigned long int i = 0; i < _numero_contornos; ++i) {
		CContorno *obj_cont = new CContorno ();
		assert (obj_cont);
		contorno.push_back (obj_cont);
	}
}

// Destrutor - Delta 0s objetos contorno criados por esta classe.
CRedeContorno::~CRedeContorno () {
	for (unsigned long int i = 0; i < contorno.size (); ++i)
		if (contorno[i])
			delete contorno[i];
}

// operator<<
// Salva dados do objeto em "os" (disco);
// Salva o número de objetos que fazem parte de grafo e a seguir os dados de cada objeto.
ostream & operator<< (ostream & os, const CRedeContorno & grafoC) {
	// Numero de objetos
	//os << setw (5) << grafoC.matrizObjetos.size () << endl;
	// Percorre os objetos e salva em disco as informações de cada objeto
	//for (unsigned long int i = 0; i < grafoC.matrizObjetos.size (); i++)
		//grafoC.matrizObjetos[i].Write (os);
	return os;
}
