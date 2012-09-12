#ifndef CMatriz2D_h
#define CMatriz2D_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CMatriz2D...
===============================================================================
Desenvolvido por:
            Laboratorio de Desenvolvimento de Software Cientifico
            [LDSC].
@author     André Duarte Bueno
@file       CMatriz2D.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno

@email      andre@lmpt.ufsc.br
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

#ifndef CBaseMatriz_h
#include <Matriz/CBaseMatriz.h>
#endif
using namespace std;
/**
 * @brief	Representa uma matriz bidimensional.
 * Permite criar e usar uma matriz bidimensional.
 * A alocacao e feita dinamicamente
 * Tem tres construtores:
 * 1 - Para criar uma matriz toda nova use
 * CMatriz2D(NX,NY),cria matriz, aloca data2D, zera valores
 * 2 - Para criar uma copia no programa use
 * CMatriz2D(CMatriz2D), cria uma copia
 * CMatriz2D=CMatriz2D;
 * 3 - Para abrir uma matriz do disco use
 * CMatriz2D("nomearquivo")
 * CMatriz2D.Read("nomearquivo")
 * Para salvar a matriz em disco use
 * CMatriz2D.Write(std::string fileName)
 * Para acessar o conteudo da matriz use data2D
 * CMatriz2D.data2D[i][j]=33;
 * int x=CMatriz2D.data2D[i][j];
 *
 * PS: Observe que com as funções AlocaMatriz2D, pode-se alocar uma estrutura de dados 2D
 * diretamente, sem criar um objeto matriz.
 * Ex: 	int **m2D = CMatriz2D::AlocaMatriz2D(nx,ny);
 * @author 	André Duarte Bueno
 * @see		Veja assunto
*/
class CMatriz2D : public CBaseMatriz
{
// --------------------------------------------------------------Atributos
protected:
  int ny;   	/// Dimensão ny
  int nx;   	/// Dimensão nx

public:
  int **data2D; /// Ponteiro para matriz 2D de inteiros
// -------------------------------------------------------------Construtor
  /// Construtor default, data2D=NULL nx=ny=0;
    CMatriz2D ();

  /// Construtor le CMatriz2D do disco sempre testar se data2D!=NULL
    CMatriz2D (std::string fileName);

  /// Lê plano de uma matriz 3D o primeiro plano é o z=0
    CMatriz2D (std::string fileName, int planoZ);

  /// Construtor cria copia CMatriz2D
    CMatriz2D (CMatriz2D &);

  	/// Construtor cria matriz extendida, com borda extra. Copia matriz na parte central
	// util pois cria uma borda extra, zerada.
	CMatriz2D ( CMatriz2D & matriz, unsigned int borda );

  /// Construtor cria nova CMatriz2D dados=lixo, para zerar Constante(0);
    CMatriz2D (int _nx, int _ny);

// --------------------------------------------------------------Destrutor

  /// Destrutor, chama Desaloca
    virtual ~ CMatriz2D ()
  {
    CMatriz2D::DesalocaMatriz2D (data2D, nx, ny);
  }

// ----------------------------------------------------------------Métodos
protected:
  /// Alocacao
  virtual bool Aloca ()
  {
    data2D = CMatriz2D::AlocaMatriz2D (nx, ny);
    return data2D ? 1 : 0;
  }

  /// Desalocacao
  virtual bool Desaloca ()
  {
    CMatriz2D::DesalocaMatriz2D (data2D, nx, ny);
    return data2D ? 0 : 1;
  }

  // As funções abaixo devem ser movidas para a classe CImagem2D
  // COLOQUEI APENAS ENQUANTO NAO TROCO NO ANAIMP CBaseMatriz POR TIMAGEM
  /// Le os dados separados por " "
  void LeDados (std::ifstream & fin);

  /// Le os dados, colados 010111001
  void LeDadosColados (std::ifstream & fin);

	/// Le os dados gravados em formato binario
	void LeDadosBinarios (std::ifstream & fin);

public:

  /// Salva dados do cabecalho
  virtual void SalvaCabecalho (std::ofstream & fout) const;

	/// Salva dados em formato binario
	virtual void SalvaDadosBinarios (std::ofstream & fout) const;

  /// Salva dados "colados" sem espaço
  virtual void SalvaDadosColados (std::ofstream & fout) const;

  /// Salva dados com um espaco " "
  virtual void SalvaDados (std::ofstream & fout) const;

  /// lê o plano z, de uma matriz 3D
  bool LePlanoZ (std::string fileName, int planoZ, bool separado = true);

