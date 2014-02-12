#if !defined(CTime_h)
#define CTime_h

/*
  ----------------------------------------------------------------------------
  PROJETO:		Biblioteca LDSC
  ----------------------------------------------------------------------------
  Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   
dos Materiais.
  Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,
 Liang Zirong,
  M.C.Damiani,Paulo C. Philippi, ...
  Copyright @1997:  	Todos os direitos reservados.
  Nome deste arquivo:	CTime.h
  Nome da classe:     CTime
  Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
*/

// ----------------------------------------------------------------------------
// Bibliotecas C/C++
// ----------------------------------------------------------------------------
//#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

// ----------------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// ----------------------------------------------------------------------------
// #ifndef _LIB_LDSC_CLASS_h
// #include <Base/_LIB_LDSC_CLASS.h>
// #endif

/**
 * @brief  Classe de controle de tempo de execucao de uma função qualquer
 * 
 * Exemplo de uso:
 * No inicio da função cria um objeto : 
 * CTime wait("nomeFuncao");
 * 
 * Quando a função sai de escopo o objeto wait do tipo CTime é destruido
 * e o tempo decorrido no processamento da função (em milisegundos),
 * é passado para a tela utilizando uma ostream.
 * @author       André Duarte Bueno	
 * 
 * Exemplo de uso:
 * #include <Tempo/CTime.h>
 * main()
 * {
	* {
	* CTime t(string("Demora criação e deleção do objeto CTime."),cout );
	* }
 * }
*/

class CTime
{
  // Atributos
  /// string com o nome da função executada.
  std::string msg{};

  /// Ponteiro para uma ostream.
  std::ostream * fout = nullptr;

  /// Hora em que a função iniciou.
//  std::chrono::high_resolution_clock inicio;
//  std::chrono::milliseconds	inicio;
  decltype(std::chrono::high_resolution_clock::now()) inicio;

  // Métodos
 public:

  /// Construtor, recebe mensagem e ostream de saída
  CTime (std::string msg, std::ostream * _out);
  
  /// Construtor, recebe mensagem e ofstream de saída
  CTime (std::string msg, std::ofstream * _out);

  /// Destrutor
  ~CTime ();
};

#endif //  CTime
