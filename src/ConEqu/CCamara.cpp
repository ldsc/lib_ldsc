/*
----------------------------------------------------------------------------
PROJETO:	LIB_LDSC
		Biblioteca de Objetos
----------------------------------------------------------------------------

Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico  
 dos Materiais.
Programadores:   Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani, 
Liang Zirong, Paulo C. Philippi,
		Marcos Damiani,...
Copyright @1997:  Todos os direitos reservados.
Nome deste arquivo:CCamara.h
Nome da classe:     CCamara
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
----------------------------------------------------------------------------
*/
#include <iostream>

using namespace std;

#include <ConEqu/CCamara.h>
			 
/** Função que define na imagem da camara as paredes da camara, chamando as diferentes funções de pintura da camara. */
void CCamara::DefineCamara ( /*int indice */ )
{
  DefineCamaraSuperior   ( /*indice */ );
  DefineMembranaSuperior ( /*indice */ );
  DefineParedeEsquerda   ( /*indice */ );
  DefineParedeDireita    ( /*indice */ );
  DefineMembranaInferior ( /*indice */ );
  DefineCamaraInferior   ( /*indice */ );
}

/// Operator<< insertion for cout, mostra atributos
ostream & operator<< (ostream & os, const CCamara & camara)
{
  // os << "Atributos do objeto  - Class CCamara" << "\n";
	os << camara.camaraSuperior << '\n'
	<< camara.membranaSuperior << '\n'
	<< camara.paredeEsquerda << '\n'
	<< camara.paredeDireita << '\n'
	<< camara.membranaInferior << '\n'
	<< camara.camaraInferior << '\n'
	<< camara.indiceParedes << '\n'
	<< camara.indiceCamaraSuperior << '\n'
	<< camara.indiceCamaraInferior << '\n'
	<< camara.nxImg << '\n'
	<< camara.nyImg << '\n'
	<< camara.nzImg << '\n';
	return os;
}

/// Operator>> extraction for cin, lê atributos
istream & operator>> (istream & is, CCamara & camara)
{
  // is >> (TImagem2D &) camara;
  // cout << "\nEntre com os atributos do objeto - Class CCamara";
  is >> camara.camaraSuperior;
  is >> camara.membranaSuperior;
  is >> camara.paredeEsquerda;
  is >> camara.paredeDireita;
  is >> camara.membranaInferior;
  is >> camara.camaraInferior;
  is >> camara.indiceParedes;
  is >> camara.indiceCamaraSuperior;
  is >> camara.indiceCamaraInferior;
  is >> camara.nxImg;
  is >> camara.nyImg;
  is >> camara.nzImg;
  return is;
}
