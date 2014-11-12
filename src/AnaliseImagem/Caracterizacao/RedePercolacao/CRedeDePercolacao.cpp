#include "CRedeDePercolacao.h"
#include <map>
#include <cmath>

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
CRedeDePercolacao::CRedeDePercolacao(unsigned short int _nx, unsigned short int _ny, unsigned short int _nz, bool _somenteSitios ) {
	srand (time(NULL)); //inicia seed randômica;

	unsigned short int min = 50;
	unsigned short int max = 1000;
	nx = ( _nx < min ) ? min : _nx;
	ny = ( _ny < min ) ? min : _ny;
	nz = ( _nz < min ) ? min : _nz;
	nx = ( _nx > max ) ? max : _nx;
	ny = ( _ny > max ) ? max : _ny;
	nz = ( _nz > max ) ? max : _nz;
	somenteSitios = _somenteSitios;
	// Ponteiro para matriz de objetos que compoem a rede de percolação.
	ptrMatObjsRede = new CMatrizObjetoRede();
}

// Construtor (recebe arquivo no formato padrão da rede)
CRedeDePercolacao::CRedeDePercolacao(std::string filePath, bool _somenteSitios) {
	somenteSitios = _somenteSitios;
	std::cout << "Lendo arquivo da rede e iniciando importação..." << std::endl;
	std::ifstream file(filePath);
	if (file.good()){
		ptrMatObjsRede = new CMatrizObjetoRede();
		numSitios = 0;
		numLigacoes = 0;
		std::vector<int> lstObjsCon;
		std::vector<int> objsCamadaOeste;
		std::vector<int> objsCamadaLeste;
		int camadaOeste = 1000000;
		int camadaLeste = 0;
		int i, numObjs, k, x, y, z, raio, tipo, nVoxels, nObjsCon;
		double daux;
		long double xSolver;
		std::string saux;
		file >> skipws >> saux; //pega o primeiro caracter ignorando espaços em branco;
		std::cout << "saux: " << saux  << std::endl;
		file >> skipws >> numObjs; //número de objetos
		//std::cout << "Rede com " << numObjs << " objetos!" << std::endl;
		file >> skipws >> nx;
		file >> skipws >> ny;
		file >> skipws >> nz;
		file >> skipws >> saux; //pega o caracter #
		file >> skipws >> saux; //pega a string fatorAmplificacao:
		file >> skipws >> fatorAmplificacao;
		std::cout << "fatorAmplificacao: " << fatorAmplificacao << std::endl;
		file >> skipws >> saux; //pega o caracter #
		file >> skipws >> saux; //pega a string dimensaoPixel:
		file >> skipws >> dimensaoPixel;
		std::cout << "dimensaoPixel: " << dimensaoPixel << std::endl;
		{
			char linha[128];
			file.getline(linha, 128);
			file.getline(linha, 128);
		}
		i = 0;
		while ( i < numObjs ) {
			file >> skipws >> i; //número do objeto (sítio ou ligação)
			std::cout << "Importando objeto: " << i << std::endl;
			file >> skipws >> x; //posição no eixo x;
			file >> skipws >> y; //posição no eixo y;
			file >> skipws >> z; //posição no eixo z;
			file >> skipws >> raio; //raio;
			file >> skipws >> tipo; //tipo do objeto (sítio ou ligação)
			file >> skipws >> nVoxels; //número de voxeis que compõem o objeto
			file >> skipws >> nObjsCon; //número de objetos conectados ao objeto
			//std::cout << x << y << z << raio << tipo << nVoxels << nObjsCon << std::endl;

			if (tipo == 2) { // somente sítios pertencem às fronteiras
				// Guarda listas de objetos que se encontram na camada leste e oeste para posteriormente atualizar o tipo de contorno.
				if (x < camadaOeste) {					// Verifica se a camada x do objeto é menor que a atual camanda esquerda
					camadaOeste = x;							// Atualiza a atual camada esquerda
					objsCamadaOeste.clear();			// Limpa a lista de objetos pertencentes a camada esquerda
					objsCamadaOeste.push_back(i);	// Inclui o objeto atual na lista de objetos pentencentes a camada esquerda
				} else if (x == camadaOeste) {  // Verifica se a camada x do objeto é igual a atual camada esquerda
					objsCamadaOeste.push_back(i);	// Inclui o objeto atual na lista de objetos pentencentes a camada esquerda
				} else if (x > camadaLeste) {		// Verifica se a camada x do objeto é maior que a atual camanda direita
					camadaLeste = x;							// Atualiza a atual camada direita
					objsCamadaLeste.clear();			// Limpa a lista de objetos pertencentes a camada direita
					objsCamadaLeste.push_back(i);	// Inclui o objeto atual na lista de objetos pentencentes a camada diretia
				} else if (x == camadaOeste) {  // Verifica se a camada x do objeto é igual a atual camada direita
					objsCamadaOeste.push_back(i);	// Inclui o objeto atual na lista de objetos pentencentes a camada direita
				}
				ptrMatObjsRede->matrizObjetos[i] = CObjetoRedeDePercolacao(ptrMatObjsRede, SITIO, nVoxels);
				++numSitios;
			} else { //ligações
				ptrMatObjsRede->matrizObjetos[i] = CObjetoRedeDePercolacao(ptrMatObjsRede, LIGACAO, nVoxels);
				++numLigacoes;
			}
			ptrMatObjsRede->matrizObjetos[i].Contorno(CContorno::ETipoContorno::CENTER);
			ptrMatObjsRede->matrizObjetos[i].PontoCentral( x, y, z, 3*raio );
			ptrMatObjsRede->matrizObjetos[i].Raio( raio );
			xSolver = (long double)x;
			ptrMatObjsRede->matrizObjetos[i].X(xSolver); //seta o x do solver que será utilizado na simulação;
			lstObjsCon.clear();
			lstObjsCon.resize(nObjsCon);
			for (k=0; k<nObjsCon; ++k) {
				file >> skipws >> lstObjsCon[k];
			}
			for (k=0; k<nObjsCon; ++k) {
				file >> skipws >> daux;
				ptrMatObjsRede->matrizObjetos[i].Conectar(lstObjsCon[k], daux);
			}
			file >> skipws >> daux;
			ptrMatObjsRede->matrizObjetos[i].Propriedade( daux );
		}
		// Percorre lista de obejos petencentes as camadas leste e oeste, atualizando seus contornos.
		for (int &obj : objsCamadaOeste ) {
			ptrMatObjsRede->matrizObjetos[obj].Contorno(CContorno::ETipoContorno::WEST);
		}
		for (int &obj : objsCamadaLeste ) {
			ptrMatObjsRede->matrizObjetos[obj].Contorno(CContorno::ETipoContorno::EST);
		}
		CriarMatObjsSolver ();
		std::cout << "A Rede foi importada!" << std::endl;
	} else {
		std::cerr << "Não foi possível abrir o arquivo " << filePath << std::endl;
	}
}

// Destrutor
CRedeDePercolacao::~CRedeDePercolacao(){
	if (dtpg.first)
		delete dtpg.first;
	if (dtpg.second)
		delete dtpg.second;
}

// Cria a matriz de objetos que serão enviados para o solver de acordo com o critério da flag somenteSitios.
void CRedeDePercolacao::CriarMatObjsSolver (){
	if (somenteSitios) {
		std::map<int,int> objAntToObjAtual;
		std::map<int, double> conexoes;
		ptrMatObjsSolver = new CMatrizObjetoRede();
		std::map<int, CObjetoRedeDePercolacao>::iterator is1; //iterator para o primeiro sítio
		std::map<int, CObjetoRedeDePercolacao>::iterator is2; //iterator para o segundo sítio
		double gSLS;
		double tamLigacao;
		int cont = 0;
		for ( auto &pmor: ptrMatObjsRede->matrizObjetos ) { //percorre a matriz de objetos (sitios e ligações)
			if ( pmor.second.Tipo()==SITIO ) { //se o objeto atual for um sítio
				++cont;
				objAntToObjAtual[pmor.first] = cont;
				ptrMatObjsSolver->matrizObjetos[cont] = CObjetoRedeDePercolacao(ptrMatObjsSolver, SITIO, pmor.second.NumObjs());
				is1 = ptrMatObjsSolver->matrizObjetos.find(cont);
				is1->second.PontoCentral( pmor.second.PontoCentral_X(), pmor.second.PontoCentral_Y(), pmor.second.PontoCentral_Z(), pmor.second.PontoCentral_DF() );
				is1->second.Raio( pmor.second.Raio() );
				is1->second.X(pmor.second.x); //seta o x do solver que será utilizado na simulação;
				is1->second.Propriedade( pmor.second.Propriedade() );
				is1->second.Contorno( pmor.second.Contorno() );
				//Percorre as conexões e para cada ligação conectada ao sítio pega o sítio que está na outra ponta da ligação
				for ( auto &lig : pmor.second.SConexao() ) { //percorre as ligações conectadas ao sítio
					for ( auto &con : ptrMatObjsRede->matrizObjetos[lig.first].SConexao() ) {//percorre os sítios conectados a ligação
						if ( con.first != pmor.first ) { //se o sítio for diferente do sítio atual
							is2 = ptrMatObjsRede->matrizObjetos.find(con.first);
							tamLigacao = DistanciaEntrePontos ( is1->second.PontoCentral_X(),
																									is1->second.PontoCentral_Y(),
																									is1->second.PontoCentral_Z(),
																									is2->second.PontoCentral_X(),
																									is2->second.PontoCentral_Y(),
																									is2->second.PontoCentral_Z()
																								) - is1->second.Raio() - is2->second.Raio();
							gSLS = CondutanciaSitioLigacaoSitio(ptrMatObjsRede->matrizObjetos[lig.first].Raio(), tamLigacao, dimensaoPixel, fatorAmplificacao );
							is1->second.Conectar(is2->first, gSLS);
							break;
						}
					}
				}
			}
		}
		//percorre a matriz de objetos a serem enviados para o solver e atualiza os rótulos dos objetos conectados
		for ( auto &pmos : ptrMatObjsSolver->matrizObjetos ) {
			//copia as conexões para o map temporário
			conexoes.clear();
			for ( auto &obj : pmos.second.SConexao() ) { //a condutância premanece a mesma, só mudou o rótulo do objeto
				conexoes.insert(make_pair(objAntToObjAtual[obj.first], obj.second));
			}
			//limpa o vetor de conexoes do objeto
			pmos.second.SConexao().clear();

			cout << "Sitio " << pmos.first << " conectado ao(s) sitio(s): ";

			//copia para os objetos as novas conexões salvas no map temporário
			for ( auto &obj : conexoes ) {
				cout << obj.first << " | ";
				pmos.second.Conectar(obj.first, obj.second);
			}
			cout << endl;
		}
		conexoes.clear();
		objAntToObjAtual.clear();
	} else {
		ptrMatObjsSolver = ptrMatObjsRede;
	}
}

// Após a rede ser criada ou importada, este método permite calcular as distribuição de tamanho de poros e gargantas da rede.
std::pair<CDistribuicao3D *, CDistribuicao3D *> CRedeDePercolacao::CalcularDistribuicaoRede() {
	std::cout << "Calculando as distribuicoes..." << std::endl;
	if (dtpg.first) //poros
		delete dtpg.first;
	if (dtpg.second) //gargantas
		delete dtpg.second;
	dtpg.first = new CDistribuicao3D();
	dtpg.second = new CDistribuicao3D();
	dtpg.first->Tipo(CBaseDistribuicao::dtp);
	dtpg.second->Tipo(CBaseDistribuicao::dtg);
	std::map<int,double>::iterator it;
	std::map<int,double>::iterator itt;
	std::vector<int> phiSitios;
	std::vector<int> phiLigacoes;
	int phiTotalSitios = 0;
	int phiTotalLigacoes = 0;
	int raio;
	double distancia;
	double area = nx * ny * nz;
	for (auto &obj: ptrMatObjsRede->matrizObjetos) {
		if ( obj.second.Tipo() == SITIO) {
			raio = obj.second.Raio();
			std::cout << "Encontrado sítio de raio: " << raio << std::endl;
			if (raio+1 > phiSitios.size())
				phiSitios.resize(raio+1 , 0);
			phiSitios[raio] += numPixeisBola[raio];
			phiTotalSitios  += numPixeisBola[raio];
			std::cout << "phiSitios[" << raio <<  "]: " << phiSitios[raio] << " - phiTotalSitios: " << phiTotalSitios << std::endl;
		} else { //Ligação
			raio = obj.second.Raio();
			//std::cout << "Encontrada ligação de raio: " << raio << std::endl;
			if (raio+1 > phiLigacoes.size())
				phiLigacoes.resize(raio+1 , 0);
			//distância entre os sitios
			it = itt = obj.second.SConexao().begin(); //primeiro objeto conectado
			++itt; //segundo e último objeto conectado
			distancia = DistanciaEntrePontos ( ptrMatObjsRede->matrizObjetos[it->first].PontoCentral_X(),
					ptrMatObjsRede->matrizObjetos[it->first].PontoCentral_Y(),
					ptrMatObjsRede->matrizObjetos[it->first].PontoCentral_Z(),
					ptrMatObjsRede->matrizObjetos[itt->first].PontoCentral_X(),
					ptrMatObjsRede->matrizObjetos[itt->first].PontoCentral_Y(),
					ptrMatObjsRede->matrizObjetos[itt->first].PontoCentral_Z()
					);
			distancia = distancia - ptrMatObjsRede->matrizObjetos[it->first].Raio() - ptrMatObjsRede->matrizObjetos[itt->first].Raio();
			if ( distancia < 1 ) // Caso os sítios se toquem, a distância dará 0, então força que seja pelo menos 1
				distancia = 1.0;
			phiLigacoes[raio] += NumPixeisCilindro(raio, distancia);
			phiTotalLigacoes  += NumPixeisCilindro(raio, distancia);
			//std::cout << "phiLigacoes[" << raio <<  "]: " << phiLigacoes[raio] << " - phiTotalLigacoes: " << phiTotalSitios << std::endl;
		}
	} //CObjetoRedeDePercolacao::SConexao()
	std::cout << "phiTotalSitios: " << phiTotalSitios <<  "\nphiTotalLigacoes: " << phiTotalLigacoes << std::endl;
#pragma omp paralel default(shared) private(raio)
#pragma omp sections
	{
#pragma omp section
		{
			std::cout << "Calculando distribuições de Sitios..." << std::endl;
			dtpg.first->AreaObjetos(((double)phiTotalSitios/area)*100.0);
			dtpg.first->distribuicao.clear();
			dtpg.first->distribuicao.resize(phiSitios.size()-1);
			//define a área para cada raio de sítio e de ligacao
			for (raio = 1; raio < phiSitios.size(); ++raio) {
				dtpg.first->distribuicao[raio-1] = (double)phiSitios[raio]/(double)phiTotalSitios;
				std::cout << "Raio sitio: " << raio << " phi: " << dtpg.first->distribuicao[raio-1] << std::endl;
			}
			std::cout << "Distribuicoes de Sítios calculadas!" << std::endl;
		}
#pragma omp section
		{
			std::cout << "Calculando distribuições de Ligacoes..." << std::endl;
			dtpg.second->AreaObjetos(((double)phiTotalLigacoes/area)*100.0);
			dtpg.second->distribuicao.clear();
			dtpg.second->distribuicao.resize(phiLigacoes.size()-1);
			for (raio = 1; raio < phiLigacoes.size(); ++raio) {
				dtpg.second->distribuicao[raio-1] = (double)phiLigacoes[raio]/(double)phiTotalLigacoes;
				std::cout << "Raio ligacao: " << raio << " phi: " << dtpg.second->distribuicao[raio-1] << std::endl;
			}
			std::cout << "Distribuicoes de Ligações calculadas!" << std::endl;
		}
	}
	std::cout << "Finalizado calculo das distribuicoes!" << std::endl;
	return dtpg;
}

// Calcula a condutância de objetos do tipo sítio usando a equação 5.17 da tese Liang (by Koplik 1983)
// g = (r^3) / (3*viscosidade) ->
double CRedeDePercolacao::CondutanciaSitio (int _raio, double &dimensaoPixel, double &fatorAmplificacao) {
	// Variáveis auxiliares
	double raio = (double)_raio * dimensaoPixel * fatorAmplificacao;
	double condutancia = (raio*raio*raio) / (3.0 * viscosidade);
	//std::cout << "Condutancia: " << condutancia << " raio: " << raio << " viscosidade: " << viscosidade << std::endl;
	return condutancia;
}

// Calcula a condutância de objetos do tipo ligação usando a equação 5.16 da tese Liang
// condutancia = pi*dH^4/(128*viscosidade*comprimento)
double CRedeDePercolacao::CondutanciaLigacao (int _raio, double &_comprimento, double &dimensaoPixel, double &fatorAmplificacao) {
	// Variáveis auxiliares
	double comprimento = _comprimento * dimensaoPixel * fatorAmplificacao;
	// Calcula o raio hidraulico do objeto já convertido para metros
	double raioHidraulico = RaioHidraulicoCirculo(_raio) * dimensaoPixel * fatorAmplificacao;
	double diametroHidraulico = 4.0 * raioHidraulico;
	double auxiliar = M_PI / (128.0 * viscosidade * comprimento);
	double condutancia = auxiliar * (diametroHidraulico*diametroHidraulico*diametroHidraulico*diametroHidraulico);
	//std::cout << "Condutancia: " << condutancia << " raio: " << objetoImagem.Raio() << " L: " << comprimento << " rH: " << raioHidraulico << " dH: " << diametroHidraulico << " aux: " << auxiliar << std::endl;
	return condutancia;
}

// Calcula a condutância entre um sítio e uma ligação (considera apenas metade da ligação, pois a outra metade será considerada na ligação com outro sítio)
double CRedeDePercolacao::CondutanciaSitioLigacao (int _raio, double &comprimento, double &dimensaoPixel, double &fatorAmplificacao) {
	double gSitio = CondutanciaSitio(_raio, dimensaoPixel, fatorAmplificacao);
	double meioL = comprimento/2;
	double gLigacao = CondutanciaLigacao(_raio,meioL,dimensaoPixel,fatorAmplificacao);
	return 1.0/( (1.0/gSitio) + (1.0/gLigacao) ); //inverso da resistência
}

// Calcula a condutância entre um sítio, uma ligação e o outro sítio conectado a ligação
double CRedeDePercolacao::CondutanciaSitioLigacaoSitio (int _raio, double &comprimento, double &dimensaoPixel, double &fatorAmplificacao) {
	double gSitio = CondutanciaSitio(_raio, dimensaoPixel, fatorAmplificacao);
	double gLigacao = CondutanciaLigacao(_raio,comprimento,dimensaoPixel,fatorAmplificacao);
	return 1.0/( (1.0/gSitio) + (1.0/gLigacao) + (1.0/gSitio) ); //inverso da resistência
}

// Executa o cálculo das distribuições e cria a rede de percolação de acordo com o modelo informado.
bool CRedeDePercolacao::Go( TCImagem3D<bool> *&_pm, int _raioMaximo, int _raioDilatacao, int _fatorReducao, int _incrementoRaio, EModelo _modelo, int _indice, int _fundo, CDistribuicao3D::Metrica3D _metrica, EModeloRede _modeloRede ){
	if (dtpg.first)
		delete dtpg.first;
	if (dtpg.second)
		delete dtpg.second;
	CDistribuicaoTamanhoPorosGargantas cdtpg = CDistribuicaoTamanhoPorosGargantas( _pm, _raioMaximo, _raioDilatacao, _fatorReducao, _incrementoRaio, _modelo, _indice, _fundo );
	// Calcula as distribuições de tamanho de poros e gargantas
	dtpg = cdtpg.Go(_metrica);
	if (dtpg.first == nullptr || dtpg.second == nullptr) {
		std::cerr << "Não foi calcular as distribuições de tamanho de poros e gargantas em CRedeDePercolacao::Go" << std::endl;
		return false;
	}
	dimensaoPixel = _pm->DimensaoPixel();
	fatorAmplificacao = _pm->FatorAmplificacao();
	bool retorno = false;
	switch (_modeloRede) {
		case EModeloRede::um: retorno = Modelo1( );
			break;
		case EModeloRede::dois: retorno = Modelo2( );
			break;
		case EModeloRede::tres: retorno = Modelo3( );
			break;
		case EModeloRede::quatro: retorno = Modelo4( );
			break;
		case EModeloRede::cinco: retorno = Modelo5( );
			break;
		case EModeloRede::seis: retorno = Modelo6( );
			break;
	}
	CriarMatObjsSolver ();
	return retorno;
}

/// Executa o cálculo das distribuições e cria a rede de percolação.
bool CRedeDePercolacao::Go( std::string dtpFile, std::string dtgFile, double _dimensaoPixel, double _fatorAmplificacao, EModeloRede _modeloRede ) {
	if (dtpg.first)
		delete dtpg.first;
	if (dtpg.second)
		delete dtpg.second;
	dtpg.first = new CDistribuicao3D(dtpFile);
	dtpg.second = new CDistribuicao3D(dtgFile);
	if (dtpg.first == nullptr || dtpg.second == nullptr || dtpg.first->distribuicao.size() == 0 || dtpg.second->distribuicao.size() == 0) {
		std::cerr << "Não foi calcular as distribuições de tamanho de poros e gargantas em CRedeDePercolacao::Go" << std::endl;
		return false;
	}
	dimensaoPixel = _dimensaoPixel;
	fatorAmplificacao = _fatorAmplificacao;
	bool retorno = false;
	switch (_modeloRede) {
		case EModeloRede::um: retorno = Modelo1( );
			break;
		case EModeloRede::dois: retorno = Modelo2( );
			break;
		case EModeloRede::tres: retorno = Modelo3( );
			break;
		case EModeloRede::quatro: retorno = Modelo4( );
			break;
		case EModeloRede::cinco: retorno = Modelo5( );
			break;
		case EModeloRede::seis: retorno = Modelo6( );
			break;
	}
	CriarMatObjsSolver ();
	return retorno;
}


