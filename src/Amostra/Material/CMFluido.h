#ifndef CMFluido_h
#define CMFluido_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CMFluido...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CMFluido.h
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
 * @brief	Representa um objeto fluido.
 * 
 * A principio um fluido contém as propriedades viscosidade, densidade
 * e compressibilidade.
 * 
 * Posteriormente elaborar melhor a hierarquia de fluídos, para considerar
 * outras propriedades usuais.
 * Dê uma olhada na hierarquia materiais.
 *
 *
 * Obs:
 * O construtor recebe estes atributos como parâmetros, se os mesmos não
 * forem passados   para o construtor são assumidos valores default.
 * Os atributos do objeto fluido podem ser alterados posteriormente
 * acessando-se as 	funções SetAtributo.
 * As propriedades default do fluido também podem estar armazenadas em
 * um arquivo de disco   chamado fluido.dat.
 * 
 * Obs:
 * O atributo molhabilidade é um atributo de ligação, surge da interação
 * de dois fluídos.
 * A principio vai ser definido pelo usuário, posteriormente deve
 * ser calculado em 	função das propriedades  dos fluidos.
 * Por este motivo a função Molhabilidade recebe um fluido B.

@author 	André Duarte Bueno	
@version						
@see		Veja a hierarquia TMateriais
*/
class CMFluido
{
// --------------------------------------------------------------Atributos
protected:

  /// Viscodidade do fluído
	double viscosidade = 1.0; //0.001002;

  /// Densidade do fluido
	double densidade = 1.0;

  /// Compressibilidade é uma propriedade do fluido
	bool compressibilidade = true;

  // Atributos ligação, só existem a partir de um relacionamento
  // Surge da interação de dois fluidos
  // definida pela função Setmolhabilidade, ou
  /// Molhabilidade
	bool molhabilidade = true;

public:
// -------------------------------------------------------------Construtor

	/// Construtor default
	CMFluido ( ) {
	}

	/// Construtor com parametros
	CMFluido (double _v, double _d, bool _c, bool _m) {
    viscosidade = _v;
    densidade = _d;
    compressibilidade = _c;
    // Atributo de ligacao
    molhabilidade = _m;
  }

  /// Construtor de cópia
  CMFluido (const CMFluido & aCMFluido);

// --------------------------------------------------------------Destrutor
  /// Destrutor
  virtual ~ CMFluido ()
  {
  }

// ----------------------------------------------------------------Métodos
public:
  // Calcula a molhabilidade do fluido com base no relacionamento
  // de this com fluidoB
  // Função ficticia


  /*
  void Molhabilidade (CMFluido * fluidoB) //  ELIMINAR/Trocar.
                                          //  Esta errada
  {                                       //  deve retornar a molhabilidade
                                          //  calcula fora e aqui coloca se é molhante ou não
    molhabilidade = (fluidoB->densidade > this->densidade) ? 1 : 0;
  }
  */

  // Abre e lê o arquivo de disco
  // virtual void Read(string nomeArquivo="fluido.dat");
  // virtual void Read(string nomeArquivo("fluido.dat"));     

// -------------------------------------------------------------Sobrecarga
  /// Operador igualdade (atribuição)
  CMFluido & operator= (const CMFluido & aCMFluido);

  /// Operador comparação igualdade
  bool operator== (const CMFluido & aCMFluido) const;

// --------------------------------------------------------------------Get
  
  /// Obtêm a viscosidade
  double Viscosidade () const
  {
    return viscosidade;
  }

  /// Obtêm a compressibilidade
  bool Compressibilidade () const
  {
    return compressibilidade;
  }

    /// Obtêm a densidade
  double Densidade () const
  {
    return densidade;
  }

    /// Obtêm a molhabilidade
  bool Molhabilidade () const
  {
    return molhabilidade;
  }

// --------------------------------------------------------------------Set
    /// Seta a viscosidade
  void Viscosidade (double av)
  {
    viscosidade = av;
	}
  
      /// Seta a compressibilidade
  void Compressibilidade (bool ac)
  {
    compressibilidade = ac;
  }

      /// Seta a densidade
  void Densidade (bool d)
  {
    densidade = d;
  }

      /// Seta a molhabilidade
  void Molhabilidade (bool d)
  {
    molhabilidade = d;
  }

// -----------------------------------------------------------------Friend
  // Sobrecarga streans
  /// Sobrecarga operador <<
  friend std::ostream & operator<< (std::ostream & os, const CMFluido & obj);
  
  /// Sobrecarga operador>>
  friend std::istream & operator>> (std::istream & is, CMFluido & obj);
};

#endif
