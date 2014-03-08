/**
===============================================================================
@PROJETO:          Biblioteca LIB_LDSC
            Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
@Desenvolvido_por:	Laboratorio de Desenvolvimento de Software Cientifico
[LDSC].
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

// #ifndef CObjetoRede_CC_Sitio_h
// #include <EstruturaDados/ObjetoRede/CObjetoRede_CC_Sitio.h>
// #endif

// #ifndef CObjetoRede_CC_Sitio_WEST_h
// #include <EstruturaDados/ObjetoRede/CObjetoRede_CC_Sitio_WEST.h>
// #endif
// 
// #ifndef CObjetoRede_CC_Sitio_EST_h
// #include <EstruturaDados/ObjetoRede/CObjetoRede_CC_Sitio_EST.h>
// #endif

#ifndef CMath_h
>>>>>>> /bad-path/
#include <Base/CMath.h>
using namespace std;

// -------------------------------------------------------------------------
// Função:       CalcularPropriedadesConeccoes
// -------------------------------------------------------------------------
/** @short: A conexão entre os sítios já foi estabelecida em DeterminarConeccoesObjetos.
 *   Aqui faço o calculo de uma propriedade relacionada a conexão entre os dois sítios.
 *  Como sou herdeiro da CGrafoConexaoSerial_M2 tenho o rotulador 2D rotint ativo, e
 *  já rotulado, podendo ser diretamente utilizado.
 *  Ex:
 *  Para o estudo da permeabilidade, determino a condutância da ligação.
 * @author:  André Duarte Bueno
 * @param	:	recebe um vetor de  CObjetoRede*
 * @return : void
*/

/*
void CGrafoConexaoSerial_M3::CalcularPropriedadesConeccoes()
{
// Percorre todos os objetos do plano atual.
// objeto.size() retorna o numero de objetos do grafo
// objeto.size();

// Para cada objeto do sítio, aloca o vetor condutância
// com o mesmo número de dados do vetor conexao
// objeto[i]->Coneccao.size() retorna o número de conexões do objeto i
 unsigned long int numeroDeLinks = objeto[i]->Coneccao.size();
objeto[i]->condutâncias.reserve( numeroDeLinks );
};
*/

