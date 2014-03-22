/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
				  Laboratorio de Desenvolvimento de Software Cientifico  [LDSC].
@author:          André Duarte Bueno
@file:            CGrafoConexaoSerial.cpp
@begin:           Sat Sep 16 2000
@copyright:       (C) 2000 by André Duarte Bueno   CTime
@email:           andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <cassert>
#include <fstream>
#include <list>
#include <deque>
#include <algorithm>
#include <iterator>
using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#ifndef CGrafoConexaoSerial_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGrafoConexaoSerial.h>
#endif

// #ifndef CObjetoRede_h
// #include <EstruturaDados/ObjetoRede/CObjetoRede.h>
// #endif

#ifndef CObjetoRede_Sitio_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Sitio.h>
#endif

#ifndef CObjetoRede_Tipo_h
#include <EstruturaDados/ObjetoRede/CObjetoRede_Tipo.h>
#endif

#ifndef CContorno_h
#include <Contorno/CContorno.h>
#endif

#ifndef CTime_h
#include <Tempo/CTime.h>
#endif

// -------------------------------------------------------------------------
// Função     Go
// -------------------------------------------------------------------------
/** Determina o grafo lendo os planos da imagem diretamente do disco.
O atributo plano é utilizado para armazenar no objeto criado, temporariamente,
a informação do plano a que pertence.
O atributo plano será usado para estimação da pressão inicial.
*/
CRede* CGrafoConexaoSerial::Go ( string nomeArquivoImagem, unsigned long int /*naoUsado*/ )
{
	ifstream fin ( nomeArquivoImagem.c_str () );

	if ( fin.fail() ) {
         cerr << "Nao conseguiu abrir o arquivo " << nomeArquivoImagem.c_str ();
         return 0;
      }

   // Leitura do cabecalho do arquivo de disco
   char d3[55]; //?
   /*unsigned*/ int numeroCores;
   /*unsigned*/ int valor;
   fin >> d3 >> nx >> ny >> nz >> numeroCores;

   // Criação dos objetos ra,rp img2D
   if ( ra ) delete ra;

   ra = nullptr;
   ra = new CRotulador2DCm ( nx, ny );
   assert ( ra );

   if ( rp ) delete rp;

   rp = nullptr;
   rp = new CRotulador2DCm ( nx, ny );
   assert ( rp );

   if ( img2D ) delete img2D;

   img2D = nullptr;
   img2D = new TCMatriz2D< int > ( nx, ny );
   assert ( img2D );

   // ------------
   // PLANO 0
   // ------------
   maiorRotuloUtilizado = 0;
   /*unsigned long */int i, j, k;
   // Le dados do plano 0
   plano = k = 0;
   cout << "\n plano = " << plano << flush;

   for ( j = 0; j < ny; j++ )
      for ( i = 0; i < nx; i++ ) {
            fin >> img2D->data2D[i][j] ;
         }

   // Rotula plano 0
   ra->Go ( img2D );

   // Determina raio hidraulico dos objetos
   ra->CalculaRaioHidraulicoObjetos ();

   // Define o contorno como sendo da face WEST (plano 0)
   ETipoObjetoGrafo tipoObjeto = ETipoObjetoGrafo::ObjetoRede_Sitio_WEST;

   // Adciona ao grafo os objetos do plano 0 (face WEST)
   AdicionarObjetos ( ra, maiorRotuloUtilizado, tipoObjeto );

   // Determina o rótulo do primeiro objeto a ser usado pelo solver.
   /// @todo: verificar retorno de ra->RotuloFinal() = último rótulo utilizado
   /// se for último rótulo utilizado precisa somar 1
   rotuloPrimeiroObjetoPlano1 = maiorRotuloUtilizado + ra->RotuloFinal (); // +1

   // ------------
   // PLANO i
   // ------------
   tipoObjeto = ETipoObjetoGrafo::ObjetoRede_Sitio;//_CENTER;

   // Le dados do plano ij
   for ( k = 1; k < nz; k++ ) {
         plano = k;
         cout << "\rPlano  " << plano << flush;

         for ( j = 0; j < ny; j++ )
            for ( i = 0; i < nx; i++ ) {
//             fin >> valor;
//             img2D->data2D[i][j] = valor;
                  fin >> img2D->data2D[i][j] ;
               }

         // Rotula plano k
         rp->Go ( img2D );

         // Determina raio hidraulico
         rp->CalculaRaioHidraulicoObjetos ();

         // Se for o ultimo plano, redefine o contorno e calcula rotuloPrimeiroObjetoPlanoN
         if ( k == ( nz - 1 ) ) {
               tipoObjeto = ETipoObjetoGrafo::ObjetoRede_Sitio_EST;
               rotuloPrimeiroObjetoPlanoN = maiorRotuloUtilizado + ra->RotuloFinal (); //+1
            }

         // Adiciona objetos do plano ij atual
         AdicionarObjetos ( rp, maiorRotuloUtilizado + ra->RotuloFinal (), tipoObjeto );

         // Estabelece os links entre os objetos
         DeterminarConexoesObjetos ( maiorRotuloUtilizado );

         // Determina o maior rotulo utilizado
         maiorRotuloUtilizado = maiorRotuloUtilizado + ra->RotuloFinal ();

         // Reordena os ponteiros para os planos de rotulagem
         swap ( ra, rp );
      }

   // Fecha o arquivo de disco
   fin.close ();

   cout << endl;
   // Elimina objetos com 0 ou 1 conexão
   {
      CTime ( "Tempo processamento função EliminarRamosMortos = ", &cout );
      EliminarRamosMortos ();
   }

   return this;
}

