#ifndef CObjetoImagem_h
#define CObjetoImagem_h

/*
=========================================================================
PROJETO:    Biblioteca libldsc CObjetoImagem
=========================================================================
Desenvolvido por:
						LDSC - Laboratorio de Desenvolvimento de Software Científico
@author     Andre Duarte Bueno - http://www.lenep.uenf.br/~bueno
@begin      2010
@copyright  (C) 2010 by Andre Duarte Bueno - http://www.lenep.uenf.br/~bueno
@email      <andreduartebueno@gmail.com>
@file 	  CObjetoImagem.h
@license    GNU General Public License - version 2
						see  $LICENSEFILE$ for the full license text.
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <set>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

// -----------------------------------------------------------------------
// Bibliotecas libldsc
// -----------------------------------------------------------------------

/**
 * Representa um objeto de uma imagem de rocha reservatório.
 * O tipo indica se é um sólido, poro, sitio, ligação ou Ramo_Morto.
	@author Andre Duarte Bueno - <andreduartebueno@gmail.com>
*/
enum ETipoObjetoImagem {
	SOLIDO = 0,
	PORO = 1,
	SITIO = 2,
	LIGACAO = 3,
	RAMO_MORTO = 4,
	NAO_IDENTIFICADO = 5
};

struct SPontoCentral {
		unsigned int x = 0;
		unsigned int y = 0;
		unsigned int z = 0;
		unsigned int df = 0; //distância ao fundo.
};

using namespace std;

class CObjetoImagem
{
	protected:
		//----------------------------------------------------Atributos
		/// Identifica o tipo do objeto
		ETipoObjetoImagem  tipo = NAO_IDENTIFICADO;

		/// Acumula a quantidade de objetos representados pela classe.
		int numObjs = 0;

		// Set das conecções
		//std::set<int> sConexao;

		/// Matriz que armazena conexões e propriedade de cada conexão
		std::map<int, double> sConexao;

	public:
		/// Identifica o voxel central do objeto
		SPontoCentral pontoCentral;

	public:
		// ---------------------------------------------------Construtor-Destrutor
		/// Construtor
		CObjetoImagem(): sConexao()	{
		}

		/// Construtor sobrecarregado. Recebe tipo do objeto, rótulo e opcionalmente o número de objetos representados
		CObjetoImagem( ETipoObjetoImagem _t, int _n=0) : tipo( _t ), numObjs( _n ), sConexao() {
		}

		/// Destrutor
		~CObjetoImagem() {}

		/// O método Conectar conecta o objeto atual a um outro objeto, recebe o rotulo do outro objeto e opcionalmente uma propriedade (condutância).
		bool Conectar( int _rotulo, double _propriedade=0.0 ) {
			//return (sConexao.insert( _rotulo )).second; // insert retorna um pair<iterator,bool> onde first é o iterator e second o bool
			return (sConexao.insert( make_pair(_rotulo, _propriedade) )).second; // insert retorna um pair<iterator,bool> onde first é o iterator e second o bool
		}

		/// Grava as informações do objeto no arquivo recebido como parâmetro (formato Rede de Percolação de Sítios e Ligações).
		void GravarObjeto(ofstream &_fout);

		// --------------------------------------------------------------------Get
		/// Retorna tipo de objeto
		inline ETipoObjetoImagem Tipo() { return tipo; }

		/// Calcula e retorna o raio de acordo com a distância ao fundo do centro de massa do objeto
		inline int Raio() { // RA = 1, se (3); RA = 2, se (4  <= df <=  6); RA = 3, se (7  <= df <=  9); RA = 4, se (10 <= df <= 12); ...
			int dft = pontoCentral.df;
			while ( (dft % 3) != 0 )
				++dft;
			return (dft/3);
		}

		/// Retorna o número de obejtos representados
		inline int NumObjs() { return numObjs; }

		/// Retorna tipo de objeto como uma string
		inline std::string StrTipo();

		// Retorna tipo de objeto como um int
		// inline int Tipo() { return (int) tipo; };

		// /// Retorna o rotulo
		//inline int Rotulo() { return rotulo; }

		/// Retorna referencia para o Set das conexões
		inline std::map<int, double> & SConexao() 	{
		//inline std::set< int > & SConexao() 	{
			return sConexao;
		}

		/// Retorna o número de conexões do objeto.
		inline int NumConexoes() {
			return sConexao.size();
		}

		// --------------------------------------------------------------------Set
		// /// Seta o rotulo
		// inline void Rotulo ( int _rotulo) { rotulo = _rotulo; }

		/// Seta o tipo de objeto
		inline void Tipo ( ETipoObjetoImagem _tipo ) { tipo = _tipo; }

		/// Seta o número de objetos
		inline void NumObjs(int _n) { numObjs = _n; }

		/// Seta o ponto central se o valor de df informado for maior que o atual.
		inline void PontoCentral ( const int &_x, const int &_y, const int &_z, const unsigned int &_df ) {
			if ( _df >= pontoCentral.df ) {
				pontoCentral.x = _x;
				pontoCentral.y = _y;
				pontoCentral.z = _z;
				pontoCentral.df = _df;
			}
		}

		/// Operador que incrementa o número de objetos representados
		inline CObjetoImagem &operator++() { ++numObjs; return *this;}

		/// Operador que decrementa o número de objetos representados
		inline CObjetoImagem &operator--() { if(numObjs>0) --numObjs; return *this; }
};

/*
set
pair<iterator,bool> insert(const value_type& x) ;
*/

/* Colocar em classe derivada:
	/// Área do objeto
	int area;

	/// Perímetro do objeto
	int perimetro;

	/// Retorna a Area do objeto
	int Area() 			{ return area; }

	/// Retorna o Perimetro
	int Perimetro() 		{ return perimetro; }

	/// Retorna Raio Hidraulico
	int RaioHidraulico() 		{ return area / perimetro; }

*/

#endif
