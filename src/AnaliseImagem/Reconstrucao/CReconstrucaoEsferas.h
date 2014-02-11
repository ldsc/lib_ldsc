#ifndef CReconstrucaoEsferas_h
#define CReconstrucaoEsferas_h

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ios>

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
    /// Porosidade calculada na imagem
    double solid;
    /// Dimensão da imagem gerada
    int dimens;
    /// Fator de sobreposição 0 <. fat_sob < 1
    double fat_sob;
    /// Fator de Borda 0 <. fat_borda < 1
    double fat_borda;
    /// Fator de amplificação utilizado na captura da imagem
    int fat_amp;
    /// Tamanho do pixel configurado na captura da imagem
    double tam_pixel;
	/// Nome do arquivo que terá a imagem reconstruída
	string fileName;

protected:
    // Metodos
    int VerBorda ( unsigned char* inimeio, int raio, int ptx, int pty, int ptz );
    int Esfera ( unsigned char* inimeio, int r, int r_aux, int ptx, int pty, int ptz, int* vol_ger );
    void Grava_dat ( string nomeimg, unsigned char* inimeio );
    void Grava_vtk ( string nomeimg, unsigned char* inimeio );
    double Calc_phi ( unsigned char* inimeio );

public:
    /// Construtor
    CReconstrucaoEsferas ( double _solid, int _dimens = 100, double _fat_sob = 0.5, double _fat_borda = 0.9, int _fat_amp=1, double _tam_pixel=5.0 );

    /// Destrutor
    ~ CReconstrucaoEsferas () { }

    /// Executa a reconstrução
    bool Go ( string nome_arq );

	/// Retorna o nome do arquivo
	inline string getFileName () { return fileName; }

	bool Grava_dbm ( string nomeimg, unsigned char* inimeio );

};
#endif
