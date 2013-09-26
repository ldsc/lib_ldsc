/*
=========================================================================
PROJETO:    Biblioteca libldsc CObjetoImagem
=========================================================================
Desenvolvido por:
						LDSC - Laboratorio de Desenvolvimento de Software Científico
@author     Andre Duarte Bueno - http://www.lenep.uenf.br/~bueno
@begin      2010
@copyright  (C) 2010 by Andre Duarte Bueno - http://www.lenep.uenf.br/~bueno
@email      <bueno@lenep.uenf.br>
@file 	  CObjetoImagem.cpp
@license    GNU General Public License - version 2
						see  $LICENSEFILE$ for the full license text.
*/

#include <Segmentacao/CObjetoImagem.h>

/// Grava as informações do objeto no arquivo recebido como parâmetro.
void CObjetoImagem::GravarObjeto(ofstream &_fout) {
	// X\tY\tZ\tN.Voxeis\tN.ObjsCon\tLstObjsCons
	_fout << pontoCentral.x << "\t";
	_fout << pontoCentral.y << "\t";
	_fout << pontoCentral.z << "\t";
	_fout << numObjs << "\t";
	_fout << sConexao.size();
	std::set<int>::iterator itr;
	for (itr=sConexao.begin(); itr!=sConexao.end(); ++itr) {
		_fout << "\t" << *itr;
	}
	_fout << endl;
}


// Retorna tipo de objeto como uma string
/*
std::string CObjetoImagem::Tipo()
{
	switch ( tipo )
	{
		case SOLIDO: 		return "Solido";
		case SITIO: 		return "Sitio";
		case LIGACAO: 		return "Ligacao";
		case RAMO_MORTO: 	return "Ramo_Morto";
		default: 		return "Não identificado";
	}
}
*/
/*
/// Seta o tipo de objeto a partir de uma string
void CObjetoImagem::Tipo ( std::string _tipo )
{
	if ( _tipo == "Sitio" )
		tipo = SITIO;
	else if ( _tipo == "Ligacao" )
		tipo = LIGACAO;
	else if ( _tipo == "Ramo_Morto" )
		tipo = RAMO_MORTO;
	else
		tipo = NAO_IDENTIFICADO;

	return;
}
*/
