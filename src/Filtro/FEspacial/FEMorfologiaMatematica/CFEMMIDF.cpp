// Ítens conferidos: 1[ ] 2[ ] 3[ ] 4[ ] 5[ ] 6[ ] 7[ ] 8[ ] 9[ ] 10[ ]

/*
----------------------------------------------------------------------------
PROJETO:					LIB_LDSC

----------------------------------------------------------------------------

Desenvolvido por:   Laboratorio de Desenvolvimento de Software Cientifico dos Materiais.
Programadores: 	Andre D.Bueno, Celso P.Fernandez, Fabio S.Magnani,  Liang Zirong, Paulo C. Philippi, ...

Copyright @1997:  	Todos os direitos reservados.
Nome deste arquivo: CFEMMIDF.cpp
Nome da classe:     CFEMMIDF
Arquivos de documentacao do projeto em: path\documentacao\*.doc, path\Help
Descricao:	 	Implementa as funções da classe CFEMMIDF.
*/

// ----------------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// ----------------------------------------------------------------------------
using namespace std;

#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDF.h"	// Classe base
#include "Filtro/FEspacial/FEMorfologiaMatematica/CFEMMIDFd34.h" // Utilizado pelo método CorrigeAbertura
#include "Geometria/Bola/BCDiscreta/CBCDiscreta.h"			// Cria objeto
//  Mascara\MCDiscreta\CBCDiscreta.h"   					// Cria objeto

/*
==================================================================================
Atributos estaticos da classe
==================================================================================
*/
// Se verdadeira após cada processamento recalcula a idf
bool CFEMMIDF::atualizaIDF = 0;	// por default, não recalcula a idf apos dilatacao ou fechamento

/*
==================================================================================
Documentacao construtor
==================================================================================
Executa o construtor da classe base, o TCMatriz2D< int >(NX,NY) que aloca a matriz2D.
Define o valor do raioBola como sendo metade do tamanhoMascara, e o raioMaximo.
*/

// Construtor sobrecarregado, recebe também o tamanho do raio máximo
CFEMMIDF::CFEMMIDF ( TCMatriz2D< int > * &matriz, unsigned int _tamanhoMascara, unsigned int _raioMax, int _indice, int _fundo )
   : CFEMorfologiaMatematica ( matriz, _tamanhoMascara, _indice, _fundo ), TCMatriz2D< int > ( matriz->NX (), matriz->NY () ),
     raioBola ( _tamanhoMascara ), raioMaximo ( _raioMax ), indiceAtivo ( _indice ), indiceInativo ( _fundo ) {
}

/*
==================================================================================
Função    ExecutadaPorGo
==================================================================================
Função usada exclusivamente por Go:

1-Quando o usuário quer calcular a idf, chama a funcao Go (pm).
 No caso da imagem pm passada ter dimemsões diferentes da pm passada para o construtor, devo
 desalocar a matris idf, definir os novos valores de nx e ny e realocar a idf.
 Se as dimensões da matriz passada e minhas forem iguais posso continuar.

2-Copia valores da matriz passada para a matriz idf, usando data2D[i][j]=pm->data2D[i][j]

 Esta função foi incluída porque o usuário programador pode chamar Go(pm)
 sendo pm diferente daquela passada par o construtor. Ou seja cria-se uma segurança a mais.

 Outro fato que justifica esta função é que antes, criava objeto idf, executava Go, e depois
 podia executar a erosao, mas sempre sobre a idf da imagem passada para o construtor
 da classe. Agora posso passar uma nova matriz para a função Go,
 diferente da passada para o construtor.

 O terceiro motivo é que no caso de se realizar uma sequencia de erosoes com a mesma
 mascara, eu precisava criar um objeto idf, calcular go e calcular a erosao,
 criar nova idf (sobre pm erodida), executar go e então realizar a erosao...
 Agora posso recalcular Go a qualquer momento, sem recriar o objeto idf.
 Exemplo:
   cria idf      		pfmm=new CFEMMIDF(pm,tamanhoMascara);
   executa go    		pfmm->Go(pm,tamanhoMascara);
   executa erosao      pfmm->Erosao(pm,raioBola);       	// erosao sobre imagem original
   executa go    		pfmm->Go(pm,tamanhoMascara);
   executa erosao      pfmm->Erosao(pm,raioBola);		// erosao sobre imagem já erodida

    Assim, toda funcao Go herdeira da CFEMMIDF deve chamar esta funcao:
 ExecutadaPorGo(matriz);

*/
// Funcao chamada exclusivamente por Go, ou seja depois de executada a idf vai ser recalculada.
void CFEMMIDF::ExecutadaPorGo ( TCMatriz2D< int > * &matriz ) {	// ,unsigned int _tamanhoMascara)
   pm = matriz;										// armazena endereço matriz
   if ( this->nx != matriz->NX() || this->ny != matriz->NY() ) { // verifica se a matriz tem as mesmas dimensoes da idf(this)
      Desaloca();				// desaloca a matriz existende e depois
      this->nx = matriz->NX();	// define novo tamanho
      this->ny = matriz->NY();
      Aloca();					// aloca de acordo com novo tamanho
   }
   // Agora tenho de armazenar valores de pm na idf
   // substituir por this=matriz;
   register int mi = Mi();
   for ( int j = 0; j < ny; j++ ) {
      for ( int i = 0; i < nx; i++ ) {
         if ( matriz->data2D[i][j] != 0 ) {	// como a imagem recebida por ser uma outra idf
            this->data2D[i][j] = mi;		// define this com 0 e 1
            // =mi    //////////// // // // // // //
            // AQUI AQUI AQUI AQUI: trocar 1 por mi
            // elimina calculo dos planos de contorno
            // fica mais rapido
            //////////// // // // // // //
         } else {
            this->data2D[i][j] = 0;
         }
      }
   }
   Write("ExecutadaPorGo.pgm");
}

