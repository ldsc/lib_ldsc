#ifndef CGra3Dby2D_M1_h
#define CGra3Dby2D_M1_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CGra3Dby2D_M1...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CGra3Dby2D_M1.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CGra3Dby2D_h
#include <Grafo/CGra3Dby2D.h>
#endif

/**
 * @brief Determina o grafo de uma imagem 3D usando o modelo 1 (+simples),
 * para cada pixel uma ligação.
 *
 * Função Go()
 * 
 * Neste modelo a função Go funciona da seguinte forma:
 * Os planos i e i+1 são rotulados, gerando-se a seguir os sítios.
 * Depois estabelece-se as conecções entre os planos consecutivos.
 * Observe que duas regiões que se interceptam vão gerar um número
 * grande de conecções (com conecções redundantes).
 * Ou seja, existe uma repetição no número de ligações entre duas regiões,
 * pois para cada conecção entre píxeis,
 * estabelece uma conecção.
 * 
 * @author 	André Duarte Bueno	
 * @see	      	Grafo
*/
class CGra3Dby2D_M1:public CGra3Dby2D
{
// --------------------------------------------------------------Atributos
public:

// -------------------------------------------------------------Construtor
	/// Construtor
  CGra3Dby2D_M1 (std::string _fileName):CGra3Dby2D (_fileName)
  {
  }

// --------------------------------------------------------------Destrutor
  /// Destrutor
  virtual ~ CGra3Dby2D_M1 ()
  {
  }

// ----------------------------------------------------------------Métodos
/** @brief Calcula as condutancias.
 * Função nova, movida de CPermeabilidadeGrafo para cá.
 * */
  virtual void CalculoCondutancias (long double _viscosidade,
				    long double _sizePixel,
				    unsigned long int fatorAmplificacao);

protected:
  /// Conecta planos,  Versão com Links duplicados.
  virtual void DeterminarConeccoesObjetos (unsigned long int
					   maiorRotuloUtilizado);

  // Função Go
  // virtual CGrafo* Go( CMatriz3D * _img3D,unsigned int _tamanhoMascara = 1);

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CGra3Dby2D_M1& obj);
//       friend istream& operator>> (istream& is, CGra3Dby2D_M1& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGra3Dby2D_M1& obj);
// istream& operator>> (istream& is, CGra3Dby2D_M1& obj);

#endif
