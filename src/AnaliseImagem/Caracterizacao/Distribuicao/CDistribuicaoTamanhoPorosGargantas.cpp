#include "CDistribuicaoTamanhoPorosGargantas.h"

//Construtor matriz binária
CDistribuicaoTamanhoPorosGargantas::CDistribuicaoTamanhoPorosGargantas(TCImagem3D<bool> *&_pm, int & _raioMaximo, int & _raioDilatacao, int & _fatorReducao, int & _incrementoRaio, EModelo _modelo, int _indice, int _fundo )
	: modelo(_modelo), indice(_indice), fundo(_fundo), pms(nullptr), pml(nullptr)
{
	filtro = new CAberturaDilatacao3D( _pm, indice, fundo );
	filtro->IncrementoRaioElementoEstruturante(_incrementoRaio);
	filtro->RaioMaximoElementoEstruturante(_raioMaximo);
	filtro->FatorReducaoRaioElemEst(_fatorReducao);
	filtro->RaioEEDilatacao(_raioDilatacao);
}

//Construtor imagem tons de cinza
CDistribuicaoTamanhoPorosGargantas::CDistribuicaoTamanhoPorosGargantas(TCImagem3D<int> *&_pm )
	: indice(1), fundo(0), filtro(nullptr)
{
	pms = nullptr;
	pml = nullptr;
	if (_pm) {
		if (_pm->NumCores() == 3) { // tem que ser uma imagem com 0 (fundo), 1 (sítio) e 2 (ligação)
			int nx = _pm->NX();
			int ny = _pm->NY();
			int nz = _pm->NZ();
			int i,j,k;
			pms = new TCMatriz3D<bool>(nx,ny,nz);
			pml = new TCMatriz3D<bool>(nx,ny,nz);
#pragma omp parallel for collapse(3) default(shared) private(i,j,k) //schedule(dynamic,10)
			for ( i=0; i<nx; ++i ) {
				for ( j=0; j<ny; ++j ) {
					for ( k=0; k<nz; ++k ) {
						if (_pm->data3D[i][j][k]==1) {
							pms->data3D[i][j][k] = 1;
							pml->data3D[i][j][k] = 0;
						} else if (_pm->data3D[i][j][k]==2) {
							pml->data3D[i][j][k] = 1;
							pms->data3D[i][j][k] = 0;
						} else {
							pms->data3D[i][j][k] = 0;
							pml->data3D[i][j][k] = 0;
						}
					}
				}
			}
		}
	}
}

//Destrutor
CDistribuicaoTamanhoPorosGargantas::~CDistribuicaoTamanhoPorosGargantas(){
	if (filtro)
		delete filtro;
	if (pms)
		delete pms;
	if (pml)
		delete pml;
}

// Go: Executa o cálculo das distribuições
std::pair< CDistribuicao3D*, CDistribuicao3D* > CDistribuicaoTamanhoPorosGargantas::Go(CDistribuicao3D::Metrica3D _metrica) {
	CDistribuicao3D *distP = nullptr; //<Ponteiro para objeto da classe que calcula a distribuição de tamanho de poros.
	CDistribuicao3D *distG = nullptr; //<Ponteiro para objeto da classe que calcula a distribuição de tamanho de gargantas.
	bool okP = false;
	bool okG = false;
	if (filtro != nullptr) {
		filtro->Go(modelo);
		distP = new CDistribuicao3D ( filtro->GetMatrizSitios() );
		distG = new CDistribuicao3D ( filtro->GetMatrizLigacoes() );
	} else if ( pms!=nullptr && pml!=nullptr ) {
		distP = new CDistribuicao3D ( pms );
		distG = new CDistribuicao3D ( pml );
	}
	if (distP != nullptr && distG != nullptr) {
#pragma omp parallel
#pragma omp sections
		{
#pragma omp section
			okP = distP->Go( CBaseDistribuicao::dtp, _metrica, indice, fundo );
#pragma omp section
			okG = distG->Go( CBaseDistribuicao::dtg, _metrica, indice, fundo );
		}
	}
	if (okP && okG) {
		return std::make_pair(distP, distG);
	} else {
		if (distP)
			delete distP;
		if (distG)
			delete distG;
		distP = nullptr;
		distG = nullptr;
		return std::make_pair(distP, distG);
	}
}
