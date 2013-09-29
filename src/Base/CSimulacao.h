#ifndef CSimulacao_h
#define CSimulacao_h

/*
	===============================================================================
	PROJETO:    Biblioteca LIB_LDSC
	Assunto/Ramo: CSimulacao
	===============================================================================
	Desenvolvido por:
	Laboratorio de Desenvolvimento de Software Cientifico
	[LDSC].
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
 * @brief Classe generica (e pura) para simulacao de sistemas.
 *
 * Usualmente o usuario vai chamar:
 * CriacaoObjetosAgregados (); para criar objetos agregados
 * Definir as condições de contorno,
 * Definir os valores iniciais, e
 * SolucaoSistemaEquacoes para resolver o sistema de equacoes.
 *
 * Depois pode chamar Go para calcular propriedades/conceitos específicos
 * (que dependem do sistema resolvido).
 *
 * Note que aqui são criadas diversas funções virtuais puras, que serão
 * redefinidas (implementadas nas classes filhas).
 *
 * Note que a funcao Go() é usada para calcular a propriedade que se deseja.
 * A funcao Go() vai ser redefinida nas classes filhas para realizar o calculo da
 * propriedade desejada.
 *
 * Observe a diferença, SolucaoSistema() resolve o sistema como um todo,
 * Go() calcula alguma propriedade de interesse.

	 @author 	Andre Duarte Bueno
	 @version
	 @see			simulacao
*/
class CSimulacao
{
		//--------------------------------------------------------------Atributos
	protected:

		/**
	 * true se o sistema já esta resolvido, falso se não resolvido.
	*/
		bool sistemaResolvido;

	public:
		//-------------------------------------------------------------/**Construtor*/
		/**
	 * Construtor
	 * @return
	 */
		CSimulacao () : sistemaResolvido (false)
		{
		}

		//--------------------------------------------------------------/**Destrutor*/
		/**
	 * Destrutor
	 * @return
	 */
		virtual ~ CSimulacao ()
		{
		}

		//----------------------------------------------------------------Métodos
	protected:

		/** Cria objetos agregados */
		virtual void CriacaoObjetosAgregados () = 0;

		/** Define as condições de contorno */
		virtual void DefinicaoCondicoesContorno () = 0;

		/** Define valores iniciais */
		virtual void DefinicaoValoresIniciais () = 0;

		/** Resolve o sistema de equações */
		virtual void SolucaoSistemaEquacoes () = 0;

	public:

		/** Resolve o sistema como um todo através da chamada das funções a seguir */
		virtual bool SolucaoSistema () {
			CriacaoObjetosAgregados ();
			DefinicaoCondicoesContorno ();
			DefinicaoValoresIniciais ();
			SolucaoSistemaEquacoes ();
			sistemaResolvido = true;
			return 1 ;
		}

		/** Resolve o problema especifico definido em Go*/
		virtual long double Go () = 0;

		//--------------------------------------------------------------------Get
		/**Define se o sistema esta resolvido ou não*/
		bool SistemaResolvido ()
		{
			return sistemaResolvido ;
		}

		//--------------------------------------------------------------------Set
		/**Define se o sistema esta resolvido ou não*/
		void SistemaResolvido (bool sr)
		{
			sistemaResolvido = sr;
		}

		//-----------------------------------------------------------------Friend
		//      friend ostream& operator<< (ostream& os, CSimulacao& obj);
		//      friend istream& operator>> (istream& is, CSimulacao& obj);
};

//-----------------------------------------------------------------Friend
//Declaracao de Funções Friend
//ostream& operator<< (ostream& os, CSimulacao& obj);
//istream& operator>> (istream& is, CSimulacao& obj);

#endif 

