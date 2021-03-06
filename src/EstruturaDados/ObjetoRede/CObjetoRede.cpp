/**
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
  ===============================================================================
  Desenvolvido por:
                     Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
  @author:           André Duarte Bueno
  @file:             CObjetoRede.cpp
  @begin:            Sat Sep 16 2000
  @copyright:        (C) 2000 by André Duarte Bueno
  @email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <iostream>
// #include <vector>
#include <iomanip>
#include <algorithm>
#include <map>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <EstruturaDados/ObjetoRede/CObjetoRede.h>
using namespace std;

// -------------------------------------------------------------------------
// Função:               Conectar
// -------------------------------------------------------------------------
/**
 * @brief Função que recebe um ponteiro para um CObjetoRede objA e o inclue na lista de conexões.
 * Faz a conexão de this com o objeto recebido. Usada por objetos do tipo Sitio.
 * Note que oculta versão que recebe CObjetoGrafo.
 * @author : André Duarte Bueno
 * @param  : objeto a quem será conectado
 * @return : void
*/
inline void CObjetoRede::Conectar ( CObjetoRede* objA )
{
    this->conexao.push_back ( objA );
    this->condutancia.push_back ( objA->propriedade );
}

// -------------------------------------------------------------------------
// Função:               Conectar
// -------------------------------------------------------------------------
/**
* @brief Função que recebe um ponteiro para um CObjetoRede objA e o inclue na lista de conexões.
* Faz a conexão de this com o objeto recebido. Usada por objetos do tipo Sitio.
* A condutância entre this e o objA é o segundo parâmetro (a _condutancia é calculada externamente).
 * @author : André Duarte Bueno
 * @param  : objeto objA a quem será conectado e condutancia entre this e objA.
 * @return : void
*/
inline void CObjetoRede::Conectar ( CObjetoRede* objA, long double _condutancia )
{
    this->conexao.push_back ( objA );
    this->condutancia.push_back ( _condutancia );
}

// -------------------------------------------------------------------------
// Função:               Conectar
// -------------------------------------------------------------------------
/**
* @brief Função que recebe um ponteiro para um CObjetoRede objA e um ponteiro para um objeto B
* e os inclue na lista de conexões. Usada por objetos do tipo Ligação.
* Faz a conexão de this com os objetos recebidos.
* Note que oculta versão que recebe CObjetoGrafo.
* Note que como não recebe a condutância, vai fazer a condutancia igual a
* propriedade do objeto ao qual estou conectado:
* conexao[i] = objA;
* condutancia[i] = objA.propriedade;
* @author : André Duarte Bueno
* @param  : objetos objA e objB a quem será conectado
* @return : void
*/
/*inline */ void CObjetoRede::Conectar ( CObjetoRede* objA, CObjetoRede* objB )
{
    this->conexao.push_back ( objA );
    this->condutancia.push_back ( objA->propriedade );
    this->conexao.push_back ( objB );
    this->condutancia.push_back ( objB->propriedade );
}

// -------------------------------------------------------------------------
// Função:               Conectar
// -------------------------------------------------------------------------
/**
* @brief Função que recebe um ponteiro para um CObjetoRede objA e um ponteiro para um objeto B
* e os inclue na lista de conexões. Usada por objetos do tipo Ligação.
* Faz a conexão de this com os objetos recebidos.
* Note que oculta versão que recebe CObjetoGrafo.
* Note que como não recebe a condutância, vai fazer a condutancia igual a
* propriedade do objeto ao qual estou conectado:
* conexao[i] = objA;
* condutancia[i] = _condutanciaA;
* @author : André Duarte Bueno
* @param  : objeto objA a quem será conectado e condutancia entre this e objA.
* @param  : objeto objB a quem será conectado e condutancia entre this e objB.
* @return : void
*/
/*inline */ void CObjetoRede::Conectar ( CObjetoRede* objA, long double _condutanciaA ,
        CObjetoRede* objB , long double _condutanciaB )
{
    this->conexao.push_back ( objA );
    this->condutancia.push_back ( _condutanciaA );
    this->conexao.push_back ( objB );
    this->condutancia.push_back ( _condutanciaB );
}