// Executa o cálculo das distribuições e cria a rede de percolação de acordo com o modelo informado.
bool CRedeDePercolacao::Go( TCImagem3D<int> *&_pm, CDistribuicao3D::Metrica3D _metrica, EModeloRede _modeloRede ) {
	CDistribuicaoTamanhoPorosGargantas cdtpg = CDistribuicaoTamanhoPorosGargantas( _pm );
	// Calcula as distribuições de tamanho de poros e gargantas
	if (dtpg.first)
		delete dtpg.first;
	if (dtpg.second)
		delete dtpg.second;
	dtpg = cdtpg.Go(_metrica);
	if (dtpg.first == nullptr || dtpg.second == nullptr) {
		std::cerr << "Não foi calcular as distribuições de tamanho de poros e gargantas em CRedeDePercolacao::Go" << std::endl;
		return false;
	}
	dimensaoPixel = _pm->DimensaoPixel();
	fatorAmplificacao = _pm->FatorAmplificacao();
	bool retorno;
	switch (_modeloRede) {
		case EModeloRede::um: retorno = Modelo1( );
			break;
		case EModeloRede::dois: retorno = Modelo2( );
			break;
		case EModeloRede::tres: retorno = Modelo3( );
			break;
		case EModeloRede::quatro: retorno = Modelo4( );
			break;
		case EModeloRede::cinco: retorno = Modelo5( );
			break;
		case EModeloRede::seis: retorno = Modelo6( );
			break;
	}
	CriarMatObjsSolver ();
	return retorno;
}

// Cálcula a distribuição de poros acumulada, cria e retorna ponteiro para o vetor de raios
std::vector<int> * CRedeDePercolacao::CriarVetorDeRaiosDosSitios() {
	std::cout << "Calculando distribuicao acumulada (poros)." << std::endl;
	int tamVetDist = dtpg.first->distribuicao.size();
	std::vector<double> distAcumulada(tamVetDist+1);
	distAcumulada[0] = dtpg.first->distribuicao[0];
	for (int i=1; i<tamVetDist; ++i) {
		distAcumulada[i] = distAcumulada[i-1] + dtpg.first->distribuicao[i];
	}

	std::cout << "Criando sitios..." << std::endl;
	// Sortear valores aleatórios entre 0 e 1. Obter o raio na distAcumulada
	std::vector<int> * raios = new std::vector<int>();
	long double phiDist = 0.0;	//porosidade da matriz de poros(sitios)
	long double phiRede = 0.0;	//porosidade da matriz de sítios
	long double phiObjeto = 0.0;	//porosidade da esfera (poro/sitio)
	int area = nx*ny*nz; //área da matriz 3D (em pixeis)
	double random;
	int raio;
	phiDist  = dtpg.first->AreaObjetos();
	phiRede = 0.0;
	// Sorteando valores aleatórios entre 0 e 1 e obtendo raios na curva de distribuição acumulada.
	while (phiRede < phiDist) {
		//std::cout << "phiRede: " << phiRede << " | phiDist: " << phiDist << std::endl;
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
		//std::cout << "random: " << random << " | raio: " << raio << " | phiObjeto antes: " << phiObjeto << std::endl;
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
		raios->push_back(raio);
		//std::cout << "raio: " << raio << " | phiObjeto depois: " << phiObjeto << std::endl;
		phiRede += phiObjeto; //acumula a porosidade
	}
	numSitios = raios->size();
	std::cout << "Sitios criados!\nphiRede: " << phiRede << " | phiDist: " << phiDist << " | Num. Sitios: " << numSitios << std::endl;
	// Ordenar os raios do maior para o menor de forma que as esferas maiores serão criadas primeiro.
	std::sort(raios->begin(), raios->end());
	std::reverse(raios->begin(), raios->end());
	return raios;
}

// Cria e retorna ponteiro para o vetor de raios. Considera a distribuição de tamanho de poros e a porosidade.
std::vector<int> * CRedeDePercolacao::CriarVetorDeRaiosDosSitiosByDTP() {
	std::cout << "Criando sitios..." << std::endl;
	// Cria os sítios do maior para o menor
	std::vector<int> * raios = new std::vector<int>();
	int area = nx*ny*nz; //área da matriz 3D (em pixeis)
	long double phiDist = dtpg.first->AreaObjetos();	//porosidade da matriz de poros(sitios)
	double phiRede = 0.0;	//porosidade da matriz de sítios
	double areaPoros = (area*phiDist)/100.0;
	double areaPoroI;
	int raio;
	int nBolas;

	for ( int i = dtpg.first->distribuicao.size()-1; i >= 0; --i ) {
		raio = i+1;
		areaPoroI = dtpg.first->distribuicao[i]*areaPoros;
		nBolas = (int)( areaPoroI/numPixeisBola[raio] );
		if (nBolas < 2)
			nBolas = 2;
		phiRede += nBolas * numPixeisBola[raio];
		while ( nBolas > 0 ) {
			--nBolas;
			raios->push_back(raio);
		}
	}
	phiRede = (phiRede/(double)area)*100.0;
	numSitios = raios->size();
	std::cout << "Sitios criados!\nphiRede: " << phiRede << " | phiDist: " << phiDist << " | Num. Sitios: " << numSitios << std::endl;
	return raios;
}

// Cria rede de percolação com sítios alinhados, porém de tamanhos aleatórios com variação do número de ligações.
bool CRedeDePercolacao::Modelo0() {
	std::cout << "Criando rede atraves do Modelo 0..." << std::endl;
	int area = nx*ny*nz; //área da matriz 3D (em pixeis)
	long double phiGargantas	= dtpg.second->AreaObjetos(); //porosidade da imagem (garganta)
	long double phiLigacoes		= 0.0; //porosidade da rede (licações)
	long double phiObjeto			= 0.0; //porosidade do objeto que esta sendo criado (esfera/sítio, cilindro/ligação)
	long double xSolver				= 0.0; //variável utilizada para setar o valor x do parametro de solver do objeto. Utilizado na simulação.
	int x, y, z; //posição na matriz
	int raio;
	int cont;
	double distancia = 1000000.0; //guarda a menor distancia encontrada  entre o ponto 0,0,0 e o centro dos sítios.
	int diametro;

	bool cabe; //flag
	std::multimap<int, int> xToObj; // xToObj será uma referência para ordenar os objetos do menor para o maior valor de x.
	std::map<int, CObjetoRedeDePercolacao> matrizObjetosTemp; // Matriz de objetos temporária;

	//============================================== SITIOS =================================================
	std::vector<int> * raios = CriarVetorDeRaiosDosSitios();
	// o vetor de raios vem ordenado do maior para o menor, então pega o primeiro raio para saber qual a maior esfera a ser alocada.
	int raioMax = raios->at(0);
	int raioMaxPP = raioMax + 1;
	// Calcula quantas fileiras de sítios podem ser alocadas na matriz, sem que os sítios se sobreponham.
	int nCol = floor ( nx / ( (2*raioMax)+2) );
	// Calcula qual será a posição x da fronteira oeste
	int xMax = (nCol-1)*((2*raioMax)+1) + raioMaxPP;

	std::cout  << std::endl << "Numero de posições disponíveis na rede: " << nCol*nCol*nCol << std::endl;
	std::cout << "Numero de sítio a serem alocados: " << raios->size() << std::endl  << std::endl;

	// se existirem mais sítios que posições a serem alocadas, retorna falso.
	if (raios->size() > nCol*nCol*nCol)
		return false;

	std::cout << "Representando os sítios na matriz..." << std::endl;
	cont = 0;
	// aloca matriz auxiliar que controla as posições ativas.
	TCMatriz3D<int> pm(nCol, nCol, nCol);
	pm.Constante(0);
	int px, py, pz;
	for ( auto &raio : *raios ) {
		diametro = ((2*raio)+1);
		do { // Sortear posição até que encontre um lugar que ainda não tenha sítio alocado.
			px = Random(0, nCol-1);
			py = Random(0, nCol-1);
			pz = Random(0, nCol-1);
		} while ( pm.data3D[px][py][pz] != 0 );
		// encontrou uma posição livre, aloca o sítio
		++cont;
		pm.data3D[px][py][pz] = cont;

		// calcula a posição que irá ocupar na rede
		x = ( px * diametro ) + raioMaxPP;
		y = ( py * diametro ) + raioMaxPP;
		z = ( pz * diametro ) + raioMaxPP;

		// aloca na matriz de objetos o sítio criado

		matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, SITIO, NumPixeisEsfera(raio));
		matrizObjetosTemp[cont].PontoCentral( x, y, z, 3*raio );
		matrizObjetosTemp[cont].Raio( raio );

		if ( x == raioMaxPP ) {
			matrizObjetosTemp[cont].Contorno(CContorno::ETipoContorno::WEST);
		} else if ( x == xMax) {
			matrizObjetosTemp[cont].Contorno(CContorno::ETipoContorno::EST);
		} else {
			matrizObjetosTemp[cont].Contorno(CContorno::ETipoContorno::CENTER);
		}
		xSolver = (long double)x;
		matrizObjetosTemp[cont].X(xSolver); //seta o x do solver que será utilizado na simulação;
		matrizObjetosTemp[cont].Propriedade( CondutanciaSitio(matrizObjetosTemp[cont].Raio(), dimensaoPixel, fatorAmplificacao) );
		// Alimenta matriz que referencía os objetos de forma que estes fiquem ordenados em x
		xToObj.insert(pair<int, int>(matrizObjetosTemp[cont].PontoCentral_X(),cont));
	}
	raios->clear(); // limpa vetor de raios (não será mais utilizado)
	delete raios;

	//============================================== LIGAÇÕES =================================================
	std::cout << "Calculando distribuicao acumulada (gargantas)." << std::endl;
	double random;
	int tamVetDist = dtpg.second->distribuicao.size();
	std::vector<double> distGargantasAcumulada(tamVetDist+1);
	distGargantasAcumulada[0] = dtpg.second->distribuicao[0];
	for (int i=1; i<tamVetDist; ++i) {
		distGargantasAcumulada[i] = distGargantasAcumulada[i-1] + dtpg.second->distribuicao[i];
	}
	bool continua = true;
	bool percorreuTodos = false;
	bool testouTodos = false;
	int nCoord, Z;
	int raioSitio;
	int raioVizinho;
	int vizinho;
	int sitio;
	int sitioVizinho;
	long double gSitioLigacao; //Condutância entre sítio e ligação
	int tamMatObjs = matrizObjetosTemp.size();
	cont = matrizObjetosTemp.rbegin()->first; //índice do último elemento da matriz
	std::vector<bool> testou(26);
	std::map<int, CObjetoRedeDePercolacao>::iterator itSitio;
	std::map<int, CObjetoRedeDePercolacao>::iterator itVizinho;
	std::map<int, CObjetoRedeDePercolacao>::iterator itMatObj;
	std::cout << "Criando ligacoes..." << std::endl;
	for (int i=0; i<nCol && continua; ++i) {
		for (int j=0; j<nCol && continua; ++j) {
			for (int k=0; k<nCol && continua; ++k) {
				sitio = pm.data3D[i][j][k];
				if ( sitio != 0 ) {
					// pega o objeto
					itSitio = matrizObjetosTemp.find(sitio);
					// após percorrer todos os sítios, precisa sortear sítios aleatórios até atingir a porosidade
					if (percorreuTodos) {
						nCoord = Random(3,5); // Sorteira números randomicos entre 3 e 5 (corresponderá ao número de coordenação  do sítiios, ou seja, quantas ligações partem dele).
						i = Random(0,nCol);
						j = Random(0,nCol);
						k = Random(0,nCol-1); // para evitar que saia do loop sem atingir a porosidade.
					} else {
						nCoord = 2;
					}
					// se o sítio ainda não atingiu o número de coordenação
					if ( itSitio->second.SConexao().size() < nCoord ) {
						//zera o vetor de testes
						for (int e=0; e<26; ++e) {
							testou[e] = false;
						}
						do {
							cabe = false;
							// sorteia vizinho a ser conectado. considera os 26 possíveis vizinhos de um objeto no meio 3D
							vizinho = Random(1,26);
							switch (vizinho) {
								case 1: testou[0] = true;
									if ( i>0 && j>0 && k>0 ) {
										x = i-1;
										y = j-1;
										z = k-1;
										cabe = true;
									}
									break;
								case 2: testou[1] = true;
									if ( j>0 && k>0 ) {
										x = i;
										y = j-1;
										z = k-1;
										cabe = true;
									}
									break;
								case 3: testou[2] = true;
									if ( i<nCol && j>0 && k>0 ) {
										x = i+1;
										y = j-1;
										z = k-1;
										cabe = true;
									}
									break;
								case 4: testou[3] = true;
									if ( i>0 && k>0 ) {
										x = i-1;
										y = j;
										z = k-1;
										cabe = true;
									}
									break;
								case 5: testou[4] = true;
									if ( k>0 ) {
										x = i;
										y = j;
										z = k-1;
										cabe = true;
									}
									break;
								case 6: testou[5] = true;
									if ( i<nCol && k>0 ) {
										x = i+1;
										y = j;
										z = k-1;
										cabe = true;
									}
									break;
								case 7: testou[6] = true;
									if ( i>0 && j<nCol && k>0 ) {
										x = i-1;
										y = j+1;
										z = k-1;
										cabe = true;
									}
									break;
								case 8: testou[7] = true;
									if ( j<nCol && k>0 ) {
										x = i;
										y = j+1;
										z = k-1;
										cabe = true;
									}
									break;
								case 9: testou[8] = true;
									if ( i<nCol && j<nCol && k>0 ) {
										x = i+1;
										y = j+1;
										z = k-1;
										cabe = true;
									}
									break;
								case 10: testou[9] = true;
									if ( i>0 && j>0 ) {
										x = i-1;
										y = j-1;
										z = k;
										cabe = true;
									}
									break;
								case 11: testou[10] = true;
									if ( j>0 ) {
										x = i;
										y = j-1;
										z = k;
										cabe = true;
									}
									break;
								case 12: testou[11] = true;
									if ( i<nCol && j>0 ) {
										x = i+1;
										y = j-1;
										z = k;
										cabe = true;
									}
									break;
								case 13: testou[12] = true;
									if ( i>0 ) {
										x = i-1;
										y = j;
										z = k;
										cabe = true;
									}
									break;
								case 14: testou[13] = true;
									if ( i<nCol ) {
										x = i+1;
										y = j;
										z = k;
										cabe = true;
									}
									break;
								case 15: testou[14] = true;
									if ( i>0 && j<nCol ) {
										x = i-1;
										y = j+1;
										z = k;
										cabe = true;
									}
									break;
								case 16: testou[15] = true;
									if ( j<nCol ) {
										x = i;
										y = j+1;
										z = k;
										cabe = true;
									}
									break;
								case 17: testou[16] = true;
									if ( i<nCol && j<nCol ) {
										x = i+1;
										y = j+1;
										z = k;
										cabe = true;
									}
									break;
								case 18: testou[17] = true;
									if ( i>0 && j>0 && k<nCol ) {
										x = i-1;
										y = j-1;
										z = k+1;
										cabe = true;
									}
									break;
								case 19: testou[18] = true;
									if ( j>0 && k<nCol ) {
										x = i;
										y = j-1;
										z = k+1;
										cabe = true;
									}
									break;
								case 20: testou[19] = true;
									if ( i<nCol && j>0 && k<nCol ) {
										x = i+1;
										y = j-1;
										z = k+1;
										cabe = true;
									}
									break;
								case 21: testou[20] = true;
									if ( i>0 && k<nCol ) {
										x = i-1;
										y = j;
										z = k+1;
										cabe = true;
									}
									break;
								case 22: testou[21] = true;
									if ( k<nCol ) {
										x = i;
										y = j;
										z = k+1;
										cabe = true;
									}
									break;
								case 23: testou[22] = true;
									if ( i<nCol && k<nCol ) {
										x = i+1;
										y = j;
										z = k+1;
										cabe = true;
									}
									break;
								case 24: testou[23] = true;
									if ( i>0 && j<nCol && k<nCol ) {
										x = i-1;
										y = j+1;
										z = k+1;
										cabe = true;
									}
									break;
								case 25: testou[24] = true;
									if ( j<nCol && k<nCol ) {
										x = i;
										y = j+1;
										z = k+1;
										cabe = true;
									}
									break;
								case 26: testou[25] = true;
									if ( i<nCol && j<nCol && k<nCol ) {
										x = i+1;
										y = j+1;
										z = k+1;
										cabe = true;
									}
									break;
							}
							if (cabe) { // verifica se já existe conexao entre [i][j][k] e [x][y][z]
								sitioVizinho = pm.data3D[x][y][z];
								for (auto &obj : itSitio->second.SConexao()) {
									if (obj.first == sitioVizinho) {
										cabe = false;
										break;
									}
								}
								if (cabe) { // se ainda cabe, verifica se o vizinho não atingiu o número de coordenação
									itVizinho = matrizObjetosTemp.find(sitioVizinho);
									if ( itVizinho->second.SConexao().size() >= nCoord ) {
										cabe = false;
									}
								}
							}
							if (!cabe) { //verifica se testou todos os vizinhos
								testouTodos = true;
								for (int e=0; e<26; ++e) {
									if (testou[e] == false) {
										testouTodos = false;
										break;
									}
								}
							}
						} while ( !cabe && !testouTodos );
						// se saiu do loop e !cabe é porque o sítio já possui todas as possíveis conexões, ou seja, testouTodos == true
						if (cabe) {// faz a ligação entre o sitio e o sitioVizinho
							raioSitio = itSitio->second.Raio();
							raioVizinho = itVizinho->second.Raio();
							// Calcula distância entre os sítios
							distancia = DistanciaEntrePontos(itSitio->second.PontoCentral_X(), itSitio->second.PontoCentral_Y(), itSitio->second.PontoCentral_Z(), itVizinho->second.PontoCentral_X(), itVizinho->second.PontoCentral_Y(), itVizinho->second.PontoCentral_Z() );
							distancia = distancia - raioSitio - raioVizinho;
							if ( distancia < 1 ) // Caso os sítios se toquem, a distância dará 0, então força que seja pelo menos 1
								distancia = 1;
							// Sortear valores aleatórios entre 0 e 1. Obter o raio na distGargantasAcumulada
							raio = 1;
							random = DRandom(); //obtem valor double randômico entre 0.0 e 1.0;
							//percorre vetor de distribuição acumulada para obter raio correspondente
							for (int n=0; n<tamVetDist; ++n) {
								if ( random <= distGargantasAcumulada[n] ) {
									if (n > 0) { //aqui o valor sorteado é menor ou igual e não estamos no primeiro elemento do vetor
										// Verifica a diferença entre o número randômico e os elementas i e i-1.
										// Seta o raio com o indice do valor mais próximo a random.
										if ( (random - distGargantasAcumulada[n-1]) < (distGargantasAcumulada[n] - random) ) {
											raio = n;
										} else {
											raio = n+1;
										}
										break;
									} else { //aqui o valor sorteado é menor e estamos no primeiro elemento do vetor
										break; //sai do loop com raio == 1
									}
								}
							}
							// Certifica que o raio da ligação será menor que o raio dos sítios a serem interligados ou igual a 1
							while ( (raio >= raioSitio || raio >= raioVizinho) && raio > 1 ) {
								--raio;
							}

							//calcular a porosidade correspondente a ligação (cilindro) que será criada com o raio sorteado.
							//phiObjeto = ((M_PI * (double)raio * (double)raio * distancia)/(double)area)*100.0;
							//phiObjeto = ((double)numPixeisDisco[raio]*(double)distancia/(double)area)*100.0;
							phiObjeto = PhiCilindro(raio, distancia, area);
							//Se a soma das porosidades for maior que a porosidade da matriz de gargantas e o raio for maior que 1,
							//decrementa o raio até que a soma das porosidades seja menor que a porosidade da matriz de gargantas, ou ate que o raio seja 1.
							//Ao sair do loop, incrementa o raio e recalcula a area do cilindro, de modo que phiLigacoes fique o mais próximo possível de phiGargantas.
							if ( phiLigacoes+phiObjeto > phiGargantas && raio > 1) {
								while( phiLigacoes+phiObjeto > phiGargantas && raio > 1) {
									--raio;
									phiObjeto = PhiCilindro(raio, distancia, area);
								}
								++raio;
								phiObjeto = PhiCilindro(raio, distancia, area);
							}
							phiLigacoes += phiObjeto; //acumula a porosidade
							// Conecta os objetos
							++cont;
							matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, LIGACAO, NumPixeisCilindro(raio, distancia));
							itMatObj = matrizObjetosTemp.find(cont);
							itMatObj->second.PontoCentral(
										(int)round((itSitio->second.PontoCentral_X() + itVizinho->second.PontoCentral_X())/2),
										(int)round((itSitio->second.PontoCentral_Y() + itVizinho->second.PontoCentral_Y())/2),
										(int)round((itSitio->second.PontoCentral_Z() + itVizinho->second.PontoCentral_Z())/2),
										3*raio
										);
							itMatObj->second.Raio( raio );
							xSolver = (long double)itMatObj->second.PontoCentral_X();
							itMatObj->second.X(xSolver);

							// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
							xToObj.insert(pair<int, int>(itMatObj->second.PontoCentral_X(),cont));

							//Cálculo de condutâncias e realiza conexões
							itMatObj->second.Propriedade ( CondutanciaLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao) );
							//primeiro sítio
							gSitioLigacao = CondutanciaSitioLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
							itMatObj->second.Conectar(itVizinho->first, gSitioLigacao);
							itVizinho->second.Conectar(cont, gSitioLigacao);
							//segundo sítio
							gSitioLigacao = CondutanciaSitioLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
							itMatObj->second.Conectar(itSitio->first, gSitioLigacao);
							itSitio->second.Conectar(cont, gSitioLigacao);

							// se já atingiu a porosidade, força a saída do loop.
							if (phiLigacoes >= phiGargantas) {
								continua = false;
							}
						} // if (cabe) {
					} // if ( itSitio->second.SConexao().size() < nCoord ) {
				} // if ( sitio != 0 ) {
				if (i==nCol && j==nCol && k==nCol) {
					percorreuTodos = true;
				}
			} // for k
		} // for j
	} // for i
	numLigacoes = cont-tamMatObjs;
	std::cout << "Ligacoes criadas!\nphiLigacoes: " << phiLigacoes << " | phiGargantas: " << phiGargantas << " | Num. Ligacoes: " << numLigacoes << std::endl;
	std::cout << cont << " objetos criados!" << std::endl;

	std::cout << "Ordenado os objetos (sítios e ligações) pelo eixo x ..." << std::endl;
	std::map<int,int> objAntToObjAtual;
	ptrMatObjsRede->matrizObjetos.clear(); // Matriz de objetos final
	cont = 0;
	std::cout << "Copia os objetos, já ordenados em x, da matriz temporária para a metriz definitiva..." << std::endl;
	for (auto &xto : xToObj ) {
		++cont;
		ptrMatObjsRede->matrizObjetos[cont] = matrizObjetosTemp[xto.second];
		objAntToObjAtual[xto.second] = cont;
		std::cout << "[x=" << xto.first << ",\t\t\tObjAnt=" << xto.second<< ",\t\t\tObjAtu=" << cont << "]" << std::endl;
	}
	matrizObjetosTemp.clear();

	//percorre a matriz de objetos e atualiza os rótulos dos objetos conectados
	std::map<int, double> conexoesAtu;
	//std::map<int, double> conexoesAnt;
	for ( auto &objs : ptrMatObjsRede->matrizObjetos ) {
		//copia as conexões para o map temporário
		conexoesAtu.clear();
		//conexoesAnt = objs.second.SConexao();
		//for ( auto obj : conexoesAnt ) { //a condutância premanece a mesma, só mudou o rótulo do objeto
		for ( auto &obj : objs.second.SConexao() ) { //a condutância premanece a mesma, só mudou o rótulo do objeto
			conexoesAtu.insert(make_pair(objAntToObjAtual[obj.first], obj.second));
		}
		//limpa o vetor de conexoes do objeto
		//conexoesAnt.clear();
		objs.second.SConexao().clear();

		//copia para os objetos as novas conexões salvas no map temporário
		for ( auto &obj : conexoesAtu ) {
			//conexoesAnt.insert(make_pair(obj.first, obj.second));
			objs.second.Conectar(obj.first, obj.second);
		}
	}

	objAntToObjAtual.clear();
	xToObj.clear();

	return true;
}

