/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
	[LDSC].
@author:          André Duarte Bueno
File:             COGLigacao.cpp
begin:            Sat Sep 16 2000
copyright:        (C) 2000 by André Duarte Bueno
email:            andreduartebueno@gmail.com
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

/*
-------------------------------------------------------------------------
Função:
-------------------------------------------------------------------------
@short  :
Adiciona os dois ponteiros recebidos objA e objB a lista de this
@author :André Duarte Bueno
@see    :
@param  :
@return :
*/

void
COGLigacao::Conectar (CObjetoGrafo * objA, CObjetoGrafo * objB)
{
  coneccaoA.push_back (objA);
  coneccaoB.push_back (objB);
}

/*
-------------------------------------------------------------------------
Função:
-------------------------------------------------------------------------
@short  : Condutancia da ligação vezes, a pressao deste sítio menos  a
          pressao do sítio conexo
	  Declarado no CGrafo  vector<CObjetoGrafo*> objeto;
@author :   André Duarte Bueno
@see    :
@param  :   nada
@return :   long double, o fluxo associado a ligação
*/
long double
COGLigacao::Fluxo () const 
{
  long double fluxo = 0;

  for (unsigned long int i = 0; i < coneccaoA.size (); i++)
    // fluxo += condutancia_ligacao * (sitioA[i]->pressão -sitioB[i]->pressão) 
    // fluxo += propriedade *  (coneccaoA[i]->x  - coneccaoB[i]->x);
    fluxo += propriedade * (coneccaoA[i]->x - coneccaoB[i]->x);
  return fluxo;
}

/*
-------------------------------------------------------------------------
Função:     Write
-------------------------------------------------------------------------
@short  :		Escreve propriedades do objeto em fout
@author :		André Duarte Bueno
@see    :
@param  :   ofstream& fout
@return :  	ostream&
*/
ostream & COGLigacao::Write (ostream & fout) const
{
  // ofstream fout(fileName,ios::app);
  //      if(fout)
  {
    // Tipo de contorno
    fout << setw (4) << Contorno ();

    // Número de links (coneccoes)
    unsigned long int
      numeroLinks = coneccaoA.size ();
    // Numero de links do sítio     
    fout << ' ' << setw (4) << numeroLinks;

    // CONECCAO A
    // lista dos rótulos
    for (unsigned long int cont_link = 0; cont_link < numeroLinks;
	 cont_link++)
      fout << ' ' << setw (4) << coneccaoA[cont_link]->rotulo;

    // lista das propriedades (condutancias)
    for (unsigned long int cont_link = 0; cont_link < numeroLinks;
	 cont_link++)
      fout << ' ' << setw (4) << coneccaoA[cont_link]->propriedade;

    // CONECCAO B
    // lista dos rótulos
    for (unsigned long int cont_link = 0; cont_link < numeroLinks;
	 cont_link++)
      fout << ' ' << setw (4) << coneccaoB[cont_link]->rotulo;

    // lista das propriedades (condutancias)
    for (unsigned long int cont_link = 0; cont_link < numeroLinks;
	 cont_link++)
      fout << ' ' << setw (4) << coneccaoB[cont_link]->propriedade;

  }
  return fout;
// fout.close();
}

/*
-------------------------------------------------------------------------
Função:  	 operator<<
-------------------------------------------------------------------------
@short  :	Escreve propriedades do objeto em fout
@author :	André Duarte Bueno
@see    :
@param  : ostream& fout, COGLigacao& s
@return : ostream&
*/
ostream & operator<< (ostream & fout, COGLigacao & s)
{
  // Tipo de contorno
  fout << " " << setw (3) << s.Contorno ();

  // Rótulo de this (**)
  fout << " " << setw (5) << s.rotulo;

  // propriedade de this (condutancia) (**)
  fout << " " << setw (10) << s.propriedade;

  // x de this (pressão)
  fout << " " << setw (10) << s.x;

  // Número de links (coneccoes)
  unsigned long int
    numeroLinks = s.coneccaoA.size ();
  fout << " " << setw (4) << numeroLinks;

  // CONECCAO A
  // lista dos rótulos
  for (unsigned long int cont_link = 0; cont_link < numeroLinks; cont_link++)
    fout << " " << setw (5) << s.coneccaoA[cont_link]->Getrotulo ();
  // lista das propriedades (condutancias)
  for (unsigned long int cont_link = 0; cont_link < numeroLinks; cont_link++)
    fout << " " << setw (5) << s.coneccaoA[cont_link]->propriedade;

  // CONECCAO B
  // lista dos rótulos
  for (unsigned long int cont_link = 0; cont_link < numeroLinks; cont_link++)
    fout << " " << setw (5) << s.coneccaoB[cont_link]->Getrotulo ();
  // lista das propriedades (condutancias)
  for (unsigned long int cont_link = 0; cont_link < numeroLinks; cont_link++)
    fout << " " << setw (5) << s.coneccaoB[cont_link]->propriedade;

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
