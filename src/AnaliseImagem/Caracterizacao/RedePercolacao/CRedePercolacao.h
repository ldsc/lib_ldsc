#ifndef CRedePercolacao_H
#define CRedePercolacao_H

#include <AnaliseImagem/Caracterizacao/Distribuicao/CDistribuicaoTamanhoPorosGargantas.h>
#include <AnaliseImagem/Geometria/Bola/BCDiscreta/CBCd3453D.h>
#include <EstruturaDados/CRedeContorno.h>
#include <AnaliseImagem/Matriz/CMatrizObjetoRede.h>
#include <MetNum/Matriz/CMatrizObjetoImagem.h>
#include <cstdlib>
#include <ctime>

/**
 * @brief A classe CRedePercolacao representa meios porosos tridimensionais
 * de forma simplificada, através de redes de percolação 3D, irregulares e com
 * número de cordenação variável.
 *
 * A distribuição de tamanho de poros e gargantas de imagens binárias ou
 * de imagens em tons de cinza segmentadas pela classe CAberturaDilatacao3D, é
 * calculada através da classe CDistribuicaoTamanhoPorosGargantas.
 *
 * Se a imagem recebída em Go for uma imagem binária, esta será segmantada através
 * da classe CAberturaDilatacao3D.
 *
 * A classe CDistribuicaoTamanhoPorosGargantas, utiliza a classe CDistribuicao3D
 * para calcular as distribuições de tamanho de poros e gargantas.
 *
 * @author Leandro Puerari <puerari@gmail.com>
 * @author André Duarte Bueno <andreduartebueno@gmail.com>
*/
class CRedePercolacao : public CRedeContorno
{		// Atributos
	public:
		/// Flag que seta se os pixeis que compõem um esfera ou um cilindro devem ser calculados ou obtidos do vetor (default=false).
		bool calcPixeis {false};

		///Dimensão x da rede
		unsigned short int nx{0};

		///Dimensão y da rede
		unsigned short int ny{0};

		///Dimensão z da rede
		unsigned short int nz{0};

	private:
		///Par de ponteiros para a classe CDistribuicao, representado respectivamente a distribuição de tamanho de portos e a distribuição de tamanho de gargantas.
		std::pair< CDistribuicao3D *, CDistribuicao3D * > dtpg;

		///Vetor estático onde cada elemento corresponde ao número de pixeis existente em uma bola com raio correspondente ao valor da chave.
		static std::vector<unsigned int> numPixeisBola;

		///Vetor estático onde cada elemento corresponde ao número de pixeis existente em um circulo com raio correspondente ao valor da chave.
		static std::vector<unsigned int> numPixeisCirculo;

		///Vetor estático onde cada elemento corresponde ao perímetro de um circulo com raio correspondente ao valor da chave.
		static std::vector<unsigned short int> perimetroCirculo;

		///Número de sítios representados na rede
		unsigned int numSitios{0};

		///Número de ligações representadas na rede
		unsigned int numLigacoes{0};

		// Construtores / Destrutor
	public:
		/// Construtor (recebe dimensões da rede que será criada)
		CRedePercolacao(unsigned short int _nx, unsigned short int _ny, unsigned short int _nz );

		/// Destrutor
		virtual ~CRedePercolacao();

		// Métodos
	public:
		/// Sementa a imagem, executa o cálculo das distribuições e cria a rede de percolação.
		bool Go( TCImagem3D<bool> *&_pm, int _raioMaximo, int _raioDilatacao, int _fatorReducao, int _incrementoRaio, EModelo _modelo, int _indice, int _fundo, CDistribuicao3D::Metrica3D _metrica = CDistribuicao3D::d345 );

		/// Executa o cálculo das distribuições e cria a rede de percolação.
		bool Go( TCImagem3D<int> *&_pm, CDistribuicao3D::Metrica3D _metrica = CDistribuicao3D::d345 );

		/// Grava em disco, com o nome informado, os objetos identificados.
		bool SalvarListaObjetos(std::string nomeArquivo) const;

		/// Grava em disco, no formato do Grafo, com o nome informado, os objetos identificados.
		bool SalvarListaObjetosGrafo(std::string nomeArquivo) const;

