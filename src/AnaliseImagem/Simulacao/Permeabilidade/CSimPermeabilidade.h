#ifndef CSimPermeabilidade_h
#define CSimPermeabilidade_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CSimPermeabilidade...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CSimPermeabilidade.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <iostream>
#include <string>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CSimulacao_h
#include <Base/CSimulacao.h>
#endif

/**
 * @brief Representa o conceito de um simulador que calcula a permeabilidade de meios porosos (rochas reservatório).
 * CSimPermeabilidade é herdeira de CSimulacao, e terá diversas classes filhas que simulam a permeabilidade
 * de diferentes maneiras.
 *
 * A primeira herdeira de CSimPermeabilidade foi a CSimPermeabilidadeGrafo que determina a permeabilidade intrinseca 
 * a partir do grafo de uma imagem tridimensional. 
 * @see CSimPermeabilidadeGrafo para maiores detalhes.
 * @author 	André Duarte Bueno
*/
class CSimPermeabilidade : public CSimulacao
{
// --------------------------------------------------------------Atributos
protected:
    ///< Permeabilidade do meio poroso (rocha reservatório)
    long double permeabilidade {0.0};

public:
// -------------------------------------------------------------Construtor
    /// Construtor Default
    CSimPermeabilidade ()  = default;

    /// Construtor com argumentos
    CSimPermeabilidade (long double _p/*, long double _erro*/) {
        permeabilidade = _p; // 		erro = _erro;
    }

    /// Construtor de cópia
    CSimPermeabilidade (const CSimPermeabilidade & aCSimPermeabilidade) {   
		permeabilidade = aCSimPermeabilidade.permeabilidade;
		erro = aCSimPermeabilidade.erro;
	}

// --------------------------------------------------------------Destrutor
    /// Destrutor
    virtual ~ CSimPermeabilidade ()  = default;

// ----------------------------------------------------------------Métodos
public:
    /// Abre e lê arquivo de disco com dados necessários
    virtual void Read (std::string nomeArquivo);

//     /// Função de cálculo da permeabilidade, será redefinida nas classes herdeiras.
//     virtual long double Go ()    {
//         return permeabilidade;
//     }

// -------------------------------------------------------------Sobrecarga
    /// Operador atribuição
    CSimPermeabilidade & operator= (const CSimPermeabilidade & aCSimPermeabilidade) = default;

    /// Comparação igualdade
    bool operator== (const CSimPermeabilidade & aCSimPermeabilidade) const;

    /// Comparação diferença
    bool operator!= (const CSimPermeabilidade & aCSimPermeabilidade) const;

// --------------------------------------------------------------------Get
    /// Retorna a permeabilidade
    long double Permeabilidade () const     {
        return permeabilidade;
    }

// --------------------------------------------------------------------Set
    /// Seta a permeabilidade
    void Permeabilidade (long double _p)    {
        permeabilidade = _p;
    }

// -----------------------------------------------------------------Friend
    /// Sobrecarga operador<<
    friend std::ostream & operator<< (std::ostream & os,
                                      const CSimPermeabilidade & obj);

    /// Sobrecarga operador>>
    friend std::istream & operator>> (std::istream & is, CSimPermeabilidade & obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream & operator<< (std::ostream & os, CSimPermeabilidade & obj);

std::istream & operator>> (std::istream & is, CSimPermeabilidade & obj);

#endif
