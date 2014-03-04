/**
  ===============================================================================
  PROJETO:          Biblioteca LIB_LDSC
  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
  ===============================================================================
  Desenvolvido por:
                     Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
  @author:           André Duarte Bueno
  @file:             CObjetoGrafo_1VetorConexoes.cpp
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
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo_1VetorConexoes.h>
using namespace std;

// -------------------------------------------------------------------------
// Função:               Conectar
// -------------------------------------------------------------------------
/** Recebe um objA e o conecta a this.
    @author :	André Duarte Bueno
    @see    :
    @param  : objeto a quem será conectado
    @return : void
*/
void CObjetoGrafo_1VetorConexoes::Conectar ( CObjetoGrafo * obj, CObjetoGrafo * )
{
     conexao.push_back ( obj );
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
*/
void CObjetoGrafo_1VetorConexoes::DeletarConeccao ( unsigned int link )
{
     this->conexao.erase ( conexao.begin() + link );
}

/** Marca e deleta as conexões para objetos invalidados (marcados para deleção).
 * Funciona assim: percorre os objetos das conexões,
 * se o rótulo do objeto corresponde a um rótulo válido (não deletado), então a conexão é preservada.
 * Já os objetos que foram marcados para deleção são desconsiderados(deletados);
 *    @short  : Deleta a conexao de um ramo morto
 *    @author : André Duarte Bueno
 *    @see    :
 *    @param  : unsigned int indiceObjetosDeletados
 *    @return : void
 *    @todo   : Pode-se otimizar o consumo de memória eliminando objetos deletados após resize.
*/
bool CObjetoGrafo_1VetorConexoes::DeletarConeccoesInvalidadas ( unsigned int deletado )
{
 return DeletarConeccoesInvalidadas_aux ( deletado , conexao );
//      unsigned int indice_rotulo_valido {0};
// 
//      // Percorre todas as coneccoes
//      for ( auto objeto: conexao )
//           // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
//           // Se foi deletado vai ser pulado.
//           if ( objeto->rotulo != deletado ) {
//                conexao[indice_rotulo_valido++] = objeto;
//           }
// 
//      // Redimensiona o vetor das coneccoes (as que apontam para objetos deletados são eliminadas)
//      conexao.resize ( indice_rotulo_valido );
//      /// @todo: aqui pode apagar, usando erase, os objetos além do size().
//      return 1;
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
ostream & CObjetoGrafo_1VetorConexoes::Write ( ostream & out ) const
{
     out.setf ( ios::right );

     // Tipo de contorno
     out << setw ( 4 ) << static_cast<unsigned char> ( Contorno() ) << '\n';

     // Rótulo de this
     out << ' ' << setw ( 5 ) << rotulo;

     // Número de links (coneccoes)
     unsigned long int numeroLinks = conexao.size ();
     out << ' ' << setw ( 4 ) << numeroLinks;

     // lista dos rótulos de quem estou conexo
     for ( auto objeto_conectado : conexao )
          out << ' ' << setw ( 5 ) << objeto_conectado->rotulo;

     return out;
}

// -------------------------------------------------------------------------
// Função:       operator<<
// -------------------------------------------------------------------------
/** Escreve os atributos do objeto em disco.
    @short  :	Sobrecarga operador entrada streams (antiga CObjetoGrafo_1VetorConexoesEsqueleto).
    @author :	André Duarte Bueno
    @see    :
    @param  :	ostream& e CObjetoGrafo_1VetorConexoes&
    @return :	ostream&
*/
ostream & operator<< ( ostream & out, CObjetoGrafo_1VetorConexoes & s )
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
    @param  : istream& is, CObjetoGrafo_1VetorConexoes& s
    @return : istream&
    @todo   : implementar esta função.
*/