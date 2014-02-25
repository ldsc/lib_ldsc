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
//#include <cstdlib>
//#include <cstdio>
//#include <cstring>

#include <Tempo/CTime.h>
using namespace std;

// Construtor, recebe uma string (normalmente com o nome da função que chamou this)
// e armazena em inicio a hora atual.
CTime::CTime (string _msg, ostream * _out):
    /*inicio (chrono::high_resolution_clock::now()),  */fout( _out), msg( _msg)
{
}

CTime::CTime (string _msg, ofstream * _out):
  /*inicio (chrono::milliseconds::now()),*/ fout( _out), msg( _msg)
{
//  inicio = time (nullptr);
}

// Destrutor, apresenta o tempo de execução da função em milisegundos.
CTime::~CTime ()
{
//  time_t fim = time (nullptr);
//  (*fout) << '\n' << msg << " " << difftime (fim, inicio) << 's' << endl;
//    auto fim = std::chrono::milliseconds::now();
    auto fim = std::chrono::high_resolution_clock::now();
    (*fout) << '\n' << msg << ' '
            << chrono::duration_cast<std::chrono::milliseconds>(fim-inicio).count()
            << "_milliseconds" << endl;
}

//Exemplo
//#include <iostream>
//#include <chrono>
//#include <thread>
//void f()
//{
//    std::this_thread::sleep_for(std::chrono::seconds(1));
//}
//int main()
//{
//    auto inicio = std::chrono::high_resolution_clock::now();
//    f(); chama funcao
//    auto fim = std::chrono::high_resolution_clock::now();
//    std::cout << "f() took "
//              << std::chrono::duration_cast<std::chrono::milliseconds>(fim-inicio).count()
//              << " milliseconds\n";
//}