// Cria rede de percolação com sítios em posições e tamanhos aleatórios com variação do número de ligações.
bool CRedeDePercolacao::Modelo1() {
	std::cout << "Criando rede atraves do Modelo 1..." << std::endl;
	TCMatriz3D<bool> pm(nx, ny, nz);
	long double xSolver = 0.0; //variável utilizada para setar o valor x do parametro de solver do objeto. Utilizado na simulação.
	int x, y, z; //posição na matriz
	CBCd3453D * esfera;
	bool cabe; //flag que indicará se a esfera cabe na região sem sobrepor outras esferas.
	std::multimap<int, int> xToObj; // xToObj será uma referência para ordenar os objetos do menor para o maior valor de x.
	std::map<int, CObjetoRedeDePercolacao> matrizObjetosTemp; // Matriz de objetos temporária;
	//============================================== SITIOS =================================================

	std::vector<int> * raios = CriarVetorDeRaiosDosSitios();

	std::cout << "Representando os sítios na matriz sem sobrepor..." << std::endl;
	int im, jm, km;
	int x1 = 0;
	int y1 = ny/2;
	int z1 = nz/2;
	int x_raio, y_raio, z_raio;
	int cont = 0;
	int objeto; //indice do objeto com menor distância entre o ponto 0,0,0 e o centro do objeto (sitio).
	double distancia = 1000000.0; //guarda a menor distancia encontrada  entre o ponto 0,0,0 e o centro dos sítios.
	double distTemp;
	int raio;
	int diametro;
	std::vector<int> objsCamadaOeste;
	std::vector<int> objsCamadaLeste;
	int camadaOeste = 1000000;
	int camadaLeste = 0;
	//percorre o vetor de raios
	for (std::vector<int>::iterator it=raios->begin(); it!=raios->end(); ++it) {
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
						if ( esfera->data3D[i][j][k]!=0 && pm.data3D[im][jm][km]!=0 ) {
							cabe = false;
						}
					}
				}
			}
			if (cabe) { //desenha a esfera
				//std::cout << "Desenhando sítio " << cont << " de " << raios.size() << endl;
				for (int i=0; i<diametro; ++i) {
					im = i+x_raio;
					for (int j=0; j<diametro; ++j) {
						jm = j+y_raio;
						for (int k=0; k<diametro; ++k) {
							km = k+z_raio;
							if ( esfera->data3D[i][j][k]!=0 ) {
								pm.data3D[im][jm][km]=1;
							}
						}
					}
				}
			}
		} while (!cabe);
		// Posso criar os objetos diretamente, em ordem decrescente de tamanho.
		// Outra opção seria, após este loop, rotular os objetos e percorrer a matriz setando cada objeto na matrizObjetos.
		// Assim teria os objetos rotulados de cima para baixo e da esquerda para a direita.
		//matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(SITIO, NumPixeisEsfera(raio));
		matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, SITIO, NumPixeisEsfera(raio));
		matrizObjetosTemp[cont].PontoCentral( x, y, z, 3*raio );
		matrizObjetosTemp[cont].Raio( raio );

		xSolver = (long double)x;
		matrizObjetosTemp[cont].X(xSolver); //seta o x do solver que será utilizado na simulação;

		// Guarda o objeto com menor distancia do ponto 0,0,0.
		distTemp = DistanciaEntrePontos(x1,y1,z1,x,y,z);
		if ( distTemp <= distancia ) {
			distancia = distTemp;
			objeto = cont;
		}
		delete esfera;
	}
	raios->clear(); // limpa vetor de raios (não será mais utilizado)
	delete raios;

	std::cout << "Ordenando os sítios por proximidade..." << std::endl;
	std::map<int, CObjetoRedeDePercolacao>::iterator it;
	std::map<int, CObjetoRedeDePercolacao>::iterator itMatObj;
	cont = 1;
	std::map<int, CObjetoRedeDePercolacao> matrizObjetosSL; // Matriz de objetos sítios e ligações temporária;
	// O primeiro objeto foi encontrado no loop anterior
	matrizObjetosSL[cont] = matrizObjetosTemp[objeto];
	matrizObjetosTemp.erase(objeto);
	// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
	xToObj.insert(pair<int, int>(matrizObjetosSL[cont].PontoCentral_X(),cont));
	while ( matrizObjetosTemp.size() > 0 ) {
		distancia = 1000000.0;
		it = matrizObjetosSL.find(cont);
		//percorre todos objetos em busca do objeto mais próximo ao objeto atual.
		for ( auto & mot : matrizObjetosTemp ) {
			distTemp = DistanciaEntrePontos(it->second.PontoCentral_X(), it->second.PontoCentral_Y(), it->second.PontoCentral_Z(), mot.second.PontoCentral_X(), mot.second.PontoCentral_Y(), mot.second.PontoCentral_Z() );
			if ( distTemp <= distancia ){
				distancia = distTemp;
				objeto = mot.first;
			}
		}
		++cont;
		//std::cout << "Objeto: " << objeto << " | Distancia: " << distancia << std::endl;
		matrizObjetosSL[cont] = matrizObjetosTemp[objeto];
		matrizObjetosTemp.erase(objeto);

		// Pega o sítio e calcula a condutância
		it = matrizObjetosSL.find(cont);
		it->second.Propriedade( CondutanciaSitio(it->second.Raio(),dimensaoPixel,fatorAmplificacao) );

		// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
		xToObj.insert(pair<int, int>(it->second.PontoCentral_X(),cont));

		// Guarda listas de objetos que se encontram na camada leste e oeste (necessário para montar grafo)
		if (it->second.PontoCentral_X() < camadaOeste) {					// Verifica se a camada x do objeto é menor que a atual camanda esquerda
			camadaOeste = it->second.PontoCentral_X();							// Atualiza a atual camada esquerda
			objsCamadaOeste.clear();															// Limpa a lista de objetos pertencentes a camada esquerda
			objsCamadaOeste.push_back(cont);											// Inclui o objeto atual na lista de objetos pentencentes a camada esquerda
		} else if (it->second.PontoCentral_X() == camadaOeste) { // Verifica se a camada x do objeto é igual a atual camada esquerda
			objsCamadaOeste.push_back(cont);											// Inclui o objeto atual na lista de objetos pentencentes a camada esquerda
		} else if (it->second.PontoCentral_X() > camadaLeste) {	// Verifica se a camada x do objeto é maior que a atual camanda direita
			camadaLeste = it->second.PontoCentral_X();							// Atualiza a atual camada direita
			objsCamadaLeste.clear();															// Limpa a lista de objetos pertencentes a camada direita
			objsCamadaLeste.push_back(cont);											// Inclui o objeto atual na lista de objetos pentencentes a camada diretia
		} else if (it->second.PontoCentral_X() == camadaOeste) { // Verifica se a camada x do objeto é igual a atual camada direita
			objsCamadaOeste.push_back(cont);											// Inclui o objeto atual na lista de objetos pentencentes a camada direita
		}
	}

	// Percorre lista de obejos petencentes as camadas leste e oeste, atualizando seus contornos.
	for (int obj : objsCamadaOeste ) {
		matrizObjetosSL[obj].Contorno(CContorno::ETipoContorno::WEST);
	}
	for (int obj : objsCamadaLeste ) {
		matrizObjetosSL[obj].Contorno(CContorno::ETipoContorno::EST);
	}

	//============================================== LIGAÇÕES =================================================
	std::cout << "Calculando distribuicao acumulada (gargantas)." << std::endl;
	long double phiDist = 0.0;	//porosidade da matriz de poros(sitios)
	long double phiRede = 0.0;	//porosidade da matriz de sítios
	long double phiObjeto = 0.0;	//porosidade da esfera (poro/sitio)
	int area = nx*ny*nz; //área da matriz 3D (em pixeis)
	double random;

	int tamVetDist = dtpg.second->distribuicao.size();
	std::vector<double> distAcumulada(tamVetDist+1);
	distAcumulada[0] = dtpg.second->distribuicao[0];
	for (int i=1; i<tamVetDist; ++i) {
		distAcumulada[i] = distAcumulada[i-1] + dtpg.second->distribuicao[i];
	}
	phiDist = dtpg.second->AreaObjetos();
	phiRede = 0.0;
	cabe = false;
	int nCoord, Z;
	int raioit;
	int raioitt;
	long double gSitioLigacao; //Condutância entre sítio e ligação
	int tamMatObjs = matrizObjetosSL.size();
	cont = matrizObjetosSL.rbegin()->first; //índice do último elemento da matriz
	std::map<int, CObjetoRedeDePercolacao>::iterator itt;

	std::cout << "Criando ligacoes..." << std::endl;
	// Durante o loop o tamanho da matrizObjetos será alterado, então, preciso percorrer somente os objetos atuais.
	for ( int obj=1; obj<=tamMatObjs; ++obj ) {
		// Inicialmente cada sítio terá número de coordenação 2.
		if (cabe) {
			nCoord = Random(3,5); // Sorteira números randomicos entre 2 e 4 (corresponderá ao número de coordenação  do sítiios, ou seja, quantas ligações partem dele).
			obj = Random(1,tamMatObjs-1);
		} else {
			nCoord = 2;
		}
		it = matrizObjetosSL.find(obj);
		if (it == matrizObjetosSL.end()) {
			continue;
		}
		itt = it;
		raioit = it->second.Raio();
		for (Z=it->second.NumConexoes()+1; Z<=nCoord; ++Z) {
			do { // Vai para o próximo objeto ainda nao conectado ao objeto atual.
				++itt; //iterator para o próximo objeto.
				if (itt == matrizObjetosSL.end() || itt->first > tamMatObjs) {
					break;
				}
			} while ( it->second.SConexao().find(itt->first) != it->second.SConexao().end() );
			if (itt == matrizObjetosSL.end() || itt->first > tamMatObjs)
				break;
			raioitt = itt->second.Raio();
			// Calcula distância entre os sítios
			distancia = DistanciaEntrePontos(it->second.PontoCentral_X(), it->second.PontoCentral_Y(), it->second.PontoCentral_Z(), itt->second.PontoCentral_X(), itt->second.PontoCentral_Y(), itt->second.PontoCentral_Z() );
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
			//ptrMatObjsRede->matrizObjetos[cont] = CObjetoRedeDePercolacao(LIGACAO,NumPixeisCilindro(raio, distancia));
			matrizObjetosSL[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, LIGACAO, NumPixeisCilindro(raio, distancia));
			itMatObj = matrizObjetosSL.find(cont);
			itMatObj->second.PontoCentral (
						(int)round((it->second.PontoCentral_X()+itt->second.PontoCentral_X())/2),
						(int)round((it->second.PontoCentral_Y()+itt->second.PontoCentral_Y())/2),
						(int)round((it->second.PontoCentral_Z()+itt->second.PontoCentral_Z())/2),
						3*raio
						);
			itMatObj->second.Raio( raio );
			xSolver = (long double)itMatObj->second.PontoCentral_X();
			itMatObj->second.X(xSolver);

			// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
			xToObj.insert(pair<int, int>(itMatObj->second.PontoCentral_X(),cont));

			//Cálculo de condutâncias e realiza conexões
			itMatObj->second.Propriedade( CondutanciaLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao) );
			//primeiro sítio
			gSitioLigacao = CondutanciaSitioLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
			itMatObj->second.Conectar(itt->first, gSitioLigacao);
			itt->second.Conectar(cont, gSitioLigacao);
			//segundo sítio
			gSitioLigacao = CondutanciaSitioLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
			itMatObj->second.Conectar(it->first, gSitioLigacao);
			it->second.Conectar(cont, gSitioLigacao);

			if (phiRede >= phiDist)
				break;
		}
		//std::cout << "Aqui4!" << std::endl;
		if (phiRede >= phiDist)
			break;
		if ( obj==tamMatObjs && phiRede < phiDist ) {
			//exit;
			cabe = true;
			obj = 1;
		}
	}
	//ptrMatObjsRede->matrizObjetos.erase(0);//apaga o objeto
	numLigacoes = cont-tamMatObjs;
	std::cout << "Ligacoes criadas!\nphiRede: " << phiRede << " | phiDist: " << phiDist << " | Num. Ligacoes: " << numLigacoes << std::endl;
	std::cout << cont << " objetos criados!" << std::endl;

	std::cout << "Ordenado os objetos (sítios e ligações) pelo eixo x ..." << std::endl;
	std::map<int,int> objAntToObjAtual;
	ptrMatObjsRede->matrizObjetos.clear(); // Matriz de objetos final
	cont = 0;
	std::cout << "Copia os objetos, já ordenados em x, da matriz temporária para a metriz definitiva..." << std::endl;
	/*
	for (auto xto : matrizObjetosSL ) {
		ptrMatObjsRede->matrizObjetos[xto.first] = xto.second;
	}
*/
	for (auto &xto : xToObj ) {
		++cont;
		ptrMatObjsRede->matrizObjetos[cont] = matrizObjetosSL[xto.second];
		objAntToObjAtual[xto.second] = cont;
		//std::cout << "[x=" << xto.first << ",\t\t\tObjAnt=" << xto.second<< ",\t\t\tObjAtu=" << cont << "]" << std::endl;
	}
	matrizObjetosSL.clear();

	//percorre a matriz de objetos e atualiza os rótulos dos objetos conectados
	std::map<int, double> conexoesAtu;
	//std::map<int, double> conexoesAnt;
	for ( auto &objs : ptrMatObjsRede->matrizObjetos ) {
		//copia as conexões para o map temporário
		conexoesAtu.clear();
		//conexoesAnt = objs.second.SConexao();
		//for ( auto obj : conexoesAnt ) { //a condutância premanece a mesma, só mudou o rótulo do objeto
		for ( auto &obj : objs.second.SConexao() ) { //a condutância premanece a mesma, só mudou o rótulo do objeto
			conexoesAtu.insert(make_pair(objAntToObjAtual[obj.first], obj.second));
		}
		//limpa o vetor de conexoes do objeto
		//conexoesAnt.clear();
		objs.second.SConexao().clear();

		//copia para os objetos as novas conexões salvas no map temporário
		for ( auto &obj : conexoesAtu ) {
			//conexoesAnt.insert(make_pair(obj.first, obj.second));
			objs.second.Conectar(obj.first, obj.second);
		}
	}

	objAntToObjAtual.clear();
	xToObj.clear();
	return true;
}

