#ifndef CGra3Dby2D_M6_h
#define CGra3Dby2D_M6_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CGra3Dby2D_M6...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico 
            [LDSC].
@author     André Duarte Bueno
@file       CGra3Dby2D_M6.h
@begin      Oct 20 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif /*  */

#ifndef CGra3Dby2D_M3_h
#include <Grafo/CGra3Dby2D_M3.h>
#endif /* cerr */

// ===============================================================================
// Documentacao CLASSE: CGra3Dby2D_M6
// ===============================================================================
/**
 * @brief Determina o grafo, modelo 6.
 * Superclasse: CGrafo -> CGra3Dby2D -> CGra3Dby2_M2 -> CGra3Dby2_M3 -> CGra3Dby2D_M6
 * @author	André Duarte Bueno
 * @see		Grafos
*/
class CGra3Dby2D_M6 : public CGra3Dby2D_M3
{

// --------------------------------------------------------------Atributos
public:

// -------------------------------------------------------------Construtor
	/// Construtor
	CGra3Dby2D_M6 (std::string _fileName):CGra3Dby2D_M3 (_fileName)
  {
  }

// --------------------------------------------------------------Destrutor
	/// Destrutor
  virtual ~ CGra3Dby2D_M6 ()
  {
  }

// ----------------------------------------------------------------Métodos
  /**
   * @brief Redefinida, na classe  CGra3Dby2_M3 calcula as condutâncias 
   * dos sítios sem considerar o deslocamento dos centros de massa
   * aqui considera as posições dos centros de massa
  */
  virtual void CalculoCondutancias (long double _viscosidade,
				    long double _sizePixel,
				    unsigned long int _fatorAmplificacao);

protected:
  /// Redefinida, para  o tipoContorno=centro retorna um COGSitioLRCM
  virtual CObjetoGrafo * GetObjetoGrafo (CContorno::ETipoContorno tipoContorno);

  /**
   * @brief Função que cria os objetos e os adiciona a lista de objetos.
   * Recebe como parâmetros um objeto rotulador (com os objetos a serem adicionados)
   * o valor do maior rotulo utilizado o tipo de objeto a ser criado
  */
  virtual void AdicionarObjetos (CRotulador2DCm * rotulador,
				 unsigned long int rotuloAtual,
				 CContorno::ETipoContorno tipo);

  // Função Go
  // virtual CGrafo* Go( TCMatriz3D<int> * _img3D,unsigned long int  _tamanhoMascara = 1);
  // virtual CGrafo* Go( std::string fileName,unsigned long int _tamanhoMascara = 0) ;

// --------------------------------------------------------------------Get
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CGra3Dby2D_M6& obj);
//       friend istream& operator>> (istream& is, CGra3Dby2D_M6& obj);
};


// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CGra3Dby2D_M6& obj);
// istream& operator>> (istream& is, CGra3Dby2D_M6& obj);
#endif
