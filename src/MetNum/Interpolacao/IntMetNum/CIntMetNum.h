#ifndef CIntMetNum_h
#define CIntMetNum_h

#include <MetNum/Interpolacao/CInterpolacao.h>

/**
 * @brief Subdivisão da hierarquia CInterpolação.
 * 
 * As classes abaixo são aquelas usadas no programa S3DMP, e são baseadas
 * nas funções de interpolação descritas em Maliska.
 * 
 * A Classe CIntMetNum deve calcular os coeficientes alfa e beta.
 * Os coeficientes alfa e beta são usados para avaliar o valor da propriedade
 * e sua derivada (veja C.R.Maliska).
 * 
 * PS: observe que se trata de um sistema especializado, específico
 * para os métodos de volumes de controle, descritos em C.R.Maliska.
 * 
 * Observe que na funcao de calculo de alfa e beta os parametros
 * sao passados por referencia, isto evita a cópia da variável o que
 * deixa a funcao mais rápida.
*/
class CIntMetNum : public CInterpolacao
{
  // Atributos
protected:

static double peclet;	///< numero de peclet 
static double pe2;	///< numero de peclet ao quadrado, peclet^2

  static double c3, c4;		///< Variaveis auxiliares

  double alfa;			///< Coeficiente alfa

  double beta;			///< Coeficiente beta

public:
  static int tipo_de_interpolacao;	///< Tipo de interpolacao selecionado

  // Metodos
public:

	/// Construtor
    CIntMetNum ();		

    /// Destrutor
    virtual ~ CIntMetNum ();	

    
    /// Calcula alfa e beta
  virtual void AlfaBeta (double &u, double &dxc, double &ro, double &dif) = 0;
  
  /// Calculo de alfa 
  inline virtual double Alfa ()
  {
    return alfa;
  }	

    /// Calculo de beta
  inline virtual double Beta ()
  {
    return beta;
  }	

  /// Entrada dos atributos
  virtual void Entrada ();	
  
  /// Saida dos atributos
  virtual void Saida ();
  	
  // sobrecarga operacoes << e >>
  // friend istream& operator >> (istream&,Interpolacao*&);
  // friend ostream& operator << (ostream&,Interpolacao*&);
};
#endif
