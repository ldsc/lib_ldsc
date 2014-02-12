#include "CRedeDePercolacao.h"
#include <map>

//std::vector<int> CRedeDePercolacao::numPixeisBola = { //só funciona no c++11
int CRedeDePercolacao::numPixeisBola[] = {
	0,7,33,123,269,499,853,1335,1977,2783,3793,5035,6501,8235,10253,12583,15241,18231,21609,25379,29557,
	34171,39237,44799,50849,57415,64537,72219,80493,89363,98869,109039,119865,131391,143633,156619,170365,
	184875,200205,216359,233353,251215,269961,289635,310229,331771,354301,377823,402369,427943,454585,
	482323,511149,541107,572213,604495,637969,672639,708561,745739,784189,823939,865005,907431,951209,
	996367,1042945,1090947,1140405,1191323,1243741,1297687,1353153,1410183,1468793,1529011,1590853,1654323,
	1719477,1786319,1854865,1925143,1997169,2070987,2146589,2224003,2303269,2384391,2467401,2552303,2639137,
	2727931,2818677,2911419,3006173,3102967,3201817,3302727,3405753,3510899,3618181,3727627,3839253,3953103,
	4069169,4187479,4308073,4430955,4556157,4683683,4813573,4945855,5080521,5217615,5357153,5499163,5643661,
	5790651,5940189,6092279,6246937,6404191,6564057,6726579,6891749,7059595,7230157,7403439,7579473,7758263,
	7939849,8124259,8311485,8501571,8694533,8890399,9089185,9290895,9495585,9703259,9913933,10127635,10344381,
	10564215,10787129,11013151,11242321,11474643,11710149,11948843,12190765,12435943,12684369,12936087,
	13191113,13449475,13711189,13976259,14244741,14516639,14791969,15070759,15353025,15638811,15928109,
	16220947,16517365,16817367,17120985,17428223,17739121,18053707,18371973,18693963,19019693,19349191,
	19682473,20019543,20360457,20705219,21053845,21406363,21762789,22123167,22487489,22855783,23228089,
	23604411,23984781,24369203,24757717,25150351,25547097,25947999,26353073,26762347,27175837,27593547,
	28015533,28441799,28872361,29307247,29746473,30190083,30638069,31090459,31547293,32008575,32474337,
	32944583,33419353,33898675,34382541,34870995,35364053,35861743,36364081,36871071,37382769,37899179,
	38420317,38946211,39476877,40012359,40552649,41097775,41647777,42202659,42762453,43327163,43896829,
	44471479,45051105,45635751,46225433,46820179,47420005,48024915,48634965,49250159,49870513,50496055,
	51126801,51762795,52404029,53050531,53702341,54359463,55021929,55689743,56362945,57041563,57725589,
	58415067,59110013,59810455,60516409,61227879,61944921,62667539,63395749,64129579,64869045,65614191,
	66365009,67121527,67883785,68651787,69425565,70205123,70990501,71781727,72578793,73381743,74190593,
	75005371,75826093,76652763,77485437,78324119,79168825,80019583,80876409,81739347,82608389,83483563,
	84364909,85252431,86146161,87046103,87952297,88864771,89783517,90708579,91639973,92577727,93521857,
	94472367,95429313,96392699,97362541
};

//NumElements retorna o tamanho do vetor
#define NumElements(x) (sizeof(x) / sizeof(x[0]))

// Construtor matriz binária
CRedeDePercolacao::CRedeDePercolacao( TCImagem3D<bool> *&_pm, int _raioMaximo, int _raioDilatacao, int _fatorReducao, int _incrementoRaio, EModelo _modelo, int _indice, int _fundo )
	: CDistribuicaoTamanhoPorosGargantas( _pm, _raioMaximo, _raioDilatacao, _fatorReducao, _incrementoRaio, _modelo, _indice, _fundo ),
		CMatrizObjetoImagem(), pm(NULL)
{
	srand (time(NULL)); //inicia seed randômica;
}

// Construtor imagem tons de cinza
CRedeDePercolacao::CRedeDePercolacao( TCImagem3D<int> *&_pm )
	: CDistribuicaoTamanhoPorosGargantas( _pm ),
		CMatrizObjetoImagem(), pm (NULL)
{
	srand (time(NULL)); //inicia seed randômica;
}

// Destrutor
CRedeDePercolacao::~CRedeDePercolacao(){
	if (dtpg.first)
		delete dtpg.first;
	if (dtpg.second)
		delete dtpg.second;
	if (pm)
		delete pm;
}

