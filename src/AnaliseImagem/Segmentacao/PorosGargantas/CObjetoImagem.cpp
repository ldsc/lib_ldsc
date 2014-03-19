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

#include <AnaliseImagem/Segmentacao/PorosGargantas/CObjetoImagem.h>

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
	//std::set<int>::iterator itr;
	std::map<int,double>::iterator itr;
	for (itr=sConexao.begin(); itr!=sConexao.end(); ++itr) {
		//_fout << " " << *itr;
		_fout << " " << itr->first;
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
