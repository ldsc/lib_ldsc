#include <AnaliseImagem/Caracterizacao/RedePercolacao/CContornoRedePercolacao.h>

#include <cstdio>
#include <cassert>
#include <iomanip>

//#include <AnaliseImagem/Caracterizacao/RedePercolacao/COGSitio.h>
//#include <AnaliseImagem/Caracterizacao/RedePercolacao/COGSitioEsquerda.h>
//#include <AnaliseImagem/Caracterizacao/RedePercolacao/COGSitioDireita.h>

// Cria os objetos de contorno e incluí no vetor contorno (recebe imagem binária que será segmentada).
CContornoRedePercolacao::CContornoRedePercolacao (unsigned int nx, unsigned int ny, unsigned int nz, unsigned long int _numero_contornos)
	: CRedeDePercolacao(nx, ny, nz) {
	for (unsigned long int i = 0; i < _numero_contornos; ++i) {
		CContornoCentro *obj_cont = new CContornoCentro ();
		assert (obj_cont);
		contorno.push_back (obj_cont);
	}
}

// Destrutor - Delta 0s objetos contorno criados por esta classe.
CContornoRedePercolacao::~CContornoRedePercolacao () {
	for ( auto contorno_i : contorno )
		if (contorno_i)
			delete contorno_i;
}

// operator<< Salva dados do objeto em "os" (disco);
// Salva o número de objetos que fazem parte de grafo e a seguir os dados de cada objeto.
ostream & operator<< (ostream & os, const CContornoRedePercolacao & obj) {
	os.setf ( ios::left );
	// Tipo de grafo
	//out << setw ( 5 ) << static_cast<uint16_t> ( Tipo() ) ;
	os << setw ( 5 ) << 105 ;
	// Numero de objetos
	os << ' '<< setw ( 5 ) << obj.ptrMatObjsRede->matrizObjetos.size() << '\n';
	// Percorre os objetos e salva em disco as informações de cada objeto.
	//for ( auto objeto_i : obj.ptrMatObjsRede->matrizObjetos ) {
	//	objeto_i->Write ( os );
	//	os << '\n';
	//}
	return os;
}
