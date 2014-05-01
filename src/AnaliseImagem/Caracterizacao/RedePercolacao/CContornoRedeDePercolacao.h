#ifndef CContornoRedeDePercolacao_h
#define CContornoRedeDePercolacao_h

#include <AnaliseImagem/Caracterizacao/RedePercolacao/CRedeDePercolacao.h>
//#include <EstruturaDados/CRedeContorno.h>
#include <MetNum/Contorno/CContornoCentro.h>

using namespace std;

/**
 * @short: A \class CContornoRedeDePercolacao representa uma rede de percolação sujeita a condições de contornos;
 * Herdeira da classe CRedeDePercolacao, a classe CContornoRedeDePercolacao acrescenta a idéia
 * de que uma rede pode estar submetida a determinadas condições de contorno.
 * Na prática acrescenta um vetor que armazena os objetos de contorno.
 * Note que existe uma hierarquia para objetos de contorno, este
 * vetor terá ponteiros para objetos da classe base da hierarquia de contornos.
 * @author 	Leandro Puerari <puerari@gmail.com>
 * @author 	André Duarte Bueno <andreduartebueno@gmail.com>
*/
class CContornoRedeDePercolacao : public CRedeDePercolacao//, public CRedeContorno
{
		// --------------------------------------------------------------Atributos
	public:
		/// Usa-se contorno[i] para obter ponteiro para contorno i.
		std::vector < CContornoCentro * >contorno;

		// -------------------------------------------------------------Construtores
		/// Cria os objetos de contorno e incluí no vetor contorno.
		CContornoRedeDePercolacao (unsigned int nx, unsigned int ny, unsigned int nz, unsigned long int _numero_contornos=0);

		// --------------------------------------------------------------Destrutor
		/// Delete os objetos de contorno do vetor contorno.
		virtual ~CContornoRedeDePercolacao ();

		// ----------------------------------------------------------------Métodos
		// --------------------------------------------------------------------Get
		// --------------------------------------------------------------------Set
		// -----------------------------------------------------------------Friend
		friend std::ostream & operator<< (std::ostream & os, const CContornoRedeDePercolacao & obj);
		//friend istream& operator>> (istream& is, CContornoRedeDePercolacao& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream & operator<< (std::ostream & os, const CContornoRedeDePercolacao & obj);
// istream& operator>> (istream& is, CContornoRedeDePercolacao& obj);

#endif
