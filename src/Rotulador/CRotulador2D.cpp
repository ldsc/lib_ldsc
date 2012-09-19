/*
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: TPadrao_ramo
  ===============================================================================

  Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
  	[LDSC].
  @author:          André Duarte Bueno
  File:             CRotulador2D.cpp
  begin:            Sat Sep 16 2000
  copyright:        (C) 2000 by André Duarte Bueno
  email:            andre@lmpt.ufsc.br
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
// #include <fstream>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Rotulador/CRotulador2D.h>
// using namespace std;
/*
  -------------------------------------------------------------------------
  Função:
  -------------------------------------------------------------------------
  @short  :	Calcula area de cada objeto
  @author :André Duarte Bueno
  @see    :
  @param  : void
  @return : void
*/
void CRotulador2D::CalculaAreaObjetos () {
   // só procede o calculo se a matriz rótulo já foi determinada
   if (!rotulado)
      Go (pm);			// Calcula a matriz rotulo

   if (rotulado) {			// Só procede o calculo se a matriz rótulo foi determinada
      if (areaObjetos)		// Se o vetor areaObjetos já existe, apaga
         delete areaObjetos;
      areaObjetos = new CVetor (numeroObjetos);	// Cria vetor área dos objetos

      if (areaObjetos) {		// Se alocado corretamente
         areaObjetos->Constante (0);	// Zera o vetor área dos objetos
         for (unsigned int i = 0; i < NX (); i++)	// Percorre direcao x
            for (unsigned int j = 0; j < NY (); j++)	// Percorre direcao y
               areaObjetos->data1D[data2D[i][j]]++;	// Incrementa vetor
      }
   }
}

