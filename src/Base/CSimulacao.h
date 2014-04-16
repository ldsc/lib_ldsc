#ifndef CSimulacao_h
#define CSimulacao_h

/**
	===============================================================================
	PROJETO:    Biblioteca LIB_LDSC
	Assunto/Ramo: CSimulacao
	===============================================================================
	Desenvolvido por:
				Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
	@author     Andre Duarte Bueno
	@file       CSimulacao.h
	@begin      Sat Sep 16 2000
	@copyright  (C) 2000 by Andre Duarte Bueno
	@email      andreduartebueno@gmail.com
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//Bibliotecas LIB_LDSC
//-----------------------------------------------------------------------.
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

/**
 * @brief Classe genérica (e pura) para simulação de sistemas.
 * Usualmente o usuário vai chamar:
 * SolucaoSistema(); para resolver o sistema como um todo,
 * depois pode chamar Go() para calcular propriedades/conceitos específicos (que dependem do sistema resolvido).
 *
 * O método SolucaoSistema(); chama os seguintes métodos:
 * - CriarObjetosAgregados (); 		para criar objetos agregados,
 * - DefinirCondicoesContorno(); 	para definir as condições de contorno,
 * - DefinirCondicoesIniciais(); 	para definir os valores iniciais, e
 * - SolucaoSistemaEquacoes(); 		para resolver o sistema de equacoes.
 *
 * A funcao Go() é usada para calcular a propriedade que se deseja;
 * vai ser redefinida nas classes filhas para realizar o cálculo da propriedade desejada.
 *
 * Observe a diferença, SolucaoSistema() resolve o sistema como um todo,
 * Go() calcula alguma propriedade de interesse.
 *
 * Note que aqui são criadas diversas funções virtuais puras, que serão redefinidas (implementadas nas classes filhas).
 * @author 	Andre Duarte Bueno
*/
class CSimulacao {
   //--------------------------------------------------------------Atributos
protected:
   /** true se o sistema já esta resolvido, false se não resolvido. */
   bool sistemaResolvido { false };

    ///< Valor do erro no cálculo da propriedade de interesse;
    long double erro {0.0};
	
public:
   //-------------------------------------------------------------Construtor
   /** Construtor */
   CSimulacao () = default;

   //--------------------------------------------------------------Destrutor
   /** Destrutor */
   virtual ~ CSimulacao () = default;

   //----------------------------------------------------------------Métodos
public:
   /** Resolve o sistema como um todo, executando as diversas etapas associadas a uma simulação padrão
	de engenharia. Note que após resolver o sistema seta o atributo sistemaResolvido. */
   virtual bool SolucaoSistema () {
      CriarObjetosAgregados ();
      DefinirCondicoesContorno ();
      DefinirCondicoesIniciais ();
      SolucaoSistemaEquacoes ();
      sistemaResolvido = true;
      return sistemaResolvido ;
   }

   /** Resolve um problema especifico, como exemplo o cálculo de uma propriedade fisica;
   * Ou seja, depois de chamar SolucaoSistema() posso chamar Go() para calcular itens específicos.
   * Go() vai ser redefinida nas classes filhas para realizar o cálculo da propriedade desejada.
   * Nota: Se o sistema ainda não foi resolvido, então Go() irá chamar SolucaoSistema();
   */
   virtual long double Go () = 0;

protected:
   /** Cria objetos agregados */
   virtual void CriarObjetosAgregados () = 0;

   /** Define as condições de contorno */
   virtual void DefinirCondicoesContorno () = 0;

   /** Define valores iniciais */
   virtual void DefinirCondicoesIniciais () = 0;

   /** Resolve o sistema de equações */
   virtual void SolucaoSistemaEquacoes () = 0;

	//--------------------------------------------------------------------Get
public:
	/** Retorna flag que informa se o sistema esta resolvido ou não*/
   bool SistemaResolvido () { return sistemaResolvido ; }

   /** Retorna o erro na determinação da propriedade de interesse */
	 long double Erro() { return erro; }

   //--------------------------------------------------------------------Set
   /**Define se o sistema esta resolvido ou não*/
   void SistemaResolvido ( bool sr ) { sistemaResolvido = sr;   }

   //-----------------------------------------------------------------Friend
   //      friend ostream& operator<< (ostream& os, CSimulacao& obj);
   //      friend istream& operator>> (istream& is, CSimulacao& obj);
};

//-----------------------------------------------------------------Friend
//Declaracao de Funções Friend
//ostream& operator<< (ostream& os, CSimulacao& obj);
//istream& operator>> (istream& is, CSimulacao& obj);

#endif

