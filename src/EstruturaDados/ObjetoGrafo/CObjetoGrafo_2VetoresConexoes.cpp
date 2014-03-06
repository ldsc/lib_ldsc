/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por: Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:          André Duarte Bueno
@file:             CObjetoGrafo_2VetoresConexoes.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>
#include <iomanip>
using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <EstruturaDados/ObjetoGrafo/CObjetoGrafo_2VetoresConexoes.h>

/**
-------------------------------------------------------------------------
Função:
-------------------------------------------------------------------------
@short  : Adiciona os dois ponteiros recebidos objA e objB aos vetores de this.
@author : André Duarte Bueno
@see    :
@param  : CObjetoGrafo * objA, CObjetoGrafo * objB
@return : void
*/
void CObjetoGrafo_2VetoresConexoes::Conectar ( CObjetoGrafo* objA, CObjetoGrafo* objB )
{
   this->conexaoA.push_back ( objA );
   this->conexaoB.push_back ( objB );
}

// -------------------------------------------------------------------------
// Função:       DeletarConexao
// -------------------------------------------------------------------------
/** Deleta a conexao de um ramo morto
    @short  :		Deleta a conexao de um ramo morto
    @author :		André Duarte Bueno
    @see    :
    @param  : 	unsigned int link
    @return :		void
*/
void CObjetoGrafo_2VetoresConexoes::DeletarConexao ( unsigned int link )
{
   this->conexaoA.erase ( conexaoA.begin() + link );
   this->conexaoB.erase ( conexaoB.begin() + link );
}

/** Marca e deleta as conexões para objetos invalidados (marcados para deleção).
 * Funciona assim: percorre os objetos das conexões,
 * se o rótulo do objeto correspond	e a rótulo válido (não deletado),
 * então a conexão é preservada.
 * Já os objetos que foram marcados para deleção são desconsiderados(deletados);
 * isto é, se a conexão foi deletada, aqui ela é desconsiderada (apagada).
    @short  : Deleta a conexao de um ramo morto
    @author : André Duarte Bueno
    @see    :
    @param  : unsigned int link
    @return : void
*/
bool CObjetoGrafo_2VetoresConexoes::DeletarConexoesInvalidadas ( unsigned int deletado )
{
   return DeletarConexoesInvalidadas_aux ( deletado , conexaoA ) &&
          DeletarConexoesInvalidadas_aux ( deletado , conexaoB );
}

/**
-------------------------------------------------------------------------
Função:     Write
-------------------------------------------------------------------------
@short  : Escreve propriedades do objeto em out
@author : André Duarte Bueno
@see    :
@param  : ofstream& out
@return : ostream&
*/
ostream& CObjetoGrafo_2VetoresConexoes::Write ( ostream& out ) const
{
   out.setf ( ios::right );
   // Tipo de contorno
   out << setw ( 5 ) << static_cast<uint8_t> ( Tipo() ) << '\n';

   // Rótulo de this
   out << ' ' << setw ( 5 ) << rotulo;

   // Número de conexões do sítio
   out << ' ' << setw ( 5 ) << conexaoA.size ();

   // CONECCAO A: lista dos rótulos
   for ( auto objeto : conexaoA )
      out << ' ' << setw ( 5 ) << objeto->rotulo;

   // CONECCAO B: lista dos rótulos
   for ( auto objeto : conexaoB )
      out << ' ' << setw ( 5 ) << objeto->rotulo;

   return out;
}

/**
-------------------------------------------------------------------------
Função:  operator<<
-------------------------------------------------------------------------
@short  : Escreve propriedades do objeto em fout
@author : André Duarte Bueno
@see    :
@param  : ostream& fout, CObjetoGrafo_2VetoresConexoes& s
@return : ostream&
*/
ostream& operator<< ( ostream& fout, CObjetoGrafo_2VetoresConexoes& s )
{
   s.Write ( fout );
   return fout;
}

/*
-------------------------------------------------------------------------
Função:    operator>>
-------------------------------------------------------------------------
@short  : Lê as propriedades do objeto a partir de arquivo os
					(normalmente de disco)
@author :	André Duarte Bueno
@see    :
@param  :
@return :
*/
/*istream& operator>> (istream& is, CObjetoGrafo_2VetoresConexoes& s)
{
  s.Read(is);
  return is;
}
*/
