/*
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: TPadrao_ramo
  ===============================================================================

  Desenvolvido por:	Laboratorio de Desenvolvimento de Software Cientifico
  	[LDSC].
  @author:          André Duarte Bueno
  File:             COGSitio.cpp
  begin:            Sat Sep 16 2000
  copyright:        (C) 2000 by André Duarte Bueno
  email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>
#include <iomanip>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/COGSitio.h>
using namespace std;

// DEBUG ofstream        COGSitio::fout;
// DEBUG int                     COGSitio::numeroSitios=0;

// -------------------------------------------------------------------------
// Função:               Conectar
// -------------------------------------------------------------------------
/** Recebe um objA e o conecta a this
    @author :	André Duarte Bueno
    @see    :
    @param  : objeto a quem será conectado
    @return : void
*/
void
COGSitio::Conectar (CObjetoGrafo * objA, CObjetoGrafo *)
{
  coneccao.push_back (objA);
}

// -------------------------------------------------------------------------
// Função:  Go
// -------------------------------------------------------------------------
/** A função Go calcula o novo valor de x, considerando o relacionamento 
    com os demais objetos a quem esta conectado.
    @short  :
    Observe que calcula o novo  valor de this->x (a pressão),
    mas não altera o valor de this->x.
					
    O novo valor de x, retornado por Go() pode ser usado externamente.
					
    Por exemplo, um solver externo
    vai usar esta nova previsão de x para definir no momento adequado o
    novo valor de this->x.
					
    Mudança de formato no acesso aos outros sítios:
    -----------------------------------------------
    A versão 1 de CGrafo usava o rótulo para identificar o objeto a ser acessado,
    mas era necessário receber o grafo todo.
    Agora o vetor conecção de cada sítio armazena direto o endereço dos sítios
    a quem esta conectado.
    Abaixo precisava do grafo para acessar os objetos
    somatorio_da_condutancia_vezes_x += coneccao[i]->propriedade *
    grafo->objeto[this->coneccao[i]->rotulo]->x;
    Agora acessa os objetos diretamente, o rótulo do objeto esta sendo utilizado
    apenas para salvamento do mesmo em disco.

    @author :	André Duarte Bueno
    @see    :
    @param  : long double
    @return :	long double
*/
long double
COGSitio::Go (long double /*d */ )
{
  // Ex:
  // propriedade = condutancia
  // x                                     =       pressao

  // Cria variáveis auxiliares (uma única vez, pois são estáticas)
  static long double somatorio_da_condutancia;
  static long double somatorio_da_condutancia_vezes_x;
  static long double condutanciaMedia;

  // zera o somatorio (a cada passagem por Go)
  somatorio_da_condutancia_vezes_x = somatorio_da_condutancia = 0;

  // Se não tem nenhum link, retorna x atual (a pressão atual)
  unsigned long int numeroLinks = coneccao.size ();
  if (numeroLinks == 0)
    return x;

  // Percorre as conecções
  for (unsigned long int i = 0; i < numeroLinks; i++)
    {
      // faz uma média das condutancias                
      condutanciaMedia =
	(this->coneccao[i]->propriedade + this->propriedade) / 2.0;
      somatorio_da_condutancia_vezes_x +=
	condutanciaMedia * this->coneccao[i]->x;

      // Acumula a condutancia total
      somatorio_da_condutancia += condutanciaMedia;
    }

  // Somatorio (condutancia*pressao) / somatorio_da_condutancia
  // retorna um novo valor de x (pressão) sem alterar x diretamente.
  return somatorio_da_condutancia_vezes_x / somatorio_da_condutancia;
}


// -------------------------------------------------------------------------
// Função:       Fluxo
// -------------------------------------------------------------------------
/**
   @short  :   Determina o fluxo associado a este sítio.
   Fluxo = Condutancia da ligação vezes, a
   pressao deste sítio menos  a pressao do sítio conexo
   @author :	André Duarte Bueno
   @see    :
   @param  : void
   @return : long double ( o fluxo associado a this)
*/
long double
COGSitio::Fluxo () const 
{
  long double fluxo = 0;
  static long double condutanciaMedia;
  for (unsigned long int i = 0; i < coneccao.size (); i++)
    {
      // Ex:
      // propriedade = condutancia
      // x                                         =       pressao
      condutanciaMedia =
	(this->coneccao[i]->propriedade + this->propriedade) / 2.0;
      fluxo += condutanciaMedia * (this->x - coneccao[i]->x);
    }
  return fluxo;
}

// -------------------------------------------------------------------------
// Função:   Write_Liang_Format
// -------------------------------------------------------------------------
/**Função  que salva os dados do sitio em disco, no mesmo formato utilizado pelo Liang.
   @short  :
   Abaixo o formato antigo Write_Liang_Format:
   -------------------------------------------
   NumeroSitios
   Tipo
   Rotulo
   NumeroConeccoes
   Lista_dos_rotulos
   Lista_das_condutancias

   @author :André Duarte Bueno
   @see    :
   @param  :ostream& out
   @return :ostream&
*/
ostream & COGSitio::Write_Liang_Format (ostream & out) const 
{
  // Tipo de contorno
  out << ' ' << setw (4) << Contorno ();

  // Número de links (coneccoes)
  unsigned long int
    numeroLinks = coneccao.size ();
  out << ' ' << setw (4) << numeroLinks;

  // Lista dos rótulos
  unsigned long int
    cont_link;
  for (cont_link = 0; cont_link < numeroLinks; cont_link++)
    out << ' ' << setw (6) << coneccao[cont_link]->rotulo;

  // Lista das propriedades (condutancias)
  for (cont_link = 0; cont_link < numeroLinks; cont_link++)
    out << ' ' << setw (10) << coneccao[cont_link]->propriedade;

  return out;
}

