#include "CMatrizObjetoImagem.h"

// Construtor
CMatrizObjetoImagem::CMatrizObjetoImagem()
{
	matrizObjetos.clear();
}

// Destrutor
CMatrizObjetoImagem::~CMatrizObjetoImagem()
{
	matrizObjetos.clear();
}

// Grava em disco, com o nome informado, os objetos identificados.
bool CMatrizObjetoImagem::SalvarListaObjetos(std::string fileName, int nx, int ny, int nz){
	ofstream fout; //  Abre arquivo disco
	fout.open(fileName.c_str());
	if (fout.good()){
		fout << "# " << matrizObjetos.size() << " " << nx << " " << ny << " " << nz << endl;
		fout << "Obj.  X    Y    Z    Raio Tipo N.Voxeis N.ObjsCon LstObjsCons" << endl;
		for (std::map<int,CObjetoImagem>::iterator it = matrizObjetos.begin(); it != matrizObjetos.end(); ++it) {
			fout << std::left << std::setw(6) << it->first;
			it->second.GravarObjeto(fout);
		}
		fout.close();
		return true;
	}
	return false;
}

// Grava em disco, no formato do Grafo, com o nome informado, os objetos identificados.
bool CMatrizObjetoImagem::SalvarListaObjetosGrafo(std::string fileName){
	ofstream fout; //  Abre arquivo disco
	fout.open(fileName.c_str());
	if (fout.good()){
		fout << matrizObjetos.size() << endl;
		for (std::map<int,CObjetoImagem>::iterator it = matrizObjetos.begin(); it != matrizObjetos.end(); ++it) {
			it->second.GravarObjetoGrafo(fout, it->first-1);
		}
		fout.close();
		return true;
	}
	return false;
}
