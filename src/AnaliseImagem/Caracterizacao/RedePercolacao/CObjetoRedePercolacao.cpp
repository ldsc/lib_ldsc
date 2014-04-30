/*
=========================================================================
PROJETO:    Biblioteca libldsc CObjetoRedePercolacao
=========================================================================
Desenvolvido por: LDSC - Laboratorio de Desenvolvimento de Software Científico
@author     Leandro Puerari
@email      <puerari@gmail.com>
@begin      2014
@file 	    CObjetoRedePercolacao.cpp
*/

#include <AnaliseImagem/Caracterizacao/RedePercolacao/CObjetoRedePercolacao.h>
#include <AnaliseImagem/Matriz/CMatrizObjetoRede.h>

// Grava as informações do objeto no arquivo recebido como parâmetro (formato Grafo de Conexão Serial).
void CObjetoRedePercolacao::GravarObjetoGrafo(ofstream &_fout, const int &seq) {
	// fronteira sequencial propriedade?  Camada N.ObjsCon LstObjsCons LstPropriedade?
	//_fout << std::right << std::setw(4) << Contorno(); //CENTER = 0, WEST=1, EST=2
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

std::ostream & operator<< (std::ostream & os, const CObjetoRedePercolacao & obj){
	// X    Y    Z    Raio Tipo N.Voxeis Condutância N.ObjsCon LstObjsCon LstCondObjsCon
	os << std::left << std::setw(5) << obj.pontoCentral.x;
	os << std::left << std::setw(5) << obj.pontoCentral.y;
	os << std::left << std::setw(5) << obj.pontoCentral.z;
	{//não aceitou chamar obj.Raio(), tive que calcular!
		int dft =  obj.pontoCentral.df;
		while ( (dft % 3) != 0 )
			++dft;
		int raio = (dft/3);
		os << std::left << std::setw(5) << raio;
	}
	os << std::left << std::setw(5)		<< obj.tipo;
	os << std::left << std::setw(9)		<< obj.numObjs;
	os << std::left << std::setw(12)	<< obj.propriedade;
	os << std::left << std::setw(9)		<< obj.sConexao.size();
	if ( obj.sConexao.size() > 0 ) {
		std::map<int,double>::const_iterator itr;
		for (itr=obj.sConexao.begin(); itr!=obj.sConexao.end(); ++itr) {
			os << std::left << std::setw(6) << itr->first;
		}
		for (itr=obj.sConexao.begin(); itr!=obj.sConexao.end(); ++itr) {
			os << " " << itr->second;
		}
	}
	os << endl;
	return os;
}

// Calcula o fluxo associado ao objeto.
long double CObjetoRedePercolacao::Fluxo (std::map<int, CObjetoRedePercolacao> * moi) {
	long double fluxo = 0.0;
	int i;
	for ( auto & c : sConexao ) {
		i = c.first;
		//		fluxo += propriedade * ( this->x - moi->at(i).x);
		fluxo += c.second * ( this->x - moi->at(i).x);
	}
	return fluxo;
}


// Função:  Go
/* A função Go calcula o novo valor de x, considerando o relacionamento com os demais objetos a quem esta conectado.
 * Observe que calcula o novo  valor de this->x (a pressão), mas não altera o valor de this->x.
 * O novo valor de x, retornado por Go() pode ser usado externamente.
 * Por exemplo, um solver externo vai usar esta nova previsão de x para definir no momento adequado o novo valor de this->x.
*/
long double CObjetoRedePercolacao::Go (long double ) {
	// Cria variáveis auxiliares (uma única vez, pois são estáticas)
	static long double somatorio_da_condutancia;
	static long double somatorio_da_condutancia_vezes_x;

	if (contorno == CContorno::ETipoContorno::CENTER) {
		// zera o somatorio (a cada passagem por Go)
		somatorio_da_condutancia_vezes_x = somatorio_da_condutancia = 0.0;
		for (auto &c : sConexao) { // Percorre as conecções
			// condutância entre this e o objeto conectado vezes x(pressão) do objeto_conectado
			somatorio_da_condutancia_vezes_x += c.second * ptrMatObjsRede->matrizObjetos[c.first].x;
			// Acumula a condutancia total
			somatorio_da_condutancia += c.second;
		}
		// Somatorio (condutancia*pressao) / somatorio_da_condutancia
		return somatorio_da_condutancia_vezes_x / somatorio_da_condutancia;
	} else { // CContorno::ETipoContorno::EST ou CContorno::ETipoContorno::WEST
		return x;
	}
}
