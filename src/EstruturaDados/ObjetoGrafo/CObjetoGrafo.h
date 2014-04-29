#ifndef CObjetoGrafo_h
#define CObjetoGrafo_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
						Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
						Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CObjetoGrafo.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// Interface de disco
#include <fstream>
#include <vector>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
// Definição export/import da classe
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CContorno_h
#include <MetNum/Contorno/CContorno.h>
#endif

/** Grupo de classes da hierarguia de objetos de grafo.
 * @defgroup HCObjetoGrafo HCObjetoGrafo = Hierarquia para diferentes objetos do grafo
 */

/// Enumeração para os diferentes tipos de grafo.
/// @enum: ETipoGrafo
// Note que sempre que criar classe herdeira tem de colocar aqui a enumeração correspondente.
#ifndef ETipoObjetoGrafo_
#define ETipoObjetoGrafo_

enum class ETipoObjetoGrafo : uint8_t {
	ObjetoGrafo = 0,                  	// Objetos da hierarquia de objetos do grafo
	ObjetoGrafo_1VetorConexoes = 1,
	ObjetoGrafo_2VetoresConexoes = 2,
	ObjetoGrafo_MatrizConexoes = 3,

	ObjetoRede = 10,                   	// Objetos da hierarquia de objetos da rede
	ObjetoRede_Final = 11, 				// Inclui o tipo como atributo (final)
	ObjetoRede_Tipo = 15, 				// Inclui o tipo como atributo (sem herança)
	ObjetoRede_Ligacao = 20,
	ObjetoRede_Ligacao_CENTER = 21,
	ObjetoRede_Ligacao_EST = 22,
	ObjetoRede_Ligacao_WEST = 23,
	ObjetoRede_Sitio = 30,
	ObjetoRede_Sitio_CENTER = 31,
	ObjetoRede_Sitio_EST = 32,
	ObjetoRede_Sitio_WEST = 33,

	ObjetoEsqueleto =  50,              	// Objetos da hierarquia de objetos do esqueleto
	ObjetoEsqueleto_Final = 51, 			// Inclui o tipo como atributo (final)
	ObjetoEsqueleto_Tipo = 55, 			// Inclui o tipo como atributo (sem herança)
	ObjetoEsqueleto_Ligacao = 60,
	ObjetoEsqueleto_Ligacao_CENTER =61,
	ObjetoEsqueleto_Ligacao_EST = 62,
	ObjetoEsqueleto_Ligacao_WEST = 63,
	ObjetoEsqueleto_Sitio = 70,
	ObjetoEsqueleto_Sitio_CENTER = 71,
	ObjetoEsqueleto_Sitio_EST = 72,
	ObjetoEsqueleto_Sitio_WEST = 73,
};
#endif

// ===============================================================================
// Documentacao Classe: CObjetoGrafo
// ===============================================================================
/**
 * @brief Representa um objeto básico de um grafo.
 * Um CGrafo representa um grafo, que é composto por um conjunto de objetos do tipo CObjetoGrafo.
 * CObjetoGrafo tem um conjunto de métodos para:
 * Conectar objetos:             Conectar().
 * Deletar objetos conectados:   DeletarConexao().
 * Deletar conexões inválidas:   DeletarConexoesInvalidadas().
 * Retornar o tipo de contorno:  Contorno().
 * Salvar o objeto em disco:     Write().
 * Ler o objeto do disco:        Read().
 * Determinar o fluxo de uma determinada propriedade: Fluxo().
 * Determinar alguma propriedade de interesse: Go().
 *
 * Nota: as conexões entre objetos serão definidas nas classes herdeiras.
 *
 * @author   André Duarte Bueno.
 * @todo:    implementar Read().
 * @ingroup  HCObjetoGrafo
*/
class CObjetoGrafo {
		// --------------------------------------------------------------Atributos
	public:
		/**
	 * @brief Todo objeto CObjetoGrafo tem um rotulo, que o identifica; O rótulo é usado
	 * para armazenar o objeto em disco, e para localizar o objeto, seus links e referências.
	 */
		unsigned int rotulo {0};

		// -------------------------------------------------------------Construtor
		/// Construtor
		CObjetoGrafo ()  = default;

		// --------------------------------------------------------------Destrutor
		/// Destrutor.
		virtual ~ CObjetoGrafo ()   = default;