/*
==================================================================================
Função    VerificaImagem
==================================================================================
Usada pelas funções Erosao,Dilatacao, fechamento e abertura para verificar se é a mesma imagem.
Se for outra imagem recalcula Go.
Tambem armazena endereço da imagem em pm.
*/
void CFEMMIDF::VerificaImagem ( TCMatriz2D< int > * &matriz ) { 					// se for a mesma imagem e tiver o mesmo tamanho sai
   if ( pm == matriz && nx == matriz->NX() && ny == matriz->NY() ) {
      return;			// sai
   } else {
      Go ( matriz );		// senão chama Go, que redefine o tamanho da imagem
   }
}					// e calcula valores idf

/*
==================================================================================
Função    Erosao
==================================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
TCMatriz2D< int > * CFEMMIDF::Erosao ( TCMatriz2D< int > * &matriz, unsigned int _RaioBola ) {	//
   // -->Padrao
   VerificaImagem ( matriz );			// verifica se é a mesma imagem (se diferente recalcula Go)
   tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
   // Deve calcular o tamanhoMascara antes de criar a mascara
   CriaMascara ( tamanhoMascara );		// Cria a mascara adequada, do tamanho de tamanhoMascara
   CBCDiscreta *maskd = dynamic_cast < CBCDiscreta * > ( mask );	// Cria ponteiro para mascara com acesso a RaioBolaTangente
   // Processamento da erosao em si
   register int raioBolaInclusa = maskd->RaioBolaInclusa ();
   register int **pmdata2D = pm->Data2D ();	// otimizacao
   for ( int j = 0; j < ny; j++ ) {
      for ( int i = 0; i < nx; i++ ) {
         // se o ponto da idf for maior que a bola tangente, faz ponto=1
         if ( data2D[i][j] > raioBolaInclusa )
            pmdata2D[i][j] = INDICE; //indiceAtivo;	 // seta ponto ativo
         else
            pmdata2D[i][j] = FUNDO; //indiceInativo; // seta ponto inativo
      }
   }
   return pm;	// pm é a matriz erodida
}

/*
==================================================================================
Função    Dilatacao
==================================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
TCMatriz2D< int > * CFEMMIDF::Dilatacao ( TCMatriz2D< int > * &matriz, unsigned int _RaioBola ) {	//
   // -->Padrao
   VerificaImagem ( matriz );	// verifica se é a mesma imagem (se diferente recalcula Go)
   tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
   // Deve calcular o tamanhoMascara antes de criar a mascara
   CriaMascara ( tamanhoMascara );	// Cria a mascara adequada,do tamanho de tamanhoMascara

   CBCDiscreta *maskd = dynamic_cast < CBCDiscreta * > ( mask );	// Cria ponteiro para mascara com acesso a RaioBolaTangente

   // Processamento da Dilatacao em si
   int mi = Mi ();		// funcao definida na CFEMMIDF, redeclarada nas filhas
   register int **pmdata2D = pm->Data2D ();
   for ( int j = 0; j < ny; j++ )	// percorre toda a idf e
      for ( int i = 0; i < nx; i++ )	// pinta pontos na imagem
         if ( data2D[i][j] >= mi )
            pmdata2D[i][j] = INDICE; //indiceAtivo;
         else
            pmdata2D[i][j] = FUNDO; //indiceInativo;

   // Otimizacao Mascara (bola)
   int raio = maskd->RaioX ();
   // int raioBolaInclusa=maskd->RaioBolaInclusa();
   // int raioBolaTangente=maskd->RaioBolaTangente();
   register int **maskdata2D = maskd->Data2D ();
   register int rmx;		// raio mais x, raio+x
   // Variáveis para SIMETRIA Bola
   int posxe, posxd;		// x esquerda e x direita
   int posys, posyn;		// y sul e y norte
   int xx, yy;			// posicoes xx e yy da bola
   for ( int j = raio; j < ny - raio; j++ ) {	// Percorre a imagem idf
      for ( int i = raio; i < nx - raio; i++ ) {	// se for maior ou igual ao raio da bola inclusa,
         // e menor igual a bola tangente pinta bola
         if ( data2D[i][j] == mi ) {	//  && data2D[i][j]<=raioBolaInclusa)
            // PINTA A BOLA: CONSIDERA SIMETRIA
            //////////// //////////// // // // // //
            // PINTA A BOLA OTIMIZADO: CONSIDERA SIMETRIA
            for ( xx = 0; xx <= raio; xx++ ) {	// Usa simetria para pintura da bola
               // só percorre meia bola
               posxe = i - xx;	// por ser simétrica
               posxd = i + xx;
               rmx = raio + xx;
               for ( yy = 0; yy <= raio; yy++ ) {
                  posys = j - yy;
                  posyn = j + yy;
                  if ( maskdata2D[rmx][raio + yy] != 0 )	{// se o ponto da bola é ativo
                     pmdata2D[posxe][posyn] = pmdata2D[posxe][posys] =
                           pmdata2D[posxd][posyn] = pmdata2D[posxd][posys] = INDICE; //indiceAtivo;
                  }
               }
            }
            /* // NAO OTIMIZADO
            for (int x=-raio; x<=raio; x++) { // percorre eixo x da bola
               imx=i+x; rmx=raio+x; // variaveis otimizacao
               for (int y=-raio; y <=raio;y++) { // percorre eixo y da bola
                  if(maskd->data2D[rmx][raio+y]!=0) { // se o ponto da bola é ativo
                     pm->data2D[imx][j+y]=indiceAtivo; // pinta na imagem
                  }
                }
            } */
         }
      }
   }
   //  DilatacaoNosContornos();                                    // Realiza a dilatacao nos contornos
   // verifica atualização idf
   if ( atualizaIDF ) // verifica o flag de atualizacao da idf após dilatação
      Go ( pm );		 // se ativo recalcula a idf
   return pm;			 // pm é a matriz Dilatacao
}

