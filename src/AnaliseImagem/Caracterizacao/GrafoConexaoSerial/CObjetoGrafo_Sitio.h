#ifndef CObjetoGrafo_Sitio_h 
#define CObjetoGrafo_Sitio_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CObjetoGrafo_Sitio.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// Interface disco
#include <fstream>

// Classe container para vetores
#include <vector>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CObjetoGrafo_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo.h>
#endif

// ===============================================================================
// Documentacao Classe: CObjetoGrafo_Sitio
// ===============================================================================
/**
 * @brief  Representa um objeto de um grafo que tem uma lista de coneccoes (Sitio).
 *
 * É herdeiro de CObjetoGrafo,tendo um rótulo (herdado de CObjetoGrafo).
 * A característica básica de um sítio é que este pode ter
 * n coneccao's, ou seja pode estar conectado a n objetos do tipo
 * CObjetoGrafo, mas esta conecção é 1 para 1 (uma garra).
 *
 * Exemplo:
 * Um sítio pode estar conectado a outro sítio.
 * Um sítio pode estar conectado a uma ligação.
 *
 * @author:  André Duarte Bueno
 * @see:     grafos
 * @todo:    implementar operadores >>.
 * @ingroup  HCObjetoGrafo
*/
class CObjetoGrafo_Sitio : public CObjetoGrafo 
{
// --------------------------------------------------------------Atributos
public:
     /**
     * @brief Lista de ponteiros para objetos do tipo CObjetoGrafo.
     * Ou seja, ponteiros para objetos da hierarquia CObjetoGrafo.
     * O vetor coneccao é o vetor dos objetos a quem estou conectado.
     * @todo: verificar vantagens de trocar vector por list.
     */
     std::vector < CObjetoGrafo * >coneccao; // default size()=0

// -------------------------------------------------------------Construtor

/// Construtor
     CObjetoGrafo_Sitio () = default;

// --------------------------------------------------------------Destrutor
/// Destrutor
     virtual ~ CObjetoGrafo_Sitio () = default;

// ----------------------------------------------------------------Métodos
     /**
       * @brief Função que recebe um ponteiro para um CObjetoGrafo,
       * e o inclue na lista de conecções. Lista dos objetos a quem estou conectado.
     */
     virtual void Conectar ( CObjetoGrafo *objA, CObjetoGrafo *objB = nullptr ) override ;

     /// Deleta uma conexão.
     virtual void DeletarConeccao ( unsigned int link ) override ;

     /**
     * @brief Deleta os links para objetos que foram marcados para deleção.
     * Recebe um número que identifica os objetos que foram marcados
     * para deleção, se o rótulo dos objetos conectados é igual a este parâmetro
	 * a conecção é eliminada.
     */
     virtual bool DeletarConeccoesInvalidadas ( int deletado ) override ; 
	 
     /// @brief Salva atributos do objeto em disco.
     virtual std::ostream &Write ( std::ostream &os ) const override ;

//      /// @brief Salva atributos do objeto em disco no formato do Liang
//      virtual std::ostream &Write_Liang_Format ( std::ostream &os ) const  {
//           Write ( os ); // deve ser reescrita nas derivadas.
//           return os;
//      }
     /**
     * @brief Função usada para calcular uma propriedade.
     */
     virtual long double Go ( long double d = 0 )   override {
	   return 0;
	 }

     /**
      * @brief Função que calcula o fluxo associado as propriedade do objeto
	  * e suas conecções.
      * Ou seja, considera-se que este objeto esta conectado a outros objetos
      * e que em função das propriedades dos objetos, existe alguma informação 
	  * que transita entre os objetos. Esta propriedade é calculada por esta função.
      * Pode ser fluxo de massa, de calor, de qualquer coisa, ...
     */
     virtual long double Fluxo () const  override {
	   return 0;
	 }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
     friend std::ostream &operator<< ( std::ostream &os, CObjetoGrafo_Sitio &obj );
//       friend istream& operator>> (istream& is, CObjetoGrafo_Sitio& obj);

};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream &operator<< ( std::ostream &os, CObjetoGrafo_Sitio &obj );
// istream& operator>> (istream& is, CObjetoGrafo_Sitio& obj);


// Cria o tipo CObjetoGrafo_Sitio_CENTER, que é igual a CObjetoGrafo_Sitio
// typedef CObjetoGrafo_Sitio CObjetoGrafo_Sitio_CENTER;
using CObjetoGrafo_Sitio_CENTER = CObjetoGrafo_Sitio;

#endif
