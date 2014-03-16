/**
===============================================================================
@PROJETO:          Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
@Desenvolvido_por:
			Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:          André Duarte Bueno
@file:            CGrafoConexaoSerial_M3.cpp
@begin:           Sat Sep 16 2000
@copyright:       (C) 2000 by André Duarte Bueno
@email:           andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cassert>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
<<<<<<< HEAD:src/AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafo_3Dby2D_M3.cpp
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafo_3Dby2D_M3.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Sitio.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Sitio_WEST.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Sitio_EST.h>
// #include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_CC_Sitio.h>
// #include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_CC_Sitio_WEST.h>
// #include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_CC_Sitio_EST.h>
=======
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial_M3.h>

#ifndef CObjetoRede_Sitio_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Sitio.h>
#endif

#ifndef CObjetoRede_Sitio_WEST_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Sitio_WEST.h>
#endif

#ifndef CObjetoRede_Sitio_EST_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Sitio_EST.h>
#endif

#ifndef CMath_h
>>>>>>> /bad-path/
#include <Base/CMath.h>
using namespace std;

// -------------------------------------------------------------------------
// Função:       DeterminarConexoesObjetos
// -------------------------------------------------------------------------
/** @short : Função DeterminarConexoesObjetos(ra,rp,maiorRotuloUtilizado) override;
  * Lembre-se no modelo M1, para cada pixel com intersecção uma conexão,
  * No modelo M2 usamos um plano intermediário para evitar que cada intersecção entre píxeis
  * corresponda a uma conexão.
  * No modelo M2 a função DeterminarConexoesObjetos funciona da seguinte forma:
  * Os planos i e i+1 são rotulados, gerando-se a seguir os sítios.
  * Depois gera-se um objeto de rotulagem com o plano intermediário rotint (da conexão dos planos i com i+1)
  * e um vetor de link's válidos.
  * Percorre as imagens i e i+1 e estabelece as conexões nos dois sentidos, só que
  * para evitar a repetição da conexão a cada píxel com intersecção válida,
  * após estabelecer a conexão entre os objetos seta o vetor de link para aquele rótulo
  * do plano intermediário como inválido (já conectado).
  * Desta forma as conexões são corretamente estabelecidas, e eliminam-se as conexões redundantes.
  * ex:
  * -------*****************----------************--------------------------- plano i
  * -----------------********************-------***************-------------- plano i+1
  *
  * -------*****************----------************--------------------------- plano i
  * -----------------#######----------###-------##--------------------------- plano intermediário (interseções)
  * -----------------********************-------***************-------------- plano i+1
  *
  * Conexões marcadas com | (note que não gera conexões extras).
  * -------*****************----------************--------------------------- plano i
  * -----------------|######----------|##-------|#--------------------------- plano intermediário (interseções)
  * -----------------********************-------***************-------------- plano i+1
 * No modelo M3  a diferença é que acrescenta  o cálculo dos raiosHidraulicos para o plano intermediário,
 * e seta em cada objeto[i]->condutancia = funcao( raioHidraulico plano intermediário );
 * ou seja, o M3 requer que os objetos da rede tenham um vetor condutância.
 * @author : André Duarte Bueno
 * @see    :
 * @param  : Recebe o número do maior rótulo já utilizado
 * @return : void
*/
void
CGrafoConexaoSerial_M3::DeterminarConexoesObjetos ( unsigned long int maiorRotuloUtilizado )
{
   // Variáveis auxiliares
   unsigned long int rpa; // rótulo do píxel[i,j] no plano ra
   unsigned long int rpp; // rótulo do píxel[i,j] no plano rp

   // rotuladorIntermediario_NumeroObjetos
   unsigned long int rotuladorIntermediario_NumeroObjetos;

   long double raioHidraulicoDaInterseccao_Objetos_i_j;

   // Copia dados da conexão das imagens ra e rp para a img2D
   // ou seja gera imagem da intersecção entre os dois planos de rotulagem
   for ( unsigned long int ii  = 0; ii < img2D->NX (); ii++ )
      for ( unsigned long int  jj = 0; jj < img2D->NY (); jj++ )
         if ( ra->data2D[ii][jj] && rp->data2D[ii][jj] )
            img2D->data2D[ii][jj] = 1;
         else
            img2D->data2D[ii][jj] = 0;

   // Rotula a imagem intermediária (img2D) usando rotInt
   rotInt->Go ( img2D );

   // Calculo dos raiosHidraulicos do plano intermediario // Novidade do Modelo 3
   rotInt->CalculaRaioHidraulicoObjetos ();

   // Determina o numero de links (rotuladorIntermediario_NumeroObjetos) + o fundo [0]
   // Ao criar +1 objeto, esta considerando o fundo, de forma que o indice i aponta para o objeto i
   // indice=0 objeto fundo 0
   // indice=1 objeto com rotulo 1....e assim sucessivamente
   rotuladorIntermediario_NumeroObjetos = rotInt->RotuloFinal () + 1;

   // Cria vetor dos links válidos, incializa com valor 1
   // Se ==1 conexão é válida (ainda não foi estabelecida)
   // Se ==0 conexão inválida (já foi usada)
   vector<unsigned long int > links_validos ( rotuladorIntermediario_NumeroObjetos , 1 );

   // Percorre imagem rotulada intermediária
   // se existe intersecção no rotint, seta o link como inválido, e conecta os sítios
   for ( unsigned long int ii = 0; ii < rotInt->NX (); ii++ )
      for ( unsigned long int jj = 0; jj < rotInt->NY (); jj++ )

         // se não for o fundo
         if ( rotInt->data2D[ii][jj] != 0 ) {
               // se for um link válido, entra
               if ( links_validos[rotInt->data2D[ii][jj]] == 1 ) {
                     // e a seguir seta o link como inválido (o que impede repetição do mesmo link)
                     links_validos[rotInt->data2D[ii][jj]] = 0;

                     // Obtem os rótulos
                     rpa = ra->data2D[ii][jj];
                     rpp = rp->data2D[ii][jj];

                     // Acrescenta o deslocamento do maiorRotuloUtilizado
                     rpa += maiorRotuloUtilizado;

                     // Soma o maiorRotuloUtilizado + ra->RotuloFinal()
                     rpp += maiorRotuloUtilizado + ra->RotuloFinal ();

                     // Definição dos raiosHidraulicos das conexões // Novo no Mod 3
                     raioHidraulicoDaInterseccao_Objetos_i_j = rotInt->RaioHidraulicoObjetos ( rotInt->data2D[ii][jj] );

                     // Foi adicionada a classe CObjetoRede uma função conectar que recebe o objeto conectado
                     // e sua condutância.
                     // Adiciona o objeto a lista de links
                     // rpa-1 esta conectado a rpp-1
                     objeto[rpa - 1]->Conectar ( objeto[rpp - 1] , raioHidraulicoDaInterseccao_Objetos_i_j );
                     // rpp-1 esta conectado a rpa-1
                     objeto[rpp - 1]->Conectar ( objeto[rpa - 1] , raioHidraulicoDaInterseccao_Objetos_i_j );
                  }
            }
}

