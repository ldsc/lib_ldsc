#include <Matriz/TCMatriz3D.h>
#include <Filtro/FEspacial3D/FEMorfologiaMatematica3D/TCFEMMIDF3D.h>

int main(int argc, char *argv[]) {
	TCMatriz3D<bool> *pm = NULL;
	TCFEMMIDFd3453D<bool> *pidf = NULL;
	pm = new TCMatriz3D<bool>("../../data/imagens/IBS0105_100kv_3.9um_cam4K_0.25d_3602_2f_0.5Al_3200ms_8x100_BIN_1-0_conected.dbm");

	pidf = new TCFEMMIDFd3453D<bool>( pm );
	pidf->Go(pm,3);
	pidf->Abertura(pm,3);

	//salva a mascara utilizada na abertura
	if (pidf->mask) {
		pidf->mask->SetFormato(D2_X_Y_Z_GRAY_ASCII);
		pidf->mask->NumCores(pidf->mask->MaiorValor());
		pidf->mask->Path("./");
		pidf->mask->Write("mascara.dgm");
	}

	//salva a idf da imagem
	pidf->SetFormato(D2_X_Y_Z_GRAY_ASCII);
	pidf->NumCores(pidf->MaiorValor());
	pidf->Path("./");
	pidf->Write("idf.dgm");
	int result = system("../../../lvp/bin/lvp mascara.dgm idf.dgm");

	if(pm)
		delete pm;
	if(pidf)
		delete pidf;

	return result;
}

