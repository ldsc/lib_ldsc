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
	//std::set<int>::iterator itr;
	std::map<int,double>::iterator itr;
	for (itr=sConexao.begin(); itr!=sConexao.end(); ++itr) {
		//_fout << " " << *itr;
		_fout << " " << itr->first;
	}
	_fout << endl;
}

// Grava as informações do objeto no arquivo recebido como parâmetro (formato Grafo de Conexão Serial).
void CObjetoImagem::GravarObjetoGrafo(ofstream &_fout, const int &seq) {
	// fronteira sequencial propriedade?  Camada N.ObjsCon LstObjsCons LstPropriedade?
	_fout << std::right << std::setw(4) << contorno; //CENTER = 0, WEST=1, EST=2
	_fout << std::right << std::setw(6) << seq;
	_fout << std::right << std::setw(11) << propriedade; //condutância
	_fout << std::right << std::setw(11) << pontoCentral.x; //camada de 0 a n
	_fout << std::right << std::setw(5) << sConexao.size();
	if ( sConexao.size() > 0 ) {
		//std::set<int>::iterator itr;
		std::map<int,double>::iterator itr;
		for (itr=sConexao.begin(); itr!=sConexao.end(); ++itr) {
			//_fout << std::right << std::setw(6) << *itr;
			_fout << std::right << std::setw(6) << itr->first;
		}
		for (itr=sConexao.begin(); itr!=sConexao.end(); ++itr) {
			_fout << " " << itr->second;
		}

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

// Calcula o fluxo associado ao objeto.
long double CObjetoImagem::Fluxo (std::map<int, CObjetoImagem> * moi) {
	long double fluxo = 0.0;
	int i;
	for ( auto & c : sConexao ) {
		i = c.first;
//		fluxo += propriedade * ( this->x - moi->at(i).x);
		fluxo += c.second * ( this->x - moi->at(i).x);
	}
	/*switch (tipo) {
		case SITIO: // SITIO
			// Fluxo = média da condutância do sítio com a ligação vezes, a pressao no sítio menos a pressao na ligação
			static long double condutanciaMedia;
			for ( auto & c : sConexao ) {
				condutanciaMedia = ( propriedade + (moi[c->first])->second.Propriedade() ) / 2.0;
				fluxo += condutanciaMedia * (this->x - (moi[c->first])->second.x);
			}
			break;
		case LIGACAO: // LIGACAO
			// Fluxo = Condutancia da ligação vezes, a pressao deste sítio menos a pressao do sítio conexo.
			for ( auto & c : sConexao ) {
				fluxo += propriedade * (this->x - (moi[c->first])->second.x);
			}
			break;
	}*/
	return fluxo;
}