// -------------------------------------------------------------------------
// Função:       Write
// -------------------------------------------------------------------------
/** Salva dados do objeto sítio em novo formato.
    @short  : 	
    Formato novo (Andre Format):
    ----------------------------
    NumeroSitios  		// salvo pelo grafo
    Tipo
    Rotulo
    propriedade 		// raio hidraulico ou condutancia
    x           		// pressão
    NumeroConeccoes
    Lista_dos_rotulos_das_coneccoes

    @author :		André Duarte Bueno
    @see    :
    @param  :   Recebe uma referencia para uma ostream
    @return :		ostream&
*/
ostream & COGSitio::Write (ostream & out) const
{
  out.setf (ios::right);

  // Tipo de contorno
  out << ' ' << setw (3) << Contorno ();



  // Rótulo de this
  out << ' ' << setw (5) << rotulo;

  // propriedade de this (condutancia)
  out << ' ' << setw (10) << propriedade;

  // x de this (pressão)
  out << ' ' << setw (10) << x;

  // Número de links (coneccoes)
  unsigned long int
    numeroLinks = coneccao.size ();
  out << ' ' << setw (4) << numeroLinks;

  // lista dos rótulos de quem estou conexo
  for (unsigned long int cont_link = 0; cont_link < numeroLinks; cont_link++)
    out << ' ' << setw (5) << coneccao[cont_link]->rotulo;

  // lista das propriedades (condutancias)





  // for(       int cont_link = 0;cont_link < numeroLinks ;cont_link++)
  //       out <<' ' << setw(4) << coneccao[cont_link]->propriedade ;
  return out;
}

// -------------------------------------------------------------------------
// Função:       operator<<
// -------------------------------------------------------------------------
/** Escreve os atributos do objeto em disco.
    @short  :	Sobrecarga operador entrada streams (antiga COGSitioEsqueleto).
    @author :	André Duarte Bueno
    @see    :	
    @param  :	ostream& e COGSitio&
    @return :	ostream&
*/
ostream & operator<< (ostream & out, COGSitio & s)
{

  // Tipo de contorno
  out << ' ' << setw (3) << s.Contorno ();

  // Rótulo de this
  out << ' ' << setw (5) << s.rotulo;

  // Propriedade de this (condutancia)
  out << ' ' << setw (10) << s.propriedade;

  // x de this (pressão)
  out << ' ' << setw (10) << s.x;

  // Número de links (coneccoes)
  unsigned long int
    numeroLinks = s.coneccao.size ();
  out << ' ' << setw (4) << numeroLinks;

  // lista dos rótulos de quem estou conexo
  for (unsigned long int cont_link = 0; cont_link < numeroLinks; cont_link++)
    out << ' ' << setw (5) << s.coneccao[cont_link]->rotulo;

  return out;
}

// -------------------------------------------------------------------------
// Função:    operator>>
// -------------------------------------------------------------------------
/** Sobrecarga operador entrada streams
    @short  :	Sobrecarga operador entrada streams
    @author :	André Duarte Bueno
    @see    :
    @param  : istream& is, COGSitio& s
    @return : istream&
*/

/*istream& operator>> (istream& is, COGSitio& s)
  {

  // Lê a informação do tipo de contorno
  int contorno;// CContorno::ETipoContorno
  is >> contorno; // Descartada ??

  // Lê a informação do numero de ligações
  int numeroLigacoes;
  is >> numeroLigacoes;

  // Precisa deletar o vetor de conecções existente,
  // mas não os objetos apontados pelo vetor,
  // os CObjetoGrafo são alocados e deletados pelo CGrafo

  // Rótulo do objeto
  int rotulo;

  // Ponteiro para objeto
  CObjetoGrafo * objA;
  double propriedade;

  // Monta vetor das conecções
  for(unsigned long int i = 0 ; i < numeroLigacoes; i++)
  {
  // Lê o rotulo do objeto a quem estou conectado em i
  is >> rotulo;

  // Pesquisa e localiza objeto que tem este rotulo
  // retorna copia do ponteiro para objeto com o rotulo
  objA = grafo->FindObjeto(rotulo);	// Implementar Find Avançado (eficiente)

  coneccao.push_back(objA);


  // Lê a propriedade
  is >> propriedade;
  // Armazena no ojeto ??
  coneccao[i]->propriedade = propriedade;
  }

  return is;
  }   */

// -------------------------------------------------------------------------
// Função:       DeletarConeccao
// -------------------------------------------------------------------------
/** Deleta a coneccao de um ramo morto
    @short  :		Deleta a coneccao de um ramo morto
    @author :		André Duarte Bueno
    @see    :	
    @param  : 	unsigned int link
    @return :		void
*/
void
COGSitio::DeletarConeccao (unsigned int link)
{
  vector < CObjetoGrafo * >::iterator it_link = this->coneccao.begin ();
  this->coneccao.erase (it_link + link);
}

/** Marca e deleta os links para objetos invalidados (marcados para deleção).
    @short  :		Deleta a coneccao de um ramo morto
    @author :		André Duarte Bueno
    @see    :	
    @param  : 	unsigned int link
    @return :		void
*/
bool
COGSitio::DeletarConeccoesInvalidadas (int deletado)
{
  int cont = 0;
  // Percorre todas as coneccoes
  for (int nl = 0; nl < coneccao.size (); nl++)
    // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
    // Se foi deletado vai ser pulado.
    if (coneccao[nl]->rotulo != deletado)
      {
	coneccao[cont++] = coneccao[nl];
      }

  // Redimensiona o vetor das coneccoes.  (as que apontam para objetos deletados são eliminadas)
  coneccao.resize (cont);
  return 1;
}
