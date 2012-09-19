#ifndef CRotulador2DCm_h
#define CRotulador2DCm_h

/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: CRotulador2D...
  ===============================================================================
  Desenvolvido por:	
  Laboratorio de Desenvolvimento de Software Cientifico 	
  [LDSC].
  @author     André Duarte Bueno
  @file       CRotulador2DCm.h
  @begin      Sat Sep 16 2000
  @copyright  (C) 2000 by André Duarte Bueno
  @email      andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <vector>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CRotulador2D_h
#include <Rotulador/CRotulador2D.h>
#endif

/**
 * @brief   Representa um objeto rotulador com método para cálculo do centro de massa.
 * Descendente de CRotulador2D que descende de CRotulador e de uma matriz 2D.
 * Rotula imagens bidimensionais.
 * @author 	André Duarte Bueno	
 * @see		Rotulador
*/
class CRotulador2DCm : public CRotulador2D
{
  // --------------------------------------------------------------Atributos
 public:

  // TVetor* cmx; ///< Vetor com as posições x
  // TVetor* cmy; ///< Vetor com as posições y
  // Usa-se objeto[i] para obter ponteiro i.
  std::vector < double > cmx;///< Vetor com as posições x
  std::vector < double > cmy;///< Vetor com as posições y


  // -------------------------------------------------------------Construtor
  // Aloca matriz de dados
  // Copia ponteiro para imagem recebida,
  // na função prepara imagem copia os dados
  /// Construtor recebe ponteiro para imagem
  CRotulador2DCm (TMatriz2D< int > * _pm) : CRotulador2D ( _pm )
  {
  }

  /// Construtor recebe nome da imagem
  CRotulador2DCm (std::string fileName) : CRotulador2D (fileName)
  {
  }

    /// Construtor, recebe nx e ny
  CRotulador2DCm (int nx, int ny) : CRotulador2D (nx, ny, 1, 0)
    // CRotulador2DCm( int nx,int ny) : CRotulador2D(nx,ny),cmx(NULL),cmy(NULL)
    {
    }

    /// Construtor default
    CRotulador2DCm ():CRotulador2D ()
    // CRotulador2DCm():CRotulador2D(),cmx(NULL),cmy(NULL)
    {
    }

  // --------------------------------------------------------------Destrutor
   /// Destrutor
    virtual ~ CRotulador2DCm () {
      // if(cmx)       delete [] cmx;
      // if(cmy)       delete [] cmy;
    }

  // ----------------------------------------------------------------Métodos
 public:
  // Métodos novos
	 /// Calcula os centros de massa nas direções x e y
  virtual void CentroMassaObjetos ();	

  // --------------------------------------------------------------------Get
  /// Retorna posição x do objeto k
  double CMXObjetos (int k) const;
  
  /// Retorna posição y do objeto k
  double CMYObjetos (int k) const;

  // --------------------------------------------------------------------Set
  // -----------------------------------------------------------------Friend
  // friend ostream& operator<< (ostream& os, CRotulador2D& obj);
  // friend istream& operator>> (istream& is, CRotulador2D& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CRotulador2D& obj);
// istream& operator>> (istream& is, CRotulador2D& obj);
#endif
