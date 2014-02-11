#include <AnaliseImagem/Caracterizacao/Distribuicao/CBaseDistribuicao.h>
#include <iostream>

// -----------------------------------------------------------------------
// Cálcula a média dos valores de correlação dos arquivos passados.
// -----------------------------------------------------------------------
bool CBaseDistribuicao::Media (set<string> filesName, Tipos _tipo){
	tipo = _tipo;
	areaObjetos = 0.0;
	distribuicao.empty();
	ifstream fdist;
	set<string>::iterator it;
	string aux;
	int cont;
	int pos;
	double dist = 0.0;
	double objt;
	int nobj = 0; // número de áreas (objetos) acumuladas
	for (it = filesName.begin(); it != filesName.end(); ++it) {
		cont = 0;
		fdist.open ((*it).c_str());
		if ( fdist.is_open() && fdist.good() ) { // abriu o arquivo corretamente
			while ( (! fdist.fail()) && ( ! fdist.eof()) ) { //percorre o arquivo até chegar o fim do arquivo ou atingir o tamanho máximo do vetor correlação
				pos = fdist.tellg(); // pega a posição atual de leitura
				//cerr << "pos: " << pos << endl;
				fdist >> aux;
				if (aux.at(0) == '#') {
					fdist.seekg(pos); //reposiciona a leitura
					getline(fdist, aux, '\n'); // le toda a linha
					//cerr << "aux: " << aux << endl;
					if ( aux.substr(0,9) == "# Solids:") { // verifica se é a linha que contém o valor da porosidade
						fdist.seekg(pos+10); // reposiciona o ponteiro de leitura para pegar a porosidade
						fdist >> objt; //pega o valor da porosidade
						areaObjetos += objt; // acumula a porosidade
						nobj++; // número de porosidades acumuladas
						//cerr << "phit: " << phit << " | nphi: " << nphi << endl;
					}
				} else {
					fdist >> dist;
					//cerr << dist << endl;
					if ( cont < distribuicao.size() ) {
						distribuicao[cont] += dist; //acumula os valores de distribuição para cada arquivo.
					} else {
						distribuicao.push_back(dist);
					}
					cont++;
				}
			}
			fdist.close();
		}  else {
			cerr << "Erro ao tentar abrir arquivo em CDistribuicao::Media!" << endl;
			return false;
		}
	}
	int divisor = filesName.size();
	for (int i = 0; i < distribuicao.size(); i++) {
		distribuicao[i] /= divisor; //divide a soma das distribuições pelo número de arquivos.
	}
	areaObjetos /= nobj; //divide a soma das áreas dos objetos pelo número de arquivos que possuiam valor de porosidade. Todos devem ter, mas...
	return true;
}

// -----------------------------------------------------------------------
// Salva em disco o vetor distribuicao de acordo com o tipo de distribuicao calculada.
// -----------------------------------------------------------------------
bool CBaseDistribuicao::Write (string fileName) {
	if ( distribuicao.empty() ) {
		cerr << "Erro em CDistribuicao::Write()! O vetor distribuição não possui elementos!" << endl;
		return false;
	}

	switch (tipo){
		case dts: fileName += ".dts"; break;
		case dtp: fileName += ".dtp"; break;
		case dtg: fileName += ".dtg"; break;
		default:	fileName += ".dtp";
	}

	ofstream fdist ( fileName.c_str() );

	if ( ! fdist.is_open() || ! fdist.good() ) {
		cerr << "Erro em CDistribuicao::Write()! O arquivo de saída não pôde ser aberto!" << endl;
		return false;
	}

	switch (tipo){
		case dts: WriteDTS (fdist); break;
		case dtp: WriteDTP (fdist); break;
		case dtg: WriteDTG (fdist); break;
		default:	WriteDTP (fdist);
	}

	fdist.close (); 						// Fecha o arquivo de disco
	return true;
}

// -----------------------------------------------------------------------
// Salva em disco o vetor distribuicao no formato dtp.
// -----------------------------------------------------------------------
void CBaseDistribuicao::WriteDTP (ofstream & fout) {
	fout << "# Distribution of Pores Size\n";
	fout << "# Size: " << distribuicao.size() << "\n";
	fout << "# Porosity: " << areaObjetos << "\n";
	fout << "# Radius\tValue\n";
	// Escreve o vetor distribuicao em disco
	for (int i = 0; i < distribuicao.size(); i++)
		fout << i+1 << "\t" <<  distribuicao[i]  << "\n";
}

// -----------------------------------------------------------------------
// Salva em disco o vetor distribuicao no formato dts.
// -----------------------------------------------------------------------
void CBaseDistribuicao::WriteDTS (ofstream & fout){
	/*----------------------------------------------
	 ATENÇÃO: Mudanças no cabeçalho do arquivo irão
	 afetar a classe CReconstrucaoEsferas e o LVP!
	 -----------------------------------------------*/
	fout << "# Distribution of Solids Size\n";
	fout << "# Size: " << distribuicao.size() << "\n";
	fout << "# Solids: " << areaObjetos << "\n";
	fout << "# Radius\tValue\n";
	// Escreve o vetor distribuicao em disco
	for (int i = 0; i < distribuicao.size(); i++)
		fout << i+1 << "\t" <<  distribuicao[i]  << "\n";
}

// -----------------------------------------------------------------------
// Salva em disco o vetor distribuicao no formato dtg.
// -----------------------------------------------------------------------
void CBaseDistribuicao::WriteDTG (ofstream & fout){
	fout << "# Distribution of Throats Size\n";
	fout << "# Size: " << distribuicao.size() << "\n";
	fout << "# Porosity: " << areaObjetos << "\n";
	fout << "# Radius\tValue\n";
	// Escreve o vetor distribuicao em disco
	for (int i = 0; i < distribuicao.size(); i++)
		fout << i+1 << "\t" <<  distribuicao[i]  << "\n";
}
