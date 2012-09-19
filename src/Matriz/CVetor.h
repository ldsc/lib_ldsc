#ifndef CVetor_h
#define CVetor_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: CVetor...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     André Duarte Bueno
@file       CVetor.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by André Duarte Bueno
@email      andre@lmpt.ufsc.br
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

#ifndef CBaseMatriz_h
#include <Matriz/CBaseMatriz.h>
#endif

/**
 * @brief Representa um vetor.
 * 
 * Permite criar e usar um vetor. 
 * A alocacao e feita dinamicamente
 * Tem tres construtores,
 * 
 * Para criar um vetor todo novo use
 * CVetor(NX),cria vetor, aloca data, zera valores
 * 
 * Para criar uma copia no programa use
 * CVetor(CVetor), cria uma copia
 * CVetor=CVetor;
 * 
 * Para abrir um vetor do disco use
 * CVetor("nomearquivo"), cria um vetor novo
 * CVetor.Read("nomeArquivo");, lê um vetor do disco
 * 
 * Para salvar o vetor em disco use
 * CVetor.Write(std::string fileName)
 * Para acessar o conteudo do vetor use data
 * CVetor.data1D[i]=33;
 * int x=CVetor.data1D[i];
 * @author 	André Duarte Bueno	
 * @see		Matriz
*/
class CVetor:public CBaseMatriz
{
// --------------------------------------------------------------Atributos

protected:

	int nx;   ///< Dimensao nx do vetor

public:

  /// Ponteiro para vetor 1D
  int *data1D;

// -------------------------------------------------------------Construtor

  // Métodos construtores          
    /// Construtor
  CVetor ();

  /// Construtor le CVetor do disco
    CVetor (std::string fileName);

  /// Construtor cria copia CVetor
    CVetor (CVetor &);

  /// Construtor cria nova CVetor
    CVetor (int _nx);


// --------------------------------------------------------------Destrutor
  ///  Destrutor
     virtual ~ CVetor ()
  {
    CVetor::DesalocaVetor (data1D, nx);
  }

// ----------------------------------------------------------------Métodos
protected:
  /// Le os dados separados por " "
  void LeDados (std::ifstream & fin);

  /// Le os dados, colados 010111001
  void LeDadosColados (std::ifstream & fin);

  // -----------------------------------------------------------------------
  // Metodos herdados virtuais
  // -----------------------------------------------------------------------
  // Alocacao, Desalocacao
  /// Aloca especificamente data1D, retorna 1 se alocou corretamente
  virtual bool Aloca ()
  {
    data1D = CVetor::AlocaVetor (nx);
    return data1D ? 1 : 0;
  }

  /// Desaloca especificamente data1D,  retorna 1 se desalocou corretamente
  virtual bool Desaloca ()
  {
    CVetor::DesalocaVetor (data1D, nx);
    return data1D ? 0 : 1;
  }

  /// Salva dados do cabecalho
  virtual void SalvaCabecalho (std::ofstream & fout) const;

	/// Salva dados em formato binario
	virtual void SalvaDadosBinarios (std::ofstream & fout) const;

  /// Salva dados "colados" sem espaço
  virtual void SalvaDadosColados (std::ofstream & fout) const;

  /// Salva dados com um espaco " "
  virtual void SalvaDados (std::ofstream & fout) const;

public:

  /// Recebe a nova dimensão do vetor,/ retorna 1 se realocou corretamente
  virtual bool Redimensiona (int NX, int NY = 0, int NZ = 0)
  {
    NY = 0; NZ = 0; //evitar warning
    CVetor::DesalocaVetor (data1D, nx);
    nx = NX;
    data1D = CVetor::AlocaVetor (nx);
    return data1D ? 1 : 0;
  }

  /// Aloca um vetor de dados quaisquer
  static int *AlocaVetor (int nx);

  /// Desaloca dat
  static bool DesalocaVetor (int *dat, int nx);

  /// Checa se o indice é válido
  bool ChecaIndice (int NX) const
  {
    return (NX >= 0 && NX < (int) nx) ? 1 : 0;
  }

  // Como é chamada pelo construtor nao pode ser virtual
  // Definida na classe base
  /// Lê vetor do disco
  bool Read (std::string fileName, bool separado = 1);

  /// Preenche com valor constante
  virtual void Constante (int cte);

  /// Inverte valores (0)-->(1)  (>0) -->0
  virtual void Inverter ();

  /// Retorna para os as propriedades da matriz
  virtual void Propriedades (std::ofstream & os) const;

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

  /// Retorna a dimensão da matriz
  virtual int DimensaoMatriz () const
  {
    return 1;
  }

  /// Troca todos os valores i por j no vetor, retorna o numero de elementos trocados
  virtual int Replace (int i, int j);

// -------------------------------------------------------------Sobregarda
  // Sobrecarga de operador
  /// Sobrecarga operador+
  CVetor & operator+ (CVetor & v);
  
    /// Sobrecarga operador-
  CVetor & operator- (CVetor & v);
  
    /// Sobrecarga operador=
  CVetor & operator= (CVetor & v);

  // TCMatriz2D< int >* operator*(TCMatriz2D< int >*& m2);
    /// Sobrecarga operador==
  bool operator== (CVetor & v) const;
  
    /// Sobrecarga operador!=
  bool operator!= (CVetor & v) const;

    /// Sobrecarga operador()
  inline int &operator  () (int x) const
  {
    return data1D[x];
  }
  
    /// Sobrecarga operador[]
  inline int &operator[] (int x) const
  {
    return data1D[x];
  };

// --------------------------------------------------------------------Get
  /// Obtêm nx
  inline int NX () const
  {
    return nx;
  }
  
  /// Obtêm data1D
  inline int *Data1D () const
  {
    return data1D;
  }


// --------------------------------------------------------------------Set
  
  /// Seta nx
  inline void NX (int _nx)
  {
    nx = _nx;
  }

// -----------------------------------------------------------------Friend
//       friend ostream& operator<< (ostream& os, CVetor& obj);
//       friend istream& operator>> (istream& is, CVetor& obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// ostream& operator<< (ostream& os, CVetor& obj);
// istream& operator>> (istream& is, CVetor& obj);

#endif