/*
==================================================================================
Função    Fechamento
==================================================================================
Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
*/
TCMatriz2D< int > * CFEMMIDF::Fechamento ( TCMatriz2D< int > * &matriz, unsigned int _RaioBola ) {
   bool atualizaIDF_old = atualizaIDF;	// armazena valor de atualizaIDF
   atualizaIDF = 1;		// ativa, para que a Dilatacao recalcule  a idf
   Dilatacao ( matriz, _RaioBola );	// processa a dilatação, e depois Go
   atualizaIDF = atualizaIDF_old;	// atualizaIDF volta ao estado anterior
   Erosao ( matriz, _RaioBola );	// Processa a erosão, considerando imagem idf atualizada
   return pm;
}

/*
 * ==================================================================================
 * Função    Abertura
 * ==================================================================================
 * Obs: A sequência de execucao desta função não deve ser alterada sem uma analise detalhada
 */
TCMatriz2D< int > * CFEMMIDF::Abertura ( TCMatriz2D< int > * &matriz, unsigned int _RaioBola ) {
   VerificaImagem ( matriz ); // verifica se é a mesma imagem (se diferente recalcula Go)
   tamanhoMascara = 2 * _RaioBola + 1;	// Define o tamanho da mascara
   // Deve calcular o tamanhoMascara antes de criar a mascara
   CriaMascara ( tamanhoMascara );		// Cria a mascara adequada de tamanho = tamanhoMascara

   CBCDiscreta *maskd = dynamic_cast < CBCDiscreta * > ( mask );	// Cria ponteiro para mascara com acesso a RaioBolaTangente

   //maskd->Write ("mascaraCBCDiscreta.pgm");

   // Processamento da abertura em si
   // Obs:Deve zerar a matriz exceto as bordas(resolver)
   pm->Constante ( indiceInativo );	// zera a matriz imagem

   // variáveis auxiliares para otimizacao
   // Otimizacao Mascara
   register int raio = maskd->RaioX ();
   int raioBolaTangente = maskd->RaioBolaTangente ();
   int raioBolaInclusa = maskd->RaioBolaInclusa ();
   register int **maskdata2D = maskd->Data2D ();
   register int rmx;		// raio mais x, raio+x

   // Otimizacao Imagem
   register int **pmdata2D = pm->Data2D ();

   // Variáveis para SIMETRIA Bola   // otimizacao
   unsigned int posxe, posxd;	// x esquerda e x direita
   unsigned int posys, posyn;	// y sul e y norte
   unsigned int xx, yy;		// posicoes xx e yy da bola

   for ( int j = raio; j < ny - raio; j++ ) { // Percorre a imagem idf
      for ( int i = raio; i < nx - raio; i++ ) {
         // se o ponto da idf for maior que a bola tangente, faz ponto=1
         if ( data2D[i][j] > raioBolaTangente ) {	// p/d34 >3(raio+1)
            pmdata2D[i][j] = indiceAtivo;	// seta ponto da imagem como pertencente a bola
            // se o ponto for maior que a bola inclusa, percorre a mascara e faz pontos=indiceAtivo
         } else if ( data2D[i][j] > raioBolaInclusa ) {	// p/d34 >3(raio)
            // PINTA A BOLA OTIMIZADO: CONSIDERA SIMETRIA
            for ( xx = 0; xx <= raio; xx++ ) {// Usa simetria para pintura da bola // só percorre meia bola
               posxe = i - xx;	// por ser simétrica
               posxd = i + xx;
               rmx = raio + xx;
               for ( yy = 0; yy <= raio; yy++ ) {
                  posys = j - yy;
                  posyn = j + yy;
                  if ( maskdata2D[rmx][raio + yy] != 0 ) {	// se o ponto da bola é ativo
                     pmdata2D[posxe][posyn] = pmdata2D[posxe][posys] =
                     pmdata2D[posxd][posyn] = pmdata2D[posxd][posys] = indiceAtivo;
                  }
               }
            }
            // PINTA A BOLA NAO OTIMIZADO: NAO CONSIDERA SIMETRIA
            // for (int xx=-raio; xx<=raio; xx++) {  // percorre eixo x da bola
            //   imx=i+xx; rmx=raio+xx;			     // variaveis otimizacao
            //	 for (int yy=-raio; yy <=raio;yy++)   // percorre eixo y da bola
            //     if(maskd->data2D[rmx][raio+yy]!=0)// se o ponto da bola é ativo
            //	     pm->data2D[imx][j+yy]=1; 	     // pinta na imagem
            // }
         }
      }
   }
   //  DilatacaoNosContornos(); // Realiza a dilatacao nos contornos
   return pm;			// pm é a matriz abertura
}

