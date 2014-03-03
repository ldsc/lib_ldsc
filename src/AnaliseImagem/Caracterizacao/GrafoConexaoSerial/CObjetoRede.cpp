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
#include <fstream>
#include <iomanip>

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoRede.h>
using namespace std;


// -------------------------------------------------------------------------
// Função:       Write
// -------------------------------------------------------------------------
/** @short  : Salva dados do objeto
    @author :	André Duarte Bueno
    @see    :
    @param  :   Recebe uma referencia para uma ostream
    @return :	ostream&
*/
ostream & CObjetoRede::Write (ostream & out) const
{
  out.setf (ios::right);

  // Tipo de contorno
  out << setw (4) << static_cast<unsigned char>( Contorno() ) << '\n';

  // Rótulo de this
  out << ' ' << setw (5) << rotulo;

  // x de this (pressão)
  out << ' ' << setw (10) << x;

  // propriedade de this (condutancia)
  out << ' ' << setw (10) << propriedade;
  
  return out;
}


/** Marca e deleta as conecções para objetos invalidados (marcados para deleção).
 * Funciona assim: percorre os objetos das conecções,
 * se o rótulo do objeto corresponde a um rótulo válido (não deletado), então a conexão é preservada.
 * Já os objetos que foram marcados para deleção são desconsiderados(deletados);
 *    @short  : Deleta a coneccao de um ramo morto
 *    @author : André Duarte Bueno
 *    @see    :
 *    @param  : unsigned int indiceObjetosDeletados
 *    @return : void
 *    @todo   : Pode-se otimizar o consumo de memória eliminando objetos deletados após resize.
*/
bool CObjetoRede::DeletarConeccoesInvalidadas_aux ( int deletado , vector<CObjetoRede*>& coneccao )
{
     unsigned int indice_rotulo_valido {0};

     // Percorre todas as coneccoes
     for ( auto objeto: coneccao )
          // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
          // Se foi deletado vai ser pulado.
          if ( objeto->rotulo != deletado ) {
               coneccao[indice_rotulo_valido++] = objeto;
          }

     // Redimensiona o vetor das coneccoes (as que apontam para objetos deletados são eliminadas)
     coneccao.resize ( indice_rotulo_valido );
     /// @todo: aqui pode apagar, usando erase, os objetos além do size().
     return 1;
}

// -------------------------------------------------------------------------
// Função:       operator<<
// -------------------------------------------------------------------------
/** Escreve os atributos do objeto em disco.
    @short  :	Sobrecarga operador entrada streams (antiga CObjetoRedeEsqueleto).
    @author :	André Duarte Bueno
    @see    :	
    @param  :	ostream& e CObjetoRede&
    @return :	ostream&
*/
ostream & operator<< (ostream & out, CObjetoRede & s)
{
  s.Write(out);
  return out;
}

// -------------------------------------------------------------------------
// Função:    operator>>
// -------------------------------------------------------------------------
/** Sobrecarga operador entrada streams
    @short  :	Sobrecarga operador entrada streams
    @author :	André Duarte Bueno
    @see    :
    @param  : istream& is, CObjetoRede& s
    @return : istream&
    @todo   : implementar esta função.
*/

/*istream& operator>> (istream& is, CObjetoRede& s)
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

