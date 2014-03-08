/**
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
  ===============================================================================
  Desenvolvido por:
                     Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
  @author:           André Duarte Bueno
  @file:             CObjetoRede_Tipo.cpp
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
#include <EstruturaDados/ObjetoRede/CObjetoRede_Tipo.h>
using namespace std;

/** Marca e deleta os links para objetos invalidados (marcados para deleção).
  @short  : Deleta a conexao de um ramo morto
  @author : André Duarte Bueno
  @see    :
  @param  : unsigned int link
  @return : void
*/
bool CObjetoRede_Tipo::DeletarConexoesInvalidadas( unsigned int deletado )
{
   unsigned int indice_rotulo_valido {0};

   // Percorre todas as conexões
   for( unsigned int i = 0; i < conexao.size(); i++ )

      // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
      // Se foi deletado vai ser pulado.
      if( conexao[i]->rotulo != deletado ) {
            conexao[indice_rotulo_valido] = conexao[i];
            condutancia[indice_rotulo_valido] = condutancia[i];
            indice_rotulo_valido++;
         }

   // Redimensiona o vetor das conexões e deleta elementos não usados.
   conexao.resize( indice_rotulo_valido );
   conexao.erase( conexao.begin() + indice_rotulo_valido , conexao.end() );

   // Redimensiona o vetor das condutâncias e deleta elementos não usados.
   condutancia.resize( indice_rotulo_valido );
   condutancia.erase( condutancia.begin() + indice_rotulo_valido , condutancia.end() );
   return 1;
}

/**
  @short  : Deleta as conexões repetidas.
  @author : André Duarte Bueno
  @see    :
  @param  :
  @return : bool
  @todo   : da forma como esta funciona, mas pode ser mais simples! testar!
*/
unsigned int  CObjetoRede_Tipo::DeletarConexoesRepetidas_e_SomarCondutanciasParalelo()
{
   // Número de links deletados
   // acumula número de links no início
   unsigned int numeroLinksDeletados = conexao.size();

   // Cria uma cópia do vetor das conexões, ordena e elimina repeticoes
   vector < CObjetoRede_Tipo * > c( conexao );

   // Funcao EliminaDuplicatas para containers (pg487 stroustrup)
   {
      sort( c.begin(), c.end() );
//     vector < CObjetoRede_Tipo * >::iterator p = unique (c.begin (), c.end ());
//     c.erase (p, c.end ()); c vai ser deletado final da função!
   }

   // Cria um map que associa o link com as condutâncias
   std::map < CObjetoRede_Tipo *, long double > m;

   // Copia para o map os links (já ordenados e únicos de c) e inicializa os valores com 0.
   for( unsigned int i = 0; i < c.size(); i++ )
      m[c[i]] = 0.0;

   // Percorre as conexões e acumula as condutâncias no map
   //    map.insert( container_map::value_type( chave, valor ) );
   for( unsigned int i = 0; i < conexao.size(); i++ )
      m[conexao[i]] += condutancia[i];

   //   for ( iter = listatelefones.begin(); iter != listatelefones.end(); ++iter )
   //   cout << setw(campo)<<iter->first << setw(20)<< iter->second ;
   for( unsigned int i = 0; i < c.size(); i++ ) {
         conexao[i] = c[i];
         condutancia[i] = m[c[i]];
      }
      
// #ifdef OTIMIZAR_MEMORIA
   // Redimensiona o vetor das conexões e deleta elementos não usados.
   conexao.resize( c.size() );
   conexao.erase( conexao.begin() + c.size() , conexao.end() );

   // Redimensiona o vetor das condutâncias e deleta elementos não usados.
   condutancia.resize( c.size() );
   condutancia.erase( condutancia.begin() + c.size() , condutancia.end() );
//#endif
   numeroLinksDeletados = numeroLinksDeletados - c.size();
   return numeroLinksDeletados;
}

/**
 * /// @todo tentativa otimizar DeletarConexoesRepetidas_e_SomarCondutanciasParalelo
 * terminar de implementar, testar velocidade, ficar com a versao + rápida.
int CObjetoRede_Tipo::DeletarConeccoesRepetidas_V2 ()
{
  // cria um vetor de pair.
  vector < pair<CObjetoRede_Tipo *, long double> > par_conexao_condutancia(conexao.size());
  // preenche o par.
  for (unsigned int i = 0 ;  i< conexao.size(); i++)
     par_conexao_condutancia[i] = make_pair( conexao[i], condutancia[i]);
  // ordena o par considerando como critério a conexão.
  sort(begin(par_conexao_condutancia), end(par_conexao_condutancia),
       []( pair<CObjetoRede_Tipo *, long double> par ) { par.first }  ); // corrigir comparação

  // Acumular as condutâncias repetidas
  ...use equalrange para localizar os trechos com condutâncias repetidas.
  //
  for ( unsigned int i = equalrange.ini;  i <  equalrange.end; i++ )
     condutanciaTotal += condut[i]
  condutancia[equalrange.ini] = condutanciaTotal;
  erase(conexao.begin()+equalrange.ini+1, conexao.begin()+ equalrange.end);
  erase(condutancia.begin()+equalrange.ini+1, condutancia.begin()+ equalrange.end);
}
*/

