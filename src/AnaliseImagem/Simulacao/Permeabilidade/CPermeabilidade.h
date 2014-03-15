#ifndef CPermeabilidade_h
#define CPermeabilidade_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CPermeabilidade...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CPermeabilidade.h
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
 * @brief Objeto para cálculo e armazenagem da permeabilidade.
 *
 * Esta é a classe base de uma hierarquia onde cada filho vai ter uma metodologia para determinação da permeabilidade.
 *
 * A primeira herdeira desenvolvida foi a CPermeabilidadeGrafo que determina a permeabilidade intrinseca a partir
 * do grafo de uma imagem tridimensional.  Veja CPermeabilidadeGrafo para maiores detalhes.
 * @author 	André Duarte Bueno
*/
class CPermeabilidade : public CSimulacao
{
// --------------------------------------------------------------Atributos
protected:
    ///< Valor da permeabilidade
    long double permeabilidade {0.0};
	/// @todo: verificar do ponto de vista da modelagem computacional, a vantagem de cada classe que envolva o
	/// cálculo de uma propriedade física ter, uma variável com o valor do erro.
    ///< Valor de erro
//     long double erro {0.0};

public:
// -------------------------------------------------------------Construtor
    /// Construtor Default
    CPermeabilidade ()  = default;

    /// Construtor com argumentos
    CPermeabilidade (long double _p/*, long double _erro*/) {
        permeabilidade = _p;
// 		erro = _erro;
    }

    /// Construtor de cópia
    CPermeabilidade (const CPermeabilidade & aCPermeabilidade) {   
		permeabilidade = aCPermeabilidade.permeabilidade;
///		erro = aCPermeabilidade.erro;
	}

// --------------------------------------------------------------Destrutor
    /// Destrutor
    virtual ~ CPermeabilidade ()  = default;

// ----------------------------------------------------------------Métodos

protected:

    // A função SolucaoSistema(); foi definida em CSimulação e chama as demais
    // virtual void SolucaoSistema();

//     // Definição das funções da classe base (herdadas)
//     /// Cria objetos agregados
//     virtual void CriarObjetosAgregados ()    {}
// 
//     /// Define as condições de contorno
//     virtual void DefinirCondicoesContorno ()     {    }
// 
//     /// Define valores iniciais
//     virtual void DefinirCondicoesIniciais ()    {    }
// 
//     /// Resolve o sistema de equações
//     virtual void SolucaoSistemaEquacoes ()    {    }

public:
    /// Abre e lê arquivo de disco com dados necessários
    virtual void Read (std::string nomeArquivo);

    /// Função de cálculo da permeabilidade, será redefinida nas classes herdeiras.
    virtual long double Go ()    {
        return permeabilidade;
    }

// -------------------------------------------------------------Sobrecarga
    /// Operador atribuição
    CPermeabilidade & operator= (const CPermeabilidade & aCPermeabilidade) = default;

    /// Comparação igualdade
    bool operator== (const CPermeabilidade & aCPermeabilidade) const;

    /// Comparação diferença
    bool operator!= (const CPermeabilidade & aCPermeabilidade) const;

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
                                      const CPermeabilidade & obj);

    /// Sobrecarga operador>>
    friend std::istream & operator>> (std::istream & is, CPermeabilidade & obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream & operator<< (std::ostream & os, CPermeabilidade & obj);

std::istream & operator>> (std::istream & is, CPermeabilidade & obj);

#endif
