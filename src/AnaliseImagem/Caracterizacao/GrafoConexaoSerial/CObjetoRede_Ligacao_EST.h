#ifndef CObjetoRede_Ligacao_EST_h
#define CObjetoRede_Ligacao_EST_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoRede_Ligacao_EST.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
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

#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Ligacao.h>

// ===============================================================================
// Documentacao Classe: CObjetoRede_Ligacao_EST
// ===============================================================================
/**
 * @brief Representa um sítio(CObjetoRede_Sitio) conectado a face direita do grafo.
 * Assume valor de contorno = CContorno::EST.
 * O valor de contorno é usado no calculo dos fluxo da malha como um todo.
 * Ou seja, um algoritimo externo percorre toda a malha,
 * e se o contorno for igual ao solicitado calcula alguma propriedade neste contorno.
 * @author André Duarte Bueno
 * @see    grafos
 * @ingroup  HCObjetoGrafo
*/
class CObjetoRede_Ligacao_EST : public CObjetoRede_Ligacao { 
// --------------------------------------------------------------Atributos
public: 

// -------------------------------------------------------------Construtor
/// Construtor
     CObjetoRede_Ligacao_EST () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CObjetoRede_Ligacao_EST ()  = default;

// ----------------------------------------------------------------Métodos
   /// Retorna o tipo de objeto do grafo.
   virtual ETipo Tipo () const  override { return ETipo::ObjetoEsqueleto_Ligacao_EST;   }

	// Redefinição de funções herdadas
	/// Retorna o tipo de contorno
	inline virtual CContorno::ETipoContorno Contorno () const  override {
		return CContorno::ETipoContorno::EST;
	}

     /**
      * @brief Função herdade da classe CParametroSolver usada para
      * calcular o valor de x retorna a pressão na fronteira direita
     */
     inline virtual long double Go ( long double d = 0 )  override {
          return x;
     }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
     // friend ostream& operator<< (ostream& os, CObjetoRede_Ligacao_EST& obj);
     // friend istream& operator>> (istream& is, CObjetoRede_Ligacao_EST& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CObjetoRede_Ligacao_EST& obj);
// istream& operator>> (istream& is, CObjetoRede_Ligacao_EST& obj);

#endif
