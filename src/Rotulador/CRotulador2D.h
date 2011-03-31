#ifndef CRotulador2D_h
#define CRotulador2D_h

/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
  Assunto/Ramo: CRotulador2D...
  ===============================================================================
  Desenvolvido por:	
  Laboratorio de Desenvolvimento de Software Cientifico 	
  [LDSC].
  @author     André Duarte Bueno
  @file       CRotulador2D.h
  @begin      Sat Sep 16 2000
  @copyright  (C) 2000 by André Duarte Bueno
  @email      andre@lmpt.ufsc.br
*/

//-----------------------------------------------------------------------
//Bibliotecas C/C++
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//Bibliotecas LIB_LDSC
//-----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif

#ifndef CMatriz2D_h
#include <Matriz/CMatriz2D.h>
#endif

#ifndef CRotulador_h
#include <Rotulador/CRotulador.h>
#endif

/**
 * @brief   Representa um objeto rotulador.
 * 
 * Descendente do CRotulador e de uma matriz 2D.
 * Rotula imagens bidimensionais.
 * @author 	André Duarte Bueno	
 * @see		Rotulador
*/

class CRotulador2D : public CRotulador, public CMatriz2D
{
  //--------------------------------------------------------------Atributos
 public:
  /** @brief Ponteiro para a imagem recebida no construtor ou em Go*/
  CMatriz2D * pm;

  //-------------------------------------------------------------Construtor
  /** Construtor. 
   * Aloca matriz de dados
   * Copia ponteiro para imagem recebida,
   * na funcão prepara imagem copia os dados
  */
  CRotulador2D (CMatriz2D * _pm):CRotulador(), CMatriz2D(_pm->NX(), _pm->NY()), pm(_pm)
  {
  	CMatriz2D::Path( _pm->Path() );
  }

  /// Construtor, abre imagem do disco
  CRotulador2D (std::string fileName):CRotulador(), CMatriz2D(fileName), pm(NULL)
  {
  }

  /// Construtor, recebe dimensões da imagem
  CRotulador2D (int nx, int ny):CRotulador(), CMatriz2D(nx, ny), pm(NULL)
  {
  }

  /** data2D deve ser alocado posteriormente*/
  CRotulador2D ():CRotulador(), CMatriz2D(), pm(NULL)
  {
  }

  //--------------------------------------------------------------Destrutor
  
  /// Destrutor
  virtual ~ CRotulador2D ()
  {
  }

  //----------------------------------------------------------------Métodos
  protected:
  /**  @brief Verifica a imagem*/
  virtual bool PreparaImagem (CMatriz2D * matriz);

  /**  @brief Passo 1: 1a Passagem.
   * OBS: o int rotuloInicial=0 foi passado para a funcao RotulaImagem	*/
  virtual void IdentificaObjetos ();

  /**  @brief Passo 4: 2a passagem.*/
  virtual void RotulaImagem ( /* int _rotuloInicial=0 */ );

  //Métodos antigos, podem ser descartados
  //virtual       void FuncaoRotulaImagemAntiga();                        //herdadas virtuais
  //virtual       void FuncaoRotulaImagemSequencialAntiga();
  //unsigned int Rotulo (i,j)        {return data2D[i][j]    + rotuloInicial;};
 public:
  //Herdadas virtuais
	 /// Calcula area dos objetos
  virtual void CalculaAreaObjetos ();
	 /// Calcula perimetro dos objetos
  virtual void CalculaPerimetroObjetos ();

  //Metodos utilizados na rotulagem, redefinidos aqui
  //virtual void Go(CMatriz2D*& matriz, int rotuloInicial = 0 );
  /// Executa toda a sequencia de rotulagem
  virtual bool Go (CMatriz2D * matriz /*, int rotuloInicial = 0 */ );

  //--------------------------------------------------------------------Get
  //--------------------------------------------------------------------Set
  //-----------------------------------------------------------------Friend
  //friend ostream& operator<< (ostream& os, CRotulador2D& obj);
  //friend istream& operator>> (istream& is, CRotulador2D& obj);
};

//-----------------------------------------------------------------Friend
//Declaração de Funções Friend
//ostream& operator<< (ostream& os, CRotulador2D& obj);
//istream& operator>> (istream& is, CRotulador2D& obj);

#endif
