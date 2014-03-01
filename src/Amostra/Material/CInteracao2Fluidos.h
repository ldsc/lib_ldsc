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

/*
===============================================================================
Documentacao CLASSE: CMFluido
===============================================================================
Assunto:        Materiais
Superclasse:    CMFluido
@short		Representa um objeto fluido.
Descrição:
	
	A principio um fluido contém as propriedades viscosidade, densidade
	e compressibilidade.
	Posteriormente elaborar melhor a hierarquia de fluídos, para considerar
	outras propriedades usuais.
	Dê uma olhada na hierarquia materiais.
	
Obs:
  O construtor recebe estes atributos como parâmetros, se os mesmos não
  forem passados   para o construtor são assumidos valores default.
  Os atributos do objeto fluido podem ser alterados posteriormente
  acessando-se as 	funções SetAtributo.
  As propriedades default do fluido também podem estar armazenadas em
  um arquivo de disco   chamado fluido.dat.
	
Posteriormente

Obs:
O atributo molhabilidade é um atributo de ligação, surge da interação
de dois fluídos.
A principio vai ser definido pelo usuário, posteriormente deve
ser calculado em 	função das propriedades  dos fluidos.
Por este motivo a função Molhabilidade recebe um fluido B.

@author 	André Duarte Bueno	
@version						
@see		Veja a hierarquia TMateriais
*/
class CMFluido
{
// --------------------------------------------------------------Atributos
protected:

  // Viscodidade do fluído
  double viscosidade;

  // Densidade do fluido
  double densidade;

  // Compressibilidade é uma propriedade do fluido
  bool compressibilidade;

  // Atributos ligação, só existem a partir de um relacionamento
  // Surge da interação de dois fluidos
  // definida pela função Setmolhabilidade, ou
  // Molhabilidade(CMFluido* fb)
  bool molhabilidade;

public:
// -------------------------------------------------------------Construtor
  // Construtor com parametros
    CMFluido (double _v = 1, double _d = 1, bool _c = 1, bool _m = 1)
  {
    viscosidade = _v;
    densidade = _d;
    compressibilidade = _c;

    // Atributo de ligação
    molhabilidade = _m;
  }

  // Construtor de cópia
  CMFluido (const CMFluido & aCMFluido);

// --------------------------------------------------------------Destrutor
  virtual ~ CMFluido ()
  {
  }

// ----------------------------------------------------------------Métodos
public:
  // Calcula a molhabilidade do fluido com base no relacionamento
  // de this com fluidoB
  // Função ficticia

  void Molhabilidade (CMFluido * fluidoB) //  ELIMINAR/Trocar Esta errada
  {                                       //  deve retornar a molhabilidade
                                          //  calcula fora e aqui coloca se é molhante ou não
    molhabilidade = (fluidoB->densidade > this->densidade) ? 1 : 0;
  }

  // Abre e lê o arquivo de disco
  // virtual void Read(string nomeArquivo="fluido.dat");
  // virtual void Read(string nomeArquivo("fluido.dat"));     

// -------------------------------------------------------------Sobrecarga
  // Operador igualdade (atribuição)
  CMFluido & operator= (const CMFluido & aCMFluido);

  // Comparação igualdade
  bool operator== (const CMFluido & aCMFluido) const;

// --------------------------------------------------------------------Get
  double Viscosidade () const
  {
    return viscosidade;
  }

  bool Compressibilidade () const
  {
    return compressibilidade;
  }

  double Densidade () const
  {
    return densidade;
  }

  bool Molhabilidade () const
  {
    return molhabilidade;
  }

// --------------------------------------------------------------------Set
  void Viscosidade (double av)
  {
    viscosidade = av;
  };
  void Compressibilidade (bool ac)
  {
    compressibilidade = ac;
  }

  void Densidade (bool d)
  {
    densidade = d;
  }

  void Molhabilidade (bool d)
  {
    molhabilidade = d;
  }

// -----------------------------------------------------------------Friend
  // Sobrecarga streans
  // friend std::ostream & operator<< (std::ostream & os, const CMFluido & obj);
  // friend std::istream & operator>> (std::istream & is, CMFluido & obj);
};

// -----------------------------------------------------------------Friend
// Declaração de Funções Friend
// std::ostream & operator<< (std::ostream & os, const CMFluido & obj);
// std::istream & operator>> (std::istream & is, CMFluido & obj);

/*
class CInteracao2Fluidos
{
 public:

 bool molhabilidadeA;
 bool molhabilidadeB;

 public:
  // CMFluido* fluidoA;
  // CMFluido* fluidoB;

  // Construtor
  CInteracao2Fluidos (CMFluido* fa,CMFluido* fb)
  {
  // Exemplo: Em funcao das propriedades dos dois fluidos determina a molhabilidade destes.
  molhabilidadeA=(fb->Getdensidade() > fa->Getdensidade() )?1:0;
  molhabilidadeB=~molhabilidadeA;
  };
  
  // Destrutor
	 virtual ~CInteracao2Fluidos(){};

 public:
  bool MolhabilidadeA(bool m)	{  molhabilidadeA = m;};
  bool MolhabilidadeB(bool m)	{  molhabilidadeB = m;};

  bool MolhabilidadeA()const	{return molhabilidadeA;};
  bool MolhabilidadeB()const	{return molhabilidadeB;};
};
*/

#endif