// Cria rede de percolação com sítios em posições e tamanhos aleatórios com variação do número de ligações.
// Este modelo primeiro aloca uma porcentagem dos sítios nas fronteiras para depois alocar o restante dos sítios
bool CRedeDePercolacao::Modelo2() {
	std::cout << "Criando rede atraves do Modelo 2..." << std::endl;
	TCMatriz3D<bool> pm(nx, ny, nz);
	int area = nx*ny*nz; //área da matriz 3D (em pixeis)
	long double phiGargantas	= dtpg.second->AreaObjetos(); //porosidade da imagem (garganta)
	long double phiLigacoes		= 0.0; //porosidade da rede (licações)
	long double phiObjeto			= 0.0; //porosidade do objeto que esta sendo criado (esfera/sítio, cilindro/ligação)
	long double xSolver				= 0.0; //variável utilizada para setar o valor x do parametro de solver do objeto. Utilizado na simulação.
	int x, y, z; //posição na matriz
	CBCd3453D * esfera;
	bool cabe; //flag que indicará se a esfera cabe na região sem sobrepor outras esferas.
	std::multimap<int, int> xToObj; // xToObj será uma referência para ordenar os objetos do menor para o maior valor de x.

	//============================================== SITIOS =================================================
	//std::vector<int> * raios = CriarVetorDeRaiosDosSitiosByDTP();
	std::vector<int> * raios = CriarVetorDeRaiosDosSitios();
	std::cout << "Representando os sítios na matriz sem sobrepor..." << std::endl;
	int im, jm, km;
	int x_raio, y_raio, z_raio;
	int cont = 0;
	int objeto; //indice do objeto com menor distância entre o ponto 0,0,0 e o centro do objeto (sitio).
	double distancia = 1000000.0; //guarda a menor distancia encontrada  entre o ponto 0,0,0 e o centro dos sítios.
	double distTemp;
	int maiorRaioEsquerda = 0;
	int maiorRaioDireita = 0;
	int raio;
	int diametro;
	std::map<int, CObjetoRedeDePercolacao> matrizObjetosTemp; // Matriz de objetos temporária;

	{	//============================================== FRONTEIRAS =================================================
		double phiTotal = dtpg.first->AreaObjetos() + phiGargantas;
		double phiFronteiraEsquerda = 0.0;
		double phiFronteiraDireita = 0.0;
		int areaFronteira = ny*nz;
		int pos = 0;
		//============================================== ESQUERDA =================================================
		// sorteando sítios da fronteira esquerda
		std::vector<int> raiosFronteiraEsquerda;
		while ( phiFronteiraEsquerda < phiTotal ) {
			pos = Random(0,raios->size()-1); // Sorteira uma posição no vetor de raios
			raio = raios->at(pos); //pega o raio do elemento sorteado
			raiosFronteiraEsquerda.push_back(raio); //Armazena o raio no vetor
			phiFronteiraEsquerda += PhiDisco(raio, areaFronteira); //Acumula a porosidade
			raios->erase(raios->begin()+pos); //Apaga do vetor de raios o raio já utilizado
			if (raio > maiorRaioEsquerda) { //Guarda o maior raio sorteado
				maiorRaioEsquerda = raio;
			}
		}
		std::cout << "PhiFronteiraEsquerda= " << phiFronteiraEsquerda << " PhiTotal= " << phiTotal << std::endl;
		// Ordenar os raios do maior para o menor de forma que as esferas maiores serão criadas primeiro.
		std::sort(raiosFronteiraEsquerda.begin(), raiosFronteiraEsquerda.end());
		std::reverse(raiosFronteiraEsquerda.begin(), raiosFronteiraEsquerda.end());

		// aloca os sítios da fronteira esquerda
		x = maiorRaioEsquerda;
		for (auto &raio : raiosFronteiraEsquerda) {
			diametro = ((2*raio)+1);
			//cria esfera de raio correspondente.
			esfera = new CBCd3453D(diametro);
			// Sortear posições testando se a esfera cabe sem sobrepor outras esferas ou ultrapassar a borda.
			do {
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
							if ( esfera->data3D[i][j][k]!=0 && pm.data3D[im][jm][km]!=0 ) {
								cabe = false;
							}
						}
					}
				}
				if (cabe) { //desenha a esfera
					//std::cout << "Desenhando sítio " << cont << " de " << raios.size() << endl;
					for (int i=0; i<diametro; ++i) {
						im = i+x_raio;
						for (int j=0; j<diametro; ++j) {
							jm = j+y_raio;
							for (int k=0; k<diametro; ++k) {
								km = k+z_raio;
								if ( esfera->data3D[i][j][k]!=0 ) {
									pm.data3D[im][jm][km]=1;
								}
							}
						}
					}
				}
			} while (!cabe);
			// aloca na matriz de objetos o sítio criado
			++cont;
			matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, SITIO, NumPixeisEsfera(raio));
			matrizObjetosTemp[cont].Contorno(CContorno::ETipoContorno::WEST);
			matrizObjetosTemp[cont].PontoCentral( x, y, z, 3*raio );
			matrizObjetosTemp[cont].Raio( raio );
			xSolver = (long double)x;
			matrizObjetosTemp[cont].X(xSolver); //seta o x do solver que será utilizado na simulação;
			matrizObjetosTemp[cont].Propriedade( CondutanciaSitio(matrizObjetosTemp[cont].Raio(), dimensaoPixel, fatorAmplificacao) );
			delete esfera;
		}
		raiosFronteiraEsquerda.clear();

		//============================================== DIREITA =================================================
		// sorteando sítios da fronteira direita
		std::vector<int> raiosFronteiraDireita;
		while ( phiFronteiraDireita < phiTotal ) {
			pos = Random(0,raios->size()-1); // Sorteira uma posição no vetor de raios
			raio = raios->at(pos); //pega o raio do elemento sorteado
			raiosFronteiraDireita.push_back(raio); //Armazena o raio no vetor
			phiFronteiraDireita += PhiDisco(raio, areaFronteira); //Acumula a porosidade
			raios->erase(raios->begin()+pos); //Apaga do vetor de raios o raio já utilizado
			if (raio > maiorRaioDireita) { //Guarda o maior raio sorteado
				maiorRaioDireita = raio;
			}
		}
		std::cout << "PhiFronteiraDireita= " << phiFronteiraDireita << " PhiTotal= " << phiTotal << std::endl;
		// Ordenar os raios do maior para o menor de forma que as esferas maiores serão criadas primeiro.
		std::sort(raiosFronteiraDireita.begin(), raiosFronteiraDireita.end());
		std::reverse(raiosFronteiraDireita.begin(), raiosFronteiraDireita.end());

		// aloca os sítios da fronteira direira
		x = nx-maiorRaioDireita-1;
		for (auto &raio : raiosFronteiraDireita) {
			diametro = ((2*raio)+1);
			//cria esfera de raio correspondente.
			esfera = new CBCd3453D(diametro);
			// Sortear posições testando se a esfera cabe sem sobrepor outras esferas ou ultrapassar a borda.
			do {
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
							if ( esfera->data3D[i][j][k]!=0 && pm.data3D[im][jm][km]!=0 ) {
								cabe = false;
							}
						}
					}
				}
				if (cabe) { //desenha a esfera
					//std::cout << "Desenhando sítio " << cont << " de " << raios.size() << endl;
					for (int i=0; i<diametro; ++i) {
						im = i+x_raio;
						for (int j=0; j<diametro; ++j) {
							jm = j+y_raio;
							for (int k=0; k<diametro; ++k) {
								km = k+z_raio;
								if ( esfera->data3D[i][j][k]!=0 ) {
									pm.data3D[im][jm][km]=1;
								}
							}
						}
					}
				}
			} while (!cabe);
			// aloca na matriz de objetos o sítio criado
			++cont;
			matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, SITIO, NumPixeisEsfera(raio));
			matrizObjetosTemp[cont].Contorno(CContorno::ETipoContorno::EST);
			matrizObjetosTemp[cont].PontoCentral( x, y, z, 3*raio );
			matrizObjetosTemp[cont].Raio( raio );
			xSolver = (long double)x;
			matrizObjetosTemp[cont].X(xSolver); //seta o x do solver que será utilizado na simulação;
			matrizObjetosTemp[cont].Propriedade( CondutanciaSitio(matrizObjetosTemp[cont].Raio(), dimensaoPixel, fatorAmplificacao) );
			delete esfera;
		}
		raiosFronteiraDireita.clear();
	}
	//============================================== CENTRO =================================================
	// Agora aloca os demais sitos que restaram no vetor de raios
	for ( auto &raio : *raios ) {
		diametro = ((2*raio)+1);
		//cria esfera de raio correspondente.
		esfera = new CBCd3453D(diametro);
		// Sortear posições testando se a esfera cabe sem sobrepor outras esferas ou ultrapassar a borda.
		do {
			x = Random(maiorRaioEsquerda+1, nx-maiorRaioDireita-2);
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
						if ( esfera->data3D[i][j][k]!=0 && pm.data3D[im][jm][km]!=0 ) {
							cabe = false;
						}
					}
				}
			}
			if (cabe) { //desenha a esfera
				//std::cout << "Desenhando sítio " << cont << " de " << raios.size() << endl;
				for (int i=0; i<diametro; ++i) {
					im = i+x_raio;
					for (int j=0; j<diametro; ++j) {
						jm = j+y_raio;
						for (int k=0; k<diametro; ++k) {
							km = k+z_raio;
							if ( esfera->data3D[i][j][k]!=0 ) {
								pm.data3D[im][jm][km]=1;
							}
						}
					}
				}
			}
		} while (!cabe);
		// aloca na matriz de objetos o sítio criado
		++cont;
		matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, SITIO, NumPixeisEsfera(raio));
		matrizObjetosTemp[cont].Contorno(CContorno::ETipoContorno::CENTER);
		matrizObjetosTemp[cont].PontoCentral( x, y, z, 3*raio );
		matrizObjetosTemp[cont].Raio ( raio );
		xSolver = (long double)x;
		matrizObjetosTemp[cont].X(xSolver); //seta o x do solver que será utilizado na simulação;
		matrizObjetosTemp[cont].Propriedade( CondutanciaSitio(matrizObjetosTemp[cont].Raio(), dimensaoPixel, fatorAmplificacao) );
		delete esfera;
	}
	raios->clear(); // limpa vetor de raios (não será mais utilizado)
	delete raios;

	std::cout << "Ordenando os sítios por proximidade..." << std::endl;
	std::map<int, CObjetoRedeDePercolacao>::iterator it;
	std::map<int, CObjetoRedeDePercolacao>::iterator itMatObj;
	std::map<int, CObjetoRedeDePercolacao> matrizObjetosSL; // Matriz de objetos sítios e ligações temporária;
	cont = 1;
	matrizObjetosSL[cont] = matrizObjetosTemp[1];
	matrizObjetosTemp.erase(1);
	// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
	xToObj.insert(pair<int, int>(matrizObjetosSL[cont].PontoCentral_X(),cont));
	while ( matrizObjetosTemp.size() > 0 ) {
		distancia = 1000000.0;
		it = matrizObjetosSL.find(cont);
		//percorre todos objetos em busca do objeto mais próximo ao objeto atual.
		for ( auto & mot : matrizObjetosTemp ) {
			distTemp = DistanciaEntrePontos(it->second.PontoCentral_X(), it->second.PontoCentral_Y(), it->second.PontoCentral_Z(), mot.second.PontoCentral_X(), mot.second.PontoCentral_Y(), mot.second.PontoCentral_Z() );
			if ( distTemp <= distancia ){
				distancia = distTemp;
				objeto = mot.first;
			}
		}
		++cont;
		//std::cout << "Objeto: " << objeto << " | Distancia: " << distancia << std::endl;
		matrizObjetosSL[cont] = matrizObjetosTemp[objeto];
		matrizObjetosTemp.erase(objeto);

		// Pega o sítio
		it = matrizObjetosSL.find(cont);
		// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
		xToObj.insert(pair<int, int>(it->second.PontoCentral_X(),cont));
	}

	//============================================== LIGAÇÕES =================================================
	std::cout << "Calculando distribuicao acumulada (gargantas)." << std::endl;
	int tamVetDist = dtpg.second->distribuicao.size();
	std::vector<double> distGargantasAcumulada(tamVetDist+1);
	distGargantasAcumulada[0] = dtpg.second->distribuicao[0];
	for (int i=1; i<tamVetDist; ++i) {
		distGargantasAcumulada[i] = distGargantasAcumulada[i-1] + dtpg.second->distribuicao[i];
	}
	cabe = false;
	bool interligados = false;
	int nCoord, Z;
	int raioit;
	int raioitt;
	long double gSitioLigacao; //Condutância entre sítio e ligação
	double random;
	int tamMatObjs = matrizObjetosSL.size();
	cont = matrizObjetosSL.rbegin()->first; //índice do último elemento da matriz
	std::map<int, CObjetoRedeDePercolacao>::iterator itt;
	std::map<int, CObjetoRedeDePercolacao>::iterator itc;

	std::cout << "Criando ligacoes..." << std::endl;
	// Durante o loop o tamanho da matrizObjetos será alterado, então, preciso percorrer somente os objetos atuais.
	for ( int obj=1; obj<=tamMatObjs; ++obj ) {
		// Inicialmente cabe==false, então, cada sítio terá número de coordenação 2.
		if (cabe) {
			obj = Random(1,tamMatObjs-1); //pega sítios aleatórios.
		}
		it = matrizObjetosSL.find(obj);
		if (it == matrizObjetosSL.end()) {
			continue;
		}
		itt = it;
		raioit = it->second.Raio();
		// Se todos os sítios já foram interligados, é preciso garantir que irá entrar no proximo loop!
		// Se cabe==true, é porque todos os sítios já foram interligados, ou,
		// se Z>2 é porque o objeto já possui pelo menos duas conexões, mas poderá ser conectado a objeto mais a frente.
		// Então, o valor de nCoord a ser setado será igual ao numero de conexões existentes no objeto + 1.
		// Senão, o valor de nCoord será 2.
		Z = it->second.NumConexoes()+1;
		if (cabe || Z > 2) {
			nCoord = Z;
		} else {
			nCoord = 2;
		}
		std::cout << "Trabalhando com o sitio " << obj << ".\t\t\tSerao criadas " << nCoord << " ligacoes!" << std::endl;
		for (Z=it->second.NumConexoes()+1; Z<=nCoord; ++Z) {
			std::cout << "Procurando proximo objeto ainda nao conectado..." << std::endl;
			/*
			do { // Vai para o próximo objeto ainda não conectado ao objeto atual.
				++itt; //iterator para o próximo objeto.
				if (itt == matrizObjetosSL.end() || itt->first > tamMatObjs) {
					break;
				}
			} while ( it->second.SConexao().find(itt->first) != it->second.SConexao().end() );
			*/
			while (true) { // Vai para o próximo objeto ainda não conectado ao objeto atual.
				interligados = false;
				++itt; //iterator para o próximo objeto.
				if (itt == matrizObjetosSL.end() || itt->first > tamMatObjs) {
					break;
				}
				for (auto &c: it->second.SConexao()) { //para cada ligação já conectada ao primeiro sítio
					itc = matrizObjetosSL.find(c.first); //pega o iterator para a ligação
					for (auto &o: itc->second.SConexao()) { //percorre a lista de objetos conectados a ligação
						if ( o.first == itt->first ) { //verifica se o primerio sítio está conectado ao segundo através da ligação
							interligados = true;
							break; //se estiverem interligado, não precisa mais procurar.
						}
					}
					if(interligados) {//se estiverem interligado, não precisa mais procurar.
						break; //sai do loop de conexões
					}
				}
				if ( ! interligados) {
					break; //sai do loop geral quando encontrar o segundo sítio ainda não interligado ao primeiro
				}
			}
			if (itt == matrizObjetosSL.end() || itt->first > tamMatObjs || interligados)
				break;

			raioitt = itt->second.Raio();
			// Calcula distância entre os sítios
			distancia = DistanciaEntrePontos(it->second.PontoCentral_X(), it->second.PontoCentral_Y(), it->second.PontoCentral_Z(), itt->second.PontoCentral_X(), itt->second.PontoCentral_Y(), itt->second.PontoCentral_Z() );
			distancia = distancia - it->second.Raio() - itt->second.Raio();
			if ( distancia < 1 ) // Caso os sítios se toquem, a distância dará 0, então força que seja pelo menos 1
				distancia = 1;
			std::cout << "Tamanho da ligacao: " << distancia << std::endl;
			// Sortear valores aleatórios entre 0 e 1. Obter o raio na distGargantasAcumulada
			raio = 1;
			random = DRandom(); //obtem valor double randômico entre 0.0 e 1.0;
			//percorre vetor de distribuição acumulada para obter raio correspondente
			for (int i=0; i<tamVetDist; ++i) {
				if ( random <= distGargantasAcumulada[i] ) {
					if (i > 0) { //aqui o valor sorteado é menor ou igual e não estamos no primeiro elemento do vetor
						// Verifica a diferença entre o número randômico e os elementas i e i-1.
						// Seta o raio com o indice do valor mais próximo a random.
						if ( (random - distGargantasAcumulada[i-1]) < (distGargantasAcumulada[i] - random) ) {
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
			// Aqui: Este procedimento está alterando muito a distribuição de ligações!
			while ( (raio >= raioit || raio >= raioitt) && raio > 1 ) {
				--raio;
			}

			//std::cout << "Raio da ligacao: " << raio << std::endl;

			//calcular a porosidade correspondente a ligação (cilindro) que será criada com o raio sorteado.
			//phiObjeto = ((M_PI * (double)raio * (double)raio * distancia)/(double)area)*100.0;
			//phiObjeto = ((double)numPixeisDisco[raio]*(double)distancia/(double)area)*100.0;
			phiObjeto = PhiCilindro(raio, distancia, area);
			//Se a soma das porosidades for maior que a porosidade da matriz de gargantas e o raio for maior que 1,
			//decrementa o raio até que a soma das porosidades seja menor que a porosidade da matriz de gargantas, ou ate que o raio seja 1.
			//Ao sair do loop, incrementa o raio e recalcula a area do cilindro, de modo que phiLigacoes fique o mais próximo possível de phiGargantas.
			if ( phiLigacoes+phiObjeto > phiGargantas && raio > 1) {
				while( phiLigacoes+phiObjeto > phiGargantas && raio > 1) {
					--raio;
					phiObjeto = PhiCilindro(raio, distancia, area);
				}
				++raio;
				phiObjeto = PhiCilindro(raio, distancia, area);
			}
			phiLigacoes += phiObjeto; //acumula a porosidade
			++cont;
			// Conecta os objetos
			//ptrMatObjsRede->matrizObjetos[cont] = CObjetoRedeDePercolacao(LIGACAO,NumPixeisCilindro(raio, distancia));
			matrizObjetosSL[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, LIGACAO, NumPixeisCilindro(raio, distancia));
			itMatObj = matrizObjetosSL.find(cont);
			itMatObj->second.PontoCentral(
						(int)round((it->second.PontoCentral_X()+itt->second.PontoCentral_X())/2),
						(int)round((it->second.PontoCentral_Y()+itt->second.PontoCentral_Y())/2),
						(int)round((it->second.PontoCentral_Z()+itt->second.PontoCentral_Z())/2),
						3*raio
						);
			itMatObj->second.Raio( raio );
			xSolver = (long double)itMatObj->second.PontoCentral_X();
			itMatObj->second.X(xSolver);

			// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
			xToObj.insert(pair<int, int>(itMatObj->second.PontoCentral_X(),cont));

			//Cálculo de condutâncias e realiza conexões
			itMatObj->second.Propriedade ( CondutanciaLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao) );
			//primeiro sítio
			gSitioLigacao = CondutanciaSitioLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
			itMatObj->second.Conectar(itt->first, gSitioLigacao);
			itt->second.Conectar(cont, gSitioLigacao);
			//segundo sítio
			gSitioLigacao = CondutanciaSitioLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
			itMatObj->second.Conectar(it->first, gSitioLigacao);
			it->second.Conectar(cont, gSitioLigacao);

			if (phiLigacoes >= phiGargantas)
				break;
		}
		//std::cout << "Aqui4!" << std::endl;
		if (phiLigacoes >= phiGargantas)
			break;
		// Se conectou o último sítio e ainda precisa criar mais ligações, volta para o primeiro sítio e seta cabe=true;
		// Assim serão criadas ligações aleatórias...
		if ( obj==tamMatObjs && phiLigacoes < phiGargantas ) {
			//exit;
			cabe = true;
			obj = 1;
		}
	}
	//ptrMatObjsRede->matrizObjetos.erase(0);//apaga o objeto
	numLigacoes = cont-tamMatObjs;
	std::cout << "Ligacoes criadas!\nphiLigacoes: " << phiLigacoes << " | phiGargantas: " << phiGargantas << " | Num. Ligacoes: " << numLigacoes << std::endl;
	std::cout << cont << " objetos criados!" << std::endl;

	std::cout << "Ordenado os objetos (sítios e ligações) pelo eixo x ..." << std::endl;
	std::map<int,int> objAntToObjAtual;
	ptrMatObjsRede->matrizObjetos.clear(); // Matriz de objetos final
	cont = 0;
	std::cout << "Copia os objetos, já ordenados em x, da matriz temporária para a metriz definitiva..." << std::endl;
	/*
	for (auto xto : matrizObjetosSL ) {
		ptrMatObjsRede->matrizObjetos[xto.first] = xto.second;
	}
*/
	for (auto &xto : xToObj ) {
		++cont;
		ptrMatObjsRede->matrizObjetos[cont] = matrizObjetosSL[xto.second];
		objAntToObjAtual[xto.second] = cont;
		//std::cout << "[x=" << xto.first << ",\t\t\tObjAnt=" << xto.second<< ",\t\t\tObjAtu=" << cont << "]" << std::endl;
	}
	matrizObjetosSL.clear();

	//percorre a matriz de objetos e atualiza os rótulos dos objetos conectados
	std::map<int, double> conexoesAtu;
	//std::map<int, double> conexoesAnt;
	for ( auto &objs : ptrMatObjsRede->matrizObjetos ) {
		//copia as conexões para o map temporário
		conexoesAtu.clear();
		//conexoesAnt = objs.second.SConexao();
		//for ( auto obj : conexoesAnt ) { //a condutância premanece a mesma, só mudou o rótulo do objeto
		for ( auto &obj : objs.second.SConexao() ) { //a condutância premanece a mesma, só mudou o rótulo do objeto
			conexoesAtu.insert(make_pair(objAntToObjAtual[obj.first], obj.second));
		}
		//limpa o vetor de conexoes do objeto
		//conexoesAnt.clear();
		objs.second.SConexao().clear();

		//copia para os objetos as novas conexões salvas no map temporário
		for ( auto &obj : conexoesAtu ) {
			//conexoesAnt.insert(make_pair(obj.first, obj.second));
			objs.second.Conectar(obj.first, obj.second);
		}
	}

	objAntToObjAtual.clear();
	xToObj.clear();
	return true;
}

// Cria rede de percolação com sítios em posições e tamanhos aleatórios com variação do número de ligações.
// Semelhante ao ModeloDois, porém, difere na escolha dos sítios que serão interligados,
// de forma que não altere a distribuição de ligações.
bool CRedeDePercolacao::Modelo3() {
	std::cout << "Criando rede atraves do Modelo 3..." << std::endl;
	TCMatriz3D<bool> pm(nx, ny, nz);
	int area = nx*ny*nz; //área da matriz 3D (em pixeis)
	long double phiGargantas	= dtpg.second->AreaObjetos(); //porosidade da imagem (garganta)
	long double phiLigacoes		= 0.0; //porosidade da rede (licações)
	long double phiObjeto			= 0.0; //porosidade do objeto que esta sendo criado (esfera/sítio, cilindro/ligação)
	long double xSolver				= 0.0; //variável utilizada para setar o valor x do parametro de solver do objeto. Utilizado na simulação.
	int x, y, z; //posição na matriz
	CBCd3453D * esfera;
	bool cabe; //flag que indicará se a esfera cabe na região sem sobrepor outras esferas.
	std::multimap<int, int> xToObj; // xToObj será uma referência para ordenar os objetos do menor para o maior valor de x.

	//============================================== SITIOS =================================================
	std::vector<int> * raios = CriarVetorDeRaiosDosSitiosByDTP();
	std::cout << "Representando os sítios na matriz sem sobrepor..." << std::endl;
	int im, jm, km;
	int x_raio, y_raio, z_raio;
	int cont = 0;
	int objeto; //indice do objeto com menor distância entre o ponto 0,0,0 e o centro do objeto (sitio).
	double distancia = 1000000.0; //guarda a menor distancia encontrada  entre o ponto 0,0,0 e o centro dos sítios.
	double distTemp;
	int maiorRaioEsquerda = 0;
	int maiorRaioDireita = 0;
	int raio;
	int diametro;
	std::map<int, CObjetoRedeDePercolacao> matrizObjetosTemp; // Matriz de objetos temporária;

	{	//============================================== FRONTEIRAS =================================================
		double phiTotal = dtpg.first->AreaObjetos() + phiGargantas;
		double phiFronteiraEsquerda = 0.0;
		double phiFronteiraDireita = 0.0;
		int areaFronteira = ny*nz;
		int pos = 0;
		//============================================== ESQUERDA =================================================
		// sorteando sítios da fronteira esquerda
		std::vector<int> raiosFronteiraEsquerda;
		while ( phiFronteiraEsquerda < phiTotal ) {
			pos = Random(0,raios->size()-1); // Sorteira uma posição no vetor de raios
			raio = raios->at(pos); //pega o raio do elemento sorteado
			raiosFronteiraEsquerda.push_back(raio); //Armazena o raio no vetor
			phiFronteiraEsquerda += PhiDisco(raio, areaFronteira); //Acumula a porosidade
			raios->erase(raios->begin()+pos); //Apaga do vetor de raios o raio já utilizado
			if (raio > maiorRaioEsquerda) { //Guarda o maior raio sorteado
				maiorRaioEsquerda = raio;
			}
		}
		std::cout << "PhiFronteiraEsquerda= " << phiFronteiraEsquerda << " PhiTotal= " << phiTotal << std::endl;
		// Ordenar os raios do maior para o menor de forma que as esferas maiores serão criadas primeiro.
		std::sort(raiosFronteiraEsquerda.begin(), raiosFronteiraEsquerda.end());
		std::reverse(raiosFronteiraEsquerda.begin(), raiosFronteiraEsquerda.end());

		// aloca os sítios da fronteira esquerda
		x = maiorRaioEsquerda;
		for (auto &raio : raiosFronteiraEsquerda) {
			diametro = ((2*raio)+1);
			//cria esfera de raio correspondente.
			esfera = new CBCd3453D(diametro);
			// Sortear posições testando se a esfera cabe sem sobrepor outras esferas ou ultrapassar a borda.
			do {
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
							if ( esfera->data3D[i][j][k]!=0 && pm.data3D[im][jm][km]!=0 ) {
								cabe = false;
							}
						}
					}
				}
				if (cabe) { //desenha a esfera
					//std::cout << "Desenhando sítio " << cont << " de " << raios.size() << endl;
					for (int i=0; i<diametro; ++i) {
						im = i+x_raio;
						for (int j=0; j<diametro; ++j) {
							jm = j+y_raio;
							for (int k=0; k<diametro; ++k) {
								km = k+z_raio;
								if ( esfera->data3D[i][j][k]!=0 ) {
									pm.data3D[im][jm][km]=1;
								}
							}
						}
					}
				}
			} while (!cabe);
			// aloca na matriz de objetos o sítio criado
			++cont;
			matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, SITIO, NumPixeisEsfera(raio));
			matrizObjetosTemp[cont].Contorno(CContorno::ETipoContorno::WEST);
			matrizObjetosTemp[cont].PontoCentral( x, y, z, 3*raio );
			matrizObjetosTemp[cont].Raio( raio );
			xSolver = (long double)x;
			matrizObjetosTemp[cont].X(xSolver); //seta o x do solver que será utilizado na simulação;
			matrizObjetosTemp[cont].Propriedade( CondutanciaSitio(matrizObjetosTemp[cont].Raio(), dimensaoPixel, fatorAmplificacao) );
			delete esfera;
		}
		raiosFronteiraEsquerda.clear();

		//============================================== DIREITA =================================================
		// sorteando sítios da fronteira direita
		std::vector<int> raiosFronteiraDireita;
		while ( phiFronteiraDireita < phiTotal ) {
			pos = Random(0,raios->size()-1); // Sorteira uma posição no vetor de raios
			raio = raios->at(pos); //pega o raio do elemento sorteado
			raiosFronteiraDireita.push_back(raio); //Armazena o raio no vetor
			phiFronteiraDireita += PhiDisco(raio, areaFronteira); //Acumula a porosidade
			raios->erase(raios->begin()+pos); //Apaga do vetor de raios o raio já utilizado
			if (raio > maiorRaioDireita) { //Guarda o maior raio sorteado
				maiorRaioDireita = raio;
			}
		}
		std::cout << "PhiFronteiraDireita= " << phiFronteiraDireita << " PhiTotal= " << phiTotal << std::endl;
		// Ordenar os raios do maior para o menor de forma que as esferas maiores serão criadas primeiro.
		std::sort(raiosFronteiraDireita.begin(), raiosFronteiraDireita.end());
		std::reverse(raiosFronteiraDireita.begin(), raiosFronteiraDireita.end());

		// aloca os sítios da fronteira direira
		x = nx-maiorRaioDireita-1;
		for (auto &raio : raiosFronteiraDireita) {
			diametro = ((2*raio)+1);
			//cria esfera de raio correspondente.
			esfera = new CBCd3453D(diametro);
			// Sortear posições testando se a esfera cabe sem sobrepor outras esferas ou ultrapassar a borda.
			do {
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
							if ( esfera->data3D[i][j][k]!=0 && pm.data3D[im][jm][km]!=0 ) {
								cabe = false;
							}
						}
					}
				}
				if (cabe) { //desenha a esfera
					//std::cout << "Desenhando sítio " << cont << " de " << raios.size() << endl;
					for (int i=0; i<diametro; ++i) {
						im = i+x_raio;
						for (int j=0; j<diametro; ++j) {
							jm = j+y_raio;
							for (int k=0; k<diametro; ++k) {
								km = k+z_raio;
								if ( esfera->data3D[i][j][k]!=0 ) {
									pm.data3D[im][jm][km]=1;
								}
							}
						}
					}
				}
			} while (!cabe);
			// aloca na matriz de objetos o sítio criado
			++cont;
			matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, SITIO, NumPixeisEsfera(raio));
			matrizObjetosTemp[cont].Contorno(CContorno::ETipoContorno::EST);
			matrizObjetosTemp[cont].PontoCentral( x, y, z, 3*raio );
			matrizObjetosTemp[cont].Raio( raio );
			xSolver = (long double)x;
			matrizObjetosTemp[cont].X(xSolver); //seta o x do solver que será utilizado na simulação;
			matrizObjetosTemp[cont].Propriedade( CondutanciaSitio(matrizObjetosTemp[cont].Raio(), dimensaoPixel, fatorAmplificacao) );
			delete esfera;
		}
		raiosFronteiraDireita.clear();
	}
	//============================================== CENTRO =================================================
	// Agora aloca os demais sitos que restaram no vetor de raios
	for ( auto &raio : *raios ) {
		diametro = ((2*raio)+1);
		//cria esfera de raio correspondente.
		esfera = new CBCd3453D(diametro);
		// Sortear posições testando se a esfera cabe sem sobrepor outras esferas ou ultrapassar a borda.
		do {
			x = Random(maiorRaioEsquerda+1, nx-maiorRaioDireita-2);
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
						if ( esfera->data3D[i][j][k]!=0 && pm.data3D[im][jm][km]!=0 ) {
							cabe = false;
						}
					}
				}
			}
			if (cabe) { //desenha a esfera
				//std::cout << "Desenhando sítio " << cont << " de " << raios.size() << endl;
				for (int i=0; i<diametro; ++i) {
					im = i+x_raio;
					for (int j=0; j<diametro; ++j) {
						jm = j+y_raio;
						for (int k=0; k<diametro; ++k) {
							km = k+z_raio;
							if ( esfera->data3D[i][j][k]!=0 ) {
								pm.data3D[im][jm][km]=1;
							}
						}
					}
				}
			}
		} while (!cabe);
		// aloca na matriz de objetos o sítio criado
		++cont;
		matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, SITIO, NumPixeisEsfera(raio));
		matrizObjetosTemp[cont].Contorno(CContorno::ETipoContorno::CENTER);
		matrizObjetosTemp[cont].PontoCentral( x, y, z, 3*raio );
		matrizObjetosTemp[cont].Raio ( raio );
		xSolver = (long double)x;
		matrizObjetosTemp[cont].X(xSolver); //seta o x do solver que será utilizado na simulação;
		matrizObjetosTemp[cont].Propriedade( CondutanciaSitio(matrizObjetosTemp[cont].Raio(), dimensaoPixel, fatorAmplificacao) );
		delete esfera;
	}
	raios->clear(); // limpa vetor de raios (não será mais utilizado)
	delete raios;

	std::cout << "Ordenando os sítios por proximidade..." << std::endl;
	std::map<int, CObjetoRedeDePercolacao>::iterator it;
	std::map<int, CObjetoRedeDePercolacao>::iterator itMatObj;
	std::map<int, CObjetoRedeDePercolacao> matrizObjetosSL; // Matriz de objetos sítios e ligações temporária;
	cont = 1;
	matrizObjetosSL[cont] = matrizObjetosTemp[1];
	matrizObjetosTemp.erase(1);
	// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
	xToObj.insert(pair<int, int>(matrizObjetosSL[cont].PontoCentral_X(),cont));
	while ( matrizObjetosTemp.size() > 0 ) {
		distancia = 1000000.0;
		it = matrizObjetosSL.find(cont);
		//percorre todos objetos em busca do objeto mais próximo ao objeto atual.
		for ( auto & mot : matrizObjetosTemp ) {
			distTemp = DistanciaEntrePontos(it->second.PontoCentral_X(), it->second.PontoCentral_Y(), it->second.PontoCentral_Z(), mot.second.PontoCentral_X(), mot.second.PontoCentral_Y(), mot.second.PontoCentral_Z() );
			if ( distTemp <= distancia ){
				distancia = distTemp;
				objeto = mot.first;
			}
		}
		++cont;
		//std::cout << "Objeto: " << objeto << " | Distancia: " << distancia << std::endl;
		matrizObjetosSL[cont] = matrizObjetosTemp[objeto];
		matrizObjetosTemp.erase(objeto);

		// Pega o sítio
		it = matrizObjetosSL.find(cont);
		// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
		xToObj.insert(pair<int, int>(it->second.PontoCentral_X(),cont));
	}

	//============================================== LIGAÇÕES =================================================
	std::cout << "Calculando distribuicao acumulada (gargantas)." << std::endl;
	int tamVetDist = dtpg.second->distribuicao.size();
	std::vector<double> distGargantasAcumulada(tamVetDist+1);
	distGargantasAcumulada[0] = dtpg.second->distribuicao[0];
	for (int i=1; i<tamVetDist; ++i) {
		distGargantasAcumulada[i] = distGargantasAcumulada[i-1] + dtpg.second->distribuicao[i];
	}
	//Variáveis auxiliares
	cabe = false;
	int Z;
	long double gSitioLigacao; //Condutância entre sítio e ligação
	double random;
	cont = matrizObjetosSL.rbegin()->first; //índice do último elemento da matriz
	std::map<int, CObjetoRedeDePercolacao>::iterator itt; //iterator para o segundo sítio
	std::map<int, CObjetoRedeDePercolacao>::iterator itc; //iterator para a conexão
	bool encontrouRaio = false;
	bool interligados = false;
	int objit = 0;
	int objitt = 0;
	int obj;

	// Primeiro interliga todos os sitios por proximidade e com número de coordenação 2.
	// Isso irá subestimar a distribuição de ligações, então, após todos os sítios estarem interligados,
	// executa uma segunda etapa com ligações aleatórias e sem reduzir o raio da ligação.
	std::cout << "Criando ligacoes! Primeira etapa..." << std::endl;
	// Durante o loop o tamanho da matrizObjetos será alterado, então, preciso percorrer somente os objetos atuais.
	int tamMatObjs = matrizObjetosSL.size();
	for ( obj=1; obj<=tamMatObjs; ++obj ) {
		encontrouRaio = false;

		// Inicialmente cabe==false, então, cada sítio terá número de coordenação 2.
		// Se cabe==true, é porque todos os sítios já foram interligados, então,
		// o valor de nCoord a ser setado será igual ao numero de conexões existentes no objeto + 1.
		if ( ! cabe ) {
			//pega o primeiro sítio
			it = matrizObjetosSL.find(obj);
			if (it == matrizObjetosSL.end()) {
				continue;
			}

			itt = it;
			for ( Z = it->second.NumConexoes()+1; Z <= 1; ++Z ) { //se o sitio já tiver duas conexões, nem entra no loop
				std::cout << "Procurando proximo objeto ainda nao conectado..." << std::endl;
				while (true) { // Vai para o próximo objeto ainda não conectado ao objeto atual.
					interligados = false;
					do { //procura o próximo sítio com até uma ligação.
						++itt; //iterator para o próximo objeto.
					} while ( itt != matrizObjetosSL.end() && itt->first <= tamMatObjs && itt->second.NumConexoes() >= 2 );
					if (itt == matrizObjetosSL.end() || itt->first > tamMatObjs) {
						break;
					}
					for (auto &c: it->second.SConexao()) { //para cada ligação já conectada ao primeiro sítio
						itc = matrizObjetosSL.find(c.first); //pega o iterator para a ligação
						for (auto &o: itc->second.SConexao()) { //percorre a lista de objetos conectados a ligação
							if ( o.first == itt->first ) { //verifica se o primerio sítio está conectado ao segundo através da ligação
								interligados = true;
								break; //se estiverem interligado, não precisa mais procurar.
							}
						}
						if(interligados) {//se estiverem interligado, não precisa mais procurar.
							break; //sai do loop de conexões
						}
					}
					if ( ! interligados) {
						break; //sai do loop geral quando encontrar o segundo sítio ainda não interligado ao primeiro
					}
				}
				if (itt == matrizObjetosSL.end() || itt->first > tamMatObjs || interligados)
					break;

				// Determinando o raio do objeto.
				// Sorteia valores aleatórios entre 0 e 1 e obtém o raio na distribuição acumulada
				raio = 1;
				random = DRandom(); //obtem valor double randômico entre 0.0 e 1.0;
				//percorre vetor de distribuição acumulada para obter raio correspondente
				for (int i=0; i<tamVetDist; ++i) {
					if ( random <= distGargantasAcumulada[i] ) {
						if (i > 0) { //aqui o valor sorteado é menor ou igual e não estamos no primeiro elemento do vetor
							// Verifica a diferença entre o número randômico e os elementas i e i-1.
							// Seta o raio com o indice do valor mais próximo a random.
							if ( (random - distGargantasAcumulada[i-1]) < (distGargantasAcumulada[i] - random) ) {
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
				std::cout << "Raio da ligacao: " << raio << std::endl;

				// Certifica que o raio da ligação será menor que o raio dos sítios a serem interligados ou igual a 1
				// Aqui: Este procedimento está alterando muito a distribuição de ligações!
				while ( (raio >= it->second.Raio() || raio >= itt->second.Raio()) && raio > 1 ) {
					--raio;
				}
				std::cout << "Novo raio da ligacao: " << raio << std::endl;
				// Aqui it e itt apontam para dois sítios distintos ainda não interligados

				std::cout << "Encontrou os sítios! Interligando..." << std::endl;

				// Calcula distância entre os sítios
				distancia = DistanciaEntrePontos(it->second.PontoCentral_X(), it->second.PontoCentral_Y(), it->second.PontoCentral_Z(), itt->second.PontoCentral_X(), itt->second.PontoCentral_Y(), itt->second.PontoCentral_Z() );
				distancia = distancia - it->second.Raio() - itt->second.Raio();
				if ( distancia < 1 ) // Caso os sítios se toquem, a distância dará 0, então força que seja pelo menos 1
					distancia = 1;
				std::cout << "Tamanho da ligacao: " << distancia << std::endl;

				//calcular a porosidade correspondente a ligação (cilindro) que será criada com o raio sorteado.
				phiObjeto = PhiCilindro(raio, distancia, area);
				//Se a soma das porosidades for maior que a porosidade da matriz de gargantas e o raio for maior que 1,
				//decrementa o raio até que a soma das porosidades seja menor que a porosidade da matriz de gargantas, ou ate que o raio seja 1.
				//Ao sair do loop, incrementa o raio e recalcula a area do cilindro, de modo que phiLigacoes fique o mais próximo possível de phiGargantas.
				if ( phiLigacoes+phiObjeto > phiGargantas && raio > 1) {
					while( phiLigacoes+phiObjeto > phiGargantas && raio > 1) {
						--raio;
						phiObjeto = PhiCilindro(raio, distancia, area);
					}
					++raio;
					phiObjeto = PhiCilindro(raio, distancia, area);
				}
				phiLigacoes += phiObjeto; //acumula a porosidade

				// Conecta os objetos
				++cont;
				matrizObjetosSL[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, LIGACAO, NumPixeisCilindro(raio, distancia));
				itMatObj = matrizObjetosSL.find(cont);
				itMatObj->second.PontoCentral(
							(int)round((it->second.PontoCentral_X()+itt->second.PontoCentral_X())/2),
							(int)round((it->second.PontoCentral_Y()+itt->second.PontoCentral_Y())/2),
							(int)round((it->second.PontoCentral_Z()+itt->second.PontoCentral_Z())/2),
							3*raio
							);
				itMatObj->second.Raio( raio );
				xSolver = (long double)itMatObj->second.PontoCentral_X();
				itMatObj->second.X(xSolver);

				// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
				xToObj.insert(pair<int, int>(itMatObj->second.PontoCentral_X(),cont));

				//Cálculo de condutâncias e realiza conexões
				itMatObj->second.Propriedade ( CondutanciaLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao) );
				//primeiro sítio
				gSitioLigacao = CondutanciaSitioLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
				itMatObj->second.Conectar(it->first, gSitioLigacao);
				it->second.Conectar(cont, gSitioLigacao);
				//segundo sítio
				gSitioLigacao = CondutanciaSitioLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
				itMatObj->second.Conectar(itt->first, gSitioLigacao);
				itt->second.Conectar(cont, gSitioLigacao);
			}
		} else { // Todos os sítios foram interligados. Daqui para a frente, até atender a porosidade, será sorteado
			// um sítio aleatório, cujo raio seja maior ou igual ao raio da garganta, e que será conectado a outro sítio
			// ainda não conectado ao primeiro, e que também tenha raio maior ou igual ao raio da garganta.

			// Determinando o raio do objeto.
			// Sorteia valores aleatórios entre 0 e 1 e obtém o raio na distribuição acumulada
			raio = 1;
			random = DRandom(); //obtem valor double randômico entre 0.0 e 1.0;
			//percorre vetor de distribuição acumulada para obter raio correspondente
			for (int i=0; i<tamVetDist; ++i) {
				if ( random <= distGargantasAcumulada[i] ) {
					if (i > 0) { //aqui o valor sorteado é menor ou igual e não estamos no primeiro elemento do vetor
						// Verifica a diferença entre o número randômico e os elementas i e i-1.
						// Seta o raio com o indice do valor mais próximo a random.
						if ( (random - distGargantasAcumulada[i-1]) < (distGargantasAcumulada[i] - random) ) {
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
			std::cout << "Raio da ligacao: " << raio << std::endl;

			//Procura dois sítios, ainda não interconectados, e com raio maior ou igual ao raio da ligação a ser criada.
			while ( ! encontrouRaio ) { //loop até encontrar os dois sítios
				encontrouRaio = false;
				//buscando o primeiro sítio
				obj = Random(1,tamMatObjs-1); //pega sítios aleatórios.
				it = matrizObjetosSL.find(obj);
				if ( it != matrizObjetosSL.end() ) {
					if ( it->second.Raio() >= raio ) {
						encontrouRaio = true;
						objit = obj;
					}
				}
				if ( it != matrizObjetosSL.end() && encontrouRaio ) {
					std::cout << "Encontrou o primeiro sitio [" << objit << "]. Buscando o segundo..." << std::endl;
					encontrouRaio = false;
					// Percorre os sítios posteriores ao primeiro sítio
					for ( obj=it->first+1; obj<=tamMatObjs; ++obj ) {
						itt = matrizObjetosSL.find(obj);
						if ( itt != matrizObjetosSL.end() ) {
							if ( itt->second.Raio() >= raio ) { //encontrou outro sítio com raio maior ou igual ao raio da ligação
								encontrouRaio = true;
								objitt = obj;
							}
						}
					}
					if ( ! encontrouRaio ) { //Se ainda não encontrou o sítio
						//Percorre os sítio anteriores ao primeiro sítio
						for ( obj=it->first-1; obj > 0; --obj ) {
							itt = matrizObjetosSL.find(obj);
							if ( itt != matrizObjetosSL.end() ) {
								if ( itt->second.Raio() >= raio ) { //encontrou outro sítio com raio maior ou igual ao raio da ligação
									encontrouRaio = true;
									objitt = obj;
								}
							}
						}
					}
					if ( ! encontrouRaio ) { //se não encontrou, sai do loop e tenta outro raio de ligação
						std::cout << "Não encontrou o segundo sitio..." << std::endl;
						break;
					} else { //Encontrou o segundo sítio. Verifica se estão interligados.
						std::cout << "Encontrou o segundo sítio [" << objitt << "]. Verificando se estão interligados..." << std::endl;
						// Verifica se os sítios já estão interligados
						interligados = false;
						for (auto &c: it->second.SConexao()) { //para cada ligação já conectada ao primeiro sítio
							itc = matrizObjetosSL.find(c.first); //pega o iterator para a ligação
							for (auto &o: itc->second.SConexao()) { //percorre a lista de objetos conectados a ligação
								if ( o.first == itt->first ) { //verifica se o primerio sítio está conectado ao segundo através da ligação
									interligados = true;
									//std::cout << "Ja estao interligados: o.first=" << o.first << "; itt->first=" << itt->first << ". Saindo do loop." << std::endl;
									break; //se estiverem interligado, não precisa mais procurar.
								}
							}
							if(interligados) {//se estiverem interligado, não precisa mais procurar.
								//std::cout << "Saindo do segundo loop" << std::endl;
								break; //sai do loop de conexões
							}
						}
						if ( interligados ) { //os sítios já estão interligados. tenta outro sítio
							std::cout << "Ja estao interligados! Procurando outro sitio..." << std::endl;
							encontrouRaio = false;
							break;
						} else { // Aqui it e itt apontam para dois sítios distintos ainda não interligados
							std::cout << "Encontrou os sítios! Interligando..." << std::endl;

							// Calcula distância entre os sítios
							distancia = DistanciaEntrePontos(it->second.PontoCentral_X(), it->second.PontoCentral_Y(), it->second.PontoCentral_Z(), itt->second.PontoCentral_X(), itt->second.PontoCentral_Y(), itt->second.PontoCentral_Z() );
							distancia = distancia - it->second.Raio() - itt->second.Raio();
							if ( distancia < 1 ) // Caso os sítios se toquem, a distância dará 0, então força que seja pelo menos 1
								distancia = 1;
							std::cout << "Tamanho da ligacao: " << distancia << std::endl;

							//calcular a porosidade correspondente a ligação (cilindro) que será criada com o raio sorteado.
							phiObjeto = PhiCilindro(raio, distancia, area);
							//Se a soma das porosidades for maior que a porosidade da matriz de gargantas e o raio for maior que 1,
							//decrementa o raio até que a soma das porosidades seja menor que a porosidade da matriz de gargantas, ou ate que o raio seja 1.
							//Ao sair do loop, incrementa o raio e recalcula a area do cilindro, de modo que phiLigacoes fique o mais próximo possível de phiGargantas.
							if ( phiLigacoes+phiObjeto > phiGargantas && raio > 1) {
								while( phiLigacoes+phiObjeto > phiGargantas && raio > 1) {
									--raio;
									phiObjeto = PhiCilindro(raio, distancia, area);
								}
								++raio;
								phiObjeto = PhiCilindro(raio, distancia, area);
							}
							phiLigacoes += phiObjeto; //acumula a porosidade

							// Conecta os objetos
							++cont;
							matrizObjetosSL[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, LIGACAO, NumPixeisCilindro(raio, distancia));
							itMatObj = matrizObjetosSL.find(cont);
							itMatObj->second.PontoCentral(
										(int)round((it->second.PontoCentral_X()+itt->second.PontoCentral_X())/2),
										(int)round((it->second.PontoCentral_Y()+itt->second.PontoCentral_Y())/2),
										(int)round((it->second.PontoCentral_Z()+itt->second.PontoCentral_Z())/2),
										3*raio
										);
							itMatObj->second.Raio( raio );
							xSolver = (long double)itMatObj->second.PontoCentral_X();
							itMatObj->second.X(xSolver);

							// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
							xToObj.insert(pair<int, int>(itMatObj->second.PontoCentral_X(),cont));

							//Cálculo de condutâncias e realiza conexões
							itMatObj->second.Propriedade ( CondutanciaLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao) );
							//primeiro sítio
							gSitioLigacao = CondutanciaSitioLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
							itMatObj->second.Conectar(it->first, gSitioLigacao);
							it->second.Conectar(cont, gSitioLigacao);
							//segundo sítio
							gSitioLigacao = CondutanciaSitioLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
							itMatObj->second.Conectar(itt->first, gSitioLigacao);
							itt->second.Conectar(cont, gSitioLigacao);
							// Aqui encontrouRaio será true, logo, sairá do loop.
						}
					}
				}
			}
		}
		if (phiLigacoes >= phiGargantas)
			break;
		// Se conectou o último sítio e ainda precisa criar mais ligações, volta para o primeiro sítio e seta cabe=true;
		// Assim serão criadas ligações aleatórias...
		if ( obj==tamMatObjs && phiLigacoes < phiGargantas ) {
			cabe = true;
			obj = 1;
		}
	}
	// Interligou todos os objetos até atender a porosidade
	numLigacoes = cont-tamMatObjs;
	std::cout << "Ligacoes criadas!\nphiLigacoes: " << phiLigacoes << " | phiGargantas: " << phiGargantas << " | Num. Ligacoes: " << numLigacoes << std::endl;
	std::cout << cont << " objetos criados!" << std::endl;
	std::cout << "Ordenado os objetos (sítios e ligações) pelo eixo x ..." << std::endl;
	std::map<int,int> objAntToObjAtual;
	ptrMatObjsRede->matrizObjetos.clear(); // Matriz de objetos final
	cont = 0;
	std::cout << "Copia os objetos, já ordenados em x, da matriz temporária para a metriz definitiva..." << std::endl;
	for (auto &xto : xToObj ) {
		++cont;
		ptrMatObjsRede->matrizObjetos[cont] = matrizObjetosSL[xto.second];
		objAntToObjAtual[xto.second] = cont;
		//std::cout << "[x=" << xto.first << ",\t\t\tObjAnt=" << xto.second<< ",\t\t\tObjAtu=" << cont << "]" << std::endl;
	}
	matrizObjetosSL.clear();
	//percorre a matriz de objetos e atualiza os rótulos dos objetos conectados
	std::map<int, double> conexoesAtu;
	//std::map<int, double> conexoesAnt;
	for ( auto &objs : ptrMatObjsRede->matrizObjetos ) {
		//copia as conexões para o map temporário
		conexoesAtu.clear();
		//conexoesAnt = objs.second.SConexao();
		//for ( auto obj : conexoesAnt ) { //a condutância premanece a mesma, só mudou o rótulo do objeto
		for ( auto &obj : objs.second.SConexao() ) { //a condutância premanece a mesma, só mudou o rótulo do objeto
			conexoesAtu.insert(make_pair(objAntToObjAtual[obj.first], obj.second));
		}
		//limpa o vetor de conexoes do objeto
		//conexoesAnt.clear();
		objs.second.SConexao().clear();
		//copia para os objetos as novas conexões salvas no map temporário
		for ( auto &obj : conexoesAtu ) {
			//conexoesAnt.insert(make_pair(obj.first, obj.second));
			objs.second.Conectar(obj.first, obj.second);
		}
	}
	objAntToObjAtual.clear();
	xToObj.clear();
	return true;
}

// Cria rede de percolação com sítios em posições e tamanhos aleatórios com variação do número de ligações.
// Semelhante ao ModeloDois, porém, após conectar todos os sítios, deixa de reduzir os raios das ligações
// de forma que não altere a distribuição de ligações.
bool CRedeDePercolacao::Modelo4() {
	std::cout << "Criando rede atraves do Modelo 4..." << std::endl;
	TCMatriz3D<bool> pm(nx, ny, nz);
	int area = nx*ny*nz; //área da matriz 3D (em pixeis)
	long double phiGargantas	= dtpg.second->AreaObjetos(); //porosidade da imagem (garganta)
	double areaTotalLigacoes	= (area*phiGargantas)/100.0;
	long double phiLigacoes		= 0.0; //porosidade da rede (licações)
	//long double phiObjeto		= 0.0; //porosidade do objeto que esta sendo criado (esfera/sítio, cilindro/ligação)
	long double xSolver				= 0.0; //variável utilizada para setar o valor x do parametro de solver do objeto. Utilizado na simulação.
	int x, y, z; //posição na matriz
	CBCd3453D * esfera;
	bool cabe; //flag que indicará se a esfera cabe na região sem sobrepor outras esferas.
	std::multimap<int, int> xToObj; // xToObj será uma referência para ordenar os objetos do menor para o maior valor de x.

	//============================================== SITIOS =================================================
	std::vector<int> * raios = CriarVetorDeRaiosDosSitiosByDTP();
	std::cout << "Representando os sítios na matriz sem sobrepor..." << std::endl;
	int im, jm, km;
	int x_raio, y_raio, z_raio;
	int cont = 0;
	int objeto; //indice do objeto com menor distância entre o ponto 0,0,0 e o centro do objeto (sitio).
	double distancia = 1000000.0; //guarda a menor distancia encontrada  entre o ponto 0,0,0 e o centro dos sítios.
	double distTemp;
	int maiorRaioEsquerda = 0;
	int maiorRaioDireita = 0;
	int raio;
	int diametro;
	std::map<int, CObjetoRedeDePercolacao> matrizObjetosTemp; // Matriz de objetos temporária;

	{	//============================================== FRONTEIRAS =================================================
		double phiTotal = dtpg.first->AreaObjetos() + phiGargantas;
		double phiFronteiraEsquerda = 0.0;
		double phiFronteiraDireita = 0.0;
		int areaFronteira = ny*nz;
		int pos = 0;
		//============================================== ESQUERDA =================================================
		// sorteando sítios da fronteira esquerda
		std::vector<int> raiosFronteiraEsquerda;
		while ( phiFronteiraEsquerda < phiTotal ) {
			pos = Random(0,raios->size()-1); // Sorteira uma posição no vetor de raios
			raio = raios->at(pos); //pega o raio do elemento sorteado
			raiosFronteiraEsquerda.push_back(raio); //Armazena o raio no vetor
			phiFronteiraEsquerda += PhiDisco(raio, areaFronteira); //Acumula a porosidade
			raios->erase(raios->begin()+pos); //Apaga do vetor de raios o raio já utilizado
			if (raio > maiorRaioEsquerda) { //Guarda o maior raio sorteado
				maiorRaioEsquerda = raio;
			}
		}
		std::cout << "PhiFronteiraEsquerda= " << phiFronteiraEsquerda << " PhiTotal= " << phiTotal << std::endl;
		// Ordenar os raios do maior para o menor de forma que as esferas maiores serão criadas primeiro.
		std::sort(raiosFronteiraEsquerda.begin(), raiosFronteiraEsquerda.end());
		std::reverse(raiosFronteiraEsquerda.begin(), raiosFronteiraEsquerda.end());

		// aloca os sítios da fronteira esquerda
		x = maiorRaioEsquerda;
		for (auto &raio : raiosFronteiraEsquerda) {
			diametro = ((2*raio)+1);
			//cria esfera de raio correspondente.
			esfera = new CBCd3453D(diametro);
			// Sortear posições testando se a esfera cabe sem sobrepor outras esferas ou ultrapassar a borda.
			do {
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
							if ( esfera->data3D[i][j][k]!=0 && pm.data3D[im][jm][km]!=0 ) {
								cabe = false;
							}
						}
					}
				}
				if (cabe) { //desenha a esfera
					//std::cout << "Desenhando sítio " << cont << " de " << raios.size() << endl;
					for (int i=0; i<diametro; ++i) {
						im = i+x_raio;
						for (int j=0; j<diametro; ++j) {
							jm = j+y_raio;
							for (int k=0; k<diametro; ++k) {
								km = k+z_raio;
								if ( esfera->data3D[i][j][k]!=0 ) {
									pm.data3D[im][jm][km]=1;
								}
							}
						}
					}
				}
			} while (!cabe);
			// aloca na matriz de objetos o sítio criado
			++cont;
			matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, SITIO, NumPixeisEsfera(raio));
			matrizObjetosTemp[cont].Contorno(CContorno::ETipoContorno::WEST);
			matrizObjetosTemp[cont].PontoCentral( x, y, z, 3*raio );
			matrizObjetosTemp[cont].Raio( raio );
			xSolver = (long double)x;
			matrizObjetosTemp[cont].X(xSolver); //seta o x do solver que será utilizado na simulação;
			matrizObjetosTemp[cont].Propriedade( CondutanciaSitio(matrizObjetosTemp[cont].Raio(), dimensaoPixel, fatorAmplificacao) );
			delete esfera;
		}
		raiosFronteiraEsquerda.clear();

		//============================================== DIREITA =================================================
		// sorteando sítios da fronteira direita
		std::vector<int> raiosFronteiraDireita;
		while ( phiFronteiraDireita < phiTotal ) {
			pos = Random(0,raios->size()-1); // Sorteira uma posição no vetor de raios
			raio = raios->at(pos); //pega o raio do elemento sorteado
			raiosFronteiraDireita.push_back(raio); //Armazena o raio no vetor
			phiFronteiraDireita += PhiDisco(raio, areaFronteira); //Acumula a porosidade
			raios->erase(raios->begin()+pos); //Apaga do vetor de raios o raio já utilizado
			if (raio > maiorRaioDireita) { //Guarda o maior raio sorteado
				maiorRaioDireita = raio;
			}
		}
		std::cout << "PhiFronteiraDireita= " << phiFronteiraDireita << " PhiTotal= " << phiTotal << std::endl;
		// Ordenar os raios do maior para o menor de forma que as esferas maiores serão criadas primeiro.
		std::sort(raiosFronteiraDireita.begin(), raiosFronteiraDireita.end());
		std::reverse(raiosFronteiraDireita.begin(), raiosFronteiraDireita.end());

		// aloca os sítios da fronteira direira
		x = nx-maiorRaioDireita-1;
		for (auto &raio : raiosFronteiraDireita) {
			diametro = ((2*raio)+1);
			//cria esfera de raio correspondente.
			esfera = new CBCd3453D(diametro);
			// Sortear posições testando se a esfera cabe sem sobrepor outras esferas ou ultrapassar a borda.
			do {
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
							if ( esfera->data3D[i][j][k]!=0 && pm.data3D[im][jm][km]!=0 ) {
								cabe = false;
							}
						}
					}
				}
				if (cabe) { //desenha a esfera
					//std::cout << "Desenhando sítio " << cont << " de " << raios.size() << endl;
					for (int i=0; i<diametro; ++i) {
						im = i+x_raio;
						for (int j=0; j<diametro; ++j) {
							jm = j+y_raio;
							for (int k=0; k<diametro; ++k) {
								km = k+z_raio;
								if ( esfera->data3D[i][j][k]!=0 ) {
									pm.data3D[im][jm][km]=1;
								}
							}
						}
					}
				}
			} while (!cabe);
			// aloca na matriz de objetos o sítio criado
			++cont;
			matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, SITIO, NumPixeisEsfera(raio));
			matrizObjetosTemp[cont].Contorno(CContorno::ETipoContorno::EST);
			matrizObjetosTemp[cont].PontoCentral( x, y, z, 3*raio );
			matrizObjetosTemp[cont].Raio( raio );
			xSolver = (long double)x;
			matrizObjetosTemp[cont].X(xSolver); //seta o x do solver que será utilizado na simulação;
			matrizObjetosTemp[cont].Propriedade( CondutanciaSitio(matrizObjetosTemp[cont].Raio(), dimensaoPixel, fatorAmplificacao) );
			delete esfera;
		}
		raiosFronteiraDireita.clear();
	}
	//============================================== CENTRO =================================================
	// Agora aloca os demais sitos que restaram no vetor de raios
	for ( auto &raio : *raios ) {
		diametro = ((2*raio)+1);
		//cria esfera de raio correspondente.
		esfera = new CBCd3453D(diametro);
		// Sortear posições testando se a esfera cabe sem sobrepor outras esferas ou ultrapassar a borda.
		do {
			x = Random(maiorRaioEsquerda+1, nx-maiorRaioDireita-2);
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
						if ( esfera->data3D[i][j][k]!=0 && pm.data3D[im][jm][km]!=0 ) {
							cabe = false;
						}
					}
				}
			}
			if (cabe) { //desenha a esfera
				//std::cout << "Desenhando sítio " << cont << " de " << raios.size() << endl;
				for (int i=0; i<diametro; ++i) {
					im = i+x_raio;
					for (int j=0; j<diametro; ++j) {
						jm = j+y_raio;
						for (int k=0; k<diametro; ++k) {
							km = k+z_raio;
							if ( esfera->data3D[i][j][k]!=0 ) {
								pm.data3D[im][jm][km]=1;
							}
						}
					}
				}
			}
		} while (!cabe);
		// aloca na matriz de objetos o sítio criado
		++cont;
		matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, SITIO, NumPixeisEsfera(raio));
		matrizObjetosTemp[cont].Contorno(CContorno::ETipoContorno::CENTER);
		matrizObjetosTemp[cont].PontoCentral( x, y, z, 3*raio );
		matrizObjetosTemp[cont].Raio ( raio );
		xSolver = (long double)x;
		matrizObjetosTemp[cont].X(xSolver); //seta o x do solver que será utilizado na simulação;
		matrizObjetosTemp[cont].Propriedade( CondutanciaSitio(matrizObjetosTemp[cont].Raio(), dimensaoPixel, fatorAmplificacao) );
		delete esfera;
	}
	raios->clear(); // limpa vetor de raios (não será mais utilizado)
	delete raios;

	std::cout << "Ordenando os sítios por proximidade..." << std::endl;
	std::map<int, CObjetoRedeDePercolacao>::iterator it;
	std::map<int, CObjetoRedeDePercolacao>::iterator itMatObj;
	std::map<int, CObjetoRedeDePercolacao> matrizObjetosSL; // Matriz de objetos sítios e ligações temporária;
	cont = 1;
	matrizObjetosSL[cont] = matrizObjetosTemp[1];
	matrizObjetosTemp.erase(1);
	// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
	xToObj.insert(pair<int, int>(matrizObjetosSL[cont].PontoCentral_X(),cont));
	while ( matrizObjetosTemp.size() > 0 ) {
		distancia = 1000000.0;
		it = matrizObjetosSL.find(cont);
		//percorre todos objetos em busca do objeto mais próximo ao objeto atual.
		for ( auto & mot : matrizObjetosTemp ) {
			distTemp = DistanciaEntrePontos(it->second.PontoCentral_X(), it->second.PontoCentral_Y(), it->second.PontoCentral_Z(), mot.second.PontoCentral_X(), mot.second.PontoCentral_Y(), mot.second.PontoCentral_Z() );
			if ( distTemp <= distancia ){
				distancia = distTemp;
				objeto = mot.first;
			}
		}
		++cont;
		//std::cout << "Objeto: " << objeto << " | Distancia: " << distancia << std::endl;
		matrizObjetosSL[cont] = matrizObjetosTemp[objeto];
		matrizObjetosTemp.erase(objeto);

		// Pega o sítio
		it = matrizObjetosSL.find(cont);
		// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
		xToObj.insert(pair<int, int>(it->second.PontoCentral_X(),cont));
	}

	//============================================== LIGAÇÕES =================================================
	std::cout << "Criando ligacoes..." << std::endl;
	//Variáveis auxiliares
	int tamVetDist = dtpg.second->distribuicao.size();
	cabe = false;
	long double gSitioLigacao; //Condutância entre sítio e ligação
	//double areaLigacao;
	cont = matrizObjetosSL.rbegin()->first; //índice do último elemento da matriz
	std::map<int, CObjetoRedeDePercolacao>::iterator itt; //iterator para o segundo sítio
	std::map<int, CObjetoRedeDePercolacao>::iterator itc; //iterator para a conexão
	bool interligados = false;
	int obj;
	std::vector<double> areaLigacoesAcumuladas(tamVetDist, 0.0);
	std::vector<double> areaLigacoes(tamVetDist);
	for ( int r=0; r<tamVetDist; ++r ) {
		areaLigacoes[r] = dtpg.second->distribuicao[r]*areaTotalLigacoes;
	}

	// Durante o loop o tamanho da matrizObjetos será alterado, então, preciso percorrer somente os objetos atuais.
	int tamMatObjs = matrizObjetosSL.size();
	//Cria as ligações da maior para a menor.
	//Percorre os sítio em busca de dois sítios próximos, com raio maior ou igual ao raio da ligação a ser criada até
	//atender a distribuição de gargantas. Se percorrer todos os sítio e a distribuição não for atendida,
	//almenta o número de ligações permitidas e busca o primeiro sítio de forma aleatória
	for (raio=tamVetDist; raio > 0; --raio ) {
		cabe = false; // Inicialmente cabe==false, então, cada sítio terá número de coordenação 2.
		std::cout << "==>Criando ligações de raio = " << raio << std::endl;
		for ( obj=1; obj<=tamMatObjs; ++obj ) {
			// Se cabe==true é porque todos os sítios de raio maior ou igual ao raio da garganta já foram interligados.
			// Então, o valor de nCoord a ser setado será igual ao numero de conexões existentes no objeto + 1.
			if ( cabe ) { //já percorreu todos os sítios e ainda cabem mais ligações. Sorteia sítios aleatórios.
				obj = Random(1,tamMatObjs-1); //pega sítios aleatórios.
			}

			//Procura dois sítios, ainda não interconectados, e com raio maior ou igual ao raio da ligação a ser criada.
			//buscando o primeiro sítio
			it = matrizObjetosSL.find(obj);
			if ( it == matrizObjetosSL.end() ) {
				continue;
			}
			if ( it->second.Raio() < raio ) {
				continue;
			}
			if ( !cabe && it->second.SConexao().size() > 1 ) {
				continue;
			}
			std::cout << "Encontrou o primeiro sitio [" << it->first << "]. Buscando o segundo..." << std::endl;

			itt=it;
			do { //pega o próximo sítio com raio maior ou igual ao raio da garganta e ainda não interligado ao primeiro
				++itt;
				if ( itt == matrizObjetosSL.end() || itt->first >= tamMatObjs ) {
					break;
				}
				if (itt->second.Raio() < raio) {
					interligados = true;
					continue;
				}
				if ( !cabe && itt->second.SConexao().size() > 1 ) {
					interligados = true;
					continue;
				}
				std::cout << "Encontrou o segundo sítio [" << itt->first << "]. Verificando se estão interligados..." << std::endl;
				interligados = false;
				for (auto &c: it->second.SConexao()) { //para cada ligação já conectada ao primeiro sítio
					itc = matrizObjetosSL.find(c.first); //pega o iterator para a ligação
					for (auto &o: itc->second.SConexao()) { //percorre a lista de objetos conectados a ligação
						if ( o.first == itt->first ) { //verifica se o primerio sítio está conectado ao segundo através da ligação
							interligados = true;
							std::cout << "Ja estao interligados! Procurando outro sitio..." << std::endl;
							break; //se estiverem interligado, não precisa mais procurar.
						}
					}
					if( interligados ) {//se estiverem interligado, não precisa mais procurar.
						break; //sai do loop de conexões
					}
				}
			} while ( interligados );

			if ( itt == matrizObjetosSL.end() || itt->first >= tamMatObjs ) {
				std::cout << "Não encontrou o segundo sitio! indo para o próximo objeto..." << std::endl;
			} else {
				std::cout << "Sitios encontrados! Interligando..." << std::endl;
				// Calcula distância entre os sítios
				distancia = DistanciaEntrePontos(it->second.PontoCentral_X(), it->second.PontoCentral_Y(), it->second.PontoCentral_Z(), itt->second.PontoCentral_X(), itt->second.PontoCentral_Y(), itt->second.PontoCentral_Z() );
				distancia = distancia - it->second.Raio() - itt->second.Raio();
				if ( distancia < 1 ) { // Caso os sítios se toquem, a distância dará 0, então força que seja pelo menos 1
					distancia = 1;
				}

				// Conecta os objetos
				++cont;
				matrizObjetosSL[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, LIGACAO, NumPixeisCilindro(raio, distancia));
				itMatObj = matrizObjetosSL.find(cont);
				itMatObj->second.PontoCentral(
							(int)round((it->second.PontoCentral_X()+itt->second.PontoCentral_X())/2),
							(int)round((it->second.PontoCentral_Y()+itt->second.PontoCentral_Y())/2),
							(int)round((it->second.PontoCentral_Z()+itt->second.PontoCentral_Z())/2),
							3*raio
							);
				itMatObj->second.Raio( raio );
				xSolver = (long double)itMatObj->second.PontoCentral_X();
				itMatObj->second.X(xSolver);

				// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
				xToObj.insert(pair<int, int>(itMatObj->second.PontoCentral_X(),cont));

				//Cálculo de condutâncias e realiza conexões
				itMatObj->second.Propriedade ( CondutanciaLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao) );
				//primeiro sítio
				gSitioLigacao = CondutanciaSitioLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
				itMatObj->second.Conectar(it->first, gSitioLigacao);
				it->second.Conectar(cont, gSitioLigacao);
				//segundo sítio
				gSitioLigacao = CondutanciaSitioLigacao(itMatObj->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
				itMatObj->second.Conectar(itt->first, gSitioLigacao);
				itt->second.Conectar(cont, gSitioLigacao);

				//calcular a porosidade correspondente a ligação (cilindro) que será criada com o raio sorteado.
				//phiObjeto = PhiCilindro(raio, distancia, area);
				//phiLigacoes += phiObjeto; //acumula a porosidade
				phiLigacoes += PhiCilindro(raio, distancia, area);

				//acumulando a distribuição
				//areaLigacao = NumPixeisCilindro(raio, distancia);
				//areaLigacoesAcumuladas[raio-1]+=areaLigacao;
				areaLigacoesAcumuladas[raio-1]+=NumPixeisCilindro(raio, distancia);
				std::cout << "Sitio " << it->first << " conectado ao sitio " << itt->first << " atraves de uma ligacao de raio " << raio << " e comprimento " << distancia << std::endl;
			}
			// Se conectou o último sítio e ainda precisa criar mais ligações, volta para o primeiro sítio e seta cabe=true;
			// Assim serão criadas ligações aleatórias...
			if ( obj==tamMatObjs /*&& phiLigacoes < phiGargantas*/ && areaLigacoesAcumuladas[raio-1] < areaLigacoes[raio-1] ) {
				cabe = true;
				obj = 1;
			} else if (/*phiLigacoes >= phiGargantas ||*/ areaLigacoesAcumuladas[raio-1] >= areaLigacoes[raio-1]) {
				break;
			}
		}
		std::cout << "Criadas ligacoes de raio: " << raio << std::endl;
		std::cout << "Area das Ligacoes: " << areaLigacoesAcumuladas[raio-1] << " | Area das Gargantas: " << areaLigacoes[raio-1] << std::endl << std::endl;

		//Analisa criterios de parada
		//if ( phiLigacoes >= phiGargantas )
		//	break;
		bool allGT = true;
		for ( int r = 0; r < tamVetDist; ++r ) {
			if ( areaLigacoesAcumuladas[r] < areaLigacoes[r] ) {
				allGT = false;
			}
		}
		if (allGT)
			break;
	}
	// Interligou todos os objetos até atender a porosidade ou a distribuição
	numLigacoes = cont-tamMatObjs;
	std::cout << "Ligacoes criadas!\nphiLigacoes: " << phiLigacoes << " | phiGargantas: " << phiGargantas << " | Num. Ligacoes: " << numLigacoes << std::endl;
	std::cout << cont << " objetos criados!" << std::endl;
	std::cout << "Ordenado os objetos (sítios e ligações) pelo eixo x ..." << std::endl;
	std::map<int,int> objAntToObjAtual;
	ptrMatObjsRede->matrizObjetos.clear(); // Matriz de objetos final
	cont = 0;
	std::cout << "Copia os objetos, já ordenados em x, da matriz temporária para a metriz definitiva..." << std::endl;
	for (auto &xto : xToObj ) {
		++cont;
		ptrMatObjsRede->matrizObjetos[cont] = matrizObjetosSL[xto.second];
		objAntToObjAtual[xto.second] = cont;
		//std::cout << "[x=" << xto.first << ",\t\t\tObjAnt=" << xto.second<< ",\t\t\tObjAtu=" << cont << "]" << std::endl;
	}
	matrizObjetosSL.clear();
	//percorre a matriz de objetos e atualiza os rótulos dos objetos conectados
	std::map<int, double> conexoesAtu;
	//std::map<int, double> conexoesAnt;
	for ( auto &objs : ptrMatObjsRede->matrizObjetos ) {
		//copia as conexões para o map temporário
		conexoesAtu.clear();
		//conexoesAnt = objs.second.SConexao();
		//for ( auto obj : conexoesAnt ) { //a condutância premanece a mesma, só mudou o rótulo do objeto
		for ( auto &obj : objs.second.SConexao() ) { //a condutância premanece a mesma, só mudou o rótulo do objeto
			conexoesAtu.insert(make_pair(objAntToObjAtual[obj.first], obj.second));
		}
		//limpa o vetor de conexoes do objeto
		//conexoesAnt.clear();
		objs.second.SConexao().clear();
		//copia para os objetos as novas conexões salvas no map temporário
		for ( auto &obj : conexoesAtu ) {
			//conexoesAnt.insert(make_pair(obj.first, obj.second));
			objs.second.Conectar(obj.first, obj.second);
		}
	}
	objAntToObjAtual.clear();
	xToObj.clear();
	return true;
}