// -------------------------------------------------------------------------
// Função     Go
// -------------------------------------------------------------------------
/** @short  :Cria os objetos de rotulagem ra,rp,
  Cria a imagem 2D auxiliar,
  Copia o plano 3D para a imagem 2D,
  Realiza a rotulagem,
  AdicionarObjetos,
  DeterminarConexoesObjetos,
  EliminarRamosMortos.
@author :	André Duarte Bueno
@param  :	Uma matriz 3D e um identificador
@return :	this
*/
CRede* CGrafoConexaoSerial::Go ( TCMatriz3D<int>* _img3D, unsigned long int /*naoUsado*/ )
{
   // Armazena a informação das dimensoes da imagem
   nx = _img3D->NX ();
   ny = _img3D->NY ();
   nz = _img3D->NZ ();

   // Criação dos objetos ra,rp img2D
   // Cria 2 objetos de rotulagem 2D
   // Rotulador para imagem anterior
   if ( ra ) delete ra;

   ra = nullptr;
   ra = new CRotulador2DCm ( nx, ny );
   assert ( ra );

   // Rotulador para imagem posterior
   if ( rp ) delete rp;

   rp = nullptr;
   rp = new CRotulador2DCm ( nx, ny );
   assert ( rp );

   // Matriz imagem bidimensional passada para rotulador->Go(matriz)
   // Contém uma copia do plano3D a ser analisado
   if ( img2D ) delete img2D;

   img2D = nullptr;
   img2D = new TCMatriz2D< int > ( nx, ny );
   assert ( img2D );

   // ------------
   // PLANO 0
   // ------------
   // Usados para percorrer  a imagem
   /*unsigned long*/ int i, j, k;
   maiorRotuloUtilizado = 0;
   plano = k = 0;
   cout << "\nplano " << plano << " " << flush;

   for ( i = 0; i < nx; i++ )
      for ( j = 0; j < ny; j++ )
         img2D->data2D[i][j] = _img3D->data3D[i][j][k];

   // Rotula plano 0
   ra->Go ( img2D );

   // Calcula os vetores area, perimetro e raio hidraulico do plano 0
   ra->CalculaRaioHidraulicoObjetos ();

   // Define o contorno como sendo da face WEST (plano 0)
   ETipoObjetoGrafo tipoObjeto = ETipoObjetoGrafo::ObjetoRede_Sitio_WEST;

   // Adiciona objetos no grafo
   AdicionarObjetos ( ra, maiorRotuloUtilizado, tipoObjeto );

   // Determina o primeiro objeto a ser usado pelo solver
   rotuloPrimeiroObjetoPlano1 = maiorRotuloUtilizado + ra->RotuloFinal ();	// +1!!!! aqui falta +1

   // ----------------------------------------------------------------------------
   // Percorre todos os demais planos
   // ----------------------------------------------------------------------------
   // Define o tipo de contorno como sendo de objeto central
   // (não é contorno esquerdo/WEST nem direito/EST)
   tipoObjeto = ETipoObjetoGrafo::ObjetoRede_Sitio;//_CENTER;

   // k é o indice da direcao z
   for ( k = 1; k < nz; k++ ) {
         // Seta o plano
         plano = k;
         cout << "\rPlano " << plano << " " << flush;

         for ( i = 0; i < nx; i++ )
            for ( j = 0; j < ny; j++ )
               // Copia plano k para img2D
               img2D->data2D[i][j] = _img3D->data3D[i][j][k];

         // Realiza a rotulagem do  plano k
         rp->Go ( img2D );

         // Calcula os vetores area,  perimetro, permitindo retornar o raioHidraulico
         rp->CalculaRaioHidraulicoObjetos ();

         // Se for o último plano muda o tipo de contorno,
         // de forma a criar um CObjetoRede situado na face direita/EST
         if ( k == ( nz - 1 ) ) {
               tipoObjeto = ETipoObjetoGrafo::ObjetoRede_Sitio_EST;
               rotuloPrimeiroObjetoPlanoN = maiorRotuloUtilizado + ra->RotuloFinal ();
            }

         // Adiciona os sítios, a lista de sítios
         AdicionarObjetos ( rp, maiorRotuloUtilizado + ra->RotuloFinal (), tipoObjeto );

         // Compara as imagens ra e rp definindo as conexões entre sítios
         DeterminarConexoesObjetos ( maiorRotuloUtilizado );

         // Adiciona ao maiorRotuloUtilizado o número de sítios da ultima imagem rotulada.
         // Depois de determinar o relacionamento entre os objetos posso
         // atualizar o identificador do maior Rotulo utilizado.
         maiorRotuloUtilizado = maiorRotuloUtilizado + ra->RotuloFinal ();

         // faz o swap dos rotuladores, rp passa a ser o ra
         swap ( ra , rp );
      }

   cout << endl;

   // aqui? ra e rp não são mais usados; deveriam ser deletados aqui!
   // Elimina sítios com 0 links
   {
      CTime ( "Tempo processamento função  EliminarRamosMortos= ", &cout );
      EliminarRamosMortos ();
   }
   return this;
}