/*================================================================================
 * Função    CorrigeAbertura
 * ==================================================================================
 * Este método corrige o erro físico que ocorre (em configurações de equilíbrio) na rotulagem da imagem após a operação de abertura.*/
void CFEMMIDF::CorrigeAbertura ( TCMatriz2D< int > * &matriz, int &regiao ) {
   char fileName[64];
   // calcula idf d34 da mascara
   CFEMMIDFd34 *idfMask = NULL;
   TCMatriz2D< int > *ptr_mask = static_cast<TCMatriz2D< int >*> ( mask );
   idfMask = new CFEMMIDFd34 ( ptr_mask );
   idfMask->Go ( ptr_mask );
   //grava em disco a IDF da mascara.
   /*
     *    static int contMasK = 1;
     *    sprintf (fileName, "idfMascara%d.pgm", contMasK++);
     *    idfMask->WriteFormat( WRITEFORM_PI_X_Y_GRAY_ASCII );
     *      idfMask->NumCores( idfMask->MaiorValor()+1 );
     *    idfMask->Write(fileName);
     */
   // calcula idf d34 da imagem abertura.
   CFEMMIDFd34 *idfAbertura = NULL;
   idfAbertura = new CFEMMIDFd34 ( matriz );
   idfAbertura->Go ( matriz );
   //grava em disco a IDF da imagem abertura.
   static int contAbertura = 1;
   sprintf ( fileName, "idfAbertura%d.pgm", contAbertura++ );
	 idfAbertura->WriteFormat ( P2_X_Y_GRAY_ASCII );
   idfAbertura->NumCores ( idfAbertura->MaiorValor() +1 );
   idfAbertura->Write ( fileName );

   // Método - 1 O melhor até agora!
   pair<int,int> maiorMenor = idfMask->MaiorMenorValorNzero(); //maiorMenor.first = centro da máscara e maiorMenor.second = bordas da máscara
   int centro = maiorMenor.first;
   int borda  = maiorMenor.second;
   int raiox = mask->RaioX();
   int raioy = mask->RaioY();
   for ( int j = raioy; j < ny - raioy; j++ ) {	// Percorre a imagem idf
      for ( int i = raiox; i < nx - raiox; i++ ) {
         if ( matriz->data2D[i][j] == regiao ) { // se o ponto analizado faz parte da região abertura
            if ( idfAbertura->data2D[i][j] == centro ) {
               if ( ( idfAbertura->data2D[i-raiox][j] == borda )  && ( idfAbertura->data2D[i][j+raioy] < centro ) ) {
                  if ( ( matriz->data2D[i][j+raioy+1] == regiao )
                       && ( matriz->data2D[i-1][j+raioy] != regiao )
                       && ( matriz->data2D[i+raiox+1][j+raioy] != regiao ) ) {
                     for ( int x = 0; x <= raiox; x++ )
                        if ( matriz->data2D[i+x][j+raioy] == regiao )
                           matriz->data2D[i+x][j+raioy] = 0;
                        else
                           break;
                  }
               } else if ( ( idfAbertura->data2D[i+raiox][j] == borda )  && ( idfAbertura->data2D[i][j+raioy] < centro ) ) {
                  if ( ( matriz->data2D[i][j+raioy+1] == regiao )
                       && ( matriz->data2D[i+raiox][j+raioy] != regiao )
                       && ( matriz->data2D[i-raiox-1][j+raioy] != regiao ) ) {
                     for ( int x = -raiox; x <= raiox; x++ )
                        if ( matriz->data2D[i+x][j+raioy-1] != regiao )
                           matriz->data2D[i+x][j+raioy] = 0;
                  }
               }
            } else if ( ( idfAbertura->data2D[i][j] < centro ) && ( idfAbertura->data2D[i][j] > borda ) ) {
               if ( ( idfAbertura->data2D[i-raiox][j] == borda )  && ( idfAbertura->data2D[i][j+raioy] < centro ) ) {
                  int raio = 1;
                  int teste = 4;
                  //cerr << "raio = " << raio << " teste = " << teste << " ponto = " << idfAbertura->data2D[i][j] << endl;
                  while ( idfAbertura->data2D[i][j] > teste ) { //quando sair do loop, ten-se o raio correspondente a bola cujo valor de IDF (d34) no ponto central, seja igual ao valor do ponto analizado
                     raio++;
                     teste += 3;
                     //cerr << "raio = " << raio << " teste = " << teste << " ponto = " << idfAbertura->data2D[i][j] << endl;
                  }
                  if ( ( matriz->data2D[i][j+raio+1] == regiao )
                       && ( matriz->data2D[i-1][j+raio] != regiao )
                       && ( matriz->data2D[i+raio+1][j+raio] != regiao ) ) {
                     for ( int x = 0; x <= raio; x++ )
                        if ( matriz->data2D[i+x][j+raio] == regiao )
                           matriz->data2D[i+x][j+raio] = 0;
                        else
                           break;
                  }
               }
            }
         }
      }
   }
   /*
     *    // Método - 2
     *    pair<int,int> maiorMenor = idfMask->MaiorMenorValorNzero(); //maiorMenor.first = centro da máscara e maiorMenor.second = bordas da máscara
     *    int centro = maiorMenor.first;
     *    int borda  = maiorMenor.second;
     *    cerr << "maior: " << maiorMenor.first << endl;
     *    cerr << "menor: " << maiorMenor.second << endl;
     *    int raiox = mask->RaioX();
     *    int raioy = mask->RaioY();
     *    cerr << "raiox: " << raiox << endl;
     *    cerr << "raioy: " << raioy << endl << endl;
     *    bool achou;
     *    for (int j = raioy; j < ny - raioy; j++) {	// Percorre a imagem idf
     *    		for (int i = raiox; i < nx - raiox; i++) {
     *    			achou = false;
     *    			if ( ( data2D[i][j] == borda ) && ( data2D[i][j-raioy] == centro ) && ( matriz->data2D[i][j] == regiao ) ) {
     *    				for (int x = 1; x <= raiox; x++) {
     *    					if(data2D[i+x][j] >= centro) {
     *    						achou = true;
     *    						break;
     }
     }
     if( ! achou ) {
      for (int x = 0; x <= raiox; x++) {
       if(matriz->data2D[i+x][j] == regiao) {
        matriz->data2D[i+x][j] = 0;
     }
     }
     }
     }
     }
     }
     */
   /*// Método - 3 Mão acançou bons resultados
     *    pair<int,int> maiorMenor = idfMask->MaiorMenorValorNzero(); //maiorMenor.first = centro da máscara e maiorMenor.second = bordas da máscara
     *    int centro = maiorMenor.first;
     *    int borda  = maiorMenor.second;
     *    int raiox = mask->RaioX();
     *    int diametrox  = (2 * raiox) + 1;
     *    int raioy = mask->RaioY();
     *    int cont;
     *    bool flag;
     *    for (int j = raioy; j < ny - raioy; j++) {	// Percorre a imagem idf
     *    	for (int i = raiox; i < nx - raiox; i++) {
     *    		cont = 0;
     *    		flag = true;
     *    		if ( matriz->data2D[i][j] == regiao ) { // se o ponto analizado faz parte da região abertura
     *    		if ( idfAbertura->data2D[i][j] == borda ) {
     *    			cont++;
     *    			while (flag){
     *    				if ( (i + cont) < nx ) { // evitar estouro.
     *    					if ( matriz->data2D[i+cont][j] == regiao ){
     *    						if (idfAbertura->data2D[i+cont][j] != borda) {
     *    							cont++;
     } else {
      cont++;
      flag = false;
     }
     } else {
      flag = false;
     }
     } else {
      flag = false;
     }
     }
     if ( (cont < diametrox) && (cont > 2) ) {
      for (int x = 0; x < cont; x++) {
       matriz->data2D[i+x][j] = 0;
     }
     } else if (cont == 2 && matriz->data2D[i-1][j] != regiao && matriz->data2D[i+2][j] != regiao) {
      matriz->data2D[i][j] = 0;
      matriz->data2D[i+1][j] = 0;
     }
     }
     }
     }
     }
     /*
      *    int maior = idfMask->MaiorValor();
      *    int menor = idfMask->MenorValorNzero();
      *    int raiox = mask->RaioX();
      *    int raioy = mask->RaioY();
      *    for (int j = raioy; j < ny - raioy; j++) {	// Percorre a imagem idf
      *    	for (int i = raiox; i < nx - raiox; i++) {
      *    		if ( ( data2D[i][j] 		< 	maior )
      *    		  && ( data2D[i-raiox][j] 	< 	menor )
      *    		  && ( data2D[i+raiox][j] 	<	menor )
      *    		  && ( matriz->data2D[i][j]	== 	regiao ) )
      *    		{
      *    			matriz->data2D[i][j] = 0;
      }
      }
      }
      */
   /*
     *    pair<int,int> maiorMenor = idfMask->MaiorMenorValorNzero(); //maiorMenor.first = centro da máscara e maiorMenor.second = bordas da máscara
     *    cerr << "maior: " << maiorMenor.first << endl;
     *    cerr << "menor: " << maiorMenor.second << endl;
     *    int raiox = mask->RaioX();
     *    int raioy = mask->RaioY();
     *    cerr << "raiox: " << raiox << endl;
     *    cerr << "raioy: " << raioy << endl;
     *    for (int j = raioy; j < ny - raioy; j++) {	// Percorre a imagem idf
     *    		for (int i = raiox; i < nx - raiox; i++) {
     *    			if ( ( data2D[i][j] 		<= 	maiorMenor.first )
     *    			  && ( data2D[i-raiox][j] 	<= 	maiorMenor.second )
     *    			  && ( data2D[i+raiox][j] 	<=	maiorMenor.second )
     *    			  && ( matriz->data2D[i][j]	== 	regiao )
     *    			  && ( data2D[i][j-raioy] 	< 	maiorMenor.first ) )
     *    			{
     *    				matriz->data2D[i][j-raioy] = 0;
     }
     }
     }
     */
   /*
     *    pair<int,int> maiorMenor = idfMask->MaiorMenorValorNzero(); //maiorMenor.first = centro da máscara e maiorMenor.second = bordas da máscara
     *    int raiox = mask->RaioX();
     *    int raioy = mask->RaioY();
     *    for (int j = raioy; j < ny - raioy; j++) {	// Percorre a imagem idf
     *    	for (int i = raiox; i < nx - raiox; i++) {
     *    		if ( ( data2D[i][j] 		<= 	maiorMenor.first )
     *    		  && ( matriz->data2D[i][j]	== 	regiao )
     *    		  && ( data2D[i-raiox][j] 	<= 	maiorMenor.second )
     *    		  && ( data2D[i+raiox][j] 	<=	maiorMenor.second ) )
     *    		{
     *    			for(int x = -raiox; x <= raiox; x++)
     *    				matriz->data2D[i+x][j] = 0;
     }
     }
     }
     */
}

