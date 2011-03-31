/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: TPadrao_ramo
===============================================================================

Desenvolvido por:Laboratorio de Desenvolvimento de Software Cientifico
			[LDSC].
@author:          André Duarte Bueno
File:             COGComponenteGrafo.cpp
begin:            Sat Sep 16 2000
copyright:        (C) 2000 by André Duarte Bueno
email:            andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <iomanip>
using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Grafo/COGComponenteGrafo.h>

// -------------------------------------------------------------------------
// Função:       Conectar
// -------------------------------------------------------------------------
/** Função: 	Conectar
@see  	:	Adiciona ao vetor conexão o vetor passado
@author:  André Duarte Bueno
@param	:	recebe um vetor de  CObjetoGrafo*
@return : void
*/
void
COGComponenteGrafo::Conectar (vector < CObjetoGrafo * >obj_vetor)
{
  coneccao.push_back (obj_vetor);
};

// ------------------------------------------------------------------------------
// Funcao:     Write
// ------------------------------------------------------------------------------
/** Salva o grafo em disco (em fout)
@see  		:
@author:    André Duarte Bueno
@param		: Recebe uma ofstream& fout
@return   : ostream&
*/
ostream & COGComponenteGrafo::Write (ostream & fout) const
{
  // ofstream fout(fileName,ios::app);
  //      if(fout)
  {
    // Tipo de contorno
    fout << setw (4) << Contorno () << endl;

    // Número de vetores coneccoes
    unsigned long int
      total_de_vetores = coneccao.size ();
    fout << " " << setw (4) << total_de_vetores;

    // Para cada vetor percorrer a lista
    unsigned long int
      numeroLinks;
    for (unsigned long int cont_vector = 0; cont_vector < total_de_vetores;
	 cont_vector++)
      {
	// Para cada vetor percorrer os objetos
	// coneccao[cont_vector] retorna um ponteiro para um vetor
	numeroLinks = coneccao[cont_vector].size ();
	fout << " " << setw (4) << numeroLinks;

	// lista dos rótulos
	for (unsigned long int cont_link = 0; cont_link < numeroLinks;
	     cont_link++)
	  fout << " " << setw (4) << coneccao[cont_vector][cont_link]->rotulo;

	// lista das propriedades (condutancias)
	for (unsigned long int cont_link = 0; cont_link < numeroLinks;
	     cont_link++)
	  fout << " " << setw (4) << coneccao[cont_vector][cont_link]->
	    propriedade;
      }
  }
  return fout;
}