/*
  -------------------------------------------------------------------------
  Função:
  -------------------------------------------------------------------------
  @short  :
  -Calcula o perimetroObjetos
  -Primeiro calcula a matrizRotulo.
  -Depois percorre todos os pontos válidos da matrizRotulo, e verifica se tem um ponto
  vizinho=FUNDO,se o ponto vizinho for = FUNDO é contabilizado o perímetro.
  -Observe que primeiro percorre a região central da matrizRotulo, depois as bordas.

  ---------------------
  Da forma como esta não calcula o perímetro do objeto de rótulo=FUNDO (o fundo).
  Como calcular o perimetro do objeto 0:
  1- Pegar a imagem binaria (0 e 1)
  2- Calcular o perimetro do objeto com rotulo =1 na imagem binaria
  este será o perímetro do objeto de rotulo 0.
  ----------------------

  @author :	André Duarte Bueno
  @see    :
  @param  : void
  @return : void
  Pré-condições:   já ter realizado a rotulagem com a chamada a GO
  Bug:			se o objeto apontado por pm tiver sido deletado, e chamar
  esta função, vai acessar ilegalmente pm.
  Excessões:        tipos de excessoes
*/
void CRotulador2D::CalculaPerimetroObjetos () {
   if (!rotulado)		// só procede o calculo se a matriz rótulo já foi determinada
      Go (pm);			// Cria e calcula a matriz rotulo
   // BUG: se pm tiver sido deletado, trocar por Go(this)?

   if (rotulado) {			// só procede o calculo se a matriz rótulo foi determinada
      if (perimetroObjetos)	// Cria vetor perimetroObjetos
         delete perimetroObjetos;
      perimetroObjetos = new CVetor (numeroObjetos);

      if (perimetroObjetos) {	// testa se o vetor perimetro foi criado
         perimetroObjetos->Constante (0);	// zera os perrimetros como sendo 0
         perimetroObjetos->data1D[0] = 1;	// Objeto 0 assume perimetro 1 (erro assumido)
         perimetroObjetos->data1D[1] = 1;	// Objeto 1 assume perimetro 1 (erro assumido)
         //agora o fundo é representado ou pelo objeto 0 ou pelo objeto 1 dependendo do valor de FUNDO setado no contrutor.

         unsigned int i, j;	// Variaveis auxiliares para loop's

         // --------------------------------
         // Região central, fora as bordas
         for (i = 1; i < NX () - 1; i++)	// Percorre direcao x, fora bordas
            for (j = 1; j < NY () - 1; j++) {	// Percorre direcao y, fora bordas
               if (data2D[i][j] != FUNDO) {
                  if (data2D[i - 1][j] == FUNDO)	// esquerdo
                     perimetroObjetos->data1D[data2D[i][j]]++;
                  if (data2D[i + 1][j] == FUNDO)	// direito
                     perimetroObjetos->data1D[data2D[i][j]]++;
                  if (data2D[i][j - 1] == FUNDO)	// acima
                     perimetroObjetos->data1D[data2D[i][j]]++;
                  if (data2D[i][j + 1] == FUNDO)	// abaixo
                     perimetroObjetos->data1D[data2D[i][j]]++;
               }
            }
         // --------------------------------
         // Borda esquerda
         i = 0;
         for (j = 1; j < NY () - 1; j++) { // Percorre direcao y,
            if (data2D[i][j] != FUNDO) {
               perimetroObjetos->data1D[data2D[i][j]]++;	// considera esquerdo como perímetro [novo]
               if (data2D[i + 1][j] == FUNDO)	// direito
                  perimetroObjetos->data1D[data2D[i][j]]++;
               if (data2D[i][j - 1] == FUNDO)	// acima
                  perimetroObjetos->data1D[data2D[i][j]]++;
               if (data2D[i][j + 1] == FUNDO)	// abaixo
                  perimetroObjetos->data1D[data2D[i][j]]++;
            }
         }
         // --------------------------------
         // Borda direita
         i = NX () - 1;
         for (j = 1; j < NY () - 1; j++) {	// Percorre direcao y,
            if (data2D[i][j] != FUNDO) {
               perimetroObjetos->data1D[data2D[i][j]]++;	// considera direito como perímetro [novo]
               if (data2D[i - 1][j] == FUNDO)	// esquerdo
                  perimetroObjetos->data1D[data2D[i][j]]++;
               if (data2D[i][j - 1] == FUNDO)	// acima
                  perimetroObjetos->data1D[data2D[i][j]]++;
               if (data2D[i][j + 1] == FUNDO)	// abaixo
                  perimetroObjetos->data1D[data2D[i][j]]++;
            }
         }
         // --------------------------------
         // Borda superior
         j = 0;
         for (i = 1; i < NX () - 1; i++) {	// Percorre direcao x,
            if (data2D[i][j] != FUNDO) {
               perimetroObjetos->data1D[data2D[i][j]]++;	// considera superior como perímetro [novo]
               if (data2D[i - 1][j] == FUNDO)	// esquerdo
                  perimetroObjetos->data1D[data2D[i][j]]++;
               if (data2D[i + 1][j] == FUNDO)	// direito
                  perimetroObjetos->data1D[data2D[i][j]]++;
               if (data2D[i][j + 1] == FUNDO)	// abaixo
                  perimetroObjetos->data1D[data2D[i][j]]++;
            }
         }
         // --------------------------------
         // Borda inferior
         j = NY () - 1;
         for (i = 1; i < NX () - 1; i++) {	// Percorre direcao x,
            if (data2D[i][j] != FUNDO) {
               // if(data2D[i][j]>0)
               perimetroObjetos->data1D[data2D[i][j]]++;	// considera inferior como perímetro [novo]
               if (data2D[i - 1][j] == FUNDO)	// esquerdo
                  perimetroObjetos->data1D[data2D[i][j]]++;
               if (data2D[i + 1][j] == FUNDO)	// direito
                  perimetroObjetos->data1D[data2D[i][j]]++;
               if (data2D[i][j - 1] == FUNDO)	// acima
                  perimetroObjetos->data1D[data2D[i][j]]++;
            }
         }

         // --------------------------------
         // 4 cantos (incluído na versão 7)
         // Falta considerar os 4 pontos dos 4 cantos da imagem
         // Canto superior esquerdo
         i = 0;
         j = 0;
         if (data2D[i][j] != FUNDO) {
            perimetroObjetos->data1D[data2D[i][j]]++;	// considera  Canto superior
            perimetroObjetos->data1D[data2D[i][j]]++;	// considera  Canto esquerdo
            if (data2D[i + 1][j] == FUNDO)
               perimetroObjetos->data1D[data2D[i][j]]++;
            if (data2D[i][j + 1] == FUNDO)
               perimetroObjetos->data1D[data2D[i][j]]++;
         }

         // Canto superior direito
         i = NX () - 1;
         j = 0;
         if (data2D[i][j] != FUNDO) {
            perimetroObjetos->data1D[data2D[i][j]]++;	// considera  Canto superior
            perimetroObjetos->data1D[data2D[i][j]]++;	// considera  Canto direito
            if (data2D[i - 1][j] == FUNDO)
               perimetroObjetos->data1D[data2D[i][j]]++;
            if (data2D[i][j + 1] == FUNDO)
               perimetroObjetos->data1D[data2D[i][j]]++;
         }
         // Canto inferior esquerdo
         i = 0;
         j = NY () - 1;
         if (data2D[i][j] != FUNDO) {
            perimetroObjetos->data1D[data2D[i][j]]++;	// considera  Canto inferior
            perimetroObjetos->data1D[data2D[i][j]]++;	// considera  Canto esquerdo
            if (data2D[i][j - 1] == FUNDO)
               perimetroObjetos->data1D[data2D[i][j]]++;
            if (data2D[i + 1][j] == FUNDO)
               perimetroObjetos->data1D[data2D[i][j]]++;
         }

         // Canto inferior direito
         i = NX () - 1;
         j = NY () - 1;
         if (data2D[i][j] != FUNDO) {
            perimetroObjetos->data1D[data2D[i][j]]++;	// considera  Canto inferior
            perimetroObjetos->data1D[data2D[i][j]]++;	// considera  Canto direito
            if (data2D[i - 1][j] == FUNDO)
               perimetroObjetos->data1D[data2D[i][j]]++;
            if (data2D[i][j - 1] == FUNDO)
               perimetroObjetos->data1D[data2D[i][j]]++;
         }
         // Salvar(perimetroObjetos,"perimetroObjetos.peo");
      }			// fim do if(perimetroObjetos)
   }				// fim do  if(matrizRotulo!=NULL)
}

