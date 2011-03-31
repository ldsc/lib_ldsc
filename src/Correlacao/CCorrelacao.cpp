
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

#include <Correlacao/CCorrelacao.h>

// -----------------------------------------------------------------------
// Construtor:
// -----------------------------------------------------------------------
CCorrelacao::CCorrelacao (  int dimensao_vetor_correlacao ):
deslocamentoMaximo( dimensao_vetor_correlacao ), correlacao(NULL) 
{ 
	correlacao = new float [deslocamentoMaximo]; 
	
	if (correlacao == NULL)
		{
		cerr << " Erro alocacao memoria vetor correlacao.\n";	
		exit(0);
		}
	
	for (int i = 0; i < deslocamentoMaximo; i++)  	// zera, usar menset
		correlacao[i] = 0.0;
}

// -----------------------------------------------------------------------
// Destrutor:
// -----------------------------------------------------------------------
CCorrelacao::~CCorrelacao () 
{
	if (correlacao) 	
		delete [] correlacao;

	// Não deleta pm2d pois irá apontar para um objeto criado fora daqui
 	
	// Não deleta Re_data pois pode apontar para um objeto criado fora daqui 
}

// -----------------------------------------------------------------------
// Salva vetor correlacao em disco.
// -----------------------------------------------------------------------
bool CCorrelacao::Write (string fileName) 
{
	if ( ! correlacao ) 
		return false;
	
	ofstream fcor ((fileName + ".cor").c_str());
	
	if ( ! fcor.is_open() )
		return false;

	// Escreve o vetor correlacao em disco
	for (int deslocamento = 0; deslocamento < size(); deslocamento++)
		fcor << deslocamento << "\t" <<  correlacao[deslocamento]  << "\n";

	fcor.close (); 						// Fecha o arquivo de disco
	return true;
}

