#ifndef CRotuladorIRA3D_h
#define CRotuladorIRA3D_h

/*
	===============================================================================
	PROJETO:    Biblioteca LIB_LDSC
	Assunto/Ramo: CRotuladorIRA3D
	===============================================================================
	Desenvolvido por:
	Laboratorio de Desenvolvimento de Software Cientifico
	[LDSC].
	@author     Leandro Puerari
	@file       CRotuladorIRA3D.h
	@begin      Sat Sep 16 2000
	@email      puerari@gmail.com
*/

#include <AnaliseImagem/Filtro/FEspacial/FERotulagem/TCRotulador3D.h>

/**
 * @brief	Representa um objeto rotulador 3D para IRA (Imagem Raio Abertura). Herdeira da TCRotulador3D.
 * @author 	Leandro Puerari, <puerari@gmail.com>
 * @see CRotulador, TCFEMMIRA
*/
class CRotuladorIRA3D : public TCRotulador3D<int>
{
		// --------------------------------------------------------------Atributos
	private:
		/// Raio para abertura. Utilizado para rotular Imagem de Raio para Abertura (IRA). Se ra for negativo, rotula o complemento da abertura.
		int ra;

		/// Raio mínimo a ser considerado na rotulagem da imagem (defalt = 0).
		unsigned int rmin;

		/// Ponteiro para a matriz com objetos a serem ignorados na rotulagem.
		TCMatriz3D<bool> * matSub;

	public:
		// -------------------------------------------------------------Construtor
		// Estamos tratando de uma IRA, logo, FUNDO será 0 e consequentemente INDICE será 1;
		/// Recebe ponteiro para imagem 3D usado para setar nx,ny,nz
		CRotuladorIRA3D (TCMatriz3D<int> * _pm, int _indice=1, int _fundo=0) : TCRotulador3D<int> ( _pm, _indice, _fundo), ra(0), rmin(0), matSub(nullptr)  {
		}

		/// Recebe o nome da matriz de disco, abre arquivo de disco e seta matriz
		CRotuladorIRA3D (std::string fileName, int _indice=1, int _fundo=0) : TCRotulador3D<int> ( fileName, _indice, _fundo), ra(0), rmin(0), matSub(nullptr) {
		}

		/// Recebe a informação das dimensoes da imagem
		CRotuladorIRA3D (unsigned int nx, unsigned int ny, unsigned int nz) : TCRotulador3D<int> ( nx, ny, nz, 1, 0), ra(0), rmin(0) {
		}

		/// data3D deve ser alocado posteriormente
		CRotuladorIRA3D ( ) : TCRotulador3D<int> ( 1, 0), ra(0), rmin(0) {
		}

		// --------------------------------------------------------------Destrutor
		/// Destrutor
		virtual ~ CRotuladorIRA3D () {
		}

		// ----------------------------------------------------------------Metodos
		/** Rotula a IRA
		 * Precisa informar o raio para abertura e opcionalmente o raio minimo.
		 * Se ra for negativo, rotula o complemento da abertura.
		 * Se rmin for informado, considera somente objetos maiores que o valor (tem efeito somente se ra for negativa, ou seja, se estiver rotulando o complemento da abertura).
		 * Se for informado o ponteiro para a matSub os objetos encontrados na matriz não serão considerados na rotulagem.
		 */
		bool Go (TCMatriz3D<int> * matriz, int _ra, unsigned int _rmin=0, TCMatriz3D<bool> * _matSub=nullptr );

	protected:
		/// Verifica se a imagem recebida tem as mesmas dimensoes do rotulador. Se diferente, realoca o rotulador
		virtual bool PreparaImagem (TCMatriz3D<int> * matriz);
};

#endif
