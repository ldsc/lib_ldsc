#ifndef CObjetoRedePercolacao_Sitio_WEST_h
#define CObjetoRedePercolacao_Sitio_WEST_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
						Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
						Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     Leandro Puerari
@file       CObjetoRedePercolacao_Sitio_WEST.h

@copyright  (C) 2000 by Leandro Puerari
@email      puerari@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#include <iostream>
#include <EstruturaDados/ObjetoRedePercolacao/CObjetoRedePercolacao_Sitio.h>

// ===============================================================================
// Documentacao Classe: CObjetoRedePercolacao_Sitio_WEST
// ===============================================================================
/**
 * @brief Representa um sítio conectado a face esquerda do grafo.
 * Assunto:        CGrafo
 * Superclasse:    CParametroSolver->CObjetoGrafo->CObjetoRedePercolacao_Sitio->CObjetoRedePercolacao_Sitio_WEST
 *
 * O valor de contorno é usado no cálculo dos fluxo da malha como  um todo.
 * Ou seja, um algoritimo externo percorre toda a malha, e se o contorno for igual ao
 * solicitado calcula alguma propriedade neste contorno.
 * @author  Leandro Puerari
 * @see     CGrafo
 * @ingroup  HCObjetoGrafo
*/
class CObjetoRedePercolacao_Sitio_WEST  final : public CObjetoRedePercolacao_Sitio {
		// --------------------------------------------------------------Atributos
	public:
		// -------------------------------------------------------------Construtor
		/// Construtor.
		CObjetoRedePercolacao_Sitio_WEST () = default;

		// --------------------------------------------------------------Destrutor
		/// Destrutor.
		virtual ~ CObjetoRedePercolacao_Sitio_WEST () =  default;

		// ----------------------------------------------------------------Métodos
		/// Retorna o tipo de objeto do grafo.
		virtual ETipoObjetoGrafo Tipo () const  override { return ETipoObjetoGrafo::ObjetoRede_Sitio_WEST;  }

		/// Retorna o tipo de contorno a que pertence
		/*inline*/ virtual CContorno::ETipoContorno Contorno () const override  {
			return CContorno::ETipoContorno::WEST;
		}

		/**
		 * @brief Função herdada da classe CParametroSolver,
		 * usada para calcular o valor de x retorna a pressão na fronteira direita
		 */
		/*inline*/
		virtual long double Go ( long double d = 0 ) override;

		// --------------------------------------------------------------------Get
		// --------------------------------------------------------------------Set
		// -----------------------------------------------------------------Friend
		//       friend ostream& operator<< (ostream& os, CObjetoRedePercolacao_Sitio_WEST& obj);
		//       friend istream& operator>> (istream& is, CObjetoRedePercolacao_Sitio_WEST& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CObjetoRedePercolacao_Sitio_WEST& obj);
// istream& operator>> (istream& is, CObjetoRedePercolacao_Sitio_WEST& obj);
#endif