		/// Salva Rede em disco
		void Write ( std::ostream& out ) const;

		/// Retorna o número de sítios representados na rede
		inline unsigned int NumSitios(){
			return numSitios;
		}

		/// Retorna o número de ligações representadas na rede
		inline unsigned int NumLigacoes(){
			return numLigacoes;
		}

	private:
		/// Método chamado por Go para criar a rede de percolação.
		bool ExecutadaPorGo( );

		/// Returna inteiro randômico entre min e max [inclusive] (srand é setado no contrutor da classe).
		inline int Random(const int &min, const int &max) { return rand()%(max-min+1)+min; }

		/// Gera double randômico entre 0.0 e 1.0
		inline double DRandom ( ) { return ( ( (double)rand() ) / ( (double)RAND_MAX ) ); }

		/// Retorna o número de pixeis em uma esfera. Se calcPixeis for verdadeiro, o valor será calculado, senão será obtido do vetor
		inline int NumPixeisEsfera( const int & _raio) {
			return (calcPixeis) ? (int)round((4.0*M_PI*(float)_raio*(float)_raio*(float)_raio)/3.0) : numPixeisBola[_raio];
		}

		/// Retorna o número de pixeis em um cilindro. Se calcPixeis for verdadeiro, o valor será calculado, senão será obtido do vetor
		inline int NumPixeisCilindro( const int & _raio, const int & _comprimento) {
			return (calcPixeis) ? (int)round(M_PI*(float)_raio*(float)_raio*(float)_comprimento) : _comprimento*numPixeisCirculo[_raio];
		}

		/// Retorna a porosidade da esfera (em %). Se calcPixeis for verdadeiro, o número de pixeis será calculado, senão, será obtido do vetor.
		inline double PhiEsfera( const int & _raio, const int & _area) {
			return ((double)NumPixeisEsfera(_raio)/(double)_area)*100.0;
		}

		/// Retorna a porosidade do cilindro (em %). Se calcPixeis for verdadeiro, o número de pixeis será calculado, senão, será obtido do vetor.
		inline double PhiCilindro( const int & _raio, const int & _comprimento, const int & _area) {
			return ((double)NumPixeisCilindro(_raio, _comprimento)/(double)_area)*100.0;
		}

		/// Calcula distância entre dois pontos no espaço 3D.
		inline double DistanciaEntrePontos(const int &x1, const int &y1, const int &z1, const int &x2, const int &y2, const int &z2){
			int vx = x2-x1;
			int vy = y2-y1;
			int vz = z2-z1;
			return sqrt( vx*vx + vy*vy + vz*vz );
		}

		/// Calcula e retorna o raio hidráulico do circulo de raio informado
		inline double RaioHidraulicoCirculo(const int & _raio) {
			return (calcPixeis) ? ( (double)_raio / 2.0 ) : ( (double)numPixeisCirculo[_raio] / (double)perimetroCirculo[_raio] );
		}

		/// Calcula a condutância de objetos do tipo sítio
		double CondutanciaSitio (CObjetoRedeDePercolacao &objetoImagem, double dimensaoPixel=1.0, double fatorAmplificacao=1.0);

		/// Calcula a condutância de objetos do tipo ligação
		double CondutanciaLigacao (CObjetoRedeDePercolacao &objetoImagem, double &comprimento, double dimensaoPixel=1.0, double fatorAmplificacao=1.0);

		/// Calcula a condutância entre um sítio e uma ligação (considera apenas metade da ligação, pois a outra metade será considerada na ligação com outro sítio)
		double CondutanciaSitioLigacao (CObjetoRedeDePercolacao &objImgSitio, CObjetoRedeDePercolacao &objImgLigacao, double &comprimento, double dimensaoPixel=1.0, double fatorAmplificacao=1.0);

		// -----------------------------------------------------------------Friend
		friend ostream& operator<< (ostream& os, CRedePercolacao& obj);
		//friend istream& operator>> (istream& is, CRedePercolacao& obj);

};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
ostream& operator<< (ostream& os, CRedePercolacao& obj);
// istream& operator>> (istream& is, CRedePercolacao& obj);
#endif // CRedePercolacao_H
