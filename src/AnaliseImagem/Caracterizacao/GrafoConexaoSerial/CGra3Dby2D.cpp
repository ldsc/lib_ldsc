
/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================

Desenvolvido por:	
				  Laboratorio de Desenvolvimento de Software Cientifico  [LDSC].
@author:          André Duarte Bueno
@file:            CGra3Dby2D.cpp
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
#ifndef CGra3Dby2D_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CGra3Dby2D.h>
#endif

#ifndef COGSitio_h
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio.h>
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
a informacao do plano a que pertence. Será usado para estimação
da pressão inicial.*/
CGrafo * CGra3Dby2D::Go (string nomeArquivoImagem, unsigned long int naoUsado)
{
   unsigned long int i, j, k;
   maiorRotuloUtilizado = 0;

   CContorno::ETipoContorno tipoContornoObjeto = CContorno::ETipoContorno::CENTER;

   ifstream fin ( nomeArquivoImagem.c_str () );
   if (fin.fail())
   {
      cerr << "Nao conseguiu abrir o arquivo " << nomeArquivoImagem.c_str ();
      return 0;
   }

   // Leitura do cabecalho do arquivo de disco
   char d3[55]; //?
   unsigned int numeroCores;
   unsigned int valor;
   fin >> d3 >> nx >> ny >> nz >> numeroCores;

   // Criacao dos objetos ra,rp img2D
   if(ra) delete ra;
   ra = nullptr;
   ra = new CRotulador2DCm (nx, ny);
   assert (ra);
   if(rp) delete rp;
   rp = nullptr;
   rp = new CRotulador2DCm (nx, ny);
   assert (rp);
   if(img2D) delete img2D;
   img2D = nullptr;
   img2D = new TCMatriz2D< int > (nx, ny);
   assert (img2D);

   // ------------
   // PLANO 0
   // ------------
   // Le dados do plano 0
   plano = k = 0;
   cout << "\n plano = " << plano /*<< flush*/;
   for (j = 0; j < ny; j++)
      for (i = 0; i < nx; i++)
      {
//          fin >> valor;
//          img2D->data2D[i][j] = valor;
         fin >> img2D->data2D[i][j] ;
      }

   // Rotula plano 0
   ra->Go (img2D);

   // Determina raio hidraulico dos objetos
   ra->CalculaRaioHidraulicoObjetos ();

   // Define o contorno como sendo da face WEST (plano 0)
   tipoContornoObjeto = CContorno::ETipoContorno::WEST;

   // Adciona ao grafo os objetos do plano 0
   AdicionarObjetos (ra, maiorRotuloUtilizado, tipoContornoObjeto);

   // Determina o rótulo do primeiro objeto a ser usado pelo solver.
   /// @todo: verificar retorno de ra->RotuloFinal() = último rótulo utilizado
   /// se for último rótulo utilizado precisa somar 1
   rotuloPrimeiroObjetoPlano1 = maiorRotuloUtilizado + ra->RotuloFinal (); // +1

   // ------------
   // PLANO i
   // ------------
   tipoContornoObjeto = CContorno::ETipoContorno::CENTER;
   // Le dados do plano ij
   for (k = 1; k < nz; k++)
   {
      plano = k;
      cout << "\rPlano  " << plano << flush;

      for (j = 0; j < ny; j++)
         for (i = 0; i < nx; i++)
         {
//             fin >> valor;
//             img2D->data2D[i][j] = valor;
               fin >> img2D->data2D[i][j] ;
         }
      // Rotula
      rp->Go (img2D);

      // Determina raio hidraulico
      rp->CalculaRaioHidraulicoObjetos ();

      // Se for o ultimo plano, redefine o contorno e calcula rotuloUltimoObjetoPlanoN_1
      if (k == (nz - 1))
      {
         tipoContornoObjeto = CContorno::ETipoContorno::EST;
         rotuloUltimoObjetoPlanoN_1 = maiorRotuloUtilizado + ra->RotuloFinal ();
      }

      // Adiciona objetos do plano ij atual
      AdicionarObjetos (rp, maiorRotuloUtilizado + ra->RotuloFinal (),tipoContornoObjeto);

      // Estabelece os links entre os objetos
      DeterminarConeccoesObjetos (maiorRotuloUtilizado);

      // Determina o maior rotulo utilizado
      maiorRotuloUtilizado = maiorRotuloUtilizado + ra->RotuloFinal ();

      // Reordena os ponteiros para os planos de rotulagem
      swap(ra,rp);
   }

   // Fecha o arquivo de disco
   fin.close ();

   cout << endl;
   // Elimina objetos com 0 links
   {
      CTime ("Tempo processamento função EliminarObjetosRedundantes= ", &cout);
      EliminarObjetosRedundantes ();
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
  DeterminarConeccoesObjetos,
  EliminarObjetosRedundantes.
@author :	André Duarte Bueno
@see    :	Grafos, rotulagem
@param  :	Uma matriz 3D e um identificador
@return :	this
*/
CGrafo * CGra3Dby2D::Go (TCMatriz3D<int> * _img3D, unsigned long int naoUsado) {

   // Usados para percorrer  a imagem
   unsigned long int i, j, k;

   CContorno::ETipoContorno tipoContornoObjeto = CContorno::ETipoContorno::CENTER;

   // Armazena a informacao das dimensoes da imagem
   nx = _img3D->NX ();
   ny = _img3D->NY ();
   nz = _img3D->NZ ();

   maiorRotuloUtilizado = 0;

   // Criacao dos objetos ra,rp img2D
   // Cria 2 objetos de rotulagem 2D
   // Rotulador para imagem anterior
   if(ra) delete ra;
   ra = nullptr;
   ra = new CRotulador2DCm (nx, ny);
   assert (ra);

   // Rotulador para imagem posterior
   if(rp) delete rp;
   rp = nullptr;
   rp = new CRotulador2DCm (nx, ny);
   assert (rp);

   // Matriz imagem bidimensional passada para rotulador->Go(matriz)
   // Contém uma copia do plano3D a ser analisado
   if(img2D) delete img2D;
   img2D = nullptr;
   img2D = new TCMatriz2D< int > (nx, ny);
   assert (img2D);

   // ------------
   // PLANO 0
   // ------------
   plano = k = 0;
   cout << "\nplano " << plano << " " << flush;
   for (i = 0; i < nx; i++)
      for (j = 0; j < ny; j++)
         img2D->data2D[i][j] = _img3D->data3D[i][j][k];

   // Rotula plano 0
   ra->Go (img2D);

   // Calcula os vetores area, perimetro e raio hidraulico do plano 0
   ra->CalculaRaioHidraulicoObjetos ();

   // Chama função que adiciona objetos rotulados (ra) ao vetor de objetos,
   // Passa o tipo do contorno (WEST)
   tipoContornoObjeto = CContorno::ETipoContorno::WEST;

   // Adiciona objetos no grafo
   AdicionarObjetos (ra, maiorRotuloUtilizado, tipoContornoObjeto);

   // Determina o primeiro objeto a ser usado pelo solver
   rotuloPrimeiroObjetoPlano1 = maiorRotuloUtilizado + ra->RotuloFinal ();	// +1

   // ----------------------------------------------------------------------------
   // Percorre todos os demais planos
   // ----------------------------------------------------------------------------
   // Define o tipo de contorno como sendo de objeto central
   // (não é contorno esquerdo/WEST nem direito/EST)
   tipoContornoObjeto = CContorno::ETipoContorno::CENTER;

   // k é o indice da direcao z
   for (k = 1; k < nz; k++)
   {
      // Seta o plano
      plano = k;
      cout << "\rPlano " << plano << " " << flush;

      for (i = 0; i < nx; i++)
         for (j = 0; j < ny; j++)
            // Copia plano k para img2D
            img2D->data2D[i][j] = _img3D->data3D[i][j][k];

      // Realiza a rotulagem do  plano k
      rp->Go (img2D);

      // Calcula os vetores area,  perimetro, permitindo retornar o raioHidraulico
      rp->CalculaRaioHidraulicoObjetos ();

      // Se for o último plano muda o tipo de contorno,
      // de forma a criar um CObjetoGrafo situado na face direita/EST 
      if (k == (nz - 1))
      {
         tipoContornoObjeto = CContorno::ETipoContorno::EST;
         rotuloUltimoObjetoPlanoN_1 = maiorRotuloUtilizado + ra->RotuloFinal ();
      }

      // Adiciona os sítios, a lista de sítios
      AdicionarObjetos (rp, maiorRotuloUtilizado + ra->RotuloFinal (),tipoContornoObjeto);

      // Compara as imagens ra e rp definindo as conexões entre sítios
      DeterminarConeccoesObjetos (maiorRotuloUtilizado);

      // Adiciona ao maiorRotuloUtilizado o número de sítios da ultima imagem rotulada.
      // Depois de determinar o relacionamento entre os objetos posso
      // atualizar o identificador do maior Rotulo utilizado.
      maiorRotuloUtilizado = maiorRotuloUtilizado + ra->RotuloFinal ();

      // faz o swap dos rotuladores
      swap(ra , rp);
   }
   cout << endl;

   // Elimina sítios com 0 links
   {
      CTime ("Tempo processamento função  EliminarObjetosRedundantes= ",&cout);
      EliminarObjetosRedundantes ();
   }
   return this;
}

// -------------------------------------------------------------------------
// Função     AdicionarObjetos
// -------------------------------------------------------------------------
/** @short  :	Função que adiciona a lista de objetos do grafo, os objetos identificados em rotulador.
 * @author :	André Duarte Bueno
 * @see    :
 * @param  : Recebe a imagem rotulada com os objetos a serem incluídos,	
 * o número do ultimo rótulo utilizado e o tipo de contorno (identifica o objeto a ser criado:
 * COGSitio_EST = 0, COGSitio_CENTER = 1,  COGSitio_EST = 2)
 * @return : void
*/
void CGra3Dby2D::AdicionarObjetos (CRotulador2DCm * rotulador, 
				   unsigned long int ultimoRotuloUtilizado,
				   CContorno::ETipoContorno tipoContornoObjeto) 
{
   // Ponteiro para objeto a ser criado
   CObjetoGrafo * data = nullptr;

   // Não deve considerar o objeto 0 que é o fundo;
   // inclue o rotulo final, o objeto final, ok
   for (unsigned long int rotulo = 1; rotulo <= rotulador->RotuloFinal (); rotulo++)
   {
      // Obtem um sítio novo passando o tipo
      data = CriarObjetoGrafo (tipoContornoObjeto);
      assert (data);

      // No rotulador o objeto 0 é o fundo, como rotulo esta iniciando em 1, 
      // faço -1, para que o primeiro sitio tenha rotulo 0.
      // Tarefa: verificar possibilidade eliminar -1; testar.
      data->rotulo = rotulo + ultimoRotuloUtilizado - 1;

      // Propriedade raio hidraulico
      data->propriedade = rotulador->RaioHidraulicoObjetos (rotulo);

      // Passa para x o plano atual (sera usado na estimacao de x)
      data->x = this->plano;

      // Insere o objeto criado a lista de objetos do grafo
      objeto.push_back (data);
   }
}

// -----------------------------------------------------------------------------
// Função EliminarObjetosRedundantes ()
// -----------------------------------------------------------------------------
void CGra3Dby2D::EliminarObjetosRedundantes ()
{
  switch (eliminaRamosMortos)
    {
    case 0:			// não elimina os ramos mortos.
      break;
    case 1:
      EliminarObjetosRedundantes_1 ();
      break;
    case 2:
    default:
      EliminarObjetosRedundantes_2 ();
      break;
    }
}
// -----------------------------------------------------------------------------
// Função EliminarObjetosRedundantes_1()
// -----------------------------------------------------------------------------
/** Elimina  sítios com 0 links;
Percorre  todo o grafo, considerando cada plano.
Localiza objeto com zero ou uma ligação e o elimina.
Se tem uma ligação, elimina as conecções com o objeto eliminado.
Repete o procedimento num do..while até que não ocorram mais deleções.
É lento porque percorre grafo várias vezes e usa delete(it) num vector grande!
Observe abaixo os resultados, observe que o número de objetos do grafo reduziu de 1827 para 1709
Comparação do tempo de processamento:
EliminarObjetosRedundantes_1 : img 300 demorou 14min41seg
EliminarObjetosRedundantes_2 : img 300 demorou 12seg

Abaixo resultado sem a funcao EliminarObjetosRedundantes (a ordem esta Rotulo Condutancia)
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

Abaixo resultado com a funcao EliminarObjetosRedundantes (a ordem esta Rotulos Condutancias)
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
void CGra3Dby2D::EliminarObjetosRedundantes_1 () 
{
   // Definição de variáveis internas
   // Ponteiro para o sitio atual (1)
   COGSitio *objetoEmAnalise = nullptr;  
   // e aquele que aponta para 1 (2)
   COGSitio *objetoConectado = nullptr; 

   // Número total de passagens realizadas
   unsigned long int numeroPassagens = 0;

   // Numero de objetos deletados em cada passagem e numero total de objetos deletados
   unsigned long int numeroObjetosDeletadosNestaPassagem ;
   unsigned long int numeroTotalObjetosDeletados = 0;

   // Número de links do objeto 1 e 2
   unsigned long int numeroLinksObjetoEmAnalise;
   unsigned long int numeroLinksObjetoConectado;

   // Rotulo do objeto 1 em análise
   unsigned long int rotulo;

   // Cria-se um vector com o rotulo dos objetos eliminados
   // permitindo a fácil identificação dos objetos que precisam,
   // ser deletados no vetor cmx e cmy.
   vector < unsigned int >vObjetosEliminados; ///@todo não é usado??
   vObjetosEliminados.reserve(1000);

   // O numero de objetos no ultimo plano é o numero total de objetos
   // menos o rótulo do ultimo objeto do plano N-1.
   unsigned int numeroObjetosNoPlanoZn = objeto.size () - rotuloUltimoObjetoPlanoN_1;

   // Passo 1: Vai varrer os planos e marcar objetos a serem deletados
   do {
      numeroObjetosDeletadosNestaPassagem = 0;

      // Percorre todos os objetos do grafo, exceto os dos planos 0 e N
      for (long int rotulo = rotuloPrimeiroObjetoPlano1; rotulo <= rotuloUltimoObjetoPlanoN_1; rotulo++)
      {
         // Um objeto[i] é um ponteiro para um CObjetoGrafo;
         // Abaixo sempre trabalho com objetos do tipo COGSitio, sendo assim,
         // preciso fazer um dynamic_cast para poder acessar o vetor das conecções.
         objetoEmAnalise = dynamic_cast < COGSitio * >(objeto[rotulo]);

         // Pega o número de conecções do objeto 1
         numeroLinksObjetoEmAnalise = objetoEmAnalise->coneccao.size();

        // Se numeroLinksObjetoEmAnalise == 0 conecções
        if (numeroLinksObjetoEmAnalise == 0) 
	     {
            // Adiciona a lista de objetos eliminados o rotulo do objeto atual
            vObjetosEliminados.push_back (rotulo);

            // Deleta o objeto 
            delete objeto[rotulo];
            // Aponta para nullptr
            objeto[rotulo] = nullptr;

            // Incrementa o numero de objetos deletados
            numeroObjetosDeletadosNestaPassagem++;
         }
         // Se numeroLinksObjetoEmAnalise ==  1 ligação, pode eliminar este sítio,
         // mas deve eliminar as conecções dos outros para ele.
        else if ( numeroLinksObjetoEmAnalise == 1)
	     {
            // --------------------
            // INICIO FUNCAO DELETAROBJETO (objetoEmAnalise)
            // Ponteiro para o objeto a quem estou conectado
            objetoConectado = dynamic_cast < COGSitio * >( objetoEmAnalise->coneccao[0] );

            // Percorre todas as conecções do objetoConectado e procura aquelas que apontam 
			// para objetoEmAnalise.
            // percorre de tras para frente.
            for (long int link = (objetoConectado->coneccao.size() - 1); link >= 0; link--)
            {
               // se o link do objetoConectado aponta para objetoEmAnalise
               if (objetoConectado->coneccao[link] == objetoEmAnalise)
               {
                  // Se for COGSitio deleta somente a conecção (modelos 1 e 2)
                  // Se for COGSitio_CC deleta a conecção e a condutancia (modelos 3,4,5,..)
                  objetoConectado->DeletarConeccao (link);	// desativada em COGSitio e COGSitio_CC

                  // Mesmo depois de deletar a primeira ligação, deve verificar as demais
                  // pois pode haver mais de uma ligação entre dois objetos (como no modelo1).
               }
            }
            // FIM FUNCAO DELETAROBJETO (objetoEmAnalise)
            // --------------------

            // Agora deleta o objetoEmAnalise
            vObjetosEliminados.push_back (rotulo);
            delete objeto[rotulo];    // deleta o objeto, e não o ponteiro
            objeto[rotulo] = nullptr; // zera o ponteiro
            numeroObjetosDeletadosNestaPassagem++;
         }
      }
	  // Passo 2: vai deletar objetos que foram marcados com nullptr.
      // Mudar trecho abaixo: Criar novo vetor, copiar objetos válidos e então deletar vetor velho,
      // vai ficar mais rápido.
      vector < CObjetoGrafo * >::iterator it = objeto.begin ();

      // Para facilitar a deleção dos objetos deleta do ultimo para o primeiro
      for (int j = rotuloUltimoObjetoPlanoN_1; j >= rotuloPrimeiroObjetoPlano1; j--)
         if ( objeto[j] == nullptr )
            objeto.erase (it + j); // processo muitoooo lento!!

      numeroTotalObjetosDeletados += numeroObjetosDeletadosNestaPassagem;
      cout << "numeroObjetosDeletadosNestaPassagem=" << numeroObjetosDeletadosNestaPassagem 
           << " numeroTotalObjetosDeletados =" << numeroTotalObjetosDeletados << endl;
   } // enquanto tem objeto sendo deletado repete o looping, varrendo todo o grafo!
   while (numeroObjetosDeletadosNestaPassagem > 0);

   // Chama função que reorganiza os valores de cmx e cmy.
   // Visto que boa parte dos objetos do grafo foram eliminados
   // eu preciso corrigir isto nos vetores de cmx e cmy.
   // Note que precisa chamar ReorganizarCmxCmy () antes de reorganizar os rótulos!
   ReorganizarCmxCmy ();

   // Após deletar todos os objetos válidos e chamar ReorganizarCmxCmy,
   // pesquisa todos os objetos e faz rótulos sequenciais.
   for (unsigned long int i = 0; i < objeto.size (); i++)
      objeto[i]->rotulo = i;
   
   // O número do ultimo objeto do solver precisa ser corrigido??
   // E se algum objeto do plano zn foi deletado ?
   // resp: objetos do plano zn não são verificados, portanto não são deletados!
   rotuloUltimoObjetoPlanoN_1 = objeto.size () - numeroObjetosNoPlanoZn;
}

// -----------------------------------------------------------------------------
// Função EliminarObjetosRedundantes_2()
// -----------------------------------------------------------------------------
/**
Etapa 1)
Varre todo o grafo e identifica objetos que estão na ponta dos ramos mortos.
Percorre o ramo e vai marcando os objetos para deleção.
Etapa 2)
Percorre todas as conecções de todos os objetos e marca para deleção
os objetos que foram marcados para deleção
Etapa 3)
Elimina as conecções para os objetos marcados para deleção
Etapa 4)
Elimina os objetos marcados para deleção
Etapa 5)
Realiza uma recursão até que nenhum objeto seja deletado.
*/
void CGra3Dby2D::EliminarObjetosRedundantes_2 () 
{
   // O numero de objetos no ultimo plano é o numero total de objetos
   // menos o indice do ultimo objeto do solver
   unsigned int numeroObjetosNoPlanoZn = objeto.size () - rotuloUltimoObjetoPlanoN_1;

   unsigned long int numeroObjetosDeletadosNestaPassagem;
   unsigned long int numeroTotalObjetosDeletados = 0;

   /// @test: Como deve deletar tudo numa única passagem, não precisa do do..while?
   do {
      int deletado = objeto.size (); // deletado é apenas um número?
      numeroObjetosDeletadosNestaPassagem = 0;
      // ------------------------------------------------
      // A função MarcarParaDelecaoObjeto percorre os objetos do grafo
      // e marca para deleção os que fazem parte de ramo morto.
	  // Todo o ramo é marcado para deleção.
      for (int i = 0; i < objeto.size (); i++)
      {
         MarcarParaDelecaoObjeto (i);
      }

      // ------------------------------------------------
      // Percorrer todos os objetos e marcar para deleção cada link invalidado
      COGSitio *obj = nullptr;
      for (int i = 0; i < objeto.size (); i++)
      {
         obj = dynamic_cast < COGSitio * >(objeto[i]);
         assert (obj); //? exceção ??
         // No objeto COGSitio tem função nova:
         // virtual bool DeletarConeccoesInvalidadas(int deletado);
         obj->DeletarConeccoesInvalidadas (deletado);
      }

      // ------------------------------------------------
      // Reorganizar efetivamente o vetor dos objetos
      int indice_rotulo_valido = 0;
      // Percorre todos os objetos
      for (int i = 0; i < objeto.size (); i++)
         // Se o objeto para quem aponta não foi deletado, armazena no vetor dos objetos;
         // Se foi deletado vai ser pulado.
         if (objeto[i]->rotulo != deletado)
         {
            objeto[indice_rotulo_valido++] = objeto[i];
         }
      // Redimensiona o vetor das conecções (as que apontam para objetos deletados são eliminadas)
      // note que V2 chama resize, não deletando efetivamente o vetor de objetos.
      objeto.resize (indice_rotulo_valido);

      numeroObjetosDeletadosNestaPassagem = deletado - objeto.size ();
      cout << "numeroObjetosDeletadosNestaPassagem=" << numeroObjetosDeletadosNestaPassagem << endl;
   }
   while (numeroObjetosDeletadosNestaPassagem > 0);

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
   for (unsigned long int i = 0; i < objeto.size (); i++)
      objeto[i]->rotulo = i;

   // O rótulo do último objeto do solver precisa ser corrigido.
   // Dúvida: e se algum objeto do plano zn foi deletado ?
   // Resp: objetos do plano zn não são marcados para deleção.
   rotuloUltimoObjetoPlanoN_1 = objeto.size () - numeroObjetosNoPlanoZn;
}

bool
CGra3Dby2D::MarcarParaDelecaoObjeto (int i)
{
   // -------------------------------------------------
   // Marca a variável deletado como sendo o size do vetor de objetos.
   int deletado = objeto.size ();

   // -------------------------------------------------
   // Como vai percorrer todo o ramo morto e deletar em sequência, a função inclui chamadas recursivas,
   // o que explica os parâmetros deste if.
   // Uma conecção que já foi deletada (marcada com deletado) pode ser chamada 2x,
   // o que explica o if(..i != deletado)
   if (	// Se o objeto ja foi deletado, o indice i == deletado, deve ser desconsiderado.
		i != deletado		// sempre primeiro a verificar
		&&
		// Se o objeto já  foi deletado
		objeto[i]->rotulo != deletado &&
		// e esta no centro, vai verificar
		objeto[i]->Contorno () == CContorno::ETipoContorno::CENTER)
   {
      COGSitio *obj = dynamic_cast < COGSitio * >( objeto[i] );
      assert (obj); // exceção..

      // Obtêm o número de conecções
      int nlinks = obj->coneccao.size ();

      // Se 0 links, é um objeto isolado, então marca para deleção
      // ........*.........
      if (nlinks == 0)
         obj->rotulo = deletado;

	  // se tem um link (ramo duplo) então marca para deleção, e pede para verificar a conecção.
      // ........*<------->*........               
	  // note que objetos nos planos z=0 e z=n terão geralmente 1 link, por isso a condição
	  // if( ..objeto[i]->Contorno () == CContorno::ETipoContorno::CENTER)
      else if (nlinks == 1)
      {
         obj->rotulo = deletado;
         // Verifica o sítio a quem estou conectado (marca para deleção)
         MarcarParaDelecaoObjeto (obj->coneccao[0]->rotulo);
      }

      // se dois links
      // ou esta no meio de um ramo válido(e não deve ser deletado), 
      // z0........*<------->*<----->*<-------->*.....zn
      // ou esta entre dois sítios dos quais um já foi deletado.
      // ........*d------->*----->*-------->*...   
      else if (nlinks == 2)
      {
         // Se a coneccao[0] se refere a objeto já deletado então o número efetivo
		 // de conecções é 1, e precisa marcar para deletação.
         if (obj->coneccao[0]->rotulo == deletado)
         {
            obj->rotulo = deletado;
            // Solicita deleção da coneccao 1.
            MarcarParaDelecaoObjeto (obj->coneccao[1]->rotulo);
         }
         // Se a coneccao[1] se refere a objeto já deletado então o número efetivo
		 // de conecções é 1, e precisa marcar para deletação.
         /*else*/ if (obj->coneccao[1]->rotulo == deletado)
         {
            obj->rotulo = deletado;
            // Solicita deleção da coneccao 0.
            MarcarParaDelecaoObjeto (obj->coneccao[0]->rotulo);
         }
      }
   }

   return 1;
}

// -------------------------------------------------------------------------
// Função:  DeletarObjeto
// -------------------------------------------------------------------------
/*
@short  : Deleta objeto do grafo que tem o rotulo passado,
 e depois deleta todas as referencias a este objeto.
 Ou seja, um objeto A pode ter uma ligação para um objeto B, sem que B
 tenha uma ligação com A. Ao deletar o objeto B, precisa eliminar  a ligação A->B.

 Outros objetos do grafo podem apontar para o objeto deletado.
 Preciso percorrer todos os objetos do grafo
 e as ligações de cada objeto para eliminar qualquer referencia ao objeto
 que vai ser deletado???
 se algum objeto aponta para o objeto a ser deletado
 isto significa que ele tem conecção e que não deveria ser deletado???
 (Para o CGrafo3Dby2D este caso não deveria ocorrer)

        objeto[i]->coneccao[link]  // retorna ponteiro para objeto
        objeto[i]		 // retorna ponteiro para objeto

        Versão 1:
 Na primeira versão a informação dos links era armazenada, guardando-se
 o rotulo dos objetos.
        Ao eliminar um objeto do vector, o vector é todo reordenado
        e os endereços todos trocados, de forma que o rótulo estaria apontando para
 o objeto errado. Veja abaixo.

        Antes de deletar qualquer objeto
        	vector<> objeto  = 0,1,2,3,4,5,6,7,8...

        Depois de deletar o objeto 4
         vector<> objeto  = 0,1,2,3,5->4,6->5,7->6,8->7...
        Observe que o rotulo 5 apontava para o objeto 5 e agora aponta para o objeto 4.

        Na versão 2:
 Os objetos apontam diretamente para os outros objetos.
 A lista das conecções não armazena mais o rótulo do objeto, e sim um ponteiro para o 
 proprio objeto.
        Usava algo como: objeto[objeto[i]->rotulo]
        Agora os vetores coneccao armazenam o endereço do objeto
        e não o rótulo do objeto, de forma que a localização do objeto
        é feita diretamente.

 Antes de deletar um objeto, devo verificar seu endereço e deletar
 todos os links a ele, somente depois de todas as ligações ao objeto
 terem sido eliminadas é que posso deletar o próprio objeto.

@author :	André Duarte Bueno
@see    : grafos
@param  : Posição do objeto a ser deletado
@return : true se ok
*/
/*bool CGra3Dby2D::DeletarObjeto(int ri)
{
  COGSitio* obj_i =	dynamic_cast<COGSitio*>(sitio);
  assert( obj_i );
  return 1;
}
 */

/*
bool CGra3Dby2D::DeletarObjeto(unsigned long int rotulo) // ou rotulo
{
 // Ponteiro para o objeto
  COGSitio* obj_i =	dynamic_cast<COGSitio*>(objeto[rotulo]);
  // bug pois não deve acessar objeto usando rotulo
  assert( obj_i );

 // Percorre os links que devem ser 0 ou 1
  for ( unsigned long int  link = 0; link < obj_i->coneccao.size() ; link++ )
   if(obj_i->coneccao[link] == objeto[rotulo->bug])nao deve acessar objeto[rotulo]
       {
     vector<CObjetoGrafo*>::iterator  it_link = obj_i->coneccao.begin();
       obj_i->coneccao.erase(it_link + link);
       }
   vector<CObjetoGrafo*>::iterator  it = objeto.begin();

   objeto.erase(it + rotulo);
return 1;
}
*/

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
    @see    : grafos
    @param  :
    @return :	bool // ostream&

    @test: o nome do arquivo "grafo.vectorX" foi modificado para considerar o nome da imagem 
    pois pode processar mais de um arquivo no mesmo diretório ao mesmo tempo. 
    Como mudou precisa testar!
*/
bool CGra3Dby2D::LerVetorPropriedades_x ()
{
  // Abre arquivo de disco
  // ifstream fin ("grafo.vectorX"); 
  ifstream fin ( (NomeGrafo() + ".vectorX").c_str() );
  if (fin.fail ())
  { cerr << "Falha abertura arquivo: "<< (NomeGrafo() + ".vectorX") << " que tem os dados do grafo.\n";
    return 0;
  }
  string primeiraLinhaTemDimensaoGrafo;
  fin >> primeiraLinhaTemDimensaoGrafo;
  unsigned int dimensaoDoGrafo;
  fin >> dimensaoDoGrafo;
  // Lê os dados do vetor (atributo propriedade).
  for ( auto objeto_grafo : objeto )
    {
	  fin >> objeto_grafo->x ;

///  @bug: Teste do uso do tag bug do doxygen.
///  @test: o código abaixo poderia ser comentado pois não deve ocorrer!
      if (fin.eof ()) 
       return 0;
    }
  if( dimensaoDoGrafo != objeto.size() )
  {  cerr << "\nVerificar erro pois dimensao do grafo no arquivo é: " << dimensaoDoGrafo
	      << "\ne o números de objetos que foram lidos é: " << objeto.size() << '\n';
	 return 0;
  }
  return 1;
}

/// Salva no arquivo "grafo.vectorX" o valor da pressão de cada objeto.
bool CGra3Dby2D::SalvarVetorPropriedades_x ()
{
  // Abre arquivo de disco
  //  ofstream fout ("grafo.vectorX");
  ofstream fout ( (NomeGrafo()+ ".vectorX").c_str() );
  if (fout.fail ())
    return 0;
  fout << "dimensaoDoGrafo= " << objeto.size() << '\n'; // manter espaço depois =
  // Salva os dados do atributo x de cada objeto em disco (ex: pressão que esta sendo calculada).
  for (auto objeto_grafo : objeto )
    fout << objeto_grafo->x << '\n';
  return 1;
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
    @see    : grafos
    @param  :
    @return : bool indicando sucesso da operação.
    @todo: como faz cast para COGSitio, deveria estar em CGrafo3Dby2D ?
    @todo: receber matriz de double !! eliminando multiplicador 1e17.
    @todo: transformar em template que recebe tipo : float, double, long double.
    @todo: transformar CVetor em template!
    @test: testar para ver se esta funcionando!
*/
bool CGra3Dby2D::SetarMatrizAVetorB (TCMatriz2D< int > * &A, CVetor * &B) const
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
  // cout << "\nrotuloUltimoObjetoPlanoN_1="               <<       rotuloUltimoObjetoPlanoN_1;
  // cout << "\nrotuloPrimeiroObjetoPlano1="      <<  rotuloPrimeiroObjetoPlano1;

  // A dimensão do sistema de equações considera os planos 1->n-1,
  // pois os planos 0 e n-1 tem pressões constantes.
  unsigned int dim = rotuloUltimoObjetoPlanoN_1 - rotuloPrimeiroObjetoPlano1 + 1;
  // cout <<"\ndim="<<dim;

  // Redimensiona a matriz A
  A->Redimensiona (dim, dim);
  // Zera a matriz A (verificar necessidade)
  A->Constante (0);

  // Redimensiona o vetor B
  B->Redimensiona (dim);
  // Zera o vetor B  (verificar necessidade)
  B->Constante (0);
// renomear i->indiceConeccao e j=indiceObjeto
  unsigned int i;
  for (unsigned long int j = 0; j < objeto.size (); j++)
    {
    // Faz um cast para sítio derivado (em função do acesso a função Contorno e vetor coneccao).
    COGSitio *objeto_j = dynamic_cast < COGSitio * >(objeto[j]);
    assert (objeto_j);  // se não der certo o cast, vai lançar exceção!

    switch ( objeto_j->Contorno () )
	{
    // Fronteira esquerda/WEST
    case CContorno::ETipoContorno::WEST:
	// Fronteira direita/EST
    case CContorno::ETipoContorno::EST:
	  // Percorre as conecções do objeto
	  for (i = 0; i < objeto_j->coneccao.size (); i++)
	    {
	      /// Calcula Cij - @todo: explicar a equacao usada.
	      Cij = (objeto_j->coneccao[i]->propriedade + objeto_j->propriedade) / 2.0;
// para modelo3 M3 Cij = objeto_j->condutancia[i]
	      Cij = Cij * 1.0e17;	// LIXO, para gerar int
	      // cij esta sendo armazenado em int por isto multiplico por e17

	      // Desloca o índice da matriz(vetor), pois só entram os sítios
	      // que não estão na interface. ? Verificar ? e o último plano?
	      mi = objeto_j->coneccao[i]->rotulo - rotuloPrimeiroObjetoPlano1;

	      // Acumula Cij no vetor B[mi] -= Cij * objeto_j->x,
	      // x deve estar definido.
	      B->data1D[mi] -= static_cast < int >(Cij * objeto_j->x);

	      // Acumula -Cij na matriz A[mi][mi]
	      // A->data2D[mi][mi] -= Cij;
	      A->data2D[mi][mi] -= static_cast < int >(Cij);
	    }
	  break;
	  
	// Fronteira Centro
    case CContorno::ETipoContorno::CENTER:
	  // Percorre as conecções do objeto
	  for (i = 0; i < objeto_j->coneccao.size (); i++)
	    {
	      // Se o link for um objeto de centro (não contorno) entra
          if (objeto_j->coneccao[i]->Contorno () == CContorno::ETipoContorno::CENTER)
		  {
		  // Calcula Cij
		  Cij = (objeto_j->propriedade + objeto_j->coneccao[i]->propriedade) / 2.0;
// para modelo3 M3 Cij = objeto_j->condutancia[i]
		  Cij = Cij * 1.0e17;	// LIXO para gerar int
		  // cij esta sendo armazenado em int por isto multiplico por e17

		  // Desloca os índices da matriz
		  mi = objeto_j->coneccao[i]->rotulo - rotuloPrimeiroObjetoPlano1;
		  mj = objeto_j->rotulo - rotuloPrimeiroObjetoPlano1;

		  // Define A->data2D[mi][mj]      
		  A->data2D[mi][mj] = static_cast < int >(Cij);	// LIXO o static

		  // Acumula A->data2D[mj][mj]
		  A->data2D[mj][mj] -= static_cast < int >(Cij); // LIXO o static
		  }
	    }
	  break;
	}			// switch
    }				// for

    /// @todo: abaixo deve considerar o nome do arquivo/imagem que foi processada!
  
  A->Write (NomeGrafo() + ".matrixA"); // A->Write ("grafo.matrixA"); 
  B->Write (NomeGrafo() + ".vectorB"); // B->Write ("grafo.vectorB");
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
  // cout << "\nrotuloUltimoObjetoPlanoN_1="		<<	 rotuloUltimoObjetoPlanoN_1;
  // cout << "\nrotuloPrimeiroObjetoPlano1="	<<  rotuloPrimeiroObjetoPlano1;

	
  unsigned int dim = rotuloUltimoObjetoPlanoN_1 - rotuloPrimeiroObjetoPlano1 + 1 ;
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
      // Faz um cast para sítio derivado (em função do acesso a função Contorno e vetor coneccao.

      COGSitio* objeto_j = dynamic_cast<COGSitio*>(objeto[j]);
      assert(objeto_j);
  		
      switch( objeto_j->Contorno() )
	{
	  // Fronteira esquerda/WEST
	case CContorno::WEST :	
  			
	  // Fronteira direira
	case CContorno::EST :	
	  // Percorre as conecções do objeto	
	  for ( i = 0; i < objeto_j->coneccao.size(); i++)
	    {
	      // Calcula Cij
	      Cij = (objeto_j->coneccao[i]->propriedade +  objeto_j->propriedade   ) /2.0 ;
	      Cij = Cij* 1.0e17;	// LIXO, para gerar int
	      // cij esta sendo armazenado em int por isto multiplico por e17
	  							
	      // Desloca o índice da matriz(vetor), pois só entram os sítios
	      // que não estão na interface.
	      mi = objeto_j->coneccao[i]->rotulo - rotuloPrimeiroObjetoPlano1; // 3;
	  							
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
	  // Percorre as conecções do objeto	
	  for ( i = 0; i < objeto_j->coneccao.size(); i++)
	    {
	      // Se o link  for  um objeto de centro (não contorno) entra
	      if( objeto_j->coneccao[i]->Contorno() == CContorno::CENTER)
		{										
		  // Calcula Cij
		  Cij = ( objeto_j->propriedade + objeto_j->coneccao[i]->propriedade ) /2.0 ;
		  Cij = Cij* 1.0e17;// LIXO para gerar int
		  // cij esta sendo armazenado em int por isto multiplico por e17
    	  								
		  // Desloca os índices da matriz
		  mi = objeto_j->coneccao[i]->rotulo - rotuloPrimeiroObjetoPlano1 ;
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
