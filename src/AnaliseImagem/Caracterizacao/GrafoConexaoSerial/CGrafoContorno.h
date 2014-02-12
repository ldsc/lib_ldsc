#ifndef CGrafoContorno_h
#define CGrafoContorno_h

/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: CGrafoContorno...
  ===============================================================================
  Desenvolvido por:	
  Laboratorio de Desenvolvimento de Software Cientifico 	
  [LDSC].
  @author     André Duarte Bueno
  @file       CGrafoContorno.h
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
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CGrafo_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafo.h>
#endif

#ifndef CContorno_h
#include <MetNum/Contorno/CContorno.h>
#endif

// ===============================================================================
// Documentacao CLASSE: CGrafoContorno
// ===============================================================================
/**
 * @short: Representa um grafo sujeito a condições de contornos; 
 * Herdeira da classe CGrafo, a classe CGrafoContorno acrescenta a idéia de que
 * um grafo pode estar submetido a determinadas condições de contorno.
 * Na prática acrescenta um vetor que armazena os objetos de contorno.
 * Note que existe uma hierarquia para objetos de contorno, este
 * vetor terá ponteiros para objetos da classe base da hierarquia de contornos.
 * @Assunto:        CGrafo
 * @Superclasse:    CGrafoContorno
 * @author 	André Duarte Bueno	
 * @see		Grafo
*/
class CGrafoContorno : public CGrafo
{
  // --------------------------------------------------------------Atributos
 public:

  /// Usa-se contorno[i] para obter ponteiro para contorno i.
  std::vector < CContorno * >contorno;

  // -------------------------------------------------------------Construtor
  /// Cria os objetos de contorno e incluí no vetor contorno.
  CGrafoContorno (std::string _fileName, unsigned long int _numero_contornos = 0);

  // --------------------------------------------------------------Destrutor
  /// Delete os objetos de contorno do vetor contorno.
  virtual ~ CGrafoContorno ();

  // ----------------------------------------------------------------Métodos
  // virtual void Write(std::string fileName);

 public:

  // --------------------------------------------------------------------Get
  // --------------------------------------------------------------------Set
  // -----------------------------------------------------------------Friend
  friend std::ostream & operator<< (std::ostream & os,
				    const CGrafoContorno & obj);
  //       friend istream& operator>> (istream& is, CGrafoContorno& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
std::ostream & operator<< (std::ostream & os, const CGrafoContorno & obj);
// istream& operator>> (istream& is, CGrafoContorno& obj);

#endif
