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

// -------------------------------------------------------------------------
// Função:       Write
// -------------------------------------------------------------------------
/** Salva dados do objeto sítio em novo formato.
    @short  : Salva dados do objeto sítio em novo formato.
    Formato Andre :
    ----------------------------
    NumeroSitios  		// salvo pelo grafo
    TipoContorno        // tipo de contorno
    Rotulo              // rotulo objeto
    propriedade 		// ex: raio hidraulico ou condutancia do objeto
    x           		// ex: pressão
    NumeroConeccoes     // quantos links
    Lista_dos_rotulos_das_coneccoes // rótulos dos links

    @author :	André Duarte Bueno
    @see    :
    @param  :   Recebe uma referencia para uma ostream
    @return :	ostream&
*/
ostream & CObjetoGrafo::Write ( ostream & out ) const
{
     out.setf ( ios::right );

     // Tipo de contorno
     out << setw ( 4 ) << static_cast<unsigned char> ( Contorno() ) << '\n';

     // Rótulo de this
     out << ' ' << setw ( 5 ) << rotulo;

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
bool CObjetoGrafo::DeletarConeccoesInvalidadas_aux ( unsigned int deletado , vector<CObjetoGrafo*>& coneccao )
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
