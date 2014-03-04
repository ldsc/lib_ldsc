/**
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
  ===============================================================================
  Desenvolvido por:
                     Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
  @author:           André Duarte Bueno
  @file:             CObjetoRede_Sitio.cpp
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede_Sitio.h>
using namespace std;

// -------------------------------------------------------------------------
// Função:               Conectar
// -------------------------------------------------------------------------
/** Recebe um objA e o conecta a this.
    @author :	André Duarte Bueno
    @see    :
    @param  : objeto a quem será conectado
    @return : void
    NOTA: mesmo código de CObjetoGrafo_1VetorConexoes
*/
inline void CObjetoRede_Sitio::Conectar ( CObjetoRede* obj, CObjetoRede* )
{
   this->conexao.push_back ( obj );
}

// -------------------------------------------------------------------------
// Função:       DeletarConeccao
// -------------------------------------------------------------------------
/** Deleta a conexao de um ramo morto
    @short  :		Deleta a conexao de um ramo morto
    @author :		André Duarte Bueno
    @see    :
    @param  : 	unsigned int link
    @return :		void
    NOTA: mesmo código de CObjetoGrafo_1VetorConexoes
*/
void CObjetoRede_Sitio::DeletarConeccao ( unsigned int link )
{
   this->conexao.erase ( conexao.begin() + link );
}

/** Marca e deleta as conexões para objetos invalidados (marcados para deleção).
 * Funciona assim: percorre os objetos das conexões,
 * se o rótulo do objeto corresponde a um rótulo válido (não deletado),
 * então a conexão é preservada.
 * Já os objetos que foram marcados para deleção são desconsiderados(deletados);
 * isto é, se a conexão foi deletada, aqui ela é desconsiderada (apagada).
    @short  : Deleta a conexao de um ramo morto
    @author : André Duarte Bueno
    @see    :
    @param  : unsigned int indiceObjetosDeletados
    @return : void
    @todo   : Pode-se otimizar o consumo de memória eliminando objetos deletados após resize.
    NOTA: mesmo código de CObjetoGrafo_1VetorConexoes
*/
bool CObjetoRede_Sitio::DeletarConeccoesInvalidadas ( unsigned int deletado )
{
   return DeletarConeccoesInvalidadas_aux ( deletado , conexao );
//   unsigned int indice_rotulo_valido {0};
//
//   // Percorre todas as coneccoes
//   for ( auto objeto: conexao )
//     // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
//     // Se foi deletado vai ser pulado.
//     if (objeto->rotulo != deletado)
//       {
//        conexao[indice_rotulo_valido++] = objeto;
//       }
//
//   // Redimensiona o vetor das coneccoes (as que apontam para objetos deletados são eliminadas)
//   conexao.resize (indice_rotulo_valido);
//   /// @todo: aqui pode apagar, usando erase, os objetos além do size().
//   return 1;
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
ostream& CObjetoRede_Sitio::Write ( ostream& out ) const
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
   out << ' ' << setw ( 4 ) << conexao.size ();

   // lista dos rótulos da conexao
   for ( auto objeto_conectado : conexao )
      out << ' ' << setw ( 4 ) << objeto_conectado->rotulo;

   return out;
}

// -------------------------------------------------------------------------
// Função:       operator<<
// -------------------------------------------------------------------------
/** Escreve os atributos do objeto em disco.
    @short  :	Sobrecarga operador entrada streams (antiga CObjetoRede_SitioEsqueleto).
    @author :	André Duarte Bueno
    @see    :
    @param  :	ostream& e CObjetoRede_Sitio&
    @return :	ostream&
*/
ostream& operator<< ( ostream& out, CObjetoRede_Sitio& s )
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
    @param  : istream& is, CObjetoRede_Sitio& s
    @return : istream&
    @todo   : implementar esta função.
*/

