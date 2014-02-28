/**
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
  ===============================================================================
  Desenvolvido por:
                     Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
  @author:           André Duarte Bueno
  @file:             COGSitio.cpp
  @begin:            Sat Sep 16 2000
  @copyright:        (C) 2000 by André Duarte Bueno
  @email:            andreduartebueno@gmail.com
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

/// @todo: definir um padrão para debugagem.
// DEBUG ofstream        COGSitio::fout;
// DEBUG int             COGSitio::numeroSitios=0;

// -------------------------------------------------------------------------
// Função:               Conectar
// -------------------------------------------------------------------------
/** Recebe um objA e o conecta a this.
    @author :	André Duarte Bueno
    @see    :
    @param  : objeto a quem será conectado
    @return : void
*/
void COGSitio::Conectar (CObjetoGrafo * objA, CObjetoGrafo *)
{
  coneccao.push_back (objA);
}

// -------------------------------------------------------------------------
// Função:  Go
// -------------------------------------------------------------------------
/** A função Go calcula o novo valor de x, considerando o relacionamento 
    com os demais objetos a quem esta conectado.
    @short  :
    Observe que calcula o novo valor de this->x (a pressão) e o retorna,
    mas não altera o valor de this->x.

    O novo valor de x, retornado por Go() pode ser usado externamente.

    Por exemplo, 
    um solver externo vai usar esta nova previsão de x para definir, 
    no momento adequado, o novo valor de this->x.

    Mudança de formato no acesso aos outros sítios:
    -----------------------------------------------
    A versão 1 de CGrafo usava o rótulo para identificar o objeto a ser acessado,
    mas era necessário receber o grafo todo.
    Agora o vetor conecção de cada sítio armazena direto o endereço dos sítios
    a quem esta conectado.
    Abaixo precisava do grafo para acessar os objetos
    somatorio_da_condutancia_vezes_x += 
        coneccao[i]->propriedade * grafo->objeto[this->coneccao[i]->rotulo]->x;

    Agora acessa os objetos diretamente, o rótulo do objeto esta sendo utilizado
    apenas para salvar o mesmo em disco.

    @author : André Duarte Bueno
    @see    :
    @param  : long double
    @return : long double
*/
long double COGSitio::Go (long double /*d */ )
{
  // Ex:
  // propriedade = condutancia
  // x                                     =       pressao

  // Cria variáveis auxiliares (uma única vez, pois são estáticas)
  static long double somatorio_da_condutancia;
  static long double somatorio_da_condutancia_vezes_x;
  static long double condutanciaMedia;

  // Se não tem nenhum link, retorna x atual (a pressão atual)
  if (coneccao.size() == 0)
    return x;

  // zera o somatorio (a cada passagem por Go)
  somatorio_da_condutancia_vezes_x = somatorio_da_condutancia = 0.0;
  // Percorre as conecções
  for ( auto objetoConectado: coneccao )
    {
      // faz uma média das condutancias
      condutanciaMedia = (objetoConectado->propriedade + this->propriedade) / 2.0;
      somatorio_da_condutancia_vezes_x += condutanciaMedia * objetoConectado->x;

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
   @short  : Determina o fluxo associado a este sítio.
   Fluxo = Condutancia média vezes a pressao deste sítio menos  a pressao do objeto conexo
   @author : André Duarte Bueno
   @see    :
   @param  : void
   @return : long double ( o fluxo associado a this)
*/
long double COGSitio::Fluxo () const 
{
  long double fluxo { 0.0 };
  static long double condutanciaMedia;
  for ( auto objetoConectado: coneccao )
    {
      // Ex:
      // propriedade = condutancia
      // x           = pressao
      condutanciaMedia = (objetoConectado->propriedade + this->propriedade) / 2.0;
      fluxo += condutanciaMedia * (this->x - objetoConectado->x);
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
  //out << ' ' << setw (4) << Contorno ();
  out << setw (4) << static_cast<unsigned char>( Contorno() ) << '\n';

  // Número de links (coneccoes)
  unsigned long int numeroLinks = coneccao.size ();
  out << ' ' << setw (4) << numeroLinks;

  // Lista dos rótulos
  for (auto objeto : coneccao)
    out << ' ' << setw (6) << objeto->rotulo;

  // Lista das propriedades (condutancias)
  for (auto objeto : coneccao)
    out << ' ' << setw (10) << objeto->propriedade;

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
  out << setw (4) << static_cast<unsigned char>( Contorno() ) << '\n';

  // Rótulo de this
  out << ' ' << setw (5) << rotulo;

  // propriedade de this (condutancia)
  out << ' ' << setw (10) << propriedade;

  // x de this (pressão)
  out << ' ' << setw (10) << x;

  // Número de links (coneccoes)
  unsigned long int numeroLinks = coneccao.size ();
  out << ' ' << setw (4) << numeroLinks;

  // lista dos rótulos de quem estou conexo
  for (auto objeto : coneccao )
    out << ' ' << setw (5) << objeto->rotulo;

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
  out << setw (4) << static_cast<unsigned char>( s.Contorno() ) << '\n';

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
    @todo   : implementar esta função.
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
void COGSitio::DeletarConeccao (unsigned int link)
{
//   vector < CObjetoGrafo * >::iterator it_link = this->coneccao.begin ();
//   this->coneccao.erase (it_link + link);
  this->coneccao.erase ( coneccao.begin() + link );
}

/** Marca e deleta as conecções para objetos invalidados (marcados para deleção).
 * Funciona assim: percorre os objetos das conecções,
 * se o rótulo do objeto correspond	e a rótulo válido (não deletado),
 * então a conexão é preservada.
 * Já os objetos que foram marcados para deleção são desconsiderados(deletados);
 * isto é, se a conecção foi deletada, aqui ela é desconsiderada (apagada).
    @short  : Deleta a coneccao de um ramo morto
    @author : André Duarte Bueno
    @see    : 
    @param  : unsigned int link
    @return : void
    @todo   : Pode-se otimizar o consumo de memória eliminando objetos deletados após resize.
*/
bool COGSitio::DeletarConeccoesInvalidadas (int deletado)
{
  unsigned int indice_rotulo_valido {0};
  
  // Percorre todas as coneccoes
  for ( auto objeto: coneccao )
    // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
    // Se foi deletado vai ser pulado.
    if (objeto->rotulo != deletado)
      {
       coneccao[indice_rotulo_valido++] = objeto;
      }

  // Redimensiona o vetor das coneccoes (as que apontam para objetos deletados são eliminadas)
  coneccao.resize (indice_rotulo_valido);
  /// @todo: aqui pode apagar, usando erase, os objetos além do size().
  return 1;
}
