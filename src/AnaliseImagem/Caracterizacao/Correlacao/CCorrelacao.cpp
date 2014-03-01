
#include <iostream>
#include <fstream>
#include <cstdlib>

#include <AnaliseImagem/Caracterizacao/Correlacao/CCorrelacao.h>

// -----------------------------------------------------------------------
// Construtor:
// -----------------------------------------------------------------------
CCorrelacao::CCorrelacao (  int tamVetCorrelaco )
   : size( tamVetCorrelaco ), correlacao(nullptr), path("./") {
   AlocarOuRealocarCorrelacaoSeNecessarioEZerar( size );
}

// -----------------------------------------------------------------------
// Destrutor:
// -----------------------------------------------------------------------
CCorrelacao::~CCorrelacao () {
   if (correlacao) {
      delete [] correlacao;
   }
}

// -----------------------------------------------------------------------
// Cálcula a média dos valores de correlação dos arquivos passados.
// -----------------------------------------------------------------------
bool CCorrelacao::Media (set<string> filesName) {
   if ( ! AlocarOuRealocarCorrelacaoSeNecessarioEZerar( size ) ) {
      return false;
   }
   ifstream fcor;
   set<string>::iterator it;
   int aux = 0;
   int cont;
   float cor = 0.0;
   for (it = filesName.begin(); it != filesName.end(); ++it) {
      cont = 0;
      fcor.open ((*it).c_str());
      if ( fcor.is_open() && fcor.good() ) { // abriu o arquivo corretamente
         while ( (! fcor.fail()) && (cont < size) ) { //percorre o arquivo até chegar o fim do arquivo ou atingir o tamanho máximo do vetor correlação
            fcor >> aux;
            fcor >> cor;
            correlacao[cont] += cor; //acumula os valores de correlação para cada arquivo.
            cont++;
         }
         if (cont < size) //se o número de valores de correlação for diferente entre os arquivos, a média irá considerar o número mínimo de valores existente nos arquivos.
            size=cont;

         fcor.close();
      } else {
         cerr << "Erro ao tentar abrir arquivo em CCorrelacao::Media!" << endl;
         return false;
      }
   }
   for (int i = 0; i < size; i++) {
      correlacao[i] /= filesName.size(); //divide a soma das correlação pelo número de arquivos.
   }
   return true;
}

// -----------------------------------------------------------------------
// Salva vetor correlacao em disco.
// -----------------------------------------------------------------------
bool CCorrelacao::Write (string nomeArquivo) {
   if ( ! correlacao )
      return false;

   ofstream fcor ((path + nomeArquivo + ".cor").c_str());

   if ( ! fcor.is_open() ) {
      return false;
   }

   // Escreve o vetor correlacao em disco
   for (int deslocamento = 0; deslocamento < size; deslocamento++) {
      fcor << deslocamento << "\t" <<  correlacao[deslocamento]  << "\n";
   }

   fcor.close (); 						// Fecha o arquivo de disco
   return true;
}

// -----------------------------------------------------------------------
// Definicao do intervalo de calculo da correlacao (deslocamento u) de acordo com tam informado (NX/2)
// Futuramente considerar extensao/reflexao da imagem
// Se a dimensao da imagem mudou, muda a dimensao do vetor correlacao e realoca o vetor.
// Se a dimensão não mudou apenas zera o vetor correlacao.
// -----------------------------------------------------------------------
bool CCorrelacao::AlocarOuRealocarCorrelacaoSeNecessarioEZerar ( int tam ) {
   if ( size != tam ) {
      // Destroe correlacao existente para posteriormente alocar nova.
      if ( correlacao ) {
         delete [] correlacao;
         correlacao = nullptr;
      }
   }

   if ( ! correlacao ) { // Se não estiver alocado, aloca...
      size = tam;
      correlacao = new float [size];
   }

   if ( ! correlacao ) {
      cerr << "Erro alocacao de correlacao - bool CCorrelacaoEspacial::AlocarOuRealocarCorrelacaoSeNecessario()." ;
      return false;
   }

   // zera ->usar memset()
   for (int i = 0; i < size; i++) {
      correlacao[i] = 0.0;
   }

   return true;
}