// // -------------------------------------------------------------------------
// // Função:   CalcularCondutancias
// // -------------------------------------------------------------------------
// /** @short : Redefinida, em relação a CGrafoConexaoSerial adiciona o cálculo das condutâncias
//  * das conexões entre os objetos.
//  * No caso do GrafoConexaoSerial todos os objetos são do tipo CObjetoRede_Sitio..
//  * mas o cálculo da condutância usa equação de POISELLE pois a conexão entre planos
//  * se assemelha a conexão dos anéis de uma ligação.
//  * @author : André Duarte Bueno
//  * @see    : Condutância
//  * @param  : nada
//  * @return : void
//  * @todo   : Otimização: Da forma como esta, para cada objeto calcula a condutancia, mas os raiosHidraulicos
//  * de todos os objetos estarão num intervalo raio=1 ->raio=max, poderia criar vetor
//  * vector<double> converteRHcondutancia; onde armazena as condutâncias.
//  * Depois faz objeto[k]->propriedade = converteRHcondutancia[objeto[k]->propriedade];
// */
// void
// CGrafoConexaoSerial_M3::CalcularCondutancias ( long double _viscosidade, long double _dimensaoPixel, unsigned long int _fatorAmplificacao )
// {
//    // Variáveis auxiliares
//    long double raio_hidraulico {0.0};
//    long double dH {0.0};
//    long double dimensaoPixelXfatorAmplificacao = _dimensaoPixel * _fatorAmplificacao;
//    long double variavelAuxiliar = ( CMath::PI ) / ( 128.0 * _viscosidade * _dimensaoPixel * _fatorAmplificacao );
// 
//    // Passo 1: Percorre  todos os objetos do grafo e calcula as condutâncias de cada um
//    // tendo como base o raio hidraulico (armazenado em objeto[k]->propriedade)
//    // armazena resultado, a condutância do objeto em objeto[k]->propriedade
//    // ou seja, converte raio hidraulico em condutância.
//    for ( unsigned long int k = 0; k < objeto.size (); k++ ) {
//          // Converte raio hidraulico de pixel para metro
//          raio_hidraulico = objeto[k]->propriedade * dimensaoPixelXfatorAmplificacao;
// 
//          //->POISELLE
//          // Calcula a condutancia da ligação dH=4.0*(raio_hidraulico);
//          dH = raio_hidraulico + raio_hidraulico + raio_hidraulico + raio_hidraulico;
//          // Converte a propriedade do objeto raio hidraulico para condutancia.
//          objeto[k]->propriedade = ( variavelAuxiliar * dH * dH * dH * dH );
//       }
// 
//    // Passo 2: Percorre  todos os objetos k do grafo e calcula as condutâncias das conexões entre os objetos,
//    // armazena resultado em objeto[k]->condutancia[i]
//    long double ct ;
// 
//    for ( unsigned long int k = 0; k < objeto.size (); k++ ) {
//          for ( unsigned int conexao_i = 0; conexao_i < objeto[k]->conexao.size (); conexao_i++ ) {
//                raio_hidraulico = objeto[k]->condutancia[conexao_i];
//                // Converte para SI (metros)
//                raio_hidraulico *= sizePixelXfatorAmplificacao;
//                // Determina o diâmetro hidraulico como sendo 4*raioHidraulico
//                dH = raio_hidraulico + raio_hidraulico + raio_hidraulico + raio_hidraulico;
//                // Calcula a condutância da ligação e armazena
//                objeto[k]->condutancia[conexao_i] = ( variavelAuxiliar * dH * dH * dH * dH );
//             }
//       }
//    return;
// }