		// ----------------------------------------------------------------Métodos
		/// Como um objeto do grafo pode estar sujeito a algum tipo de condição
		/// de contorno, o mesmo tem um método que informa o contorno.
		/// Nas classes herdeiras o tipo Contorno poderá ser, por exemplo,
		/// CContorno::ETipoContorno::WEST
		/// Enumeração para o tipo de contorno, por default assume CContorno::CENTER.
		/// Esboço:
		///                         3
		///            5         0  1  2            6
		///                         4
		///                         NORTH
		///            FRONT WEST  CENTER  EST    BACK
		///                         SUL
		/// @bug : como os valores da Enumeração foram alterados verificar se não implica em bug!
		///        ou seja, no código não usar conversão para unsigned char.
		///  enum class ETipoContorno : unsigned char
		///  { CENTER = 1, WEST=0, EST=2, SOUTH=4, NORTH=3, FRONT=5, BACK=6 };
		/// @return o número (identificação) do contorno ao qual esta associado
		virtual CContorno::ETipoContorno Contorno () const  {
			return CContorno::ETipoContorno::CENTER;
		}

		/// Retorna a enumeração com o tipo de objeto do grafo. Sobrescrita nas herdeiras.
		virtual ETipoObjetoGrafo Tipo () const  {
			return ETipoObjetoGrafo::ObjetoGrafo;
		}

		/**
	 * @brief Função que conecta este objeto e um objeto externo.
	 * Note que recebe um ponteiro para um CObjetoGrafo, e o inclue na lista de conexões.
	 * Deve ser sobrescrita nas herdeiras pois aqui não temos o vetor de objetos.
	 * Na hierarquia de objetos do tipo CObjetoRede, receberá um CObjetoRede*.
	 */
		inline virtual void Conectar ( CObjetoGrafo* objA, CObjetoGrafo* objB = nullptr ) {
			objA = objA;
			objB = objB;
		} //=0;

		/**
			* @brief Função que conecta este objeto e um vetor de objetos externos.
		 * Note que recebe um um vetor de CObjetoGrafo*, e o inclue na lista de conexões.
		*/
		inline virtual void Conectar ( std::vector < CObjetoGrafo* >& obj_vetor ) {
			obj_vetor = obj_vetor;
		}
		/// Deleta uma conexão dada pela posição pos.
		/// Se a pos for inválida lança exceção.
		/// Note que o controle dos objetos criados/deletados é feito por CGrafo/CRefe/CEsqueleto
		/// Aqui controlamos apenas as conexões entre eles. Nunca deletamos os objetos!
		inline virtual void DeletarConexao ( unsigned int pos )  = 0; //{};

		/// Deleta as conexões repetidas, retorna o número de conexões deletadas.
		virtual unsigned int DeletarConexoesRepetidas_e_SomarCondutanciasParalelo() {
			return 0;
		}

		/**
	 * @brief Percorre o vetor de conexões e deleta as ligações para objetos que foram marcados para deleção.
	 * Recebe um número que identifica os objetos que foram marcados para deleção,
	 * se o rótulo dos objetos conectados é igual a este parâmetro a conexão é eliminada.
	 * Chama função auxiliar.
	 */
		inline virtual bool DeletarConexoesInvalidadas ( unsigned int deletado )  = 0 ;

		/// @brief Salva atributos do objeto em disco.
		virtual std::ostream& Write ( std::ostream& os ) const = 0 ;

		/// @brief Lê atributos do objeto em disco.
		/// Note que não esta fazendo nada Implementar nas herdeiras!!!
		virtual std::istream& Read ( std::istream& in ) {
			return in;
		}

	protected:
		/// Função auxiliar que recebe o indice das conexões a serem deletadas e um vetor de conexões.
		/// criada para reduzir códigos nas herdeiras.
		bool DeletarConexoesInvalidadas_aux ( unsigned deletado, std::vector<CObjetoGrafo*>& conexao );

		// --------------------------------------------------------------------Get
	public:
		/// @brief Retorna o rótulo do objeto.
		unsigned int Rotulo () const  {
			return rotulo;
		}

		// --------------------------------------------------------------------Set
		/// @brief Seta o rótulo do objeto. Note que não deveria ser setado diretamente!
		void Rotulo ( unsigned int _r ) {
			rotulo = _r;
		}

		// -----------------------------------------------------------------Friend
		/// Sobrecarga operador extração, use para salva dados objeto em disco.
		friend std::ostream& operator<< ( std::ostream& os, const CObjetoGrafo& obj );
		/// Sobrecarga operador inserção, use para ler dados do objeto em disco.
		friend std::istream& operator>> ( std::istream& is, CObjetoGrafo& obj );
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
inline std::ostream& operator<< ( std::ostream& os, const CObjetoGrafo& obj );
inline std::istream& operator>> ( std::istream& is, CObjetoGrafo& obj );

#endif

