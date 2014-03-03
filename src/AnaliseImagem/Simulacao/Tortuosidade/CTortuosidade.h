#ifndef CTortuosidade_h
#define CTortuosidade_h

/**
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author     André Duarte Bueno
@file       CGra_3Dby2D_M6.h
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
// ===============================================================================
// Documentacao Classe: CTortuosidade
// ===============================================================================
/**
 * @brief Representa a tortuosidade de um meio poroso.
 * @author	André Duarte Bueno
 * @see		
*/
class CTortuosidade
{
// --------------------------------------------------------------Atributos
protected:
  /// Representa a tortuosidade na direção z
  long double tortuosidade { 0.0 };
//   double tortuosidade_x; Para cálculo nas 3 direções vai ter de
//   double tortuosidade_y; calcular o grafo em cada uma das direções
//   double tortuosidade_z;

// -------------------------------------------------------------Construtor
public:
  /// Construtor detault.
  CTortuosidade() = default;
  
  /// Construtor sobrecarregado, recebe a tortuosidade.
  CTortuosidade( long double _tortuosidade )  { tortuosidade = _tortuosidade; } ;
  
// --------------------------------------------------------------Destrutor
  /// Destrutor detault.
  ~CTortuosidade() = default;

// ----------------------------------------------------------------Métodos
// --------------------------------------------------------------------Get
public:
  long double Tortuosidade() { return tortuosidade; }
// --------------------------------------------------------------------Set
// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CGra_3Dby2D_M6& obj);
//       friend istream& operator>> (istream& is, CGra_3Dby2D_M6& obj);
  
};
#endif