// Cria rede de percolação com sítios em posições e tamanhos aleatórios conforme a distribuição de tamanho de poros
// As ligações entre os sítios atendem a distribuição do tamanho de gargantas.
// Este modelo aloca um porcentagem de sítios nas fronteiras para que hajam fronteiras bem definidas.
bool CRedeDePercolacao::Modelo5() {
	std::cout << "Criando rede atraves do Modelo 5..." << std::endl;
	//Variáveis auxiliares
	TCMatriz3D<bool> pm(nx, ny, nz);
	int area = nx*ny*nz; //área da matriz 3D (em pixeis)
	int x, y, z; //posição na matriz
	int x0, y0, z0, r0; //posição na matriz, raio e objeto trabalhando no instatne anterior
	int im, jm, km;
	int x_rs, y_rs, z_rs;
	int yMin, yMax, zMin,zMax; //delimitam a região de alocação do sítio no plano y,z
	int cont = 0;
	int rs = 0; //raio sítio
	int pos;
	int diametro;
	int tamLigacao; //tamanho da ligação a ser criada.
	int tamMaxLig; //tamanho máximo da garganta a ser criada.
	int numSCRMIRG; //numero de sítios com raio maior ou igual ao raio da ligação
	double distancia;
	long double gSitioLigacao; //Condutância entre sítio e ligação
	long double xSolver	= 0.0; //variável utilizada para setar o valor x do parametro de solver do objeto. Utilizado na simulação.
	bool cabe = false; //flag que indicará se a esfera cabe na região sem sobrepor outras esferas.
	bool cabeEmAlgumaPosicao;
	CBCd3453D * esfera;
	std::map<int, CObjetoRedeDePercolacao> matrizObjetosTemp; // Matriz de objetos temporária;
	std::map<int, CObjetoRedeDePercolacao>::iterator is1; //iterator para o primeiro sítio
	std::map<int, CObjetoRedeDePercolacao>::iterator is2; //iterator para o segundo sítio
	std::map<int, CObjetoRedeDePercolacao>::iterator itl; //iterator para a conexão
	std::multimap<int, int> xToObj; // xToObj será uma referência para ordenar os objetos do menor para o maior valor de x.
	std::vector<std::pair<int,int>> posicoes;
	std::vector<bool> cabeNaPosicao;

	//========================================= SÍTIOS E LIGAÇÕES ===========================================
	//Tratando as distribuições
	int tamVetDistPor = dtpg.first->distribuicao.size();
	int tamVetDistGar = dtpg.second->distribuicao.size();
	//se por algum motivo (problema) a distribuição de gargantas for maior que a distribuição de poros, inverte as distribuições.
	if (tamVetDistPor < tamVetDistGar) {
		std::swap(dtpg.first, dtpg.second);
		std::swap(tamVetDistPor, tamVetDistGar);
		cout << "Realizou swap nas distribuições" << endl;
	}

	int x_fronteiraDireita = nx-tamVetDistPor-1;

	//Cria vetor de raio dos sítios
	std::vector<int> * raiosSitios = CriarVetorDeRaiosDosSitiosByDTP();
	std::vector<int> numSitiosByRaio(tamVetDistPor+1,0);
	for (auto &r: *raiosSitios) {//acumula o número de sítios a serem criados, para cada tamanho de raio.
		numSitiosByRaio[r] = numSitiosByRaio[r]+1;
	}

	//Ligações
	long double phiGargantas	= dtpg.second->AreaObjetos(); //porosidade da imagem (garganta)
	long double areaTotalGargantas	= (area*phiGargantas)/100.0;
	std::vector<double> areaLigacoesAcumuladas(tamVetDistGar+1, 0.0);
	std::vector<double> areaLigacoes(tamVetDistGar+1, 0.0);
	for ( int r=1; r<=tamVetDistGar; ++r ) {
		areaLigacoes[r] = dtpg.second->distribuicao[r-1]*areaTotalGargantas;
	}

	x = 0;
	bool fronteiraEsquerda = true;
	bool fronteiraDireita = false;
	bool aindaExistemSitios = false;
	bool fronteira = false;
	//Percorre, do maior para o menor, os raios de ligações a serem criadas
	for (int rl=tamVetDistGar; rl > 0; --rl ) {
		//calcula o tamanho máximo da ligação
		tamMaxLig = (int)round((double)areaLigacoes[rl]/(double)numPixeisCirculo[rl]);
		//verifica quantos sítios disponíveis possuem raio maior ou igual ao raio da ligação
		numSCRMIRG = 0;
		for (int r=rl; r < numSitiosByRaio.size(); ++r) {
			numSCRMIRG += numSitiosByRaio[r];
		}
		cout << "numSCRMIRG=" << numSCRMIRG <<  endl;
		//se não existir mais de um sítio maior que a ligação, vai para o próximo riao de ligação.
		if (numSCRMIRG <= 1) {
			std::cout << "===========>Nao encontrou sitio com raio maior ou igual a " << rl << std::endl;
			continue;
		}
		//conclui o cálculo do tamanho máximo, dividindo pelo número de sítios disponíveis, menos um (fronteira)
		//ou seja, uma ligação para cada sítio. Porém as fronteiras não contam!
		tamLigacao = (int)round((float)tamMaxLig/((float)numSCRMIRG - 1.0));
		if (tamLigacao < rl) // o comprimento da garganta, a princípio, não será menor que seu raio.
			tamLigacao = rl;

		//Inicio da alocação de sítios e ligações. Entra no loop até alocar todas as gargantas de raio rl
		//Após alocar todas as garganas, para todos os raios, se ainda existirem sítios  a serem alocados, continua no loop
		while ( (areaLigacoesAcumuladas[rl] < areaLigacoes[rl]) || aindaExistemSitios ) {
			rs = 0;
			//Percorre o vetor de raios em busca de algum que seja maior ou igual ao raio da ligação
			for (std::vector<int>::iterator itr = raiosSitios->begin(); itr!=raiosSitios->end(); ++itr){
				if (*itr >= rl) {
					rs = *itr;
					raiosSitios->erase(itr);
					--numSCRMIRG;
					break;
				}
			}
			if (rs == 0) { //Não encontrou sítio. Sai do loop indo para o próximo raio de ligação.
				std::cout << "Nao encontrou sitio com raio maior ou igual ao raio da ligacao: " << rl << std::endl;
				break;
			}
			//toda vez que estiver na fronteira esquerda inverte o valor da flag fronteira de forma que, ora o ramo irá iniciar na fronteira e hora o ramo irá iniciar de um sítio sorteado.
			if (fronteiraEsquerda) {
				fronteira = !fronteira;
			}
			// Verifica em que possição do eixo x o sítio será alocado
			if ( fronteiraEsquerda && fronteira ) { // estamos na fronteira esquerda e o ramo partirá de um novo sítio.
				x = tamVetDistPor; //precisamos respeitar o maior raio de sítio
			} else {
				if ( fronteiraEsquerda && ! fronteira ) { // estamos na fronteira esquerda mas o ramo partirá de um sítio sorteado
					fronteiraEsquerda = false;
					++numSCRMIRG; //como irá partir de um sítio existente, precisa recalcular o tamanho da ligação levando em consideração um sítio a mais.
					tamLigacao = (int)round((float)tamMaxLig/((float)numSCRMIRG - 1.0));
					if (tamLigacao < rl) // o comprimento da ligação, a princípio, não será menor que seu raio.
						tamLigacao = rl;
					do { //sorteia o sítio (não pode ser da fronteira direita!)
						pos = Random(1,matrizObjetosTemp.size());
						is1 = matrizObjetosTemp.find(pos);
					} while ( ( is1->second.Tipo() == LIGACAO ) || (is1->second.PontoCentral_X() >= x_fronteiraDireita) || (is1->second.Raio() < rl ) );
					x0 = is1->second.PontoCentral_X();
					y0 = is1->second.PontoCentral_Y();
					z0 = is1->second.PontoCentral_Z();
					r0 = is1->second.Raio();
				}
				if ( x0+r0+tamLigacao+rs >= x_fronteiraDireita) { // estamos na fronteira direita, também é preciso respeitar o maior raio de sítio
					fronteiraDireita = true;
					x = x_fronteiraDireita;
				} else { //estamos entre as fronteiras. sorte valor para x.
					x = Random(x0, x0+r0+tamLigacao+rs);
				}
			}
			//calcula o diametro da esfera
			diametro = ((2*rs)+1);
			//cria esfera de raio correspondente.
			esfera = new CBCd3453D(diametro);
			if (fronteiraEsquerda) { //se estiver na fronteira esquerda, considera todo o plano y,z para alocar o sítio
				yMin = rs;
				yMax = ny-rs-1;
				zMin = rs;
				zMax = nz-rs-1;
				// Sortear posições testando se a esfera cabe sem sobrepor outras esferas ou ultrapassar a borda.
				do {
					y = Random(yMin, yMax);
					z = Random(zMin, zMax);
					x_rs = x-rs;
					y_rs = y-rs;
					z_rs = z-rs;
					cabe = true;
					for (int i=0; i<diametro && cabe; ++i) {
						im = i+x_rs;
						for (int j=0; j<diametro && cabe; ++j) {
							jm = j+y_rs;
							for (int k=0; k<diametro && cabe; ++k) {
								km = k+z_rs;
								if ( esfera->data3D[i][j][k]!=0 && pm.data3D[im][jm][km]!=0 ) {
									cabe = false;
								}
							}
						}
					}
					if (cabe) { //desenha a esfera
						for (int i=0; i<diametro; ++i) {
							im = i+x_rs;
							for (int j=0; j<diametro; ++j) {
								jm = j+y_rs;
								for (int k=0; k<diametro; ++k) {
									km = k+z_rs;
									if ( esfera->data3D[i][j][k]!=0 ) {
										pm.data3D[im][jm][km]=1;
									}
								}
							}
						}
					} else {
						std::cout << "A esfera não cabe nesta posição! Procurando outra..." << std::endl;
					}
				} while (!cabe);
			} else { //no centro ou na fronteira direita, considera somente uma pequena área do plano y,z, suficiente para atingir o raio da ligação
				yMin = (y0-r0-rs-tamLigacao-1 < rs) ? rs : y0-r0-rs-tamLigacao-1;
				zMin = (z0-r0-rs-tamLigacao-1 < rs) ? rs : z0-r0-rs-tamLigacao-1;
				yMax = (y0+r0+rs+tamLigacao+1 > ny-rs-1) ? ny-rs-1 : y0+r0+rs+tamLigacao+1;
				zMax = (z0+r0+rs+tamLigacao+1 > nz-rs-1) ? nz-rs-1 : z0+r0+rs+tamLigacao+1;
				posicoes.clear();
				cabeNaPosicao.clear();
				cabeEmAlgumaPosicao = false;
				x_rs = x-rs;
				for ( y=yMin; y<=yMax; ++y ) {
					for ( z=zMin; z<=zMax; ++z ) {
						distancia = round( DistanciaEntrePontos(x0,y0,z0,x,y,z) - (double)r0 - (double)rs );
						if (tamLigacao == distancia ) {
							y_rs = y-rs;
							z_rs = z-rs;
							cabe = true;
							for (int i=0; i<diametro && cabe; ++i) {
								im = i+x_rs;
								for (int j=0; j<diametro && cabe; ++j) {
									jm = j+y_rs;
									for (int k=0; k<diametro && cabe; ++k) {
										km = k+z_rs;
										if ( esfera->data3D[i][j][k]!=0 && pm.data3D[im][jm][km]!=0 ) {
											cabe = false;
										}
									}
								}
							}
							if (cabe)
								cabeEmAlgumaPosicao = true;
							cabeNaPosicao.push_back(cabe);
							posicoes.push_back(make_pair(y,z));
							//std::cout << "TamLigacao: " << tamLigacao << " | Distancia: " << (int)distancia << " | yMin: " << yMin << " | yMax: " << yMax << " | zMin: " << zMin << " | zMax: " << zMax << std::endl;
							//std::cout << "x0 = " << x0 << " | y0 = " << y0 << " | z0 = " << z0 << " | r0 = " << r0 << std::endl;
							//std::cout << "x = " << x << " | y = " << y << " | z = " << z  << " | rs = " << rs << std::endl;
						}
					}
				}
				if (cabeEmAlgumaPosicao){ //sorteia a posição; pega y e z; desenha a esfera.
					do {
						pos = Random(0,posicoes.size()-1);
					} while ( ! cabeNaPosicao[pos] );
					y = posicoes[pos].first;
					z = posicoes[pos].second;
				} else if ( posicoes.size() > 0 ) { //não cabe em nenhuma posição; Pega qualquer posição e sobrepõe.
					pos = Random(0,posicoes.size()-1);
					y = posicoes[pos].first;
					z = posicoes[pos].second;
				} else { //o tamanho da ligação não corresponde em nenhuma das posições. Desenha uma ligação reta paralela a x. Dificilmente este caso acontecerá e se acontecer irá subestimar a área de ligações
					y = y0;
					z = z0;
				}
			}
			delete esfera;
			//Definida a posição do sítio. Alocar e interligar
			++cont;
			matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, SITIO, NumPixeisEsfera(rs));
			is2 = matrizObjetosTemp.find(cont);
			is2->second.PontoCentral( x, y, z, 3*rs );
			is2->second.Raio( rs );
			xSolver = (long double)x;
			is2->second.X(xSolver); //seta o x do solver que será utilizado na simulação;
			is2->second.Propriedade( CondutanciaSitio(is2->second.Raio(), dimensaoPixel, fatorAmplificacao) );

			// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
			xToObj.insert(pair<int, int>(is2->second.PontoCentral_X(),cont));

			if ( fronteiraEsquerda ) {
				is2->second.Contorno(CContorno::ETipoContorno::WEST); //esquerda
			} else { // centro ou fronteira direita
				if ( fronteiraDireita ) {
					is2->second.Contorno(CContorno::ETipoContorno::EST); //direita
					//ao alocar um sítio na fronteira direita, precisa recalcular o tamanho da garganta, pois a princípio
					//este sítio só terá uma garganta conectada a ele.
					tamLigacao = (int)round((float)tamMaxLig/((float)numSCRMIRG - 1.0));
					if (tamLigacao < rl) // o comprimento da garganta, a princípio, não será menor que seu raio.
						tamLigacao = rl;
				} else {
					is2->second.Contorno(CContorno::ETipoContorno::CENTER); //centro
				}
				//Ligação. Conecta os	objetos...
				++cont;
				areaLigacoesAcumuladas[rl] += NumPixeisCilindro(rl, tamLigacao);

				matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, LIGACAO, NumPixeisCilindro(rl, tamLigacao));
				itl = matrizObjetosTemp.find(cont);
				itl->second.PontoCentral( (int)round((x0+x)/2),(int)round((y0+y)/2),(int)round((z0+z)/2),3*rl );
				itl->second.Raio( rl );
				xSolver = (long double)itl->second.PontoCentral_X();
				itl->second.X(xSolver);

				// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
				xToObj.insert(pair<int, int>(itl->second.PontoCentral_X(),cont));

				//Cálculo de condutâncias e realiza conexões
				distancia = (double)tamLigacao;
				itl->second.Propriedade ( CondutanciaLigacao(itl->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao) );
				//primeiro sítio
				gSitioLigacao = CondutanciaSitioLigacao(itl->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
				itl->second.Conectar(is1->first, gSitioLigacao);
				is1->second.Conectar(cont, gSitioLigacao);
				//segundo sítio
				gSitioLigacao = CondutanciaSitioLigacao(itl->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
				itl->second.Conectar(is2->first, gSitioLigacao);
				is2->second.Conectar(cont, gSitioLigacao);
			}
			//atualiza informações do último objeto
			x0 = x;
			y0 = y;
			z0 = z;
			r0 = rs;
			is1 = is2;
			if (fronteiraDireita) { //se atingiu a fronteira direita, volta para a esquerda.
				fronteiraEsquerda=true;
				fronteiraDireita=false;
			} else {
				fronteiraEsquerda=false;
				fronteiraDireita=false;
			}
			if (raiosSitios->size() == 0) {
				aindaExistemSitios = false;
			}
		}
		// se criou a última ligação, mas ainda existem sítios a serem alocados,
		// extrapola as ligações de raio 1 até alocar todos os sítios.
		if (rl == 1 && raiosSitios->size() > 0) {
			++rl; //incrementa a ligação, pois será decrementada no for e voltará a ser 1
			aindaExistemSitios = true;
		}
	}
	delete raiosSitios;
	std::cout << "Ordenado os objetos (sítios e ligações) pelo eixo x ..." << std::endl;
	std::map<int,int> objAntToObjAtual;
	ptrMatObjsRede->matrizObjetos.clear(); // Matriz de objetos final
	cont = 0;
	std::cout << "Copia os objetos, ja ordenados em x, da matriz temporaria para a metriz definitiva..." << std::endl;
	for (auto &xto : xToObj ) {
		++cont;
		ptrMatObjsRede->matrizObjetos[cont] = matrizObjetosTemp[xto.second];
		objAntToObjAtual[xto.second] = cont;
		//std::cout << "[x=" << xto.first << ",\t\t\tObjAnt=" << xto.second<< ",\t\t\tObjAtu=" << cont << "]" << std::endl;
	}
	matrizObjetosTemp.clear();

	//percorre a matriz de objetos e atualiza os rótulos dos objetos conectados
	std::map<int, double> conexoesAtu;
	//std::map<int, double> conexoesAnt;
	for ( auto &objs : ptrMatObjsRede->matrizObjetos ) {
		//copia as conexões para o map temporário
		conexoesAtu.clear();
		for ( auto &obj : objs.second.SConexao() ) { //a condutância premanece a mesma, só mudou o rótulo do objeto
			conexoesAtu.insert(make_pair(objAntToObjAtual[obj.first], obj.second));
		}
		//limpa o vetor de conexoes do objeto
		objs.second.SConexao().clear();

		//copia para os objetos as novas conexões salvas no map temporário
		for ( auto &obj : conexoesAtu ) {
			objs.second.Conectar(obj.first, obj.second);
		}
	}
	objAntToObjAtual.clear();
	xToObj.clear();
	return true;
}

