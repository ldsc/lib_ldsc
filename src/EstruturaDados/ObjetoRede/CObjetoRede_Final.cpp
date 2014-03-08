/**
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
  ===============================================================================
  Desenvolvido por:
                     Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
  @author:           André Duarte Bueno
  @file:             CObjetoRede_Final.cpp
  @begin:            Sat Sep 16 2000
  @copyright:        (C) 2000 by André Duarte Bueno
  @email:            andreduartebueno@gmail.com
*/
// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <map>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <EstruturaDados/ObjetoRede/CObjetoRede_Final.h>
using namespace std;

// -------------------------------------------------------------------------
// Função:       operator<<
// -------------------------------------------------------------------------
/** Escreve os atributos do objeto em disco.
    @short  :		Escreve os atributos do objeto em disco.
    Sobrecarga operador entrada streams (antiga CObjetoRede_FinalEsqueleto).
    @author :	André Duarte Bueno
    @see    :
    @param  : 	ostream& e CObjetoRede_Final&
    @return :		ostream&
*/
ostream& operator<< ( ostream& out, CObjetoRede_Final& s )
{
   s.Write ( out );
   return out;
}

// -------------------------------------------------------------------------
// Função:    operator>>
// -------------------------------------------------------------------------
/** Sobrecarga operador entrada streams
    @short  :	Sobrecarga operador entrada streams
    @author :	André Duarte Bueno
    @see    :
    @param  : istream& is, CObjetoRede_Final& s
    @return : istream&
*/

/*istream& operator>> (istream& is, CObjetoRede_Final& s)
  {

  // Lê a informação do tipo de contorno
  int contorno;// CContorno::ETipoContorno
  is >> contorno; // Descartada ??

  // Lê a informação do numero de ligações
  int numeroLigacoes;
  is >> numeroLigacoes;

  // Precisa deletar o vetor de conexões existente,
  // mas não os objetos apontados pelo vetor,
  // os CObjetoGrafo são alocados e deletados pelo CGrafo

  // Rótulo do objeto
  int rotulo;

  // Ponteiro para objeto
  CObjetoGrafo * objA;
  double propriedade;

  // Monta vetor das conexões
  for(unsigned long int i = 0 ; i < numeroLigacoes; i++)
  {
  // Lê o rotulo do objeto a quem estou conectado em i
  is >> rotulo;

  // Pesquisa e localiza objeto que tem este rotulo
  // retorna copia do ponteiro para objeto com o rotulo
  objA = grafo->FindObjeto(rotulo);	// Implementar Find Avançado (eficiente)

  conexao.push_back(objA);


  // Lê a propriedade
  is >> propriedade;
  // Armazena no ojeto ??
  conexao[i]->propriedade = propriedade;
  }

  return is;
  }   */

// -------------------------------------------------------------------------
// Função:  Go
// -------------------------------------------------------------------------
/** A função Go calcula o novo valor de x, considerando o relacionamento
    com os demais objetos a quem esta conectado.
    Note que x(a pressão do objeto) é desconsiderada, considera condutâncias
    e x(pressão dos vizinhos).

    @short  :
    Observe que calcula o novo valor de this->x (a pressão) e o retorna,
    mas não altera o valor de this->x.

    O novo valor de x, retornado por Go() pode ser usado externamente.

    Por exemplo,
    um solver externo vai usar esta nova previsão de x para definir,
    no momento adequado, o novo valor de this->x.
    ex: objeto->x = objeto->Go();

    Mudança de formato no acesso aos outros sítios:
    -----------------------------------------------
    A versão 1 de CGrafo usava o rótulo para identificar o objeto a ser acessado,
    mas era necessário receber o grafo todo.
    Agora o vetor conexão de cada sítio armazena direto o endereço dos sítios
    a quem esta conectado.
    Abaixo precisava do grafo para acessar os objetos
    somatorio_da_condutancia_vezes_x +=
        conexao[i]->propriedade * grafo->objeto[this->conexao[i]->rotulo]->x;

    Agora acessa os objetos diretamente, o rótulo do objeto esta sendo utilizado
    apenas para salvar o mesmo em disco.
    somatorio_da_condutancia_vezes_x +=
        conexao[i]->propriedade * grafo->objeto[i]->x;

    @author : André Duarte Bueno
    @see    :
    @param  : long double
    @return : long double
*/
long double CObjetoRede_Final::Go ( long double /*d */ )
{
   // Se this esta num contorno, apenas retorna o valor de x.
   if ( Tipo()     == ETipoObjetoGrafo::ObjetoRede_Sitio_EST
         or Tipo() == ETipoObjetoGrafo::ObjetoRede_Sitio_WEST
         or Tipo() == ETipoObjetoGrafo::ObjetoRede_Ligacao_EST
         or Tipo() == ETipoObjetoGrafo::ObjetoRede_Ligacao_WEST
         or Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Sitio_EST
         or Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Sitio_WEST
         or Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Ligacao_EST
         or Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Ligacao_WEST )
      return x;

   // Senão calcula novo valor de x usando conexões e conditâncias.
   // Ex: propriedade = condutancia;  x = pressao

   // Cria variáveis auxiliares (uma única vez, pois são estáticas)
   static long double somatorio_da_condutancia;
   static long double somatorio_da_condutancia_vezes_x;
   static long double condutancia;

   // Se não tem nenhum link, retorna x atual (a pressão atual)
   // tecnicamente nunca ocorre pois objetos sem conexão foram deletados!
   // if (conexao.size() == 0)
   //     return x;

   // zera o somatório (a cada passagem por Go)
   somatorio_da_condutancia_vezes_x = somatorio_da_condutancia = 0.0;

   for ( unsigned int i = 0; i < conexao.size(); i++ ) {
         // condutância entre this e o objeto conectado vezes x(pressão) do objeto_conectado
         somatorio_da_condutancia_vezes_x += this->condutancia[i] * conexao[i]->x;
         // Acumula a condutancia total
         somatorio_da_condutancia += this->condutancia[i];
      }

   // Somatorio (condutancia*pressao) / somatorio_da_condutancia
   // retorna um novo valor de x (pressão) sem alterar x diretamente.
   return somatorio_da_condutancia_vezes_x / somatorio_da_condutancia;
}
