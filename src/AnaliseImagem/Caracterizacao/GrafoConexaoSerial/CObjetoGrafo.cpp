/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================
Desenvolvido por:
			  Laboratorio de Desenvolvimento de Software Cientifico [LDSC].
@author:      André Duarte Bueno
@file:        CObjetoGrafo.cpp
@begin:       Sat Sep 16 2000
@copyright:   (C) 2000 by André Duarte Bueno
@email:       andreduartebueno@gmail.com
*/
// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo.h>

/** Marca e deleta as conexões para objetos invalidados (marcados para deleção).
 * Funciona assim: percorre os objetos das conexões,
 * se o rótulo do objeto corresponde a um rótulo válido (não deletado), então a conexão é preservada.
 * Já os objetos que foram marcados para deleção são desconsiderados(deletados);
 *    @short  : Deleta a conexao de um ramo morto
 *    @author : André Duarte Bueno
 *    @see    :
 *    @param  : os objetos marcados para deleção tem rótulo = deletado
 *    @param  : o vetor das conexões.
 *    @return : bool
*/
bool CObjetoGrafo::DeletarConexoesInvalidadas_aux ( unsigned int deletado , vector<CObjetoGrafo*>& conexao )
{
     unsigned int indice_rotulo_valido {0};

     // Percorre todas as conexões
     for ( auto objeto: conexao )
          // Se o objeto para quem aponta não foi deletado, armazena no vetor das conexões.
          // Se foi deletado vai ser pulado.
          if ( objeto->rotulo != deletado ) {
               conexao[indice_rotulo_valido++] = objeto;
          }

     // Redimensiona o vetor das conexões (as que apontam para objetos deletados são eliminadas)
     conexao.resize ( indice_rotulo_valido );
     /// @todo: aqui pode apagar, usando erase, os objetos além do size(). Otimiza memória.
     return 1;
}

/**
-------------------------------------------------------------------------
Função:    operator<<
-------------------------------------------------------------------------
@short  : Escreve em os as propriedades do objeto
@author : André Duarte Bueno
@see    :
@param  : const CContorno &
@return : ostream&
*/
ostream &operator<< ( ostream &out, const CObjetoGrafo &obj )
{
     obj.Write(out);
     return out;
}

/**
-------------------------------------------------------------------------
Função:    operator>>
-------------------------------------------------------------------------
@short  : Lê as propriedades do objeto
@author : André Duarte Bueno
@see    :
@param  : CContorno &
@return : ostream&
@todo   : Note que esta armazenando o tipo de contorno em x; corrigir/analizar!
*/
istream  &operator>> ( istream &is,  CObjetoGrafo &obj )
{
     unsigned char contorno; ///@bug: corrigir!
     is >> contorno;
     is >> obj.rotulo;
//   obj.Read(in);
     return is;
}