/*
  -------------------------------------------------------------------------
  Função:
  -------------------------------------------------------------------------
  @short  :Executa toda a sequencia de rotulagem
  @author :André Duarte Bueno
  @see    :
  @param  :
  @return :
*/
// Pende em mudar para void*, que será formatado na função PreparaImagem
// bool CRotulador2D::Go(void* matriz)
bool CRotulador2D::Go (TMatriz2D< int > * matriz) {
   // this->rotuloInicial=rotuloInicial;

   // Verifica a imagem
   if (PreparaImagem (matriz) == false)
      return false;

   // debug
   // {IProcessTime wait("IdentificaObjetos.tempo.txt");
   // IdentificaObjetos();}
   // vConversao->Write("1-VetorConversaoDepoisIdentificaObjetos.txt");
   // IdentificaObjetos(rotuloInicial);
   IdentificaObjetos ();

   // debug
   // {IProcessTime wait("PesquisaRotulosValidosEOrdena.tempo.txt");
   // PesquisaRotulosValidosEOrdena();}
   // vConversao->Write("2-VetorConversaoDepoisPesquisaRotulosValidosEOrdena.txt");
   PesquisaRotulosValidosEOrdena ();

   // debug
   // {IProcessTime wait("UniformizaTabelaRotulos.tempo.txt");
   // UniformizaTabelaRotulos();}
   // vConversao->Write("3-VetorConversaoDepoisUniformizaTabelaRotulos.txt");
   UniformizaTabelaRotulos ();

   // debug
   // {IProcessTime wait("RotulaImagem.txt");
   // RotulaImagem();}
   RotulaImagem ( /*_rotuloInicial*/ );

   // Define que a imagem já foi rotulada
   rotulado = true;

   // Retorna true indicando que ocorreu tudo bem
   return true;
};


