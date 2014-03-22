#ifndef CHistograma_h
#define CHistograma_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CHistograma...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CHistograma.h
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
#ifndef CVetor_h
#include <MetNum/Matriz/CVetor.h>
#endif

#ifndef TCMatriz2D_H
#include <MetNum/Matriz/TCMatriz2D.h>
#endif

/**
 * @brief   Representa o histograma de níveis de cinza de uma imagem.
 *  O usuário cria o histograma, chamando CHistograma(numeroCores);
 * o número de cores é usado para alocar o vetor histograma.
 * Depois o usuário acessa a função Go(pm)
 * passa a matriz para a função Go que executa o cálculo do histograma
 * em sí.
 * 
 * Pode-se obter o valor médio do histograma acessando a função
 * Media() definida na classe CVetor .
 * Pode-se criar um histograma a partir de outro.
 * Pode-se carregar o histograma de um arquivo de disco.
 * Pode-se construir um histograma vazio, tendo necessariamente
 * de ser alocado com Aloca().
 * Pode-se ainda definir e obter o número de cores.
 * @author 	André Duarte Bueno	
 * @see		Matrizes
*/
class CHistograma : public CVetor 
{
// --------------------------------------------------------------Atributos

protected:
	int numeroCores; ///< numero de cores, default = 256

public:
// -------------------------------------------------------------Construtor
	/// Construtor
    CHistograma (std::string nomeArquivo):CVetor  (nomeArquivo), numeroCores (256)
  {
  }

  /// Construtor
CHistograma (CHistograma & hist):
  // CVetor (dynamic_cast<CVetor *>(hist)),numeroCores(hist->NumeroCores())
  CVetor  (*dynamic_cast < CVetor  * >(&hist)), numeroCores (hist.NumeroCores())	
  {
  }

  /// Construtor
CHistograma (CVetor  & v): CVetor  (v), numeroCores (256)
  {
  }

  /// Construtor
CHistograma (int nx = 256): CVetor  (nx), numeroCores (256)
  {
  }

// --------------------------------------------------------------Destrutor
  /// Destrutor
  virtual ~ CHistograma ()
  {				
  }

// ----------------------------------------------------------------Métodos
  /// Calcula o histograma
	CHistograma *Go (TCMatriz2D< int > * matriz);

  /// Calcula o nível de corte para a percentagem passada
  int NivelCortePara (double percentagem);

  /// Calcula a percentagem para o nível de corte passado
  double PercentagemPara (int NivelCorte);

  /// Seta o numero de cores
  void NumeroCores (int _nc)
  {
    numeroCores = _nc;
  }

  /// obtêm o numero de cores
  int NumeroCores () const
  {
    return numeroCores;
  }

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CHistograma& obj);
//       friend istream& operator>> (istream& is, CHistograma& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CHistograma& obj);
// istream& operator>> (istream& is, CHistograma& obj);
#endif
