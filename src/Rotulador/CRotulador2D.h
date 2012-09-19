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

#ifndef TMatriz2D_h
#include <Matriz/TMatriz2D.h>
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

class CRotulador2D : public CRotulador, public TMatriz2D< int >
{
  //--------------------------------------------------------------Atributos
 public:
  /** @brief Ponteiro para a imagem recebida no construtor ou em Go*/
  TMatriz2D< int > * pm;

  //-------------------------------------------------------------Construtor
  /** Construtor. 
   * Aloca matriz de dados
   * Copia ponteiro para imagem recebida,
   * na funcão prepara imagem copia os dados
  */
  CRotulador2D (TMatriz2D< int > * _pm, int _indice=1, int _fundo=0) :
     CRotulador( _indice, _fundo ), TMatriz2D< int >(_pm->NX(), _pm->NY()), pm(_pm) {
  	TMatriz2D< int >::Path( _pm->Path() );
  }

  /// Construtor, abre imagem do disco
  CRotulador2D (std::string fileName, int _indice=1, int _fundo=0) :
     CRotulador( _indice, _fundo ), TMatriz2D< int >(fileName), pm(NULL) {
  }

  /// Construtor, recebe dimensões da imagem
  CRotulador2D (int nx, int ny, int _indice=1, int _fundo=0) :
     CRotulador( _indice, _fundo ), TMatriz2D< int >(nx, ny), pm(NULL) {
  }

  /** data2D deve ser alocado posteriormente*/
  CRotulador2D (int _indice=1, int _fundo=0) :
     CRotulador( _indice, _fundo ), TMatriz2D< int >(), pm(NULL) {
  }

  //--------------------------------------------------------------Destrutor
  
  /// Destrutor
  virtual ~ CRotulador2D ()
  {
  }

  //----------------------------------------------------------------Métodos
  protected:
  /**  @brief Verifica a imagem*/
  virtual bool PreparaImagem (TMatriz2D< int > * matriz);

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
  //virtual void Go(TMatriz2D< int >*& matriz, int rotuloInicial = 0 );
  /// Executa toda a sequencia de rotulagem
  virtual bool Go (TMatriz2D< int > * matriz /*, int rotuloInicial = 0 */ );

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