// -------------------------------------------------------------------------
// Função     AdicionarObjetos
// -------------------------------------------------------------------------
/** @short  :	Função que adiciona a lista de objetos do grafo, os objetos identificados em rotulador.
 * @author :	André Duarte Bueno
 * @param  : Recebe a imagem rotulada com os objetos a serem incluídos, o número do ultimo rótulo utilizado e 
 * o tipo de contorno.
 * @return : void
*/
void CGrafoConexaoSerial::AdicionarObjetos ( CRotulador2DCm* rotulador,
      unsigned long int ultimoRotuloUtilizado,
      ETipoObjetoGrafo tipoObjeto )
{
   // Não deve considerar o objeto 0 que é o fundo;
   // inclue o rotulo final, o objeto final, ok
   for ( unsigned long int rotulo = 1; rotulo <= rotulador->RotuloFinal (); rotulo++ ) {
         // Vai criar objeto do tipo CObjetoRede ou CObjetoRede_Tipo(passando o tipo (+rápida sem polimorfismo)).
         // depende do valor do flag de pré-proessamento OTIMIZAR_VELOCIDADE_PROCESSAMENTO
         value_type_objeto* data = nullptr;

         // Obtem um objeto novo passando o tipo de objeto a ser criado
         data = CriarObjeto ( tipoObjeto ); // implementada em CRede::CriarObjeto(..)
         assert ( data );

         // No rotulador o objeto 0 é o fundo, como rotulo esta iniciando em 1,
         // faço -1, para que o primeiro sitio tenha rotulo 0.
         data->rotulo = rotulo + ultimoRotuloUtilizado - 1;

         // Propriedade raio hidraulico, obtida da imagem rotulada
         data->propriedade = rotulador->RaioHidraulicoObjetos ( rotulo );

         // Passa para x o plano atual (sera usado na estimacao de x)
         data->x = this->plano;

         // Insere o objeto criado a lista de objetos do grafo
         objeto.push_back ( data );
      }
}

