#ifndef CObjetoRedePercolacao_Sitio_EST_h
#define CObjetoRedePercolacao_Sitio_EST_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
						Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
						Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     Leandro Puerari
@file       CObjetoRedePercolacao_Sitio_EST.h

@copyright  (C) 2000 by Leandro Puerari
@email      puerari@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <iostream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#include <EstruturaDados/ObjetoRedePercolacao/CObjetoRedePercolacao_Sitio.h>

// ===============================================================================
// Documentacao Classe: CObjetoRedePercolacao_Sitio_EST
// ===============================================================================
/**
 * @brief Representa um sítio(CObjetoRedePercolacao_Sitio) conectado a face direita do grafo.
 * Assume valor de contorno = CContorno::EST.
 * O valor de contorno é usado no cálculo dos fluxo da malha como um todo.
 * Ou seja, um algoritimo externo percorre toda a malha,
 * e se o contorno for igual ao solicitado calcula alguma propriedade neste contorno.
 * @author Leandro Puerari
 * @see    grafos
 * @ingroup  HCObjetoGrafo
*/
class CObjetoRedePercolacao_Sitio_EST  final : public CObjetoRedePercolacao_Sitio { 
		// --------------------------------------------------------------Atributos
	public:

		// -------------------------------------------------------------Construtor
		/// Construtor
		CObjetoRedePercolacao_Sitio_EST () = default;

		// --------------------------------------------------------------Destrutor
		/// Destrutor
		virtual ~ CObjetoRedePercolacao_Sitio_EST ()  = default;

		// ----------------------------------------------------------------Métodos
		/// Retorna o tipo de objeto do grafo.
		virtual ETipoObjetoGrafo Tipo () const  override { return ETipoObjetoGrafo::ObjetoRede_Sitio_EST;   }

		// Redefinição de funções herdadas
		/// Retorna o tipo de contorno
		/*inline*/ virtual CContorno::ETipoContorno Contorno () const  override {
			return CContorno::ETipoContorno::EST;
		}

		/**
			* @brief Função herdade da classe CParametroSolver usada para
			* calcular o valor de x retorna a pressão na fronteira direita
		 */
		/*inline*/
		virtual long double Go ( long double d = 0 ) override;

		// --------------------------------------------------------------------Get
		// --------------------------------------------------------------------Set
		// -----------------------------------------------------------------Friend
		// friend ostream& operator<< (ostream& os, CObjetoRedePercolacao_Sitio_EST& obj);
		// friend istream& operator>> (istream& is, CObjetoRedePercolacao_Sitio_EST& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CObjetoRedePercolacao_Sitio_EST& obj);
// istream& operator>> (istream& is, CObjetoRedePercolacao_Sitio_EST& obj);

#endif
