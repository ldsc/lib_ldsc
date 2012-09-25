#ifndef CContorno_h
#define CContorno_h

/*
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
 * Posteriormente esta classe deve ser ampliada e definida uma herança.
 *
 * Quando temos um problema de engenharia para resolver,
 * de uma maneira geral, precisamos definir os limites espaciais do problema.
 * -Se o sistema for isolado, significa que não existe interferência do meio (contorno).
 * Desta forma, um sistema isolado forma um universo independente.
 *
 * -Se entretanto, o meio externo interferir de alguma forma na solução do nosso
 * problema, diz-se que o sistema não é isolado, e portanto é mais complexo.
 *
 * É de praxe pesquisar este contorno a procura de elementos que possam interferir
 * na solução de nosso problema.
 *
 * Esta hierarquia de classes procura representar este conceito, o conceito de contorno.
 *
 * @author 	André Duarte Bueno
 * @version
 * @see		contorno
 *
 * Tarefas:
 * Adicionar sobrecarga >>
 * Verificar necessidade de se incluir um tipoContorno?
*/
class CContorno
{

// --------------------------------------------------------------Atributos
public:

    /// Enumeração para o tipo de contorno
    enum ETipoContorno
    { CENTER = 0, WEST=1, EST=2, SOUTH=3, NORTH=4, FRONT=5, BACK=6 };

    /// Valor da propriedade no contorno
    long double valorContorno;

// -------------------------------------------------------------Construtor
    /// Construtor
    CContorno ()
    {
    }

// --------------------------------------------------------------Destrutor
    /// Destrutor
    virtual ~ CContorno ()
    {
    }

// ----------------------------------------------------------------Métodos
public:
    /// Calcula (atualiza) valor da propriedade no contorno
    virtual long double Go (long double x = 0, long double y = 0) {
		x=x; y=y; //evitar warning
        return valorContorno;
    }

// -------------------------------------------------------------Sobrecarga
    /// Recebe um double: double x = 5.1; obj = x;
    long double &operator= (long double &_d)
    {
        valorContorno = _d;
        return valorContorno;
    }

    /// Converte para double (cast): double x = obj;
    // usar explicit ?
    operator  long double ()
    {
        return valorContorno;
    }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
/// Sobrecarga do operador <<.
    friend std::ostream & operator<< (std::ostream & os, const CContorno & obj);
//   friend istream& operator>> (istream& is, CContorno& obj);

};

// -----------------------------------------------------------------Friend

// Declaração de Funções Friend
std::ostream & operator<< (std::ostream & os, const CContorno & obj);
// istream& operator>> (istream& is, CContorno& obj);

#endif
