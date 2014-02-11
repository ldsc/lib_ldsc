#include <AnaliseImagem/Matriz/TCMatriz3D.h>
#include <AnaliseImagem/Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIDF3D.h>

int main(int argc, char *argv[]) {
	TCMatriz3D<bool> *pm = NULL;
	TCFEMMIDFd3453D<bool> *pidf = NULL;
	pm = new TCMatriz3D<bool>("../../data/images/IBS0105-50x50x50.dbm");

	pidf = new TCFEMMIDFd3453D<bool>( pm );
	pidf->Go(pm,3);
	pidf->Abertura(pm,3);

	//salva a mascara utilizada na abertura
	if (pidf->mask) {
		pidf->mask->SetFormato(D2_X_Y_Z_GRAY_ASCII);
		pidf->mask->NumCores(pidf->mask->MaiorValor());
		pidf->mask->Path("../../data/images/");
		pidf->mask->Write("mascara.dgm");
	}

	//salva a idf da imagem
	pidf->SetFormato(D2_X_Y_Z_GRAY_ASCII);
	pidf->NumCores(pidf->MaiorValor());
	pidf->Path("../../data/images/");
	pidf->Write("idf.dgm");
	int result = system("../../../lvp/bin/lvp ../../data/images/mascara.dgm ../../data/images/idf.dgm");

	if(pm)
		delete pm;
	if(pidf)
		delete pidf;

	return result;
}