// -------------------------------------------------------------------------
// Função:       DeterminarConeccoesObjetos
// -------------------------------------------------------------------------
/** @short : É a mesma função da classe base, a diferença é
 *     o calculo dos raiosHidraulicos para o plano intermediário
 *     e a setagem em cada objeto do valor do raioHidraulico da conexão.
 * @author :	André Duarte Bueno
 * @see    :
 * @param  : Recebe o número do maior rótulo já utilizado
 * @return : void
*/
void
CGrafoConexaoSerial_M3::DeterminarConeccoesObjetos ( unsigned long int maiorRotuloUtilizado )
{
   // Variáveis auxiliares
   unsigned long int pa, pp;

   // numeroLinksimgIntermediaria
   unsigned long int nli;

   long double raioHidraulicoLigacoes;
//CObjetoRede_CC_Sitio* ptrSitio;
   
   // Copia dados da conexão das duas imagens para a img2D
   // ou seja gera imagem da intersecção entre os dois planos de rotulagem
   unsigned long int ii;
   unsigned long int jj;

   for ( ii = 0; ii < img2D->NX (); ii++ )
      for ( jj = 0; jj < img2D->NY (); jj++ )
         if ( ra->data2D[ii][jj] && rp->data2D[ii][jj] )
            img2D->data2D[ii][jj] = 1;
         else
            img2D->data2D[ii][jj] = 0;

   // Rotula a imagem intermediária (img2D) usando rotInt
   rotInt->Go ( img2D );

   // Calculo dos raiosHidraulicos do plano intermediario
   rotInt->CalculaRaioHidraulicoObjetos ();

   // Determina o numero de links (nli) + o fundo [0]
   // Ao criar +1 objeto, esta considerando o fundo, de forma que
   // o indice i aponta para o objeto i
   // indice=0 objeto fundo 0
   // indice=1 objeto com rotulo 1....e assim sucessivamente
   nli = rotInt->RotuloFinal () + 1;

   // Cria lista dos links válidos, se =0 não validado, se =1 validado
   CVetor* links_validos = new CVetor ( nli );	// usar vector
   assert ( links_validos );

   // Seta todos os links como válidos
   links_validos->Constante ( 1 );

   // Percorre imagem rotulada intermediária
   // se conectada, seta o link como válido, e conecta os sítios
   for ( ii = 0; ii < rotInt->NX (); ii++ )
      for ( jj = 0; jj < rotInt->NY (); jj++ )

         // se não for o fundo
         if ( rotInt->data2D[ii][jj] != 0 ) {
               // se for um link válido, entra
               if ( links_validos->data1D[rotInt->data2D[ii][jj]] == 1 ) {
                     // Abaixo, seta o link como inválido (o que impede repetição do mesmo link)
                     links_validos->data1D[rotInt->data2D[ii][jj]] = 0;

                     // Obtem os rótulos
                     pa = ra->data2D[ii][jj];
                     pp = rp->data2D[ii][jj];

                     // Acrescenta o deslocamento do maiorRotuloUtilizado
                     pa += maiorRotuloUtilizado;

                     // Soma o maiorRotuloUtilizado + ra->RotuloFinal()
                     pp += maiorRotuloUtilizado + ra->RotuloFinal ();

// ANTIGO ANTIGO	INICIO
//                      // Adiciona o objeto a lista de links
//                      // pa-1 esta conectado a pp-1
//                      objeto[pa - 1]->Conectar ( objeto[pp - 1] );
//                      // pp-1 esta conectado a pa-1
//                      objeto[pp - 1]->Conectar ( objeto[pa - 1] );
// 
//                      // Definição dos raiosHidraulicos das conexões
//                      raioHidraulicoLigacoes =
//                         rotInt->RaioHidraulicoObjetos ( rotInt->data2D[ii][jj] );
//                      ptrSitio = dynamic_cast < CObjetoRede_CC_Sitio* > ( objeto[pa - 1] );
//                      ptrSitio->condutancia.push_back ( raioHidraulicoLigacoes );
//                      ptrSitio = dynamic_cast < CObjetoRede_CC_Sitio* > ( objeto[pp - 1] );
//                      ptrSitio->condutancia.push_back ( raioHidraulicoLigacoes );
// ANTIGO ANTIGO	FIM
// NOVO INICIO		
                     // Definição dos raiosHidraulicos das conexões
                     raioHidraulicoLigacoes =
                        rotInt->RaioHidraulicoObjetos ( rotInt->data2D[ii][jj] );
					 // Foi adicionada a classe CObjetoRede uma função conectar 
					 // que recebe o objetoconectado e sua condutância
                     // Para objeto [pa-1], que esta conectado a pp-1
					 /// @todo : Testar!
                     // Adiciona o objeto a lista de links
                     // pa-1 esta conectado a pp-1
                     objeto[pa - 1]->Conectar ( objeto[pp - 1] , raioHidraulicoLigacoes);
                     // pp-1 esta conectado a pa-1
                     objeto[pp - 1]->Conectar ( objeto[pa - 1] , raioHidraulicoLigacoes);
// NOVO FIM					 
                  }
            }

   delete links_validos;
}