/*istream& operator>> (istream& is, CObjetoRede_Sitio& s)
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
    Agora o vetor conexão de cada sítio armazena direto o endereço dos sítios
    a quem esta conectado.
    Abaixo precisava do grafo para acessar os objetos
    somatorio_da_condutancia_vezes_x +=
        conexao[i]->propriedade * grafo->objeto[this->conexao[i]->rotulo]->x;

    Agora acessa os objetos diretamente, o rótulo do objeto esta sendo utilizado
    apenas para salvar o mesmo em disco.

    @author : André Duarte Bueno
    @see    :
    @param  : long double
    @return : long double
*/
long double CObjetoRede_Sitio::Go ( long double /*d */ )
{
   // Ex:
   // propriedade = condutancia
   // x           = pressao

   // Cria variáveis auxiliares (uma única vez, pois são estáticas)
   static long double somatorio_da_condutancia;
   static long double somatorio_da_condutancia_vezes_x;
   static long double condutancia;

   // Se não tem nenhum link, retorna x atual (a pressão atual)
   // tecnicamente nunca ocorre pois objetos sem conexão foram deletados!
   //   if (conexao.size() == 0)
   //     return x;

   // zera o somatório (a cada passagem por Go)
   somatorio_da_condutancia_vezes_x = somatorio_da_condutancia = 0.0;

//   // Percorre as conexões
//   // (calculo antigo, considera média -> calculo errado! certo é 1/ct = 1/c1 + 1/c2)
//   for ( auto objeto_conectado: conexao )
//     {
//       // faz uma média das condutâncias
//       condutancia = (objeto_conectado->propriedade + this->propriedade) / 2.0;
//       somatorio_da_condutancia_vezes_x += condutancia * objeto->x;
//
//       // Acumula a condutancia total
//       somatorio_da_condutancia += condutancia;
//     }

   // Percorre as conexões (calculo novo)
   // Para calcular condutancia total ct entre c1 e c2
   // 1/ct = 1/c1 + 1/c2
   // assim, ct = (c1*c2) / (c1+c2)
   // Mas devo considerar que c1 e c2 consideram como distância 1 pixel, de forma que ct
   // é a condutância entre 2 píxeis. Mas quero considerar 1 pixel;
   // Fazendo 1/c2p = 1/c1p + 1/c1p encontro c1p = 2*c2p
   // ou seja para converter condutância distância 2 píxeis em condutância distância 1 píxel,
   // devo multiplicar por 2
   // Ficando: ct_1p = 2* ct_2p; como ct_2p = ct = (c1*c2) / (c1+c2)
   // ct_1p = 2  (c1*c2) / (c1+c2)
   // sendo ct_1p a condutancia entre dois objetos, mas considerando distância 1 píxel,
   // o que corresponde ao grafo de conexão serial.
   for ( auto objeto_conectado : conexao ) {
         // calcula condutância entre objetos 1 e 2(metade distância): ct = 2(c1*c2) / (c1+c2)
         condutancia = 2.0 * ( objeto_conectado->propriedade * this->propriedade ) / ( objeto_conectado->propriedade + this->propriedade );
         somatorio_da_condutancia_vezes_x += condutancia * objeto_conectado->x;
         // Acumula a condutancia total
         somatorio_da_condutancia += condutancia;
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
long double CObjetoRede_Sitio::Fluxo () const
{
   long double fluxo { 0.0 };
   static long double condutancia;

   for ( unsigned long int i = 0; i < conexao.size (); i++ ) {
         // Ex: propriedade = condutancia    // x = pressao
         // Cálculo antigo considerava a média simples  -> calculo errado!
         // certo é 1/ct = 1/c1 + 1/c2) veja explicação em CObjetoRede_Sitio::Go.
         condutancia = 2.0 * ( conexao[i]->propriedade * this->propriedade ) / ( conexao[i]->propriedade + this->propriedade );
         fluxo += condutancia * ( this->x - conexao[i]->x );
      }

   return fluxo;
}