// -------------------------------------------------------------------------
// Função:       DeletarConexao
// -------------------------------------------------------------------------
/** Deleta conexões de ramos mortos
  @short  : Deleta conexões de ramos mortos
  @author : André Duarte Bueno
  @param  : unsigned int pos
  @return : void
  Nota: o código abaixo pode deixar o processo lento se o número de conexões foram
  grande e se este método for muito chamado! Pensar em usar <list>
*/
void CObjetoRede::DeletarConexao ( unsigned int pos )
{
    // Deleta a conexão
    this->conexao.erase ( conexao.begin() + pos );

    // e, adicionalmente, deleta a condutancia associada ao objeto pos
    this->condutancia.erase ( condutancia.begin () + pos );
}

// -------------------------------------------------------------------------
// Função:       DeletarConexao
// -------------------------------------------------------------------------
/**
  @short  : Recebe um vetor com o índice das conexões a serem deletadas.
  (Deleta várias conexões ao mesmo tempo).
  @author : André Duarte Bueno
  @param  : vetor com índice i dos objetos a serem deletados.
  @return : void
*/
void CObjetoRede::DeletarConexao ( vector<unsigned int> di )
{
    unsigned int marcar_para_delecao = conexao.size();
    for ( unsigned int i = 0; i < di.size (); i++ )
        // Se di[i] for um rótulo inválido para conexao ocorre estouro de pilha!
        conexao[ di[i] ]->rotulo =  marcar_para_delecao;

    // Chama  DeletarConexoesInvalidadas que deleta conexões marcadas para deleção.
    this->DeletarConexoesInvalidadas ( marcar_para_delecao );
}

/** Marca e deleta os links para objetos invalidados (marcados para deleção).
  @short  : Deleta a conexao de um ramo morto
  @author : André Duarte Bueno

  @param  : unsigned int marcada_para_delecao
  @return : void
*/
bool CObjetoRede::DeletarConexoesInvalidadas ( unsigned int marcado_para_delecao )
{
    unsigned int indice_rotulo_valido {0};

    // Percorre todas as conexões
    for ( unsigned int i = 0; i < conexao.size (); i++ )

        // Se o objeto para quem aponta não foi marcado_para_delecao, armazena no vetor das conexões.
        // Se foi marcado_para_delecao vai ser pulado.
        if ( conexao[i]->rotulo != marcado_para_delecao ) {
            conexao[indice_rotulo_valido] = conexao[i];
            condutancia[indice_rotulo_valido] = condutancia[i];
            indice_rotulo_valido++;
        }

    // Redimensiona o vetor das conexões e deleta elementos não usados.
    conexao.resize ( indice_rotulo_valido );
    conexao.erase ( conexao.begin() + indice_rotulo_valido , conexao.end () );

    // Redimensiona o vetor das condutâncias e deleta elementos não usados.
    condutancia.resize ( indice_rotulo_valido );
    condutancia.erase ( condutancia.begin() + indice_rotulo_valido , condutancia.end () );
    return 1;
}

/**
  @short  : Deleta as conexões repetidas.
  @author : André Duarte Bueno

  @param  :
  @return : bool
  @todo   : da forma como esta funciona, mas pode ser mais simples! testar!
*/
unsigned int  CObjetoRede::DeletarConexoesRepetidas_e_SomarCondutanciasParalelo()
{
    // Número de links deletados
    // acumula número de links no início
    unsigned int numeroLinksDeletados = conexao.size ();

    // Cria uma cópia do vetor das conexões, ordena e elimina repeticoes
    vector < CObjetoRede* > c ( conexao );

    // Funcao EliminaDuplicatas para containers (pg487 stroustrup)
    sort ( c.begin (), c.end () );        // c sai de escopo e é deletado!

    // Cria um map que associa o link com as condutâncias
    std::map < CObjetoRede*, long double > m;

    // Copia para o map os links (já ordenados e únicos de c) e inicializa os valores com 0.
    for ( unsigned int i = 0; i < c.size (); i++ )
        m[c[i]] = 0.0;

    // Percorre as conexões e acumula as condutâncias no map
    //    map.insert( container_map::value_type( chave, valor ) );
    for ( unsigned int i = 0; i < conexao.size (); i++ )
        m[conexao[i]] += condutancia[i];

    //   for ( iter = listatelefones.begin(); iter != listatelefones.end(); ++iter )
    //   cout << setw(campo)<<iter->first << setw(20)<< iter->second ;
    for ( unsigned int i = 0; i < c.size (); i++ ) {
        conexao[i] = c[i];
        condutancia[i] = m[c[i]];
    }

// #ifdef OTIMIZAR_MEMORIA
    // Redimensiona o vetor das conexões e deleta elementos não usados.
    conexao.resize ( c.size () );
    conexao.erase ( conexao.begin() + c.size () , conexao.end () );

    // Redimensiona o vetor das condutâncias e deleta elementos não usados.
    condutancia.resize ( c.size () );
    condutancia.erase ( condutancia.begin() + c.size () , condutancia.end () );
// #endif

    numeroLinksDeletados = numeroLinksDeletados - c.size ();
    return numeroLinksDeletados;
}