// -------------------------------------------------------------------------
// Função:   CalcularCondutancias
// -------------------------------------------------------------------------
/** @short : Redefinida, em relação a CGrafo adiciona o calculo das condutâncias das ligações.
 * @author : André Duarte Bueno
 * @see    : Condutância
 * @param  : nada
 * @return : void
 * @todo   : Da forma como esta, para cada objeto calcula a condutancia, mas os raiosHidraulicos
 * de todos os objetos estarão num intervalo raio=1 ->raio=max, poderia criar vetor
 * vector<double> converteRHcondutancia; onde armazena as condutâncias.
 * Depois faz objeto[k]->propriedade = converteRHcondutancia[objeto[k]->propriedade];
 *
*/
void
CGrafoConexaoSerial_M3::CalcularCondutancias ( long double _viscosidade, long double _dimensaoPixel, unsigned long int _fatorAmplificacao )
{
   // Variáveis auxiliares
   long double raio_hidraulico;
   long double dH;

   long double dimensaoPixelXfatorAmplificacao = _dimensaoPixel * _fatorAmplificacao;
   // long double PI=3.141592653589;
   long double variavelAuxiliar = ( CMath::PI ) / ( 128.0 * _viscosidade * _dimensaoPixel * _fatorAmplificacao );

//    A classe CGrafoConexaoSerial_M3 tem sítios do tipo CObjetoRede_CC_Sitio
//    que tem uma lista de conexões.
//    aqui usa um ponteiro do tipo CObjetoRede_CC_Sitio para acessar as conexões.
//    CObjetoRede_CC_Sitio* ptrSitioLR = nullptr;

   // Passo 1: Percorre  todos os objetos do grafo e calcula as condutâncias de cada um
   // armazena resultado em objeto[k]->propriedade
   for ( unsigned long int k = 0; k < objeto.size (); k++ ) {
         // Converte raio hidraulico de pixel para metro
         raio_hidraulico = objeto[k]->propriedade * dimensaoPixelXfatorAmplificacao;

         // CALCULO DA CONDUTANCIA DO OBJETO LIGACAO->POISELLE
         if ( objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoRede_Ligacao
               or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoRede_Ligacao_EST
               or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoRede_Ligacao_WEST
               or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Ligacao
               or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Ligacao_EST
               or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Ligacao_EST ) {
               // Calcula a condutancia da ligação dH=4.0*(raio_hidraulico);
               dH = raio_hidraulico + raio_hidraulico + raio_hidraulico + raio_hidraulico;
               // Converte a propriedade do objeto raio hidraulico para condutancia.
               objeto[k]->propriedade = ( variavelAuxiliar * dH * dH * dH * dH );
            }
         // CALCULO DA CONDUTANCIA DO OBJETO SITIO->KOPLIK
         else if ( objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoRede_Ligacao
                   or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoRede_Ligacao_EST
                   or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoRede_Ligacao_WEST
                   or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Ligacao
                   or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Ligacao_EST
                   or objeto[k]->Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Ligacao_EST ) {
               /// @todo: Trocar as duas linhas abaixo pela equação de KOPLIK
               // Calcula a condutancia da ligação dH=4.0*(raio_hidraulico);
               dH = raio_hidraulico + raio_hidraulico + raio_hidraulico + raio_hidraulico;
               // Converte a propriedade do objeto raio hidraulico para condutancia.
               objeto[k]->propriedade = ( variavelAuxiliar * dH * dH * dH * dH );
            }
         else	   {
               cerr << "\nErro: Tipo de objeto desconhecido em CGrafoConexaoSerial_M3::CalcularCondutancias.";
               exit ( 0 );
            }
      }

   // Passo 2: Percorre  todos os objetos k do grafo e calcula as condutâncias das conexões entre os objetos,
   // armazena resultado em objeto[k]->condutancia[i]
   long double ct ;

   for ( unsigned long int k = 0; k < objeto.size (); k++ ) {
         // Percorre todas as conexões do objeto
         // objeto[k]->propriedade = condutancia objeto
         // objeto[k]->conexao[link]->propriedade = condutancia objeto conectado
         // ct = 1.0 / (1.0/c1 + 1.0/c2); -> o mesmo que -> ct = c1*c2/(c1+c2);
	     // como preciso calcular no centro e não nas bordas
	     // ct' = 2*c1*c2/(c1+c2)
	   // @todo: o calculo abaixo pode incluir um if..else para os tipos de objetos, ou 
	   // CalcularCondutancias pode ser reescrito de acordo com tipo de grafo.
         for ( unsigned int link = 0; link < objeto[k]->conexao.size (); link++ ) {
               ct = objeto[k]->propriedade * objeto[k]->conexao[link]->propriedade /
                    ( objeto[k]->propriedade + objeto[k]->conexao[link]->propriedade );
               objeto[k]->condutancia[link] = ct;
            }
      }

   return;
}