// -----------------------------------------------------------------------------
// Função EliminarRamosMortos ()
// -----------------------------------------------------------------------------
void CGrafoConexaoSerial::EliminarRamosMortos ()
{
   switch ( eliminaRamosMortos ) {
      case 0:   // não elimina os ramos mortos.
         break;

      case 1:
         EliminarRamosMortos_0_ou_1_conexao_v1 ();
         break;

      case 2:
      default:
         EliminarRamosMortos_0_ou_1_conexao_v2 ();
         break;
      }
}
// -----------------------------------------------------------------------------
// Função EliminarRamosMortos_0_ou_1_conexao_v1()
// -----------------------------------------------------------------------------
/** Elimina  sítios com 0 links;
Percorre  todo o grafo, considerando cada plano.
Localiza objeto com zero ou uma ligação e o elimina.
Se tem uma ligação, elimina as conexões com o objeto eliminado.
Repete o procedimento num do..while até que não ocorram mais deleções.
É lento porque percorre grafo várias vezes e usa delete(it) num vector grande!
Observe abaixo os resultados, observe que o número de objetos do grafo reduziu de 1827 para 1709
Comparação do tempo de processamento:
EliminarRamosMortos_0_ou_1_conexao_v1 : img 300 demorou 14min41seg
EliminarRamosMortos_0_ou_1_conexao_v2 : img 300 demorou 12seg

Abaixo resultado sem a funcao EliminarRamosMortos (a ordem esta Rotulo Condutancia)
1827
   0    1   28 1.75737e+09
   0    3   28 1.75737e+09   28 1.75737e+09   28 1.75737e+09
   0    1   28 1.75737e+09
   0    0			<<<<<---Presença de sítios com 0 link's
   0    1   28 1.75737e+09

   0    1   29 1.91887e+09
   0    1   26 1.50737e+08
   0    2   32 1.10932e+09   32 1.10932e+09
   0    5   30 8.08313e+08   34 2.352e+08   21 4.7694e+07   34 2.352e+08   20 1.50737e+08
 ...
   2    1 1801 2.83958e+09
   2    2 1800 1.41374e+09 1802 2.97751e+07

Abaixo resultado com a funcao EliminarRamosMortos (a ordem esta Rotulos Condutancias)
1709
   1    1   28 1.75737e+09
   1    3   28   28   28 1.75737e+09 1.75737e+09 1.75737e+09
   1    1   28 1.75737e+09
   1    1   28 1.75737e+09
   1    1   29 1.91887e+09

   1    1   26 1.50737e+08
   1    2   32   32 1.10932e+09 1.10932e+09
   ...
   2    1 1801 2.83958e+09
   2    2 1800 1802 1.41374e+09 2.97751e+07
*/
void CGrafoConexaoSerial::EliminarRamosMortos_0_ou_1_conexao_v1 ()
{
   // Definição de variáveis internas
   // Ponteiro para o sitio atual (1)
   value_type_objeto* objetoEmAnalise { nullptr };
   // e aquele para quem o objetoEmAnalise aponta (1->2)
   value_type_objeto* objetoConectado { nullptr };

   // Número total de passagens realizadas
   unsigned long int numeroPassagens { 0 };

   // Número de objetos deletados nesta passagem
   unsigned long int numeroObjetosDeletadosNestaPassagem  { 0 };

   // Número total de objetos deletados
   unsigned long int numeroTotalObjetosDeletados  { 0 };

   // Número de links do objeto 1 e 2
   unsigned long int numeroLinksObjetoEmAnalise  { 0 };
   unsigned long int numeroLinksObjetoConectado  { 0 };

   // Rotulo do objeto 1 em análise
   unsigned long int rotulo  { 0 };

   // O número de objetos no último plano é o número total de objetos
   // menos o rótulo do ultimo objeto do plano N-1.
   unsigned int numeroObjetosNoPlanoZn = objeto.size () - rotuloPrimeiroObjetoPlanoN;

   // Passo 1: Vai varrer os planos e marcar objetos a serem deletados
   do {
         numeroObjetosDeletadosNestaPassagem = 0;

         // Percorre todos os objetos do grafo, exceto os dos planos 0 e N
         for ( long int rotulo = rotuloPrimeiroObjetoPlano1; rotulo < rotuloPrimeiroObjetoPlanoN; rotulo++ ) {
               // Um objeto[i] é um ponteiro para um CObjetoRede;
               // Abaixo sempre trabalho com objetos do tipo CObjetoRede_Sitio, sendo assim,
               // preciso fazer um dynamic_cast para poder acessar o vetor das conexões.
               objetoEmAnalise = dynamic_cast < CObjetoRede_Sitio* > ( objeto[rotulo] );

               // Pega o número de conexões do objeto 1
               numeroLinksObjetoEmAnalise = objetoEmAnalise->conexao.size();

               // Se numeroLinksObjetoEmAnalise == 0 conexões
               if ( numeroLinksObjetoEmAnalise == 0 ) {
                     // Adiciona a lista de objetos eliminados o rotulo do objeto atual

                     // Deleta o objeto
                     delete objeto[rotulo];
                     // Aponta para nullptr
                     objeto[rotulo] = nullptr;

                     // Incrementa o numero de objetos deletados
                     numeroObjetosDeletadosNestaPassagem++;
                  }
               // Se numeroLinksObjetoEmAnalise ==  1 ligação, pode eliminar este sítio,
               // mas deve eliminar as conexões dos outros para ele.
               else if ( numeroLinksObjetoEmAnalise == 1 ) {
                     // --------------------
                     // INICIO FUNCAO DELETAROBJETO (objetoEmAnalise)
                     // Ponteiro para o objeto a quem estou conectado
                     objetoConectado = dynamic_cast < CObjetoRede_Sitio* > ( objetoEmAnalise->conexao[0] );

                     // Percorre todas as conexões do objetoConectado e procura aquelas que apontam
                     // para objetoEmAnalise.
                     // percorre de tras para frente.
                     for ( long int link = ( objetoConectado->conexao.size() - 1 ); link >= 0; link-- ) {
                           // se o link do objetoConectado aponta para objetoEmAnalise
                           if ( objetoConectado->conexao[link] == objetoEmAnalise ) {
                                 // Se for CObjetoRede_Sitio deleta somente a conexão (modelos 1 e 2)
                                 // Se for CObjetoRede_CC_Sitio deleta a conexão e a condutancia (modelos 3,4,5,..)
                                 objetoConectado->DeletarConexao ( link );

                                 // Mesmo depois de deletar a primeira ligação, deve verificar as demais
                                 // pois pode haver mais de uma ligação entre dois objetos (como no modelo1).
                              }
                        }

                     // FIM FUNCAO DELETAROBJETO (objetoEmAnalise)
                     // --------------------
                     // Agora deleta o objetoEmAnalise
                     delete objeto[rotulo];    // deleta o objeto, e não o ponteiro
                     objeto[rotulo] = nullptr; // zera o ponteiro
                     numeroObjetosDeletadosNestaPassagem++;
                  }
            }

         // Passo 2: vai deletar objetos que foram marcados com nullptr.
         // Mudar trecho abaixo: Criar novo vetor, copiar objetos válidos e então deletar vetor velho,
         // vai ficar mais rápido.
         // vector < CObjetoRede* >::iterator it = objeto.begin ();
         vector < value_type_objeto* >::iterator it = objeto.begin ();

         // Para facilitar a deleção dos objetos deleta do ultimo para o primeiro
         for ( int j = rotuloPrimeiroObjetoPlanoN - 1; j >= rotuloPrimeiroObjetoPlano1; j-- )
            if ( objeto[j] == nullptr )
               objeto.erase ( it + j ); // processo muitoooo lento!!

         numeroTotalObjetosDeletados += numeroObjetosDeletadosNestaPassagem;
         cout << "numeroObjetosDeletadosNestaPassagem=" << numeroObjetosDeletadosNestaPassagem
              << " numeroTotalObjetosDeletados =" << numeroTotalObjetosDeletados << endl;
      } // enquanto tem objeto sendo deletado repete o looping, varrendo todo o grafo!

   while ( numeroObjetosDeletadosNestaPassagem > 0 );

   // Chama função que reorganiza os valores de cmx e cmy.
   // Visto que boa parte dos objetos do grafo foram eliminados
   // eu preciso corrigir isto nos vetores de cmx e cmy.
   // Note que precisa chamar ReorganizarCmxCmy () antes de reorganizar os rótulos!
   ReorganizarCmxCmy ();

   // Após deletar todos os objetos válidos e chamar ReorganizarCmxCmy,
   // pesquisa todos os objetos e faz rótulos sequenciais.
   for ( unsigned long int i = 0; i < objeto.size (); i++ )
      objeto[i]->rotulo = i;

   // O número do ultimo objeto do solver precisa ser corrigido??
   // E se algum objeto do plano zn foi deletado ?
   // resp: objetos do plano zn não são verificados, portanto não são deletados!
   rotuloPrimeiroObjetoPlanoN = objeto.size () - numeroObjetosNoPlanoZn;
}

