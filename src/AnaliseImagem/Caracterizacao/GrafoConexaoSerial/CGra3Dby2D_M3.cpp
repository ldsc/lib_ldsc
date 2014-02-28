/*
===============================================================================
@PROJETO:          Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
@Desenvolvido_por:	Laboratorio de Desenvolvimento de Software Cientifico 
[LDSC].
@author:          André Duarte Bueno
@file:             CGra3Dby2D_M3.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cassert>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGra3Dby2D_M3.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_LR.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_LR_WEST.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio_LR_EST.h>
#include <Base/CMath.h>

using namespace std;

// -------------------------------------------------------------------------
// Função:       CalcularPropriedadesConeccoes
// -------------------------------------------------------------------------
/*
@see  	:A conecção entre os sítios já foi estabelecida em DeterminarConeccoesObjetos.
   Aqui faço o calculo de uma propriedade relacionada a conecção entre os dois sítios.
  Como sou herdeiro da CGra3Dby2D_M2 tenho o rotulador 2D rotint ativo, e
  já rotulado, podendo ser diretamente utilizado.

  Ex:
  Para o estudo da permeabilidade, determino a condutância da ligação.
@author:  André Duarte Bueno
@param	:	recebe um vetor de  CObjetoGrafo*
@return : void
*/

/*
void CGra3Dby2D_M3::CalcularPropriedadesConeccoes()
{
// Percorre todos os objetos do plano atual.
// objeto.size() retorna o numero de objetos do grafo
// objeto.size();

// Para cada objeto do sítio, aloca o vetor condutância
// com o mesmo número de dados do vetor coneccao    		
// objeto[i]->Coneccao.size() retorna o número de conecções do objeto i
 unsigned long int numeroDeLinks = objeto[i]->Coneccao.size();
objeto[i]->condutancias.reserve( numeroDeLinks );
};
*/

// -------------------------------------------------------------------------
// Função:       DeterminarConeccoesObjetos
// -------------------------------------------------------------------------
/**@short : É a mesma função da classe base, a diferença é
     o calculo dos raiosHidraulicos para o plano intermediário
     e a setagem em cada objeto do valor do raioHidraulico da conecção.
@author :	André Duarte Bueno
@see    :
@param  : Recebe o número do maior rótulo já utilizado
@return : void
*/
void
CGra3Dby2D_M3::DeterminarConeccoesObjetos (unsigned long int maiorRotuloUtilizado)
{
   // Variáveis auxiliares
   unsigned long int pa, pp;

   // numeroLinksimgIntermediaria
   unsigned long int nli;

   long double raioHidraulicoLigacoes;
   COGSitio_LR *ptrSitio;

   // Copia dados da conexão das duas imagens para a img2D
   // ou seja gera imagem da intersecção entre os dois planos de rotulagem
   unsigned long int ii;
   unsigned long int jj;

   for (ii = 0; ii < img2D->NX (); ii++)
      for (jj = 0; jj < img2D->NY (); jj++)
         if (ra->data2D[ii][jj] && rp->data2D[ii][jj])
            img2D->data2D[ii][jj] = 1;
         else
            img2D->data2D[ii][jj] = 0;

   // Rotula a imagem intermediária (img2D) usando rotInt
   rotInt->Go (img2D);

   // Calculo dos raiosHidraulicos do plano intermediario
   rotInt->CalculaRaioHidraulicoObjetos ();

   // Determina o numero de links (nli) + o fundo [0]
   // Ao criar +1 objeto, esta considerando o fundo, de forma que
   // o indice i aponta para o objeto i
   // indice=0 objeto fundo 0
   // indice=1 objeto com rotulo 1....e assim sucessivamente
   nli = rotInt->RotuloFinal () + 1;

   // Cria lista dos links válidos, se =0 não validado, se =1 validado
   CVetor *links_validos = new CVetor (nli);	// usar vector
   assert (links_validos);

   // Seta todos os links como válidos
   links_validos->Constante (1);

   // Percorre imagem rotulada intermediária
   // se conectada, seta o link como válido, e conecta os sítios
   for (ii = 0; ii < rotInt->NX (); ii++)
      for (jj = 0; jj < rotInt->NY (); jj++)
         // se não for o fundo
         if (rotInt->data2D[ii][jj] != 0)
         {
            // se for um link válido, entra
            if (links_validos->data1D[rotInt->data2D[ii][jj]] == 1)
            {
               // Abaixo, seta o link como inválido (o que impede repetição do mesmo link)
               links_validos->data1D[rotInt->data2D[ii][jj]] = 0;

               // Obtem os rótulos
               pa = ra->data2D[ii][jj];
               pp = rp->data2D[ii][jj];

               // Acrescenta o deslocamento do maiorRotuloUtilizado
               pa += maiorRotuloUtilizado;

               // Soma o maiorRotuloUtilizado + ra->RotuloFinal()
               pp += maiorRotuloUtilizado + ra->RotuloFinal ();

               // Adiciona o objeto a lista de links
               // pa-1 esta conectado a pp-1
               objeto[pa - 1]->Conectar (objeto[pp - 1]);
               // pp-1 esta conectado a pa-1
               objeto[pp - 1]->Conectar (objeto[pa - 1]);

               // NOVO  : Definição dos raiosHidraulicos das coneccoes
               raioHidraulicoLigacoes =
                     rotInt->RaioHidraulicoObjetos (rotInt->data2D[ii][jj]);
               ptrSitio = dynamic_cast < COGSitio_LR * >(objeto[pa - 1]);
               ptrSitio->condutancia.push_back (raioHidraulicoLigacoes);
               ptrSitio = dynamic_cast < COGSitio_LR * >(objeto[pp - 1]);
               ptrSitio->condutancia.push_back (raioHidraulicoLigacoes);
            }
         }
   delete links_validos;
}

