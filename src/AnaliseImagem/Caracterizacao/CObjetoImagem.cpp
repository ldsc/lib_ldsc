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

#include <AnaliseImagem/Caracterizacao/CObjetoImagem.h>

// Grava as informações do objeto no arquivo recebido como parâmetro (formato Rede de Percolação de Sítios e Ligações).
void CObjetoImagem::GravarObjeto(ofstream &_fout) {
	// X    Y    Z    Raio Tipo N.Voxeis N.ObjsCon LstObjsCons
	_fout << std::left << std::setw(5) << pontoCentral.x;
	_fout << std::left << std::setw(5) << pontoCentral.y;
	_fout << std::left << std::setw(5) << pontoCentral.z;
	_fout << std::left << std::setw(5) << Raio();
	_fout << std::left << std::setw(5) << Tipo();
	_fout << std::left << std::setw(9) << numObjs;
	_fout << std::left << std::setw(9) << sConexao.size();
	std::set<int>::iterator itr;
	for (itr=sConexao.begin(); itr!=sConexao.end(); ++itr) {
		_fout << " " << *itr;
	}
	_fout << endl;
}

// Grava as informações do objeto no arquivo recebido como parâmetro (formato Grafo de Conexão Serial).
void CObjetoImagem::GravarObjetoGrafo(ofstream &_fout, const int &seq) {
	// fronteira sequencial propriedade?  Camada N.ObjsCon LstObjsCons LstPropriedade?
	_fout << std::right << std::setw(4) << camada; //camada superior=1, intermediária=0, inferior=2
	_fout << std::right << std::setw(6) << seq;
	_fout << std::right << std::setw(11) << "?"; //condutância?
	_fout << std::right << std::setw(11) << pontoCentral.y; //camada de 0 a n
	_fout << std::right << std::setw(5) << sConexao.size();
	if ( sConexao.size() > 0 ) {
		std::set<int>::iterator itr;
		for (itr=sConexao.begin(); itr!=sConexao.end(); ++itr) {
			_fout << std::right << std::setw(6) << *itr;
		}
		/*
		std::vector<double>::iterator itrp;
		for (itr=sPropriedade.begin(); itr!=sPropriedade.end(); ++itrp) {
			_fout << " " << *itrp;
		}
		*/
	}
	_fout << endl;

}

// Retorna tipo de objeto como uma string
std::string CObjetoImagem::StrTipo() {
	switch ( tipo ) {
		case SITIO:
			return "Sítio";
		case LIGACAO:
			return "Ligação";
		case SOLIDO:
			return "Sólido";
		case PORO:
			return "Poro";
		case RAMO_MORTO:
			return "Ramo_Morto";
		default:
			return "Não_Identificado";
	}
}

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