/*
==================================================================================
Função    Esqueleto.
==================================================================================
*/
//         virtual TCMatriz2D< int >* Esqueleto(TCMatriz2D< int >*& matriz,unsigned int _RaioBola=0);
//////////// //////////// //////////// //////////// // // // // //
// Depois que o calculo do esqueleto com o objeto esqueleto
// estiver pronto, copiar aqui?? resolver
//////////// //////////// //////////// //////////// // // // // //
TCMatriz2D< int > * CFEMMIDF::Esqueleto ( TCMatriz2D< int > * &matriz, unsigned int /*_RaioBola*/ ) {
   int mi = Mi ();
   // ----------------------------------------
   // -->Padrao
   // ----------------------------------------
   VerificaImagem ( matriz );	// verifica se é a mesma imagem (se diferente recalcula Go)
   // Calculou a idf.
   // ----------------------------------------
   // Calcula o raio Maximo
   // ----------------------------------------
   // raioMaximo=GetmaiorValor()/mi;
   for ( int j = 0; j < ny; j++ )	// percorre toda a idf e
      for ( int i = 0; i < nx; i++ )	// pinta pontos na imagem
         if ( data2D[i][j] > raioMaximo )
            raioMaximo = data2D[i][j];	// é na verdade o maior valor da matriz
   raioMaximo /= mi;		// agora é o raioMaximo  0->raioMaximo

   // ----------------------------------------
   //  Percorre a imagem de mi até raio maximo
   // ----------------------------------------
   //pm->Constante ( 0 );		// zera a imagem
   pm->Constante ( FUNDO );		// zera a imagem
   int raioBolaInclusa;
   for ( int raio = 1; raio < raioMaximo; raio++ ) {
      raioBolaInclusa = mi * raio;	// 3*1
      for ( int j = 2; j < ny - 1; j++ )	// vai de 2 a ny-1 por causa d5711
      {			// para d34 pode ser de 1 a ny
         for ( int i = 2; i < nx - 1; i++ ) {
            // se o ponto da idf for maior que a bola tangente, faz ponto=1
            if ( data2D[i][j] == raioBolaInclusa ) {	// pesquisa
               if ( data2D[i][j + 1] > raioBolaInclusa )
                  continue;	// se eu tiver algum vizinho maior que
               else if ( data2D[i][j - 1] > raioBolaInclusa )
                  continue;	// o raio da bola inclusa, eu não sou
               else if ( data2D[i + 1][j + 1] > raioBolaInclusa )
                  continue;	// um ponto do esqueleto (para o raio atual)
               else if ( data2D[i + 1][j] > raioBolaInclusa )
                  continue;	// sai
               else if ( data2D[i + 1][j - 1] > raioBolaInclusa )
                  continue;	//
               else if ( data2D[i - 1][j + 1] > raioBolaInclusa )
                  continue;	// se eu nao tiver nenhum vizinho
               else if ( data2D[i - 1][j] > raioBolaInclusa )
                  continue;	// maior que o raioBolaInclusa, então
               else if ( data2D[i - 1][j - 1] > raioBolaInclusa )
                  continue;	// sou um ponto do esqueleto
               else
                  pm->data2D[i][j] = 1;
            }
         }
      }
   }
   return pm;
}

