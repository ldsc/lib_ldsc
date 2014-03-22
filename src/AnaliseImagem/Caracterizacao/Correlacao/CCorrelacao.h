#ifndef CCorrelacao_h
#define CCorrelacao_h

#include <string>
#include <set>
#include "MetNum/Matriz/TCMatriz2D.h"

// ---------------------------------------------------------------------
//               DEFINES
// ---------------------------------------------------------------------
// Observe que nesta classe e nas classes derivadas os nomes
// Re_data, Im_data e NX precisam ser preservados.
#define MatrizReal(x,y)	Re_data [(y)*NX + (x)]
#define MatrizImag(x,y)	Im_data [(y)*NX + (x)]

using namespace std;

/**
 * @brief Classe base para a determinação da curva de auto-correlação de imagens 2D.
*/
class CCorrelacao
{
protected:
   //Atributos
   /// Dimensão nx da imagem 2D
   // int NX;

   /// Dimensão ny da imagem 2D
   // int NY;

   /// Menor dimensao da imagem 2D
   // int dimensaoMinima;

   /// Matriz 2D da parte real
   // float * Re_data;

   /// Matriz 2D da parte imaginária
   // float * Im_data;

   /// Ponteiro para Matriz 2D
   // TCMatriz2D< int > *pm2d;

   /// Deslocamento máximo da imagem usado para cálculo correlacao
   int size;

public:
   /// Vetor correlação
   float *correlacao;

   /// Caminho onde o arquivo correlação será salvo.
   string path;

   /// Valor que representa o índice na imagem.
   //int INDICE;

   /// Valor que representa o fundo na imagem.
   //int FUNDO;

   //Métodos
public:
   /// Construtor, recebe a dimensao_vetor_correlacao ( default = 320 = 640/2 )
   CCorrelacao ( int tamVetCorrelaco = 320 );

   /// Destrutor
   virtual ~ CCorrelacao ();

   /// Determina a função autocorrelação. Recebe ponteiro para vetor imagem, as dimensões da imagem e o valore que representa índice na imagem, ou seja, o valor que será correlacionado.
   virtual bool Go (float *Re_data, int NX, int NY, int indice=1) = 0;

   /// Determina a função autocorrelação. Recebe ponteiro para TCMatriz2D< int > e o valore que representa índice na imagem, ou seja, o valor que será correlacionado.
	 virtual bool Go (TCMatriz2D< bool > *img, int indice=1) = 0;

   /// Recebe lista de arquivos correlação, cálcula a média das curvas e armazena os valores no vetor correlacao
   bool Media (set<string> filesName);

   /// Salva em disco os dados de correlação (.cor).
   bool Write (string nomeArquivo);

   /// Retorna dimensao do vetor correlacao (igual ao size).
   inline int Size() {
      return size;
   }
protected:
   bool AlocarOuRealocarCorrelacaoSeNecessarioEZerar ( int tam );
};

#endif

