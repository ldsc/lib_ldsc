#ifndef CContorno_h
#define CContorno_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
						Assunto/Ramo: CContorno...
===============================================================================
Desenvolvido por:
						Laboratorio de Desenvolvimento de Software Cientifico
						[LDSC].
@author     André Duarte Bueno
@file       CContorno.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
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

/**
 * @brief Classe utilizada para especificar condições de contorno.
 *
 * Quando da solução de sistemas que envolvam uma malha, de uma maneira
 * geral deve-se especificar as condições de contorno.
 * Este objeto procura representar este conceito.
 *
 * Da forma como esta ele funciona como um número double, podendo ser igualado
 * a um double e podendo receber um double.
 * @todo: Posteriormente esta classe deve ser ampliada e definida uma herança.
 *
 * Quando temos um problema de engenharia para resolver,
 * de uma maneira geral, precisamos definir os limites espaciais do problema.
 * -Se o sistema for isolado, significa que não existe interferência do meio (contorno).
 * Desta forma, um sistema isolado forma um universo independente.
 *
 * -Se, entretanto, o meio externo interferir de alguma forma na solução do nosso
 * problema, diz-se que o sistema não é isolado, e portanto é mais complexo.
 *
 * É de praxe pesquisar este contorno a procura de elementos que possam interferir
 * na solução do problema.
 *
 * Esta hierarquia de classes procura representar este conceito, o conceito de contorno.
 *
 * @author      André Duarte Bueno
 * @version
 * @see         contorno
 * @todo:       Adicionar sobrecarga >>
*/
class CContorno
{
		// --------------------------------------------------------------Atributos
	public:
  
  /// Enumeração para o tipo de contorno
  ///  Esboço:
  ///                         3
  ///            5         0  1  2            6
  ///                         4
  ///                         NORTH
  ///            FRONT WEST  CENTER  EST    BACK
  ///                         SUL
  /// @bug : como os valores abaixo foram alterados verificar se não implica em bug!
  ///        ou seja, no código não usar conversão para unsigned char.
  enum class ETipoContorno : uint8_t 
    { CENTER = 1, WEST=0, EST=2, SOUTH=4, NORTH=3, FRONT=5, BACK=6 };

   /// Valor da propriedade no contorno, valor inicial = default, = 0.0.
   /// @return: retorna cópia do valorContorno.
   long double valorContorno{};

// -------------------------------------------------------------Construtor
    /// Construtor.
    CContorno () = default;

// --------------------------------------------------------------Destrutor
    /// Destrutor.
    /*virtual */~ CContorno ()  = default;

// ----------------------------------------------------------------Métodos
    /// Calcula (atualiza) valor da propriedade no contorno.
    /// @todo: ? Adicionar operador paranteses?
    /*virtual*/ long double Go (long double /*x = 0*/, long double /*y = 0*/) {
        return valorContorno;
    }

// -------------------------------------------------------------Sobrecarga
    /// Recebe um double: double x = 5.1; obj_contorno = x;
    /// @param: um double.
    /// @return: valorContorno por referência.
    long double& operator= (long double &_d)    {
        valorContorno = _d;
        return valorContorno;
    }

    /// Converte para double (cast): double x = obj_contorno;
    /// @todo usar explicit ?
    /// @todo retornar referência ?
    operator  long double ()    {
        return valorContorno;
    }
   
// --------------------------------------------------------------------Get
   /// Retorna o valor de contorno.
   long double ValorContorno() const { return valorContorno ; }

// --------------------------------------------------------------------Set
   /// Seta o valor de contorno.
   void ValorContorno(const long double& _v) { valorContorno = _v; }

// -----------------------------------------------------------------Friend
/// Sobrecarga do operador <<.
    friend std::ostream & operator<< (std::ostream & os, const CContorno & obj);
    friend std::istream&  operator>> (std::istream& is,  CContorno& obj);
};

// -----------------------------------------------------------------Friend

// Declaração de Funções Friend
std::ostream & operator<< (std::ostream & os, const CContorno & obj);
std::istream&  operator>> (std::istream& is, CContorno& obj);
#endif
