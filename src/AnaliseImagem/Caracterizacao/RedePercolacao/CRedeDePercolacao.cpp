#include "CRedeDePercolacao.h"
#include <map>

// Analizar se é melhor calcular a porosidade assim ou se devo calcular a área da esfera e do cilindro.
//int CRedeDePercolacao::numPixeisBola[] = {
std::vector<unsigned int> CRedeDePercolacao::numPixeisBola =
{ //só funciona a partir do c++11
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

std::vector<unsigned int> CRedeDePercolacao::numPixeisCirculo
{	0,5,13,29,53,81,113,153,201,253,309,373,445,521,601,689,785,885,989,
	1101,1221,1345,1473,1609,1753,1901,2053,2213,2381,2553,2729,2913,3105,3301,3501,3709,3925,4145,4369,
	4601,4841,5085,5333,5589,5853,6121,6393,6673,6961,7253,7549,7853,8165,8481,8801,9129,9465,9805,10149,
	10501,10861,11225,11593,11969,12353,12741,13133,13533,13941,14353,14769,15193,15625,16061,16501,16949,
	17405,17865,18329,18801,19281,19765,20253,20749,21253,21761,22273,22793,23321,23853,24389,24933,25485,
	26041,26601,27169,27745,28325,28909,29501,30101,30705,31313,31929,32553,33181,33813,34453,35101,35753,
	36409,37073,37745,38421,39101,39789,40485,41185,41889,42601,43321,44045,44773,45509,46253,47001,47753,
	48513,49281,50053,50829,51613,52405,53201,54001,54809,55625,56445,57269,58101,58941,59785,60633,61489,
	62353,63221,64093,64973,65861,66753,67649,68553,69465,70381,71301,72229,73165,74105,75049,76001,76961,
	77925,78893,79869,80853,81841,82833,83833,84841,85853,86869,87893,88925,89961,91001,92049,93105,94165,
	95229,96301,97381,98465,99553,100649,101753,102861,103973,105093,106221,107353,108489,109633,110785,
	111941,113101,114269,115445,116625,117809,119001,120201,121405,122613,123829,125053,126281,127513,128753,
	130001,131253,132509,133773,135045,136321,137601,138889,140185,141485,142789,144101,145421,146745,148073,
	149409,150753,152101,153453,154813,156181,157553,158929,160313,161705,163101,164501,165909,167325,168745,
	170169,171601,173041,174485,175933,177389,178853,180321,181793,183273,184761,186253,187749,189253,190765,
	192281,193801,195329,196865,198405,199949,201501,203061,204625,206193,207769,209353,210941,212533,214133,
	215741,217353,218969,220593,222225,223861,225501,227149,228805,230465,232129,233801,235481,237165,238853,
	240549,242253,243961,245673,247393,249121,250853,252589,254333,256085,257841,259601,261369,263145,264925,
	266709,268501,270301
};

std::vector<unsigned short int> CRedeDePercolacao::perimetroCirculo
{ 0,12,20,28,36,44,52,60,68,76,84,92,100,108,116,124,132,140,148,156,164,172,180,188,196,204,212,220,228,236,
	244,252,260,268,276,284,292,300,308,316,324,332,340,348,356,364,372,380,388,396,404,412,420,428,436,444,452,
	460,468,476,484,492,500,508,516,524,532,540,548,556,564,572,580,588,596,604,612,620,628,636,644,652,660,668,
	676,684,692,700,708,716,724,732,740,748,756,764,772,780,788,796,804,812,820,828,836,844,852,860,868,876,884,
	892,900,908,916,924,932,940,948,956,964,972,980,988,996,1004,1012,1020,1028,1036,1044,1052,1060,1068,1076,
	1084,1092,1100,1108,1116,1124,1132,1140,1148,1156,1164,1172,1180,1188,1196,1204,1212,1220,1228,1236,1244,
	1252,1260,1268,1276,1284,1292,1300,1308,1316,1324,1332,1340,1348,1356,1364,1372,1380,1388,1396,1404,1412,
	1420,1428,1436,1444,1452,1460,1468,1476,1484,1492,1500,1508,1516,1524,1532,1540,1548,1556,1564,1572,1580,
	1588,1596,1604,1612,1620,1628,1636,1644,1652,1660,1668,1676,1684,1692,1700,1708,1716,1724,1732,1740,1748,
	1756,1764,1772,1780,1788,1796,1804,1812,1820,1828,1836,1844,1852,1860,1868,1876,1884,1892,1900,1908,1916,
	1924,1932,1940,1948,1956,1964,1972,1980,1988,1996,2004,2012,2020,2028,2036,2044,2052,2060,2068,2076,2084,
	2092,2100,2108,2116,2124,2132,2140,2148,2156,2164,2172,2180,2188,2196,2204,2212,2220,2228,2236,2244,2252,
	2260,2268,2276,2284,2292,2300,2308,2316,2324,2332,2340,2348,2356,2364,2372,2380,2388,2396,2404
};

//NumElements retorna o tamanho do vetor
//#define NumElements(x) (sizeof(x) / sizeof(x[0]))

// Construtor matriz binária
CRedeDePercolacao::CRedeDePercolacao(unsigned int nx, unsigned int ny, unsigned int nz ) {
	srand (time(NULL)); //inicia seed randômica;

	unsigned int min = 100;
	unsigned int max = 1000;
	nx = ( nx < min ) ? min : nx;
	ny = ( ny < min ) ? min : ny;
	nz = ( nz < min ) ? min : nz;
	nx = ( nx > max ) ? max : nx;
	ny = ( ny > max ) ? max : ny;
	nz = ( nz > max ) ? max : nz;
	// Cria matriz 3D que servirá para verificar sobreposições na rede.
	pm = new TCMatriz3D<bool>(nx, ny, nz);
	ptrMatObjsRede = new CMatrizObjetoRede();
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
bool CRedeDePercolacao::SalvarListaObjetos(std::string nomeArquivo) {
	return CMatrizObjetoImagem::SalvarListaObjetos(nomeArquivo, pm->NX(), pm->NY(), pm->NZ());
}

// Grava em disco, no formato do Grafo, com o nome informado, os objetos identificados.
bool CRedeDePercolacao::SalvarListaObjetosGrafo(std::string nomeArquivo) {
	return CMatrizObjetoImagem::SalvarListaObjetosGrafo(nomeArquivo);
}

// Calcula a condutância de objetos do tipo sítio usando a equação 5.17 da tese Liang (by Koplik 1983)
// g = (r^3) / (3*viscosidade) ->
double CRedeDePercolacao::CondutanciaSitio (CObjetoRede &objetoImagem, double sizePixel, double fatorAmplificacao) {
	// Variáveis auxiliares
	double viscosidade = 1.0;
	double raio = (double)objetoImagem.Raio() * sizePixel * fatorAmplificacao;
	double condutancia = (raio*raio*raio) / (3.0 * viscosidade);
	objetoImagem.Propriedade( condutancia );
	//std::cerr << "Condutancia: " << condutancia << " raio: " << raio << " viscosidade: " << viscosidade << std::endl;
	return condutancia;
}

// Calcula a condutância de objetos do tipo ligação usando a equação 5.16 da tese Liang
// condutancia = pi*dH^4/(128*viscosidade*comprimento)
double CRedeDePercolacao::CondutanciaLigacao (CObjetoRede &objetoImagem, double &_comprimento, double sizePixel, double fatorAmplificacao) {
	// Variáveis auxiliares
	double viscosidade = 1.0;
	double comprimento = _comprimento * sizePixel * fatorAmplificacao;
	// Calcula o raio hidraulico do objeto já convertido para metros
	double raioHidraulico = RaioHidraulicoCirculo(objetoImagem.Raio()) * sizePixel * fatorAmplificacao;
	double diametroHidraulico = 4.0 * raioHidraulico;
	double auxiliar = M_PI / (128.0 * viscosidade * comprimento);
	double condutancia = auxiliar * (diametroHidraulico*diametroHidraulico*diametroHidraulico*diametroHidraulico);
	objetoImagem.Propriedade( condutancia );
	//std::cerr << "Condutancia: " << condutancia << " raio: " << objetoImagem.Raio() << " L: " << comprimento << " rH: " << raioHidraulico << " dH: " << diametroHidraulico << " aux: " << auxiliar << std::endl;
	return condutancia;
}

// Calcula a condutância entre um sítio e uma ligação (considera apenas metade da ligação, pois a outra metade será considerada na ligação com outro sítio)
double CRedeDePercolacao::CondutanciaSitioLigacao (CObjetoRede &objImgSitio, CObjetoRede &objImgLigacao, double &comprimento, double sizePixel, double fatorAmplificacao) {
	double gSitio = CondutanciaSitio(objImgSitio, sizePixel, fatorAmplificacao);
	double meioL = comprimento/2;
	double gLigacao = CondutanciaLigacao(objImgLigacao,meioL,sizePixel,fatorAmplificacao);
		return 1.0/(1.0/gSitio + 1.0/gLigacao);
}

// Executa o cálculo das distribuições e cria a rede de percolação.
bool CRedeDePercolacao::Go( TCImagem3D<bool> *&_pm, int _raioMaximo, int _raioDilatacao, int _fatorReducao, int _incrementoRaio, EModelo _modelo, int _indice, int _fundo, CDistribuicao3D::Metrica3D _metrica ){
	CDistribuicaoTamanhoPorosGargantas cdtpg = CDistribuicaoTamanhoPorosGargantas( _pm, _raioMaximo, _raioDilatacao, _fatorReducao, _incrementoRaio, _modelo, _indice, _fundo );
	// Calcula as distribuições de tamanho de poros e gargantas
	dtpg = cdtpg.Go(_metrica);
	if (dtpg.first == nullptr || dtpg.second == nullptr) {
		std::cerr << "Não foi calcular as distribuições de tamanho de poros e gargantas em CRedeDePercolacao::Go" << std::endl;
		return false;
	}
	return ExecutadaPorGo();
}

// Executa o cálculo das distribuições e cria a rede de percolação.
bool CRedeDePercolacao::Go( TCImagem3D<int> *&_pm, CDistribuicao3D::Metrica3D _metrica ) {
	CDistribuicaoTamanhoPorosGargantas cdtpg = CDistribuicaoTamanhoPorosGargantas( _pm );
	// Calcula as distribuições de tamanho de poros e gargantas
	dtpg = cdtpg.Go(_metrica);
	if (dtpg.first == nullptr || dtpg.second == nullptr) {
		std::cerr << "Não foi calcular as distribuições de tamanho de poros e gargantas em CRedeDePercolacao::Go" << std::endl;
		return false;
	}
	return ExecutadaPorGo();
}

// Executa o cálculo das distribuições e cria a rede de percolação.
bool CRedeDePercolacao::ExecutadaPorGo( ) {
	int nx = pm->NX();
	int ny = pm->NY();
	int nz = pm->NZ();
	int area = nx*ny*nz; //área da matriz 3D (em pixeis)
	double phiDist = 0.0;	//porosidade da matriz de poros(sitios)
	double phiRede = 0.0;	//porosidade da matriz de sítios
	double phiObjeto = 0.0;	//porosidade da esfera (poro/sitio)
	int x, y, z; //posição na matriz
	CBCd3453D * esfera;
	bool cabe; //flag que indicará se a esfera cabe na região sem sobrepor outras esferas.
	//std::multimap<int, int> xToObj; // xToObj será uma referência para ordenar os objetos do menor para o maior valor de x.

	//============================================== SITIOS =================================================
	std::cerr << "Calculando distribuicao acumulada (poros)." << std::endl;
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
	std::map<int, CObjetoRede> matrizObjetosTemp; // Matriz de objetos temporária;
	ptrMatObjsRede->matrizObjetos.clear(); // Matriz de objetos final
	phiDist  = dtpg.first->AreaObjetos();
	phiRede = 0.0;
	std::cerr << "Criando sitios..." << std::endl;
	// Sorteando valores aleatórios entre 0 e 1 e obtendo raios na curva de distribuição acumulada.
	while (phiRede < phiDist) {
		//std::cerr << "phiRede: " << phiRede << " | phiDist: " << phiDist << std::endl;
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
		//phiObjeto = ((double)numPixeisBola[raio]/(double)area)*100.0;
		phiObjeto = PhiEsfera(raio, area);
		//std::cerr << "random: " << random << " | raio: " << raio << " | phiObjeto antes: " << phiObjeto << std::endl;
		//Se a soma das porosidades for maior que a porosidade da matriz de poros e o raio for maior que 1,
		//decrementa o raio até que a soma das porosidades seja menor que a porosidade da matriz de poros,
		//ou ate que o raio seja 1.
		//Ao sair do loop, incrementa o raio e recalcula a area da esfera, de modo que phiRede
		//fique o mais próximo possível de phiDist.
		if ( phiRede+phiObjeto > phiDist && raio > 1) {
			while( phiRede+phiObjeto > phiDist && raio > 1) {
				--raio;
				//phiObjeto = ((double)numPixeisBola[raio]/(double)area)*100.0;
				phiObjeto = PhiEsfera(raio, area);
			}
			++raio;
			//phiObjeto = ((double)numPixeisBola[raio]/(double)area)*100.0;
			phiObjeto = PhiEsfera(raio, area);
		}
		raios.push_back(raio);
		//std::cerr << "raio: " << raio << " | phiObjeto depois: " << phiObjeto << std::endl;
		phiRede += phiObjeto; //acumula a porosidade
	}
	std::cerr << "Sitios criados!\nphiRede: " << phiRede << " | phiDist: " << phiDist << " | Num. Sitios: " << raios.size() << std::endl;
	// Ordenar os raios do maior para o menor de forma que as esferas maiores serão criadas primeiro.
	std::sort(raios.begin(), raios.end());
	std::reverse(raios.begin(), raios.end());
	int im, jm, km;
	int x1 = 0;
	int y1 = ny/2;
	int z1 = nz/2;
	int x_raio, y_raio, z_raio;
	int cont = 0;
	int objeto; //indice do objeto com menor distância entre o ponto 0,0,0 e o centro do objeto (sitio).
	double distancia = 1000000.0; //guarda a menor distancia encontrada  entre o ponto 0,0,0 e o centro dos sítios.
	double distTemp;
	std::vector<int> objsCamadaOeste;
	std::vector<int> objsCamadaLeste;
	int camadaOeste = 1000000;
	int camadaLeste = 0;
	std::cerr << "Representando os sítios na matriz..." << std::endl;
	//percorre o vetor de raios
	for (std::vector<int>::iterator it=raios.begin(); it!=raios.end(); ++it) {
		++cont;
		//pega o raio do primeiro elemento
		raio = *it;
		diametro = ((2*raio)+1);
		//cria esfera de raio correspondente.
		esfera = new CBCd3453D(diametro);
		// Sortear posições testando se a esfera cabe sem sobrepor outras esferas ou ultrapassar a borda.
		do {
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
				//std::cerr << "Desenhando sítio " << cont << " de " << raios.size() << endl;
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
		matrizObjetosTemp[cont] = CObjetoRede(SITIO, NumPixeisEsfera(raio));
		matrizObjetosTemp[cont].pontoCentral.df = 3*raio;
		matrizObjetosTemp[cont].pontoCentral.x = x;
		matrizObjetosTemp[cont].pontoCentral.y = y;
		matrizObjetosTemp[cont].pontoCentral.z = z;

		// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
		//xToObj.insert(pair<int, int>(x,cont));

		// Guarda o objeto com menor distancia do ponto 0,0,0.
		distTemp = DistanciaEntrePontos(x1,y1,z1,x,y,z);
		if ( distTemp <= distancia ) {
			distancia = distTemp;
			objeto = cont;
		}
		delete esfera;
	}

	raios.clear(); // limpa vetor de raios (não será mais utilizado)
	// 0rdena os objetos (sitios) por proximidade.
	std::cerr << "Ordenando os sítios por proximidade..." << std::endl;
	std::map<int, CObjetoRede>::iterator it;
	std::map<int, CObjetoRede>::iterator itMatObj;
	cont = 1;
	// O primeiro objeto foi encontrado no loop anterior
	ptrMatObjsRede->matrizObjetos[cont] = matrizObjetosTemp[objeto];
	matrizObjetosTemp.erase(objeto);

	while ( matrizObjetosTemp.size() > 0 ) {
		distancia = 1000000.0;
		it = ptrMatObjsRede->matrizObjetos.find(cont);
		//for (it = matrizObjetosTemp.begin(); it!=matrizObjetosTemp.end(); ++it) {
		for ( auto & mot : matrizObjetosTemp ) {
			distTemp = DistanciaEntrePontos(it->second.pontoCentral.x, it->second.pontoCentral.y, it->second.pontoCentral.z, mot.second.pontoCentral.x, mot.second.pontoCentral.y, mot.second.pontoCentral.z );
			if ( distTemp <= distancia ){
				distancia = distTemp;
				objeto = mot.first;
			}
		}
		++cont;
		//std::cerr << "Objeto: " << objeto << " | Distancia: " << distancia << std::endl;
		ptrMatObjsRede->matrizObjetos[cont] = matrizObjetosTemp[objeto];
		matrizObjetosTemp.erase(objeto);

		// Pega o sítio e calcula a condutância
		it = ptrMatObjsRede->matrizObjetos.find(cont);
		CondutanciaSitio(it->second);

		// Guarda listas de objetos que se encontram na camada leste e oeste (necessário para montar grafo)
		if (it->second.pontoCentral.x < camadaOeste) {					// Verifica se a camada y do objeto é menor que a atual camanda superior
			camadaOeste = it->second.pontoCentral.x;							// Atualiza a atual camada superior
			objsCamadaOeste.clear();															// Limpa a lista de objetos pertencentes a camada superior
			objsCamadaOeste.push_back(cont);											// Inclui o objeto atual na lista de objetos pentencentes a camada superior
		} else if (it->second.pontoCentral.x == camadaOeste) { // Verifica se a camada y do objeto é igual a atual camada superior
			objsCamadaOeste.push_back(cont);											// Inclui o objeto atual na lista de objetos pentencentes a camada superior
		} else if (it->second.pontoCentral.x > camadaLeste) {	// Verifica se a camada y do objeto é maior que a atual camanda inferior
			camadaLeste = it->second.pontoCentral.x;							// Atualiza a atual camada inferior
			objsCamadaLeste.clear();															// Limpa a lista de objetos pertencentes a camada inferior
			objsCamadaLeste.push_back(cont);											// Inclui o objeto atual na lista de objetos pentencentes a camada inferior
		} else if (it->second.pontoCentral.x == camadaOeste) { // Verifica se a camada y do objeto é igual a atual camada inferior
			objsCamadaOeste.push_back(cont);											// Inclui o objeto atual na lista de objetos pentencentes a camada inferior
		}
	}
	/*
	std::cerr << "Elementos em xToObj: " << std::endl;
	for (multimap<int, int>::iterator it = xToObj.begin(); it != xToObj.end(); ++it) {
		std::cerr << "  [x=" << (*it).first << ", Obj=" << (*it).second << "]" << std::endl;
	}
	*/
	// Percorre lista de obejos petencentes a camada superior atualizando suas camadas
	for (int obj : objsCamadaOeste ) {
		ptrMatObjsRede->matrizObjetos[obj].Contorno(CContorno::ETipoContorno::WEST);
	}
	for (int obj : objsCamadaLeste ) {
		ptrMatObjsRede->matrizObjetos[obj].Contorno(CContorno::ETipoContorno::EST);
	}

	//============================================== LIGAÇÕES =================================================
	std::cerr << "Calculando distribuicao acumulada (gargantas)." << std::endl;
	tamVetDist = dtpg.second->distribuicao.size();
	distAcumulada.clear();
	distAcumulada.resize(tamVetDist+1);
	distAcumulada[0] = dtpg.second->distribuicao[0];
	for (int i=1; i<tamVetDist; ++i) {
		distAcumulada[i] = distAcumulada[i-1] + dtpg.second->distribuicao[i];
	}
	phiDist  = dtpg.second->AreaObjetos();
	phiRede = 0.0;
	cabe = false;
	int nCoord, Z;
	int raioit;
	int raioitt;
	double gSitioLigacao; //Condutância entre sítio e ligação
	int tamMatObjs = ptrMatObjsRede->matrizObjetos.size();
	cont = ptrMatObjsRede->matrizObjetos.rbegin()->first; //índice do último elemento da matriz
	std::map<int, CObjetoRede>::iterator itt;

	std::cerr << "Criando ligacoes..." << std::endl;
	// Durante o loop o tamanho da matrizObjetos será alterado, então, preciso percorrer somente os objetos atuais.
	for ( int obj=1; obj<=tamMatObjs; ++obj ) {
		// Inicialmente cada sítio terá número de coordenação 2.
		if (cabe) {
			nCoord = Random(3,5); // Sorteira números randomicos entre 2 e 4 (corresponderá ao número de coordenação  do sítiios, ou seja, quantas ligações partem dele).
			obj = Random(1,tamMatObjs-1);
		} else {
			nCoord = 2;
		}
		it = ptrMatObjsRede->matrizObjetos.find(obj);
		if (it == ptrMatObjsRede->matrizObjetos.end()) {
			continue;
		}
		itt = it;
		raioit = it->second.Raio();
		for (Z=it->second.NumConexoes()+1; Z<=nCoord; ++Z) {
			do { // Vai para o próximo objeto ainda não conectado ao objeto atual.
				++itt; //iterator para o próximo objeto.
				if (itt == ptrMatObjsRede->matrizObjetos.end() || itt->first > tamMatObjs) {
					break;
				}
			} while ( it->second.SConexao().find(itt->first) != it->second.SConexao().end() );
			if (itt == ptrMatObjsRede->matrizObjetos.end() || itt->first > tamMatObjs)
				break;
			raioitt = itt->second.Raio();
			// Calcula distância entre os sítios
			distancia = DistanciaEntrePontos(it->second.pontoCentral.x, it->second.pontoCentral.y, it->second.pontoCentral.z, itt->second.pontoCentral.x, itt->second.pontoCentral.y, itt->second.pontoCentral.z );
			distancia = distancia - it->second.Raio() - itt->second.Raio();
			if ( distancia < 1 ) // Caso os sítios se toquem, a distância dará 0, então força que seja pelo menos 1
				distancia = 1;
			// Sortear valores aleatórios entre 0 e 1. Obter o raio na distAcumulada
			raio = 1;
			random = DRandom(); //obtem valor double randômico entre 0.0 e 1.0;
			//percorre vetor de distribuição acumulada para obter raio correspondente
			for (int i=0; i<tamVetDist; ++i) {
				if ( random <= distAcumulada[i] ) {
					if (i > 0) { //aqui o valor sorteado é menor ou igual e não estamos no primeiro elemento do vetor
						// Verifica a diferença entre o número randômico e os elementas i e i-1.
						// Seta o raio com o indice do valor mais próximo a random.
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
			// Certifica que o raio da ligação será menor que o raio dos sítios a serem interligados ou igual a 1
			while ( (raio >= raioit || raio >= raioitt) && raio > 1 ) {
				--raio;
			}

			//calcular a porosidade correspondente a ligação (cilindro) que será criada com o raio sorteado.
			//phiObjeto = ((M_PI * (double)raio * (double)raio * distancia)/(double)area)*100.0;
			//phiObjeto = ((double)numPixeisDisco[raio]*(double)distancia/(double)area)*100.0;
			phiObjeto = PhiCilindro(raio, distancia, area);
			//Se a soma das porosidades for maior que a porosidade da matriz de gargantas e o raio for maior que 1,
			//decrementa o raio até que a soma das porosidades seja menor que a porosidade da matriz de gargantas, ou ate que o raio seja 1.
			//Ao sair do loop, incrementa o raio e recalcula a area do cilindro, de modo que phiRede fique o mais próximo possível de phiDist.
			if ( phiRede+phiObjeto > phiDist && raio > 1) {
				while( phiRede+phiObjeto > phiDist && raio > 1) {
					--raio;
					phiObjeto = PhiCilindro(raio, distancia, area);
				}
				++raio;
				phiObjeto = PhiCilindro(raio, distancia, area);
			}
			phiRede += phiObjeto; //acumula a porosidade
			++cont;
			// Conecta os objetos
			ptrMatObjsRede->matrizObjetos[cont] = CObjetoRede(LIGACAO,NumPixeisCilindro(raio, distancia));
			itMatObj = ptrMatObjsRede->matrizObjetos.find(cont);
			itMatObj->second.pontoCentral.df = 3*raio;
			itMatObj->second.pontoCentral.x = (int)((it->second.pontoCentral.x+itt->second.pontoCentral.x)/2);
			itMatObj->second.pontoCentral.y = (int)((it->second.pontoCentral.y+itt->second.pontoCentral.y)/2);
			itMatObj->second.pontoCentral.z = (int)((it->second.pontoCentral.z+itt->second.pontoCentral.z)/2);
			//Cálculo de condutâncias / Conexões
			CondutanciaLigacao(itMatObj->second, distancia);

			gSitioLigacao = CondutanciaSitioLigacao(itt->second, itMatObj->second, distancia );
			itMatObj->second.Conectar(itt->first, gSitioLigacao);
			itt->second.Conectar(cont, gSitioLigacao);

			gSitioLigacao = CondutanciaSitioLigacao(it->second, itMatObj->second, distancia );
			itMatObj->second.Conectar(it->first, gSitioLigacao);
			it->second.Conectar(cont, gSitioLigacao);

			if (phiRede >= phiDist)
				break;
		}
		//std::cerr << "Aqui4!" << std::endl;
		if (phiRede >= phiDist)
			break;
		if ( obj==tamMatObjs && phiRede < phiDist ) {
			//exit;
			cabe = true;
			obj = 1;
		}
	}
	std::cerr << "Ligacoes criadas!\nphiRede: " << phiRede << " | phiDist: " << phiDist << " | Num. Ligacoes: " << cont-tamMatObjs << std::endl;
	//xToObj.clear();
	return true;
}
