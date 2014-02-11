/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: CMatriz
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
			[LDSC].
@author:          André Duarte Bueno
File:             CImagem.cpp
begin:            Sat Sep 16 2000
copyright:        (C) 2000 by André Duarte Bueno
email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>
#include <algorithm>
using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Matriz/CImagem.h>

/*
-------------------------------------------------------------------------
Função:  contem
-------------------------------------------------------------------------
@short  :Verifica se a imagem contem o ponto passado
@author :André Duarte Bueno
@see    :
@param  :Posição x e y do píxel a ser testado
@return :
*/
/*
bool CImagem::Contem (int i, int j)  const
{
  // acusa ser sempre verdadeira?
  if (i < nx && j < ny)
    return 1;
  else
    return 0;
};
*/

/*
-------------------------------------------------------------------------
Função:  Inverter
-------------------------------------------------------------------------
@short  :	Inverte valores 0-1
@author : André Duarte Bueno
@see    : Matrizes
@param  : void
@return : void
*/
void
CImagem::Inverter ()
{
  for (int i = 0; i < nx; i++)
    for (int j = 0; j < ny; j++)
      if (data2D[i][j] == 0)
	data2D[i][j] = 1;
      else
	data2D[i][j] = 0;
}

/*
-------------------------------------------------------------------------
Função:  Intersecao
-------------------------------------------------------------------------
@short  :Faz a interseção entre this e pm2
@author :André Duarte Bueno
@see    :Matrizes
@param  :Um ponteiro para uma matriz bidimensional
@return :Retorna this, com o resultado da intersecção
*/
TCMatriz2D< int > *
CImagem::Intersecao (TCMatriz2D< int > * pm2)
{
  // int minx = std::min(this->nx,pm2->NX());
  int minx = this->nx > pm2->NX ()? pm2->NX () : this->nx;

  // int miny = std::min(this->ny,pm2->NY());
  int miny = this->ny > pm2->NY ()? pm2->NY () : this->ny;

//        int minx  =  std::min( this->nx , pm2->NX()  ) ;
//        int miny = std::min( this->ny , pm2->NY() );

  for (int i = 0; i < minx; i++)
    for (int j = 0; j < miny; j++)
      if (this->data2D[i][j] && pm2->data2D[i][i])	// se ambos diferentes de 0

	this->data2D[i][j] = 1;	// ativa 1
      else
	this->data2D[i][j] = 0;	// senão 0
  return this;
}

/*
-------------------------------------------------------------------------
Função:  Uniao
-------------------------------------------------------------------------
@short  :
	Faz a união entre this e pm2
	Verifica pontos ativos em pm2, se for ativo em pm2 ativa em this

@author :	André Duarte Bueno
@see    : Matrizes
@param  : Uma matriz bidimensional
@return :	Retorna uma matriz bidimensional
*/
TCMatriz2D< int > *
CImagem::Uniao (TCMatriz2D< int > * pm2)
{
  int minx = this->nx > pm2->NX ()? pm2->NX () : this->nx;
  int miny = this->ny > pm2->NY ()? pm2->NY () : this->ny;

  for (int i = 0; i < minx; i++)
    for (int j = 0; j < miny; j++)
      if (pm2->data2D[i][j])
	this->data2D[i][j] = 1;
  return this;
}

/*
-------------------------------------------------------------------------
Função:  Global (Liang)
-------------------------------------------------------------------------
@short  :
Abaixo uma função global pertencia a biblioteca lib.c aloca uma matriz 2D.
trocar por TCMatriz2D< int >* =  new TCMatriz2D< int >(nr,nc);

@author :	André Duarte Bueno
@see    :	Matrizes
@param  : Dimensões nx y ny
@return : Ponteiro para matriz de float's
*/
float **
f2d (int nx, int ny) // CHAMAR TCMatriz2D< int ><float>::Aloca2D(nx,ny) 
{
  float **ptr_m2D = 0;			// , *y;
  int i;

  ptr_m2D = (float **) calloc (nx, sizeof (float *));
  if (ptr_m2D == 0)
    {
      ofstream fout ("anaimp.err");
      fout << "Out of storage: na função F2D.\n";
      fout.close ();
      cerr << "Out of storage: na função F2D.\n";
      exit (1);
    }

  for (i = 0; i < nx; i++)
    {
      ptr_m2D[i] = 0;
      ptr_m2D[i] = (float *) calloc (ny, sizeof (float));
      if (ptr_m2D[i] == 0)
	{
	  ofstream fout ("anaimp.err");
	  fout << "Out of storage: na função F2D.\n";
	  fout.close ();
	  cerr << "Out of storage: na função F2D.\n";
	  exit (1);
	}
    }
  return ptr_m2D;
}