// Cria rede de percolação com sítios em posições e tamanhos aleatórios conforme a distribuição de tamnho de poros
// As ligações entre os sítios atendem a distribuição do tamanho de gargantas.
// Este modelo aloca um porcentagem de sítios nas fronteiras para que hajam fronteiras bem definidas.
bool CRedeDePercolacao::Modelo6() {
	std::cout << "Criando rede atraves do Modelo 6..." << std::endl;
	//Variáveis auxiliares
	TCMatriz3D<bool> pm(nx, ny, nz);
	int area = nx*ny*nz; //área da matriz 3D (em pixeis)
	int areaFronteira = ny*nz;
	int x, y, z; //posição na matriz
	int x0, y0, z0, r0; //posição na matriz, raio e objeto trabalhando no instatne anterior
	int im, jm, km;
	int x_rs, y_rs, z_rs;
	int yMin, yMax, zMin,zMax; //delimitam a região de alocação do sítio no plano y,z
	int cont = 0;
	int contS = 0;
	int rs = 0; //raio sítio
	int pos;
	int diametro;
	int tamLigacao; //tamanho da ligação a ser criada.
	int tamMaxLig; //tamanho total das ligações a serem criadas.
	int numSCRMIRG; //numero de sítios com raio maior ou igual ao raio da ligação
	double distancia;
	long double gSitioLigacao; //Condutância entre sítio e ligação
	long double xSolver	= 0.0; //variável utilizada para setar o valor x do parametro de solver do objeto. Utilizado na simulação.
	long double phiPoros; //espaço poroso ocupado pela matriz de poros
	long double phiSFE = 0.0; //espaço poroso ocupado pelos sítios da fronteira esquerda
	bool cabe = false; //flag que indicará se a esfera cabe na região sem sobrepor outras esferas.
	bool cabeEmAlgumaPosicao;
	CBCd3453D * esfera;
	std::map<int, CObjetoRedeDePercolacao> matrizObjetosTemp; // Matriz de objetos temporária;
	std::map<int, CObjetoRedeDePercolacao>::iterator is1; //iterator para o primeiro sítio
	std::map<int, CObjetoRedeDePercolacao>::iterator is2; //iterator para o segundo sítio
	std::map<int, CObjetoRedeDePercolacao>::iterator itl; //iterator para a conexão
	std::multimap<int, int> xToObj; // xToObj será uma referência para ordenar os objetos do menor para o maior valor de x.
	std::vector<std::pair<int,int>> posicoes;
	std::vector<bool> cabeNaPosicao;


	//========================================= SÍTIOS E LIGAÇÕES ===========================================
	//Tratando as distribuições
	int tamVetDistPor = dtpg.first->distribuicao.size();
	int tamVetDistGar = dtpg.second->distribuicao.size();

	//se por algum motivo (problema) a distribuição de gargantas for maior que a distribuição de poros, inverte as distribuições.
	if (tamVetDistPor < tamVetDistGar) {
		std::swap(dtpg.first, dtpg.second);
		std::swap(tamVetDistPor, tamVetDistGar);
		cout << "Realizou swap nas distribuições" << endl;
	}

	cout << "tamVetDistPor = " << tamVetDistPor << endl;
	cout << "tamVetDistGar = " << tamVetDistGar << endl;

	int x_fronteiraDireita = nx-tamVetDistPor-1;
	phiPoros = dtpg.first->AreaObjetos(); //porosidade da imagem (poros)

	//Cria vetor de raio dos sítios
	std::vector<int> * raiosSitios = CriarVetorDeRaiosDosSitiosByDTP();
	std::vector<int> numSitiosByRaio(tamVetDistPor+1,0);
	for (auto &r: *raiosSitios) {//acumula o número de sítios a serem criados, para cada tamanho de raio.
		numSitiosByRaio[r] = numSitiosByRaio[r]+1;
	}
	int totalSitios = raiosSitios->size();

	//Ligações
	long double phiGargantas	= dtpg.second->AreaObjetos(); //porosidade da imagem (garganta)
	long double areaTotalGargantas	= (area*phiGargantas)/100.0;
	std::vector<double> areaLigacoesAcumuladas(tamVetDistGar+1, 0.0);
	std::vector<double> areaLigacoes(tamVetDistGar+1, 0.0);
	for ( int r=1; r<=tamVetDistGar; ++r ) {
		areaLigacoes[r] = dtpg.second->distribuicao[r-1]*areaTotalGargantas;
	}

	x = 0;
	bool fronteiraEsquerda = true;
	bool primeiroSitio = true;
	bool fronteiraDireita = false;
	bool aindaExistemSitios = false;
	bool fronteira = false;
	bool interligados = false;
	bool finalizarRamo = false;
	bool testarFinalizacao = false;
	//Percorre, do maior para o menor, os raios de ligações a serem criadas
	for (int rl=tamVetDistGar; rl > 0; --rl ) {
		cout << "Criando ligações de raio=" << rl << endl;
		//verifica quantos sítios disponíveis possuem raio maior ou igual ao raio da ligação
		numSCRMIRG = 0;
		for (int r=rl; r < numSitiosByRaio.size(); ++r) {
			numSCRMIRG += numSitiosByRaio[r];
		}
		//se não existir mais de um sítio maior que a ligação, vai para o próximo riao de ligação.
		if (numSCRMIRG <= 1) {
			std::cout << "===========>Nao encontrou sitio com raio maior ou igual a " << rl << std::endl;
			continue;
		}
		//calcula o tamanho total das ligações e o tamanho de cada ligação
		tamMaxLig = (int)round((double)areaLigacoes[rl]/(double)numPixeisCirculo[rl]);
		tamLigacao = (int)round((float)tamMaxLig/((float)numSCRMIRG - 1.0));
		if (tamLigacao < 1) // o comprimento da garganta, a princípio, não será menor que seu raio.
			tamLigacao = 1;

		cout << "Tamanho das ligações para o raio " << rl << ": " << tamLigacao << endl;

		//Inicio da alocação de sítios e ligações. Entra no loop até alocar todas as gargantas de raio rl
		//Após alocar todas as garganas, para todos os raios, se ainda existirem sítios  a serem alocados, continua no loop
		while ( (areaLigacoesAcumuladas[rl] < areaLigacoes[rl]) || aindaExistemSitios ) {
			finalizarRamo = false;
			testarFinalizacao = (bool)Random(0,1); //aleatóriamente testa se um ramo pode ser finalizado antes de chegar na fronteira
			if ( ! fronteiraEsquerda && testarFinalizacao) { //Não estamos na fronteira esquerda. Estuda a possibilidade de integrar ramo atual a outro ramo já existente
				//Sorteia a partir de qual posição do eixo x irá tentar a finalização do ramo.
				// Este procedimento evita que a maioria dos ramos sejam finalizado próximos a fronteira esquerda.
				if ( x0 >= Random(nx/3, nx-tamVetDistPor-3) ) {
					//verifica se existe sítio na redondeza, com raio maior ou igual ao raio da ligação e cuja distância do sítio anterior seja igual ao tamanho da ligação.
					for ( auto &is: matrizObjetosTemp ) {
						if ( is.second.Tipo() == SITIO && is.second.Raio() >= rl) {
							x = is.second.PontoCentral_X();
							y = is.second.PontoCentral_Y();
							z = is.second.PontoCentral_Z();
							rs = is.second.Raio();
							distancia = round( DistanciaEntrePontos(x0,y0,z0,x,y,z) - (double)r0 - (double)rs );
							if (tamLigacao == distancia ) { //atendeu a distâcia. Verifica se não estão interligados...
								interligados = false;
								for (auto &c: is.second.SConexao()) { //para cada ligação já conectada ao sítio
									itl = matrizObjetosTemp.find(c.first); //pega o iterator para a ligação
									for (auto &o: itl->second.SConexao()) { //percorre a lista de objetos conectados a ligação
										if ( o.first == is1->first ) { //verifica se o primerio sítio está conectado ao segundo através da ligação
											interligados = true;
											std::cout << "Ja estao interligados! Procurando outro sitio..." << std::endl;
											break; //se estiverem interligado, não precisa mais procurar.
										}
									}
									if( interligados ) {//se estiverem interligado, não precisa mais procurar.
										break; //sai do loop de conexões
									}
								}
								if (!interligados) {
									finalizarRamo = true;
									is2 = matrizObjetosTemp.find(is.first);
									break;
								}
							}
						}
					}
				}
			}
			if (finalizarRamo) {
				cout << "======================FINALIZOU O RAMO=======================" << endl;
				//Ligação. Conecta os	objetos...
				++cont;
				tamMaxLig -= tamLigacao;
				areaLigacoesAcumuladas[rl] += NumPixeisCilindro(rl, tamLigacao);
				matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, LIGACAO, NumPixeisCilindro(rl, tamLigacao));
				itl = matrizObjetosTemp.find(cont);
				itl->second.PontoCentral( (int)round((x0+x)/2),(int)round((y0+y)/2),(int)round((z0+z)/2),3*rl );
				itl->second.Raio( rl );
				xSolver = (long double)itl->second.PontoCentral_X();
				itl->second.X(xSolver);

				// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
				xToObj.insert(pair<int, int>(itl->second.PontoCentral_X(),cont));

				//Cálculo de condutâncias e realiza conexões
				distancia = (double)tamLigacao;
				itl->second.Propriedade ( CondutanciaLigacao(itl->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao) );
				//primeiro sítio
				gSitioLigacao = CondutanciaSitioLigacao(itl->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
				itl->second.Conectar(is1->first, gSitioLigacao);
				is1->second.Conectar(cont, gSitioLigacao);
				//segundo sítio
				gSitioLigacao = CondutanciaSitioLigacao(itl->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
				itl->second.Conectar(is2->first, gSitioLigacao);
				is2->second.Conectar(cont, gSitioLigacao);
				//atualiza informações do último objeto
				x0 = x;
				y0 = y;
				z0 = z;
				r0 = rs;
				is1 = is2;
				fronteiraEsquerda=true; //volta para a fronteira esquerda.
			} else {
				rs = 0;
				//Percorre o vetor de raios em busca de algum que seja maior ou igual ao raio da ligação
				for (std::vector<int>::iterator itr = raiosSitios->begin(); itr!=raiosSitios->end(); ++itr){
					if (*itr >= rl) {
						rs = *itr;
						raiosSitios->erase(itr);
						--numSCRMIRG;
						break;
					}
				}
				if (rs == 0) { //Não encontrou sítio. Sai do loop indo para o próximo raio de ligação.
					std::cout << "Nao encontrou sitio com raio maior ou igual ao raio da ligacao: " << rl << std::endl;
					break;
				}
				if (fronteiraEsquerda) {
					if (!primeiroSitio) {
						fronteira = (bool)Random(0,1); //toda vez que estiver na fronteira esquerda, seta a flag aleatoriamente de forma que, ora o ramo irá iniciar na fronteira e hora o ramo irá iniciar de um sítio sorteado.
					} else { //se estivermos na fronteira esquerda e for o primeiro sítio/ramo a ser alocado força que inicie na fronteira, senão irá buscar sítio aleatório na matriz de objetos e dará falha de segmentação
						primeiroSitio = false;
						fronteira = true;
					}
				}
				// Verifica em que possição do eixo x o sítio será alocado
				if ( fronteiraEsquerda && fronteira /*&& phiSFE < phiPoros*/ ) { // estamos na fronteira esquerda e o ramo partirá de um novo sítio.
					x = tamVetDistPor; //precisamos respeitar o maior raio de sítio
				} else {
					if ( fronteiraEsquerda /*&& ! fronteira*/ ) { // estamos na fronteira esquerda mas o ramo partirá de um sítio sorteado
						fronteiraEsquerda = false;
						++numSCRMIRG; //como irá partir de um sítio existente, precisa recalcular o tamanho da ligação levando em consideração um sítio a mais.
						tamLigacao = (int)round((float)tamMaxLig/((float)numSCRMIRG - 1.0));
						if (tamLigacao < 1) // o comprimento da ligação, a princípio, não será menor que seu raio.
							tamLigacao = 1;
						do { //sorteia o sítio (não pode ser da fronteira direita!)
							pos = Random(1,matrizObjetosTemp.size());
							is1 = matrizObjetosTemp.find(pos);
						} while ( ( is1->second.Tipo() == LIGACAO ) || (is1->second.PontoCentral_X() >= x_fronteiraDireita) || (is1->second.Raio() < rl ) );
						x0 = is1->second.PontoCentral_X();
						y0 = is1->second.PontoCentral_Y();
						z0 = is1->second.PontoCentral_Z();
						r0 = is1->second.Raio();
					}
					if ( x0+r0+tamLigacao+rs >= x_fronteiraDireita) { // estamos na fronteira direita, também é preciso respeitar o maior raio de sítio
						fronteiraDireita = true;
						x = x_fronteiraDireita;
					} else { //estamos entre as fronteiras. sorteia valor para x.
						x = Random(x0, x0+r0+tamLigacao+rs);
					}
				}
				//calcula o diametro da esfera
				diametro = ((2*rs)+1);
				//cria esfera de raio correspondente.
				esfera = new CBCd3453D(diametro);
				if (fronteiraEsquerda) { //se estiver na fronteira esquerda, considera todo o plano y,z para alocar o sítio
					yMin = rs;
					yMax = ny-rs-1;
					zMin = rs;
					zMax = nz-rs-1;
					// Sortear posições testando se a esfera cabe sem sobrepor outras esferas ou ultrapassar a borda.
					do {
						y = Random(yMin, yMax);
						z = Random(zMin, zMax);
						x_rs = x-rs;
						y_rs = y-rs;
						z_rs = z-rs;
						cabe = true;
						for (int i=0; i<diametro && cabe; ++i) {
							im = i+x_rs;
							for (int j=0; j<diametro && cabe; ++j) {
								jm = j+y_rs;
								for (int k=0; k<diametro && cabe; ++k) {
									km = k+z_rs;
									if ( esfera->data3D[i][j][k]!=0 && pm.data3D[im][jm][km]!=0 ) {
										cabe = false;
									}
								}
							}
						}
						if (cabe) { //desenha a esfera
							for (int i=0; i<diametro; ++i) {
								im = i+x_rs;
								for (int j=0; j<diametro; ++j) {
									jm = j+y_rs;
									for (int k=0; k<diametro; ++k) {
										km = k+z_rs;
										if ( esfera->data3D[i][j][k]!=0 ) {
											pm.data3D[im][jm][km]=1;
										}
									}
								}
							}
						} else {
							std::cout << "A esfera não cabe nesta posição! Procurando outra..." << std::endl;
						}
					} while (!cabe);
				} else { //no centro ou na fronteira direita, considera somente uma pequena área do plano y,z, suficiente para atingir o raio da ligação
					yMin = (y0-r0-rs-tamLigacao-1 < rs) ? rs : y0-r0-rs-tamLigacao-1;
					zMin = (z0-r0-rs-tamLigacao-1 < rs) ? rs : z0-r0-rs-tamLigacao-1;
					yMax = (y0+r0+rs+tamLigacao+1 > ny-rs-1) ? ny-rs-1 : y0+r0+rs+tamLigacao+1;
					zMax = (z0+r0+rs+tamLigacao+1 > nz-rs-1) ? nz-rs-1 : z0+r0+rs+tamLigacao+1;
					posicoes.clear();
					cabeNaPosicao.clear();
					cabeEmAlgumaPosicao = false;
					x_rs = x-rs;
					for ( y=yMin; y<=yMax; ++y ) {
						for ( z=zMin; z<=zMax; ++z ) {
							distancia = round( DistanciaEntrePontos(x0,y0,z0,x,y,z) - (double)r0 - (double)rs );
							if (tamLigacao == distancia ) {
								y_rs = y-rs;
								z_rs = z-rs;
								cabe = true;
								for (int i=0; i<diametro && cabe; ++i) {
									im = i+x_rs;
									for (int j=0; j<diametro && cabe; ++j) {
										jm = j+y_rs;
										for (int k=0; k<diametro && cabe; ++k) {
											km = k+z_rs;
											if ( esfera->data3D[i][j][k]!=0 && pm.data3D[im][jm][km]!=0 ) {
												cabe = false;
											}
										}
									}
								}
								if (cabe)
									cabeEmAlgumaPosicao = true;
								cabeNaPosicao.push_back(cabe);
								posicoes.push_back(make_pair(y,z));
								//std::cout << "TamLigacao: " << tamLigacao << " | Distancia: " << (int)distancia << " | yMin: " << yMin << " | yMax: " << yMax << " | zMin: " << zMin << " | zMax: " << zMax << std::endl;
								//std::cout << "x0 = " << x0 << " | y0 = " << y0 << " | z0 = " << z0 << " | r0 = " << r0 << std::endl;
								//std::cout << "x = " << x << " | y = " << y << " | z = " << z  << " | rs = " << rs << std::endl;
							}
						}
					}
					if (cabeEmAlgumaPosicao){ //sorteia a posição; pega y e z; desenha a esfera.
						do {
							pos = Random(0,posicoes.size()-1);
						} while ( ! cabeNaPosicao[pos] );
						y = posicoes[pos].first;
						z = posicoes[pos].second;
					} else if ( posicoes.size() > 0 ) { //não cabe em nenhuma posição; Pega qualquer posição e sobrepõe.
						pos = Random(0,posicoes.size()-1);
						y = posicoes[pos].first;
						z = posicoes[pos].second;
					} else { //o tamanho da ligação não corresponde em nenhuma das posições. Desenha uma ligação reta paralela a x. Dificilmente este caso acontecerá e se acontecer irá subestimar a área de ligações
						y = y0;
						z = z0;
					}
				}
				delete esfera;
				//Definida a posição do sítio. Alocar e interligar
				std::cout << "Alocando sítio " << ++contS << " de " << totalSitios << std::endl;
				++cont;
				matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, SITIO, NumPixeisEsfera(rs));
				is2 = matrizObjetosTemp.find(cont);
				is2->second.PontoCentral( x, y, z, 3*rs );
				is2->second.Raio( rs );
				xSolver = (long double)x;
				is2->second.X(xSolver); //seta o x do solver que será utilizado na simulação;
				is2->second.Propriedade( CondutanciaSitio(is2->second.Raio(), dimensaoPixel, fatorAmplificacao) );

				// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
				xToObj.insert(pair<int, int>(is2->second.PontoCentral_X(),cont));

				if ( fronteiraEsquerda ) {
					is2->second.Contorno(CContorno::ETipoContorno::WEST); //esquerda
					phiSFE += PhiDisco(rs, areaFronteira);
				} else { // centro ou fronteira direita
					//Ligação. Conecta os	objetos...
					++cont;
					areaLigacoesAcumuladas[rl] += NumPixeisCilindro(rl, tamLigacao);
					tamMaxLig -= tamLigacao;
					matrizObjetosTemp[cont] = CObjetoRedeDePercolacao(ptrMatObjsRede, LIGACAO, NumPixeisCilindro(rl, tamLigacao));
					itl = matrizObjetosTemp.find(cont);
					itl->second.PontoCentral( (int)round((x0+x)/2),(int)round((y0+y)/2),(int)round((z0+z)/2),3*rl );
					itl->second.Raio( rl );
					xSolver = (long double)itl->second.PontoCentral_X();
					itl->second.X(xSolver);
					// Alimenta matriz que referencia aos objetos de forma que estes fiquem ordenados em x
					xToObj.insert(pair<int, int>(itl->second.PontoCentral_X(),cont));

					//Cálculo de condutâncias e realiza conexões
					distancia = (double)tamLigacao;
					itl->second.Propriedade ( CondutanciaLigacao(itl->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao) );
					//primeiro sítio
					gSitioLigacao = CondutanciaSitioLigacao(itl->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
					itl->second.Conectar(is1->first, gSitioLigacao);
					is1->second.Conectar(cont, gSitioLigacao);
					//segundo sítio
					gSitioLigacao = CondutanciaSitioLigacao(itl->second.Raio(), distancia, dimensaoPixel, fatorAmplificacao );
					itl->second.Conectar(is2->first, gSitioLigacao);
					is2->second.Conectar(cont, gSitioLigacao);
					if ( fronteiraDireita ) {
						is2->second.Contorno(CContorno::ETipoContorno::EST); //direita
						//ao alocar um sítio na fronteira direita, precisa recalcular o tamanho da garganta, pois a princípio
						//este sítio só terá uma garganta conectada a ele.
						tamLigacao = (int)round((float)tamMaxLig/((float)numSCRMIRG - 1.0));
						if (tamLigacao < 1) // o comprimento da garganta, a princípio, não será menor que seu raio.
							tamLigacao = 1;
					} else {
						is2->second.Contorno(CContorno::ETipoContorno::CENTER); //centro
					}
				}
				//atualiza informações do último objeto
				x0 = x;
				y0 = y;
				z0 = z;
				r0 = rs;
				is1 = is2;
				if (fronteiraDireita) { //se atingiu a fronteira direita, volta para a esquerda.
					fronteiraEsquerda=true;
					fronteiraDireita=false;
				} else {
					fronteiraEsquerda=false;
					fronteiraDireita=false;
				}
				if (raiosSitios->size() == 0) {
					aindaExistemSitios = false;
				}
			}
		}
		// se criou a última ligação, mas ainda existem sítios a serem alocados,
		// extrapola as ligações de raio 1 até alocar todos os sítios.
		if (rl == 1 && raiosSitios->size() > 0) {
			++rl; //incrementa a ligação, pois será decrementada no for e voltará a ser 1
			aindaExistemSitios = true;
		}
	}
	delete raiosSitios;
	std::cout << "Ordenado os objetos (sítios e ligações) pelo eixo x ..." << std::endl;
	std::map<int,int> objAntToObjAtual;
	ptrMatObjsRede->matrizObjetos.clear(); // Matriz de objetos final
	cont = 0;
	std::cout << "Copia os objetos, ja ordenados em x, da matriz temporaria para a metriz definitiva..." << std::endl;
	for (auto &xto : xToObj ) {
		++cont;
		ptrMatObjsRede->matrizObjetos[cont] = matrizObjetosTemp[xto.second];
		objAntToObjAtual[xto.second] = cont;
		//std::cout << "[x=" << xto.first << ",\t\t\tObjAnt=" << xto.second<< ",\t\t\tObjAtu=" << cont << "]" << std::endl;
	}
	matrizObjetosTemp.clear();

	//percorre a matriz de objetos e atualiza os rótulos dos objetos conectados
	std::map<int, double> conexoesAtu;
	//std::map<int, double> conexoesAnt;
	for ( auto &objs : ptrMatObjsRede->matrizObjetos ) {
		//copia as conexões para o map temporário
		conexoesAtu.clear();
		for ( auto &obj : objs.second.SConexao() ) { //a condutância premanece a mesma, só mudou o rótulo do objeto
			conexoesAtu.insert(make_pair(objAntToObjAtual[obj.first], obj.second));
		}
		//limpa o vetor de conexoes do objeto
		objs.second.SConexao().clear();

		//copia para os objetos as novas conexões salvas no map temporário
		for ( auto &obj : conexoesAtu ) {
			objs.second.Conectar(obj.first, obj.second);
		}
	}
	objAntToObjAtual.clear();
	xToObj.clear();
	return true;
}

// Grava em disco, com o nome informado, os objetos identificados.
bool CRedeDePercolacao::SalvarListaObjetos(std::string nomeArquivo) const {
	return ptrMatObjsRede->SalvarListaObjetos(nomeArquivo, nx, ny, nz, fatorAmplificacao, dimensaoPixel);
}

// Grava em disco, no formato do Grafo, com o nome informado, os objetos identificados.
bool CRedeDePercolacao::SalvarListaObjetosGrafo(std::string nomeArquivo) const {
	return ptrMatObjsRede->SalvarListaObjetosGrafo(nomeArquivo);
}

// Write: Salva Rede em disco
void CRedeDePercolacao::Write ( std::ostream& os ) const {
	os.setf ( ios::left );
	//ptrMatObjsRede->matrizObjetos.erase(0);
	//out << setw ( 5 ) << static_cast<uint8_t> ( Tipo() ) ;
	os << "# " << ptrMatObjsRede->matrizObjetos.size()  << " " << nx << " " << ny << " " << nz << endl;
	//os << "# fatorAmplificacao: " << fatorAmplificacao << endl;
	//os << "# dimensaoPixel: " << dimensaoPixel << endl;
	os << "Obj.  X    Y    Z    Raio Tipo N.Voxeis Condutância N.ObjsCon LstObjsCon LstCondObjsCon" << endl;
	for ( auto objeto_i :  ptrMatObjsRede->matrizObjetos ) {
		os << std::left << std::setw(6) << objeto_i.first;
		os << objeto_i.second;
	}
}

ostream& operator<< (ostream& os, CRedeDePercolacao& obj){
	obj.Write(os);
	return os;
}