// -----------------------------------------------------------------------------
// Função EliminarRamosMortos_0_ou_1_conexao_v2()
// -----------------------------------------------------------------------------
/**
Etapa 1)
Varre todo o grafo e identifica objetos que estão na ponta dos ramos mortos.
Percorre o ramo e vai marcando os objetos para deleção.
Etapa 2)
Percorre todas as conexões de todos os objetos e marca para deleção
os objetos que foram marcados para deleção
Etapa 3)
Elimina as conexões para os objetos marcados para deleção
Etapa 4)
Elimina os objetos marcados para deleção
Etapa 5)
Realiza uma recursão até que nenhum objeto seja deletado.
*/
void CGrafoConexaoSerial::EliminarRamosMortos_0_ou_1_conexao_v2 ()
{
   // O numero de objetos no ultimo plano é o numero total de objetos
   // menos o indice do ultimo objeto do solver
   unsigned int numeroObjetosNoPlanoZn = objeto.size () - rotuloPrimeiroObjetoPlanoN;

   unsigned long int numeroObjetosDeletadosNestaPassagem {0};
   unsigned long int numeroTotalObjetosDeletados {0};

   /// @test: Como deve deletar tudo numa única passagem, não precisa do do..while?
   do {
         int deletado = objeto.size (); // deletado é apenas um número?
         numeroObjetosDeletadosNestaPassagem = 0;

         // ------------------------------------------------
         // A função MarcarParaDelecaoObjeto percorre os objetos do grafo
         // e marca para deleção os que fazem parte de ramo morto.
         // Todo o ramo é marcado para deleção.
		 for ( int i = 0; i < objeto.size (); i++ ) {
			MarcarParaDelecaoObjeto ( i );
            }

         // ------------------------------------------------
         // Percorrer todos os objetos e marcar para deleção cada link invalidado
         //  value_type_objeto* obj = nullptr;
          for ( int i = 0; i < objeto.size (); i++ ) {
               objeto[i]->DeletarConexoesInvalidadas ( deletado );
            }

         // ------------------------------------------------
         // Reorganizar efetivamente o vetor dos objetos
         int indice_rotulo_valido = 0;

         // Percorre todos os objetos
          for ( int i = 0; i < objeto.size (); i++ )
            // Se o objeto para quem aponta não foi deletado, armazena no vetor dos objetos;
            // Se foi deletado vai ser pulado.
            if ( objeto[i]->rotulo != deletado ) {
                  objeto[indice_rotulo_valido] = objeto[i];
                  indice_rotulo_valido++;
               }

         // Redimensiona o vetor das conexões (as que apontam para objetos deletados são eliminadas)
         // note que V2 chama resize, não deletando efetivamente o vetor de objetos.
         objeto.resize ( indice_rotulo_valido );

         numeroObjetosDeletadosNestaPassagem = deletado - objeto.size ();
         cout << "numeroObjetosDeletadosNestaPassagem=" << numeroObjetosDeletadosNestaPassagem << endl;
      }
   while ( numeroObjetosDeletadosNestaPassagem > 0 );

   // Chama função que reorganiza os valores de cmx e cmy
   // visto que boa parte dos objetos do grafo foram eliminados
   // é preciso corrigir isto nos vetores de cmx e cmy
   // Chamar antes de reorganizar os rótulos
   ReorganizarCmxCmy ();

   // Reorganizar os rótulos
   // Após deletar todos os objetos válidos e chamar ReorganizaCmxCmy
   // Pesquisa todos os objetos e faz rotulos sequenciais.
   // Dúvida: Se o rótulo do objeto é seu índice, nao precisa armazenar o rotulo?
   // Resp: o rótulo é usado para salvar dados objeto em disco.
   for ( unsigned long int i = 0; i < objeto.size (); i++ )
      objeto[i]->rotulo = i;

   // O rótulo do último objeto do solver precisa ser corrigido.
   // Dúvida: e se algum objeto do plano zn foi deletado ?
   // Resp: objetos do plano zn não são marcados para deleção.
   rotuloPrimeiroObjetoPlanoN = objeto.size () - numeroObjetosNoPlanoZn;
}

