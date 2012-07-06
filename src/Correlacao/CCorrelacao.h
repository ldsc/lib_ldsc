#ifndef CCorrelacao_h
#define CCorrelacao_h

#include <string>
#include <set>

// ---------------------------------------------------------------------
//               DEFINES
// ---------------------------------------------------------------------
// Observe que nesta classe e nas classes derivadas os nomes
// Re_data, Im_data e NX precisam ser preservados.
#define MatrizReal(x,y)	Re_data [(y)*NX + (x)]
#define MatrizImag(x,y)	Im_data [(y)*NX + (x)]

using namespace std;

class CMatriz2D;

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
   // CMatriz2D *pm2d;

   /// Deslocamento máximo da imagem usado para calculo correlacao
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

   /// Determina a função autocorrelação. Recebe ponteiro para CMatriz2D e o valore que representa índice na imagem, ou seja, o valor que será correlacionado.
   virtual bool Go (CMatriz2D *img, int indice=1) = 0;

   /// Recebe lista de arquivos correlação, cálcula a média das curvas e armazena os valores no vetor correlacao
   bool Media (set<string> filesName);

   /// Salva em disco os dados de correlação (.cor).
   bool Write (string fileName);

   /// Retorna dimensao do vetor correlacao (igual ao size).
   inline int Size() {
      return size;
   }
protected:
   bool AlocarOuRealocarCorrelacaoSeNecessarioEZerar ( int tam );
};

#endif

