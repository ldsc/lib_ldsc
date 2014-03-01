#ifndef CCorrelacao3D_h
#define CCorrelacao3D_h

#include <string>

using namespace std;

#include "AnaliseImagem/Matriz/TCMatriz3D.h"

/**
 * @brief Classe utilizada para determinação da curva de auto-correlação de uma imagem 3D.
 * Faz uma média das correlações de um numero x de planos da imagem 3D.
*/
class CCorrelacao3D
{
protected:
   /// Deslocamento máximo da imagem usado para calculo correlacao
   int size;

public:
    /// Enumera os tipos de calculo de correlação
    enum Tipos {
       Espacial,
       Frequencial
    };

    /// Vetor correlação
    float *correlacao;

    /// Caminho onde o arquivo correlação será salvo.
    string path;

    //Métodos
public:
    /// Construtor
    CCorrelacao3D ( ) : correlacao(nullptr), path("./") {
    }

    /// Destrutor
    ~ CCorrelacao3D ();

    /// Determina autocorrelação. Recebe ponteiro para CMatriz3D, o tipo de correlação a ser calculada, o valor de índice a ser considerado e o número de planos que serão utilizados para o calculo da média das correlações.
    /// Se o número de planos informado for menor que 1, o defalt será 10% dos planos.
		bool Go (TCMatriz3D<bool> *img, Tipos tipo, int indice=1, int numPlanos=0);

    /// Salva em disco os dados de correlação (.cor).
    bool Write (string nomeArquivo);
};

#endif


