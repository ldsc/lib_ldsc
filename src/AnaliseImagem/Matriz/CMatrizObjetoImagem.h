#ifndef CMatrizObjetoImagem_H
#define CMatrizObjetoImagem_H

#include <AnaliseImagem/Caracterizacao/CObjetoImagem.h>
#include <map>

class CMatrizObjetoImagem
{		//atributos
	public:
		std::map<int, CObjetoImagem> matrizObjetos;

		// Construtores / Destrutor
	public:
		CMatrizObjetoImagem();

		~CMatrizObjetoImagem();

		// Métodos
	public:
		/// Grava em disco, com o nome informado, os objetos identificados.
		bool SalvarListaObjetos(std::string fileName, int nx, int ny, int nz);

		/// Grava em disco, no formato do Grafo, com o nome informado, os objetos identificados.
		bool SalvarListaObjetosGrafo(std::string fileName);
};

#endif // CMatrizObjetoImagem_H
