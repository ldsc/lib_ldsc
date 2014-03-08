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
#include <EstruturaDados/ObjetoGrafo/CObjetoGrafo_1VetorConexoes.h>
using namespace std;

// -------------------------------------------------------------------------
// Função:               Conectar
// -------------------------------------------------------------------------
/** Recebe um objA e o conecta a this.
    @author : André Duarte Bueno
    @see    :
    @param  : objeto a quem será conectado
    @return : void
*/
void CObjetoGrafo_1VetorConexoes::Conectar ( CObjetoGrafo * obj, CObjetoGrafo * )
{
     conexao.push_back ( obj );
}

// -------------------------------------------------------------------------
// Função:       DeletarConexao
// -------------------------------------------------------------------------
/** Deleta a conexao de um ramo morto
    @short  :		Deleta a conexao de um ramo morto
    @author :		André Duarte Bueno
    @see    :
    @param  : 	unsigned int pos
    @return :		void
*/
void CObjetoGrafo_1VetorConexoes::DeletarConexao ( unsigned int pos )
{
     this->conexao.erase ( conexao.begin() + pos );
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
*/
bool CObjetoGrafo_1VetorConexoes::DeletarConexoesInvalidadas ( unsigned int deletado )
{
 return DeletarConexoesInvalidadas_aux ( deletado , conexao );
}

// -------------------------------------------------------------------------
// Função:       Write
// -------------------------------------------------------------------------
/** @short  : Salva dados do objeto sítio em novo formato (André).
    @author :	André Duarte Bueno
    @see    :
    @param  :   Recebe uma referencia para uma ostream
    @return :	ostream& 
*/
ostream & CObjetoGrafo_1VetorConexoes::Write ( ostream & out ) const
{ 
     out.setf ( ios::right );

     // Tipo de contorno
     out << setw ( 5 ) << static_cast<uint8_t> ( Tipo() ) << '\n';

     // Rótulo de this
     out << ' ' << setw ( 5 ) << rotulo;

     // Número de links (conexões)
     unsigned long int numeroLinks = conexao.size ();
     out << ' ' << setw ( 5 ) << numeroLinks;

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

