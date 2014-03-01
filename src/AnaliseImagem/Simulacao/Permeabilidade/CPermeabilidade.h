#ifndef CPermeabilidade_h
#define CPermeabilidade_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CPermeabilidade...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
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

#ifndef TSimulacao_h
#include <Base/CSimulacao.h>
#endif

/**
 * @brief	Objeto para calculo e armazenagem da permeabilidade.
 *
 * Conta com hierarquia onde cada filho vai ter uma metodologia para
 * determinação da permeabilidade.
 *
 * A primeira herdeira desenvolvida foi a CPermeabilidadeGrafo
 * que determina a permeabilidade intrinseca a partir
 * do grafo de uma imagem tridimensional.
 *
 * Veja CPermeabilidadeGrafo para maiores detalhes.
 * @author 	André Duarte Bueno
 * @see		Propriedades Físicas
*/
class CPermeabilidade : public CSimulacao
{
// --------------------------------------------------------------Atributos
public:
    ///< Valor da permeabilidade
    long double permeabilidade;
    ///< Valor de erro
    long double erro;
public:
// -------------------------------------------------------------Construtor
    /// Construtor Default
    CPermeabilidade () {
        erro = 0.0;
        permeabilidade = 0.0;
    }

    /// Construtor com argumentos
    CPermeabilidade (long double _p) {
        permeabilidade = _p;
    }

    /// Construtor de cópia
    CPermeabilidade (const CPermeabilidade & aCPermeabilidade);

// --------------------------------------------------------------Destrutor
    /// Destrutor
    virtual ~ CPermeabilidade ()
    {
    }

// ----------------------------------------------------------------Métodos

protected:

    // A função abaixo foi definida em TSimulação e chama as demais
    // virtual void SolucaoSistema();

    // Definição das funções da classe base (herdadas)
    /// Cria objetos agregados
    virtual void CriarObjetosAgregados ()
    {
    }

    /// Define as condições de contorno
    virtual void DefinirCondicoesContorno ()
    {
    }

    /// Define valores iniciais
    virtual void DefinirValoresIniciais ()
    {
    }

    /// Resolve o sistema de equações
    virtual void SolucaoSistemaEquacoes ()
    {
    }


public:
    /// Abre e lê o arquivo de disco
    virtual void Read (std::string nomeArquivo);

    /// Função de calculo da permeabilidade
    virtual long double Go ()
    {
        return permeabilidade;
    }

// -------------------------------------------------------------Sobrecarga
    /// Operador igualdade (atribuição)
    CPermeabilidade & operator= (const CPermeabilidade & aCPermeabilidade);

    /// Comparação igualdade
    bool operator== (const CPermeabilidade & aCPermeabilidade) const;

    /// Comparação diferença
    bool operator!= (const CPermeabilidade & aCPermeabilidade) const;

// --------------------------------------------------------------------Get
    /// Retorna a permeabilidade
    long double Getpermeabilidade () const
    {
        return permeabilidade;
    }

// --------------------------------------------------------------------Set
    /// Seta a permeabilidade
    void Setpermeabilidade (long double _p)
    {
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