// -------------------------------------------------------------------------
// Função:   EliminarCondutanciasRepetidas
// -------------------------------------------------------------------------
/** @short  : Redefinida, em relação a CGrafo3Dby2D.
 * Percorre todos os objetos e elimina conexões repetidas (deve ser chamada depois do
 * calculo das condutâncias, pois soma as condutâncias das conexões repetidas.
 * se chamar antes, vai acumular o raio hidraulico)
 * Chama função de cada sítio, que elimina links repetidos.
 * @author :	André Duarte Bueno
 * @see    : Condutância
 * @param  : nada
 * @return : void
*/
void
CGrafoConexaoSerial_M3::EliminarCondutanciasRepetidas ()
{
   int totalLinksDeletados = 0;

   // Percorre todos os objetos do grafo
   if ( EliminarConeccoesRepetidas () == 1 )
      for ( int i = 0; i < objeto.size (); i++ ) {
// Cria ponteiro para objeto do tipo CObjetoRede_CC_Sitio
// CObjetoRede_CC_Sitio* obj_i = dynamic_cast < CObjetoRede_CC_Sitio* > ( objeto[i] );
// assert ( obj_i );

            // Chama DeletarConexoesRepetidas_e_SomarCondutanciasParalelo, que retorna o número de links eliminados
//            totalLinksDeletados += obj_i->DeletarConexoesRepetidas_e_SomarCondutanciasParalelo ();
            totalLinksDeletados += objeto[i]->DeletarConexoesRepetidas_e_SomarCondutanciasParalelo ();
         }

   // Write( "Grafo_DeletarConexoesRepetidas_e_SomarCondutanciasParalelo.txt");        Emedio
   cerr << "NumeroTotal de links deletados= " << totalLinksDeletados << endl;
}

// // -------------------------------------------------------------------------
// // Função:       CriarObjetoGrafo
// // -------------------------------------------------------------------------
// /** @short  : 	Cria objeto herdeiro de CObjetoRede, de acordo com o tipo solicitado.
//  * @author :	André Duarte Bueno
//  * @see    :	grafos
//  * @param  :       Inteiro identificando o tipo de sítio a ser criado.
//  *       Formato andre: esquerda(0) centro(1) direita(2)
//  *       Formato Liang: esquerda(1) centro(0) direita(2)
//  * @return : 	Retorna um ponteiro para um sítio novo alocado.
// */
// CObjetoRede*
// CGrafoConexaoSerial_M3::CriarObjetoGrafo ( CContorno::ETipoContorno tipoContorno )
// {
//    CObjetoRede* data;
// 
//    switch ( tipoContorno ) {
//       case CContorno::ETipoContorno::CENTER:
//          data = new CObjetoRede_CC_Sitio ();
//          break;
// 
//       case CContorno::ETipoContorno::WEST:
//          data = new CObjetoRede_CC_Sitio_WEST ();
//          break;
// 
//       case CContorno::ETipoContorno::EST:
//          data = new CObjetoRede_CC_Sitio_EST ();
//          break;
// 
//       default:
//          data = new CObjetoRede_CC_Sitio ();
//          break;
//       }
//    assert ( data );
//    return data;
// }

// -------------------------------------------------------------------------
// Função:               SetarMatrizAVetorB
// -------------------------------------------------------------------------
/** @short  : Recebe uma matriz A (vazia) e um vetor B (vazio)
 * e preenche os mesmos com os coeficientes necessários para determinação do sistema de equações.
 * 1- O grafo já deve ter sido determinado
 * 2- Os valores iniciais de pressão já devem ter sido definidos
 * (valores de contorno, normalmente Plano_0 = 1, Plano_n = 0)
 * 3- Deve receber uma matriz e um vetor vazios
 * @author :	André Duarte Bueno
 * @see    : grafos
 * @param  :
 * @return :	bool indicando sucesso da operação.
*/