/*
==================================================================================
Função    DilatacaoNosContornos
==================================================================================
Como a mascara nao pode ser encaixada nos pontos de contorno, estes são
desconsiderados durante a diltacao. Aqui pinta os pontos desconsiderados
*/

void CFEMMIDF::DilatacaoNosContornos () {	// usada pela abertura
   // Indices para percorrer a matriz
   /*
   for ( int y = 0; y < ny; y++ ) {	// plano back z=0
      if ( data2D[0][y] != 0 )
         pm->data2D[0][y] = 1;
      if ( data2D[nx - 1][y] != 0 )
         pm->data2D[nx - 1][y] = 1;
   }
   for ( int x = 1; x < nx; x++ ) {
      if ( data2D[x][0] != 0 )
         pm->data2D[x][0] = 1;
      if ( data2D[x][ny - 1] != 0 )
         pm->data2D[x][ny - 1] = 1;
   }
*/
   //Estou usando INDICE e FUNDO ao invés de 0 e 1
   for ( int y = 0; y < ny; y++ ) {	// plano back z=0
      if ( data2D[0][y] != FUNDO )
         pm->data2D[0][y] = INDICE;
      if ( data2D[nx-1][y] != FUNDO )
         pm->data2D[nx-1][y] = INDICE;
   }
   for ( int x = 1; x < nx; x++ ) {
      if ( data2D[x][0] != FUNDO )
         pm->data2D[x][0] = INDICE;
      if ( data2D[x][ny-1] != FUNDO )
         pm->data2D[x][ny-1] = INDICE;
   }
}

