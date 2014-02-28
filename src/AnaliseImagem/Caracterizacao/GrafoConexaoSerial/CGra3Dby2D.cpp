/*
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================

Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
   [LDSC].
@author:          André Duarte Bueno
@file:             CGra3Dby2D.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno   CTime
@email:            andreduartebueno@gmail.com
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

/*
template <class T>
void ImprimeDeque(const deque<T> & dequeRef, ofstream& eob);

 // copia de volta
 eob << "Copiando da lista para deque" << endl ;
 list<unsigned int>::iterator it = lista.begin();
 for(int i = 0; i < listaObjetosEliminados.size() ;   i++)
  {
  listaObjetosEliminados[i] = *it;
  it++;
  }
*/

/**Determina o grafo lendo os planos da imagem diretamente do disco
O atributo plano e utilizado para armazenar no objeto criado
a informacao do plano a que pertence. Sera usado para estimacao
da pressao inicial.*/
CGrafo * CGra3Dby2D::Go (string fileName, unsigned long int funcao) 
{
   unsigned long int i, j, k;
   maiorRotuloUtilizado = 0;

   ifstream fin (fileName.c_str ());
   if (fin.fail())
   {
      cerr << "Nao conseguiu abrir o arquivo " << fileName.c_str ();
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
// {} é usado em lista de inicialização e usado para criar objetos
//   ra = new CRotulador2DCm {nx, ny}; ok
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
   cout << "\n plano = " << plano << flush;
   for (j = 0; j < ny; j++)
      for (i = 0; i < nx; i++)
      {
         fin >> valor;
         img2D->data2D[i][j] = valor;
      }

   // Rotula plano 0
   ra->Go (img2D);

   // Determina raio hidraulico dos objetos
   ra->CalculaRaioHidraulicoObjetos ();

   // Define o contorno como sendo da face WEST (plano 0)
   tipoContornoObjeto = CContorno::ETipoContorno::WEST;

   // Adciona ao grafo os objetos do plano 0
   AdicionarObjetos (ra, maiorRotuloUtilizado, tipoContornoObjeto);

   // Determina o primeiro objeto do solver
   // Tarefa: verificar retorno de ra->RotuloFinal() = último rótulo utilizado
   // se for último rótulo utilizado precisa somar 1
   firstObjectOfSolver = maiorRotuloUtilizado + ra->RotuloFinal ();	// +1

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
            fin >> valor;
            img2D->data2D[i][j] = valor;
         }
      // Rotula
      rp->Go (img2D);

      // Determina raio hidraulico
      rp->CalculaRaioHidraulicoObjetos ();

      // Se for o ultimo plano, redefine o contorno e calcula lastObjectOfSolver
      if (k == (nz - 1))
      {
         tipoContornoObjeto = CContorno::ETipoContorno::EST;
         lastObjectOfSolver = maiorRotuloUtilizado + ra->RotuloFinal ();
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
   // Elimina sítios com 0 links
   {
      CTime ("Tempo processamento função EliminarObjetosRedundantes= ", &cout);
      EliminarObjetosRedundantes ();
   }

   return this;
}

/**
@short  :Cria os objetos de rotulagem ra,rp,
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
CGrafo * CGra3Dby2D::Go (TCMatriz3D<int> * _img3D, unsigned long int funcao) {

   // Armazena a informacao das dimensoes da imagem
   nx = _img3D->NX ();
   ny = _img3D->NY ();
   nz = _img3D->NZ ();

   // Usados para percorrer  a imagem
   unsigned long int i, j, k;

   maiorRotuloUtilizado = 0;

   // Cria 2 objetos de rotulagem 2D
   // Rotulador para imagem anterior
   ra = new CRotulador2DCm (nx, ny);
   assert (ra);

   // Rotulador para imagem posterior
   rp = new CRotulador2DCm (nx, ny);
   assert (rp);

   // Ponteiro temporário para troca dos planos de rotulagem
   CRotulador2DCm *rt;

   // Matriz imagem bidimensional passada para rotulador->Go(matriz)
   // Contém uma copia do plano3D a ser analisado
   img2D = new TCMatriz2D< int > (nx, ny);
   assert (img2D);

   // ------------
   // PLANO 0
   // ------------
   plano = k = 0;
   cout << "\nplano " << plano << " " <<flush;
   for (i = 0; i < nx; i++)
      for (j = 0; j < ny; j++)
         img2D->data2D[i][j] = _img3D->data3D[i][j][k];

   // Rotula plano 0
   ra->Go (img2D);

   // Calcula os vetores area e perimetro do plano 0
   ra->CalculaRaioHidraulicoObjetos ();

   // Chama função que adiciona objetos rotulados (ra) ao vetor de objetos,
   // Passa o tipo do contorno (WEST)
   tipoContornoObjeto = CContorno::ETipoContorno::WEST;

   // Adiciona objetos no grafo
   AdicionarObjetos (ra, maiorRotuloUtilizado, tipoContornoObjeto);

   // Determina o primeiro objeto do solver
   firstObjectOfSolver = maiorRotuloUtilizado + ra->RotuloFinal ();	// +1

   // ----------------------------------------------------------------------------
   // Percorre todos os demais planos
   // ----------------------------------------------------------------------------
   // Define o tipo de contorno como sendo de objeto central
   // (não é contorno esquerdo nem direito)
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

      // Calcula os vetores area e perimetro
      rp->CalculaRaioHidraulicoObjetos ();

      // Se for o último plano muda o tipo de contorno,
      // de forma a criar um CObjetoGrafo situado na face direita
      if (k == (nz - 1))
      {
         tipoContornoObjeto = CContorno::ETipoContorno::EST;
         lastObjectOfSolver = maiorRotuloUtilizado + ra->RotuloFinal ();
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
/**
@short  :	Função que adiciona a lista de objetos do grafo,
     os objetos identificados em rotulador.
@author :	André Duarte Bueno
@see    :
@param  : Recebe a imagem rotulada com os objetos a serem incluídos,	
   o número do ultimo rótulo utilizado e o
   tipo de contorno (identifica o objeto a ser criado:
   COGSitio_EST = 0, COGSitio_CENTER = 1,  COGSitio_EST = 2)
@return : void
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
      data = GetObjetoGrafo (tipoContornoObjeto);
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
// Função EliminarObjetosRedundantes()
// -----------------------------------------------------------------------------
/** Elimina  sítios com 0 links;
Pelo que me lembro, percorre  o grafo, considerando cada plano.
Localiza objeto com uma ligação e o elimina.
Repete o procedimento num do..while até que não ocorram mais deleções.

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
   COGSitio *objetoSitio1 = nullptr;
   // e aquele que aponta para 1 (2)
   COGSitio *objetoSitio2 = nullptr;

   // Número total de passagens realizadas
   unsigned long int numeroPassagens = 0;

   // Numero de objetos deletados em cada passagem e numero total de objetos deletados
   unsigned long int numeroObjetosDeletados ;
   unsigned long int numeroTotalObjetosDeletados = 0;

   // Número de links do objeto 1 e 2
   unsigned long int numeroLinks1;
   unsigned long int numeroLinks2;

   // Rotulo do objeto 1 em análise
   unsigned long int rotulo;

   // Cria-se um deque com o rotulo dos objetos eliminados
   // permitindo a fácil identificação dos objetos que precisam,
   // ser deletados no vetor cmx e cmy
   deque < unsigned int >listaObjetosEliminados;

   // O numero de objetos no ultimo plano é o numero total de objetos
   // menos o indice do ultimo objeto do solver.
   unsigned int numeroObjetosNoPlanoZn = objeto.size () - lastObjectOfSolver;

   // Vai varrer os planos e eliminar objetos
   do {
      numeroObjetosDeletados = 0;

      // Percorre todos os objetos do grafo
      // Tarefa: i< lastObjectOfSolver
      for (long int i = firstObjectOfSolver; i < (objeto.size () - numeroObjetosNoPlanoZn); i++) 
      {
         rotulo = objeto[i]->rotulo;	// =i+1

         // Um objeto[i] é um ponteiro para um CObjetoGrafo;

         // Aqui sempre trabalho com objetos do tipo COGSitio,
         // e preciso fazer um dynamic_cast para poder acessar o vetor das conecções
         objetoSitio1 = dynamic_cast < COGSitio * >(objeto[i]);

         // Pega o número de coneccoes do objeto 1
         numeroLinks1 = objetoSitio1->coneccao.size ();

         // Se numeroLinks== 0 ligações,
         if (numeroLinks1 == 0) 
	 {
            // Adiciona a lista de objetos eliminados o rotulo do objeto atual
            listaObjetosEliminados.push_back (rotulo);

            // Deleta o objeto i
            delete objeto[i];
            // Aponta para zero
            objeto[i] = nullptr;

            // Incrementa o numero de objetos deletados
            numeroObjetosDeletados++;
         }
         // Se numeroLinks ==  1 ligação, pode eliminar este sítio,
         // mas deve eliminar os links dos outros para ele
         else if (numeroLinks1 == 1) 
	 {
            // --------------------
            // INICIO FUNCAO DELETAROBJETO (objeto 1)
            // Ponteiro para o objeto 2
            objetoSitio2 = dynamic_cast < COGSitio * >(objetoSitio1->coneccao[0]);

            // Verifica o numero de conecções de quem estou conectado (de 2)
            numeroLinks2 = objetoSitio2->coneccao.size ();

            // Percorre todas as conecções de 2 e procura aquelas que apontam para 1
            // percorre de tras para frente
            for (long int link = (numeroLinks2 - 1); link >= 0; link--)
            {
               // se o link em 2 aponta para 1

               if (objetoSitio2->coneccao[link] == objetoSitio1)
               {
                  // Se for COGSitio deleta somente a conecção (modelos 1 e 2)
                  // Se for COGSitio_LR deleta a conecção e a condutancia (modelos 3,4,5,..)
                  objetoSitio2->DeletarConeccao (link);	// desativada em COGSitio e COGSitio_LR

                  // Mesmo depois de deletar a primeira ligação, deve verificar as demais
                  // pois pode haver mais de uma ligação entre dois objetos (como no modelo1).
               }

            }
            // Fim funcao deletar OBJETO
            // --------------------

            // Agora deleta o proprio objeto 1
            listaObjetosEliminados.push_back (rotulo);
            delete objeto[i];
            objeto[i] = nullptr;
            numeroObjetosDeletados++;
         }
      }			// for objeto.size()

      // Mudar: Criar novo vetor, copiar objetos validos e então deletar vetor velho,
      // vai ficar mais rápido.
      vector < CObjetoGrafo * >::iterator it = objeto.begin ();

      // Para facilitar a deleção dos objetos deleta do ultimo para o primeiro
      for (int j = objeto.size () - 1; j >= 0; j--)	// novo>=0
         if ( objeto[j] == nullptr )
         {
            objeto.erase (it + j);
         }

      numeroTotalObjetosDeletados += numeroObjetosDeletados;
      cout << "numeroObjetosDeletados=" << numeroObjetosDeletados 
           << " numeroTotalObjetosDeletados =" << numeroTotalObjetosDeletados << endl;
   }
   while (numeroObjetosDeletados > 0);

   // Chama função que reorganiza os valores de cmx e cmy
   // visto que boa parte dos objetos do grafo foram eliminados
   // e preciso corrigir isto nos vetores de cmx e cmy
   // Chamar antes de reorganizar os rótulos
   ReorganizarCmxCmy ();

   // Após deletar todos os objetos válidos e chamar ReorganizarCmxCmy,
   // pesquisa todos os objetos e faz rotulos sequenciais.
   // se o rotulo do objeto e seu indice, nao precisa armazenar o rotulo ?
   for (unsigned long int i = 0; i < objeto.size (); i++)
      objeto[i]->rotulo = i;
   // O numero do ultimo objeto do solver precisa ser corrigido.
   // BUG ? e se algum objeto do plano zn foi deletado ?// objetos do plano zn não são verificados.
   lastObjectOfSolver = objeto.size () - numeroObjetosNoPlanoZn;
}

// -----------------------------------------------------------------------------
// Função EliminarObjetosRedundantes()
// -----------------------------------------------------------------------------
/** Função EliminarObjetosRedundantes() v2
Etapa 1)
Varre todo o grafo e identifica objetos que estão na ponta dos ramos mortos.
Percorre o ramo e vai marcando os objetos para deleção.
Etapa 2)
Percorre todas as coneccoes de todos os objetos e marca para deleção
os objetos que foram marcados para deleção
Etapa 3)
Elimina as coneccoes para os objetos marcados para deleção
Etapa 4)
Elimina os objetos marcados para deleção
Etapa 5)
Realiza uma recursão até que nenhum objeto seja deletado.

Comparação:
EliminarObjetosRedundantes_1 : img 300 demorou 14min41seg
EliminarObjetosRedundantes_2 : img 300 demorou 12seg
*/
void CGra3Dby2D::EliminarObjetosRedundantes_2 () 
{
   // O numero de objetos no ultimo plano é o numero total de objetos
   // menos o indice do ultimo objeto do solver
   unsigned int numeroObjetosNoPlanoZn = objeto.size () - lastObjectOfSolver;

   unsigned long int numeroObjetosDeletados;
   unsigned long int numeroTotalObjetosDeletados = 0;

   do {
      int deletado = objeto.size ();
      numeroObjetosDeletados = 0;
      // ------------------------------------------------
      // Marcar para deleção cada objeto do grafo
      // Chama Deletar objeto para todos os objetos do grafo
      for (int i = 0; i < objeto.size (); i++)
      {
         MarcarParaDelecaoObjeto (i);
      }

      // ------------------------------------------------
      // Marcar para deleção cada link invalidado
      // Para todos os objetos
      for (int i = 0; i < objeto.size (); i++)
      {
         COGSitio *obj = dynamic_cast < COGSitio * >(objeto[i]);
         assert (obj);
         obj->DeletarConeccoesInvalidadas (deletado);
         // no objeto COGSitio tem função nova:
         // virtual bool DeletarConeccoesInvalidadas(int deletado);
      }

      // ------------------------------------------------
      // Reorganizar efetivamente o vetor dos objetos
      int cont = 0;
      // Percorre todos os objetos
      for (int i = 0; i < objeto.size (); i++)
         // Se o objeto para quem aponta não foi deletado, armazena no vetor dos objetos
         // Se foi deletado vai ser pulado.
         if (objeto[i]->rotulo != deletado)
         {
            objeto[cont++] = objeto[i];
         }
      // Redimensiona o vetor das coneccoes (as que apontam para objetos deletados são eliminadas)
      objeto.resize (cont);

      numeroObjetosDeletados = deletado - objeto.size ();
      cout << "NumeroObjetosDeletados=" << numeroObjetosDeletados << endl;
   }
   while (numeroObjetosDeletados > 0);

   // Chama função que reorganiza os valores de cmx e cmy
   // visto que boa parte dos objetos do grafo foram eliminados
   // é preciso corrigir isto nos vetores de cmx e cmy
   // Chamar antes de reorganizar os rótulos
   ReorganizarCmxCmy ();

   // Reorganizar os rótulos
   // Após deletar todos os objetos válidos e chamar ReorganizarCmxCmy
   // Pesquisa todos os objetos e faz rotulos sequenciais
   // se o rotulo do objeto e seu indice, nao precisa armazenar o rotulo ?
   for (unsigned long int i = 0; i < objeto.size (); i++)
      objeto[i]->rotulo = i;

   // O numero do ultimo objeto do solver precisa ser corrigido.
   // BUG ? e se algum objeto do plano zn foi deletado ?// objetos do plano zn não são verificados.
   lastObjectOfSolver = objeto.size () - numeroObjetosNoPlanoZn;	// VERIFICAR AQUI
}

bool
CGra3Dby2D::MarcarParaDelecaoObjeto (int i)
{
   // -------------------------------------------------
   // Anota a informação dos objetos deletados
   int deletado = objeto.size ();

   // -------------------------------------------------
   if (	// Se o objeto ja foi deletado, o indice i que recebi pode ser == a deletado
                  i != deletado		// sempre primeiro a verificar
                  &&
                  // Se o objeto já  foi deletado
                  objeto[i]->rotulo != deletado &&
                  // e esta no centro, vai verificar
                  objeto[i]->Contorno () == CContorno::ETipoContorno::CENTER)
   {
      COGSitio *obj = dynamic_cast < COGSitio * >(objeto[i]);
      assert (obj);

      // Obtêm o número de links
      int nlinks = obj->coneccao.size ();

      // -------------------------------------------------
      // ........*.........
      // Se 0 links, marca para deleção
      if (nlinks == 0)
         obj->rotulo = deletado;

      // -------------------------------------------------
      // ........*------->*........               // se um link (ramo curto), marca para deleção
      // ........*------->*----->*-------->*...   // se um link (aponta para ramo maior), marca para deleção
      else if (nlinks == 1)
      {
         obj->rotulo = deletado;

         // Verifica o sítio a quem estou conectado (marca para deleção)
         MarcarParaDelecaoObjeto (obj->coneccao[0]->rotulo);
      }
      // -------------------------------------------------
      // se dois links
      // ou esta no meio de um ramo válido, ou esta entre dois sítios dos quais um já foi deletado
      else if (nlinks == 2)
      {
         // Aconeccao[0] se refere a objeto já deletado

         if (obj->coneccao[0]->rotulo == deletado)
         {
            // Deleta i
            obj->rotulo = deletado;
            // Solicita deleção da coneccao 1.
            MarcarParaDelecaoObjeto (obj->coneccao[1]->rotulo);
         }
         // Aconeccao[1] se refere a objeto já deletado
         if (obj->coneccao[1]->rotulo == deletado)
         {
            // Deleta i
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
/**
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
 Os objetos apontam diretamente parar os outros objetos.
 A lista das coneccoes não armazena mais o rótulo do objeto, e sim um ponteiro para o 
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
// #endif
