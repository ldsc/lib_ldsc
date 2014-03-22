#include <AnaliseImagem/Caracterizacao/Correlacao/CCorrelacao3D.h>
#include <AnaliseImagem/Caracterizacao/Correlacao/CCorrelacaoEspacial.h>
#include <AnaliseImagem/Caracterizacao/Correlacao/CCorrelacaoFFT.h>
#include <MetNum/Matriz/TCMatriz3D.h>
#include <fstream>

// -----------------------------------------------------------------------
// Destrutor:
// -----------------------------------------------------------------------
CCorrelacao3D::~CCorrelacao3D () {
   if (correlacao)
      delete [] correlacao;
}

// -----------------------------------------------------------------------
// Determina autocorrelação. Recebe ponteiro para CMatriz3D, o tipo de correlação a ser
// calculada e o número de planos que serão utilizados para o cálculo da média das correlações.
// Se o número de planos informado for zero ou negativo, o defalt será 10% dos planos.
// -----------------------------------------------------------------------
bool CCorrelacao3D::Go (TCMatriz3D<bool> *img, Tipos tipo, int indice, int numPlanos) {
   CCorrelacao * cor = nullptr;
   int nx = img->NX();
   size = nx/2;
   if (correlacao != nullptr){ // se o vetor correlação já estiver alocado, desaloca.
      delete [] correlacao;
      correlacao = nullptr;
   }
   correlacao = new float[size];
   if ( ! correlacao ) {
      cerr << "Erro ao executar CCorrelacao3D::Go. Vetor correlacao não foi criado!";
      return false;
   }
   for (int i=0; i<size; i++){
      correlacao[i] = 0.0;
   }

   if (numPlanos < 1) {
      numPlanos = (nx/10); // => 10%
   } else if (numPlanos > nx) {
      numPlanos = nx;
   }

   switch (tipo){
   case Espacial:
      cor = new CCorrelacaoEspacial( size );
      break;
   case Frequencial:
      cor = new CCorrelacaoFFT( size );
      break;
   }
   for ( int cont = 0; cont < nx; cont += int(nx/numPlanos) ){
      cor->Go( img->LePlano(cont), indice );
      for (int i=0; i<size; i++){
         correlacao[i] += cor->correlacao[i];
      }
   }
   for (int i=0; i<size; i++){
      correlacao[i] = correlacao[i]/numPlanos;
   }
   return true;
}

// -----------------------------------------------------------------------
// Salva vetor correlacao em disco.
// -----------------------------------------------------------------------
bool CCorrelacao3D::Write (string nomeArquivo) {
   if ( ! correlacao )
      return false;

   ofstream fcor ((path + nomeArquivo + ".cor").c_str());

   if ( ! fcor.is_open() )
      return false;

   // Escreve o vetor correlacao em disco
   for (int deslocamento = 0; deslocamento < size; deslocamento++)
      fcor << deslocamento << "\t" <<  correlacao[deslocamento]  << "\n";

   fcor.close (); 						// Fecha o arquivo de disco
   return true;
}