// -------------------------------------------------------------------------
// Função:   CalculoCondutancias
// -------------------------------------------------------------------------
/**@short  : Redefinida, em relação a CGrafo
     adiciona o calculo das condutâncias das ligações

@author :	André Duarte Bueno
@see    : Condutância
@param  : nada
@return : void
*/
void
CGra3Dby2D_M3::CalculoCondutancias (long double _viscosidade, long double _sizePixel, unsigned long int _fatorAmplificacao) {
   // Variáveis auxiliares
   long double raio_hidraulico;
   long double dH;

   long double sizePixelXfatorAmplificacao = _sizePixel * _fatorAmplificacao;
   // long double PI=3.141592653589;
   long double variavelAuxiliar = (CMath::PI) / (128.0 * _viscosidade * _sizePixel * _fatorAmplificacao);

   // A classe CGra3Dby2D_M3 tem sítios do tipo COGSitio_LR
   // que tem uma lista de conecções.
   // aqui usa um ponteiro do tipo COGSitio_LR para acessar as conecções.
   COGSitio_LR * ptrSitioLR = nullptr;

   // Percorre  todos os objetos do  grafo
   for (unsigned long int k = 0; k < objeto.size (); k++) {
      // Pega o raio hidraulico do objeto k, e converte para metros
      raio_hidraulico = objeto[k]->propriedade * sizePixelXfatorAmplificacao;

      // Calcula a condutancia da ligação
      // dH=4.0*(raio_hidraulico) ;
      dH = raio_hidraulico + raio_hidraulico + raio_hidraulico + raio_hidraulico;

      objeto[k]->propriedade = (variavelAuxiliar * dH * dH * dH * dH);

      // Calcula as condutancias das ligações
      // objeto[k] é um ponteiro para CObjetoGrafo, para obter a informação do
      // número de conecções, preciso de um ponteiro para um COGSitio (ou derivado)
      // abaixo, usa dynamic_cast para obter ponteiro p/ COGSitio*
      COGSitio * ptrSitio = dynamic_cast < COGSitio * >(objeto[k]);

      // Percorre todas as conecções do objeto
      for (unsigned int link = 0; link < ptrSitio->coneccao.size (); link++) {
         // Converte o ponteiro ObjetoGrafo para COGSitio_LR
         ptrSitioLR = dynamic_cast < COGSitio_LR * >(objeto[k]);
         // Obtêm o raio hidraulico da ligação
         raio_hidraulico = ptrSitioLR->condutancia[link];
         // Converte para SI (metros)
         raio_hidraulico *= sizePixelXfatorAmplificacao;
         // Determina o diâmetro hidraulico como sendo 4*raioHidraulico
         dH = raio_hidraulico + raio_hidraulico + raio_hidraulico + raio_hidraulico;
         // Calcula a condutância da ligação e armazena
         ptrSitioLR->condutancia[link] = (variavelAuxiliar * dH * dH * dH * dH);
      }
   }
   return;
}