/*
  -------------------------------------------------------------------------
  Função:   PreparaImagem
  -------------------------------------------------------------------------
  @short  :
  Funcao chamada por Go, para verificar a imagem passada.
  Ou seja antes de inicializar o procedimento de rotulagem,
  faz algumas verificações com a imagem.
  Armazena o ponteiro para a imagem e verifica se a imagem tem as mesmas dimensões
  caso contrário, realoca a imagem.
  Depois copia os dados da imagem.
  @author :André Duarte Bueno
  @see    :
  @param  :
  @return :
*/
// bool CRotulador2D::PreparaImagem(void* matriz)
bool CRotulador2D::PreparaImagem (TMatriz2D< int > * matriz) {
   pm = matriz;			// armazena endereço matriz
   if (pm == NULL)		// Se for nulo após igualar, sai
      return false;

   // 1)Verifica se a matriz tem as mesmas dimensoes do rotulador
   // se tiver dimensões diferentes redimensiona a matriz do rotulador
   // 2) Se foi usado o construtor sem parâmetros, data2D não foi alocada
   // nx=ny=0 e o numero objetos=0, ou seja, data2D não pode ser acessada,
   // mas esta responsabilidade é do programador, pois ao criar uma matriz
   // sem passar as dimensoes nx, e ny, este sabe que a mesma não foi alocada,
   // e não pode ser utilizada.
   // Se data2D==NULL (não alocada), a mesma vai ser alocada abaixo.
   if (this->nx != matriz->NX () || this->ny != matriz->NY ()) {
      Desaloca ();		// desaloca a matriz existende e depois
      this->nx = matriz->NX ();	// define novo tamanho
      this->ny = matriz->NY ();
      Aloca ();			// aloca de acordo com novo tamanho
   }
   // Agora tenho de armazenar valores de pm na idf

   // Tarefa:
   // Abaixopassa tudo para 0 e 1.
   // Verificar o que precisa ser alterado para realizar corretamente a rotulagem
   // sem passar tudo para 0 e 1.
   // Ou seja eliminar a necessidade do código abaixo
   for (unsigned int j = 0; j < ny; j++)
      for (unsigned int i = 0; i < nx; i++)
         //LP - if (matriz->data2D[i][j] != 0)	// como a imagem recebida pode ser uma outra idf
         if (matriz->data2D[i][j] == INDICE)	// como a imagem recebida pode ser uma outra idf
            this->data2D[i][j] = INDICE;	// define this com 0 e 1
         else
            this->data2D[i][j] = FUNDO;	// Garante FUNDO = 0 // v7

   // Como pode chamar Go mais de uma vez, preciso apagar os vetores area e perimetro
   // que foram alocados na chamada anterior a Go (Versão 7)
   if (areaObjetos)
      delete areaObjetos;
   areaObjetos = NULL;

   if (perimetroObjetos)
      delete perimetroObjetos;
   perimetroObjetos = NULL;
   return true;

   // Write("PreparaImagem.txt");
}

