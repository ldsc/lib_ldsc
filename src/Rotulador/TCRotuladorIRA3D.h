#ifndef TCRotuladorIRA3D_h
#define TCRotuladorIRA3D_h

/*
	===============================================================================
	PROJETO:    Biblioteca LIB_LDSC
	Assunto/Ramo: TCRotuladorIRA3D
	===============================================================================
	Desenvolvido por:
	Laboratorio de Desenvolvimento de Software Cientifico
	[LDSC].
	@author     Leandro Puerari
	@file       TCRotuladorIRA3D.h
	@begin      Sat Sep 16 2000
	@email      puerari@gmail.com
*/

#include <Rotulador/TCRotulador3D.h>

/**
 * @brief	Representa um objeto rotulador 3D para IRA (Imagem Raio Abertura). Herdeira da TCRotulador3D.
 * @author 	Leandro Puerari, <puerari@gmail.com>
 * @see CRotulador, TCFEMMIRA
*/
template<typename T>
class TCRotuladorIRA3D : public TCRotulador3D<T>
{
		// --------------------------------------------------------------Atributos
	public:
		/// Raio para abertura. Utilizado para rotular Imagem de Raio para Abertura (IRA). Se ra for negativo, rotula o complemento da abertura.
		int ra;

		// -------------------------------------------------------------Construtor
		// Estamos tratando de uma IRA, logo, FUNDO será 0 e consequentemente INDICE será 1;
		/// Recebe ponteiro para imagem 3D usado para setar nx,ny,nz
		TCRotuladorIRA3D (TCMatriz3D<int> * _pm) : TCRotulador3D<int> ( _pm, 1, 0), ra(0)  {
		}

		/// Recebe o nome da matriz de disco, abre arquivo de disco e seta matriz
		TCRotuladorIRA3D (std::string fileName) : TCRotulador3D<int> ( fileName, 1, 0), ra(0) {
		}

		/// Recebe a informação das dimensoes da imagem
		TCRotuladorIRA3D (unsigned int nx, unsigned int ny, unsigned int nz) : TCRotulador3D<int> ( nx, ny, nz, 1, 0), ra(0) {
		}

		/// data3D deve ser alocado posteriormente
		TCRotuladorIRA3D ( ) : TCRotulador3D<int> ( 1, 0), ra(0) {
		}

		// --------------------------------------------------------------Destrutor
		/// Destrutor
		virtual ~ TCRotuladorIRA3D () {
		}

		// ----------------------------------------------------------------Metodos
		/// Método para rotular da IRA. Precisa informar o raio para abertura.
		bool Go (TCMatriz3D<int> * matriz, int _ra );

	protected:
		/// Verifica se a imagem recebida tem as mesmas dimensoes do rotulador. Se diferente, realoca o rotulador
		virtual bool PreparaImagem (TCMatriz3D<int> * matriz);
};

#include <Rotulador/TCRotuladorIRA3D.cpp>

#endif
