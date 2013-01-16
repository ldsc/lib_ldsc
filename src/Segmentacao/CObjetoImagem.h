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
#include <string>

// -----------------------------------------------------------------------
// Bibliotecas libldsc
// -----------------------------------------------------------------------

/**
 * Representa um objeto de uma imagem de rocha reservatório.
 * O tipo indica se é um sólido, poro, sitio, ligação ou Ramo_Morto.
	@author Andre Duarte Bueno - <andreduartebueno@gmail.com>
*/
enum ETipoObjetoImagem { SOLIDO = 0, PORO = 1, SITIO = 2, LIGACAO = 3, RAMO_MORTO = 4, NAO_IDENTIFICADO = 5 };

class CObjetoImagem
{
	protected:
		//----------------------------------------------------Atributos
		/// Identifica o tipo do objeto
		ETipoObjetoImagem  tipo;

		/// Rotulo que identifica o objeto
		int rotulo;

		/// Set das conecções
		std::set< int > sConeccao;

	public:
		// ---------------------------------------------------Construtor-Destrutor
		/// Construtor
		CObjetoImagem(): tipo( NAO_IDENTIFICADO ), rotulo(0), sConeccao()
		{}

		/// Construtor sobrecarregado
		CObjetoImagem( ETipoObjetoImagem _t , int _r) 	// std::vector< int > _v = 0 )
			: tipo( _t ), rotulo( _r ),  sConeccao()
		{}

		/// Destrutor
		~CObjetoImagem() {}

		/// O método Conectar conecta o objeto atual a um outro objeto, recebe o rotulo do outro objeto
		bool Conectar( int _rotulo ) {
			return (sConeccao.insert( _rotulo )).second; // insert retorna um pair<iterator,bool> onde first é o iterator e second o bool
		}

		// --------------------------------------------------------------------Get
		/// Retorna tipo de objeto
		ETipoObjetoImagem Tipo() 	{
			return tipo;
		}

		// Retorna tipo de objeto como uma string
		//	std::string Tipo() ;

		// Retorna tipo de objeto como um int
		// int Tipo() 			{ return (int) tipo; };

		/// Retorna o rotulo
		int Rotulo() 					{
			return rotulo;
		}

		/// Retorna referencia para o Set das conexoes
		std::set< int > & SConeccao() 	{
			return sConeccao;
		}

		// Retorna rotulo da conexao i
		//int  SConeccao( int i ) { return sConeccao[i]; }

		// Retorna tipo como uma string
		// 	std::string Tipo() ;

		// --------------------------------------------------------------------Set
		/// Seta o rotulo
		void Rotulo ( int _rotulo) 		{
			rotulo = _rotulo;
		}

		/// Seta o tipo de objeto
		void Tipo ( ETipoObjetoImagem _tipo )
		{
			tipo = _tipo;
		}

		/// Seta o tipo de objeto a partir de uma string
		void Tipo ( std::string _tipo );
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