/*
  -------------------------------------------------------------------------
  Função: IdentificaObjetos
  -------------------------------------------------------------------------
  @short  :
  O procedimento de rotulagem é executado pela função Go.
  Go chama IdentificaObjetos, que realiza a primeira passagem sobre a imagem.

  O objeto CRotulador2D é uma imagem 2D, que inicialmente contem um conjunto de pontos
  cujos valores sao 0 se representam a fase solida e valores diferentes de 0 se poros.
  -O vetor vConversao é uma tabela de conversão, um pixel de valor k na imagem terá
  seu valor convertido para vConversao[k], ou seja rotulo = vConversao[k].
  Ex: matriz2D[i][j] = vConversao[ matriz2D[i][j] ];
  -Se o valor do vetor vConversao for negativo, então o seu conteúdo não um rótulo válido,
  mas um índice indireto para uma outra posição no vetor de conversão.

  A função IdentificaObjetos segue o seguinte procedimento:
  1- Verifica o ponto [0][0], se preto (!=FUNDO), rotula.
  2- Verifica a primeira linha [i][0], rotulando a mesma.
  3- Varre as demais colunas e linhas, rotulando cada ponto.
  -Realiza procedimento especial para a primeira coluna (para evitar acesso a ponto inexistente).
  -Processa todas as outras linhas e colunas.
  A rotulagem de cada ponto leva em conta o ponto acima e o ponto a esquerda.
  (motivo pelo qual realizou-se processo separado para o ponto [0][0], para a
  primeira linha [i][0]	e a primeira coluna [0][j]).

  Obs: Existe a necessidade de se verificar dois casos de alteração de rotulos
  (No codigo atual, estão funcionando):
  1)
  O primeiro caso envolve a transformação de um rotulo válido (ex 8 -> 8) para um indireto (ex 8-> -15).
  Este procedimento não traz problemas. Mas pode ocorrer de um outro pixel de rotulo 8 apontar indiretamente
  para -20 (8-> -20). Neste caso, o 8 deve apontar para -15 ou -20?
  A solução é fazer os dois apontarem para -20, para tal
  devo fazer 8-> -20 e procurar o -15 e fazê-lo apontar para -20 (15 -> -20).
  Ex: 8->-15 (oito aponta indiretamente para -15)
  e logo depois faz 8->-20 (oito aponta para indiretamente para -20).

  2)
  O segundo problema que precisa ser verificado é a ocorrência de um loop infinito.
  Este caso ocorre quando um rotulo aponta para só mesmo.
  Ex: 30->-30, ou seja, evitar que um rotulo aponte para ele mesmo criando um loop infinito.

  Obs: Os primeiros testes realizados, demonstram que esta rotina de identificação é eficiente
  e funciona corretamente.
  Agora sua total segurança só poderá ser comprovada depois de inúmeros testes com diferentes
  tipos de imagens.
  @author :André Duarte Bueno
  @see    :
  @param  :void
  @return :void
*/
void CRotulador2D::IdentificaObjetos () {
   // i,j mesmo tipo da matriz ou seja Tipo
   // Usados nos for
   unsigned long int i, j;

   // Valores dos pixel's acima e a esquerda
   int acima, esquerda;

   // índice do rotulo atual
   // LP - unsigned long int rotuloAtual = 0;
   unsigned long int rotuloAtual = 2; //Irá rotular a partir de 2 para poder utilizar 1 como fundo

   // Necessario pois Go pode ser chamada diretamente + de uma vez,
   // no final de Go o numeroObjetos é o encontrado na imagem
   // e pode ser menor na segunda chamada de Go provocando estouro da pilha.
   rotuloMaximoUtilizado = numeroObjetos = NX () * NY () / 2;

   // Cria vetor de conversão vConversao
   if (vConversao != NULL)
      delete vConversao;
   vConversao = new CVetor (numeroObjetos);

   // se vConversao[i]>=0 indica um rotulo valido
   // se vConversao[i]<0 indica uma referencia indireta na vConversao
   // Os valores do vetor de conversao sao pre definidos
   // a ser uma sequencia 0->0,1->1,2->2,3->3,...
   for (i = 0; i < vConversao->NX (); i++)
      vConversao->data1D[i] = i;

   // ---------------------------------------------
   // PONTO 0 0
   // ---------------------------------------------
   // FAZ pixel[0][0]=1, ou seja objeto 0 com rotulo 0
   // assume o valor 1
   if (data2D[0][0] != FUNDO)
      data2D[0][0] = ++rotuloAtual;

   // ---------------------------------------------
   // PRIMEIRA LINHA [i][0]
   // ---------------------------------------------
   for (i = 1; i < NX (); i++)
      if (data2D[i][0] != FUNDO)	// se for poro (preto)
         if (data2D[i - 1][0] != FUNDO)	// e se o anterior for preto
            data2D[i][0] = rotuloAtual;	// usa rotulo atual (igual ao do ponto a esquerda)
         else
            data2D[i][0] = ++rotuloAtual;	// incrementa rotulo e usa

   // ---------------------------------------------
   // TODAS AS DEMAIS LINHAS E COLUNAS [i][j]
   // ---------------------------------------------
   for (j = 1; j < NY (); j++) {	// Varre as colunas y=j
      // PRIMEIRA COLUNA
      if (data2D[0][j] != FUNDO) {	// se for poro (preto)
         data2D[0][j] = ++rotuloAtual;	// incrementa o rotulo e usa
         acima = data2D[0][j - 1];	// armazena valor do ponto acima
         VerificaContorno (acima, rotuloAtual);	// rotulo atual de forma indireta.
      }
      // ---------------------------------------------
      // ABAIXO PONTOS [i][j]
      // ABAIXO TENTATIVA DE ALTERACAO PARA FICAR MAIS CLARO
      for (i = 1; i < NX (); i++) {	// Varre as linhas x=i
         if (data2D[i][j] != FUNDO) {	// se for poro (preto)
            acima = data2D[i][j - 1];	// armazena informação do ponto acima
            esquerda = data2D[i - 1][j];	// e da esquerda
            if (esquerda != FUNDO) {	// se a esquerda e acima forem != FUNDO
               data2D[i][j] = rotuloAtual;	// fica com o rotulo atual
               VerificaContorno (acima, rotuloAtual);
            } else {		// esquerda==FUNDO
               data2D[i][j] = ++rotuloAtual;	// incrementa rotulo e usa
               VerificaContorno (acima, rotuloAtual);
            }
         }			// fim if
      }			// fim  do              for i
   }				// fim do       for j

   rotuloMaximoUtilizado = rotuloAtual;	// necessario, define o maior rotulo utilizado
   // Write("identificaObjetos.ma2.txt");
}