  /// Redimensiona a matriz
  virtual bool Redimensiona (int NX, int NY = 0, int NZ = 0)
  {
    if( nx != NX || ny != NY ) {
    		CMatriz2D::DesalocaMatriz2D (data2D, nx, ny);
    		nx = NX;
    		ny = NY;
    		NZ = 0; // evitar warning
    		data2D = CMatriz2D::AlocaMatriz2D (nx, ny);
    		return data2D ? 1 : 0;
    }
    return true; // não precisou redimensionar...
  }

  /// Aloca uma matriz de dados 2D qualquer.
  static int **AlocaMatriz2D (int nx, int ny);

  /// Desaloca dat
  static bool DesalocaMatriz2D (int **&dat, int nx, int ny);

  /// Preenche com valor constante
  virtual void Constante (int cte);

  /// Inverte valores (0)-->(1)  (>0)-->(0)
  virtual void Inverter ();

  /// Retorna para os as propriedades da matriz
  virtual void Propriedades (std::ofstream & os) const;
  // virtual void        Propriedades(std::ostream& os)const;

  /// Retorna o maior valor da matriz
  virtual int MaiorValor () const;

  /// Retorna o menor valor da matriz
  virtual int MenorValor () const;

  /// Retorna o menor valor da matriz (diferente de zero). Se a matriz só tiver zeros, irá retornar 9999999999
  virtual int MenorValorNzero () const;

  /// Retorna o um par correspondente ao maior e ao menor valor da matriz (diferente de zero). Se a matriz só tiver zeros, irá retornar 0 e 9999999999
  virtual pair<int,int> MaiorMenorValorNzero () const;

  /// Calcula e retorna a média
  virtual double Media () const;

  virtual int DimensaoMatriz () const
  {
    return 2;
  }

  /// Troca todos os valores i por j no vetor, retorna o numero de elementos trocados
  virtual int Replace (int i, int j);

  bool ChecaIndice (int NX, int NY) const
  {
    return (NX >= 0 && NX < nx && NY >= 0 && NY < ny) ? 1 : 0;
  }

  /// Como é chamada pelo construtor nao pode ser virtual
  bool Read (std::string fileName, int separado = 1);

  // Novidade trazida para cá de COperacao
  /// Lê imagem 2D do disco, usa vetor de dados
//  static bool Read2D (std::string inputFile, BUG float * _reDdata, int _nx,  int _ny);
  static bool Read2D (std::string inputFile, float * & _reDdata, int _nx,  int _ny);

  /// Salva imagem 2D no disco, usa vetor de dados
  static bool Write2D (std::string inputFile,  float * _redata, int _nx,  int _ny);

  /// rotaciona a imagem 90 graus a direita
  bool Rotacionar90 ();

// -----------------------------------------------------------------------Sobrecarga de operador
  /// Sobrecarga +
  CMatriz2D & operator+ (CMatriz2D & pm2);

  /// Sobrecarga -
  CMatriz2D & operator- (CMatriz2D & pm2);

  /// Sobrecarga =
  CMatriz2D & operator= (CMatriz2D & pm2);

  // CMatriz2D* operator*(CMatriz2D*& m2);
  // Sobrecarga *

  /// Sobrecarga ==
  bool operator== (CMatriz2D & pm2);

  /// Sobrecarga !=
  bool operator!= (CMatriz2D & pm2);

  /// Aceita matriz(x,y)
  inline int &operator  () (int x, int y) const
  {
    return data2D[x][y];
  };

  /// Poderia criar um SetColuna[y], ai teria data2D[x][coluna]
  inline int &operator[] (int x) const
  {
    return data2D[x][0];
  }

// --------------------------------------------------------------------Get
  /// Retorna nx
  inline int NX () const
  {
    return nx;
  }

  /// Retorna ny
  inline int NY () const
  {
    return ny;
  }

  /// Retorna data2D
  inline int **Data2D () const
  {
    return data2D;
  }

// --------------------------------------------------------------------Set
  /// Define nx
  inline void NX (int NX)
  {
    nx = NX;
  }

  /// Define ny
  inline void NY (int NY)
  {
    ny = NY;
  }

// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CMatriz2D& obj);
//       friend istream& operator>> (istream& is, CMatriz2D& obj);
// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
/// Sobrecarga operador<<.
 friend std::ostream& operator<< (std::ostream& os, const CMatriz2D& obj);

/// Sobrecarga operador>>.
 friend std::istream& operator>> (std::istream& is, CMatriz2D& obj);

};

//std::ostream& operator<< (std::ostream& os, const CMatriz2D& obj);

//std::istream& operator>> (std::istream& is, CMatriz2D& obj);

#endif
