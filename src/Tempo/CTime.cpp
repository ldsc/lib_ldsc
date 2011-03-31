/*
----------------------------------------------------------------------------
PROJETO:		Biblioteca Anaimp
----------------------------------------------------------------------------
Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico   dos Materiais.
Programadores:   	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, Liang Zirong,
			Damiani,Paulo C. Philippi, ...
Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo:	CTime.cpp
Nome da classe:         CTime
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Observações:   Implementa um objeto de marcação de tempo de execução de uma função
*/

// ----------------------------------------------------------------------------
// Bibliotecas
// ----------------------------------------------------------------------------
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <Tempo/CTime.h>
using namespace std;

// Construtor, recebe uma string (normalmente com o nome da função que chamou this)
// e armazena em inicio a hora atual.
CTime::CTime (string _msg, ostream * _out):
  inicio (time (NULL)),  fout( _out), msg( _msg)
{
}

CTime::CTime (string _msg, ofstream * _out):
  fout( _out), msg( _msg)
{
  inicio = time (NULL);
}

// Destrutor, apresenta o tempo de execução da função
CTime::~CTime ()
{
  time_t fim = time (NULL);
  (*fout) << '\n' << msg << " " << difftime (fim, inicio) << 's' << endl;
}


