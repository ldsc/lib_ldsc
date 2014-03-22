#ifndef TCRotulador2D_h
#define TCRotulador2D_h

/*
  ===============================================================================
  PROJETO:    Biblioteca LIB_LDSC
	Assunto/Ramo: TCRotulador2D...
  ===============================================================================
  Desenvolvido por:	
  Laboratorio de Desenvolvimento de Software Cientifico 	
  [LDSC].
  @author     André Duarte Bueno
	@file       TCRotulador2D.h
  @begin      Sat Sep 16 2000
  @copyright  (C) 2000 by André Duarte Bueno
  @email      andreduartebueno@gmail.com
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

#ifndef TCMatriz2D_H
#include <MetNum/Matriz/TCMatriz2D.h>
#endif

#ifndef CRotulador_h
#include <AnaliseImagem/Filtro/FEspacial/FERotulagem/CRotulador.h>
#endif

/**
 * @brief   Representa uma imagem 2D rotulada.  Herdeira de CRotulador e de uma TCMatriz2D.
 * Rotula imagens bidimensionais.
 * @author 	André Duarte Bueno	
 * @see		Rotulador
*/
template<typename T>
class TCRotulador2D : public CRotulador, public TCMatriz2D<int>
{
  //--------------------------------------------------------------Atributos
 public:
  /** @brief Ponteiro para a imagem recebida no construtor ou em Go*/
	TCMatriz2D<T> * pm;

  //-------------------------------------------------------------Construtor
  /** Construtor. 
   * Aloca matriz de dados
   * Copia ponteiro para imagem recebida,
   * na funcão prepara imagem copia os dados
  */
	TCRotulador2D (TCMatriz2D<T> * _pm, int _indice=1, int _fundo=0) :
     CRotulador( _indice, _fundo ), TCMatriz2D< int >(_pm->NX(), _pm->NY()), pm(_pm) {
  	TCMatriz2D< int >::Path( _pm->Path() );
  }

  /// Construtor, abre imagem do disco
	TCRotulador2D (std::string nomeArquivo, int _indice=1, int _fundo=0) :
     CRotulador( _indice, _fundo ), TCMatriz2D< int >(nomeArquivo), pm(nullptr) {
  }

  /// Construtor, recebe dimensões da imagem
	TCRotulador2D (int nx, int ny, int _indice=1, int _fundo=0) :
     CRotulador( _indice, _fundo ), TCMatriz2D< int >(nx, ny), pm(nullptr) {
  }

  /** data2D deve ser alocado posteriormente*/
	TCRotulador2D (int _indice=1, int _fundo=0) :
     CRotulador( _indice, _fundo ), TCMatriz2D< int >(), pm(nullptr) {
  }

  //--------------------------------------------------------------Destrutor
  
  /// Destrutor
	virtual ~ TCRotulador2D ()
  {
  }

  //----------------------------------------------------------------Métodos
  protected:
  /**  @brief Verifica a imagem*/
	virtual bool PreparaImagem (TCMatriz2D<T> * matriz);

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
  //virtual void Go(TCMatriz2D< int >*& matriz, int rotuloInicial = 0 );
  /// Executa toda a sequencia de rotulagem
	virtual bool Go (TCMatriz2D<T> * matriz /*, int rotuloInicial = 0 */ );

  //--------------------------------------------------------------------Get
  //--------------------------------------------------------------------Set
  //-----------------------------------------------------------------Friend
	//friend ostream& operator<< (ostream& os, TCRotulador2D& obj);
	//friend istream& operator>> (istream& is, TCRotulador2D& obj);
};

//-----------------------------------------------------------------Friend
//Declaração de Funções Friend
//ostream& operator<< (ostream& os, TCRotulador2D& obj);
//istream& operator>> (istream& is, TCRotulador2D& obj);

#include <AnaliseImagem/Filtro/FEspacial/FERotulagem/TCRotulador2D.cpp>

#endif
