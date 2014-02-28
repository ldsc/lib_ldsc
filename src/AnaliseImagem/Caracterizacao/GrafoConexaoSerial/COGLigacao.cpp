/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:          André Duarte Bueno
@file:             COGLigacao.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>
#include <iomanip>
using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGLigacao.h>

/**
-------------------------------------------------------------------------
Função:
-------------------------------------------------------------------------
@short  : Adiciona os dois ponteiros recebidos objA e objB aos vetores de this.
@author : André Duarte Bueno
@see    : 
@param  : CObjetoGrafo * objA, CObjetoGrafo * objB
@return : void
*/
void COGLigacao::Conectar (CObjetoGrafo * objA, CObjetoGrafo * objB)
{
  coneccaoA.push_back (objA);
  coneccaoB.push_back (objB);
}

/**
-------------------------------------------------------------------------
Função:
-------------------------------------------------------------------------
@short  :   Calcula condutancia considerando eq.XX, ou seja,
            cond = 1/(1/condutancia_sitio_esq + 1/condutancia_this + 1/condutancia_sitio_dir).
@author :   André Duarte Bueno
@see    :
@param  :   nada
@return :   long double, o fluxo associado a ligação
*/
long double COGLigacao::Fluxo () const 
{
  long double fluxo { 0.0 };
  static long double condutanciaEntreObjetosConectados;

  for (unsigned long int i = 0; i < coneccaoA.size (); i++)
  {
   condutanciaEntreObjetosConectados =  1.0 / 
   (1.0/coneccaoA[i]->propriedade + 1.0/this->propriedade + 1.0/coneccaoB[i]->propriedade);
   // o fluxo é a condutancia total entre objetos vezes a diferença de x(pressão) dos objetos
   // a quem this esta conectado.
   fluxo += condutanciaEntreObjetosConectados * (this->coneccaoA[i]->x - this->coneccaoB[i]->x);
  }
  return fluxo;
}

/**
-------------------------------------------------------------------------
Função:     Write
-------------------------------------------------------------------------
@short  : Escreve propriedades do objeto em fout
@author : André Duarte Bueno
@see    :
@param  : ofstream& fout
@return : ostream&
*/
ostream & COGLigacao::Write (ostream & fout) const
{
    // Tipo de contorno
    /// @todo trocar por tipo ojeto grafo!
    // fout << setw (4) << Contorno ();
    fout << setw (4) << static_cast<unsigned char>( Contorno() ) << '\n';

    // Numero de links do sítio     
    fout << ' ' << setw (4) << coneccaoA.size ();

    // CONECCAO A
    // lista dos rótulos
    for ( auto objeto : coneccaoA )
      fout << ' ' << setw (4) << objeto->rotulo;

    // lista das propriedades (condutancias)
    for ( auto objeto : coneccaoA )
      fout << ' ' << setw (10) << objeto->propriedade;

    // CONECCAO B
    // lista dos rótulos
    for ( auto objeto : coneccaoB )
      fout << ' ' << setw (4) << objeto->rotulo;

    // lista das propriedades (condutancias)
    for ( auto objeto : coneccaoB )
      fout << ' ' << setw (10) << objeto->propriedade;
}

/**
-------------------------------------------------------------------------
Função:  operator<<
-------------------------------------------------------------------------
@short  : Escreve propriedades do objeto em fout
@author : André Duarte Bueno
@see    :
@param  : ostream& fout, COGLigacao& s
@return : ostream&
*/
ostream & operator<< (ostream & fout, COGLigacao & s)
{
  // Tipo de contorno
  //fout << " " << setw (4) << s.Contorno ();
  fout << setw (4) << static_cast<unsigned char>( s.Contorno() ) << '\n';

  // Rótulo de this (**)
  fout << " " << setw (5) << s.rotulo;

  // propriedade de this (condutancia) (**)
  fout << " " << setw (10) << s.propriedade;

  // x de this (pressão)
  fout << " " << setw (10) << s.x;

  // Número de links (coneccoes)
  unsigned long int numeroLinks = s.coneccaoA.size ();
  fout << " " << setw (4) << numeroLinks;

  // CONECCAO A
  // lista dos rótulos
  for ( auto objeto : s.coneccaoA )
     fout << " " << setw (5) << objeto->rotulo;
  // lista das propriedades (condutancias)
  for ( auto objeto : s.coneccaoA )
     fout << " " << setw (5) << objeto->propriedade;

  // CONECCAO B
  // lista dos rótulos
  for ( auto objeto : s.coneccaoB )
     fout << " " << setw (5) << objeto->rotulo;
  // lista das propriedades (condutancias)
  for ( auto objeto : s.coneccaoB )
     fout << " " << setw (5) << objeto->propriedade;

  return fout;
}

/*
-------------------------------------------------------------------------
Função:    operator>>
-------------------------------------------------------------------------
@short  : Lê as propriedades do objeto a partir de arquivo os 		
					(normalmente de disco)
@author :	André Duarte Bueno
@see    :
@param  :
@return :
*/
/*istream& operator>> (istream& is, COGLigacao& s)
{
  return is;
}
*/
