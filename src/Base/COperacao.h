#ifndef COperacao_h
#define COperacao_h

/*
===============================================================================
PROJETO:    Biblioteca LIB_LDSC
            Assunto/Ramo: COperacao...
===============================================================================
Desenvolvido por:	
            Laboratorio de Desenvolvimento de Software Cientifico 	
            [LDSC].
@author     Andre Duarte Bueno
@file       COperacao.h
@begin      Sat Sep 16 2000
@copyright  (C) 2000 by Andre Duarte Bueno


@email      andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <string>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef _LIB_LDSC_CLASS_h
#include <Base/_LIB_LDSC_CLASS.h>
#endif 

using namespace std;
/**
 * @brief Classe definida para realizacao de operacaes comuns;
 * usadas em diferentes tipos de procedimentos.
 * 
 * Sao funcaes que normalmente seriam globais.
 * Revisar a necessidade desta classe.
 * 
@author 	Andre Duarte Bueno	
@version	
@see		Diversos
*/
class COperacao
{
// --------------------------------------------------------------Atributos
public:

  // Enumerated para resposta
  // ex: if( obj->Resposta == 0)    
	/**
	 * EResposta a uma enumeracao para respostas do usuario
	*/
  enum EResposta
  { no = 0, yes, dont_care };

// -------------------------------------------------------------Construtor
  COperacao ()
  {
  }


// --------------------------------------------------------------Destrutor
  virtual ~ COperacao ()
  {
  }

// ----------------------------------------------------------------Matodos
  // Manipulcaao de atributos
  // static void   swapDouble(double& d1, double& d2);   ->STL
  // static void   swapInt(int& i1, int& i2);            ->STL

  /// Testa a alocacao de ponteiros, recebe string para mensagem de erro.
  static void TestaAlocacao (void *ptr, std::string s);

  // static double  ErroPercentual(double x,double y);

  /// Manipulcao de tela modo texto (Criar TMT Tipo ManipulacaoTela)
  /// realiza uma pausa.
  static void Pausa (); // ver exemplo de Pausa em CGnuplot

  /// Limpar a tela
  static void LimpaTela ();

  /// Mostra indicador de que o programa esta rodando.
  static void RunningView ();

  /// Mostra indicador de que o programa esta rodando e acrescenta mensagem passada como parâmetro.
  static void RunningView (string msg);
  
  // As funcaes abaixo usavam a strxfrm que nao existe no Linux
  // por isso desativei. Refazer usando padrao Ansi C
};

typedef COperacao CO;
#endif