bool
CGrafoConexaoSerial::MarcarParaDelecaoObjeto ( int i )
{
   // -------------------------------------------------
   // Marca a variável deletado como sendo o size do vetor de objetos.
   /// Note que deletado aponta para índice inválido, o size.
   int deletado = objeto.size ();

   // -------------------------------------------------
   // Como vai percorrer todo o ramo morto e deletar em sequência, a função inclui chamadas recursivas,
   // o que explica os parâmetros deste if.
   // Uma conexão que já foi deletada (marcada com deletado) pode ser chamada 2x,
   // o que explica o if(..i != deletado)
   if (	// Se o objeto ja foi deletado, o indice i == deletado, deve ser desconsiderado.
      i != deletado		// sempre primeiro a verificar
      // Se o objeto já foi deletado
      and objeto[i]->rotulo != deletado 
      // e esta no centro, vai verificar
      and objeto[i]->Contorno () == CContorno::ETipoContorno::CENTER ) {

         // Obtêm o número de conexões
         int nlinks = objeto[i]->conexao.size ();

         // Se 0 links, é um objeto isolado, então marca para deleção
         // ........*.........
         if ( nlinks == 0 )
            objeto[i]->rotulo = deletado;

         // se tem um link (ramo duplo) então marca para deleção, e pede para verificar a conexão.
         // ........*<------->*........
         // note que objetos nos planos z=0 e z=n terão geralmente 1 link, por isso a condição
         // if( ..objeto[i]->Contorno () == CContorno::ETipoContorno::CENTER)
         else if ( nlinks == 1 ) {
               objeto[i]->rotulo = deletado;
               // Verifica o sítio a quem estou conectado (marca para deleção)
               MarcarParaDelecaoObjeto ( objeto[i]->conexao[0]->rotulo );
            }

         // se dois links
         // ou esta no meio de um ramo válido(e não deve ser deletado),
         // z0........*<------->*<----->*<-------->*.....zn
         // ou esta entre dois sítios dos quais um já foi deletado.
         // ........*d------->*----->*-------->*...
         else if ( nlinks == 2 ) {
               // Se a conexao[0] se refere a objeto já deletado então o número efetivo
               // de conexões é 1, e precisa marcar para deletação.
               if ( objeto[i]->conexao[0]->rotulo == deletado ) {
                     objeto[i]->rotulo = deletado;
                     // Solicita deleção da conexao 1.
                     MarcarParaDelecaoObjeto ( objeto[i]->conexao[1]->rotulo );
                  }

               // Se a conexao[1] se refere a objeto já deletado então o número efetivo
               // de conexões é 1, e precisa marcar para deletação.
               /*else*/ if ( objeto[i]->conexao[1]->rotulo == deletado ) {
                     objeto[i]->rotulo = deletado;
                     // Solicita deleção da conexao 0.
                     MarcarParaDelecaoObjeto ( objeto[i]->conexao[0]->rotulo );
                  }
            }
      }

   return 1;
}

// -------------------------------------------------------------------------
// Função:                    LerVetorPropriedades_x()
// -------------------------------------------------------------------------
/**
@short  : Lê os dados da propriedade x dos objetos do grafo que foram salvos no disco.
O sistema de proteçao contra quedas de energia salva, a cada conjunto de iterações,
os dados dos objetos do grafo em disco (salva o vetor das pressões do grafo).
Se o micro caiu (queda de luz), o programa pode solicitar a reinicialização da simulação,
criando o grafo, determinando o mesmo e então chamando a função LerVetorPropriedades_x(),
que vai ler somente as pressãos do arquivo de disco NomeGrafo()+ ".vectorX".
@author :	André Duarte Bueno
@return :	bool // ostream&
@test: o nome do arquivo "grafo.vectorX" foi modificado para considerar o nome da imagem
pois pode processar mais de um arquivo no mesmo diretório ao mesmo tempo.
Como mudou precisa testar!
*/
bool CGrafoConexaoSerial::LerVetorPropriedades_x ()
{
   // Abre arquivo de disco
   // ifstream fin ("grafo.vectorX");
   ifstream fin ( ( NomeGrafo() + ".vectorX" ).c_str() );

   if ( fin.fail () ) {
         cerr << "Falha abertura arquivo: " << ( NomeGrafo() + ".vectorX" ) << " que tem os dados do grafo.\n";
         return 0;
      }

   string primeiraLinhaTemDimensaoGrafo;
   fin >> primeiraLinhaTemDimensaoGrafo;
   unsigned int dimensaoDoGrafo;
   fin >> dimensaoDoGrafo;

   // Lê os dados do vetor (atributo propriedade).
   for ( auto objeto_grafo : objeto ) {
         fin >> objeto_grafo->x ;

		 /// o código abaixo poderia ser comentado pois não deve ocorrer!
         if ( fin.eof () )
            return 0;
      }

   if ( dimensaoDoGrafo != objeto.size() ) {
         cerr << "\nVerificar erro pois dimensao do grafo no arquivo é: " << dimensaoDoGrafo
              << "\ne o números de objetos que foram lidos é: " << objeto.size() << '\n';
         return 0;
      }

   return 1;
}

/// Salva no arquivo "grafo.vectorX" o valor da pressão de cada objeto.
bool CGrafoConexaoSerial::SalvarVetorPropriedades_x ()
{
   // Abre arquivo de disco
   //  ofstream fout ("grafo.vectorX");
   ofstream fout ( ( NomeGrafo() + ".vectorX" ).c_str() );

   if ( fout.fail () )
      return 0;

   fout << "dimensaoDoGrafo= " << objeto.size() << '\n'; // manter espaço depois =

   // Salva os dados do atributo x de cada objeto em disco (ex: pressão que esta sendo calculada).
   for ( auto objeto_grafo : objeto )
      fout << objeto_grafo->x << '\n';

   return 1;
}