// -------------------------------------------------------------------------
// Função:       Write
// -------------------------------------------------------------------------
/** Salva dados do objeto sítio em novo formato.
@short  :
Formato novo Write (Andre Format):
---------------------------------
NumeroSitios  // salvo pelo grafo
Tipo
Rotulo
propriedade   // raio hidraulico ou condutancia
x             // pressão
NumeroConeccoes
Lista_dos_rotulos_das_conexões
@author : André Duarte Bueno
@see    :
@param  : Recebe uma referencia para uma ostream
@return : void
*/
/*inline*/ ostream & CObjetoRede_Tipo::Write( ostream & out ) const
{
   out.setf( ios::right );

   // Tipo de contorno
   out << setw( 5 ) << static_cast<uint8_t>( Tipo() ) << '\n';

   // Rótulo de this
   out << ' ' << setw( 5 ) << rotulo;

   // x de this (pressão)
   out << ' ' << setw( 10 ) << x;

   // propriedade de this (condutancia)
   out << ' ' << setw( 10 ) << propriedade;

   // Numero de links do sítio
   out << ' ' << setw( 5 ) << conexao.size();

   // lista dos rótulos da conexao
   for( auto objeto_conectado : conexao )
      out << ' ' << setw( 5 ) << objeto_conectado->rotulo;

   // Lista das propriedades (condutâncias das ligações)
   for( auto condutancia_i : condutancia )
      out << ' ' << setw( 10 ) << condutancia_i;

   return out;
}

// -------------------------------------------------------------------------
// Função:       operator<<
// -------------------------------------------------------------------------
/** Escreve os atributos do objeto em disco.
    @short  :		Escreve os atributos do objeto em disco.
    Sobrecarga operador entrada streams (antiga CObjetoRede_TipoEsqueleto).
    @author :	André Duarte Bueno
    @see    :
    @param  : 	ostream& e CObjetoRede_Tipo&
    @return :		ostream&
*/
ostream & operator<< ( ostream & out, CObjetoRede_Tipo & s )
{
   s.Write( out );
   return out;
}

// -------------------------------------------------------------------------
// Função:    operator>>
// -------------------------------------------------------------------------
/** Sobrecarga operador entrada streams
    @short  :	Sobrecarga operador entrada streams
    @author :	André Duarte Bueno
    @see    :
    @param  : istream& is, CObjetoRede_Tipo& s
    @return : istream&
*/

/*istream& operator>> (istream& is, CObjetoRede_Tipo& s)
  COPIAR DE CObjetoRede
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
long double CObjetoRede_Tipo::Go( long double /*d */ )
{
   // Se this esta num contorno, apenas retorna o valor de x.
   if( Tipo() ==  ETipoObjetoGrafo::ObjetoRede_Sitio_EST
         or Tipo() == ETipoObjetoGrafo::ObjetoRede_Sitio_WEST
         or Tipo() ==  ETipoObjetoGrafo::ObjetoRede_Ligacao_EST
         or Tipo() == ETipoObjetoGrafo::ObjetoRede_Ligacao_WEST
         or Tipo() ==  ETipoObjetoGrafo::ObjetoEsqueleto_Sitio_EST
         or Tipo() == ETipoObjetoGrafo::ObjetoEsqueleto_Sitio_WEST
         or Tipo() ==  ETipoObjetoGrafo::ObjetoEsqueleto_Ligacao_EST
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

   for( unsigned int i = 0; i < conexao.size(); i++ ) {
         // condutância entre this e o objeto conectado vezes x(pressão) do objeto_conectado
         somatorio_da_condutancia_vezes_x += this->condutancia[i] * conexao[i]->x;
         // Acumula a condutancia total
         somatorio_da_condutancia += this->condutancia[i];
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
long double CObjetoRede_Tipo::Fluxo() const
{
   static long double fluxo ;
   fluxo =  0.0 ; //zera a cada passagem

   for( unsigned long int i = 0; i < conexao.size(); i++ ) {
         // Ex: x = pressao
         fluxo += condutancia[i] * ( this->x - conexao[i]->x );
      }

   return fluxo;
}

