/**
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
  ===============================================================================

  Desenvolvido por: 
                    Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
  @author:          André Duarte Bueno
  @file:            CObjetoRede_Sitio_CC.cpp
  @begin:           Sat Sep 16 2000
  @copyright:       (C) 2000 by André Duarte Bueno
  @email:           andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <Base/_LIB_LDSC_CLASS.h>
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Sitio_CC.h>

// -------------------------------------------------------------------------
// Função:       DeletarConeccao
// -------------------------------------------------------------------------
/** Deleta coneccoes de ramos mortos
  @short  : Deleta coneccoes de ramos mortos
  @author : André Duarte Bueno
  @see    : 
  @param  : unsigned int link
  @return : void
*/
void CObjetoRede_Sitio_CC::DeletarConeccao (unsigned int link)
{
  // Deleta a conecção
  this->coneccao.erase (coneccao.begin() + link);

  // e, adicionalmente, elimina a condutancia associada ao objeto link
  this->condutancia.erase (condutancia.begin () + link);
}

/** Marca e deleta os links para objetos invalidados (marcados para deleção).
  @short  : Deleta a coneccao de um ramo morto
  @author : André Duarte Bueno
  @see    : 
  @param  : unsigned int link
  @return : void
*/
bool CObjetoRede_Sitio_CC::DeletarConeccoesInvalidadas (unsigned int deletado)
{
  unsigned int indice_rotulo_valido {0};
  // Percorre todas as coneccoes
  for (unsigned int i = 0; i < coneccao.size (); i++)
    // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
    // Se foi deletado vai ser pulado.
    if (coneccao[i]->rotulo != deletado)
      {
	  coneccao[indice_rotulo_valido++] = coneccao[i];
	  condutancia[indice_rotulo_valido] = condutancia[i];
      }

  // Redimensiona o vetor das coneccoes.  (as que apontam para objetos deletados são eliminadas)
  coneccao.resize (indice_rotulo_valido);
  condutancia.resize (indice_rotulo_valido);
  return 1;
}

/** Deleta os links repetidos e acumula (soma) as condutâncias em paralelo.
  @short  : Deleta as coneccoes repetidas.
  @author : André Duarte Bueno
  @see    : 
  @param  :  
  @return : bool
  @todo   : testar!
  @todo   : da forma como esta funciona, mas pode ser mais simples!
*/
// NOVO EM TESTE
int CObjetoRede_Sitio_CC::DeletarConeccoesRepetidas_e_SomarCondutanciasParalelo ()
{
  // Número de links deletados
  // acumula número de links no início
  unsigned int numeroLinksDeletados = coneccao.size ();

  // Cria uma cópia do vetor das coneccoes, ordena e elimina repeticoes
  vector < CObjetoRede * > c (coneccao);

  // Funcao EliminaDuplicatas para containers (pg487 stroustrup)
  {
    sort (c.begin (), c.end ());
    vector < CObjetoRede * >::iterator p = unique (c.begin (), c.end ());
    c.erase (p, c.end ());
  }

  // Cria um map que associa o link com as condutâncias
  std::map < CObjetoRede *, long double > m;

  // Copia para o map os links (já ordenados e únicos de c) e inicializa os valores com 0.
  for (unsigned int i = 0; i < c.size (); i++)
    m[c[i]] = 0.0;

  // Percorre as coneccoes e acumula as condutâncias no map
  //    map.insert( container_map::value_type( chave, valor ) );
  for (unsigned int i = 0; i < coneccao.size (); i++)
    m[coneccao[i]] += condutancia[i];

  //   for ( iter = listatelefones.begin(); iter != listatelefones.end(); ++iter )
  //   cout << setw(campo)<<iter->first << setw(20)<< iter->second ;
  for (unsigned int i = 0; i < c.size (); i++)
    {
      coneccao[i] = c[i];
      condutancia[i] = m[c[i]];
    }
  coneccao.resize (c.size ());
  condutancia.resize (c.size ());

  numeroLinksDeletados = numeroLinksDeletados - c.size ();
  return numeroLinksDeletados;
}