// -------------------------------------------------------------------------
// Função:   EliminarCondutanciasRepetidas
// -------------------------------------------------------------------------
/**@short  : Redefinida, em relação a CGrafo3Dby2D
 Percorre todos os objetos e elimina coneccoes repetidas
 (deve ser chamada depois do calculo das condutâncias,
 pois soma as condutâncias das coneccoes repetidas.
 se chamar antes, vai acumular o raio hidraulico)
 Chama função de cada sítio, que elimina links repetidos.

@author :	André Duarte Bueno
@see    : Condutância
@param  : nada
@return : void
*/
void
CGra3Dby2D_M3::EliminarCondutanciasRepetidas ()
{
   int totalLinksDeletados = 0;

   // Percorre todos os objetos do grafo
   if (GeteliminaConeccoesRepetidas () == 1)
      for (int i = 0; i < objeto.size (); i++)
      {
         // Cria ponteiro para objeto do tipo COGSitio_LR
         COGSitio_LR *obj_i = dynamic_cast < COGSitio_LR * >(objeto[i]);
         assert (obj_i);

         // Chama DeletarConeccoesRepetidas, que retorna o número de links eliminados
         totalLinksDeletados += obj_i->DeletarConeccoesRepetidas ();
      }
   // Write( "Grafo_DeletarConeccoesRepetidas.txt");        Emedio
   cerr << "NumeroTotal de links deletados= " << totalLinksDeletados << endl;
}

// -------------------------------------------------------------------------
// Função:       GetObjetoGrafo
// -------------------------------------------------------------------------
/**@short  : 	Cria objeto herdeiro de CObjetoGrafo, de acordo com o tipo solicitado.
@author :	André Duarte Bueno
@see    :	grafos
@param  :       Inteiro identificando o tipo de sítio a ser criado.
       Formato andre: esquerda(0) centro(1) direita(2)
       Formato Liang: esquerda(1) centro(0) direita(2)
@return : 	Retorna um ponteiro para um sítio novo alocado.
*/
CObjetoGrafo *
CGra3Dby2D_M3::GetObjetoGrafo (CContorno::ETipoContorno tipoContorno)
{
   CObjetoGrafo *data;
   switch (tipoContorno)
   {
   case CContorno::ETipoContorno::CENTER:
      data = new COGSitio_LR ();
      break;

   case CContorno::ETipoContorno::WEST:
      data = new COGSitio_LR_WEST ();
      break;

   case CContorno::ETipoContorno::EST:
      data = new COGSitio_LR_EST ();
      break;

   default:
      data = new COGSitio_LR ();
      break;
   }
   assert (data);
   return data;
}

/*CGrafo* CGra3Dby2D_M3::Go( string fileName,unsigned long int funcao)
{
 // Chama função que determina o grafo
 CGra3Dby2D_M2::Go( fileName , funcao);
return this;     	
}
*/
/*
CGrafo* CGra3Dby2D_M3::Go( TCMatriz3D<int> * _img3D,unsigned long int funcao)
{	
 // Chama função que determina o grafo
 CGra3Dby2D_M2::Go( _img3D , funcao);

return this;     	
}
*/


// -------------------------------------------------------------------------
// Função:               SetMatrizAVetorB
// -------------------------------------------------------------------------

/** Recebe uma matriz A (vazia) e um vetor B (vazio) e preenche os mesmos com os coeficientes necessários
@short  : Recebe uma matriz A (vazia) e um vetor B (vazio)
 e preenche os mesmos com os coeficientes necessários
 para determinação do sistema de equações.
 1- O grafo já deve ter sido determinado
 2- Os valores iniciais de pressão já devem ter sido definidos
 (valores de contorno, normalmente Plano_0 = 1, Plano_n = 0)
 3- Deve receber uma matriz e um vetor vazios
@author :	André Duarte Bueno
@see    : grafos
@param  :
@return :	bool indicando sucesso da operação.
*/

