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
@email      <bueno@lenep.uenf.br>
@file 	  CObjetoImagem.h
@license    GNU General Public License - version 2
						see  $LICENSEFILE$ for the full license text.
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <MetNum/Contorno/CContorno.h>

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
		unsigned int x;
		unsigned int y;
		unsigned int z;
		unsigned int df; //distância ao fundo.
};

using namespace std;

class CObjetoImagem
{
	protected:
		//----------------------------------------------------Atributos
		/// Identifica o tipo do objeto
		ETipoObjetoImagem  tipo;

		/// Identifica em qual posição o objeto se encontra (Leste, Oeste, Centro...).
		CContorno::ETipoContorno contorno;

		/// Acumula a quantidade de objetos representados pela classe.
		int numObjs;

		/// Set das conecções
		std::set< int > sConexao;
	public:
		/// Identifica o voxel central do objeto
		SPontoCentral pontoCentral;

	public:
		// ---------------------------------------------------Construtor-Destrutor
		/// Construtor
		CObjetoImagem(): tipo( NAO_IDENTIFICADO ), contorno( CContorno::ETipoContorno::CENTER ), numObjs(0), sConexao()	{
			pontoCentral.x = 0;
			pontoCentral.y = 0;
			pontoCentral.z = 0;
			pontoCentral.df = 0;
		}

		/// Construtor sobrecarregado. Recebe tipo do objeto, rótulo e opcionalmente o número de objetos representados
		CObjetoImagem( ETipoObjetoImagem _t, int _n=0) : tipo( _t ), contorno( CContorno::ETipoContorno::CENTER ), numObjs( _n ), sConexao() {
			pontoCentral.x = 0;
			pontoCentral.y = 0;
			pontoCentral.z = 0;
			pontoCentral.df = 0;
		}

		/// Destrutor
		~CObjetoImagem() {}

		/// O método Conectar conecta o objeto atual a um outro objeto, recebe o rotulo do outro objeto
		bool Conectar( int _rotulo ) {
			return (sConexao.insert( _rotulo )).second; // insert retorna um pair<iterator,bool> onde first é o iterator e second o bool
		}

		/// Grava as informações do objeto no arquivo recebido como parâmetro (formato Rede de Percolação de Sítios e Ligações).
		void GravarObjeto(ofstream &_fout);

		/// Grava as informações do objeto no arquivo recebido como parâmetro (formato Grafo de Conexão Serial).
		void GravarObjetoGrafo(ofstream &_fout, const int & seq);

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

		/// Retorna referencia para o Set das conexoes
		inline std::set< int > & SConexao() 	{
			return sConexao;
		}

		/// Retorna o número de conexões do objeto.
		inline int NumConexoes() {
			return sConexao.size();
		}

		/// Retorna a camada na qual o objeto se encontra
		inline CContorno::ETipoContorno Contorno() {
			return contorno;
		}

		// Retorna rotulo da conexao i
		//int  SConexao( int i ) { return sConexao[i]; }

		// Retorna tipo como uma string
		// 	std::string Tipo() ;

		// --------------------------------------------------------------------Set
		// /// Seta o rotulo
		// inline void Rotulo ( int _rotulo) { rotulo = _rotulo; }

		/// Seta o tipo de objeto
		inline void Tipo ( ETipoObjetoImagem _tipo ) { tipo = _tipo; }

		/// Seta o número de objetos
		inline void NumObjs(int _n) { numObjs = _n; }

		/// Seta a camada na qual o objeto se encontra
		inline void Contorno(CContorno::ETipoContorno _c) { contorno = _c; }

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