bool
CGrafoConexaoSerial_M3::SetarMatrizAVetorB ( TCMatriz2D< int >*& A, CVetor*& B )  const
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
   // cout << "\nrotuloUltimoObjetoPlanoN_1="               <<       rotuloUltimoObjetoPlanoN_1;
   // cout << "\nrotuloPrimeiroObjetoPlano1="      <<  rotuloPrimeiroObjetoPlano1;
   unsigned int dim = rotuloUltimoObjetoPlanoN_1 - rotuloPrimeiroObjetoPlano1 + 1;
   // cout <<"\ndim="<<dim;

   // Redimensiona a matriz A
   A->Redimensiona ( dim, dim );
   // Zera a matriz A
   A->Constante ( 0 );

   // Redimensiona o vetor B
   B->Redimensiona ( dim );
   // Zera o vetor B
   B->Constante ( 0 );

   unsigned int i;

   for ( unsigned long int j = 0; j < objeto.size (); j++ ) {
// Faz um cast para sítio derivado (em função do acesso a função Contorno e vetor conexao.
//CObjetoRede_CC_Sitio* objeto_j = dynamic_cast < CObjetoRede_CC_Sitio* > ( objeto[j] );
//assert ( objeto_j );

         switch ( objeto[j]->Contorno () ) {
               // Fronteira esquerda
            case CContorno::ETipoContorno::WEST:

               // Fronteira direira
            case CContorno::ETipoContorno::EST:

               // Percorre as conexões do objeto
               for ( i = 0; i < objeto[j]->conexao.size (); i++ ) {
                     // Calcula Cij
                     Cij = objeto[j]->condutancia[i];
                     Cij = Cij * 1.0e17;	// LIXO, para gerar int
                     // cij esta sendo armazenado em int por isto multiplico por e17

                     // Desloca o índice da matriz(vetor), pois só entram os sítios que não estão na interface.
                     mi = objeto[j]->conexao[i]->rotulo - rotuloPrimeiroObjetoPlano1;	// 3;

                     // Acumula Cij no vetor B[mi] -= Cij     * objeto[j]->x, x deve estar definido
                     // B->data1D[ mi ] -= Cij * objeto[j]->x;
                     B->data1D[mi] -= static_cast < int > ( Cij * objeto[j]->x );	// LIXO o static

                     // Acumula -Cij na matriz A[mi][mi]
                     // A->data2D[mi][mi] -= Cij;
                     A->data2D[mi][mi] -= static_cast < int > ( Cij );	// LIXO o static
                  }

               break;

               // Fronteira Centro
            case CContorno::ETipoContorno::CENTER:

               // Percorre as conexões do objeto
               for ( i = 0; i < objeto[j]->conexao.size (); i++ ) {
                     // Se o link  for  um objeto de centro (não contorno) entra
                     if ( objeto[j]->conexao[i]->Contorno () == CContorno::ETipoContorno::CENTER ) {
                           // Calcula Cij
                           // Cij = ( objeto[j]->propriedade + objeto[j]->conexao[i]->propriedade ) /2.0 ;
                           Cij = objeto[j]->condutancia[i];
                           Cij = Cij * 1.0e17;	// LIXO para gerar int
                           // cij esta sendo armazenado em int por isto multiplico por e17

                           // Desloca os índices da matriz
                           mi = objeto[j]->conexao[i]->rotulo - rotuloPrimeiroObjetoPlano1;
                           mj = objeto[j]->rotulo - rotuloPrimeiroObjetoPlano1;

                           // Define A->data2D[mi][mj]
                           A->data2D[mi][mj] = static_cast < int > ( Cij );	// LIXO o static

                           // Acumula A->data2D[mj][mj]
                           A->data2D[mj][mj] -= static_cast < int > ( Cij );	// LIXO o static
                        }
                  }

               break;
            }			// switch
      }				// for

   A->Write ( "grafo.matrixA" );
   B->Write ( "grafo.vectorB" );
   return 1;
}