/*
  -------------------------------------------------------------------------
  Função:
  -------------------------------------------------------------------------
  @short  :
  é a 2a passagem,
  define os rotulos finais, ou seja
  rotula a imagem final usando  o vetor de conversao: vConversao.

  Versão 6:
  Para considerar o rotulo inicial, basta alterar aqui:
  Como quero que os rotulos obedeçam um intervalo
  que vai de rotuloInicial (passado pelo usuário), até
  rotuloInicial+numeroObjetos.
  Ficou mais fácil só alterar aqui.
  Em todos os passos anteriores os rótulos vão de 0->RotuloMaximoUtilizado.
  Aqui faço o rotulo 0 -> rotuloInicial e assim sucessivamente.

  // Ex:
  // 5 objetos: Rotulos: 0,1,2,3,4
  // rotuloInicial = 0
  // numeroObjetos = 5
  // rotuloFinal   = 4 =  rotuloInicial + numeroObjetos - 1
  // rotuloInicial  =  _rotuloInicial;


  Versão 7: Temporariamente o deslocamente de rótulos foi eliminado
  por adicionar verificações no calculo do perimetro e do raioHidraulico.

  @author :André Duarte Bueno
  @see    :
  @param  :void
  @return :void
*/
// void CRotulador2D::RotulaImagem(/*unsigned int _rotuloInicial*/)
void CRotulador2D::RotulaImagem () {
   // O numero de objetos é calculado na função PesquisaRotulosValidosEOrdena()
   // rotuloFinal    =  rotuloInicial  +   numeroObjetos - 1; // Versão 6

   // Corrigir os rótulos da imagem usando o vetor de conversão
   for (unsigned int i = 0; i < NX (); i++)
      for (unsigned int j = 0; j < NY (); j++)
         // data2D[i][j] = vConversao->data1D[ data2D[i][j] ]; formato antigo
         // Com o+rotuloInicial, faz uma translação dos rotulos
         // mas também esta transladando o rotulo = 0 do fundo
         // por isso preciso de if's adicionais na função Area e perimetro
         // if( data2D[i][j] != 0)
         if (data2D[i][j] != FUNDO)	// Este if é desnecessario, pois o vconversao[0] aponta para 0 (ou rotuloInicial)
            data2D[i][j] = vConversao->data1D[data2D[i][j]];
   // data2D[i][j] = vConversao->data1D[ data2D[i][j] ]  + rotuloInicial;

   if (vConversao != NULL)
      delete vConversao;		// Não preciso mais do vetor de conversão (é deletado porque é grande)
   vConversao = NULL;		// novo

   rotulado = true;		// Define que o objeto ja esta rotulado
   // Write("rotulaImagem.ma2.txt");
}