// Grava em disco, com o nome informado, os objetos identificados.
bool CRedeDePercolacao::SalvarListaObjetos(std::string fileName) {
	return CMatrizObjetoImagem::SalvarListaObjetos(fileName, pm->NX(), pm->NY(), pm->NZ());
}

// Executa o cálculo das distribuições e cria a rede de percolação.
bool CRedeDePercolacao::Go(  int nx, int ny, int nz, CDistribuicao3D::Metrica3D _metrica ) {
	// Determina tamanho mínimo e máximo para a rede de percolação
	int min = 100;
	int max = 1000;
	nx = ( nx < min ) ? min : nx;
	ny = ( ny < min ) ? min : ny;
	nz = ( nz < min ) ? min : nz;
	nx = ( nx > max ) ? max : nx;
	ny = ( ny > max ) ? max : ny;
	nz = ( nz > max ) ? max : nz;
	int area = nx*ny*nz; //área da matriz 3D (em pixeis)
	double phiPoros = 0.0;	//porosidade da matriz de poros(sitios)
	double phiSitios = 0.0;	//porosidade da matriz de sítios
	double phiEsfera = 0.0;	//porosidade da esfera (poro/sitio)
	int x, y, z; //posição na matriz
	CBCd3453D * esfera;
	bool cabe; //flag que indicará se a esfera cabe na região sem sobrepor outras esferas.
	// Calcula as distribuições de tamanho de poros e gargantas
	dtpg = CDistribuicaoTamanhoPorosGargantas::Go(_metrica);
	if (dtpg.first == NULL || dtpg.second == NULL) {
		std::cout << "Não foi calcular as distribuições de tamanho de poros e gargantas em CRedeDePercolacao::Go" << std::endl;
		return false;
	}
	// Cria matriz 3D que servirá para verificar sobreposições na rede.
	pm = new TCMatriz3D<bool>(nx, ny, nz);
	if (pm == NULL) {
		delete dtpg.first;
		delete dtpg.second;
		dtpg.first = NULL;
		dtpg.second = NULL;
		std::cout << "Não foi criar matriz 3D em CRedeDePercolacao::Go" << std::endl;
		return false;
	}
	std::cerr << "Calculando distribuição acumulada (poros)." << std::endl;
	int tamVetDist = dtpg.first->distribuicao.size();
	std::vector<double> distAcumulada(tamVetDist+1);
	distAcumulada[0] = dtpg.first->distribuicao[0];
	for (int i=1; i<tamVetDist; ++i) {
		distAcumulada[i] = distAcumulada[i-1] + dtpg.first->distribuicao[i];
	}

	// Sortear valores aleatórios entre 0 e 1. Obter o raio na distAcumulada
	std::vector<int> raios;
	double random;
	int raio;
	int diametro;
	matrizObjetos.clear();
	phiPoros  = dtpg.first->AreaObjetos();
	phiSitios = 0.0;
	std::cerr << "Sorteando valores aleatórios entre 0 e 1 e obtendo raios na curva de distribuição acumulada." << std::endl;
	while (phiSitios < phiPoros) {
		//std::cerr << "phiSitios: " << phiSitios << " | phiPoros: " << phiPoros << std::endl;
		raio = 1;
		random = DRandom(); //obtem valor double randômico entre 0.0 e 1.0;
		//percorre vetor de distribuição acumulada para obter raio correspondente
		for (int i=0; i<tamVetDist; ++i) {
			if ( random <= distAcumulada[i] ) {
				if (i > 0) { //aqui o valor sorteado é menor ou igual e não estamos no primeiro elemento do vetor
					//verifica a diferença entre o número randômico e os elementas i e i-1. Seta o raio com o indice do valor mais próximo a random.
					if ( (random - distAcumulada[i-1]) < (distAcumulada[i] - random) ) {
						raio = i;
					} else {
						raio = i+1;
					}
					break;
				} else { //aqui o valor sorteado é menor e estamos no primeiro elemento do vetor
					break; //sai do loop com raio == 1
				}
			}
		}
		//calcular a porosidade correspondente a esfera que será criada com o raio sorteado.
		phiEsfera = ((double)numPixeisBola[raio]/(double)area)*100.0;
		//std::cerr << "random: " << random << " | raio: " << raio << " | phiEsfera antes: " << phiEsfera << std::endl;
		//Se a soma das porosidades for maior que a porosidade da matriz de poros e o raio for maior que 1,
		//decrementa o raio até que a soma das porosidades seja menor que a porosidade da matriz de poros,
		//ou ate que o raio seja 1.
		//Ao sair do loop, incrementa o raio e recalcula a area da esfera, de modo que phiSitios
		//fique o mais próximo possível de phiPoros.
		if ( phiSitios+phiEsfera > phiPoros && raio > 1) {
			while( phiSitios+phiEsfera > phiPoros && raio > 1) {
				--raio;
				phiEsfera = ((double)numPixeisBola[raio]/(double)area)*100.0;
			}
			++raio;
			phiEsfera = ((double)numPixeisBola[raio]/(double)area)*100.0;
		}
		raios.push_back(raio);
		//std::cerr << "raio: " << raio << " | phiEsfera depois: " << phiEsfera << std::endl;
		phiSitios += phiEsfera; //acumula a porosidade
	}
	std::cerr << "Saiu do loop!\nphiSitios: " << phiSitios << " | phiPoros: " << phiPoros << std::endl;
	// Ordenar os raios do maior para o menor de forma que as esferas maiores serão criadas primeiro.
	std::sort(raios.begin(), raios.end());
	std::reverse(raios.begin(), raios.end());
	std::multimap<int, int> yToObj; //será uma referência para ordenar os objetos do menor para o maior valor de y.
	int im, jm, km;
	int x_raio, y_raio, z_raio;
	int cont = 0;
	int nTentativas;
	//percorrea o vetor de raios
	for (std::vector<int>::iterator it=raios.begin(); it!=raios.end(); ++it) {
		++cont;
		//pega o raio do primeiro elemento
		raio = *it;
		diametro = ((2*raio)+1);
		//cria esfera de raio correspondente.
		esfera = new CBCd3453D(diametro);
		// Sortear posições testando se a esfera cabe sem sobrepor outras esferas ou ultrapassar a borda.
		nTentativas = 0;
		do {
			//++nTentativas;
			//std::cerr << "Desenhando sítio " << cont << " de " << raios.size() << ". Tentativa: " << nTentativas << endl;
			x = Random(raio, nx-raio-1);
			y = Random(raio, ny-raio-1);
			z = Random(raio, nz-raio-1);
			x_raio = x-raio;
			y_raio = y-raio;
			z_raio = z-raio;
			cabe = true;
			for (int i=0; i<diametro && cabe; ++i) {
				im = i+x_raio;
				for (int j=0; j<diametro && cabe; ++j) {
					jm = j+y_raio;
					for (int k=0; k<diametro && cabe; ++k) {
						km = k+z_raio;
						if ( esfera->data3D[i][j][k]!=0 && pm->data3D[im][jm][km]!=0 ) {
							cabe = false;
						}
					}
				}
			}
			if (cabe) { //desenha a esfera
				std::cerr << "Desenhando sítio " << cont << " de " << raios.size() << endl;
				for (int i=0; i<diametro; ++i) {
					im = i+x_raio;
					for (int j=0; j<diametro; ++j) {
						jm = j+y_raio;
						for (int k=0; k<diametro; ++k) {
							km = k+z_raio;
							if ( esfera->data3D[i][j][k]!=0 ) {
								pm->data3D[im][jm][km]=1;
							}
						}
					}
				}
			}
		} while (!cabe);
		// Posso criar os objetos diretamente, em ordem decrescente de tamanho.
		// Outra opção seria, após este loop, rotular os objetos e percorrer a matriz setando cada objeto na matrizObjetos.
		// Assim teria os objetos rotulados de cima para baixo e da esquerda para a direita.
		matrizObjetos[cont] = CObjetoImagem(SITIO,numPixeisBola[raio]);
		matrizObjetos[cont].pontoCentral.df = 3*raio;
		matrizObjetos[cont].pontoCentral.x = x;
		matrizObjetos[cont].pontoCentral.y = y;
		matrizObjetos[cont].pontoCentral.z = z;
		yToObj.insert(pair<int, int>(y,cont));
		delete esfera;
	}
/*
	std::cerr << "Elements in yToObj: " << std::endl;
	for (multimap<int, int>::iterator it = yToObj.begin(); it != yToObj.end(); ++it) {
		std::cerr << "  [y=" << (*it).first << ", Obj=" << (*it).second << "]" << std::endl;
	}
*/

	// Próximos passos:

	// Partindo da borda superior, conectar a borda aos sítios mais próximos.
	// Percorrer a matriz em Y e para cada sítio encontrado:
	// =>Sortear número de coordenação (Z);
	// =>Sortear raio das Z ligações;
	// =>Conectar o sítio a Z sítios próximos e ir acumulando a porosidade representada pelas ligações;
	yToObj.clear();
	return true;
}