// Abaixo nova versão; possibilita calcular as condutâncias considerando os diferentes tipos de objetos
void
CGrafoConexaoSerial_M3::CalcularCondutancias ( long double _viscosidade, long double _dimensaoPixel, unsigned long int _fatorAmplificacao )
{
   // Variáveis auxiliares
   long double raio_hidraulico {0.0};
   long double dH {0.0};
   long double dimensaoPixelXfatorAmplificacao = _dimensaoPixel * _fatorAmplificacao;
   long double variavelAuxiliar = ( CMath::PI ) / ( 128.0 * _viscosidade * _dimensaoPixel * _fatorAmplificacao );

   // Passo 1: Percorre  todos os objetos do grafo e calcula as condutâncias de cada um
   // tendo como base o raio hidraulico (armazenado em objeto[k]->propriedade)
   // armazena resultado, a condutância do objeto em objeto[k]->propriedade
   // ou seja, converte raio hidraulico em condutância.
   for ( unsigned long int k = 0; k < objeto.size (); k++ ) {
         // Converte raio hidraulico de pixel para metro
         raio_hidraulico = objeto[k]->propriedade * dimensaoPixelXfatorAmplificacao;

         // CALCULO DA CONDUTANCIA DOs OBJETOs do Tipo LIGACAO ->POISELLE
         if ( objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoRede_Ligacao
               or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoRede_Ligacao_CENTER
               or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoRede_Ligacao_EST
               or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoRede_Ligacao_WEST
               or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Ligacao
               or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Ligacao_CENTER
               or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Ligacao_EST
               or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Ligacao_WEST ) {
               //->POISELLE
               // Calcula a condutancia da ligação dH=4.0*(raio_hidraulico);
               dH = raio_hidraulico + raio_hidraulico + raio_hidraulico + raio_hidraulico;
               // Converte a propriedade do objeto raio hidraulico para condutancia.
               objeto[k]->propriedade = ( variavelAuxiliar * dH * dH * dH * dH );
            }
         // CALCULO DA CONDUTANCIA DOs OBJETOs do tipo  SITIO->KOPLIK
         else if ( objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoRede_Sitio
                   or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoRede_Sitio_CENTER
                   or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoRede_Sitio_EST
                   or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoRede_Sitio_WEST
                   or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Sitio
                   or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Sitio_CENTER
                   or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Sitio_EST
                   or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Sitio_WEST ) {
/// @todo: Trocar as duas linhas abaixo pela equação de KOPLIK
               // Calcula a condutancia da ligação dH=4.0*(raio_hidraulico);
               dH = raio_hidraulico + raio_hidraulico + raio_hidraulico + raio_hidraulico;
               // Converte a propriedade do objeto raio hidraulico para condutancia.
               objeto[k]->propriedade = ( variavelAuxiliar * dH * dH * dH * dH );
            }
         else	   {
               cerr << "\nErro: Tipo de objeto desconhecido em CGrafoConexaoSerial_M3::CalcularCondutancias.";
            }
      }

   // Passo 2: Percorre  todos os objetos k do grafo e calcula as condutâncias das conexões entre os objetos,
   // armazena resultado em objeto[k]->condutancia[i]
   long double ct ;

   for ( unsigned long int k = 0; k < objeto.size (); k++ ) {
         // Percorre todas as conexões do objeto
         // objeto[k]->propriedade = condutancia objeto
         // objeto[k]->condutancia[link] = condutancia entre objetos k e link
         // ct = 1.0 / (1.0/c1 + 1.0/c2); -> o mesmo que -> ct = c1*c2/(c1+c2);
         // como preciso calcular no centro e não nas bordas
         // ct' = 2*c1*c2/(c1+c2)
         // @todo: o cálculo abaixo pode incluir um if..else para os tipos de objetos, ou
         // CalcularCondutancias pode ser reescrito de acordo com tipo de grafo.
//          for ( unsigned int link = 0; link < objeto[k]->conexao.size (); link++ ) {
//                ct = objeto[k]->propriedade * objeto[k]->conexao[link]->propriedade /
//                     ( objeto[k]->propriedade + objeto[k]->conexao[link]->propriedade );
//                objeto[k]->condutancia[link] = ct;
//             }
         for ( unsigned int conexao_i = 0; conexao_i < objeto[k]->conexao.size (); conexao_i++ ) {
               raio_hidraulico = objeto[k]->condutancia[conexao_i];
               // Converte para SI (metros)
               raio_hidraulico *= dimensaoPixelXfatorAmplificacao;
               // Determina o diâmetro hidraulico como sendo 4*raioHidraulico
               dH = raio_hidraulico + raio_hidraulico + raio_hidraulico + raio_hidraulico;
               // Calcula a condutância da ligação e armazena
               objeto[k]->condutancia[conexao_i] = ( variavelAuxiliar * dH * dH * dH * dH );
            }
      }

   return;
}