// -------------------------------------------------------------------------
// Função:   EliminarConexoesParalelo_SomarCondutancias
// -------------------------------------------------------------------------
/** @short  : Percorre todos os objetos e elimina conexões repetidas (deve ser chamada depois do
 * cálculo das condutâncias, pois soma as condutâncias das conexões repetidas.
 * se chamar antes, vai acumular o raio hidraulico)
 * Chama função de cada sítio, que elimina links repetidos.
 * @author : André Duarte Bueno
 * @param  : nada
 * @return : void
*/
void
CGrafoConexaoSerial::EliminarConexoesParalelo_SomarCondutancias ()
{
   CTime ( "Tempo processamento função EliminarConexoesParalelo_SomarCondutancias  = ", &cout );
   unsigned long int totalConexoesParalelo = 0;
   unsigned long int ConexoesParaleloObjeto_i = 0;
   // Percorre todos os objetos do grafo
   if ( deletaConexoesParalelo  == true )
      for ( auto objeto_i :  objeto ) {
            // Chama DeletarConexoesRepetidas_e_SomarCondutanciasParalelo, que retorna o número de links eliminados
            ConexoesParaleloObjeto_i = objeto_i->DeletarConexoesRepetidas_e_SomarCondutanciasParalelo ();
            totalConexoesParalelo += ConexoesParaleloObjeto_i ;
		 cout << "\nNúmero total de ConexoesParaleloObjeto_i = " << ConexoesParaleloObjeto_i << '\n';
         }
	cout << "\nNúmero total de conexões em paralelo que foram deletadas = " << totalConexoesParalelo << endl;
}

// -------------------------------------------------------------------------
// Função:               SetarMatrizAVetorB
// -------------------------------------------------------------------------
/** @short  : Recebe uma matriz A (vazia) e um vetor B (vazio)
e preenche os mesmos com os coeficientes necessários
para determinação do sistema de equações.
Pré-condições:
1- O grafo já deve ter sido determinado.
2- Os valores iniciais de pressão já devem ter sido definidos
(valores de contorno, normalmente Plano_0 = 1, Plano_n = 0).
3- Deve receber uma matriz e um vetor vazios.
@author : André Duarte Bueno
@param  :
@return : bool indicando sucesso da operação.
@todo: receber matriz de double !! eliminando multiplicador 1e17.
@todo: transformar em template que recebe tipo : float, double, long double.
@test: testar para ver se esta funcionando!
*/
bool CGrafoConexaoSerial::SetarMatrizAVetorB ( TCMatriz2D< int >*& A, CVetor*& B ) const
{
   // vector< vector<double> > A;
   // vector<double> B;

   // Passo 0: Definição de variáveis auxiliares
   // índice i da matriz A (ou vetor B)
   unsigned long int mi;

   // índice j da matriz A
   unsigned long int mj;

   // Condutância total Cij = (Cii+Cjj)/2 para o modelo 2;
   /// @todo: calcular Condutância total Cij para demais modelos!
   long double Cij;

   // Passo 1:
   // Determinação da dimensão da matriz e do vetor
   // cout << "\nrotuloPrimeiroObjetoPlanoN="               <<       rotuloPrimeiroObjetoPlanoN;
   // cout << "\nrotuloPrimeiroObjetoPlano1="      <<  rotuloPrimeiroObjetoPlano1;

   // A dimensão do sistema de equações considera os planos 1->n-1,
   // pois os planos 0 e n-1 tem pressões constantes.
   unsigned int dim = rotuloPrimeiroObjetoPlanoN - rotuloPrimeiroObjetoPlano1 + 1;
   // cout <<"\ndim="<<dim;

   // Redimensiona a matriz A
   A->Redimensiona ( dim, dim );
   // Zera a matriz A (verificar necessidade)
   A->Constante ( 0 );

   // Redimensiona o vetor B
   B->Redimensiona ( dim );
   // Zera o vetor B  (verificar necessidade)
   B->Constante ( 0 );
// renomear i->indiceConeccao e j=indiceObjeto
   unsigned int i;

   for ( unsigned long int j = 0; j < objeto.size (); j++ ) {
         switch ( objeto[j]->Contorno () ) {
               // Fronteira esquerda/WEST
            case CContorno::ETipoContorno::WEST:

               // Fronteira direita/EST
            case CContorno::ETipoContorno::EST:

               // Percorre as conexões do objeto
               for ( i = 0; i < objeto[j]->conexao.size (); i++ ) {
                     /// Calcula Cij - @todo: explicar a equacao usada.
                     Cij = ( objeto[j]->conexao[i]->propriedade + objeto[j]->propriedade ) / 2.0;
                     Cij = Cij * 1.0e17;	// LIXO, para gerar int
                     // cij esta sendo armazenado em int por isto multiplico por e17

                     // Desloca o índice da matriz(vetor), pois só entram os sítios
                     // que não estão na interface. ? Verificar ? e o último plano?
                     mi = objeto[j]->conexao[i]->rotulo - rotuloPrimeiroObjetoPlano1;

                     // Acumula Cij no vetor B[mi] -= Cij * objeto_j->x,
                     // x deve estar definido.
                     B->data1D[mi] -= static_cast < int > ( Cij * objeto[j]->x );

                     // Acumula -Cij na matriz A[mi][mi]
                     // A->data2D[mi][mi] -= Cij;
                     A->data2D[mi][mi] -= static_cast < int > ( Cij );
                  }

               break;

               // Fronteira Centro
            case CContorno::ETipoContorno::CENTER:

               // Percorre as conexões do objeto
               for ( i = 0; i < objeto[j]->conexao.size (); i++ ) {
                     // Se o link for um objeto de centro (não contorno) entra
                     if ( objeto[j]->conexao[i]->Contorno () == CContorno::ETipoContorno::CENTER ) {
                           // Calcula Cij
                           Cij = ( objeto[j]->propriedade + objeto[j]->conexao[i]->propriedade ) / 2.0;
                           Cij = Cij * 1.0e17;	// LIXO para gerar int
                           // cij esta sendo armazenado em int por isto multiplico por e17

                           // Desloca os índices da matriz
                           mi = objeto[j]->conexao[i]->rotulo - rotuloPrimeiroObjetoPlano1;
                           mj = objeto[j]->rotulo - rotuloPrimeiroObjetoPlano1;

                           // Define A->data2D[mi][mj]
                           A->data2D[mi][mj] = static_cast < int > ( Cij );	// LIXO o static

                           // Acumula A->data2D[mj][mj]
                           A->data2D[mj][mj] -= static_cast < int > ( Cij ); // LIXO o static
                        }
                  }

               break;
            }			// switch
      }				// for

   A->Write ( NomeGrafo() + ".matrixA" ); // A->Write ("grafo.matrixA");
   B->Write ( NomeGrafo() + ".vectorB" ); // B->Write ("grafo.vectorB");
   return 1;
}

