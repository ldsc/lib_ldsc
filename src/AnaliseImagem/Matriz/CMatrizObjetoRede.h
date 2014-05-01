#ifndef CMatrizObjetoRede_H
#define CMatrizObjetoRede_H

#include <AnaliseImagem/Caracterizacao/RedePercolacao/CObjetoRedeDePercolacao.h>
#include <map>

class CMatrizObjetoRede
{		//atributos
	public:
		std::map<int, CObjetoRedeDePercolacao> matrizObjetos;

		// Construtores / Destrutor
	public:
		CMatrizObjetoRede();

		~CMatrizObjetoRede();

		// Métodos
	public:
		/// Grava em disco, com o nome informado, os objetos identificados.
		bool SalvarListaObjetos(std::string fileName, int nx, int ny, int nz);

		/// Grava em disco, no formato do Grafo, com o nome informado, os objetos identificados.
		bool SalvarListaObjetosGrafo(std::string fileName);

		// Salva o grafo e seus objetos em disco.
		//virtual void Write ( std::ostream& out ) const override;
};

#endif // CMatrizObjetoRede_H
