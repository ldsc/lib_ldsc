/*
=========================================================================
PROJETO:    Biblioteca libldsc CObjetoRede
=========================================================================
Desenvolvido por: LDSC - Laboratorio de Desenvolvimento de Software Científico
@author     Leandro Puerari
@email      <puerari@gmail.com>
@begin      2014
@file 	    CObjetoRede.h
*/

#include <AnaliseImagem/Caracterizacao/RedePercolacao/CObjetoRede.h>

// Grava as informações do objeto no arquivo recebido como parâmetro (formato Grafo de Conexão Serial).
void CObjetoRede::GravarObjetoGrafo(ofstream &_fout, const int &seq) {
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

// Calcula o fluxo associado ao objeto.
long double CObjetoRede::Fluxo (std::map<int, CObjetoRede> * moi) {
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
long double CObjetoRede::Go (long double ) {
	// Cria variáveis auxiliares (uma única vez, pois são estáticas)
	static long double somatorio_da_condutancia;
	static long double somatorio_da_condutancia_vezes_x;

	// zera o somatorio (a cada passagem por Go)
	somatorio_da_condutancia_vezes_x = somatorio_da_condutancia = 0;

	// Se não tem nenhum link, retorna x atual (a pressão atual)
	if (sConexao.size() == 0)
		return x;

	// Percorre as conecções
	for (auto &c : sConexao) {

		//somatorio_da_condutancia_vezes_x += c.second * this->coneccao[i]->x;

		// Acumula a condutancia total
		somatorio_da_condutancia += c.second;
	}

	// Somatorio (condutancia*pressao) / somatorio_da_condutancia
	// retorna um novo valor de x (pressão) sem alterar x diretamente.
	return somatorio_da_condutancia_vezes_x / somatorio_da_condutancia;
}