/*
// 	void IDFNosPlanosDeContornoIda(int& base);     // Calcula a idf nos planos de contorno
// 	void IDFNosPlanosDeContornoVolta(int& base);     // Calcula a idf nos planos de contorno
void CFEMMIDF::IDFNosPlanosDeContornoVOLTA(int& base)
{
              // Indices para percorrer a matriz
  for (int x=nx-2; x > 0 ;x--)			// nx-1
   if (data2D[x][0]!=0)
    data2D[x][0]=base;
  for (int y=ny-1; y > 0 ;y--)			// plano back z=0
   if (data2D[0][y]!=0)
    data2D[0][y]=base;
}
void CFEMMIDF::IDFNosPlanosDeContornoIDA(int& base)
{
              // Indices para percorrer a matriz
  for (int x=0; x < nx ;x++)
    if (data2D[x][ny-1]!=0)
    data2D[x][ny-1]=base;
  for (int y=0; y < ny ;y++)			// plano back z=0
    if (data2D[nx-1][y]!=0)
    data2D[nx-1][y]=base;
}
*/
// Funcoes inline
// inline void min(int& t);
// void
// CFEMMIDF::min (const int &t)
// {
// if (t < minimo)
//    minimo = t;
// }

/*
================================================================================
Função: InverterSeNecessario
================================================================================
Método chamado por Go das classes herdeiras para inverter a imagem caso FUNDO!=0
Necessário para que a imagem IDF seja criada corretamente
*/
void CFEMMIDF::InverterSeNecessario(){
   if (FUNDO != 0){ //inverte a imagem
      for ( int y = 0; y < ny; y++ ) {
         for ( int x = 0; x < nx; x++ ) {
            if ( data2D[x][y] == INDICE) {
               data2D[x][y] = FUNDO;
            } else {
               data2D[x][y] = INDICE;
            }
         }
      }
   }
}
