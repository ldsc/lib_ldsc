#ifndef TCFEMMIRA_H
#define TCFEMMIRA_H

/*
----------------------------------------------------------------------------
PROJETO:		LIBLDSC
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
Programadores:    Leandro Puerari
Copyright @2013:  Todos os direitos reservados.
Nome deste arquivo:	TCFEMMIRA.h
Nome da classe:      TCFEMMIRA
*/

// ----------------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// ----------------------------------------------------------------------------
#include <Matriz/TCMatriz2D.h>
#include <Filtro/FEspacial/FEMorfologiaMatematica/TCFEMorfologiaMatematica.h>
#include <Mascara/CMascara.h>

/**
 * @brief A classe TCFEMMIRA determina a matriz Imagem Raio para Abertura (IRA)
 * A IRA é uma matriz onde o valor de cada pixel corresponde ao menor raio de bola que seria necessário para que
 * o pixel fosse eliminado em uma operação de abertura (Morfologia Matemática)
 */
template<typename T>
class TCFEMMIRA : public TCFEMorfologiaMatematica<T> {
		// Atributos
	protected:
		/// Ponteiro para a matriz Raio Abertura.
		TCMatriz2D<int> * pmra;

	public:
		/// Construtor
		TCFEMMIRA ( TCMatriz2D<T> * &matriz, int _indice=1, int _fundo=0 );

		/// Destrutor
		virtual ~TCFEMMIRA ( ){
		}

		/// O método Go determina a imagem IRA
		TCMatriz2D<int> *Go ( );

	private:
		/// Retorma o menor valor diferente de zero. Só retorna zero se os dois valores passados forem zero.
		inline int MinNotZero(const int& a, const int& b) {
			return ( ( (a < b) && (a != 0) ) || (b == 0) ) ? a : b;
		}

		/// Retorma o menor valor maior que zero. Só retorna zero se os dois valores passados forem zero. Não está considerando tratar dois valores negativos.
		inline int MinGtZero(const int& a, const int& b) {
			return ( ( (a < b) && (a > 0) ) || (b <= 0) ) ? a : b;
		}

		/// Retorma o menor valor diferente de um. Só retorna um se os dois valores passados forem um.
		inline int MinNotOne(const int& a, const int& b) {
			return ( ( (a < b) && (a != 1) ) || (b == 1) ) ? a : b;
		}
};

#include "Filtro/FEspacial/FEMorfologiaMatematica/TCFEMMIRA.cpp"

#endif
