#ifndef CReconstrucaoEsferas_h
#define CReconstrucaoEsferas_h

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Classe base para reconstrucao de imagens 3D pelo método das Esferas Sobrepostas.
 * Constroi um meio 3d a partir da distribuição de sólidos.
*/
class  CReconstrucaoEsferas {
	// Atributos
	int i;
	int flag;
	int r;
	int d;
	int ptx;
	int pty;
	int ptz;
	int d_cor;
	int r_aux;
	double porosidade;
	double solido;
	double perhist;
	double dif;

	double soma;
	/// Dimensão da imagem gerada
	int dimens;
	/// Fator de sobreposição 0 <. fat_sob < 1
	double fat_sob;
	/// Fator de Borda 0 <. fat_borda < 1
	double fat_borda;


	protected:
	// Metodos
	int VerBorda ( unsigned char*, int, int, int, int, int );
	int Esfera ( unsigned char*, int, int, int, int, int, int, double, int* );
	void Grava_dat ( string, unsigned char*, double, double, int );
	void Grava_vtk ( string, unsigned char*, int );
	double Calc_phi ( unsigned char*, int );

	public:
	/// Construtor
	CReconstrucaoEsferas ( int _dimens = 100, double _fat_sob = 0.5, double _fat_borda = 0.9 );

	/// Destrutor
	~ CReconstrucaoEsferas () { }

	/// Executa a reconstrução
	void Go ( string nome_arq );

};
#endif