// // -------------------------------------------------------------------------
// // Função:               SetarMatrizAVetorB
// // -------------------------------------------------------------------------
// /** @short  : Recebe uma matriz A (vazia) e um vetor B (vazio)
//  * e preenche os mesmos com os coeficientes necessários para determinação do sistema de equações.
//  * 1- O grafo já deve ter sido determinado
//  * 2- Os valores iniciais de pressão já devem ter sido definidos
//  * (valores de contorno, normalmente Plano_0 = 1, Plano_n = 0)
//  * 3- Deve receber uma matriz e um vetor vazios
//  * @author :	André Duarte Bueno
//  * @see    : grafos
//  * @param  :
//  * @return :	bool indicando sucesso da operação.
// */
//
// bool
// CGrafoConexaoSerial_M3::SetarMatrizAVetorB ( TCMatriz2D< int >*& A, CVetor*& B )  const
// {
//    // vector< vector<float> > A;
//    // vector<float> B;
//
//    // Passo 0: Definição de variáveis auxiliares
//    // índice i da matriz A (ou vetor B)
//    unsigned long int mi;
//
//    // índice j da matriz A
//    unsigned long int mj;
//
//    // Condutância total Cij = (Cii+Cjj)/2 para o modelo 2
//    long double Cij;
//
//    // Passo 1:
//    // Determinação da dimensão da matriz e do vetor
//    // cout << "\nrotuloPrimeiroObjetoPlanoN="               <<       rotuloPrimeiroObjetoPlanoN;
//    // cout << "\nrotuloPrimeiroObjetoPlano1="      <<  rotuloPrimeiroObjetoPlano1;
//    unsigned int dim = rotuloPrimeiroObjetoPlanoN - rotuloPrimeiroObjetoPlano1 + 1;
//    // cout <<"\ndim="<<dim;
//
//    // Redimensiona a matriz A
//    A->Redimensiona ( dim, dim );
//    // Zera a matriz A
//    A->Constante ( 0 );
//
//    // Redimensiona o vetor B
//    B->Redimensiona ( dim );
//    // Zera o vetor B
//    B->Constante ( 0 );
//
//    unsigned int i;
//
//    for ( unsigned long int j = 0; j < objeto.size (); j++ ) {
// // Faz um cast para sítio derivado (em função do acesso a função Contorno e vetor conexao.
// //CObjetoRede_CC_Sitio* objeto_j = dynamic_cast < CObjetoRede_CC_Sitio* > ( objeto[j] );
// //assert ( objeto_j );
//
//          switch ( objeto[j]->Contorno () ) {
//                // Fronteira esquerda
//             case CContorno::ETipoContorno::WEST:
//
//                // Fronteira direira
//             case CContorno::ETipoContorno::EST:
//
//                // Percorre as conexões do objeto
//                for ( i = 0; i < objeto[j]->conexao.size (); i++ ) {
//                      // Calcula Cij
//                      Cij = objeto[j]->condutancia[i];
//                      Cij = Cij * 1.0e17;	// LIXO, para gerar int
//                      // cij esta sendo armazenado em int por isto multiplico por e17
//
//                      // Desloca o índice da matriz(vetor), pois só entram os sítios que não estão na interface.
//                      mi = objeto[j]->conexao[i]->rotulo - rotuloPrimeiroObjetoPlano1;	// 3;
//
//                      // Acumula Cij no vetor B[mi] -= Cij     * objeto[j]->x, x deve estar definido
//                      // B->data1D[ mi ] -= Cij * objeto[j]->x;
//                      B->data1D[mi] -= static_cast < int > ( Cij * objeto[j]->x );	// LIXO o static
//
//                      // Acumula -Cij na matriz A[mi][mi]
//                      // A->data2D[mi][mi] -= Cij;
//                      A->data2D[mi][mi] -= static_cast < int > ( Cij );	// LIXO o static
//                   }
//
//                break;
//
//                // Fronteira Centro
//             case CContorno::ETipoContorno::CENTER:
//
//                // Percorre as conexões do objeto
//                for ( i = 0; i < objeto[j]->conexao.size (); i++ ) {
//                      // Se o link  for  um objeto de centro (não contorno) entra
//                      if ( objeto[j]->conexao[i]->Contorno () == CContorno::ETipoContorno::CENTER ) {
//                            // Calcula Cij
//                            // Cij = ( objeto[j]->propriedade + objeto[j]->conexao[i]->propriedade ) /2.0 ;
//                            Cij = objeto[j]->condutancia[i];
//                            Cij = Cij * 1.0e17;	// LIXO para gerar int
//                            // cij esta sendo armazenado em int por isto multiplico por e17
//
//                            // Desloca os índices da matriz
//                            mi = objeto[j]->conexao[i]->rotulo - rotuloPrimeiroObjetoPlano1;
//                            mj = objeto[j]->rotulo - rotuloPrimeiroObjetoPlano1;
//
//                            // Define A->data2D[mi][mj]
//                            A->data2D[mi][mj] = static_cast < int > ( Cij );	// LIXO o static
//
//                            // Acumula A->data2D[mj][mj]
//                            A->data2D[mj][mj] -= static_cast < int > ( Cij );	// LIXO o static
//                         }
//                   }
//
//                break;
//             }			// switch
//       }				// for
//
//    A->Write ( "grafo.matrixA" );
//    B->Write ( "grafo.vectorB" );
//    return 1;
// }
