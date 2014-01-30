#include "CRedeDePercolacao.h"

// Construtor matriz binária
CRedeDePercolacao::CRedeDePercolacao( TCImagem3D<bool> *&_pm, int & _raioMaximo, int & _raioDilatacao, int & _fatorReducao, int & _incrementoRaio, EModelo _modelo, int _indice, int _fundo )
	: CDistribuicaoTamanhoPorosGargantas( _pm, _raioMaximo, _raioDilatacao, _fatorReducao, _incrementoRaio, _modelo, _indice, _fundo ),
		pm(NULL)
{

}

// Construtor imagem tons de cinza
CRedeDePercolacao::CRedeDePercolacao( TCImagem3D<int> *&_pm )
	: CDistribuicaoTamanhoPorosGargantas( _pm ), pm (NULL)
{

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

// Executa o cálculo das distribuições e cria a rede de percolação.
bool CRedeDePercolacao::Go(  int &nx, int &ny, int &nz, CDistribuicao3D::Metrica3D _metrica ) {
	// Determina tamanho mínimo e máximo para a rede de percolação
	int min = 100;
	int max = 1000;
	nx = ( nx < min ) ? min : nx;
	ny = ( ny < min ) ? min : ny;
	nz = ( nz < min ) ? min : nz;
	nx = ( nx > max ) ? max : nx;
	ny = ( ny > max ) ? max : ny;
	nz = ( nz > max ) ? max : nz;
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
	// Próximos passos:
	// Sortear raios de esferas até atingir a porosidade da matriz de sítios.
	// Ordenar os raios do maior para o menor de forma que as esferas maiores serão criadas primeiro.
	// Sortear posições testando se a esfera cabe sem sobrepor outras esferas ou ultrapassar a borda.
	// Representar as esferas (sitios) na matriz 3D.

	// Partindo da borda superior, conectar a borda aos sítios mais próximos.

	// Percorrer a matriz em Y e para cada sítio encontrado:
	// =>Sortear número de coordenação (Z);
	// =>Sortear raio das Z ligações;
	// =>Conectar o sítio a Z sítios próximos e ir acumulando a porosidade representada pelas ligações;
}