/*
  -------------------------------------------------------------------------
  Função:
  -------------------------------------------------------------------------
  @short  :
  Funcao desenvolvida pelo Fabio Magnani e adaptada pelo Andre Bueno para o ANAIMP
  Esta funcao esta funcionamndo.
  é o algoritimo de rotulagem de Hoshen-Kopelman.
  A funcao  realiza duas passagens pela imagem. A imagem final tem uma
  sequencia de rotulos quebrada (ex: 0,1,4,7,9,12,...).
  Se voce deseja uma sequencia de rotulos uniforme (0,1,2,3,4,5,6,..)
  chame após a função  FuncaoRotulaImagemSequencialAntiga()
  Obs: Esta função já calcula a área dos objetos (desnecessário) e a substituição
  dos rótulos indiretos é realizado na segunda passagem (+lento).
  @author :André Duarte Bueno
  @see    :
  @param  :void
  @return :void
*/
/*  
// renomear para Go_HoshenKopelman()
void CRotulador2D::FuncaoRotulaImagemAntiga()
{
if(rotulado==1)				// se ja rotulado sai
return;
unsigned int   i,j;				// usados nos for
int	acima, esquerda; 			// valores dos pixel's acima a esquerda
int 	maior, menor;         // 
int   rotulo=0;  				// rotuloAtual
int	mRotulo[10000];         // vetor com número de pontos por rotulo

// se mRotulo[i]>0 indica número de pontos
// se mRotulo[i]<0 indica rótulo correto
int	PRETO=1;                // poro
int 	BRANCO=0;             // sólido

for(i=0; i < 10000 ; i++)
mRotulo[i]=0;       // zera vetor mRotulo

// 1- Verifica o ponto [0][0], se preto, rotula.
// Primeiro ponto [0][0]
if (data2D[0][0] == PRETO)
{
data2D[0][0]=++rotulo;	// =1
mRotulo[rotulo]=1;      // mRotulo[1]=1
}
// 2- Verifica a primeira linha [i][0], rotulando a mesma
// Primeira linha
for (i=1; i < NX() ; i++)
if (data2D[i][0] == PRETO)			// se for poro:
if (data2D[i-1][0] == BRANCO)	// e se o anterior for branco
{	data2D[i][0]=++rotulo;	mRotulo[rotulo]=1;	}  // incrementa rotulo, rotula, e acumula vetor mRotulo
else
{  data2D[i][0]=rotulo;	mRotulo[rotulo]++;		}  // e se o anterior for preto, rotula, e acumula vetor mRotulo

// 3- Varre as colunas e as linhas, rotulando cada ponto.
// Varre as colunas
for (j=1; j < NY() ;j++)
{// ????????	Varia o número da coluna
// Aqui considera a  coluna 0 (para evitar estouro da pilha)
if (data2D[0][j] == PRETO)
if (data2D[0][j-1] == BRANCO)
{ data2D[0][j]=++rotulo;	  mRotulo[rotulo]=1;
}
else
{ 	while ( mRotulo[data2D[0][j-1]] < 0 )
data2D[0][j-1] = (-mRotulo[data2D[0][j-1]]);
data2D[0][j] = data2D[0][j-1];
mRotulo[ data2D[0][j] ]=mRotulo[ data2D[0][j] ] + 1;
}
// Varre as linhas
for (i=1; i < NX() ;i++)
{
if (rotulo > 10000 )
// MessageBox("Mais de 10.000 objetos!!","Erro",MB_OK);
cout<<"Mais de 10.000 objetos!!";
if (data2D[i][j] == PRETO)
{
acima    = data2D[i][j-1];
esquerda = data2D[i-1][j];
if (acima == BRANCO && esquerda == BRANCO)
{
data2D[i][j]= ++rotulo;
mRotulo[rotulo]=1;
}
else	// #1
{
while ( mRotulo[acima] < 0 )					 // procura ultimo indice
acima= -mRotulo[acima];
while ( mRotulo[esquerda] < 0 )
esquerda= -mRotulo[esquerda];
if (acima == BRANCO || esquerda == BRANCO)
{
if (acima == BRANCO)
{ menor = esquerda; maior = acima;}
if (esquerda == BRANCO)
{ menor = acima; maior = esquerda;}
}
else
{
if ( acima <esquerda )
{	menor=acima;   maior=esquerda; 	}
if ( acima > esquerda )
{	menor=esquerda; maior=acima;    	}
if ( acima == esquerda )
{	menor=esquerda;  	maior=acima;	}
}

if ( maior != menor )
{
mRotulo[menor]++;
mRotulo[menor]+= mRotulo[maior];
mRotulo[maior]=-menor;
mRotulo[0]=0;
data2D[i][j]=menor;
}
else
{
mRotulo[menor]++;
data2D[i][j]=menor;
}
}// fim else #1
}// fim 	do		if (data2D[i][j] == PRETO)

}// fim do	 for (i=1; i < pm->NX() ;i++)
}// fim do 	for (j=1; j < pm->NY() ;j++)

// 4-Corrige os rótulos com valores errados
// Agora vai corrigir os rótulos. Ficando a matrizRotulo com os valores adequados
// Aqui pode usar tecnica Andre, de primeiro corrigir a mRotulo deixando todos os indices positivos
// e depois deixar os indices em uma ordem crescente.
for(i=0; i < NX() ; i++)
for(j=0; j < NY() ; j++)
while ( mRotulo[data2D[i][j] ] < 0 )
data2D[i][j]=(-mRotulo[ data2D[i][j] ]);

// Abaixo foi incluido pelo Andre para determinação do maior rotulo utilizado
// 5-Determina o maior rotulo utilizado, usado para alocar vetores area,perimetro e raio hidraulico.
// O numero correto de objetos, é determinado na função  FuncaoRotulaImagemSequencialAntiga
for(i=1; i< 10000 ; i++)
if( mRotulo[i] > 0 )
numeroObjetos= i;
numeroObjetos++;					// acrescentei para considerar o 0 que é o primeiro objeto.
//  Write("matrizRotulo.rot"); // salva matriz resultado em disco
rotulado=1;							// define que o objeto ja esta rotulado
}
*/