/**
 * /// @todo tentativa otimizar DeletarConexoesRepetidas_e_SomarCondutanciasParalelo
 * terminar de implementar, testar velocidade, ficar com a versao + rápida.
int CObjetoRede::DeletarConeccoesRepetidas_V2 ()
{
  // cria um vetor de pair.
  vector < pair<CObjetoRede *, long double> > par_conexao_condutancia(conexao.size());
  // preenche o par.
  for (unsigned int i = 0 ;  i< conexao.size(); i++)
     par_conexao_condutancia[i] = make_pair( conexao[i], condutancia[i]);
  // ordena o par considerando como critério a conexão.
  sort(begin(par_conexao_condutancia), end(par_conexao_condutancia),
       []( pair<CObjetoRede *, long double> par ) { par.first }  ); // corrigir comparação

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
@param  : Recebe uma referencia para uma ostream
@return : void
*/
ostream& CObjetoRede::Write ( ostream& out ) const
{
    out.setf ( ios::left );

    // Tipo de contorno
		out << setw ( 5 ) << static_cast<uint8_t> ( Tipo() ) ;

    // Rótulo de this
    out << ' ' << setw ( 5 ) << rotulo;

    // x de this (pressão)
    out << ' ' << setw ( 10 ) << x;

    // propriedade de this (condutancia)
    out << ' ' << setw ( 10 ) << propriedade;

    // Numero de links do sítio
    out << ' ' << setw ( 5 ) << conexao.size ();

    // lista dos rótulos da conexao
		for ( auto &objeto_conectado : conexao )
        out << ' ' << setw ( 5 ) << objeto_conectado->rotulo;

    // Lista das propriedades (condutâncias das ligações)
		for ( auto &condutancia_i : condutancia )
        out << ' ' << setw ( 10 ) << condutancia_i;

    //out << '\n';

    return out;
}

// -------------------------------------------------------------------------
// Função:       operator<<
// -------------------------------------------------------------------------
/** Escreve os atributos do objeto em disco.
@short  :		Escreve os atributos do objeto em disco.
Sobrecarga operador entrada streams (antiga CObjetoRedeEsqueleto).
@author :	André Duarte Bueno
@param  : 	ostream& e CObjetoRede&
@return :		ostream&
*/
ostream& operator<< ( ostream& out, CObjetoRede& s )
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

@param  : istream& is, CObjetoRede& s
@return : istream&
*/

/*istream& operator>> (istream& is, CObjetoRede& s)
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
/** 
* A função Go calcula o novo valor de x, considerando o relacionamento
com os demais objetos a quem esta conectado.
Note que x(ex:a pressão do objeto) é desconsiderada, considera condutâncias
e x(ex:pressão dos vizinhos).

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
@param  : long double
@return : long double
*/
long double CObjetoRede::Go ( long double /*d */ )
{
    // Ex:
    // propriedade = condutancia
    // x           = pressao

    // Cria variáveis auxiliares (uma única vez, pois são estáticas)
    static long double somatorio_da_condutancia;
    static long double somatorio_da_condutancia_vezes_x;

    // Se não tem nenhuma conexão, retorna x atual (a pressão atual)
    // tecnicamente nunca ocorre pois objetos sem conexão foram deletados!
    // if (conexao.size() == 0)
    //     return x;

    // zera o somatório (a cada passagem por Go)
    somatorio_da_condutancia_vezes_x = somatorio_da_condutancia = 0.0;

    for ( unsigned int i = 0; i < conexao.size (); i++ ) {
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
   Fluxo = Condutancia média vezes a pressao deste sítio menos a pressao do objeto conexo
   @author : André Duarte Bueno
   @param  : void
   @return : long double ( o fluxo associado a this)
*/
long double CObjetoRede::Fluxo () const
{
    static long double fluxo ;
    fluxo =  0.0 ; // zera a cada passagem

    for ( unsigned long int i = 0; i < conexao.size (); i++ ) {
        // Ex: se x é a  pressao; calcula condutancia * dP
        fluxo += condutancia[i] * ( this->x - conexao[i]->x );
    }

    return fluxo;
}