bool
CGra3Dby2D_M3::SetMatrizAVetorB (TCMatriz2D< int > * &A, CVetor * &B)  const
{
   // vector< vector<float> > A;
   // vector<float> B;

   // Passo 0: Definição de variáveis auxiliares
   // índice i da matriz A (ou vetor B)
   unsigned long int mi;

   // índice j da matriz A
   unsigned long int mj;

   // Condutância total Cij = (Cii+Cjj)/2 para o modelo 2
   long double Cij;

   // Passo 1:
   // Determinação da dimensão da matriz e do vetor
   // cout << "\nlastObjectOfSolver="               <<       lastObjectOfSolver;
   // cout << "\nfirstObjectOfSolver="      <<  firstObjectOfSolver;
   unsigned int dim = lastObjectOfSolver - firstObjectOfSolver + 1;
   // cout <<"\ndim="<<dim;

   // Redimensiona a matriz A
   A->Redimensiona (dim, dim);
   // Zera a matriz A
   A->Constante (0);

   // Redimensiona o vetor B
   B->Redimensiona (dim);
   // Zera o vetor B
   B->Constante (0);

   unsigned int i;
   for (unsigned long int j = 0; j < objeto.size (); j++)
   {
      // Faz um cast para sítio derivado (em função do acesso a função Contorno e vetor coneccao.
      COGSitio_LR *objeto_j = dynamic_cast < COGSitio_LR * >(objeto[j]);
      assert (objeto_j);

      switch (objeto_j->Contorno ())
      {
      // Fronteira esquerda
      case CContorno::ETipoContorno::WEST:

         // Fronteira direira
      case CContorno::ETipoContorno::EST:
         // Percorre as conecções do objeto
         for (i = 0; i < objeto_j->coneccao.size (); i++)
         {
            // Calcula Cij
            Cij = objeto_j->condutancia[i];
            Cij = Cij * 1.0e17;	// LIXO, para gerar int
            // cij esta sendo armazenado em int por isto multiplico por e17

            // Desloca o índice da matriz(vetor), pois só entram os sítios que não estão na interface.
            mi = objeto_j->coneccao[i]->rotulo - firstObjectOfSolver;	// 3;

            // Acumula Cij no vetor B[mi] -= Cij     * objeto_j->x, x deve estar definido
            // B->data1D[ mi ] -= Cij * objeto_j->x;
            B->data1D[mi] -= static_cast < int >(Cij * objeto_j->x);	// LIXO o static

            // Acumula -Cij na matriz A[mi][mi]
            // A->data2D[mi][mi] -= Cij;
            A->data2D[mi][mi] -= static_cast < int >(Cij);	// LIXO o static
         }
         break;

         // Fronteira Centro
      case CContorno::ETipoContorno::CENTER:
         // Percorre as conecções do objeto
         for (i = 0; i < objeto_j->coneccao.size (); i++)
         {
            // Se o link  for  um objeto de centro (não contorno) entra
            if (objeto_j->coneccao[i]->Contorno () == CContorno::ETipoContorno::CENTER)
            {
               // Calcula Cij
               // Cij = ( objeto_j->propriedade + objeto_j->coneccao[i]->propriedade ) /2.0 ;
               Cij = objeto_j->condutancia[i];
               Cij = Cij * 1.0e17;	// LIXO para gerar int
               // cij esta sendo armazenado em int por isto multiplico por e17

               // Desloca os índices da matriz
               mi = objeto_j->coneccao[i]->rotulo - firstObjectOfSolver;
               mj = objeto_j->rotulo - firstObjectOfSolver;

               // Define A->data2D[mi][mj]
               A->data2D[mi][mj] = static_cast < int >(Cij);	// LIXO o static

               // Acumula A->data2D[mj][mj]
               A->data2D[mj][mj] -= static_cast < int >(Cij);	// LIXO o static
            }
         }
         break;
      }			// switch
   }				// for

   A->Write ("grafo.matrixA");
   B->Write ("grafo.vectorB");
   return 1;
}