/*
   -------------------------------------------------------------------------
   Função: FuncaoRotulaImagemSequencialAntiga
   -------------------------------------------------------------------------
   @short  :
   Rotula a imagem de maneira sequencial
   Rotulo areaObjeto   objetoAtivo
   0      5555         0 (Desconsiderado)
   1      67           1
   2      78           2
   3      0 (inativo)  0
   4      76           3
   Primeiro rotula a imagem
   Depois atualiza os rotulos, numa sequencia 0,1,2,3,4... sem saltos.
   Da forma como esta realiza 2 passagens para rotulagem e +2 para deixar sequencial
   @author :André Duarte Bueno
   @see    :
   @param  :
   @return :
 */
/*
   void CRotulador2D::FuncaoRotulaImagemSequencialAntiga()
   {
   CVetor objetoAtivo(numeroObjetos);	// cria vetor dos objetos ativos
   objetoAtivo.Constante(0);		// todos os objetos são marcados como inativos
   int   i,x,y;                      	// Variaveis auxiliares para loop's
   for(x=0; x<NX(); x++)	       	// percorre a matrizRotulo
   for(y=0; y<NY(); y++)              // Se for ativo anota =1
   objetoAtivo.data1D[data2D[x][y]]=1;

   int contador=0;
   for(i=0;i<numeroObjetos;i++)		// Percorre todos os objetos
   if(objetoAtivo.data1D[i]==1)      	// Se o objeto esta ativo
   {
   objetoAtivo.data1D[i]=contador; // Anota novo rotulo
   contador++;
   }
   numeroObjetos=contador;		// O indice 0 é considerado no ultimo contador++
   // conferir se o numero de objetos esta  correto??
   for(x=0; x<NX(); x++)         	// percorre a matriz e coloca o rótulo sequencial
   for(y=0; y<NY(); y++)
   data2D[x][y]=objetoAtivo.data1D[data2D[x][y]];
   }
 */