/*ANTIGA, FUNCIONA, USA
bool CGrafo::SetarMatrizAVetorB(TCMatriz2D< int >* &A, CVetor* &B) const
{
  // Passo 0: Definição de variáveis auxiliares
  // índice i da matriz A (ou vetor B)
  unsigned long int mi;

  // índice j da matriz A
  unsigned long int mj;

  // Condutância total Cij = (Cii+Cjj)/2 para o modelo 2
  long double Cij;

  // Passo 1:
  // Determinação da dimensão da matriz e do vetor
  // cout << "\nrotuloPrimeiroObjetoPlanoN="		<<	 rotuloPrimeiroObjetoPlanoN;
  // cout << "\nrotuloPrimeiroObjetoPlano1="	<<  rotuloPrimeiroObjetoPlano1;


  unsigned int dim = rotuloPrimeiroObjetoPlanoN - rotuloPrimeiroObjetoPlano1 + 1 ;
  // cout <<"\ndim="<<dim;

  // Redimensiona a matriz A
  A->Redimensiona(dim,dim);
  // Zera a matriz A
  A->Constante(0);

  // Redimensiona o vetor B
  B->Redimensiona(dim);
  // Zera o vetor B
  B->Constante(0);

  unsigned int i;
  for( unsigned long int j = 0 ; j < objeto.size();  j++ )
    {
      // Faz um cast para sítio derivado (em função do acesso a função Contorno e vetor conexao.

      CObjetoRede_Sitio* objeto_j = dynamic_cast<CObjetoRede_Sitio*>(objeto[j]);
      assert(objeto_j);

      switch( objeto_j->Contorno() )
	{
	  // Fronteira esquerda/WEST
	case CContorno::WEST :

	  // Fronteira direira
	case CContorno::EST :
	  // Percorre as conexões do objeto
	  for ( i = 0; i < objeto_j->conexao.size(); i++)
	    {
	      // Calcula Cij
	      Cij = (objeto_j->conexao[i]->propriedade +  objeto_j->propriedade   ) /2.0 ;
	      Cij = Cij* 1.0e17;	// LIXO, para gerar int
	      // cij esta sendo armazenado em int por isto multiplico por e17

	      // Desloca o índice da matriz(vetor), pois só entram os sítios
	      // que não estão na interface.
	      mi = objeto_j->conexao[i]->rotulo - rotuloPrimeiroObjetoPlano1; // 3;

	      // Acumula Cij no vetor B[mi] -= Cij	* objeto_j->x,
	      // x deve estar definido
	      // B->data1D[ mi ] -= Cij * objeto_j->x;
	      B->data1D[ mi ] -= static_cast<int>( Cij * objeto_j->x);

	      // Acumula -Cij na matriz A[mi][mi]
	      // A->data2D[mi][mi] -= Cij;
	      A->data2D[mi][mi] -= static_cast<int>(Cij);
	    }
	  break;

	  // Fronteira Centro
	case CContorno::CENTER :
	  // Percorre as conexões do objeto
	  for ( i = 0; i < objeto_j->conexao.size(); i++)
	    {
	      // Se o link  for  um objeto de centro (não contorno) entra
	      if( objeto_j->conexao[i]->Contorno() == CContorno::CENTER)
		{
		  // Calcula Cij
		  Cij = ( objeto_j->propriedade + objeto_j->conexao[i]->propriedade ) /2.0 ;
		  Cij = Cij* 1.0e17;// LIXO para gerar int
		  // cij esta sendo armazenado em int por isto multiplico por e17

		  // Desloca os índices da matriz
		  mi = objeto_j->conexao[i]->rotulo - rotuloPrimeiroObjetoPlano1 ;
		  mj = objeto_j->rotulo  - rotuloPrimeiroObjetoPlano1 ;

		  // Define A->data2D[mi][mj]
		  A->data2D[mi][mj]	 = static_cast<int>(Cij); // LIXO o static

		  // Acumula A->data2D[mj][mj]
		  A->data2D[mj][mj]	-= static_cast<int>(Cij);// LIXO o static
		}
	    }
	  break;
	}// switch
    }// for

  A->Write("Matriz_A.txt");
  B->Write("Vetor_B.txt");
  return 1;
}
*/