/**
 * /// @todo tentativa otimizar DeletarConeccoesRepetidas_e_SomarCondutanciasParalelo
 * terminar de implementar, testar velocidade, ficar com a versao + rápida.
int CObjetoRede_Sitio_CC::DeletarConeccoesRepetidas_V2 ()
{
  // cria um vetor de pair.
  vector < pair<CObjetoRede *, long double> > par_conexao_condutancia(conexao.size());
  // preenche o par.
  for (unsigned int i = 0 ;  i< coneccao.size(); i++)
     par_conexao_condutancia[i] = make_pair( conexao[i], condutancia[i]);
  // ordena o par considerando como critério a conexão.
  sort(begin(par_conexao_condutancia), end(par_conexao_condutancia),
       []( pair<CObjetoRede *, long double> par ) { par.first }  ); // corrigir comparação

  // Acumular as condutancias repetidas  
  ...use equalrange para localizar os trechos com condutancias repetidas.
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
Lista_dos_rotulos_das_coneccoes
@author : André Duarte Bueno
@see    :
@param  : Recebe uma referencia para uma ostream
@return : void
*/
ostream & CObjetoRede_Sitio_CC::Write (ostream & out) const
{
    out.setf ( ios::right );

    // Tipo de contorno
    /// @todo trocar por tipo ojeto grafo!
    out << setw ( 4 ) << static_cast<unsigned char> ( Contorno() ) << '\n';

    // Rótulo de this
    out << ' ' << setw ( 5 ) << rotulo;

    // x de this (pressão)
    out << ' ' << setw ( 10 ) << x;

    // propriedade de this (condutancia)
    out << ' ' << setw ( 10 ) << propriedade;

    // Numero de links do sítio
    out << ' ' << setw ( 4 ) << coneccao.size ();

    // lista dos rótulos da coneccao
    for ( auto objeto_conectado : coneccao )
        out << ' ' << setw ( 4 ) << objeto_conectado->rotulo;

  // Lista das propriedades (condutancias das ligações)
  for ( auto condutancia_i : condutancia ) 
    out << ' ' << setw (12) << condutancia_i;
  
  return out;
}

// -------------------------------------------------------------------------
// Função:       operator<<
// -------------------------------------------------------------------------
/** Escreve os atributos do objeto em disco.
    @short  :		Escreve os atributos do objeto em disco.
    Sobrecarga operador entrada streams (antiga CObjetoRede_Sitio_CCEsqueleto).
    @author :	André Duarte Bueno
    @see    :	
    @param  : 	ostream& e CObjetoRede_Sitio_CC&
    @return :		ostream&
*/
ostream & operator<< (ostream & out, CObjetoRede_Sitio_CC & s)
{
  s.Write(out);
  return out;
}

// -------------------------------------------------------------------------
// Função:       Fluxo
// -------------------------------------------------------------------------
/** Determina o fluxo associado a este sítio.
    @short  :
    Fluxo = Condutancia da ligação vezes a  pressao deste sítio menos  a
    pressao do sítio conexo.
    Nesta classe, considera a condutancia da ligacao.
    @author : André Duarte Bueno
    @see    :
    @param  : void
    @return : long double (o fluxo associado a this)
    @todo   : verificar se vale a pena substituir long double por long double&& (uso move)
*/
long double CObjetoRede_Sitio_CC::Fluxo () const
{
  long double fluxo = 0.0;

  // Calculo considerando a condutancia da ligação
  for (unsigned long int i = 0; i < coneccao.size (); i++)
    {
      fluxo += this->condutancia[i] * (this->x - coneccao[i]->x);
    }
  return fluxo;
}

// -------------------------------------------------------------------------
// Função:  Go
// -------------------------------------------------------------------------
/** Calcula o valor da propriedade x (a pressão)
    @short  : Veja CObjetoRede_Sitio->Go para detalhes do funcionamento.
    Aqui foi redefinida, pois considera a condutancia da coneccão obtida do atributo
    vector <long double> condutancia;
    Ex:
    popriedade = condutancia_do_sitio
    x  = pressao
    condutancia[i] = condutancia_da_ligacao_i

    @author : André Duarte Bueno
    @see    :
    @param  : long double
    @return : long double
*/
long double CObjetoRede_Sitio_CC::Go (long double /*d */ )
{
  // Cria variáveis auxiliares (uma única vez, pois são estáticas)
  static long double somatorio_da_condutancia;
  static long double somatorio_da_condutancia_vezes_x;

  // zera o somatorio (a cada passagem por Go)
  somatorio_da_condutancia_vezes_x = somatorio_da_condutancia = 0.0;

	// Se não tem nenhum link, retorna x atual (a pressão atual)
	// tecnicamente nunca ocorre pois objetos sem conexão foram deletados!
	//   if (coneccao.size() == 0)
	//     return x;

  // Percorre as conecções
  // Observe que o numero de conecções deve ser igual ao numero de condutancias.
  // Note que não dá para usar ranged for de C++11 pois lida com dois vetores diferentes.
  for (unsigned long int i = 0; i < coneccao.size (); i++)
    {
      // Usa a condutancia[i] e a coneccao[i]
      somatorio_da_condutancia_vezes_x += this->condutancia[i] * coneccao[i]->x;

      // Acumula a condutancia total
      somatorio_da_condutancia += this->condutancia[i];
    }

  // retorna um novo valor de x (pressão)
  return somatorio_da_condutancia_vezes_x / somatorio_da_condutancia;
}
