/**
===============================================================================
PROJETO:          Biblioteca LIB_LDSC
                  Ramo: AnaliseImagem/Caracterizacao/GrafoConexaoSerial
===============================================================================

Desenvolvido por:Laboratorio de Desenvolvimento de Software Cientifico
			[LDSC].
@author:          André Duarte Bueno
@file:             CObjetoGrafo_MatrizConexoes.cpp
@begin:            Sat Sep 16 2000
@copyright:        (C) 2000 by André Duarte Bueno
@email:            andreduartebueno@gmail.com
*/

// -----------------------------------------------------------------------
// Bibliotecas C/C++
// -----------------------------------------------------------------------
#include <iomanip>
using namespace std;

// -----------------------------------------------------------------------
// Bibliotecas LIB_LDSC
// -----------------------------------------------------------------------
#include <AnaliseImagem/Caracterizacao/GrafoConexaoSerial/CObjetoGrafo_MatrizConexoes.h>

// -------------------------------------------------------------------------
// Função:       Conectar
// -------------------------------------------------------------------------
/** Função : Conectar
 * @see    : Adiciona ao vetor conexão o vetor passado
 * @author:  André Duarte Bueno
 * @param  : recebe um vetor de  CObjetoGrafo*
 * @return : void
 * @test   : testar!
*/
void CObjetoGrafo_MatrizConexoes::Conectar ( vector < CObjetoGrafo* >obj_vetor )
{
   mconexao.push_back ( obj_vetor );
}

// -------------------------------------------------------------------------
// Função:       DeletarConexao
// -------------------------------------------------------------------------
/** Deleta a conexao de um ramo morto, note que esta deletando um vetor inteiro.
    @short  : Deleta a conexao de um ramo morto
    @author : André Duarte Bueno
    @see    :
    @param  : unsigned int link
    @return : void
    @test   : testar!
*/
void CObjetoGrafo_MatrizConexoes::DeletarConexao ( unsigned int ivetor )
{
   this->mconexao.erase ( mconexao.begin() + ivetor );
}

void CObjetoGrafo_MatrizConexoes::DeletarConexao ( unsigned int ivetor, unsigned int link )
{
   mconexao[ivetor].erase ( mconexao[ivetor].begin() + link );
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
    @todo   : Pode-se otimizar o consumo de memória eliminando objetos deletados após resize.
    @test   : testar!
*/
bool CObjetoGrafo_MatrizConexoes::DeletarConexoesInvalidadas ( unsigned int deletado )
{
   unsigned int indice_rotulo_valido {0};

   // Percorre todas as conexões e chama DeletarConexoesInvalidadas_aux
   for ( auto vobjetos_conectados : mconexao )
      DeletarConexoesInvalidadas_aux ( deletado , vobjetos_conectados );

   return 1;
}

// ------------------------------------------------------------------------------
// Funcao:     Write
// ------------------------------------------------------------------------------
/** Salva o grafo em disco (em fout)
@see  		:
@author:    André Duarte Bueno
@param		: Recebe uma ofstream& fout
@return   : ostream&
@test   : testar!
*/
ostream& CObjetoGrafo_MatrizConexoes::Write ( ostream& out ) const
{
   // Tipo de contorno
   out << setw ( 5 ) << static_cast<uint8_t> ( Contorno() ) << '\n';

   // Rótulo de this
   out << ' ' << setw ( 5 ) << rotulo;

   // Número de vetores conexões
   out << ' ' << setw ( 5 ) << mconexao.size ();

   // Para cada vetor percorrer a lista
   for ( unsigned long int cont_conexoes = 0; cont_conexoes < mconexao.size (); cont_conexoes++ ) {
         // Para cada vetor percorrer os objetos
         // mconexao[cont_vector] retorna um ponteiro para um vetor
         out << " " << setw ( 5 ) << mconexao[cont_conexoes].size ();

         // lista dos rótulos
         for ( auto objeto_conectado :  mconexao[cont_conexoes] )
            out << " " << setw ( 5 ) << objeto_conectado->rotulo;
      }

   return out;
}

// ------------------------------------------------------------------------------
// Funcao:     operator<<
// ------------------------------------------------------------------------------
ostream& operator<< ( ostream& out, CObjetoGrafo_MatrizConexoes& obj )
{
   obj.Write ( out );
   return out;
}
